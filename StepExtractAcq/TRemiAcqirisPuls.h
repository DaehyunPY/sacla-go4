#ifndef __TRemiAcqirisPuls_H_
#define __TRemiAcqirisPuls_H_

#include "Rtypes.h"

#include <vector>

class TRemiAcqirisPeak;
class TRemiAcqirisChannel;
class TRemiAcqirisArchive;
class TRemiAcqirisIO;

class TRemiAcqirisPuls 
{
	public:
		TRemiAcqirisPuls(TRemiAcqirisChannel * pc)						{        fParentChannel=pc; mDataSize=0; mData=0; mIndex=0; mAdcValue=0;}
		virtual ~TRemiAcqirisPuls();

	protected:
		TRemiAcqirisPuls& operator=(const TRemiAcqirisPuls& in)			{return *this;}
		
	public:
		TRemiAcqirisPuls(const TRemiAcqirisPuls& in);

		Bool_t readPuls(TRemiAcqirisIO* input);
		void clear();

		TRemiAcqirisPeak * addPeak();
		TRemiAcqirisPeak * getPeak(Int_t index) const		{return fPeaks.size()?fPeaks[index]:0;}

		/** \brief Returns pointer to the vector of peaks */
		std::vector<TRemiAcqirisPeak* > * getPeaks()	{ return &fPeaks; }

		void	setTimestampNs(Int_t &in)	{ mTimestamp = in; }
		Int_t getTimestamp()	const					{return mTimestamp;}

		void appendLength(Int_t in)					{        mLength += in;}
		Int_t getLength() const						{return mLength;}

		const Char_t * getData() const				{return mData;}

		TRemiAcqirisChannel * getParentChannel() const				{return fParentChannel;}
		UInt_t getNbrPeaks() const					{return fPeaks.size();}

		void writePulsToFile(TRemiAcqirisArchive &ar);
		
		void setIndex (Int_t in)		{ mIndex = in; }
		Int_t getIndex () const		{ return mIndex; }
		
		void setAdcValue (Double_t &in)	{ mAdcValue = in; }
		Double_t setAdcValue () const	{ return mAdcValue; }

	private:
		Char_t * mData;	///< pointer to the waveform
		size_t mDataSize;	///< the size of the allocated buffer to hold the waveform

		Double_t mTimestamp;	///< timestamp describing where in the channel the puls (waveform) starts
		Int_t mLength;	///< length of the waveform
		
		TRemiAcqirisChannel * fParentChannel;	///< pointer to the channel that this puls belongs to
		
		Int_t	mIndex;	///< Running number of this pulse in the current channel

		std::vector<TRemiAcqirisPeak*> fPeaks;	///< vector containing Pointer to peaks that are in this puls
		
		Double_t	mAdcValue;	///< ADC value of this pulse, can be its integral. Depends on SoftADC

	ClassDef(TRemiAcqirisPuls, 1);
};

#endif 
