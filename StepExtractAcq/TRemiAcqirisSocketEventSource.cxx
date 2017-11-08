//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class provides TDC and ADC raw data arrays from Acqiris data file
//----------------------------------------------------------------

#include "TRemiAcqirisSocketEventSource.h"

#include "TRemiAcqirisDataStreamError.h"
#include "TRemiAcqirisBuffer.h"
#include "../EventElement/TRemiExtractEvent.h"

#include "TGo4UserSourceParameter.h"

#include "TSocket.h"
#include "TMonitor.h"
#include "TClass.h"
#include "TSystem.h"
#include "TMessage.h"
#include "../Core/RootMod/TObjTArrC.h"



/**
* \brief Default constructor. Not to be used
*/
TRemiAcqirisSocketEventSource::TRemiAcqirisSocketEventSource() :
	TRemiAcqirisEventSource()
{
	mServerUrl = "131.169.215.62";
	Init();
	
}

/**
* \brief Constructor with text arguments
*/
TRemiAcqirisSocketEventSource::TRemiAcqirisSocketEventSource(const char* name, const char* args, Int_t port) :
	TRemiAcqirisEventSource(name, args, port)
{
	mServerUrl = name;
	Init();
}

/**
* \brief Constructor invoked with TGo4UserSourceParameter object
* 
* This is the principal constructor called by the GUI framework
*/
TRemiAcqirisSocketEventSource::TRemiAcqirisSocketEventSource(TGo4UserSourceParameter* ParUserSource) :
	TRemiAcqirisEventSource(ParUserSource)
{
	mServerUrl = ParUserSource->GetName();
	Init();
}

/** \brief Initialise Acqiris File Event Source */
void TRemiAcqirisSocketEventSource::Init()
{
	std::cout << "**** TRemiAcqirisSocketEventSource: Initialise" << std::endl;
	
	InitGeneral();
	mEventHasData = kFALSE;
	
	// Intialise other variables
	mBuffer = 0;
	mSizeOfBuffer = 0;
	
	fEventBuffer = std::vector<TRemiAcqirisEvent*> (0);

	mWaitingProcessesThen = (Long64_t) gSystem->Now();
	
	mReconnectThen = (Long64_t) gSystem->Now();
	mReconnectInterval = fConfigFile->read("acq_reconnect_interval", 1000);
	
	fAcqirisEvent.init(mNumAcqChan, fConfigFile);
	
	// Open Socket
	mServerIsOpen = kFALSE;
/*
	if (!fMonitor)
		fMonitor = new TMonitor ();
*/
	OpenSocket();
	
	// Enable rescaling of Fill 'n' Go histograms
	EnableRescaleFillToHist();
}

/** \brief Destructor */
TRemiAcqirisSocketEventSource::~TRemiAcqirisSocketEventSource()
{
	CloseSocket();
}

Bool_t TRemiAcqirisSocketEventSource::CheckEventClass(TClass* cl)
{
   return cl->InheritsFrom(TRemiExtractEvent::Class());
}

Bool_t TRemiAcqirisSocketEventSource::BuildEvent(TGo4EventElement* dest)
{
	fOutput = dynamic_cast<TRemiExtractEvent*> (dest);
	if (fOutput==0) return kFALSE;
	fOutput->CheckSizes(mNumAdcChan, mNumTdcChan);
	fOutput->Clear();
	
	mValidOutputEvent = kFALSE;

	// We need to check whether the same "while (!mEventHasData)" logic as in
	// TRemiAcqirisFileEventSource::BuildEvent is needed here!

	if (!mServerIsOpen)
		CheckForReconnect();

	// Get Event from Buffer if buffer is not empty and no event currently open
	if (!mEventHasData && mServerIsOpen )
	{
		// Whenever a wrong (negative or very large) length for a to-be-made
		// array allocation is read from the datastream an exception is thrown
		// Catch those here, close the socket, restart ...

		try {

//			ReceiveDataDirect();
			ReceiveDataAsObject();

		}
		catch (const TRemiAcqirisDataStreamError & e) {
			std::cout << "Caught: " << e.what() << std::endl;

			fReceiverSocket->Close();
			mServerIsOpen = kFALSE;

			return 0;
		}

	}
	
	// Extract data of one minibunch and copy it into the output event
	if (mEventHasData)
		SingleShotEvent();
	
	FillTo1DHist(mValidOutputEvent, "Valid output", "Valid output event", 2, 0, 2, "Acq");

	// Disable rescaling of Fill 'n' Go histograms
	DisableRescaleFillToHist();
	
	return mValidOutputEvent; // must be set to kTRUE to save output event. This happens in SingleShotEvent()
}

