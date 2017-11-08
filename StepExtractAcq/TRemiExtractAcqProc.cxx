//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the main processing file for the ExtractMBS step
//----------------------------------------------------------------
#include "TRemiExtractAcqProc.h"

#include "../EventElement/TRemiExtractEvent.h"

#include "../Core/Extern/ConfigFile.h"

#include "TClass.h"


/**
 * \brief Default constructor. Not usable!
 */
TRemiExtractAcqProc::TRemiExtractAcqProc ()
: TGo4EventProcessor()
{
}

/**
 * \brief Constructor to be used
 */
TRemiExtractAcqProc::TRemiExtractAcqProc(const char* name)
: TGo4EventProcessor(name), TRemiIncludeFunctions(ACQIRISCONFIG)
{
	//------------------------------------------//
	// Read information from configuration file //
	//------------------------------------------//

}

/**
 * \brief Destructor
 */
TRemiExtractAcqProc::~TRemiExtractAcqProc ()
{
}

/**
* \brief Check Event Class
*/
Bool_t TRemiExtractAcqProc::CheckEventClass(TClass* ul)
{
	return ul->InheritsFrom(TRemiExtractEvent::Class());
}


/**
 * \brief Main processing function: Actually just passes the data from TRemiAcqirisEventSource to the next step
 */
Bool_t TRemiExtractAcqProc::BuildEvent(TGo4EventElement* dest)
{

	fInput = dynamic_cast<TRemiExtractEvent*> (GetInputEvent());

	SetKeepOutputEvent(!fInput->IsValid()); // Skips all later analysis steps, if the fInput ExtractEvent is not valid

	fOutput = dynamic_cast<TRemiExtractEvent*> (dest);
	fOutput->CheckSizes(fInput->fiADC.size(), fInput->fiTDC.size());
	fOutput->Clear();
	
	for (UInt_t i=0; i<fInput->fiADC.size(); i++)
		fOutput->fiADC[i] = fInput->fiADC[i];
	
	for (UInt_t i=0; i<fInput->fiTDC.size(); i++)
	{
		for (UInt_t j=0; j<fInput->fiTDC[i].size(); j++)
			fOutput->AddTdcHit(i, fInput->fiTDC[i][j]);
	}
	
	return 1;
}

