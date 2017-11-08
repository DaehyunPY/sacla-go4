//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Base class for time-of-flight retrieval of a hit. This is the base class for position calculation
//----------------------------------------------------------------

#ifndef STOF_H
#define STOF_H

#include "../Core/TRemiIncludeFunctions.h"

#include "TString.h"
#include "TRandom3.h"

#include <vector>

class TRemiExtractEvent;
class TRemiUnpackParameter;
class TRemiRollingGraph;

class ConfigFile;

class TGo4WinCond;

class TH1I;
class TH2I;

/**
 * \brief Basic class for event extraction from raw data of one detector
 * 
 * Provides method for time of flight calculation.
 * 
 * If only time-of-flight information is needed, one can directly make an object of this class
 * 
 * For particles where position is detected together with time-of-flight extended classes through TRemiUnpackProcPos.
 */

class TRemiUnpackProcTof : public TRemiIncludeFunctions
{
	public :
		TRemiUnpackProcTof();
		TRemiUnpackProcTof(ConfigFile* UnpackConf, TString particle);
		virtual ~TRemiUnpackProcTof();
		void SetInput (TRemiExtractEvent *Input);
		void CalculateTof();
		void TestTofCond();
		void FillTofHistograms();
		void GetTof(std::vector<Double_t> &Tof) const;
		void GetCondTof(std::vector<Bool_t> &CondTof) const;
		void SetTof(std::vector<Double_t> tof);
		void SetDelay(Float_t delay); 
		UInt_t GetNumTofHits();
		
		Bool_t TestTofCondition (Float_t Tof);
		
		
	protected :
		Double_t CalculateSingleTof(Float_t McpTime);
		
		TString	mShortName; //!< Is added to the internal name of all created analysis objects for this detector 
		TString	mLongName; //!< Is added to the internal descripton of all created analysis objects for this detector 
		
		Bool_t	mUsePulser;	///< Is a external pulser used as timing reference
		Int_t	mTdcChanPulser;	///< No. of the TDC channel containing the pulser signal
		Int_t	mTdcChanMCP;	///< No. of the TDC channel containing this particle's MCP signal
		Float_t	mTdcBins2ns;	///< conversion factor from Tdc bins to nano seconds
		
		UInt_t	mNumTofHits;	///< No. of Tof hits in the current event
		
		std::vector<Double_t>	mTof;	//!< Current time-of-flight
		std::vector<Double_t>	mTofPrevious;	//!< Previous time-of-flight
		std::vector<Bool_t>	mFlagTofAll;	//!< Current flags for good time-of-flight in "all" condition
		
		TRemiExtractEvent	*fRawData; ///< Contains raw data as collected by TRemiUnpackProcExtract
		TRemiUnpackParameter	*fParaGlobal; ///< Global parameter for the unpack step
		
		TH1I*	fMcpHits;	///< No. of MCP hits per shot
		TRemiRollingGraph*	fGraphMcpHits;	///< Rolling graph for online monitoring of the no. of Mcp hits
		
		TH1I* 	fTofAll;	//!< Histogram for all times-of-flight
		TH2I*	fPiPiCoAll;			//PiPiCo for all hits
		TH2I*	fPiPiNoAll;			//PiPiNo for all hits for un-correlated bunches
		TH1I*	fTofDiffAll;	///< Histogram showing the difference of all neighbouring times-of-flight
		
		TGo4WinCond* 	fcon_TofAll;	//!< Time-of-flight condition for "all" histogram
		
		Bool_t	mUseDelay;		///< Generate spectra with delay stage?
		Float_t mDelayValue;		///< Delay stage value
		TH2I*	fDelayTofAll;		///< Delay vs. Tof spectrum for all hits
		TH1I*	fDelayCTofAll;		///< Delay spectrum with Time-of-flight condition
		
		TRandom3	fRandom;	///< Random number generator
		
	ClassDef(TRemiUnpackProcTof, 1);
};

#endif