/** \brief Open the connection to server */
Int_t TRemiAcqirisSocketEventSource::OpenSocket()
{
/*
	if ((fSocket) && (fMonitor->IsActive(fSocket)) && (fSocket->IsValid()) )
		cout << "**** TRemiAcqirisSocketEventSource: Already connected to: "<<fSocket->GetInetAddress().GetHostName()<<endl;
	else
	{
*/
		//make some status output//
		std::cout << "**** TRemiAcqirisSocketEventSource: Currently trying to connect to:" << mServerUrl << std::endl;
		
		//create a socket and connect it to the adress given above
		fReceiverSocket = new TSocket(mServerUrl, 9090);
		if (!fReceiverSocket->IsValid())
		{
			SetCreateStatus(1);
			SetErrMess(Form("**** TRemiAcqirisSocketEventSource: could not connect to: %s", mServerUrl.Data()));
			std::cout << "**** TRemiAcqirisSocketEventSource: could not connect to:" << mServerUrl.Data() << std::endl;
			mServerIsOpen = kFALSE;
			return 0;
		}
		else
		{
			std::cout << "**** TRemiAcqirisSocketEventSource: Connected to: " << fReceiverSocket->GetInetAddress().GetHostName() << std::endl;

/*
// 			if (!fMonitor)
			{
				fMonitor = new TMonitor();
// 			if (!fMonitor->IsActive(Socket))
// 			{
// 				fMonitor->Add(fReceiverSocket);

				// Connect socket with ReceiveData() function
// 				fMonitor->Connect("Ready(TSocket*)", "TRemiAcqirisSocketEventSource", this, "ReceiveData(TSocket*)");
			}
*/
		}

/*
	}
*/
	mServerIsOpen = kTRUE;
	return 1;
}

/** \brief Close the connection to server */
Int_t TRemiAcqirisSocketEventSource::CloseSocket()
{
	//cout << "**** TRemiAcqirisSocketEventSource: Close connection"<< endl;
//	if (fMonitor)
//		delete fMonitor;
	if (fReceiverSocket)
		delete fReceiverSocket;
	// clear event buffer
	while (!fEventBuffer.empty())
	{
		delete fEventBuffer.back();
		fEventBuffer.pop_back();
	}
	return 0;
}

/** \brief Read data from socket into buffer and fill it into TRemiAcqirisEvent */
Bool_t TRemiAcqirisSocketEventSource::ReceiveData(TSocket *Socket)
{
	if (Socket && Socket->IsValid())
	{
		//read the first 4 bytes (sizeof(Int_t)), which include the length of the total buffer
		Int_t  n;
		Int_t len;
		if ((n = Socket->RecvRaw(&len, sizeof(len))) <= 0) 
		{
			if (n == -5) // Connection reset or broken
			{
				std::cout << "Connection closed after reading length" << std::endl;
				Socket->Close();
				mServerIsOpen = kFALSE;
			}
			SetCreateStatus(1);
			SetErrMess("something bad happened getting the length ");
		}
		
		//we do not need to read the complete buffer, because we already read the first 4 bytes
		len -= sizeof(len); 
		
		//read the remaining buffer to buf, resize it if it is too small
		if (mSizeOfBuffer < len)
		{
			//cout <<"resize buffer "<<sizeOfBuffer<<"  "<<len<<endl;
			delete [] mBuffer;
			mBuffer = new char[len*2];
			mSizeOfBuffer = len*2;
		}
		
		if ((n = Socket->RecvRaw(mBuffer, len)) <= 0)
		{
			if (n == -5)// Connection reset or broken
			{
				std::cout << "Connection closed after reading buffer" << std::endl;
				Socket->Close();
				mServerIsOpen = kFALSE;
			}
			SetCreateStatus(1);
			SetErrMess("something bad happened reading the buffer ");
		}
		if (n != len)
		{
			std::cout << "We have not received the right amount of Data: " << len<<" " << n << std::endl;
			return 0;
		}
		
		//give the buffer to the mybuffer class to help serialize it//
		TRemiAcqirisBuffer* buffer = new TRemiAcqirisBuffer(mBuffer,len);
		
		fTempEvent = new TRemiAcqirisEvent(mNumAcqChan, fConfigFile);
		fTempEvent->readEvent(buffer);
		fEventBuffer.push_back(fTempEvent);
		
		delete buffer;
	}
	
	return kTRUE;
}

