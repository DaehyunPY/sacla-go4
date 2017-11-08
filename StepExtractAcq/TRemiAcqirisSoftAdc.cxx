#include "TRemiAcqirisSoftAdc.h"

#include "TRemiAcqirisEvent.h"
#include "TRemiAcqirisPuls.h"
#include "TRemiAcqirisPeak.h"
#include "TRemiAcqirisAdcPulse.h"
#include "TRemiAcqirisChannel.h"

#include "TGo4Picture.h"

#include "TH1.h"
#include "TMath.h"
#include "TString.h"
#include "TGraph.h"

#include "Riostream.h"

/** \brief Default constructor. Not to be used */
TRemiAcqirisSoftAdc::TRemiAcqirisSoftAdc()
{

}


/** \brief Principal constructor */
TRemiAcqirisSoftAdc::TRemiAcqirisSoftAdc(ConfigFile* AcqConf, TRemiAcqirisChannel* ParentChannel) : 
	TRemiIncludeFunctions(AcqConf)
{
	if (!fConfigFile)
	{
		std::cout << "**** TRemiAcqirisSoftAdc: No pointer to Config File";
		return;
	}
	fParentChannel = ParentChannel;
		
	mMethod = fParentChannel->getAdcMethod();
	
	TString  basefolder="Acq", hname, htitle, folder;
	
	// Create histograms to show contents of this channel
	folder.Form(basefolder + "/raw/AcqChan%02i", fParentChannel->getChannelNbr()+1);
	if (mMethod != 0)
	{
		hname.Form("Acq_Adc_%02i", fParentChannel->getChannelNbr()+1);
		htitle.Form("Adc values in Acqiris channel %i", fParentChannel->getChannelNbr()+1);
		fHistRawAdc = MakeH1<TH1I>(hname, htitle, "acq_hist_adc_raw_bins", "acq_hist_adc_raw_min", "acq_hist_adc_raw_max", folder);
	}

	// Initialise Watchmode if requested
	mWatchMode = fConfigFile->read("acq_use_adc_watchmode", false);
	if (mWatchMode)
		initWatchMode();
}


/**
 * \brief Plots the raw spectrum for each pulse
 */
void TRemiAcqirisSoftAdc::fillRawSpectra()
{
	std::vector<TRemiAcqirisAdcPulse>::iterator ThisAdc, AdcBegin = fParentChannel->getAdcPulses()->begin(), AdcEnd = fParentChannel->getAdcPulses()->end();
	for (ThisAdc = AdcBegin; ThisAdc != AdcEnd; ++ThisAdc)
		fHistRawAdc->Fill(ThisAdc->getAdcValue());
}


/** 
* \brief General function to call to calculate ADC value
* \param Pulse Pointer to current pulse trace
* \param NumPulses Number of Pulses in the current channel
* 
* Actually passes the call to runAdcTemplate() with the correct template argument
*/
void TRemiAcqirisSoftAdc::runAdc(TRemiAcqirisPuls *Pulse)
{
	Short_t nbrOfBits = fParentChannel->getParentEvent()->getNbrBits();
	if(nbrOfBits==TRemiAcqirisSoftAdc::k8Bit)
		runAdcTemplate<Char_t>(Pulse);
	else if(nbrOfBits==TRemiAcqirisSoftAdc::k16Bit)
		runAdcTemplate<Short_t>(Pulse);
}


/** 
* \brief Internally used function to caclulate ADC value for a given event
* \param Pulse Pointer to current pulse trace
* \param NumPulses Number of Pulses in the current channel
* \tparam T Data type
* Goes through all channels of the data and calculates value with the specified function
*/
template<class T> void TRemiAcqirisSoftAdc::runAdcTemplate(TRemiAcqirisPuls *Pulse)
{
	switch(mMethod)
	{
		case TRemiAcqirisSoftAdc::kIntegral :
			Integral<T>(Pulse);
			break;
			
		case TRemiAcqirisSoftAdc::kMaximum :
			Maximum<T>(Pulse);
			break;
			
		case TRemiAcqirisSoftAdc::kFirstTime :
			FirstTime<T>(Pulse);
			break;
			
		case TRemiAcqirisSoftAdc::kModulusIntegral :
			ModulusIntegral<T>(Pulse);
			break;
			
		default :
			break;
	}
	
	// Run watchmode, if activated
	if (mWatchMode)
		watchMode<T>(Pulse);
}

/**
* \brief Calculates ADC value by integrating the pulse
* \param Pulse Pointer to current pulse trace
* \tparam T Data type
* \returns ADC value
*/
template<class T> void TRemiAcqirisSoftAdc::Integral(TRemiAcqirisPuls *Pulse)
{
	Double_t gain = fParentChannel->getVertGain();
	Double_t offset = fParentChannel->getOffset();
	
	T * Data = (T*) Pulse->getData();
	Int_t length = Pulse->getLength();
	if (length)
	{
		Double_t value = 0;
		// Loop through pulse and sum up all values
		for (Int_t j=0; j<length; ++j)
			value += ((static_cast<Double_t>(Data[j]) * gain) - offset);
		value /= static_cast<Double_t>(length);
		
		// Write to new AdcPulse object
		TRemiAcqirisAdcPulse* NewAdcPulse = fParentChannel->addAdcPulse();
		NewAdcPulse->setAdcValue(value);
		NewAdcPulse->setTimestamp(Pulse->getTimestamp() + length/2*fParentChannel->getSampDistNs());
	}
}

