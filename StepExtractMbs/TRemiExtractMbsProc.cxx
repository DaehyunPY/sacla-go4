//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the main processing file for the ExtractMBS step
//----------------------------------------------------------------
#include "TRemiExtractMbsProc.h"

#include "../EventElement/TRemiExtractEvent.h"

#include "TGo4WinCond.h"
#include "TGo4MbsEvent.h"

#include "TClass.h"
#include "TH1.h"

#include <algorithm>
#include <cstdlib>

/**
 * \brief Default constructor. Not usable!
 */
TRemiExtractMbsProc::TRemiExtractMbsProc ()
: TGo4EventProcessor()
{
}

/**
 * \brief Constructor to be used
 */
TRemiExtractMbsProc::TRemiExtractMbsProc(const char* name)
: TGo4EventProcessor(name), TRemiIncludeFunctions(MBSCONFIG),
fRandom(0)
{
	//------------------------------------------//
	// Read information from configuration file //
	//------------------------------------------//

	mNumAdcChan = fConfigFile->read ("num_adc_chan", 16);
	mNumTdcChan = fConfigFile->read ("num_tdc_chan", 16);
	mNumTdcHits = fConfigFile->read ("num_tdc_hits", 10);
	
	mTdcGeoAddress = fConfigFile->read ("tdc_geo_address", 0);
	mAdcGeoAddress = fConfigFile->read ("adc_geo_address", 2);
	
	Double_t hist_raw_tdc_min = fConfigFile->read ("ext_hist_raw_tdc_min", 1);
	Double_t hist_raw_tdc_max = fConfigFile->read ("ext_hist_raw_tdc_max", 5000);
	Int_t hist_raw_tdc_bins = fConfigFile->read ("ext_hist_raw_tdc_bins", 1000);

	Double_t hist_raw_adc_min = fConfigFile->read ("ext_hist_raw_adc_min", 1);
	Double_t hist_raw_adc_max = fConfigFile->read ("ext_hist_raw_adc_max", 1<<21);
	Int_t hist_raw_adc_bins = fConfigFile->read ("ext_hist_raw_adc_bins", 1<<12);

	TString condname_tstart = fConfigFile->read<std::string> ("ext_condname_tdc", "c_tpulser");

	mUseRandomizer = fConfigFile->read("ext_use_randomization", true);

	//-------------------//
	// Create histograms //
	//-------------------//

	TString hname, htitle, folder, basefolder = "raw";

	fADCraw = std::vector<TH1I*> (mNumAdcChan);
	folder = basefolder + "/ADC";
	for (UInt_t i=0; i<mNumAdcChan; i++)
	{
		hname = "Adc_"; hname += i;
		htitle = "ADC channel "; htitle += i;
		fADCraw[i] = MakeH1<TH1I> (hname, htitle, hist_raw_adc_bins, hist_raw_adc_min, hist_raw_adc_max, folder);
	}

	fTDCraw = std::vector< std::vector<TH1I*> > (mNumTdcChan);
	fTDCrawAll = std::vector< TH1I*> (mNumTdcChan);
	fTDChits = std::vector<TH1I*> (mNumTdcChan);
	for (UInt_t i=0; i<mNumTdcChan; i++)
		fTDCraw[i] = std::vector<TH1I*> (mNumTdcHits);
	for (UInt_t i=0; i<mNumTdcChan; i++)
	{
		folder.Form(basefolder + "/TDC/Channel%i", i);
		
		hname = "TdcChan"; hname += i;
		htitle = "All hits of TDC Channel "; htitle += i;
		fTDCrawAll[i] = MakeH1<TH1I> (hname, htitle, hist_raw_tdc_bins, hist_raw_tdc_min, hist_raw_tdc_max, folder);
		
		hname = "TdcChanHits"; hname += i;
		htitle = "Number of hits per event for TDC Channel "; htitle += i;
		fTDChits[i] = MakeH1<TH1I> (hname, htitle, "ext_hist_tdchits_bins", "ext_hist_tdchits_min", "ext_hist_tdchits_max", folder);
		
		folder += "/Hits";
		
		for (UInt_t j=0; j<mNumTdcHits; j++)
		{
			hname.Form("Tdc_%i_%i", i, j+1);
			htitle.Form("Tdc channel %i hit %i", i, j+1);
			fTDCraw[i][j] = MakeH1<TH1I> (hname, htitle, "ext_hist_tdchits_bins", "ext_hist_tdchits_min", "ext_hist_tdchits_max", folder);
		}
	}



	// Enable rescaling of Fill 'n' Go histograms
	EnableRescaleFillToHist();
	
}

/**
 * \brief Destructor
 */
TRemiExtractMbsProc::~TRemiExtractMbsProc ()
{
}

/**
* \brief Check Event Class
*/
Bool_t TRemiExtractMbsProc::CheckEventClass(TClass* ul)
{
	return ul->InheritsFrom(TRemiExtractEvent::Class());
}


