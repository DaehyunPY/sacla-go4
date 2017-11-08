//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class for two-dimensional position vectors
//----------------------------------------------------------------

#ifndef SXY_H
#define SXY_H

#include "TRemiPoint.h"

/**
 * \brief Simple class to create objects with X and Y properties
 */
class TRemiXY : public TRemiPoint<Float_t, 2>
{
	public :
		TRemiXY ();
		TRemiXY (Float_t xvalue, Float_t yvalue);
		TRemiXY (const TRemiXY& point2);
		TRemiXY (const TRemiPoint<Float_t, 2> &point2);
		
		virtual ~TRemiXY();
		
		TRemiXY& operator= (const TRemiPoint<Float_t, 2> &xy2);
				
		Float_t X () const;
		Float_t Y () const; 
		
		void Rotate (Float_t angle);
		
		void SetX (Float_t xvalue);
		void SetY (Float_t yvalue);
		void SetXY (Float_t xvalue, Float_t yvalue);
		void SetPhiR (Float_t phivalue, Float_t rvalue);
		
		Float_t Phi () const;
		Float_t R () const;
		
	ClassDef(TRemiXY, 1);
};

#endif
