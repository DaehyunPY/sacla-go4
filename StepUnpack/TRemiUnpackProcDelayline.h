//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class calculates the position of a hit on a delayline
//----------------------------------------------------------------

#ifndef SDELAY_H
#define SDELAY_H

#include "TRemiUnpackProcPos.h"

#include "TString.h"
#include "Rtypes.h"

#include <vector>

class TRemiDelaylineParameter;
class TRemiReconstructParameter;
class TRemiUnpackProcReconstruct;
class TRemiRollingGraph;

class ConfigFile;

class TGo4Picture;
class TGo4WinCond;

class TH1I;


/**
 * \brief Extended class to get event information from single 2-wire delayline detector
 * 
 * Provides method for position calculation and inherits method for time-of-flight calculation from class TRemiUnpackProcTof
 */

class TRemiUnpackProcDelayline : public TRemiUnpackProcPos
{
	public :
		TRemiUnpackProcDelayline(); 
		TRemiUnpackProcDelayline(ConfigFile* UnpackConf, TString particle);
		virtual ~TRemiUnpackProcDelayline();
		void CalculatePosition();
		
		void SortDetecor();
		
	protected :
		enum eDelaylineReconstruct {kX1 = 1, kX2 = 2, kY1 = 3, kY2 = 4};
		
		void AddPosition(Float_t McpValue, Float_t X1Value, Float_t X2Value, Float_t Y1Value, Float_t Y2Value);
		
		void Reconstruct ();
		Int_t ReconstructMcp ();
		Int_t ReconstructDelayline ();
		
		Bool_t ReconstructDelaylineSingleHit (std::vector< Float_t > &KnownList1, std::vector< Float_t > &KnownList2, TGo4WinCond* KnownConTimeSum, TGo4WinCond* KnownConSort1, TGo4WinCond* KnownConSort2, std::vector<Float_t>::iterator &McpIt, std::vector< Float_t > &UnknownList1, TGo4WinCond* UnknownConTimeSum, TGo4WinCond* UnKnownConSort1, TGo4WinCond* UnKnownConSort2, TRemiUnpackProcDelayline::eDelaylineReconstruct WhichReconstruct);
		
		TString mXlabel; /**< label for U layer in internal names */
		TString mYlabel; /**< label for V layer in internal names */
		
		Int_t mTdcChanX1; ///< Tdc channel of delay-line signal x1
		Int_t mTdcChanX2; ///< Tdc channel of delay-line signal x2
		Int_t mTdcChanY1; ///< Tdc channel of delay-line signal y1
		Int_t mTdcChanY2; ///< Tdc channel of delay-line signal y2
		
		TRemiDelaylineParameter* fParaPos; /**< User parameter for the position calculation */
		
		// Histograms including all hits
		TH1I*	fAllTsum_x;	///< Time sum spectra x-wire
		TH1I*	fAllTsum_y;	///< Time sum spectra y-wire
		TH1I*	fX1Hits;	///< No. of X1 hits per shot
		TH1I*	fX2Hits;	///< No. of X2 hits per shot
		TH1I*	fY1Hits;	///< No. of Y1 hits per shot
		TH1I*	fY2Hits;	///< No. of Y2 hits per shot
		TH2I*	fX1McpHits;	///< No. of X1 hits vs. MCP hits per shot
		TH2I*	fX2McpHits;	///< No. of X2 hits vs. MCP hits per shot
		TH2I*	fY1McpHits;	///< No. of Y1 hits vs. MCP hits per shot
		TH2I*	fY2McpHits;	///< No. of Y2 hits vs. MCP hits per shot
		
		// Histograms for sorted hits
		TH1I*	fSortTsum_x;	///< Time sum spectra x-wire
		TH1I*	fSortTsum_y;	///< Time sum spectra y-wire
		TH2I*	fSortX_Tsum_x;	///< Time sum vs. coordinate X-wire
		TH2I*	fSortY_Tsum_y;	///< Time sum vs. coordinate Y-wire
		TH1I*	fSortedEvents;	///< No. sorted events
		TH2I*	fSortvsMcpHits;	///< No. of sorted events vs. Mcp hits
		TH1I*	fSortedYield;	///< Sorted Events yield as a quotient
		
		TH1I*	fSortX1Mcp;	///< Plots X1 minus MCP signal
		TH1I*	fSortX2Mcp;	///< Plots X2 minus MCP signal
		TH1I*	fSortY1Mcp;	///< Plots Y1 minus MCP signal
		TH1I*	fSortY2Mcp;	///< Plots Y1 minus MCP signal
		
		TRemiRollingGraph*	fGraphX1Hits;	///< No. of X1 hits per shot
		TRemiRollingGraph*	fGraphX2Hits;	///< No. of X2 hits per shot
		TRemiRollingGraph*	fGraphY1Hits;	///< No. of Y1 hits per shot
		TRemiRollingGraph*	fGraphY2Hits;	///< No. of Y2 hits per shot
		TRemiRollingGraph*	fGraphSortedEvents;	///< No. sorted events
		TGo4Picture*	fPictureRollingGraphs;	///< Picture with the rolling graphs
		
		// Reconstruction histograms
		TH1I*	fRecoMcp;	///< Histograms to count reconstructed MCP signals
		TH1I*	fRecoMcpSorted;	///< Histogram counting number of hits after MCP reconstruction
		TH2I*	fRecoMcpSortvsMcpHits;	///< No. of sorted events after MCP reconstruction vs. Mcp hits
		TH1I*	fRecoMcpSortedYield;	///< Sorted Events yield after MCP reconstruction as a quotient
		
		TH1I*	fRecoDline;	///< Histograms to count reconstructed Delayline signals
		TH1I*	fRecoDlineSorted;	///< Histogram counting number of hits after Delayline reconstruction
		TH2I*	fRecoDlineSortvsMcpHits;	///< No. of sorted events after Delayline reconstruction vs. Mcp hits
		TH1I*	fRecoDlineSortedYield;	///< Sorted Events yield after Delayline reconstruction as a quotient
		
		TGo4WinCond*	fconTsum_x;	///< Time sum condition x wire
		TGo4WinCond*	fconTsum_y;	///< Time sum condition y wire
		
		TGo4Picture*	fPicTimeSums;	///< Picture showing both time sum conditions

		Bool_t	mUseReconstruct;	///< Saves if reconstruction is used for events
		TRemiUnpackProcReconstruct*	fReconstruct;	///< Contains functions used for reconstruction
		TRemiReconstructParameter*	fParaReconst;	///< Parameter object for reconstruction
		
		TGo4WinCond*	fconSortX1;	///< Window condition to determine range of X1 signal
		TGo4WinCond*	fconSortX2;	///< Window condition to determine range of X2 signal
		TGo4WinCond*	fconSortY1;	///< Window condition to determine range of X1 signal
		TGo4WinCond*	fconSortY2;	///< Window condition to determine range of X2 signal

	ClassDef(TRemiUnpackProcDelayline, 1);
};

#endif
