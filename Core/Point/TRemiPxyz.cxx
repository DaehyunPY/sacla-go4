//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Three-component momentum vector class
//----------------------------------------------------------------
#include "TRemiPxyz.h"
#include "TMath.h"

/**
 * \brief Default constructor
 */
TRemiPxyz::TRemiPxyz ()
	: TRemiPoint<Float_t, 3> ()
{
} 

/**
 * \brief Create new 3D object with coordinates xvalue, yvalue and zvalue
 */
TRemiPxyz::TRemiPxyz (Float_t xvalue, Float_t yvalue, Float_t zvalue) 
	: TRemiPoint<Float_t, 3> (xvalue, yvalue, zvalue)
{
} 

/**
* \brief Copy constructor
*/
TRemiPxyz::TRemiPxyz(const TRemiPxyz& point2): TRemiPoint< Float_t, 3 >(point2)
{
}

/**
* \brief Copy constructor for upwards compability
*/
TRemiPxyz::TRemiPxyz(const TRemiPoint<Float_t, 3 >& point2): TRemiPoint< Float_t, 3 >(point2)
{
}

/**
 * \brief Destructor
 */
TRemiPxyz::~TRemiPxyz()
{
}


/**
* \brief Assigment operator for upwards compability
* 
* This operator allows to add etc. two momentum vectors and assign them to a third momentum vector using the generic operator definitions of TRemiPoint
*/
TRemiPxyz& TRemiPxyz::operator= (const TRemiPoint<Float_t, 3> &point2)
{
	mData[0] = point2[0];
	mData[1] = point2[1];
	mData[2] = point2[2];
	return *this;
}

/**
* \brief Assigment operator
*/
TRemiPxyz& TRemiPxyz::operator= (const TRemiPxyz &point2)
{
	mData[0] = point2[0];
	mData[1] = point2[1];
	mData[2] = point2[2];
	return *this;
}

/**
 * \brief Vector product of two point vectors
 */
TRemiPxyz TRemiPxyz::operator % (const TRemiPxyz &point2) const
{
	TRemiPxyz newpt;
	newpt.mData[0] = mData[1] * point2.mData[2] - mData[2] * point2.mData[1];
	newpt.mData[1] = mData[2] * point2.mData[0] - mData[0] * point2.mData[2];
	newpt.mData[2] = mData[0] * point2.mData[1] - mData[1] * point2.mData[0];
	return newpt;
}

/**
 * \brief Returns X position 
 */
Float_t TRemiPxyz::X () const
{ 
	return mData[0]; 
} 

/**
 * \brief Returns Y position 
 */
Float_t TRemiPxyz::Y () const
{ 
	return mData[1]; 
} 

/**
 * \brief Returns Z position 
 */
Float_t TRemiPxyz::Z () const
{ 
	return mData[2]; 
} 

/**
 * \brief Sets X position
 */
void TRemiPxyz::SetX (Float_t xvalue) 
{ 
	mData[0] = xvalue; 
} 

/**
 * \brief Sets Y position
 */
void TRemiPxyz::SetY (Float_t yvalue)
{ 
	mData[1] = yvalue; 
}  

/**
 * \brief Sets Z position
 */
void TRemiPxyz::SetZ (Float_t zvalue)
{ 
	mData[2] = zvalue; 
}  

/**
 * \brief Simultaneously sets X, Y and Z values
 */
void TRemiPxyz::SetXYZ (Float_t xvalue, Float_t yvalue, Float_t zvalue)
{
	mData[0] = xvalue;
	mData[1] = yvalue;
	mData[2] = zvalue;
}

/**
 * \brief Sets the vector values from cylindric coordinate input
 * \param rhovalue radial component in xy plane
 * \param phivalue azimuthal angle in degrees!
 * \param zvalue Z component
 */
void TRemiPxyz::SetRhoPhiZ (Float_t rhovalue, Float_t phivalue, Float_t zvalue)
{
	phivalue *= TMath::DegToRad();
	mData[0] = TMath::Cos(phivalue) * rhovalue;
	mData[1] = TMath::Sin(phivalue) * rhovalue;
	mData[2] = zvalue;
}

/**
 * \brief Sets the vector values from spherical coordinate input
 * \param phivalue Azimuthal angle of the vector
 * \param thetavalue Polar angle of the vector
 * \param rvalue Radius (Length) of the vector
 */
void TRemiPxyz::SetPhiThetaR (Float_t phivalue, Float_t thetavalue, Float_t rvalue)
{
	phivalue *= TMath::DegToRad();
	thetavalue *= TMath::DegToRad();
	mData[0] = TMath::Sin(thetavalue) * TMath::Cos(phivalue) * rvalue;
	mData[1] = TMath::Sin(thetavalue) * TMath::Sin(phivalue) * rvalue;
	mData[2] = TMath::Cos(thetavalue) * rvalue;
}

/**
 * \brief Rotate around x axis
 * \param angle in degrees!
 */
void TRemiPxyz::RotateX(Float_t angle)
{
	angle *= TMath::DegToRad();
	Float_t tempy = mData[1];
	mData[1] = tempy * TMath::Cos(angle) - mData[2] * TMath::Sin(angle); // y'= y*cos(a) - z*sin(a)
	mData[2] = tempy * TMath::Sin(angle) + mData[2] * TMath::Cos(angle); // z'= y*sin(a) + z*cos(a)
}

/**
 * \brief Rotate around y axis
 * \param angle in degrees!
 */
void TRemiPxyz::RotateY(Float_t angle)
{
	angle *= TMath::DegToRad();
	Float_t tempz = mData[2];
	mData[2] = tempz * TMath::Cos(angle) - mData[0] * TMath::Sin(angle); // z'= z*cos(a) - x*sin(a)
	mData[0] = tempz * TMath::Sin(angle) + mData[0] * TMath::Cos(angle); // x'= z*sin(a) + x*cos(a)
}