/**
* \brief Calculates ADC value by determining the maximum value of the pulse
* \param Pulse Pointer to current pulse trace
* \tparam T Data type
*/
template<class T> void TRemiAcqirisSoftAdc::Maximum(TRemiAcqirisPuls *Pulse)
{
	Double_t gain = fParentChannel->getVertGain();
	Double_t offset = fParentChannel->getOffset();
	
	T * Data = (T*) Pulse->getData();
	Int_t length = Pulse->getLength();
	if (length)
	{
		Double_t value = 0, currentvalue;
		// Loop through pulse and find the maximum
		for (Int_t j=0; j<length; ++j)
		{
			currentvalue = TMath::Abs((static_cast<Double_t>(Data[j]) * gain) - offset);
			if (currentvalue > value)
				value = currentvalue;
		}
		
		// Write to new AdcPulse object
		TRemiAcqirisAdcPulse* NewAdcPulse = fParentChannel->addAdcPulse();
		NewAdcPulse->setAdcValue(value);
		NewAdcPulse->setTimestamp(Pulse->getTimestamp() + length/2*fParentChannel->getSampDistNs());
	}
}

/**
* \brief Calculates ADC value by taking the arrival time of the first peak in the pulse
* \param Pulse Pointer to current pulse trace
* \tparam T Data type
*/
template<class T> void TRemiAcqirisSoftAdc::FirstTime(TRemiAcqirisPuls *Pulse)
{
	if (Pulse->getNbrPeaks())
	{
		// Write to new AdcPulse object
		TRemiAcqirisAdcPulse* NewAdcPulse = fParentChannel->addAdcPulse();
		NewAdcPulse->setAdcValue(Pulse->getPeak(0)->getPosition());
		NewAdcPulse->setTimestamp(Pulse->getTimestamp() + Pulse->getLength()/2*fParentChannel->getSampDistNs());
	}
}

/**
* \brief Calculates ADC value by integrating the modulus of the pulse
* \param Pulse Pointer to current pulse trace
* \tparam T Data type
* \returns ADC value
*/
template<class T> void TRemiAcqirisSoftAdc::ModulusIntegral(TRemiAcqirisPuls *Pulse)
{
	Double_t gain = fParentChannel->getVertGain();
	Double_t offset = fParentChannel->getOffset();
	
	T * Data = (T*) Pulse->getData();
	Int_t length = Pulse->getLength();
	if (length)
	{
		Double_t value = 0;
		// Loop through pulse and sum up all values
		for (Int_t j=0; j<length; ++j)
			value += TMath::Abs((static_cast<Double_t>(Data[j]) * gain) - offset);
		value /= static_cast<Double_t>(length);
		
		// Write to new AdcPulse object
		TRemiAcqirisAdcPulse* NewAdcPulse = fParentChannel->addAdcPulse();
		NewAdcPulse->setAdcValue(value);
		NewAdcPulse->setTimestamp(Pulse->getTimestamp() + length/2*fParentChannel->getSampDistNs());
	}
}



/**
* \brief Initialise watchmode component
*/
void TRemiAcqirisSoftAdc::initWatchMode()
{
	TString picname, pictitle, gname, gtitle, folder, basefolder="Acq";
	mNumPulseGraphColumns = fConfigFile->read("acq_adc_watchmode_num_columns", 3);
	mNumPulseGraphs = fConfigFile->read("acq_adc_watchmode_num_pulses", 5);
	
	Int_t row, col;
	if (mMethod != 0)
	{
		picname.Form("PicAdcChan%i", fParentChannel->getChannelNbr()+1);
		pictitle.Form("Watching ADC contents for channel %i", fParentChannel->getChannelNbr()+1);
		folder = basefolder + "/ADCwatch";
		fPicGraphs = MakePicture(picname, pictitle, folder, ((mNumPulseGraphs-1)/mNumPulseGraphColumns)+1, mNumPulseGraphColumns);
		
		fPulseGraph = std::vector<TGraph*> (mNumPulseGraphs);
		for (Int_t j=0; j<mNumPulseGraphs; j++)
		{
			row = j/mNumPulseGraphColumns; // Current row in picture
			col = j%mNumPulseGraphColumns; // Current column in picture
			
			// Add graph with pulse
			gname.Form("AdcGraph_%i_%i", fParentChannel->getChannelNbr()+1, j);
			gtitle.Form("Pulse No. %i in Channel %i", j, fParentChannel->getChannelNbr()+1);
			fPulseGraph[j] = MakeGraph(gname, gtitle, folder);
			AddGraphToPicture(fPulseGraph[j], fPicGraphs, row, col);
			fPicGraphs->Pic(row, col)->SetDrawOption("AL"); // Draw lines
		}
	}
}



/**
* \brief Creates graphs to watch ADC contents
* \param Pulse Pointer to current pulse trace
* \param IndexPulse Running index of this pulse trace within current channel
* \param NumPulses Number of Pulses in the current channel
* \tparam T data type
*/
template<class T> void TRemiAcqirisSoftAdc::watchMode(TRemiAcqirisPuls* Pulse)
{
	if (mWatchMode)
		if (mMethod != kNone)
		{
			T * Data = (T*) Pulse->getData();
			
			// Create graph of pulse
			Int_t psize = Pulse->getLength();
			Int_t pindex = Pulse->getIndex();
			if (pindex < mNumPulseGraphs)
			{
				fPulseGraph[pindex]->Set(psize);
				for (Int_t i=0; i<psize; i++)
					fPulseGraph[pindex]->SetPoint(i, fParentChannel->getParentEvent()->getSampleInterval() * 1e9 * static_cast<Float_t>( i) + Pulse->getTimestamp(), Data[i]);
			}
		}
						
}


ClassImp(TRemiAcqirisSoftAdc);
