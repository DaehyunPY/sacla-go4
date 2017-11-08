//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class to process a coincidence channel
//----------------------------------------------------------------

#ifndef TREMICALCPROCCOINCIDENCES_H
#define TREMICALCPROCCOINCIDENCES_H

#include "../Core/Point/TRemiPxyz.h"
#include "../Core/TRemiIncludeFunctions.h"

#include "TString.h"
#include "Rtypes.h"

#include <vector>

class TRemiCalcProcMomentum;
class TRemiCoincidence;
class TRemiParticle;

class ConfigFile;

class TGo4WinCond;
class TGo4Picture;

class TH1I;
class TH2I;
class TH3I;


class TRemiCalcProcCoincidences : public TRemiIncludeFunctions
{
	public :
		TRemiCalcProcCoincidences();
		TRemiCalcProcCoincidences(ConfigFile* CalcConf, std::vector<TRemiCalcProcMomentum> &MomentumCalculators, Short_t Index=0);
		
		virtual ~TRemiCalcProcCoincidences() {}
		
		void Clear();
		
		void FindCoincidences (std::vector<TRemiCoincidence> &Coincidences);
		
		void FillCoincidenceHistograms();
		
		TString	GetLongName() const	{ return mLongName; }
		TString GetShortName() const	{ return mShortName; }
		UInt_t GetIdentifier() const	{ return mIdentifier; }
		UInt_t GetNumParticles() const	{ return mNumParticles; }
		
		TString GetParticleName(UInt_t index) const	{ return (index<mNumParticles) ? mParticleNames[index] : TString(); } 
		
		TRemiCalcProcMomentum* GetMomentumCalculator(UInt_t index) const	{ return (index<mNumParticles) ? fMomentumCalculators[index] : 0; }
		
		std::vector<TRemiCoincidence*> &	GetFoundCoincidences()	{ return mFoundCoincidences; }
		
	private :
		Bool_t GetNextParticle (std::vector< std::vector<TRemiParticle*>::iterator > &PartIts, UInt_t Level);
		Bool_t CheckUnique(std::vector< std::vector<TRemiParticle*>::iterator > &PartIts, UInt_t Level);
		
		void InitNewtonPlots(ConfigFile* CalcConf);
		void FillCoincidenceNewtonPlots(TRemiCoincidence& ThreeParticles);
		void FillParticleNewtonPlots(Int_t Index, TRemiParticle Fixed, TRemiParticle Plotted1, TRemiParticle Plotted2);
		
		void InitDalitzPlots(ConfigFile* CalcConf);
		void FillCoincidenceDalitzPlots(TRemiCoincidence& ThreeParticles);
		void FillParticleDalitzPlots(Int_t Index, TRemiParticle Particle1, TRemiParticle Particle2, TRemiParticle Particle3);
		
		/** \name General properties */
		//@{
		TString	mShortName;	///< Short name identifying this channel
		TString	mLongName;	///< Long name (description) of this channel
		UInt_t	mIdentifier;	///< Hash code identifying this channel
		//@}
		
		/** \name Properties of the constituent particles */
		//@{
		UInt_t	mNumParticles;	///< Number of particles in this coincidence channel
		std::vector<TString>	mParticleNames;	///< Short names of the different species in this channel
		std::vector<UInt_t>	mParticleIdentifiers;	///< Hash codes of the species' short names
		//@}
		
		/** \name Momentum sum conditions */
		//@{
		TGo4WinCond*	fConPxSum;	///< X Momentum sum condition
		TGo4WinCond*	fConPySum;	///< Y Momentum sum condition
		TGo4WinCond*	fConPzSum;	///< Z Momentum sum condition
		TGo4WinCond*	fConKer;	///< Condition on kinetic energy release
		//@}
		
		/** \name Spectra and picture showing momentum sums */
		//@{
		TH1I*	fHistPzSum;	///< Histogram for longitudinal momentum sum
		TH1I*	fHistPxSum;	///< Histogram for x-momentum sum
		TH1I*	fHistPySum;	///< Histogram for y-momentum sum
		TGo4Picture*	fPicSums;	///< Picture showing the three momentum sums
		TH1I*	fHistKer;	///< Histogram for total kinetic energy release
		//@}
		
		/** \name Spectra and pictures showing momentum sums vs. individual momentum components */
		//@{
		std::vector<TH2I*>	fHistPxSumVsPx;	///< Spectra showing spectra of the X momentum sum vs. X momentum components
		TGo4Picture*	fPicPxSum;	///< Picture showing spectra of the X momentum sum vs. X momentum components of the particles
		std::vector<TH2I*>	fHistPxVsOther;	///< Spectra showing the X momentum of one particle vs the sum of other particles in the coincidence
		TGo4Picture*	fPicPxVsOther;	///< Picture showing the spectra of X momenta vs. some of other X momenta
		std::vector<TGo4Picture*>	fPicPartSumX;	///< Picture showing X momentum sum related information for each particle
		std::vector<TH2I*>	fHistPySumVsPy;	/// Spectra showing spectra of the Y momentum sum vs. Y momentum components
		TGo4Picture*	fPicPySum;	///< Picture showing spectra of the Y momentum sum vs. Y momentum components of the particles
		std::vector<TH2I*>	fHistPyVsOther;	///< Spectra showing the Y momentum of one particle vs the sum of other particles in the coincidence
		TGo4Picture*	fPicPyVsOther;	///< Picture showing the spectra of Y momenta vs. some of other Y momenta
		std::vector<TGo4Picture*>	fPicPartSumY;	///< Picture showing y momentum sum related information for each particle
		std::vector<TH2I*>	fHistPzSumVsPz;	/// Spectra showing spectra of the Z momentum sum vs. Z momentum components
		TGo4Picture*	fPicPzSum;	///< Picture showing spectra of the Z momentum sum vs. Z momentum components of the particles
		std::vector<TH2I*>	fHistPzVsOther;	///< Spectra showing the Z momentum of one particle vs the sum of other particles in the coincidence
		TGo4Picture*	fPicPzVsOther;	///< Picture showing the spectra of Z momenta vs. some of other Z momenta
		std::vector<TGo4Picture*>	fPicPartSumZ;	///< Picture showing Z momentum sum related information for each particle
		
