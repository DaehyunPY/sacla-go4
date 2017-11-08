//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class provides TDC and ADC raw data arrays from Acqiris data file
//----------------------------------------------------------------

#include "TRemiAcqirisEventSource.h"

#include "TRemiExtractAcqProc.h"
#include "Parameters/TRemiAcqirisSingleShotParameter.h"
#include "../Core/TRemiIncludeFunctions.h"
#include "../EventElement/TRemiExtractEvent.h"
#include "TRemiAcqirisChannel.h"
#include "TRemiAcqirisPeak.h"
#include "TRemiAcqirisAdcPulse.h"
#include "TRemiAcqirisArchive.h"
#include "TRemiAcqirisEvent.h"
#include "TRemiAcqirisSoftTdc.h"
#include "TRemiAcqirisSoftAdc.h"

#include "TGo4EventSource.h"
#include "TGo4UserSourceParameter.h"
#include "TGo4WinCond.h"

#include "TH1.h"
#include "TH2.h"
#include "TClass.h"
#include "TROOT.h"
#include "Riostream.h"
#include "Rtypes.h"

#include <algorithm>
#include <vector>


/**
* \brief Default constructor. Not to be used
*/
TRemiAcqirisEventSource::TRemiAcqirisEventSource() :
TGo4EventSource("no event source"),
			 TRemiIncludeFunctions(ACQIRISCONFIG)
{
	InitGeneral();
}

/**
* \brief Constructor with text arguments
*/
TRemiAcqirisEventSource::TRemiAcqirisEventSource(const char* name, const char* args, Int_t port) :
	TGo4EventSource(name),
	TRemiIncludeFunctions(ACQIRISCONFIG)
{
	InitGeneral();
}

/**
* \brief Constructor invoked with TGo4UserSourceParameter object
* 
* This is the principal constructor called by the GUI framework
*/
TRemiAcqirisEventSource::TRemiAcqirisEventSource(TGo4UserSourceParameter* ParUserSource) :
	TGo4EventSource(ParUserSource->GetName()),
	TRemiIncludeFunctions(ACQIRISCONFIG)
{
	InitGeneral();
}

/**
* \brief Destructor
*/
TRemiAcqirisEventSource::~TRemiAcqirisEventSource()
{
}

