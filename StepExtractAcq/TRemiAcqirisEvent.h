#ifndef __TRemiAcqirisEvent_H_
#define __TRemiAcqirisEvent_H_

#include "../Core/TRemiIncludeFunctions.h"

#include "Rtypes.h"

#include <vector>

class TRemiAcqirisChannel;
class TRemiAcqirisArchive;
class TRemiAcqirisBuffer;
class ConfigFile;

class TRemiAcqirisEvent : public TRemiIncludeFunctions
{
	public:
		TRemiAcqirisEvent()								{        mNumChannels=0;}
		TRemiAcqirisEvent(Short_t nbrChannels, ConfigFile* AcqConf);
		virtual ~TRemiAcqirisEvent();

		TRemiAcqirisEvent& operator=(const TRemiAcqirisEvent& in);
		TRemiAcqirisEvent(const TRemiAcqirisEvent& in);

		void init(size_t nbrChns, ConfigFile* AcqConf);	// must be invoked when standard constructor is used
		void clear();
		void readEvent(TRemiAcqirisBuffer* input);
		void readEvent(TRemiAcqirisArchive* input);
		void readFileHeader(TRemiAcqirisArchive & ar);
		void writeEventHeader(TRemiAcqirisArchive &ar);
		void writeEventHeader(TRemiAcqirisArchive &ar, Int_t UsedChans_Bitmask);
		void writeEventData(TRemiAcqirisArchive &ar);
		TRemiAcqirisChannel * getChannel(UInt_t index) const		{return (index<mNumChannels)?fChannels[index]:0;}
		UShort_t getNbrOfChannels() const			{return mNumChannels;}

		Int_t getEventID() const					{return mEventID;}
		Double_t getHorpos() const				{return mHorpos;}
		Short_t getNbrBits() const				{return mNumBits;}
		Double_t getSampleInterval() const		{return mSampleInterval;}
		Int_t getNbrSamples() const				{return mNumSamples;}
		Double_t getDelayTime() const				{return mTriggerDelay;}
		Short_t getTriggerChannel() const			{return mTrigChan;}
		Double_t getTriggerLevel() const			{return mTrigLevel;}
		Short_t getTriggerSlope() const			{return mTrigSlope;}
		Int_t getUsedChannels() const			{return mUsedChans;}
		Int_t getChanCombUsedChannels() const	{return mChanCombUsedChans;}
		Short_t getNbrConvPerChans() const		{return mNumConPerCh;}
		Bool_t	getGmdPresent() const		{ return mGMD_present; }
		Bool_t	getBunchIdPresent() const		{ return mBunchID_present; }
		Bool_t	getBunchIdDirectPresent() const		{ return mBunchIDdirect_present; }
		Bool_t	getPpDelayPresent() const		{ return mPpDelay_present; }
		Bool_t	getTimeStampPresent() const		{ return mTimeStamp_present; }
		Bool_t	getStreakCamPresent() const		{ return mStreakCam_present; }
		Bool_t	getStreakCamFastPresent() const	{ return mStreakCamFast_present; }
		Bool_t	getCrossCorrPresent() const		{ return mCrossCorr_present; }
		Bool_t	getBamPresent() const			{ return mBam_present; }
		Bool_t	getDelayEncPresent() const		{ return mPpDelayEnc_present; }
		
		float getGMD_ethernet(Int_t index)		{return mGmd[index];}
		const float *const getGMD_pointer()			{return mGmd;}
		Int_t getGMDarray_length()				{return GMDarray_length;}
		Int_t getBunchID() const        { return mBunchId; }
		Int_t getBunchIDdirect() const        { return mBunchIdDirect; }
		float getPPdelay() const        { return mPpDelay;}
		
		float getStreakCam() const	{return mStreakCam;}
		float getCrossCorr() const	{return mCrossCorr;}
		float getBam() const	{return mBam;}
		float getStreakCamFast() const	{return mStreakCamFast;}
		float getPPdelayEnc() const        { return mPpDelayEnc;}
		
		

	private:
		Int_t mEventID;	///< an id for this event. It is just the time for the time the event was recorded
		Double_t mHorpos;	///< the horpos value for this event
		
		// all these values are in the file header
		Short_t mNumBits;	///< nbr of bits of the adc values (either 8 or 16)
		Double_t mSampleInterval;	///< the time between two consecutive points (in ns)
		Int_t mNumSamples;	///< nbr of Points (multiplied by the sampInter it will give the timewindow in ns)
		Double_t mTriggerDelay;	///< the delay of the trigger with respect to the window
		Short_t mTrigChan;	///< trigger Channel
		Double_t mTrigLevel;	///< trigger Level
		Short_t mTrigSlope;	///< trigger Slope
		Int_t mUsedChans;	///< Bitmask discribing which Channels have been recorded 
		Int_t mChanCombUsedChans;	///< Bitmask discribing how many Converters per Channel were used
		Short_t mNumConPerCh;
		
		Bool_t mPadding_present;	///< Is there GMD data to be read?
		Bool_t mGMD_present;	///< Is there GMD data to be read?
		Bool_t mBunchID_present;	///< Is there a bunchID to be read?
		Bool_t mBunchIDdirect_present;	///< Is there a bunchIDdirect to be read?
		Bool_t mTimeStamp_present;	///< is there a time stamp to be read?
		Bool_t mPpDelay_present;	///< Is there a Pump-prope delay to be read?
		Bool_t mStreakCam_present;	///< Is there a streak cam signal to be read?
		Bool_t mStreakCamFast_present;	///< Is there a fast streak cam signal to be read?
		Bool_t mCrossCorr_present;	///< Is there a cross correlator signal to be read?
		Bool_t mBam_present;	///< Is there a Bam signal to be read?
		Bool_t mPpDelayEnc_present;	///< Is there a delay encoder signal to be read?
		
		std::vector<TRemiAcqirisChannel*> fChannels;	///< vector of Pointers to TRemiAcqirisChannel
		UShort_t mNumChannels;	///< nbr of Channels in this event

		static const Int_t GMDarray_length = 400;
		Float_t	mGmd[GMDarray_length];	///< Contains GMD data provided by FLASH
		Int_t	mBunchId;         ///< BunchID from FLASH
		Int_t	mBunchIdDirect;         ///< BunchIDdirect from FLASH
		Long64_t	mTimeStamp;	///< Time Stamp from the measurement computer; locked to Desy time
		Float_t	mPpDelay;          ///< Pump probe laser delay
		Float_t	mStreakCam;	///< StreakCam signal from DOOCS
		Float_t mStreakCamFast;	///< Fast StreakCam signal from DOOCS
		Float_t mCrossCorr;	///< Crosscorrelator signal from DOOCS
		Float_t mBam;	///< BAM signal from DOOCS
		Float_t mPpDelayEnc;	///< Delay encoder signal from DOOCS
		
		ClassDef(TRemiAcqirisEvent, 1);
};

#endif
