//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the main header file for the Finish step
//----------------------------------------------------------------
#ifndef TRemiFDCSPROCESSOR_H
#define TRemiFDCSPROCESSOR_H

#define FINISHCONFIG "cFinish.conf"


#include "../Core/Point/TRemiPxyz.h"
#include "../Core/TRemiIncludeFunctions.h"

#include "TGo4EventProcessor.h"

#include "Rtypes.h"

#include <vector>

class TRemiFinishParameter;
class TRemiFinishEvent;
class TRemiCalcEvent;

class TH1I;
class TClass;

/**
 * \brief Main class running the Finish step
 */
class TRemiFinishProc : public TGo4EventProcessor, protected TRemiIncludeFunctions
{
	public:
		TRemiFinishProc() ;
		TRemiFinishProc(const char* name);
		virtual ~TRemiFinishProc() ;
		virtual Bool_t BuildEvent(TGo4EventElement* dest);

		/** This method checks if event class is suited for the source */
		virtual Bool_t CheckEventClass(TClass* fl);
	
	private:
		TRemiCalcEvent		*fInput; ///< Input event
		TRemiFinishEvent	*fOutput; ///< Output event
	
		TRemiFinishParameter *fParaFinish; ///< Parameter Object for Finish step
		
		/** \name Default histograms */
		//@{
		TH1I*	fHistNumParticles;	///< Histogram showing the number of particles per event passed from Calc step
		TH1I*	fHistNumCoincidences;	///< Histogram showing the number of coincidences per event passed from Calc step
		//@}
		
		/** \name Examples */
		TH1F*	fHistAsymEleEnergy;	///< Example asymmetry plot vs. electron energy
	
	
		ClassDef(TRemiFinishProc, 1);
};
#endif //TRemiFDCSPROCESSOR_H




//----------------------------END OF GO4 SOURCE FILE ---------------------