/**
 * \brief Main processing function: Extract the raw data from the MBS event
 *
 * NOTE: This is not universal but the matched counterpart of the f_user.c program running on the MBS stream servers for reaction microscope data acquisition
 */
Bool_t TRemiExtractMbsProc::BuildEvent(TGo4EventElement* dest)
{

	fInput = dynamic_cast<TGo4MbsEvent*> (GetInputEvent());
	
	fOutput = dynamic_cast<TRemiExtractEvent*> (dest);
	fOutput->CheckSizes(mNumAdcChan, mNumTdcChan);
	fOutput->Clear();
	
	Int_t value=0;
	UInt_t	lwords, i, j, channel=0;
	Int_t data;
	
	// ****** VME-Module declarations ******
	UInt_t adc_c=0,tdc_c=0;
	Int_t *TDCEvent, *ADCEvent;
	Int_t GEOaddress;
	UInt_t TDCEventMax = mNumTdcHits*mNumTdcChan;
	TDCEvent = (Int_t*) malloc (TDCEventMax*sizeof(Int_t));
	UInt_t ADCEventMax = mNumAdcChan+4;
	ADCEvent = (Int_t*) malloc (ADCEventMax*sizeof(Int_t));

	for (i=0; i<mNumAdcChan; i++)
		fOutput->fiADC[i] = -1;

	if(fInput == 0)
	{
		std::cout << "**** TRemiExtractMbsProc: no input event !"<< std::endl;
		free (TDCEvent);
		free (ADCEvent);
		return 0;
	}
	
	fInput->ResetIterator();
	
	

	while((fSubevt = fInput->NextSubEvent()) != 0) // loop over subevents
	{
		// get number of 32bit words in the data field
		lwords = fSubevt->GetIntLen();

		if(fSubevt->GetSubcrate() == 0)
		{
			adc_c=0;
			tdc_c=0;
			i = 0;
			
			for(i=0; (i<lwords) && (tdc_c<TDCEventMax) && (adc_c<ADCEventMax); i++) // loop over words
			{
				data = fSubevt->Data(i); // Get data for current position in the data field
				GEOaddress = data >> 26; // Extract GEO address from current data
				if (GEOaddress == mTdcGeoAddress)
				{
					TDCEvent[tdc_c] = data;
					tdc_c++;
				}
				else if (GEOaddress == mAdcGeoAddress)
				{
					ADCEvent[adc_c] = data;
					adc_c++;
				}
				else
				{
					std::cout << "no valid data " << std::endl;
				} 
			}
		} // if(fSubevt->GetSubcrate() == 0)
	} // while((fSubevt = fInput->NextSubEvent()) != 0)

	// Decode ADC data
	for(i = 0; i<adc_c; i++)
	{
		value	=  ADCEvent[i]&0xfff;   		// first 12 bit are data
		channel = (ADCEvent[i]>>17)&0xf; 		// these 5 bits are channel number

		if( ADCEvent[i]&0x00001000 ) 	value = 0xffffffff; // ADC overflow p. 41

		// ADC values are written into the fOutput object
		if (channel < mNumAdcChan)
			fOutput->fiADC[channel] = static_cast<Float_t>(value) + ( mUseRandomizer ? static_cast<Float_t>(fRandom.Rndm() - .5) : 0. );
	}

	// Fill ADC histograms
	for (i=0; i<mNumAdcChan;i++)
	{
		fADCraw[i]->Fill(fOutput->fiADC[i]); // Fill ADC histograms
	}

	/***** Sorting of TDC Multihit-Data**********/

	for(i = 0; i<tdc_c; i++)
	{
		value	=  TDCEvent[i]&0x1fffff;   		// first 21 bit are data
		channel = (TDCEvent[i]>>21)&0x1f; 		// next 5 bit are channelnumber

		if((channel < mNumTdcChan) && (value < (1<<21)) && (value >= 0) )
		{
			fOutput->AddTdcHit(channel, static_cast<Float_t>(value) + ( mUseRandomizer ? static_cast<Float_t>(fRandom.Rndm() - .5) : 0.) );
		}
	} // for(i = 0; i<tdc_c; i++)

	// Arrange TDC hits in increasing order
	for(i=0; i<mNumTdcChan; i++)
	{
		std::sort (fOutput->fiTDC[i].begin(), fOutput->fiTDC[i].end());
	}

	// Fill TDC histograms
	for (i=0; i<mNumTdcChan; i++)
	{
		for (j=0; (j<fOutput->fiTDC[i].size()) && (j<mNumTdcHits); j++)
		{
			fTDCraw[i][j]->Fill(fOutput->fiTDC[i][j]);
			fTDCrawAll[i]->Fill(fOutput->fiTDC[i][j]);
		}
		fTDChits[i]->Fill(fOutput->fiTDC[i].size());
	}
	/***** END Sorting of TDC Multihit-Data END ******/

	free (TDCEvent);
	free (ADCEvent);
	
	// Disable rescaling of Fill 'n' Go histograms
	DisableRescaleFillToHist();
	
	return 1;
}

