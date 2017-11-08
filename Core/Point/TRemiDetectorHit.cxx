//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class for a detector hit consisting of position and Tof
//----------------------------------------------------------------

#include "TRemiDetectorHit.h"

/**
 * \brief Default constructor
 */
TRemiDetectorHit::TRemiDetectorHit() :
	TRemiXY(),
	mTof(0),
	mIndex(0)
{
}

/**
 * \brief Constructor with position and time initialisation
 */
TRemiDetectorHit::TRemiDetectorHit(Float_t tofValue, Float_t xValue, Float_t yValue) : 
	TRemiXY(xValue, yValue),
	mTof(tofValue),
	mIndex(0)
{
}

/**
* \brief Constructor with position and time initialisation where a TRemiXY object is used
*/
TRemiDetectorHit::TRemiDetectorHit(Float_t tofValue, const TRemiXY &point) : 
TRemiXY(point),
mTof(tofValue),
mIndex(0)
{
}

/**
 *\brief Copy constructor
 */
TRemiDetectorHit::TRemiDetectorHit(const TRemiDetectorHit& hit2): 
TRemiXY(hit2),
mTof(hit2.mTof),
mIndex(hit2.mIndex)
{
}

/**
 * \brief Assignment operator
 */
TRemiDetectorHit& TRemiDetectorHit::operator=(const TRemiDetectorHit& hit2)
{
	mData[0] = hit2.mData[0];
	mData[1] = hit2.mData[1];
	mTof = hit2.mTof;
	mIndex = hit2.mIndex;
	return *this;
}

/** 
* \brief Operator to test equality of two hits
*/
Bool_t TRemiDetectorHit::operator==(const TRemiDetectorHit& hit2)
{
	return (mData[0] == hit2.mData[0]) && (mData[1] == hit2.mData[1]) && (mTof == hit2.mTof);
}

/** 
* \brief Operator to test inequality of two hits
*/
Bool_t TRemiDetectorHit::operator!=(const TRemiDetectorHit& hit2)
{
	return (mData[0] != hit2.mData[0]) || (mData[1] != hit2.mData[1]) || (mTof != hit2.mTof);
}


/**
 * \brief Ordering operator used for sorting
 * 
 * Compares the two time-of-flights
 */
bool TRemiDetectorHit::operator<(const TRemiDetectorHit& rhs) const
{
	return (this->mTof < rhs.GetTof());
}


ClassImp(TRemiDetectorHit);
