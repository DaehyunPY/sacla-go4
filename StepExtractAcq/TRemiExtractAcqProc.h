//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the main header for the ExtractMBS step
//----------------------------------------------------------------

#ifndef SEXTRACTACQPROC_H
#define SEXTRACTACQPROC_H

#define ACQIRISCONFIG "cAcqiris.conf"


#include "../Core/TRemiIncludeFunctions.h"

#include "TGo4EventProcessor.h"

class TRemiExtractEvent;


/**
 * \brief Step processor for event extraction from MBS TDC and ADC raw data
 */

class TRemiExtractAcqProc : public TGo4EventProcessor, public TRemiIncludeFunctions
{
	public :
		TRemiExtractAcqProc();
		TRemiExtractAcqProc(const char* name);
		~TRemiExtractAcqProc();
		virtual Bool_t BuildEvent(TGo4EventElement* dest);
		/** This method checks if event class is suited for the source */
		virtual Bool_t CheckEventClass(TClass* ul);
		
	private :
		TRemiExtractEvent*	fInput;
		TRemiExtractEvent*	fOutput;
		
	ClassDef(TRemiExtractAcqProc, 1);
};

#endif
