//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Three-component momentum vector class
//----------------------------------------------------------------
#ifndef SPXYZ_H
#define SPXYZ_H

#include "TRemiPoint.h"

/**
 * \brief Class to store components of the 3d momentum vector and their angles
 */
class TRemiPxyz : public TRemiPoint<Float_t, 3>
{
	public :
		TRemiPxyz ();
		TRemiPxyz (Float_t xvalue, Float_t yvalue, Float_t zvalue);
		TRemiPxyz (const TRemiPxyz &point2);
		TRemiPxyz (const TRemiPoint<Float_t, 3> & point2);
		
		virtual ~TRemiPxyz();
		
		TRemiPxyz& operator= (const TRemiPoint<Float_t, 3> &xyz2);
		TRemiPxyz& operator= (const TRemiPxyz &xyz2);
		TRemiPxyz operator % (const TRemiPxyz &point2) const;
				
		Float_t X () const;
		Float_t Y () const; 
		Float_t Z () const; 
		
		void SetX (Float_t xvalue);
		void SetY (Float_t yvalue);
		void SetZ (Float_t yvalue);
		
		void SetXYZ (Float_t xvalue, Float_t yvalue, Float_t zvalue);
		void SetRhoPhiZ (Float_t rhovalue, Float_t phivalue, Float_t zvalue);
		void SetPhiThetaR (Float_t phivalue, Float_t thetavalue, Float_t rvalue);
		
		void RotateX (Float_t angle);
		void RotateY (Float_t angle);
		void RotateZ (Float_t angle);
		
		void Rotate (TRemiPoint<Float_t, 3> axis, Float_t angle);
		
		Float_t Angle (const TRemiPoint<Float_t, 3> &point2) const;
		
		Float_t Phi () const;
		Float_t PhiSym () const;
		Float_t Theta () const;
		Float_t Theta2Pi () const;
		Float_t ThetaSym () const;
		Float_t Rho () const;
		Float_t Energy (Float_t mass = 1) const;
		Float_t OutOfScatPlane () const;
		
	private :

	ClassDef(TRemiPxyz, 1);
};

#endif