/** \brief Initialise Acqiris Event Source */
void TRemiAcqirisEventSource::InitGeneral()
{
	std::cout << "**** TRemiAcqirisEventSource: Initialise" << std::endl;
	UInt_t i,j;
	TString fieldname;
	
	// First of all load configuration
	mNumAdcChan = fConfigFile->read ("num_adc_chan", 16);
	mNumTdcChan = fConfigFile->read ("num_tdc_chan", 16);
	mNumTdcHits = fConfigFile->read ("num_tdc_hits", 10);
	mNumAcqChan = fConfigFile->read ("num_acq_chan", 0);
	
	mAdcChanEventId = fConfigFile->read ("acq_adc_chan_eventid", -1);
	mAdcChanMbIndex = fConfigFile->read ("acq_adc_chan_mbindex", -1);
	mAdcChanOffset = fConfigFile->read ("acq_adc_chan_offset", -1);
	mAdcChanBunchId = fConfigFile->read ("acq_adc_chan_bunchid", -1);
	mAdcChanPpDelay = fConfigFile->read ("acq_adc_chan_ppdelay", -1);
	mAdcChanStreakCam = fConfigFile->read ("acq_adc_chan_streakcam", -1);
	mAdcChanCrossCorr = fConfigFile->read ("acq_adc_chan_crosscorr", -1);
	mAdcChanBam = fConfigFile->read ("acq_adc_chan_bam", -1);
	mAdcChanStreakCamFast = fConfigFile->read ("acq_adc_chan_streakcamfast", -1);
	mAdcChanPpDelayEnc = fConfigFile->read ("acq_adc_chan_ppdelay_enc", -1);
	
	mUseTdcVeto = std::vector<Bool_t> (mNumTdcChan);
	mTdcSourceChannel = std::vector<Int_t> (mNumTdcChan);
	mTdcSourcePolarity = std::vector<Short_t> (mNumTdcChan);
	for (i=0; i<mNumTdcChan; i++)
	{
		fieldname.Form("tdc_chan_%i_use_veto", i);
		mUseTdcVeto[i] = fConfigFile->read(fieldname.Data(), false);
		fieldname.Form("tdc_chan_%i_acq_channel", i);
		mTdcSourceChannel[i] = fConfigFile->read(fieldname.Data(), 0) - 1;
		fieldname.Form("tdc_chan_%i_acq_polarity", i);
		mTdcSourcePolarity[i] = fConfigFile->read(fieldname.Data(), 0);
	}
	
	mAdcSourceChannel = std::vector<Int_t> (mNumAdcChan);
	mAdcChannelPerMinibunch = std::vector<Bool_t> (mNumAdcChan);
	mAdcChannelWhichPulse = std::vector<Short_t> (mNumAdcChan);
	for (i=0; i<mNumAdcChan; i++)
	{
		fieldname.Form("adc_chan_%i_acq_channel", i);
		mAdcSourceChannel[i] = fConfigFile->read(fieldname.Data(), 0) - 1;
		fieldname.Form("adc_chan_%i_per_minibunch", i);
		mAdcChannelPerMinibunch[i] = fConfigFile->read(fieldname.Data(), false);
		fieldname.Form("adc_chan_%i_which_pulse", i);
		mAdcChannelWhichPulse[i] = fConfigFile->read(fieldname.Data(), 0);
	}
	
	// Intialise other variables
	mMinibunchIndex = 0;
	
	fParaSingleShot = MakeParam<TRemiAcqirisSingleShotParameter>("Par SingleShot", "Acq");
	
	fPeakStart = TRemiAcqirisPeak ();
	fPeakEnd = TRemiAcqirisPeak ();
	fAdcStart = TRemiAcqirisAdcPulse ();
	fAdcEnd = TRemiAcqirisAdcPulse ();
	
	fAcqirisEvent.init(std::max(mNumAdcChan, mNumTdcChan), fConfigFile);
	
	//-------------------//
	// Create histograms //
	//-------------------//
	
	TString basefolder = "Acq";
	TString folder, hname, htitle;
	
	Double_t hist_tdc_min = fConfigFile->read("acq_hist_tdc_min", 0.);
	Double_t hist_tdc_max = fConfigFile->read("acq_hist_tdc_max", 100000.);
	Int_t hist_tdc_bins = fConfigFile->read("acq_hist_tdc_bins", 1000);
	
	Double_t hist_adc_min = fConfigFile->read("acq_hist_adc_min", 0.);
	Double_t hist_adc_max = fConfigFile->read("acq_hist_adc_max", 100000.);
	Int_t hist_adc_bins = fConfigFile->read("acq_hist_adc_bins", 1000);
	
	Int_t hist_minibunch_max = fConfigFile->read("acq_hist_minibunch_max", 20);
	Int_t hist_tdchits_mb_max = fConfigFile->read("acq_hist_tdchits_mb_max", 20);
	
	fHistTdcMinibunch = std::vector<TH1I*> (mNumTdcChan);
	fHistTdcMbHits = std::vector<TH2I*> (mNumTdcChan);
	fHistTdcAll = std::vector<TH1I*> (mNumTdcChan);
	fHistTdcHits = std::vector< std::vector<TH1I*> > (mNumTdcChan);
	
	folder = basefolder + "/TDC";
	hname = "TdcChanHitCount";
	htitle = "Hit Counter for TDC Channels";
	fHistTdcChanHitCount = MakeH1<TH1I> (hname, htitle, mNumTdcChan, -.5, mNumTdcChan - .5, folder);
	
	for (i=0; i<mNumTdcChan; ++i)
	{
		folder.Form(basefolder + "/TDC/AcqChan%02i", i);
		
		hname = "TdcChan"; hname += i;
		htitle = "All hits of TDC Channel "; htitle += i;
		fHistTdcAll[i] = MakeH1<TH1I> (hname, htitle, hist_tdc_bins, hist_tdc_min, hist_tdc_max, folder);
		
		hname = "MinibunchChan"; hname += i;
		htitle = "Total TDC events per minibunch for channel "; htitle += i;
		fHistTdcMinibunch[i] = MakeH1<TH1I> (hname, htitle, hist_minibunch_max, -.5, hist_minibunch_max-.5, folder);
		
		hname.Form("MbHitsChan%i", i);
		htitle.Form("TDC hits per minibunch for each event for channel %i", i);
		fHistTdcMbHits[i] = MakeH2<TH2I>(hname, htitle, hist_minibunch_max, -.5, hist_minibunch_max-.5, hist_tdchits_mb_max, -.5, hist_tdchits_mb_max-.5, folder, "Minibunch index", "Hits per minibunch");
		
		folder += "/Hits";
		
		fHistTdcHits[i] = std::vector<TH1I*> (mNumTdcHits);
		for (j=0; j<mNumTdcHits; j++)
		{
			hname.Form("Tdc_%i_%i", i, j+1);
			htitle.Form("Tdc channel %i hit %i", i, j+1);
			fHistTdcHits[i][j] = MakeH1<TH1I> (hname, htitle, hist_tdc_bins, hist_tdc_min, hist_tdc_max, folder);
		}
	}
	
	fHistAdc = std::vector<TH1I*>(mNumAdcChan);
	fHistAdcMinibunch = std::vector<TH2I*>(mNumAdcChan);
	for (i=0; i<mNumAdcChan; i++)
	{
		folder.Form(basefolder + "/ADC/AcqChan%02i", i);
		hname.Form("AdcChan%i", i);
		htitle.Form("Adc channel %i", i);
		fHistAdc[i] = MakeH1<TH1I>(hname, htitle, hist_adc_bins, hist_adc_min, hist_adc_max, folder);
		
		hname.Form("AdcChan%iMinibunch", i);
		htitle.Form("Adc channel %i vs. minibunch index", i);
		fHistAdcMinibunch[i] = MakeH2<TH2I>(hname, htitle, hist_minibunch_max, -.5, hist_minibunch_max-.5, hist_adc_bins, hist_adc_min, hist_adc_max, folder, "Minibunch index", "ADC value");
	}
	
	//-------------------//
	// Create conditions //
	//-------------------//
	
	folder = basefolder;
	hname = fConfigFile->read<TString> ("acq_veto_histogram", "Tdc_0_1");
	fConTdcVeto = MakeWinCon("Tdc_veto", "Veto TDC events", hist_tdc_min, hist_tdc_max, hname, folder);
	
	
	// To enable or disable the execution of conpar_online.C
	// use the corresponding switch in the cAcqiris.conf
	if (fConfigFile->read("use_conpar_online", false))
	{
		gROOT->ProcessLine(".x conpar_online.C()");
		std::cout << "**** TRemiAcqirisEventSource: conpar_online macro read!" << std::endl;
	}
}


