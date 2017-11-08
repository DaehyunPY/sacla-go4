//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class for two-dimensional position vectors
//----------------------------------------------------------------

#include "TRemiXY.h"
#include "TMath.h"

/**
 * \brief Default constructor
 */
TRemiXY::TRemiXY ()
	: TRemiPoint<Float_t, 2> ()
{
} 

/**
 * \brief Create new 2D object with coordinates xvalue and yvalue 
 */
TRemiXY::TRemiXY (Float_t xvalue, Float_t yvalue) 
	: TRemiPoint<Float_t, 2> (xvalue, yvalue)
{
}

/**
 * \brief Copy constructor
 */
TRemiXY::TRemiXY(const TRemiXY& point2): TRemiPoint< Float_t, 2 >(point2)
{
}

/**
 * \brief Copy constructor for upwards compability
 */
TRemiXY::TRemiXY(const TRemiPoint<Float_t, 2 >& point2): TRemiPoint< Float_t, 2 >(point2)
{
}


/**
 * \brief Destructor
 */
TRemiXY::~TRemiXY()
{
}


/**
 * \brief Assigment operator for upwards compability
 *
 * Through this operator additions etc. of two TRemiXY objects can again be assigned to a TRemiXY object
 */
TRemiXY& TRemiXY::operator= (const TRemiPoint<Float_t, 2> &point2)
{
	mData[0] = point2[0];
	mData[1] = point2[1];
	return *(this);
}

/**
 * \brief Returns X position 
 */
Float_t TRemiXY::X () const
{ 
	return mData[0]; 
} 

/**
 * \brief Returns Y position 
 */
Float_t TRemiXY::Y () const
{ 
	return mData[1]; 
} 

/**
 * \brief Rotate in XY plane
 * \param angle in degrees!
 */
void TRemiXY::Rotate(Float_t angle)
{
	angle *= TMath::DegToRad();
	Float_t tempx = mData[0];
	mData[0] = tempx * TMath::Cos(angle) - mData[1] * TMath::Sin(angle); // x'= x*cos(a) - y*sin(a)
	mData[1] = tempx * TMath::Sin(angle) + mData[1] * TMath::Cos(angle); // y'= x*sin(a) + y*cos(a)
}

/**
 * \brief Sets X position
 */
void TRemiXY::SetX (Float_t xvalue) 
{ 
	mData[0] = xvalue; 
} 

/**
 * \brief Sets Y position
 */
void TRemiXY::SetY (Float_t yvalue)
{ 
	mData[1] = yvalue; 
}  

/**
 * \brief Sets both X and y position
 */
void TRemiXY::SetXY (Float_t xvalue, Float_t yvalue)
{
	mData[0] = xvalue;
	mData[1] = yvalue;
}

/**
 * \brief Set vector in polar coordinates
 */
void TRemiXY::SetPhiR(Float_t phivalue, Float_t rvalue)
{
	mData[0] = rvalue * TMath::Cos(phivalue);
	mData[1] = rvalue * TMath::Sin(phivalue);
}


/**
 * \brief Returns azimuthal angle
 * \return Phi in degrees
 */
Float_t TRemiXY::Phi () const
{ 
	// Root function TMath::ATan2 already checks for division by 0 and spreads result over 2 pi
	Float_t phi = TMath::RadToDeg() * TMath::ATan2 (mData[1], mData[0]); 
	// Only the negative half has to be shifted 
	if (phi<0)
		phi += 360;
	return phi; 
} 

/**
 * \brief Returns radius
 */
Float_t TRemiXY::R () const
{ 
	return Length(); 
} 

ClassImp(TRemiXY);
