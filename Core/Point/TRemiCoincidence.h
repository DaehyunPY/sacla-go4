//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class for several physical particles forming a coincidence
//----------------------------------------------------------------

#ifndef TREMICOINCIDENCE_H
#define TREMICOINCIDENCE_H

class TRemiParticle;
#include "TRemiPxyz.h"

#include "Rtypes.h"

#include <vector>

class TRemiCoincidence
{
	public:
		TRemiCoincidence ();
		TRemiCoincidence (UInt_t Identifier, UInt_t Size=0);
		TRemiCoincidence (const TRemiCoincidence& Coin2);
		
		virtual ~TRemiCoincidence () {}
		
		void Clear();
		void Sort();
		
		TRemiCoincidence&	operator = (const TRemiCoincidence& Coin2);
		Bool_t	operator == (const TRemiCoincidence& Coin2) const;
		
		UInt_t	GetIdentifier () const	{ return mIdentifier; }
		void SetIdentifier ( UInt_t in)	{ mIdentifier = in; }
		
		TRemiParticle&	GetParticle (UInt_t i);
		void 	SetParticle (UInt_t i, TRemiParticle& Particle);
		void 	AddParticle (TRemiParticle& Particle);
		
		TRemiPxyz&	GetMomentumSum()	{ return mMomentumSum; }
		void	SetMomentumSum(const TRemiPxyz& in)	{ mMomentumSum = in; }
		void	CalculateMomentumSum();
		
		Float_t	GetKer()	{ return mKer; }
		void	SetKer(Float_t in)	{ mKer = in; }
		void	CalculateKer();
		
		UInt_t	GetSize()	{ return mParticles.size(); }	///< Returns number of particles in the coincidence container
		
	protected:
		std::vector<TRemiParticle>	mParticles;	///< Pointers to particles forming this coincidence
		UInt_t	mIdentifier;	///< Hash code to identify the coincidence channel this object belongs to
		Float_t	mKer;	///< Total kinetic energy release of the reaction
		TRemiPxyz	mMomentumSum;	///< Momentum sum of all particles in the coincidence
		
		static TRemiParticle mDUMMY;	///< Dummy particle used to return references to by GetParticle() if no actual pointer to an object is saved
		
		ClassDef(TRemiCoincidence, 1);
};

#endif // TREMICOINCIDENCE_H