Bool_t TRemiAcqirisEventSource::CheckEventClass(TClass* cl)
{
   return cl->InheritsFrom(TRemiExtractEvent::Class());
}


/**
 * \brief Extracts the data for a single shot from the Acqiris raw event
 */
Bool_t TRemiAcqirisEventSource::SingleShotEvent()
{
	Int_t hitstdc=0, hitsadc=0;
	fPeakStart.setPositionNs(fParaSingleShot->Offset + static_cast<Double_t>(mMinibunchIndex) * fParaSingleShot->MinibunchDistance);
	fAdcStart.setTimestamp(fPeakStart.getPosition());
	fPeakEnd.setPositionNs(fParaSingleShot->Offset + static_cast<Double_t>(mMinibunchIndex+1) * fParaSingleShot->MinibunchDistance + fParaSingleShot->Overlap);
	fAdcEnd.setTimestamp(fPeakEnd.getPosition());
	TRemiAcqirisChannel* SourceChannel;
	
	// go through all possible output channels
	for (UInt_t chan=0; chan<mNumTdcChan; chan++) // TDC channels
		if ( ( mTdcSourceChannel[chan] < fAcqirisEvent.getNbrOfChannels() ) && (fAcqirisEvent.getUsedChannels() & (0x1<<mTdcSourceChannel[chan])) )
		{
			SourceChannel = fAcqirisEvent.getChannel(mTdcSourceChannel[chan]);
			if ( SourceChannel && (SourceChannel->getTdcMethod() != TRemiAcqirisSoftTdc::kNone) )
				hitstdc += FillTdcChannel(chan, SourceChannel);
		}
	for (UInt_t chan=0; chan<mNumAdcChan; chan++) // ADC channels
	{
		if ( ( mAdcSourceChannel[chan] < fAcqirisEvent.getNbrOfChannels() ) && (fAcqirisEvent.getUsedChannels() & (0x1<<mAdcSourceChannel[chan])) )
		{
			SourceChannel = fAcqirisEvent.getChannel(mAdcSourceChannel[chan]);
			if ( SourceChannel && (SourceChannel->getAdcMethod() != TRemiAcqirisSoftAdc::kNone) )
				hitsadc += FillAdcChannel(chan, SourceChannel);
		}
	}

	if ( (hitsadc + hitstdc) > 0 )
	{
		// Write Acqiris properties to ADC channels WARNING: will overwrite existing channel contents
		if (mAdcChanEventId != -1)
			fOutput->fiADC[mAdcChanEventId] = fAcqirisEvent.getEventID();
		if (mAdcChanMbIndex != -1)
			fOutput->fiADC[mAdcChanMbIndex] = mMinibunchIndex;
		if (mAdcChanOffset != -1)
			fOutput->fiADC[mAdcChanOffset] = fPeakStart.getPosition();
		if (fAcqirisEvent.getBunchIdPresent() && (mAdcChanBunchId != -1))
			fOutput->fiADC[mAdcChanBunchId] = fAcqirisEvent.getBunchID();
		if (fAcqirisEvent.getPpDelayPresent() && (mAdcChanPpDelay != -1))
			fOutput->fiADC[mAdcChanPpDelay] = fAcqirisEvent.getPPdelay();
			
		if (fAcqirisEvent.getStreakCamPresent() && (mAdcChanStreakCam != -1))
			fOutput->fiADC[mAdcChanStreakCam] = fAcqirisEvent.getStreakCam();
		if (fAcqirisEvent.getCrossCorrPresent() && (mAdcChanCrossCorr != -1))
			fOutput->fiADC[mAdcChanCrossCorr] = fAcqirisEvent.getCrossCorr();
		if (fAcqirisEvent.getBamPresent() && (mAdcChanBam != -1))
			fOutput->fiADC[mAdcChanBam] = fAcqirisEvent.getBam();	
		if (fAcqirisEvent.getStreakCamFastPresent() && (mAdcChanStreakCamFast != -1))
			fOutput->fiADC[mAdcChanStreakCamFast] = fAcqirisEvent.getStreakCamFast();		
		if (fAcqirisEvent.getDelayEncPresent() && (mAdcChanPpDelayEnc != -1))
			fOutput->fiADC[mAdcChanPpDelayEnc] = fAcqirisEvent.getPPdelayEnc();

		mValidOutputEvent = kTRUE;	// Save event if at least one value has been transferred to Output object
		
		// Fill ADC Histograms
		for (UInt_t i=0; i<mNumAdcChan; i++)
		{
			fHistAdc[i]->Fill(fOutput->fiADC[i]);
			fHistAdcMinibunch[i]->Fill(mMinibunchIndex, fOutput->fiADC[i]);
		}
	}
	
	// At the end increase current Minibunch index and check if end of puls train is reached
	mMinibunchIndex++;
	if (mMinibunchIndex >= fParaSingleShot->NumMinibunches)
		mEventHasData = kFALSE;

	fOutput->SetValid(mValidOutputEvent);
	return kTRUE;
}

