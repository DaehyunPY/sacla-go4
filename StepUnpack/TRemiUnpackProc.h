
//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the main header file for the Unpack step
//----------------------------------------------------------------

#ifndef TRemiUNPACKPROCESSOR_H
#define TRemiUNPACKPROCESSOR_H

#define UNPACKCONFIG "cUnpack.conf"

#include "TRemiUnpackProcTof.h"

#include "../Core/Point/TRemiDetectorHit.h"
#include "../Core/TRemiIncludeFunctions.h"

#include "TGo4EventProcessor.h"

#include "TString.h"
#include "Rtypes.h"

#include <vector>

class TRemiUnpackParameter;
class TRemiUnpackProcPos;
class TRemiUnpackProcDelay;
class TRemiUnpackEvent;
class TRemiExtractEvent;

class ConfigFile;

class TH1I;
class TH1F;
class TH2I;


/**
 * \brief Main object for the Unpack step
 */
class TRemiUnpackProc : public TGo4EventProcessor, public TRemiIncludeFunctions
{
	public:
		TRemiUnpackProc() ;
		TRemiUnpackProc(const char* name);
		virtual ~TRemiUnpackProc() ;
		virtual Bool_t BuildEvent(TGo4EventElement* dest);
		/** This method checks if event class is suited for the source */
		virtual Bool_t CheckEventClass(TClass* ul);
		
		
	private:
		TRemiUnpackParameter*	fParaUnpack; ///< Global parameter settings for the Unpack step
		
		TRemiUnpackProcPos* fElectron; ///< Produces time-of-flight and position for electrons
		TRemiUnpackProcPos* fRecoil; ///< Produces time-of-flight and position for electrons
		
		TString mDetElec;	///< Type of electron detector
		TString mDetIon;	///< Type of ion detector
		
		
	// electron related objects
		std::vector<TRemiDetectorHit>	mHit_e;	///< Position of the electrons
	
	// Ion related objects
		std::vector<TRemiDetectorHit>	mHit_r;	///< Position of the ions
		
	// Delay related objects
		TRemiUnpackProcDelay* fDelay;	///< Handles calibration of Delay stage signal
		Bool_t	mUseDelay;	///< Is a delay stage employed?
		Float_t mDelay;	///< Value of the a.c. delay
		Int_t mMbIndex;		///Value of Minibunch Index

	// other objects
		
		TH1I          *fOutputValid; ///< Count valid output events
		
		TRemiExtractEvent*	fInput;	///< Input event
		TRemiUnpackEvent* fOutput; ///< Output event
		
		Int_t mAdcChanMbIndex; ///Mb-Index of single shot event
		
		TH1I*	fHistMbIndex;	///< Histogram of the Mb-Index of single shot event without any conditions
		TH1I*	fHistMbIndexISort;	///< Histogram of the Mb-Index for sorted ions only
		TH2I*	fHistMbIndexTofISort;	///< Mb-index vs. Tof for sorted ions


		/** \name /(p[ie])+co/ related objects */
		//@{
		TH2I*	fPiPiCo;	///< Ion-Ion coincidence plot
		TH2I*	fPePiCo;	///< Ion-Electron coincidence plot
		TH2I*	fPePePiCo;	///< Ion-Electron-Electron coincidence plot
		TH2I*	fTriPiCo1;	///< Ion-Ion-Ion coincidence plot
		TH2I*	fTriPiCo2;	///< Ion-Ion-Ion coincidence plot
		TH2I*	fTriPiCo3;	///< Ion-Ion-Ion coincidence plot

		TGo4PolyCond*	fconPiPiCo;	///< Polygon condition in the Pipico plot
		TGo4PolyCond*	fconPePiCo;
		TGo4PolyCond*	fconPePePiCo;
		TGo4PolyCond*	fconPePiPiCo;

		TGo4PolyCond*	fconPos_r; // fSortConPos from TRemiUnpackProcPos for recoil ions
		TGo4PolyCond*	fconPos_e; // fSortConPos from TRemiUnpackProcPos for electrons
		TGo4WinCond*	fconTof_r; // fcon_TofAll from TRemiUnpackProcTof for recoil ions
		TGo4WinCond*	fconTof_e; // fcon_TofAll from TRemiUnpackProcTof for electrons
		TGo4WinCond*	fconY_e;   // fcon_YSort  from TRemiUnpackProcTof for electrons

		TH2I*	fPiPiCo_conPos;
		TH1I*	fDelay_conPiPiCo;	///< Delay with Pipico condition
		TH2I*	fPos_conPiPiCo;

		TH2I*	fPePiCo_conPos;
		TH1I*	fDelay_conTof;
		TH1I*	fDelay_conPePiCo;
		TH2I*	fPos_r_conTof;
		TH2I*	fPos_r_conPePiCo;
		TH2I*	fPos_e_conTof;
		TH2I*	fPos_e_conPePiCo;
		TH2I*	fX_r_Delay_conTof;
		TH2I*	fX_r_Delay_conPePiCo;
		TH2I*	fX_e_Delay_conTof;
		TH2I*	fX_e_Delay_conPePiCo;
		TH2I*	fX_e_Delay_conTofconY;

		TH2I*	fPePePiCo_conPos;
		TH2I*	fPePePiCo_conTof;
		TH1I*	fDelay_conPePePiCo;
		TH2I*	fPos_r_conPePePiCo;
		TH2I*	fPos_e_conPePePiCo;

		TH2I*	fPePiPiCo;
		TH2I*	fPePiPiCo_conPos;
		TH2I*	fPePiPiCo_conTof;
		TH1I*	fDelay_conPePiPiCo;
		TH2I*	fPos_e_conPePiPiCo;
		TH2I*	fPos_r_conPePiPiCo;
		//@}


	// Intensity related objects
		Bool_t	mUseIntensity;	///< Switch whether intensity signal is used
		Short_t	mAdcChanIntensity;	///< ADC channel with intensity signal
		
	ClassDef(TRemiUnpackProc, 1);
};

#endif //TONLUNPACKPROCESSOR_H

//----------------------------END OF GO4 SOURCE FILE ---------------------