/**
 * \brief Rotate around z axis
 * \param angle in degrees!
 */
void TRemiPxyz::RotateZ(Float_t angle)
{
	angle *= TMath::DegToRad();
	Float_t tempx = mData[0];
	mData[0] = tempx * TMath::Cos(angle) - mData[1] * TMath::Sin(angle); // x'= x*cos(a) - y*sin(a)
	mData[1] = tempx * TMath::Sin(angle) + mData[1] * TMath::Cos(angle); // y'= x*sin(a) + y*cos(a)
}

/**
 * \brief Rotates around arbitrary axis
 * \param axis 3-dimensional TRemiPoint vector that points in the direction of the rotation axis. This can be a TRemiPxyz momentum object
 * \param angle in degrees!
 */
void TRemiPxyz::Rotate(TRemiPoint<Float_t, 3> axis, Float_t angle)
{
	angle *= TMath::DegToRad();
	axis.Normalize();
	Float_t tempx = mData[0];
	Float_t tempy = mData[1];
	Float_t tempz = mData[2];
	
	Float_t cosa = TMath::Cos(angle);
	Float_t sina = TMath::Sin(angle);
	
	mData[0] = tempx * ( cosa + axis[0]*axis[0]*(1.-cosa) ) + tempy * ( axis[0]*axis[1]*(1.-cosa) - axis[2]*sina ) + tempz * ( axis[0]*axis[2]*(1.-cosa) + axis[1]*sina);
	mData[1] = tempx * ( axis[1]*axis[0]*(1.-cosa) + axis[2]*sina ) + tempy * ( cosa + axis[1]*axis[1]*(1.-cosa) ) + tempz * ( axis[1]*axis[2]*(1.-cosa) - axis[0]*sina);
	mData[2] = tempx * ( axis[2]*axis[0]*(1.-cosa) - axis[1]*sina ) + tempy * ( axis[2]*axis[1]*(1.-cosa) + axis[0]*sina ) + tempz * ( cosa + axis[2]*axis[2]*(1.-cosa) );
}

/**
 * \brief Returns angle with another object
 */
Float_t TRemiPxyz::Angle(const TRemiPoint<Float_t, 3> &point2) const
{
	if ( (Length2()!=0) && (point2.Length2()!=0) )
	{
		Float_t scalar = (point2 ^ *this) / (Length() * point2.Length());
		return (TMath::RadToDeg() * TMath::ACos(scalar));
	}
	else
		return 0;
}

/**
 * \brief Returns azimuthal angle
 * \return Phi in degrees ranging from 0 to 360
 */
Float_t TRemiPxyz::Phi () const
{ 
	// Root function TMath::ATan2 already checks for division by 0 and spreads result over 2 pi
	Float_t phi = TMath::RadToDeg() * TMath::ATan2 (mData[1], mData[0]); 
	// Only the negative half has to be shifted 
	if (phi<0)
		phi += 360;
	return phi; 
} 

/**
 * \brief Returns azimuthal angle for a system symmetric to the xz and yz plane
 * \return Angle ranges from 0 to 90 deg
 */
Float_t TRemiPxyz::PhiSym() const
{
	if (!mData[0])
		return 90;
	Float_t phi = TMath::RadToDeg() * TMath::ATan (mData[1]/mData[0]); 
	// Only the negative half has to be mirrored
	return  (phi>0) ? phi : -1.*phi;
}

/**
 * \brief Returns polar angle
 * \return Theta in degrees ranging from 0 to 180
 */
Float_t TRemiPxyz::Theta () const
{ 
	if (!mData[2]) // z = 0
		return 90;
	Float_t theta = TMath::RadToDeg() * TMath::ATan(Rho()/mData[2]);
	// Shift the negative half to values from 90 to 180
	return (theta>0) ? theta : 180. + theta;
} 

/**
 * \brief Returns polar angle over the hole surface
 * \return Angle ranges from 0 to 360 deg
 */
Float_t TRemiPxyz::Theta2Pi () const
{ 
	return (mData[0] > 0) ? Theta() : 360-Theta(); // theta becomes > 180 for negative x
} 

/**
 * \brief Returns polar angle when assuming system to be symmetric with respect to the xy-plane
 * \return Angle ranges from 0 to 90 deg
 */
Float_t TRemiPxyz::ThetaSym () const
{ 
	if (!mData[2]) // z = 0
		return 90;
	Float_t theta = TMath::RadToDeg() * TMath::ATan(Rho()/mData[2]);
	// Mirror the negative half to values
	return (theta>0) ? theta : -1. * theta;
} 


/**
 * \brief Returns xy-planar radius
 */
Float_t TRemiPxyz::Rho () const
{ 
	return TMath::Sqrt(mData[0]*mData[0] + mData[1]*mData[1]); 
} 

/**
 * \brief Returns energy of the particle in eV
 * \param mass (optional) Mass of the particle in a.u. To be used to get the energy for any particle with mass unequal 1
 */
Float_t TRemiPxyz::Energy(Float_t mass) const
{
/*	return (mEnergy/mass);*/
	return (Length2() * 13.606 / mass);
}

/**
 * \brief Returns the modulus of the angle (in degrees) to the xz-plane (scattering plane)
 */
Float_t TRemiPxyz::OutOfScatPlane() const
{
	Float_t help = TMath::Sqrt(mData[0]*mData[0] + mData[2]*mData[2]); // sqrt (x^2 + z^2)
	help = TMath::ATan ( mData[1]/help) * TMath::RadToDeg(); // atan (y/help)
	return TMath::Abs (help);
}

ClassImp(TRemiPxyz);


