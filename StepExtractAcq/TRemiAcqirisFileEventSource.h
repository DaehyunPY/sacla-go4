//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class provides TDC and ADC raw data arrays from Acqiris data file
//----------------------------------------------------------------

#ifndef TREMIACQIRISFILEEVENTSOURCE_H
#define TREMIACQIRISFILEEVENTSOURCE_H

#include "TRemiAcqirisEventSource.h"

#include "Rtypes.h"

#include <vector>

class TRemiAcqirisArchive;

class TGo4UserSourceParameter;

class TClass;


class TRemiAcqirisFileEventSource : public TRemiAcqirisEventSource
{
	public:
		TRemiAcqirisFileEventSource();

		/** Create source specifying values directly */
		TRemiAcqirisFileEventSource(const char* name, const char* args, Int_t port);

		/** Creat source from setup within usersource parameter object */
		TRemiAcqirisFileEventSource(TGo4UserSourceParameter* par);

		virtual ~TRemiAcqirisFileEventSource();


		/** This method checks if event class is suited for the source */
		virtual Bool_t CheckEventClass(TClass* cl);

		/** This methods actually fills the target event class which is passed as pointer.
		* Uses the latest event which is referenced
		* by fxEvent or fxBuffer. Does _not_ fetch a new event
		* from source, therefore one source event may be used
		* to fill several TRemiExtractEvent classes. */
		virtual Bool_t BuildEvent(TGo4EventElement* dest);

		
	private:
		void Init();
		Int_t OpenListFile();
		Int_t CloseListFile();
		Bool_t ProcessListFile();

		Bool_t 	OpenLMAFile (TString FileName);
		Bool_t	ProcessLMAFile ();
		
		std::ifstream* 	fListFile;	///< file that contains the list of data files in ascii format.
		TString 	mListFileName;	///< name of text file containing list of data files
		Bool_t 	mListFileIsOpen;	///< Flag whether text file is open
		
		TRemiAcqirisArchive*	fLMAfile;	///< Archive object that handles the communication with the LMA data file
		Int_t mLMAheaderSize;	///< Size of the file header
		Bool_t mLMAfileActive;	///< Is there a active LMA file connected to the archive?
		

	ClassDef(TRemiAcqirisFileEventSource, 1);
};

#endif //TGO4MBSSOURCE_H
