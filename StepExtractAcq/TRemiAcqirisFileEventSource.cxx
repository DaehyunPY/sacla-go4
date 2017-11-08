//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class provides TDC and ADC raw data arrays from Acqiris data file
//----------------------------------------------------------------

#include "TRemiAcqirisFileEventSource.h"

#include "TRemiAcqirisDataStreamError.h"
#include "../EventElement/TRemiExtractEvent.h"
#include "TRemiAcqirisArchive.h"

#include "TGo4Analysis.h"
#include "TGo4UserSourceParameter.h"

#include "TClass.h"
#include "TROOT.h"

/**
* \brief Default constructor. Not to be used
*/
TRemiAcqirisFileEventSource::TRemiAcqirisFileEventSource() :
	TRemiAcqirisEventSource()
{
}

/**
* \brief Constructor with text arguments
*/
TRemiAcqirisFileEventSource::TRemiAcqirisFileEventSource(const char* name, const char* args, Int_t port) :
	TRemiAcqirisEventSource(name, args, port)
{
	mListFileName = name;
	Init();
}

/**
* \brief Constructor invoked with TGo4UserSourceParameter object
* 
* This is the principal constructor called by the GUI framework
*/
TRemiAcqirisFileEventSource::TRemiAcqirisFileEventSource(TGo4UserSourceParameter* ParUserSource) :
	TRemiAcqirisEventSource(ParUserSource)
{
	mListFileName = ParUserSource->GetName();
	Init();
}

/** \brief Initialise Acqiris File Event Source */
void TRemiAcqirisFileEventSource::Init()
{
	cout << "**** TRemiAcqirisFileEventSource: Initialise" <<endl;
	
	mEventHasData = kFALSE;
	
	// Intialise other variables
	mListFileIsOpen = kFALSE;
	
	mLMAheaderSize = 0;
	mLMAfileActive = kFALSE;
	
	fLMAfile = new TRemiAcqirisArchive(TRemiAcqirisArchive::ArReading);
	
	// Open List file
	OpenListFile();
	
	// Enable rescaling of Fill 'n' Go histograms
	EnableRescaleFillToHist();
	
}

/** \brief Destructor */
TRemiAcqirisFileEventSource::~TRemiAcqirisFileEventSource()
{
	CloseListFile();
	delete fLMAfile;
}

Bool_t TRemiAcqirisFileEventSource::CheckEventClass(TClass* cl)
{
   return cl->InheritsFrom(TRemiExtractEvent::Class());
}

Bool_t TRemiAcqirisFileEventSource::BuildEvent(TGo4EventElement* dest)
{
	fOutput = dynamic_cast<TRemiExtractEvent*> (dest);
	if (fOutput==0) return kFALSE;
	fOutput->CheckSizes(mNumAdcChan, mNumTdcChan);
	fOutput->Clear();
	
	mValidOutputEvent = kFALSE;

	while (!mEventHasData) // process files until there is data for further steps
	{
		// process list file if no LMA file is active. 
		if (!mLMAfileActive)
			ProcessListFile(); // Throws ThrowEOF (Stops Analysis) at end of the lml file, i.e. leaves the while-loop at that point.
		// process Acqiris event from LMA file, if one is active
		else
		{
			// Whenever a wrong (negative or very large) length for a to-be-made
			// array allocation is read from the datastream an exception is thrown
			// Catch those here, deactivate file, take next in list ...
			try {
				ProcessLMAFile(); // sets mEventHasData to kTRUE, after reading a bunchtrain
			}
			catch (const TRemiAcqirisDataStreamError & e) {
				std::cout << "Caught: " << e.what() << std::endl;
				mLMAfileActive = kFALSE;
				return 0;
			}
		}
	}

	// Extract data of one minibunch and copy it into the output event (TRemiExtractEvent)
	SingleShotEvent();
	
	// Disable rescaling of Fill 'n' Go histograms
	DisableRescaleFillToHist();

	return mValidOutputEvent; // Is set accordingly in SingleShotEvent(). [But has most likely no further effects.]
}

/** \brief Open the list file */
Int_t TRemiAcqirisFileEventSource::OpenListFile()
{
	if(mListFileIsOpen) return -1;
	cout << "**** TRemiAcqirisFileEventSource: Open file "<< mListFileName << endl;
	// open connection/file
	fListFile = new std::ifstream(mListFileName);
	if(( fListFile==0) || !fListFile->good()) {
		delete fListFile;
		fListFile = 0;
		SetCreateStatus(1);
		SetErrMess(Form("Error opening user file: %s", mListFileName.Data()));
	}
	else 
		mListFileIsOpen=kTRUE;
	return 0;
}

