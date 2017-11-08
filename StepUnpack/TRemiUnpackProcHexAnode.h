//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class calculates the position of a hit on a hexanode
//----------------------------------------------------------------

#ifndef SDELAYHEX_H
#define SDELAYHEX_H

#include "TRemiUnpackProcPos.h"

#include "TString.h"
#include "Rtypes.h"

#include <vector>

class TRemiHexAnodeParameter;
class TRemiReconstructParameter;
class TRemiUnpackProcReconstruct;

class TGo4WinCond;
class TGo4Picture;

class TH2I;
class TH1I;

/**
 * \brief Extended class to get event information from a single 3-wire hexagonal delayline detector
 * 
 * Provides method for position calculation and inherits method for time-of-flight calculation from class TRemiUnpackProcTof
 */

class TRemiUnpackProcHexAnode : public TRemiUnpackProcPos
{
	public :
		TRemiUnpackProcHexAnode(); 
		TRemiUnpackProcHexAnode(ConfigFile* UnpackConf, TString particle);
		virtual ~TRemiUnpackProcHexAnode();
		void CalculatePosition();
		
		void SortDetecor();
		
	protected :
		enum eHexAnodeReconstruct {kU1 = 1, kU2 = 2, kV1 = 3, kV2 = 4, kW1 = 5, kW2 = 6};
		
		void AddUVWPosition(Float_t McpValue, Float_t U1Value, Float_t U2Value, Float_t V1Value, Float_t V2Value, Float_t W1Value, Float_t W2Value);
		void AddUVPosition(Float_t McpValue, Float_t U1Value, Float_t U2Value, Float_t V1Value, Float_t V2Value);
		void AddUWPosition(Float_t McpValue, Float_t U1Value, Float_t U2Value, Float_t W1Value, Float_t W2Value);
		void AddVWPosition(Float_t McpValue, Float_t V1Value, Float_t V2Value, Float_t W1Value, Float_t W2Value);
		void AddXYPosition(Float_t McpValue, TRemiXY &XYvalues);
		
		Float_t	GetScaledUvalue (Float_t Udiff);
		Float_t	GetScaledVvalue (Float_t Vdiff);
		Float_t	GetScaledWvalue (Float_t Wdiff);
		
		TRemiXY	CalculateXYfromUV (Float_t Uvalue, Float_t Vvalue);
		TRemiXY	CalculateXYfromUW (Float_t Uvalue, Float_t Wvalue);
		TRemiXY	CalculateXYfromVW (Float_t Vvalue, Float_t Wvalue);
		
		void Reconstruct ();
		Int_t ReconstructMcpUVW ();
		Int_t ReconstructMcpUV ();
		Int_t ReconstructMcpUW ();
		Int_t ReconstructMcpVW ();
		Int_t ReconstructHexAnode ();
		
		Bool_t ReconstructHexAnodeSingleHit (std::vector< Float_t > &KnownList1, std::vector< Float_t > &KnownList2, TGo4WinCond* KnownConTimeSum, TGo4WinCond* KnownConSort1, TGo4WinCond* KnownConSort2, std::vector<Float_t>::iterator &McpIt, std::vector< Float_t > &UnknownAList1, TGo4WinCond* UnknownAConTimeSum, TGo4WinCond* UnKnownAConSort1, TGo4WinCond* UnKnownAConSort2, TRemiUnpackProcHexAnode::eHexAnodeReconstruct WhichReconstructA, std::vector< Float_t > &UnknownBList1, TGo4WinCond* UnknownBConTimeSum, TGo4WinCond* UnKnownBConSort1, TGo4WinCond* UnKnownBConSort2, TRemiUnpackProcHexAnode::eHexAnodeReconstruct WhichReconstructB);
		void AddReconstructedHexanodeHit( std::vector<Float_t>::iterator &McpIt, std::vector<Float_t>::iterator &Known1It, std::vector<Float_t>::iterator &Known2It, std::vector<Float_t>::iterator &UnknownA1It, Float_t &NewValueA, TRemiUnpackProcHexAnode::eHexAnodeReconstruct WhichReconstructA, TRemiUnpackProcHexAnode::eHexAnodeReconstruct WhichReconstructB );
		
		
		void FindBestScalefactors(Float_t &uvalue, Float_t &vvalue, Float_t &wvalue);
		
		/** \name constants of this detector */
		//@{
		TString mUlabel; /**< label for U layer in internal names */
		TString mVlabel; /**< label for V layer in internal names */
		TString mWlabel; /**< label for W layer in internal names */
		
