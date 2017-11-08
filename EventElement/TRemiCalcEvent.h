//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class describes the interface between the Calc and the finish step
//----------------------------------------------------------------
#ifndef TRemiANLEVENT_H
#define TRemiANLEVENT_H

#include "../Core/TRemiIncludeFunctions.h"
#include "../Core/TRemiEventElement.h"
#include "../Core/Point/TRemiParticle.h"
#include "../Core/Point/TRemiCoincidence.h"

#include "Rtypes.h"

#include <vector>

/**
 * \brief Defines the event produced by the Analysis step
 */
class TRemiCalcEvent : public TRemiEventElement
{
	public:
		TRemiCalcEvent() ;
		TRemiCalcEvent(const char* name) ;
		virtual ~TRemiCalcEvent() ;
		void  Clear(Option_t *t="");  
		
		void CheckSizes ( UInt_t numPart, UInt_t numCoinc );
		void AddParticle ( const TRemiParticle &NewPart );
		void AddCoincidence ( const TRemiCoincidence &NewCoinc );
		
		std::vector<TRemiParticle>	fParticles;	///< Non-coincident particles passed from Calc Step
		std::vector<TRemiCoincidence>	fCoincidences;	///< Coincident events passed from Calc Step
		
		Float_t	fDelayValue;	///< Pump-Probe delay
	
	private:
		
ClassDef(TRemiCalcEvent, 1);
};
#endif //TRemiANLEVENT_H




//----------------------------END OF GO4 SOURCE FILE ---------------------

