//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the header of the actual momentum calculation class
//----------------------------------------------------------------
#ifndef TREMIANLMOMENTUM_H
#define TREMIANLMOMENTUM_H

#include "../Core/TRemiIncludeFunctions.h"
#include "../Core/Point/TRemiParticle.h"

#include "TString.h"

#include <vector>

class TRemiMomentumParameter;
class TRemiDetectorHit;

class ConfigFile;

class TGo4WinCond;
class TGo4PolyCond;

class TH1I;
class TH2I;
class TH3I;


/**
 * \brief Main class to calculate a particle's momentum
*/
class TRemiCalcProcMomentum : protected TRemiIncludeFunctions
{
	public:
		TRemiCalcProcMomentum(ConfigFile *CalcConf, TString Particle, Short_t Index=0, TH1I* HistTof=0);
		TRemiCalcProcMomentum();
		virtual ~TRemiCalcProcMomentum();
		
		void	Clear();
		void	CalculateMomentum (std::vector<TRemiParticle> &Particles, std::vector<TRemiDetectorHit> &Hits);
		void	FillMomentumHistograms ();
		
		TRemiMomentumParameter*	GetParameter ()	{ return fParaMom; }	///< Returns Parameter object of momentum calculator
		
		TString	GetShortName() const	{ return mShortName; }	///< Returns short name of this particle used for object names
		TString	GetLongName() const	{ return mLongName; }	///< Returns long name of this particle used for object description
		UInt_t	GetIdentifier() const	{ return mIdentifier; }	///< Returns numerical identifier of this particle
		Double_t	GetMassAu() const	{ return mMass/mEMASS; } ///< Returns the mass of the associated particle in atomic units
		
		std::vector<TRemiParticle*> &	GetCalculatedParticles()	{ return mCalculatedParticles; }	///< Returns reference to vector of pointers to recently calculated particles
		
	protected:
		TRemiMomentumParameter*	fParaMom;	///< Parameter for momentum calibration and calculation
		
		TH1I*	fPtransX;	///< Histograms to show transverse X momentum
		TH1I*	fPtransY;	///< Histograms to show transverse Y momentum
		TH1I*	fPlong;	///< Histograms for longitudinal Z momentum
		TH2I*	fPlongPtrans;	///< Histograms to show transverse vs. longitudinal momentum
		TH2I*	fPxy;	///< Histograms to show XY momentum
		TH1I*	fEnergy;	///< Histogram to show energy
		TH2I*   fEnergyPlong; ///< Histograms to show energy vs. longitudinal momentum 
		TH2I*   fEnergyPx; ///< Histograms to show energy vs. x momentum
		TH2I*   fEnergyPy; ///< Histograms to show energy vs. y momentum  
		TH2I*   fEnergyPhi; ///< Histograms to show energy vs. momentum azimuthal angle
		TH2I*   fEnergyTheta; ///< Histograms to show energy vs. momentum polar angle
		TH2I*   fEnergyCosTheta; ///< Histograms to show energy vs. cosine of momentum polar angle
		TH2I*   fPtotalPlong; ///< Histograms to show total vs. longitudinal momentum 
		TH2I*   fPtotalPx; ///< Histograms to show total vs. x momentum
		TH2I*   fPtotalPy; ///< Histograms to show total vs. y momentum  
		
		TH1I*	fAnglePolarization;	///< Histogram over the angle between the current momentum and the polarization axis
		
		TH1I*	fNewtonI;	///< Histograms to save number of Newton iterations
		TH2I*	fRad_alpha;	///< Histogram visualising moved portion of cyclotron trajectory
		TH1I*	fRadius;	///< Histogram showing radius of particle on detector (after shifting and scaling)
		TH2I*	fXY_shifted;	///< Position check histogram with applied shifting and scaling
		TH2I*	fXY_ConTof;	///< Position check histogram with applied Tof condition
		TH1I*	fNumParticles;	///< Spectrum for number of particles of this species
		TH2I*   fX_TOF;       ///< Histogram X position vs. TOF to check LinCor of electron wiggles in X direction
		TH2I*   fY_TOF;       ///< Histogram Y position vs. TOF to check LinCor of electron wiggles in Y direction
		
		TH3I* fFull3DMomentum;	///< Histograms for full 3D momentum

		TGo4WinCond*	fConTof;	///< Time-of-flight selection for this species
		TGo4PolyCond*	fConPos;	///< Position selection for this species
		
		TGo4WinCond*	fConRad;	///< Radius selection for centering plots
		
		TGo4WinCond*	fConAnglePol;	///< Condition for the angle with the polarization axis
		TGo4WinCond*	fConEnergy;   ///< Condition on the particle's energy, mainly useful to define different "kinds" of electrons
		
		TGo4WinCond*	fConPx;
		TGo4WinCond*	fConPy;
		TGo4WinCond*	fConPz;
		
		TH2I*	fXY_ConRad;	///< XY position with radius condition
		TH2I*	fXY_centering;	///< XY position with radius condition for centering

		TString	mShortName;	///< Short name to identify particle in internal object namespace
		TString	mLongName;	///< Long name to identify particle in object descriptions
		UInt_t	mIdentifier;	///< Identifier for current species. Identical to the Hash-code of mShortName
		Double_t	mMassMultiplier;	///< Multiplier for the mass of all species in a group. Is normally 1 for electrons and 1822.89 for ions
		
		TRemiParticle	mNewParticle;	///< Temporary particle object used for new calculation
		
		// Constants for this particle
		Double_t mCharge;	///< Charge value in SI
		Double_t mAbsCharge;	///< Absolute charge value in SI
		Double_t mMass;	///< Mass value in SI
		Int_t mAccelDirection;	///< Acceleration direction
		
		// Constants for all momentum calculators
		static const Double_t mEMASS;	///< Electron rest mass
		static const Double_t mECHARGE;	///< Elementary charge
		static const Double_t mAUP;		///< Momentum conversion a.u. into SI
		static const Double_t mNANO;	///< Alias for 1e-9
		static const Double_t mMILLI;	///< Alias for 1e-3
		static const Double_t mGS2T;	///< Alias for conversion Gauss to Tesla (1e-4)
		
		std::vector<TRemiParticle*>	mCalculatedParticles;	///< vector with pointers to the recently calculated particles
		
		TRemiPxyz	mPolarizationAxis;	///< Polarization vector
	
	ClassDef(TRemiCalcProcMomentum, 1);
};


#endif
