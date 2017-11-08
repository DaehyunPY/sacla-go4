//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class for several physical particles forming a coincidence
//----------------------------------------------------------------

#include "TRemiCoincidence.h"

#include "TRemiParticle.h"


/**
 * \brief Default constructor
 */
TRemiCoincidence::TRemiCoincidence():
mParticles(),
mIdentifier(0),
mKer(0),
mMomentumSum()
{
}

/**
 * \brief Constructor with identifier and optional size
 */
TRemiCoincidence::TRemiCoincidence(UInt_t Identifier, UInt_t Size):
mParticles(Size),
mIdentifier(Identifier),
mKer(0),
mMomentumSum()
{
	for (UInt_t i=0; i<Size; i++)
		mParticles[i] = mDUMMY;
}

/**
 * \brief Copy constructor
 */
TRemiCoincidence::TRemiCoincidence(const TRemiCoincidence& Coin2):
mParticles(Coin2.mParticles),
mIdentifier(Coin2.mIdentifier),
mKer(Coin2.mKer),
mMomentumSum(Coin2.mMomentumSum)
{
}

/**
 * \brief Sorts the internal vector of pointers to particles
 * 
 * This is a prerequisite for successful comparison of coincidences
 */
void TRemiCoincidence::Sort()
{
	std::sort(mParticles.begin(), mParticles.end());
}


/**
 * \brief Clear the current object
 */
void TRemiCoincidence::Clear()
{
	mParticles.clear();
	mIdentifier = 0;
	mMomentumSum.Null();
}

/**
 * \brief Assignment operator
 */
TRemiCoincidence& TRemiCoincidence::operator=(const TRemiCoincidence& Coin2)
{
	mIdentifier = Coin2.mIdentifier;
	mParticles = Coin2.mParticles;
	mMomentumSum = Coin2.mMomentumSum;
	mKer = Coin2.mKer;
	return *this;
}

/**
 * \brief Test equality of two coincidences
 * 
 * Does a cross check of all particles with all particles of the compared coincidence
 */
Bool_t TRemiCoincidence::operator==(const TRemiCoincidence& Coin2) const
{
	if (Coin2.mIdentifier != mIdentifier)
		return kFALSE; // Coincidences have different identifiers - cannot be identical
	UInt_t Equals = 0;
	for (UInt_t i=0; i<mParticles.size(); i++)
		for (UInt_t j=0; j<mParticles.size(); j++)
			if (mParticles[i].GetTof() == Coin2.mParticles[j].GetTof())
			{
				Equals++;
				break; // Note: breaks only outer loop. This is desired here.
			}
	return (Equals == mParticles.size());
}


/**
 * \brief Access a Particle in the coincidence container
 */
TRemiParticle& TRemiCoincidence::GetParticle(UInt_t i)
{
	if (i < mParticles.size())
		return mParticles[i];
	else
		return mDUMMY;
}

/**
 * \brief Set Particle i
 */
void TRemiCoincidence::SetParticle(UInt_t i, TRemiParticle& Particle)
{
	if (i < mParticles.size())
		mParticles[i] = Particle;
}

/** 
 * \brief Add a new particle to the container
 */
void TRemiCoincidence::AddParticle(TRemiParticle& Particle)
{
	mParticles.push_back(Particle);
}

/**
 * \brief Calculate momentum sum from particles in the coincidence
 */
void TRemiCoincidence::CalculateMomentumSum()
{
	mMomentumSum.Null();
	for (std::vector<TRemiParticle>::iterator PartIt = mParticles.begin(); PartIt != mParticles.end(); ++ PartIt)
		mMomentumSum += PartIt->Mom();
}

/** 
 * \brief Calculate Kinetic energy release
 */
void TRemiCoincidence::CalculateKer()
{
	mKer = 0;
	for (std::vector<TRemiParticle>::iterator PartIt = mParticles.begin(); PartIt != mParticles.end(); ++ PartIt)
		mKer += PartIt->Energy();
}



// Initialize static member
TRemiParticle TRemiCoincidence::mDUMMY = TRemiParticle();

ClassImp(TRemiCoincidence);

