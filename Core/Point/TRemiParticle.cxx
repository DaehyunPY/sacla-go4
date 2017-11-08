#include "TRemiParticle.h"

/**
* \brief Default constructor
*/
TRemiParticle::TRemiParticle() :
TRemiDetectorHit(),
mMomentum(),
mIdentifier(0),
mMass(1)
{
}

/**
* \brief Constructor with position and time initialisation
*/
TRemiParticle::TRemiParticle(Float_t tofValue, Float_t xValue, Float_t yValue) : 
TRemiDetectorHit(tofValue, xValue, yValue),
mMomentum(),
mIdentifier(0),
mMass(1)
{
}

/**
 * \brief Constructor using a TRemiDetectorHit as input
 */
TRemiParticle::TRemiParticle(const TRemiDetectorHit& Hit): 
TRemiDetectorHit(Hit),
mMomentum(),
mIdentifier(0),
mMass(1)
{
}

/** 
 * \brief Copy constructor
 */
TRemiParticle::TRemiParticle(const TRemiParticle& part2): 
TRemiDetectorHit(part2),
mMomentum(part2.mMomentum),
mIdentifier(part2.mIdentifier),
mMass(part2.mMass)
{
}

/**
 * \brief Assignment operator
 */
TRemiParticle& TRemiParticle::operator=(const TRemiParticle& part2)
{
	mTof = part2.mTof;
	mData[0] = part2[0];
	mData[1] = part2[1];
	mIndex = part2.mIndex;
	mIdentifier = part2.mIdentifier;
	mMass = part2.mMass;
	mMomentum = part2.mMomentum;
	return *this;
}


/**
* \brief Assignment operator using a TRemiDetectorHit as input
*/
TRemiParticle& TRemiParticle::operator=(const TRemiDetectorHit& Hit)
{
	mTof = Hit.GetTof();
	mData[0] = Hit[0];
	mData[1] = Hit[1];
	mIndex = Hit.GetIndex();
	
	// reset all momentum related things
	mIdentifier = 0;
	mMass = 1;
	mMomentum.SetXYZ(0,0,0);
	return *this;
}

ClassImp(TRemiParticle);