/** \brief Close the list file */
Int_t TRemiAcqirisFileEventSource::CloseListFile()
{
	if(!mListFileIsOpen) return -1;
	cout << "**** TRemiAcqirisFileEventSource: Close file"<< endl;
	Int_t status=0; // closestatus of source
	delete fListFile;
	mListFileIsOpen=kFALSE;
	return status;
}

/** \brief Read next line from List file */
Bool_t TRemiAcqirisFileEventSource::ProcessListFile()
{
	if (mListFileIsOpen)
	{
		TString buffer;
		buffer.ReadLine(*fListFile); // read whole line into buffer
		if( fListFile->eof() || !fListFile->good()) 
		{
			// reached last line or read error?
			cout << "**** TRemiAcqirisFileEventSource: End of input file " << mListFileName << endl;
			ThrowEOF( 1, 1, Form("**** TRemiAcqirisFileEventSource: End of input file %s", mListFileName.Data()) );
			return 0;
		}
		else if (buffer.BeginsWith("#") || buffer.BeginsWith("!")) // skip comment lines
		{
			cout << "**** TRemiAcqirisFileEventSource: Skipping line:        " << buffer.Data() << endl << endl;
		}
		else if (buffer.BeginsWith("@")) // Enables the execution of Root commands from the List Files
		{
			buffer.Remove(0, 1); // Remove leading @
			TGo4Analysis::Instance()->ExecuteLine(buffer.Data());
		}
		else if (buffer.Contains(".lma"))
		{
			if (buffer.EndsWith("\n") || buffer.EndsWith("\r"))
				buffer.Chop();
			OpenLMAFile(buffer);
		}
	}
	
	return 1;
}


Bool_t TRemiAcqirisFileEventSource::OpenLMAFile ( TString FileName )
{
	cout << "**** TRemiAcqirisFileEventSource: Analysing data file " << FileName << endl;
	
	//--open an Archive with filename and check wether the file was opened correctly--//
	fLMAfile->newFile(FileName);
	if(!fLMAfile->fileIsOpen())
	{
		std::cout << "**** TRemiAcqirisFileEventSource: Error opening file " << FileName << std::endl;
		return kFALSE;
	}
	
	//--reset the event counter--//
// 	EventCounter=0;
	
	Short_t numChan;
	//--read file Header--//
	*fLMAfile >> mLMAheaderSize;	//size of header
	*fLMAfile >> numChan;		//nbr of Channels in Instrument


	if ((mLMAheaderSize <= 0) || (MAX_ARRAY_LENGTH < mLMAheaderSize))
	{
		throw TRemiAcqirisDataStreamError(mLMAheaderSize, "mLMAheaderSize", __func__, __FILE__, __LINE__);
	}

	if ((numChan <= 0) || (MAX_ARRAY_LENGTH < numChan))
	{
		throw TRemiAcqirisDataStreamError(numChan, "numChan", __func__, __FILE__, __LINE__);
	}


	if (numChan > mNumAcqChan)
		cout << "**** TRemiAcqirisFileEventSource: Data source has more channels (" << numChan << ") than set in the Config File (" << mNumAcqChan << ")" << endl;
	
	//--create an Event with the right amount of Channels--//
	fAcqirisEvent.init(numChan, fConfigFile);
	fAcqirisEvent.readFileHeader(*fLMAfile);	//let the event read the rest of the header
	
	mLMAfileActive = kTRUE;
	return kTRUE;
}

/**
 * \brief Main data file processing step
 * Reads data from the current LMA file into fAcqirisEvent and runs the Software TDC
 */
Bool_t TRemiAcqirisFileEventSource::ProcessLMAFile()
{
	if (fLMAfile->EndOfFile())
	{
		mLMAfileActive = kFALSE;
		return kFALSE;
	}
	else
	{
		fAcqirisEvent.clear();
		fAcqirisEvent.readEvent(fLMAfile);	// Datei einlesen
		mEventHasData = kTRUE;
		mMinibunchIndex = 0;
	}
	return kTRUE;
}

ClassImp(TRemiAcqirisFileEventSource);