		Int_t mTdcChanU1; ///< Tdc channel of delay-line signal u1
		Int_t mTdcChanU2; ///< Tdc channel of delay-line signal u2
		Int_t mTdcChanV1; ///< Tdc channel of delay-line signal v1
		Int_t mTdcChanV2; ///< Tdc channel of delay-line signal v2
		Int_t mTdcChanW1; ///< Tdc channel of delay-line signal w1
		Int_t mTdcChanW2; ///< Tdc channel of delay-line signal w2
		//@}
		
		TRemiHexAnodeParameter* fParaPos; /**< User parameter for the position calculation */
		
		
		/** \name Pictures and Spectra for layer overlap check */
		//@{
		TGo4Picture*	fPicOverlap;	///< Picture for HexAnode layer overlap check
	
		TH2I*	fYuv_yuw;	///< Spectrum to check overlap of different layers
		TH2I*	fYvw_yuv;	///< Spectrum to check overlap of different layers
		TH2I*	fYuw_yvw;	///< Spectrum to check overlap of different layers
		TH2I*	fXuv_xuw;	///< Spectrum to check overlap of different layers
		TH2I*	fXvw_xuv;	///< Spectrum to check overlap of different layers
		TH2I*	fXuw_xvw;	///< Spectrum to check overlap of different layers
		//@}
		
		/** \name Histograms including all hits */
		//@{
		TH1I*	fAllTsum_u;	///< Time sum spectra u-wire
		TH1I*	fAllTsum_v;	///< Time sum spectra v-wire
		TH1I*	fAllTsum_w;	///< Time sum spectra w-wire
		TH1I*	fU1Hits;	///< No. of U1 hits per shot
		TH1I*	fU2Hits;	///< No. of U2 hits per shot
		TH1I*	fV1Hits;	///< No. of V1 hits per shot
		TH1I*	fV2Hits;	///< No. of V2 hits per shot
		TH1I*	fW1Hits;	///< No. of W1 hits per shot
		TH1I*	fW2Hits;	///< No. of W2 hits per shot
		TH2I*	fU1McpHits;	///< No. of U1 hits vs. MCP hits per shot
		TH2I*	fU2McpHits;	///< No. of U2 hits vs. MCP hits per shot
		TH2I*	fV1McpHits;	///< No. of V1 hits vs. MCP hits per shot
		TH2I*	fV2McpHits;	///< No. of V2 hits vs. MCP hits per shot
		TH2I*	fW1McpHits;	///< No. of W1 hits vs. MCP hits per shot
		TH2I*	fW2McpHits;	///< No. of W2 hits vs. MCP hits per shot
		//@}
		
		/** \name Histograms for sorted hits */
		//@{
		TH1I*	fSortTsum_u;	///< Time sum spectra u-wire
		TH1I*	fSortTsum_v;	///< Time sum spectra v-wire
		TH1I*	fSortTsum_w;	///< Time sum spectra w-wire
		TH2I*	fSortU_Tsum_u;	///< Time sum us. coordinate U-wire
		TH2I*	fSortV_Tsum_v;	///< Time sum us. coordinate V-wire
		TH2I*	fSortW_Tsum_w;	///< Time sum us. coordinate W-wire
		TH1I*	fSortedEvents;	///< No. sorted events
		TH2I*	fSortvsMcpHits;	///< No. of sorted events vs. Mcp hits
		TH1I*	fSortedYield;	///< Sorted Events yield as a quotient
		
		TH1I*	fSortU;	///< U values of sorted hits
		TH1I*	fSortV;	///< V values of sorted hits
		TH1I*	fSortW;	///< W values of sorted hits
		
		TH2I*	fSortXY_uv;	///< Position from U and V wire
		TH2I*	fSortXY_uw;	///< Position from U and W wire
		TH2I*	fSortXY_vw;	///< Position from V and W wire
		//@}
		
		/** \name Sort-Check spectra and conditions: limiting difference between MCP signal and delayline signals */
		//@{
		TH1I*	fSortU1Mcp;	///< Plots U1 minus MCP signal
		TH1I*	fSortU2Mcp;	///< Plots U2 minus MCP signal
		TH1I*	fSortV1Mcp;	///< Plots V1 minus MCP signal
		TH1I*	fSortV2Mcp;	///< Plots V1 minus MCP signal
		TH1I*	fSortW1Mcp;	///< Plots W1 minus MCP signal
		TH1I*	fSortW2Mcp;	///< Plots W1 minus MCP signal
		
