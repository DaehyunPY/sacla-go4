//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Generic point vector class
//----------------------------------------------------------------
#ifndef SPOINT_H
#define SPOINT_H

#include "TMath.h"
#include "Rtypes.h"

/** 
 * \brief Universal point vector class
 * 
 * In most parts re-implemented the Point vector class of Gaspard Petit
 * See http://www-etud.iro.umontreal.ca/~petitg/cpp/point.html
 */

template <class Type, Int_t DIM>
class TRemiPoint
{
	public :
		TRemiPoint ();
		TRemiPoint (Type x1, Type x2 = 0, Type x3 = 0, Type x4 = 0, Type x5 = 0);
		TRemiPoint (Type* array);
		TRemiPoint (const TRemiPoint& point2);
		
		virtual ~TRemiPoint ();
		
		TRemiPoint& operator = (const TRemiPoint &point2);
		const Type& operator [] (Int_t index) const;
		
		Bool_t	operator == (const TRemiPoint &point2);
		Bool_t	operator != (const TRemiPoint &point2);
		
		TRemiPoint&	operator += (const TRemiPoint &point2);
		TRemiPoint&	operator -= (const TRemiPoint &point2);
		TRemiPoint	operator + (const TRemiPoint &point2) const;
		TRemiPoint	operator - (const TRemiPoint &point2) const;
		
		TRemiPoint&	operator /= (const Type &scalar);
		TRemiPoint	operator * (const Type &scalar) const;
		TRemiPoint	operator / (const Type &scalar) const;
		
		Type	operator ^ (const TRemiPoint &point2) const;
		
		Type	Length2 () const;
		Type	Length () const;
		
		Type	Distance (const TRemiPoint &point2) const;
		
		void	Null ();
		
		void	Normalize ();
		
	protected :
		Type	mData[DIM]; ///< stores the components of the point vector
		
	ClassDef(TRemiPoint, 1);
};

#include "TRemiPoint.inline.h"
				 
#endif

