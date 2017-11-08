#include "TRemiAcqirisEvent.h"

#include "TRemiAcqirisDataStreamError.h"
#include "TRemiAcqirisChannel.h"
#include "TRemiAcqirisArchive.h"
#include "TRemiAcqirisBuffer.h"

#include "Riostream.h"


//______________________________________________________________________________________________________________________
TRemiAcqirisEvent::TRemiAcqirisEvent(Short_t nbrCh, ConfigFile* AcqConf) 
	: TRemiIncludeFunctions(AcqConf)
{
	mNumChannels=0;
	fChannels = std::vector<TRemiAcqirisChannel*>();
	init(nbrCh, AcqConf);
}

//______________________________________________________________________________________________________________________
TRemiAcqirisEvent::~TRemiAcqirisEvent()
{
	for (UInt_t i=0;i<fChannels.size();++i) 
		delete fChannels[i];
}
//______________________________________________________________________________________________________________________
void TRemiAcqirisEvent::init(size_t nbrChns, ConfigFile* AcqConf)
{
	// The following is a workaround
	// The argument-less default constructor does not set fConfigFile
	// The above constructor with AcqConf as argument is never called
	fConfigFile = AcqConf;

	if(nbrChns!=mNumChannels) // only init if sth has changed
	{
/*
		for (UInt_t i=0;i<fChannels.size();++i) 
			delete fChannels[i];
*/
		mNumChannels=nbrChns;	
		//--in the vector of pointers to channels create the number of channels--//
		fChannels.resize(mNumChannels);
		for (UInt_t i=0;i<mNumChannels;++i)
		{
			fChannels[i] = new TRemiAcqirisChannel(this, i, AcqConf);
		}

	}

	mPadding_present = AcqConf->read("acq_padding_present", false);
	mGMD_present = AcqConf->read("acq_gmd_present", false);
	mBunchID_present = AcqConf->read("acq_bunchid_present", false);
	mBunchIDdirect_present = AcqConf->read("acq_bunchiddirect_present", false);
	mTimeStamp_present = AcqConf->read("acq_timestamp_present", false);
	mPpDelay_present = AcqConf->read("acq_ppdelay_present", false);
	mStreakCam_present = AcqConf->read("acq_streakcam_present", false);
	mStreakCamFast_present = AcqConf->read("acq_streakcam_fast_present", false);
	mCrossCorr_present = AcqConf->read("acq_crosscorr_present", false);
	mBam_present = AcqConf->read("acq_bam_present", false);
	mPpDelayEnc_present = AcqConf->read("acq_ppdelay_enc_present", false);
}

TRemiAcqirisEvent::TRemiAcqirisEvent(const TRemiAcqirisEvent& in)
{
	TRemiAcqirisChannel *channel;
	mNumChannels = in.getNbrOfChannels();
	for (UInt_t i=0; i<in.getNbrOfChannels(); i++)
	{
		channel = new TRemiAcqirisChannel(*(in.getChannel(i)));
		fChannels.push_back(channel);
	}
	mGMD_present = in.getGmdPresent();
	mBunchID_present = in.getBunchIdPresent();
	mBunchIDdirect_present = in.getBunchIdDirectPresent();
	mPpDelay_present = in.getPpDelayPresent();
	mStreakCam_present = in.getStreakCamPresent();
	mCrossCorr_present = in.getCrossCorrPresent();
	mBam_present = in.getBamPresent();
	mStreakCamFast_present = in.getStreakCamFastPresent();
}

TRemiAcqirisEvent& TRemiAcqirisEvent::operator=(const TRemiAcqirisEvent& in)
{
	TRemiAcqirisChannel *channel;
	mNumChannels = in.getNbrOfChannels();
	for (UInt_t i=0; i<in.getNbrOfChannels(); i++)
	{
		channel = new TRemiAcqirisChannel(*(in.getChannel(i)));
		fChannels.push_back(channel);
	}
	mGMD_present = in.getGmdPresent();
	mBunchID_present = in.getBunchIdPresent();
	mBunchIDdirect_present = in.getBunchIdDirectPresent();
	mPpDelay_present = in.getPpDelayPresent();
	mStreakCam_present = in.getStreakCamPresent();
	mCrossCorr_present = in.getCrossCorrPresent();
	mBam_present = in.getBamPresent();
	mStreakCamFast_present = in.getStreakCamFastPresent();
	return *this;
}


//______________________________________________________________________________________________________________________
void TRemiAcqirisEvent::readFileHeader(TRemiAcqirisArchive & ar)
{
	//--read all the values in the header for the Event--//
	ar >> mNumBits;
	ar >> mSampleInterval;
	ar >> mNumSamples;
	ar >> mTriggerDelay;
	ar >> mTrigChan;
	ar >> mTrigLevel;
	ar >> mTrigSlope;
	ar >> mUsedChans;
	ar >> mChanCombUsedChans;
	ar >> mNumConPerCh;
	//now go through all channels and read the stuff in the header for them
	//but only if the channel was recorded
	for (UInt_t i=0;i<mNumChannels;++i)
		if (mUsedChans & (0x1<<i))
			fChannels[i]->readFileHeader(ar);
}
//______________________________________________________________________________________________________________________

void TRemiAcqirisEvent::writeEventData(TRemiAcqirisArchive &ar)
{
	ar	<< (Int_t)mEventID;
	ar.writeArray(mGmd,sizeof(float)*getGMDarray_length());
	ar	<< (Double_t) mHorpos;
}
//______________________________________________________________________________________________________________________

