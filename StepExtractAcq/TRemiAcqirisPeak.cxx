#include "TRemiAcqirisPeak.h"

#include "TRemiAcqirisChannel.h"
#include "TRemiAcqirisEvent.h"

TRemiAcqirisPeak::TRemiAcqirisPeak(TRemiAcqirisChannel* Channel)
{
	fParentChannel=Channel;
	clear();        
}


TRemiAcqirisPeak::TRemiAcqirisPeak(const TRemiAcqirisPeak& in)
{
	mFwhm = in.mFwhm;
	mPosition = in.mPosition;
	mStartPos = in.mStartPos;
	mStopPos = in.mStopPos;
	mMaximum = in.mMaximum;
	mMaxPos = in.mMaxPos;
	mPolarity = in.mPolarity;
	mUsed = in.mUsed;
	fParentChannel = in.fParentChannel;
}

TRemiAcqirisPeak& TRemiAcqirisPeak::operator=(const TRemiAcqirisPeak& in)
{
	mFwhm = in.mFwhm;
	mPosition = in.mPosition;
	mStartPos = in.mStartPos;
	mStopPos = in.mStopPos;
	mMaximum = in.mMaximum;
	mMaxPos = in.mMaxPos;
	mPolarity = in.mPolarity;
	mUsed = in.mUsed;
	fParentChannel = in.fParentChannel;
	return *this;
}


//______________________________________________________________________________________________________________________
void TRemiAcqirisPeak::clear()
{
	mFwhm = 0;
	mPosition = 0;
	mStartPos = 0;
	mStopPos = 0;
	mMaxPos = 0;
	mMaximum = 0;
	mPolarity = 0;
	mUsed = false;

}

