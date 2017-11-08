#ifndef __TRemiAcqirisPeak_H__
#define __TRemiAcqirisPeak_H__

#include "Rtypes.h"

class TRemiAcqirisChannel;



enum EPolarity
{
	kBad=0,
	kPositive=1,
	kNegative=-1
};


/**
 * \brief Class for peaks obtained by the Soft TDC
 */
class TRemiAcqirisPeak  
{
	public:
		TRemiAcqirisPeak(TRemiAcqirisChannel * Channel);						
		TRemiAcqirisPeak()								{clear();}
		virtual ~TRemiAcqirisPeak()								{}

		TRemiAcqirisPeak(const TRemiAcqirisPeak& in);

		TRemiAcqirisPeak& operator=(const TRemiAcqirisPeak&);
		bool	operator< (const TRemiAcqirisPeak& rightPeak) const	{ return (this->getPosition() < rightPeak.getPosition()); }

		void clear();

		void   setPositionNs(Double_t in)				{mPosition = in;}	///< Set position in nano seconds
		Double_t getPosition() const					{return mPosition;}

		void   setFWHMNs(Double_t in)				{        mFwhm = in;}
		Double_t getFWHM() const					{return mFwhm;}

		void   setStartPos(Int_t in)				{        mStartPos = in;}
		Int_t   getStartPos() const				{return mStartPos;}

		void   setStopPos(Int_t in)				{        mStopPos = in;}
		Int_t   getStopPos() const				{return mStopPos;}

		void   setMaxPos(Int_t in)				{        mMaxPos = in;}
		Int_t   getMaxPos() const				{return mMaxPos;}

		void   setMaximum(Int_t in)				{        mMaximum = in;}
		Int_t   getMaximum() const				{return mMaximum;}

		void   setPolarity(Int_t in)				{        mPolarity = in;}
		Short_t   getPolarity() const				{return mPolarity;}

		void   setUsed(Bool_t in)				{        mUsed = in;}
		Bool_t   getUsed() const				{return mUsed;}
		
		TRemiAcqirisChannel * getParentChannel() const				{return fParentChannel;}

	private:
		
		Double_t mPosition;	///< Position of the peak as determined by the TDC
		Double_t mFwhm;	///< Full width at half maximum of the peak
		Int_t mStartPos;	///< Starting position of the peak
		Int_t mStopPos;	///< Terminating position of the peak
		Int_t mMaxPos;	///< Position of the maximum
		Int_t mMaximum;	///< Maximum value
		Int_t mPolarity;	///< Polarity
		Bool_t mUsed;	///< Flag if this peak has been used already
		
		TRemiAcqirisChannel * fParentChannel;	///< Pointer to the Channel where this peak is derived from
		
	ClassDef(TRemiAcqirisPeak, 1);
};

/**
 * \brief This class as used to compare pointers to peaks e.g. for sorting
 */
class TRemiAcqirisPeakSorter
{
	public :
		virtual ~TRemiAcqirisPeakSorter(){}
		bool operator () (TRemiAcqirisPeak* leftPeak, TRemiAcqirisPeak* rightPeak)
		{
			return (leftPeak->getPosition() < rightPeak->getPosition());
		}
	ClassDef(TRemiAcqirisPeakSorter, 1);
};

#endif
