//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class for a complete physical particle including the detector hit and the calculated momentum
//----------------------------------------------------------------

#ifndef TREMIPARTICLE_H
#define TREMIPARTICLE_H

#include "TRemiDetectorHit.h"
#include "TRemiPxyz.h"

#include "Rtypes.h"

class TRemiParticle : public TRemiDetectorHit
{
	public: 
		TRemiParticle();
		TRemiParticle(Float_t tofValue, Float_t xValue, Float_t yValue);
		TRemiParticle(const TRemiParticle& part2);
		TRemiParticle(const TRemiDetectorHit &Hit);
		virtual ~TRemiParticle() {}
		
		TRemiParticle&	operator= (const TRemiParticle& part2);
		TRemiParticle&	operator= (const TRemiDetectorHit &Hit);
		
		TRemiPxyz&	Mom()	{ return mMomentum; }	///< Returns reference to the particle's momentum
		TRemiPxyz&	GetMomentum()	{ return mMomentum; }	///< Returns reference to the particle's momentum
		
		UInt_t	GetIdentifier() const	{ return mIdentifier; }
		void SetIdentifier(UInt_t in)	{ mIdentifier = in; }
		
		Float_t	GetMass()	{ return mMass; }	///< Returns Mass of particle
		void SetMass(Float_t in)	{mMass = in; }	///< Set Mass of particle
		
		Float_t	GetEnergy()	{ return mMomentum.Energy(mMass); }
		Float_t	Energy()	{ return mMomentum.Energy(mMass); }
		
	protected:
		TRemiPxyz	mMomentum;	///< Three-vector of the particle's momentum in a.u.
		UInt_t	mIdentifier;	///< Identifying number of the species
		Float_t mMass;	///< Mass of the particle in a.u.
		
	ClassDef(TRemiParticle, 1);
};

#endif // TREMIPARTICLE_H