		std::vector<TH2I*>	fHistDeltaPhi;	///< Histograms showing the difference of one particle's azimuthal angle and the azimuthal angle of the remaining particles' momentum sum vs. longitudinal momentum
		//@}
		
		/** \name Spectra and pictures showing kinetic energie release vs. individual particles' momenta */
		//@{
		std::vector<TH2I*>	fHistKerVsEnergy;	///< Spectra showing KER vs. energy of each particle
		std::vector<TH2I*>	fHistKerVsPx;	///< Spectra showing KER vs. X momentum of each particle
		std::vector<TH2I*>	fHistKerVsPy;	///< Spectra showing KER vs. y momentum of each particle
		std::vector<TH2I*>	fHistKerVsPz;	///< Spectra showing KER vs. Z momentum of each particle
		std::vector<TGo4Picture*>	fPicKerVsMomenta;	///< Pictures combingh the KER vs. momentum component plots for each particle
		//@}
		
		/** \name Spectra and pictures showing momenta of individual particles with coincidence condition */
		//@{
		std::vector<TH1I*>	fHistPx;	///< Spectra for X momentum of each particle
		std::vector<TH1I*>	fHistPy;	///< Spectra for Y momentum of each particle
		std::vector<TH1I*>	fHistPz;	///< Spectra for Z momentum of each particle
		std::vector<TH2I*>	fHistPxy;	///< Spectra showing XY-plane momentum projection
		std::vector<TH2I*>	fHistPxz;	///< Spectra showing XZ-plane momentum projection
		std::vector<TH2I*>	fHistPyz;	///< Spectra showing YZ-plane momentum projection
		std::vector<TH3I*>	fHistPxyz;	///< Spectra showing full 3D momentum
		std::vector<TH2I*>	fHistPtranslong;	///< Spectra showing Transverse vs. Z momentum
		std::vector<TH2I*>	fHistRadTof;	///< Spectra showing Radius on the detector vs. Time of flight
		std::vector<TH2I*>	fHistXTof;	///< Spectra showing Radius on the detector vs. Time of flight
		std::vector<TH2I*>	fHistYTof;	///< Spectra showing Radius on the detector vs. Time of flight
		std::vector<TGo4Picture*>	fPicPartMom;	///< Combines all momentum spectra for one particle
		std::vector<TH1I*>	fHistEnergy;	///< Spectra for the individual particle's energy
		std::vector<TH1I*>	fHistPhi;	///< Spectra for the azimuthal angle of each particle
		std::vector<TH1I*>	fHistTheta;	///< Spectra for the polar angle of each particle
		std::vector<TH2I*>	fHistShperical;	///< Spectra for the solid angle distribution of each particle
		//@}
		
		/** \name Spectra with relations between particles in coincidence */
		//@{
		std::vector< std::vector<TH1I*> >	fHistMutualAngle;	///< 1D Sinusoidal histogram with mutual angles between all particles in coincidence event
		std::vector< std::vector<TH2I*> >	fHistEnergy2Particle;	///< Energy correlation spectrum between two particles
		//@}
		
		/** \name Newton plots for three-particle coincidences */
		//@{
		TH2I*	fHistNewton[3];	///< Newton plots without normalisation and shifting
		TH2I*	fHistNewtonN[3];	///< Newton plots normalised to length of fixed particle's momentum
		TH2I*	fHistNewtonS[3];	///< Newton plots with shift into centre of mass of plotted particles
		TH2I*	fHistNewtonNS[3];	///< Shifted and scaled Newton plots
		//@}
		
		/** \name Dalitz plots for three-particle coincidences */
		//@{
		TH2I*	fHistDalitzP[3];	///< Dalitz plots in terms of paticle's momentum
		TH2I*	fHistDalitzE[3];	///< Dalitz plots in terms of paticle's energy
		//@}
		
		TH1I*	fHistNumCoincidences;	///< Spectrum showing No. of coincidences per event
		
		TRemiPxyz	mMomentumSum;	///< 3D Vector used for the momentum sum
		
		Bool_t mHaveSameSpecies;	///< Flag if this coincidence contains a species more than once
		
		std::vector<TRemiCalcProcMomentum*>	fMomentumCalculators;	///< Pointers to the momentum calculators of the constituent particles
		
		/** \name Iterators for the coincidence finding */
		//@{
		std::vector< std::vector<TRemiParticle*>::iterator >	fPartIts;	///< Vector of particle iterators used in the coincidence finder
		std::vector< std::vector<TRemiParticle*>::iterator >	fPartBegins;	///< Vector keeping the iterators to the begin of each Particle vector
		std::vector< std::vector<TRemiParticle*>::iterator >	fPartEnds;	///< Vector keeping the iterators to the end of each Particle vector
		//@}
		
		std::vector<TRemiCoincidence*>	mFoundCoincidences;	///< Vector of pointers to the coincidences found by this handler
		
		ClassDef(TRemiCalcProcCoincidences, 1);
};

#endif // TREMICALCPROCCOINCIDENCES_H