		TGo4WinCond*	fconSortU1;	///< Window condition to determine range of U1 signal
		TGo4WinCond*	fconSortU2;	///< Window condition to determine range of U2 signal
		TGo4WinCond*	fconSortV1;	///< Window condition to determine range of V1 signal
		TGo4WinCond*	fconSortV2;	///< Window condition to determine range of V2 signal
		TGo4WinCond*	fconSortW1;	///< Window condition to determine range of W1 signal
		TGo4WinCond*	fconSortW2;	///< Window condition to determine range of W2 signal
		//@}
		
		/** \name  Hex anode scale factor optimisation */
		//@{
		Bool_t	mUseScalef;	///< Use scale factor optimisation?
		Float_t	mScalefMin;	///< Minimum value to look for best scale factors
		Float_t	mScalefMax;	///< Maxmimum value to look for best scale factors
		Float_t	mScalefStepsize;	///< Stepsize when looking for best scale factors
		Float_t	mScalefMinerror;	///< Minimal error achieved with scale factors
		TH2I*	fScalefactors;	///< Histograms showing optimal scale factors for each event
		//@}
		
		/** \name Time sum conditions */
		//@{
		TGo4WinCond*	fconTsum_u;	///< Time sum condition u wire
		TGo4WinCond*	fconTsum_v;	///< Time sum condition v wire
		TGo4WinCond*	fconTsum_w;	///< Time sum condition w wire

		TGo4Picture*	fPicTimeSums;	///< Picture for Time Sums
		//@}

		/** \name Reconstruction objects */
		//@{
		Bool_t	mUseReconstruct;	///< Saves if reconstruction is used for events
		TRemiUnpackProcReconstruct*	fReconstruct;	///< Contains functions used for reconstruction
		TRemiReconstructParameter*	fParaReconst;	///< Parameter object for reconstruction
		Char_t	mNumReconstructSingleKnownU;
		Char_t	mNumReconstructSingleKnownV;
		Char_t	mNumReconstructSingleKnownW;
		Char_t	mNumReconstructDoubleKnownU;
		Char_t	mNumReconstructDoubleKnownV;
		Char_t	mNumReconstructDoubleKnownW;
		//@}
		
		/** \name Reconstruction histograms */
		//@{
		TH1I*	fRecoMcp;	///< Histograms to count total reconstructed MCP signals
		TH1I*	fRecoMcpUV;	///< Histograms to count reconstructed MCP signals from U and V layer
		TH1I*	fRecoMcpUW;	///< Histograms to count reconstructed MCP signals from U and W layer
		TH1I*	fRecoMcpVW;	///< Histograms to count reconstructed MCP signals from V and W layer
		TH1I*	fRecoMcpUVW;	///< Histograms to count reconstructed MCP signals from all three layers
		TH1I*	fRecoMcpSorted;	///< Histogram counting number of hits after MCP reconstruction
		TH2I*	fRecoMcpSortvsMcpHits;	///< No. of sorted events after MCP reconstruction vs. Mcp hits
		TH1I*	fRecoMcpSortedYield;	///< Sorted Events yield after MCP reconstruction as a quotient
		
		TH1I*	fRecoDline;	///< Histograms to count total reconstructed Delayline signals
		TH1I*	fRecoDlineSingleKnownU;	///< Histograms to count reconstructed single Delayline signals with known U hit
		TH1I*	fRecoDlineSingleKnownV;	///< Histograms to count reconstructed single Delayline signals with known V hit
		TH1I*	fRecoDlineSingleKnownW;	///< Histograms to count reconstructed single Delayline signals with known W hit
		TH1I*	fRecoDlineDoubleKnownU;	///< Histograms to count reconstructed double Delayline signals with known U hit
		TH1I*	fRecoDlineDoubleKnownV;	///< Histograms to count reconstructed double Delayline signals with known V hit
		TH1I*	fRecoDlineDoubleKnownW;	///< Histograms to count reconstructed double Delayline signals with known W hit
		TH1I*	fRecoDlineSorted;	///< Histogram counting number of hits after Delayline reconstruction
		TH2I*	fRecoDlineSortvsMcpHits;	///< No. of sorted events after Delayline reconstruction vs. Mcp hits
		TH1I*	fRecoDlineSortedYield;	///< Sorted Events yield after Delayline reconstruction as a quotient
		//@}

	ClassDef(TRemiUnpackProcHexAnode, 1);
};

#endif