/**
 * \brief Fills a Tdc channel of the output event 
 * \param tdctarget Output TDC channel number
 */
Int_t TRemiAcqirisEventSource::FillTdcChannel(Int_t tdctarget, TRemiAcqirisChannel* SourceChannel)
{
	// get Pointers 
	std::vector<TRemiAcqirisPeak> *peaks = SourceChannel->getPeaks();
	std::vector<TRemiAcqirisPeak>::iterator peak = std::lower_bound(peaks->begin(), peaks->end(), fPeakStart);
	
	UInt_t outputcounter = 0;
	Float_t value;
	while ( (peak != peaks->end()) && (*peak < fPeakEnd) )
	{
		// retrieve current peak position into "value" to copy it into output and use for histograms
		value = peak->getPosition() - fPeakStart.getPosition() ;
		
		if (peak->getPolarity() == mTdcSourcePolarity[tdctarget]) // test polarity
		{
			// Fill some histograms
			fHistTdcAll[tdctarget]->Fill(value);
			if (outputcounter < fHistTdcHits[tdctarget].size())
				fHistTdcHits[tdctarget][outputcounter]->Fill(value);
			fHistTdcMinibunch[tdctarget]->Fill(mMinibunchIndex);
			
			if ( (mUseTdcVeto[tdctarget] == kFALSE) || (fConTdcVeto->Test( value )  == kFALSE) )// Test veto
			{
				fOutput->AddTdcHit(tdctarget, value);
				outputcounter++; // Increase hit counter
			}
		}
		++peak; // move to next peak
	}
	fHistTdcMbHits[tdctarget]->Fill(mMinibunchIndex, outputcounter);
	fHistTdcChanHitCount->Fill(tdctarget, outputcounter);
	return outputcounter;
}

