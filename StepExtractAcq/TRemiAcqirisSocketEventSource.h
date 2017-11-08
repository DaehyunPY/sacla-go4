//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class provides TDC and ADC raw data arrays from Acqiris data file
//----------------------------------------------------------------

#ifndef TREMIACQIRISSOCKETEVENTSOURCE_H
#define TREMIACQIRISSOCKETEVENTSOURCE_H

#include "TRemiAcqirisEventSource.h"

#include "TString.h"
#include "Rtypes.h"

#include <vector>

class TRemiAcqirisEvent;

class TGo4UserSourceParameter;

class TMonitor;
class TSocket;
class TClass;

class TRemiAcqirisSocketEventSource : public TRemiAcqirisEventSource
{
	public:
		TRemiAcqirisSocketEventSource();

		/** Create source specifying values directly */
		TRemiAcqirisSocketEventSource(const char* name, const char* args, Int_t port);

		/** Creat source from setup within usersource parameter object */
		TRemiAcqirisSocketEventSource(TGo4UserSourceParameter* par);

		virtual ~TRemiAcqirisSocketEventSource();


		/** This method checks if event class is suited for the source */
		virtual Bool_t CheckEventClass(TClass* cl);

		/** This methods actually fills the target event class which is passed as pointer.
		* Uses the latest event which is referenced
		* by fxEvent or fxBuffer. Does _not_ fetch a new event
		* from source, therefore one source event may be used
		* to fill several TRemiExtractEvent classes. */
		virtual Bool_t BuildEvent(TGo4EventElement* dest);

		Bool_t ReceiveData(TSocket* Socket);

		
	private:
		void Init();
		Int_t OpenSocket();
		Int_t CloseSocket();
		
		Int_t ReceiveDataDirect();
		Int_t recvChunks(TSocket * socket, Char_t * buffer, Int_t length);

		Int_t ReceiveDataAsObject();
		Int_t recvObject(TSocket * socket);

		TMonitor*	fMonitor;	///< Monitor to wait for data
		TSocket*	fReceiverSocket;	///< Socket to receive data
		
		Bool_t	GetEventFromBuffer ();
		
		void		RunWaitingProcesses ();
		Long64_t 	mWaitingProcessesThen;	///< Time reference for waiting processes checker
		
		void 	CheckForReconnect ();
		Long64_t	mReconnectInterval;	///< Times between reconnect attempts
		Long64_t	mReconnectThen;	///< Time reference for reconnect checker
		
		TString 	mServerUrl;	///< name of Stream server
		Bool_t 	mServerIsOpen;	///< Flag whether server connection is open
		
		Char_t*	mBuffer;
		Int_t	mSizeOfBuffer;
		Bool_t	mUseGmd;
		
		TRemiAcqirisEvent* fTempEvent;
		
//		Long_t mLMAheaderSize;	///< Size of the file header
//		Bool_t mLMAfileActive;	///< Is there a active LMA file connected to the archive?
		
		std::vector<TRemiAcqirisEvent*>	fEventBuffer;	///< Buffer to temporarily save arrays
		

	ClassDef(TRemiAcqirisSocketEventSource, 1);
};

#endif //TGO4MBSSOURCE_H
