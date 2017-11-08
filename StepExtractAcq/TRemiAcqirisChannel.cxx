#include "TRemiAcqirisChannel.h"

#include "TRemiAcqirisDataStreamError.h"
#include "TRemiAcqirisPuls.h"
#include "TRemiAcqirisArchive.h"
#include "TRemiAcqirisBuffer.h"
#include "TRemiAcqirisEvent.h"
#include "TRemiAcqirisSoftAdc.h"
#include "TRemiAcqirisSoftTdc.h"

#include "Riostream.h"


/**
 * \brief Principal constructor
 * \param pe Pointer to event that creates this Channel
 * \param channel Number of new channel
 * \param AcqConf ConfigFile object for this context
 */
TRemiAcqirisChannel::TRemiAcqirisChannel(TRemiAcqirisEvent * pe, Int_t channel, ConfigFile* AcqConf)
{
	mChannelNumber = channel;
	fParentEvent = pe;
	
	// read from ConfigFile
	TString fieldname;
	TString basename("acq_chan_");
	basename += (channel + 1);
	
	fieldname = basename + "_tdc_method";
	mTdcMethod = AcqConf->read(fieldname.Data(), 0);
	
	fieldname = basename + "_adc_method";
	mAdcMethod = AcqConf->read(fieldname.Data(), 0);
	
	fTempPulse = new TRemiAcqirisPuls(this);
	fSoftAdc = new TRemiAcqirisSoftAdc(AcqConf, this);
	fSoftTdc = new TRemiAcqirisSoftTdc(AcqConf, this);
	
	fPeaks = std::vector<TRemiAcqirisPeak> ();
	fTempPeak = TRemiAcqirisPeak (this);
	fTempAdcPulse = TRemiAcqirisAdcPulse ();
}

/** \brief Copy constructor
*/
TRemiAcqirisChannel::TRemiAcqirisChannel(const TRemiAcqirisChannel& in)
{
	mChannelNumber = in.getChannelNbr();
	fParentEvent = in.getParentEvent();
	
	mTdcMethod = in.getTdcMethod();
	mAdcMethod = in.getAdcMethod();
	fSoftAdc = in.fSoftAdc;
	fSoftTdc = in.fSoftTdc;
	fPeaks = in.fPeaks;
	
	fTempPeak = TRemiAcqirisPeak(this);	
	fTempPulse = new TRemiAcqirisPuls(this);
	fTempAdcPulse = TRemiAcqirisAdcPulse();
}

//______________________________________________________________________________________________________________________
TRemiAcqirisChannel::~TRemiAcqirisChannel()
{
	delete fTempPulse;
	delete fSoftAdc;
	delete fSoftTdc;
}

//______________________________________________________________________________________________________________________
void TRemiAcqirisChannel::clear()
{
	fTempPulse->clear();
	fSoftTdc->clear();

	//--first clear the peaks in the vector, that we have used before--//
	fPeaks.clear();
	fAdcPulses.clear();
}

//______________________________________________________________________________________________________________________
void TRemiAcqirisChannel::readFileHeader(TRemiAcqirisArchive &ar)
{
	//read all the values in the header from the archive
	ar >> mFullscale;
	ar >> mOffset;
	ar >> mGain;
	ar >> mBaseline;
	ar >> mNoise;
	ar >> mStepsize;
	ar >> mBacksize;

	return;
}

/** \brief Add a peak to this channel
*
* This function is usually called by the corresponding function in the TRemiAcqirisPuls class
*/
TRemiAcqirisPeak* TRemiAcqirisChannel::addPeak()
{
	// Create another copy of fTempPeak in the peaks vector
	fPeaks.push_back(fTempPeak);
	
	//--return the added Peak--//
	return &(fPeaks.back());
}


/** \brief Add a AdcPulse to this channel
*/
TRemiAcqirisAdcPulse* TRemiAcqirisChannel::addAdcPulse()
{
	// Create another copy of fTempPeak in the peaks vector
	fAdcPulses.push_back(fTempAdcPulse);
	
	//--return the added Peak--//
	return &(fAdcPulses.back());
}


/**
* \brief Read Channel data from file input
* \param input File archive opened for reading
*/
void TRemiAcqirisChannel::readChannel(TRemiAcqirisArchive* input)
{
	processChannel(input);
}

/**
* \brief Read Channel data from file input
* \param input Buffer opened for reading
*/
void TRemiAcqirisChannel::readChannel(TRemiAcqirisBuffer* input)
{
	
	
	*input >> mFullscale;		//the fullscale of the channel in mV
	*input >> mOffset;			//the offset in mV
	*input >> mGain;			//the vertical gain to convert ADC bytes to mV
	*input >> mBaseline;		//the baseline in bits
	*input >> mNoise;			//the noise in bits
	*input >> mStepsize;			//the stepsize in values of Sample Interval
	*input >> mBacksize;				//backsize in values of Sample Interval
	
	processChannel(input);
}

/**
* \brief Internal function to process a channel that is being readChannel
* \param input File archive or buffer opened for reading
*/
void TRemiAcqirisChannel::processChannel(TRemiAcqirisIO* input)
{
	setSampDistNs(fParentEvent->getSampleInterval()*1e9);
	Short_t tmpNbrPulses;
	*input >> tmpNbrPulses;		//nbr of Pulses found by suppress zero

	if ((tmpNbrPulses < 0) || (MAX_ARRAY_LENGTH < tmpNbrPulses))
	{
		throw TRemiAcqirisDataStreamError(tmpNbrPulses, "tmpNbrPulses", __func__, __FILE__, __LINE__);
	}

	for (int i=0; i<tmpNbrPulses; ++i)	
	{
		fTempPulse->clear();
		fTempPulse->setIndex(i);
		fTempPulse->readPuls(input);
		if (mTdcMethod != TRemiAcqirisSoftTdc::kNone)
			fSoftTdc->search_for_Peaks(fTempPulse);
		if (mAdcMethod != TRemiAcqirisSoftAdc::kNone)
			fSoftAdc->runAdc(fTempPulse);
	}
	
	// Fill ADC and TDC raw data spectra
	if (mTdcMethod != TRemiAcqirisSoftTdc::kNone)
		fSoftTdc->fillRawSpectra();
	if (mAdcMethod != TRemiAcqirisSoftAdc::kNone)
		fSoftAdc->fillRawSpectra();
}

Double_t TRemiAcqirisChannel::mSampDistNs = 1.;

ClassImp(TRemiAcqirisChannel);