void TRemiAcqirisEvent::readEvent(TRemiAcqirisBuffer* input)
{
	//--read the event values from the archive--//

	if (mPadding_present)
	{
		// The following is a workaround
		// When reading from 64bit poirot (i.e., linux agat3),
		// due to a 'long' that should be an 'Int_t',
		// the data stream has a strange padding, remove it:

		Int_t dummy;
		*input >> dummy;
//		cout << "Data stream padding: " << dummy << endl;

		if (dummy != 0)
		{
			throw TRemiAcqirisDataStreamError(dummy, "Data stream padding", __func__, __FILE__, __LINE__);
		}
	}


/*
	*input >>  mEventID;				//event time stamp

	if (mGMD_present)
		input->readArray(mGmd,sizeof(float)*getGMDarray_length()); // read in Ethernet GMD-signal
	if (mBunchID_present)
		*input >> mBunchId;
	if (mPpDelay_present)
		*input >> mPpDelay;
	if (mTimeStamp_present)
		*input >> mTimeStamp;
	if (mStreakCam_present)
		*input >> mStreakCam;
	if (mStreakCamFast_present)
		*input >> mStreakCamFast;
	if (mCrossCorr_present)
		*input >> mCrossCorr;
	if (mBam_present)
		*input >> mBam;
	if (mPpDelayEnc_present)
		*input >> mPpDelayEnc;
*/

	if (mTimeStamp_present)
		*input >> mTimeStamp;

	*input >>  mEventID;				//event time stamp

	if (mBunchIDdirect_present)
		*input >> mBunchIdDirect;
	if (mBunchID_present)
		*input >> mBunchId;
	if (mPpDelay_present)
		*input >> mPpDelay;
	if (mPpDelayEnc_present)
		*input >> mPpDelayEnc;
	if (mCrossCorr_present)
		*input >> mCrossCorr;
	if (mStreakCam_present)
		*input >> mStreakCam;
	if (mStreakCamFast_present)
		*input >> mStreakCamFast;
	if (mBam_present)
		*input >> mBam;
	if (mGMD_present)
	{
		input->readArray(mGmd,sizeof(float)*getGMDarray_length()); // read in Ethernet GMD-signal
	}


	*input >> mHorpos;				//horpos from acqiris
	*input >> mNumBits;			//number of bits used for readout (either byte(8) or short(16))
	*input >> mSampleInterval;			//sample Interval (1ns = 1GigaSample abtastrate)
	*input >> mNumSamples;			//number of samples (timerange in ns is (nbrSamples * sampleIntveral))
	*input >> mTriggerDelay;			//where the trigger occured
	*input >> mTrigChan;			//the channel we are triggering on (-1) for external
	*input >> mTrigLevel;			//the TriggerLevel
	*input >> mTrigSlope;			//trigger on positive edge or negative edge
	*input >> mNumConPerCh;		//the number of Converters per channel (for channelcombination)
	*input >> mChanCombUsedChans;	//a bit pattern describing which channles are used for the channel combination
	*input >> mUsedChans;			//a bit pattern describing which channles are used
	*input >> mNumChannels;		//nbrChannels


	if ((mNumChannels <= 0) || (fChannels.size() < mNumChannels))
	{
		throw TRemiAcqirisDataStreamError(mNumChannels, "mNumChannels", __func__, __FILE__, __LINE__);
	}


	//cout << mHorpos<< " "<< mNumBits << " " << mSampleInterval<< " " << mTriggerDelay<< " " << mTrigChan<< " " << mTrigLevel<< " " << mTrigSlope<< " " << mNumConPerCh<< " " << mChanCombUsedChans << " " <<mUsedChans << " " << mNumChannels<< endl;

// The following is probably never a good idea:
// The number of channels should be set in the config files
/*
	if (mNumChannels > fChannels.size())
	{
		UShort_t oldnum = fChannels.size();
		fChannels.resize(mNumChannels);
		for (UShort_t i=oldnum;i<fChannels.size();++i)
			fChannels[i] = new TRemiAcqirisChannel(this, i, fConfigFile);
	}
*/


	//--if the channel was recorded serialize it
	for (UInt_t i=0; i<mNumChannels;++i)
		if (mUsedChans & (0x1<<i))
			fChannels[i]->readChannel(input);
}



void TRemiAcqirisEvent::readEvent(TRemiAcqirisArchive* input)
{
 	//--read the event values from the archive--//
	if (mTimeStamp_present)
		*input >> mTimeStamp;

	*input >>  mEventID;				//event time stamp

	if (mBunchIDdirect_present)
		*input >> mBunchIdDirect;
	if (mBunchID_present)
		*input >> mBunchId;
	if (mPpDelay_present)
		*input >> mPpDelay;
	if (mPpDelayEnc_present)
		*input >> mPpDelayEnc;
	if (mCrossCorr_present)
		*input >> mCrossCorr;
	if (mStreakCam_present)
		*input >> mStreakCam;
	if (mStreakCamFast_present)
		*input >> mStreakCamFast;
	if (mBam_present)
		*input >> mBam;
	if (mGMD_present)
	{
		input->readArray(mGmd,sizeof(float)*getGMDarray_length()); // read in Ethernet GMD-signal
	}

	//std::cerr<<"EventID:"<<EventID<<std::endl;
	*input >> mHorpos;				//horpos from acqiris
	//std::cerr<<"horpos:"<<horpos<<std::endl;

	//--if the channel was recorded serialize it
	for (UInt_t i=0; i<mNumChannels;++i)
		if (mUsedChans & (0x1<<i))
			fChannels[i]->readChannel(input);
}

//______________________________________________________________________________________________________________________
void TRemiAcqirisEvent::clear()
{
	//--go through the whole vector an clear the elements--//
	//--but only if the channel was recorded before--//
	for (UInt_t i=0; i<fChannels.size();++i)
		if (mUsedChans & (0x1<<i))
			fChannels[i]->clear();
}

ClassImp(TRemiAcqirisEvent);