/** \brief Directly read data from socket into buffer and fill it into TRemiAcqirisEvent */
Int_t TRemiAcqirisSocketEventSource::ReceiveDataDirect()
{
	if (fReceiverSocket && fReceiverSocket->IsValid())
	{
//		Int_t val;

//		val = -1;
//		fReceiverSocket->GetOption(kSendBuffer, val);
//		std::cout << "SendBuffer: " << val << std::endl;

//		val = -1;
//		fReceiverSocket->GetOption(kRecvBuffer, val);
//		std::cout << "RecvBuffer: " << val << std::endl;

		Int_t length;
//		std::cout << "length: select before: " << fReceiverSocket->Select(TSocket::kRead) << std::endl;
		Int_t answer = fReceiverSocket->RecvRaw(&length, sizeof(length));
//		std::cout << "length: select after: " << fReceiverSocket->Select(TSocket::kRead) << std::endl;

		if ((length <= 0) || (MAX_ARRAY_LENGTH < length))
		{
			throw TRemiAcqirisDataStreamError(length, "length", __func__, __FILE__, __LINE__);
		}

		if (answer != sizeof(length))
		{
			fReceiverSocket->Close();
			mServerIsOpen = kFALSE;
			std::cout << "**** TRemiAcqirisSocketEventSource: Server " << mServerUrl << " failed to send the length of the buffer " << std::endl;
			return 0;
		}
		else	// We got the buffer length
		{
//			if (val < length)
//			{
//				std::cout << "RecvBuffer is smaller than buffer length: " << val << " < " << length << std::endl;

//				fReceiverSocket->Close();
//				mServerIsOpen = kFALSE;
//				return 0;
//			}

			// we do not need to read the complete buffer, because we already read the first 4 bytes
			length -= sizeof(length); 
			
			// resize mBuffer if it is too small
			if (mSizeOfBuffer < length)
			{
				std::cout << "increase buffer size from " << mSizeOfBuffer << " to 2*" << length << std::endl;
				delete [] mBuffer;
				mBuffer = new char[length*2];
				mSizeOfBuffer = length*2;
			}

			// read stream into buffer
//			std::cout << "buffer: select before: " << fReceiverSocket->Select(TSocket::kRead) << std::endl;
//			answer = fReceiverSocket->RecvRaw(mBuffer, length);
			answer = recvChunks(fReceiverSocket, mBuffer, length);
//			std::cout << "buffer: select after: " << fReceiverSocket->Select(TSocket::kRead) << std::endl;
			if (answer <= 0)
			{
				fReceiverSocket->Close();
				mServerIsOpen = kFALSE;
				std::cout << "**** TRemiAcqirisSocketEventSource: Server " << mServerUrl << " failed to send the buffer " << std::endl;
				return 0;
			}
			else // We have a buffer
			{
//				std::cout << "length: " << length << " answer: " << answer << std::endl;
				TRemiAcqirisBuffer* BufferHandler = new TRemiAcqirisBuffer(mBuffer,length);
				fAcqirisEvent.clear();
				fAcqirisEvent.readEvent(BufferHandler);
				delete BufferHandler;

				mEventHasData = kTRUE;
				mMinibunchIndex = 0;
				return 1;
			}
		}
	} 
	else	// Socket not active
	{
		mServerIsOpen = kFALSE;
		return 0;
	}
}

