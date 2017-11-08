//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class provides TDC and ADC raw data arrays from Acqiris data file
//----------------------------------------------------------------

#ifndef TREMIACQIRISEVENTSOURCE_H
#define TREMIACQIRISEVENTSOURCE_H

#include "../Core/TRemiIncludeFunctions.h"
#include "TRemiAcqirisPeak.h"
#include "TRemiAcqirisAdcPulse.h"
#include "TRemiAcqirisEvent.h"

#include "TGo4EventSource.h"

#include "Rtypes.h"

#include <vector>

class TRemiAcqirisSingleShotParameter;
class TRemiExtractEvent;
class TRemiAcqirisChannel;
class TRemiAcqirisIO;

class TGo4UserSourceParameter;
class TGo4WinCond;

class TH1I;
class TH2I;
class TClass;


class TRemiAcqirisEventSource : public TGo4EventSource, public TRemiIncludeFunctions
{
	public:
		enum EInputType {kFile=0, kSocket=1};

		TRemiAcqirisEventSource();

		/** Create source specifying values directly */
		TRemiAcqirisEventSource(const char* name, const char* args, Int_t port);

		/** Creat source from setup within usersource parameter object */
		TRemiAcqirisEventSource(TGo4UserSourceParameter* par);

		virtual ~TRemiAcqirisEventSource();


		/** This method checks if event class is suited for the source */
		virtual Bool_t CheckEventClass(TClass* cl);

		/** This methods actually fills the target event class which is passed as pointer.
		* Uses the latest event which is referenced
		* by fxEvent or fxBuffer. Does _not_ fetch a new event
		* from source, therefore one source event may be used
		* to fill several TRemiExtractEvent classes. */
		virtual Bool_t BuildEvent(TGo4EventElement* dest) = 0;

		
	protected:
		void InitGeneral();
		
		Bool_t SingleShotEvent();
		Int_t FillTdcChannel(Int_t tdctarget, TRemiAcqirisChannel* SourceChannel);
		Int_t FillAdcChannel(Int_t adctarget, TRemiAcqirisChannel* SourceChannel);
		
		TRemiExtractEvent* 	fOutput;	///< Output event
		
		UInt_t 	mNumAdcChan;	///< number of ADC channels
		UInt_t 	mNumTdcChan;	///< number of TDC channels
		UInt_t 	mNumTdcHits;	///< number of TDC hits per channel
		
		Int_t	mAdcChanEventId;	///< ADC channel for event ID
		Int_t	mAdcChanMbIndex;	///< ADC channel for minibunch index
		Int_t	mAdcChanOffset;	///< ADC channel for the offset of the current minibunch window
		Int_t	mAdcChanBunchId;	///< ADC channel for FLASH bunch ID
		Int_t	mAdcChanPpDelay;	///< ADC channel for pump-probe laser delay
		Int_t	mAdcChanStreakCam;	///ADC channel for Streak Camera
		Int_t	mAdcChanCrossCorr;	///ADC channel for Cross Correlator
		Int_t	mAdcChanBam;	///ADC channel for Bam
		Int_t	mAdcChanStreakCamFast;	///ADC channel for Straek Camera Fast
		Int_t	mAdcChanPpDelayEnc;	///< ADC channel for pump-probe laser delay from encoder
		
		TRemiAcqirisIO*	fDataInput;	///< I/O object that handles the communication with the data input
		Int_t	mNumAcqChan;	///< Number of Acqiris channels
		
		TRemiAcqirisEvent	fAcqirisEvent;	///< Containing and processing the full Event for each pulse train
		Bool_t	mEventHasData; ///< Is there still data in fAcqirisEvent that has not been converted into an Output event?
		
		TRemiAcqirisSingleShotParameter*	fParaSingleShot;	///< Parameters for the extraction of the single shot events
		Int_t mMinibunchIndex;	///< Current minibunch index with in a pulse traun
		TRemiAcqirisPeak	fPeakStart;	///< Peak that represents the start of the range to extract into final object
		TRemiAcqirisPeak	fPeakEnd;	///< Peak that represents the end of the range to extract into final object
		TRemiAcqirisAdcPulse	fAdcStart;	///< Peak that represents the start of the range to extract into final object
		TRemiAcqirisAdcPulse	fAdcEnd;	///< Peak that represents the end of the range to extract into final object
		
		Bool_t mValidOutputEvent;	///< Flag to activate if a valid output event has been produced
		
		std::vector<TH1I*>	fHistTdcAll;	///< Histograms showing all peaks per TDC output channel
		std::vector< std::vector<TH1I*> >	fHistTdcHits;	///< Histograms showing all peaks per TDC output channel per hit
		std::vector<TH1I*>	fHistTdcMinibunch;	///< Histogram showing total number of hits per channel for each minibunch
		std::vector<TH2I*>	fHistTdcMbHits;	///< Histogram showing the distribution of hits per minibunch for each channel
		TH1I*	fHistTdcChanHitCount;	///< Histogram showing a hit counter for all TDC channels
		
		std::vector<TH1I*>	fHistAdc;	///< Histogram showing ADC results
		std::vector<TH2I*>	fHistAdcMinibunch;	///< Histogram showing ADC results
		
		TGo4WinCond*	fConTdcVeto;	///< Condition to veto hits from a certain time range
		
		std::vector<Bool_t>	mUseTdcVeto;	///< List of booleans that define if vetoeing is used for a channel
		std::vector<Int_t>	mTdcSourceChannel;	///< List of Source channel for each TDC channel
		std::vector<Short_t>	mTdcSourcePolarity;	///< List of source polarity for each TDC channel
		
		std::vector<Int_t>	mAdcSourceChannel;	///< List of Source channel for each ADC channel
		std::vector<Bool_t>	mAdcChannelPerMinibunch;	///< List of flags if ADC value is evaluated per minibunch
		std::vector<Short_t>	mAdcChannelWhichPulse;	///< List of identifiers which pulses are to be used for ADC value
		

	ClassDef(TRemiAcqirisEventSource, 1);
};

#endif //TGO4MBSSOURCE_H