/**
* \brief Fills a Adc channel of the output event 
* \param adctarget Output ADC channel number
*/
Int_t TRemiAcqirisEventSource::FillAdcChannel(Int_t adctarget, TRemiAcqirisChannel* SourceChannel)
{
	Double_t adcvalue=0;
	std::vector<TRemiAcqirisAdcPulse>::iterator ThisAdcPulse, LastAdcPulse, Begin = SourceChannel->getAdcPulses()->begin(), End = SourceChannel->getAdcPulses()->end();
	switch (mAdcChannelWhichPulse[adctarget])
	{
		case 0: /// Use sum of ADC values from all pulses
			if (mAdcChannelPerMinibunch[adctarget]) ///< only select pulses from current minibunch
			{
				Begin = std::lower_bound(Begin, End, fAdcStart);
				End = std::lower_bound(Begin, End, fAdcEnd);
			}
			if (Begin==End)
				return 0;
			for (ThisAdcPulse = Begin; ThisAdcPulse != End; ++ThisAdcPulse)
			{
				adcvalue += ThisAdcPulse->getAdcValue();
			}
			break;
		case 1:	// only take first pulse in range
			if (mAdcChannelPerMinibunch[adctarget])
			{
				ThisAdcPulse = std::lower_bound(Begin, End, fAdcStart);
				if (ThisAdcPulse == End)
					return 0;
				else
					adcvalue = ThisAdcPulse->getAdcValue();
			}
			else
				if (SourceChannel->getAdcPulses()->size())
					adcvalue =  SourceChannel->getAdcPulses()->front().getAdcValue();
				else
					return 0;
			break;
		case -1: // only take last pulse in range
			if (mAdcChannelPerMinibunch[adctarget])
			{
				ThisAdcPulse = std::lower_bound(Begin, End, fAdcStart);
				while (*ThisAdcPulse < fAdcEnd && ThisAdcPulse != End)
				{
					LastAdcPulse = ThisAdcPulse;
					++ThisAdcPulse;
				}
			}
			else
				while (ThisAdcPulse != End)
				{
					LastAdcPulse = ThisAdcPulse;
					++ThisAdcPulse;
				}
			if (ThisAdcPulse == End)
				return 0;
			else
				adcvalue = ThisAdcPulse->getAdcValue();
			break;
		default:
			return 0;
			break;
	}
	// at his point we will have a value
	fOutput->fiADC[adctarget] = adcvalue;
	return 1;
}

ClassImp(TRemiAcqirisEventSource);