/** \brief Receive data from socket in chunks */
Int_t TRemiAcqirisSocketEventSource::recvChunks(TSocket * s, Char_t * b, Int_t l)
{
//	Int_t cs = 16*1024;
	Int_t sent = 0, ret;

	Int_t wnb_count = 0, max_wnb_count = 5;


//	Int_t SizeSendBuffer = -1;
//	s->GetOption(kSendBuffer, SizeSendBuffer);
//	std::cout << "SendBuffer: " << SizeSendBuffer << std::endl;

	Int_t SizeRecvBuffer = -1;
	s->GetOption(kRecvBuffer, SizeRecvBuffer);
//	std::cout << "RecvBuffer: " << SizeRecvBuffer << std::endl;

	Int_t cs = SizeRecvBuffer / 2;
	cs = TMath::Min(cs, l);
//	std::cout << "cs: " << cs << std::endl;


	while (sent < l) {

		ret = s->Select(TSocket::kRead);
		if (ret < 0) {
			return ret;
		}

		ret = s->RecvRaw(b + sent, cs);
		if ( (ret < 0) || (ret != cs) ) {
			if ( ret == -4 ) {
				wnb_count += 1;
				std::cout << "would not block: " << wnb_count << std::endl;
				usleep(100);
				if (wnb_count >= max_wnb_count)
					return ret;
				continue;
			}
			std::cout << "error: " << ret << "\tcs: " << cs << std::endl;
			return ret;
		}
		sent += ret;
//		std::cout << "sent: " << sent << "\tret: " << ret << "\tcs: " << cs << "\tremaining: " << l-sent << std::endl;
		if (sent + cs >= l){
			cs = l - sent;
//			std::cout << "new cs: " << cs << std::endl;
		}
//		usleep(1500);
	}

	return sent;
}

/** \brief Read data from socket as object, unpack into buffer and fill it into TRemiAcqirisEvent */
Int_t TRemiAcqirisSocketEventSource::ReceiveDataAsObject()
{
	if (!(fReceiverSocket && fReceiverSocket->IsValid()))
	{
		// Socket not active
		mServerIsOpen = kFALSE;
		return 0;
	}

	Int_t answer = recvObject(fReceiverSocket);

	if (answer <= 0)
	{
		// An error has occurred
		fReceiverSocket->Close();
		mServerIsOpen = kFALSE;
		std::cout << "**** TRemiAcqirisSocketEventSource: Server " << mServerUrl << " failed to send the buffer " << std::endl;
		return 0;
	}
	else
	{
		// We have a buffer
		TRemiAcqirisBuffer* BufferHandler = new TRemiAcqirisBuffer(mBuffer, mSizeOfBuffer);
		fAcqirisEvent.clear();
		fAcqirisEvent.readEvent(BufferHandler);
		delete BufferHandler;

		mEventHasData = kTRUE;
		mMinibunchIndex = 0;
		return 1;
	}
}

/** \brief Receive data from socket as object */
Int_t TRemiAcqirisSocketEventSource::recvObject(TSocket * s)
{
	TMessage *msg = 0;

//	s->Select(TSocket::kRead);
	Int_t ret = s->Recv(msg);

	if (!msg) {
		std::cout << "Failed to read message, got: " << ret << std::endl;
		return ret;
	}

//	TObjTArrC * arr = (TObjTArrC *) msg->ReadObject(msg->GetClass());
	TObjTArrC * arr = dynamic_cast<TObjTArrC *>(msg->ReadObject(msg->GetClass()));

	if (!arr) {
		std::cout << "Failed to unpack array, got: " << ret << std::endl;
		return ret;
	}

	Char_t * buf = arr->GetArray();
	Int_t len = arr->GetSize();

	if (mSizeOfBuffer < len) {
		std::cout << "Increase buffer size from " << mSizeOfBuffer << " to 2*" << len << std::endl;
		delete [] mBuffer;
		mBuffer = new char[len*2];
		mSizeOfBuffer = len*2;
	}

	std::copy(buf+sizeof(len), buf+len, mBuffer);

	delete arr;
	delete msg;

	return ret;
}


/**
 * \brief Get the last event from the temporary buffer and process it
 */
Bool_t TRemiAcqirisSocketEventSource::GetEventFromBuffer()
{
	fAcqirisEvent = *fEventBuffer.back();
	delete fEventBuffer.back();
	fEventBuffer.pop_back();
	mEventHasData = kTRUE;
	mMinibunchIndex = 0;
	return kTRUE;
}

/**
 * \brief Executes waiting processes every 5 ms
 */
void TRemiAcqirisSocketEventSource::RunWaitingProcesses()
{
	if ( ((Long64_t) gSystem->Now() - mWaitingProcessesThen) > 5)
	{
		gSystem->ProcessEvents();
		mWaitingProcessesThen = (Long64_t) gSystem->Now();
	}
}

/**
 * \brief Checks for reconnection in the defined interval
 */
void TRemiAcqirisSocketEventSource::CheckForReconnect()
{
	if ( ((Long64_t) gSystem->Now() - mReconnectThen) > mReconnectInterval)
	{
		OpenSocket();
		mReconnectThen = (Long64_t) gSystem->Now();
	}
}



ClassImp(TRemiAcqirisSocketEventSource);
