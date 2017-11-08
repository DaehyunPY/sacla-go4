#ifndef __TRemiAcqirisChannel_H_
#define __TRemiAcqirisChannel_H_

#include "TRemiAcqirisPeak.h"
#include "TRemiAcqirisAdcPulse.h"

#include "Rtypes.h"

#include <vector>

class TRemiAcqirisPuls;
class TRemiAcqirisArchive;
class TRemiAcqirisBuffer;
class TRemiAcqirisIO;
class TRemiAcqirisEvent;
class TRemiAcqirisSoftAdc;
class TRemiAcqirisSoftTdc;
class ConfigFile;

class TRemiAcqirisChannel 
{
	public:
		TRemiAcqirisChannel(TRemiAcqirisEvent * pe, Int_t channel, ConfigFile* AcqConf);
		virtual ~TRemiAcqirisChannel();

	protected:
		TRemiAcqirisChannel& operator=(const TRemiAcqirisChannel& in)			{return *this;}
		
	public:
		TRemiAcqirisChannel(const TRemiAcqirisChannel& in);

		void clear();
		void readChannel(TRemiAcqirisArchive* input);
		void readChannel(TRemiAcqirisBuffer* input);
		void readFileHeader(TRemiAcqirisArchive & ar);

		UInt_t getNbrPeaks() const							{return fPeaks.size();}		
		TRemiAcqirisPeak * getPeak(Int_t index)						{return fPeaks.empty()?0:&fPeaks[index];}
		TRemiAcqirisPeak * addPeak();
		
		UInt_t getNbrAdcPulses() const							{return fAdcPulses.size();}		
		TRemiAcqirisAdcPulse * getAdcPulse(Int_t index)	{return fAdcPulses.empty() ? 0 : &fAdcPulses[index];}
		TRemiAcqirisAdcPulse * addAdcPulse();
		
		/** \brief Returns pointer to the vector of peaks */
		std::vector<TRemiAcqirisPeak> * getPeaks()	{ return &fPeaks; }
		
		/** \brief Returns pointer to the vector of ADC values */
		std::vector<TRemiAcqirisAdcPulse> * getAdcPulses()	{ return &fAdcPulses; }
		
		void   setBaseline(Short_t in)						{        mBaseline = in;}
		Short_t  getBaseline() const							{return mBaseline;}

		void   setNoise(Short_t in)							{        mNoise = in;}
		Short_t  getNoise() const								{return mNoise;}

		void   setFullScale(Short_t in)						{        mFullscale = in;}
		Short_t  getFullScale()const 							{return mFullscale;}

		void   setOffset(Short_t in)							{        mOffset = in;}
		Short_t  getOffset() const							{return mOffset;}

		void   setVertGain(Double_t in)						{        mGain = in;}
		Double_t getVertGain()const 							{return mGain;}

		void   setStepSize(Int_t in)							{        mStepsize = in;}
		Int_t   getStepSize() const							{return mStepsize;}

		void   setBackSize(Int_t in)							{        mBacksize = in;}
		Int_t   getBackSize() const							{return mBacksize;}

		void   setChannelNbr(Int_t in)						{        mChannelNumber = in;}
		Int_t   getChannelNbr() const						{return mChannelNumber;}
		
		void setTdcMethod(Int_t in)	{ mTdcMethod = in; }
		Int_t getTdcMethod() const	{ return mTdcMethod; }
		
		void setAdcMethod(Int_t in)	{ mAdcMethod = in; }
		Int_t getAdcMethod() const	{ return mAdcMethod; }
		
		void setSampDistNs (Double_t in)	{ mSampDistNs = in;}
		Double_t getSampDistNs () const	{ return mSampDistNs; }
		
		const TRemiAcqirisEvent * getParentEvent() const				{return fParentEvent;}

	private:
		void processChannel (TRemiAcqirisIO* input);
		
		Short_t mFullscale;	///< the fullscale for this channel (in mV)
		Short_t mOffset;	///< the offset for this channel (in adc bytes)
		Double_t mGain;	///< the conversion factor from adc to mV (adc * gain = mV)
		Short_t mBaseline;	///< the baseline for this channel (in adc bytes)
		Short_t mNoise;	///< the noiselevel for this channel (in adc bytes)
		Int_t mStepsize;	///< the stepsize for this channel
		Int_t mBacksize;	///< the backsize for this channel
		Short_t mChannelNumber;	///< the channel number of this channel
		
		static Double_t mSampDistNs; ///< Distance between 2 samples in ns -- is used to convert timerelated Output directly into ns independent of samplingrate
		
		TRemiAcqirisSoftAdc*	fSoftAdc;	///< Soft ADC for this channel
		TRemiAcqirisSoftTdc*	fSoftTdc;	///< Soft TDC for this channel
		
		Int_t mTdcMethod;	///< TDC method for this channel
		Int_t mAdcMethod;	///< ADC method for this channel
		
		TRemiAcqirisPuls* fTempPulse;	///< temporary Pulse object during reading of events
		
		TRemiAcqirisPeak	fTempPeak;	///< temporary Peak object 
		std::vector<TRemiAcqirisPeak> fPeaks;	///< vector containing peaks (will be filled from puls)
		
		TRemiAcqirisAdcPulse	fTempAdcPulse;	///< temporary AdcPulse object
		std::vector<TRemiAcqirisAdcPulse>	fAdcPulses;	///< vector containing ADC values belonging to each pulse in the channel

		const TRemiAcqirisEvent * fParentEvent;	///< pointer to the event that this channel belongs to
		
		ClassDef(TRemiAcqirisChannel, 1);
};

#endif
