//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class for a detector hit consisting of position and Tof
//----------------------------------------------------------------

#ifndef TREMIDETECTORHIT_H
#define TREMIDETECTORHIT_H

#include "TRemiXY.h"

#include "Rtypes.h"

class TRemiDetectorHit : public TRemiXY
{
	public:
		TRemiDetectorHit();
		TRemiDetectorHit(Float_t tofValue, Float_t xValue, Float_t yValue);
		TRemiDetectorHit(const TRemiDetectorHit& hit2);
		TRemiDetectorHit(Float_t tofValue, const TRemiXY& point);
		virtual ~TRemiDetectorHit() {}
		
		TRemiDetectorHit&	operator = (const TRemiDetectorHit& hit2);
		
		Bool_t	operator == (const TRemiDetectorHit& hit2);
		Bool_t	operator != (const TRemiDetectorHit& hit2);
		
		const Float_t	GetTof() const	{ return mTof; }	///< Returns value of Tof
		const Float_t	Tof() const	{ return mTof; }	///< Returns value of Tof
		void	SetTof(Float_t tofValue)	{ mTof = tofValue; } ///< Set value of Tof
		
		const UShort_t	GetIndex() const	{ return mIndex; }	///< Returns value of Index
		const UShort_t	Index() const	{ return mIndex; }	///< Returns value of Index
		void	SetIndex(UShort_t indexValue)	{ mIndex = indexValue; } ///< Set value of Index

		bool operator< (const TRemiDetectorHit& Hit2) const;
		
	protected:
		Float_t mTof;	///< Saves the time-of-flight
		UShort_t	mIndex;	///< Index of hit in current Event
		
	ClassDef(TRemiDetectorHit, 1);
};

#endif // TREMIDETECTORHIT_H
