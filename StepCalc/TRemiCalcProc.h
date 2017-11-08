//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the header for the main processing class of the Calculate step
//----------------------------------------------------------------
#ifndef TRemiANLPROCESSOR_H
#define TRemiANLPROCESSOR_H

#define CALCCONFIG "cCalc.conf"


#include "TRemiCalcProcMomentum.h"
#include "TRemiCalcProcCoincidences.h"
#include "../Core/TRemiIncludeFunctions.h"
#include "../Core/Point/TRemiParticle.h"
#include "../Core/Point/TRemiDetectorHit.h"
#include "../Core/Point/TRemiCoincidence.h"

#include "TGo4EventProcessor.h"

#include "TString.h"

#include <vector>

class TRemiCalcParameter;
class TRemiCalcEvent;
class TRemiUnpackEvent;

class TGo4PolyCond;
class TGo4CondArray;

class TH2I;
class TH1I;

class TRemiCalcEvent; // This declaration is unavoidable because of a loop inclusion between TRemiCalcProc and TRemiCalcEvent

/**
 * \brief Main class running the Analysis step
 */
class TRemiCalcProc : public TGo4EventProcessor, protected TRemiIncludeFunctions
{

	public:
		TRemiCalcProc() ;
		TRemiCalcProc(const char * name);
		virtual ~TRemiCalcProc();
		virtual Bool_t BuildEvent(TGo4EventElement* dest);
		/** This method checks if event class is suited for the source */
		virtual Bool_t CheckEventClass(TClass* cl);

	private:

		/** \name Input histograms */
		//@{
		TH2I*	fXYr_input;	///< Display ion position picture as it comes from Unpack step
		TH1I*	fTr_input;	///< Display ion time-of-flight as it comes from Unpack step
		TH2I*	fXYe_input;	///< Display electron position picture as it comes from Unpack step
		TH1I*	fTe_input;	///< Display electron time-of-flight as it comes from Unpack step
		TH1I*	fIntensity_input;	///< Display intensity signal as it comes form Unpack step
		//@}
		
		/** \name Spectra with intensity condition */
		//@{
		TGo4CondArray*	fConArIntensity;	///< Array of conditions on intensity
		UShort_t	mNumConIntensity;	///< No. of intensity conditions in array fConArIntensity
		std::vector<TH1I*>	fTr_inputConIntensity;	///< Ion Time-of-flight plots for the intensity conditions
		//@}

//		/** \name Pipico related objects */
//		//@{
//		TH2I*	fPiPiCo;	///< Ion-Ion coincidence plot
//		TGo4PolyCond*	fconPiPiCo;	///< Polygon condition in the Pipico plot
//		TH1I*	fDelayPiPiCo;	///< Delay with Pipico condition
//		//@}

//		TH2I*	fPePiCo;	///< Ion-Electron coincidence plot
//		TH2I*	fPePePiCo;	///< Ion-Electron-Electron coincidence plot

		
		/** \name Input */
		//@{
		TRemiUnpackEvent 	*fInput; ///< Input event
		UShort_t	mNumIonHits;	///< Actual number of ion hits
		UShort_t	mNumElecHits;	///< Actual number of electron hits
		//@}

		/** \name Output*/
		//@{
		TRemiCalcEvent		*fOutput; ///< Output event
		std::vector<TRemiParticle>	mParticles; ///< Tof, Position and Momenta of all particles
		//@}

		/** \name Momentum calculators */
		//@{
		std::vector<TRemiCalcProcMomentum>	fMomCalc;	///< Object handling momentum calculations
		UShort_t	mNumIonSpecies;	///< No. of different ion species for momentum calculation
		UShort_t	mNumElec;	///< No. of electrons (in terms of different energies ranges) for momentum calculation
		//@}

		/** \name Coincidence handlers */
		//@{
		UShort_t	mNumCoincidenceChannels;	///< No. of coincidence channels to analyse
		std::vector<TRemiCalcProcCoincidences>	fCoincidenceHandler;	///< Analyses coincidence channels
		std::vector<TRemiCoincidence>	mCoincidences;	///< Saves coincident particles
		//@}

		/** \name Counter histograms */
		//@{
		TH1I*	fNumTotalParticles;	///< Histogram for total no. of particles
		TH1I*	fNumTotalCoincidences;	///< Histogram for total no. of coincidences
		//@}

		/** \name Histograms for non-coincident ion species */
		//@{
		std::vector<TH1I*>	fHistIonDelay;	///< Delay spectrum
		std::vector<TH2I*>	fHistIonEnergyDelay;	///< Energy vs. Delay
		std::vector<TH1I*>	fHistIonIntensity;	///< Intensity dependence
		std::vector<TH1I*>	fHistIonMinibunch;	///< Minibunch index spectrum
		std::vector<TH2I*>	fHistIonPxDelay;	///< Px vs. Delay
		std::vector<TH2I*>	fHistIonPyDelay;	///< Py vs. Delay
		std::vector<TH2I*>	fHistIonPzDelay;	///< Pz vs. Delay
		//@}
		
		
		/** \name Histograms for coincidence channels */
		//@{
		std::vector<TH1I*>	fHistCoinDelay;	///< Delay spectrum
		std::vector<TH2I*>	fHistCoinKerDelay;	///< Energy vs. Delay
		std::vector<TH1I*>	fHistCoinIntensity;	///< Intensity dependence
		std::vector<TH1I*>	fHistCoinMinibunch;	///< Minibunch index spectrum
		
		std::vector< std::vector< TH2I* > >	fHistCoinEnergyDelay;	///< Energy of one particle vs. Delay
		std::vector< std::vector< std::vector< TH2I* > > >	fHistCoinAngleDelay;	///< Angle between two particles vs. Delay
		//@}

		/** \name Output handlers */
		//@{
		std::vector<TRemiCalcProcMomentum*>	fOutputMomCalc;	///< Links to those momentum handlers that are supposed to create output events
		std::vector<TRemiCalcProcCoincidences*>	fOutputCoincHandler;	///< Links to those coincidence handlers that are supposed to create output events
		//@}

		/** \name Delay related objects */
		//@{
		Float_t	mDelay;	///< Value of Pump-probe delay
		TRemiBasicParameter*	fParaDelayShift;	///< Parameter to shift the zero point of the delay
		Bool_t	mUseDelay;		///< Switch if a delay has been used
		Bool_t	mSymmetricDelay;	///< Switch if the delay is symmetric with respect to zero
		//@}


	ClassDef(TRemiCalcProc, 1);
};
#endif //TRemiANLPROCESSOR_H




//----------------------------END OF GO4 SOURCE FILE ---------------------

