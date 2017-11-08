//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the main header for the ExtractMBS step
//----------------------------------------------------------------

#ifndef SEXTRACTMBSPROC_H
#define SEXTRACTMBSPROC_H

#define MBSCONFIG "cExtractMbs.conf"


#include "../Core/TRemiIncludeFunctions.h"

#include "TGo4EventProcessor.h"

#include "TRandom3.h"

#include <vector>

class TRemiExtractEvent;

class TGo4MbsEvent;
class TGo4MbsSubEvent;
class TGo4WinCond;

class TH1I;


/**
 * \brief Step processor for event extraction from MBS TDC and ADC raw data
 */

class TRemiExtractMbsProc : public TGo4EventProcessor, public TRemiIncludeFunctions
{
	public :
		TRemiExtractMbsProc();
		TRemiExtractMbsProc(const char* name);
		~TRemiExtractMbsProc();
		virtual Bool_t BuildEvent(TGo4EventElement* dest);
		/** This method checks if event class is suited for the source */
		virtual Bool_t CheckEventClass(TClass* ul);
		
	private :
		TGo4MbsEvent*	fInput;
		TRemiExtractEvent*	fOutput;
		
		TGo4MbsSubEvent* fSubevt;
		std::vector< std::vector<TH1I*> > fTDCraw; ///< TDC raw data histograms
		std::vector<TH1I*>	fTDCrawAll; ///< TDC raw data histograms for all hits
		std::vector<TH1I*>	fTDChits;	///< Histogram showing no. if hits per event for each TDC channel
		std::vector<TH1I*> fADCraw; ///< ADC raw data histograms

		UInt_t mNumTdcChan; ///< No. of TDC channels
		UInt_t mNumTdcHits; ///< No. of TDC hits for histograms
		UInt_t mNumAdcChan; ///< No. of ADC channels

		Int_t mTdcGeoAddress;	///< GEO address of the TDC
		Int_t mAdcGeoAddress;	///< GEO address of the ADC

		Bool_t	mUseRandomizer;	///< Randomize values when writing to output ?
		TRandom3 fRandom;	///< Random number generator

	ClassDef(TRemiExtractMbsProc, 1);
};

#endif
