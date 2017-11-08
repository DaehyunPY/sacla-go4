//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Generic point vector class - Inline file that contains declarations
//----------------------------------------------------------------
#ifndef SPOINT_INLINE_H
#define SPOINT_INLINE_H

#include "TRemiPoint.h"

/**
 * \brief Default constructor
 */
template <class Type, Int_t DIM>
		TRemiPoint<Type, DIM>::TRemiPoint ()
{
	Int_t i;
	for (i=0; i<DIM; i++)
		mData[i] = 0;
}

/** 
 * \brief Constructor that allows to initialise up to five elements. 
 */
template <class Type, Int_t DIM>
		TRemiPoint<Type, DIM>::TRemiPoint (Type x1, Type x2, Type x3, Type x4, Type x5)
{
	if (DIM >= 1) mData[0] = x1;
	if (DIM >= 2) mData[1] = x2;
	if (DIM >= 3) mData[2] = x3;
	if (DIM >= 4) mData[3] = x4;
	if (DIM >= 5) mData[4] = x5;
	if (DIM > 5)
	{
		Int_t i;
		for (i=5; i<DIM; i++)
			mData[i] = 0;
	}
}

/**
 * \brief Constructor from C array with DIM elements
 */
template <class Type, Int_t DIM>
		TRemiPoint<Type, DIM>::TRemiPoint (Type* array)
{
	memcpy(mData, array, sizeof(Type)*DIM);
}

/**
 * \brief Copy constructor
 */
template <class Type, Int_t DIM>
		TRemiPoint<Type, DIM>::TRemiPoint (const TRemiPoint<Type, DIM> &point2)
{
	memcpy(mData, point2.mData, sizeof(Type)*DIM);
}

/**
 * \brief Destructor
 */
template <class Type, Int_t DIM>
		TRemiPoint<Type, DIM>::~TRemiPoint<Type,DIM>()
{}

/**
 * \brief Assignment operator
 */
template <class Type, Int_t DIM>
		TRemiPoint<Type, DIM>& TRemiPoint<Type, DIM>::operator = (const TRemiPoint<Type, DIM> &point2)
{
	memcpy(mData, point2.mData, sizeof(Type)*DIM);
	return *this;
}

/** 
 * \brief Returns a single co-ordinate of the vector
 */
template <class Type, Int_t DIM>
		const Type& TRemiPoint<Type, DIM>::operator [] (Int_t index) const
{
	return mData[index];
}

/**
* \brief Test equality of two point vectors
*/
template <class Type, Int_t DIM>
Bool_t TRemiPoint<Type, DIM>::operator == (const TRemiPoint<Type, DIM> &point2)
{
	return (memcmp(mData, point2.mData, sizeof(Type)*DIM) == 0);
}

/**
* \brief Test inequality of two point vectors
*/
template <class Type, Int_t DIM>
Bool_t TRemiPoint<Type, DIM>::operator != (const TRemiPoint<Type, DIM> &point2)
{
	return (memcmp(mData, point2.mData, sizeof(Type)*DIM) != 0);
}

/**
 * \brief Add another point (vector) to current point
 */
template<class Type, Int_t DIM>
		TRemiPoint<Type, DIM>& TRemiPoint<Type, DIM>::operator += (const TRemiPoint<Type, DIM> &point2)
{
	Int_t i;
	for (i=0; i<DIM; i++)
		mData[i] += point2[i];
	return *this;
}

/**
 * \brief Substract another point (vector) from current point
 */
template<class Type, Int_t DIM>
		TRemiPoint<Type, DIM>& TRemiPoint<Type, DIM>::operator -= (const TRemiPoint<Type, DIM> &point2)
{
	Int_t i;
	for (i=0; i<DIM; i++)
		mData[i] -= point2[i];
	return *this;
}

/** 
 * \brief Addition of two points 
 */
template<class Type, Int_t DIM>
		TRemiPoint<Type, DIM> TRemiPoint<Type, DIM>::operator + (const TRemiPoint<Type, DIM> &point2) const
{
	TRemiPoint<Type, DIM> newpt;
	Int_t i;
	for (i=0; i<DIM; i++)
		newpt.mData[i] = mData[i] + point2.mData[i];
	return newpt;
}

/** 
 * \brief Substraction of two points 
 */
template<class Type, Int_t DIM>
		TRemiPoint<Type, DIM> TRemiPoint<Type, DIM>::operator - (const TRemiPoint<Type, DIM> &point2) const
{
	TRemiPoint<Type, DIM> newpt;
	Int_t i;
	for (i=0; i<DIM; i++)
		newpt.mData[i] = mData[i] - point2.mData[i];
	return newpt;
}

/**
 * \brief Divide current point vector by a scalar
 */
template<class Type, Int_t DIM>
		TRemiPoint<Type, DIM>& TRemiPoint<Type, DIM>::operator /= (const Type &scalar)
{
	Int_t i;
	for (i=0; i<DIM; i++)
		mData[i] /= scalar;
	return *this;
}

/**
 * \brief Returns current point vector multiplied by a scalar
 */
template<class Type, Int_t DIM>
		TRemiPoint<Type, DIM> TRemiPoint<Type, DIM>::operator * (const Type &scalar) const
{
	TRemiPoint<Type, DIM> newpt = *this;
	Int_t i;
	for (i=0; i<DIM; i++)
		newpt.mData[i] *= scalar;
	return newpt;
}

/**
 * \brief Returns current point vector divided by a scalar
 */
template<class Type, Int_t DIM>
		TRemiPoint<Type, DIM> TRemiPoint<Type, DIM>::operator / (const Type &scalar) const
{
	TRemiPoint<Type, DIM> newpt = *this;
	return (newpt /= scalar);
}

/**
 * \brief Scalar product of two point vectors
 */
template<class Type, Int_t DIM>
		Type TRemiPoint<Type, DIM>::operator ^  (const TRemiPoint<Type, DIM> &point2) const
{
	Type scalar=0;
	Int_t i;
	for (i=0; i<DIM; i++)
		scalar += mData[i] * point2.mData[i];
	return scalar;
}


/** 
 * \brief Sets the point vector to null
 */
template<class Type, Int_t DIM>
		void TRemiPoint<Type, DIM>::Null()
{
	memset (mData, 0, sizeof(Type)*DIM);
}

/**
 * \brief Returns squared length of the point vector
 */
template<class Type, Int_t DIM>
		Type TRemiPoint<Type, DIM>::Length2() const
{
	Type length2 = 0;
	Int_t i;
	for (i=0; i<DIM; i++)
		length2 += mData[i]*mData[i];
	return length2;
}

/**
 * \brief Returns length of the point vector (also referred to as 'Radius')
 */
template<class Type, Int_t DIM>
		Type TRemiPoint<Type, DIM>::Length() const
{
	return sqrt (Length2());
}

/**
 * \brief Returns distance to a second point
 */
template<class Type, Int_t DIM>
		Type TRemiPoint<Type, DIM>::Distance( const TRemiPoint<Type, DIM> &point2 ) const
{
	TRemiPoint<Type, DIM> newpt;
 	newpt = point2 - *this;
	return newpt.Length();
}

/**
 * \brief Normalizes the vector to a length of 1
 */
template<class Type, Int_t DIM>
		void TRemiPoint<Type, DIM>::Normalize ()
{
	*this /= this->Length();
}


#endif
