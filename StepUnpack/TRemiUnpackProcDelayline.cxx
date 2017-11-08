//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class calculates the position of a hit on a delayline
//----------------------------------------------------------------

#include "TRemiUnpackProcDelayline.h"

#include "Parameters/TRemiDelaylineParameter.h"
#include "Parameters/TRemiReconstructParameter.h"
#include "TRemiUnpackProcReconstruct.h"
#include "../Core/Point/TRemiDetectorHit.h"
#include "../EventElement/TRemiExtractEvent.h"
#include "../Core/RootMod/TRemiRollingGraph.h"

#include "../Core/Extern/ConfigFile.h"

#include "TGo4WinCond.h"
#include "TGo4Picture.h"

#include "TH1.h"
#include "TH2.h"

/**
 * \brief Default constructor. Not to be used!
 */
TRemiUnpackProcDelayline::TRemiUnpackProcDelayline()
{
}

/**
 * \brief Constructor to directly build an object of this class using configuration file
 */
TRemiUnpackProcDelayline::TRemiUnpackProcDelayline(ConfigFile* UnpackConf, TString particle) :
	TRemiUnpackProcPos(UnpackConf, particle)
{
	TString fieldname, binsname, minname, maxname, basename="unp_", hname, htitle, folder, basefolder = "Unpack", cname, ctitle, paraname, hxtitle, hytitle, pname, ptitle;

	//------------------------------------------//
	// Read information from configuration file //
	//------------------------------------------//

	basename = basename + particle + "_";

	fieldname = basename + "xlabel";
	mXlabel = fConfigFile->read<TString>(fieldname.Data(), "x");
	fieldname = basename + "ylabel";
	mYlabel = fConfigFile->read<TString>(fieldname.Data(), "y");

	fieldname = basename + "tdc_x1";
	mTdcChanX1 = fConfigFile->read (fieldname.Data(), 1);
	fieldname = basename + "tdc_x2";
	mTdcChanX2 = fConfigFile->read (fieldname.Data(), 2);
	fieldname = basename + "tdc_y1";
	mTdcChanY1 = fConfigFile->read (fieldname.Data(), 3);
	fieldname = basename + "tdc_y2";
	mTdcChanY2 = fConfigFile->read (fieldname.Data(), 4);

	fieldname = basename + "hist_xy_min";
	Double_t hist_xy_min = fConfigFile->read (fieldname.Data(), -50);
	fieldname = basename + "hist_xy_max";
	Double_t hist_xy_max = fConfigFile->read (fieldname.Data(), 50);
	fieldname = basename + "hist_xy_bins";
	Int_t hist_xy_bins = fConfigFile->read (fieldname.Data(), 200);

	fieldname = basename + "hist_tsum_min";
	Double_t hist_tsum_min = fConfigFile->read (fieldname.Data(), 0.);
	fieldname = basename + "hist_tsum_max";
	Double_t hist_tsum_max = fConfigFile->read (fieldname.Data(), 20000.);
	fieldname = basename + "hist_tsum_bins";
	Int_t hist_tsum_bins = fConfigFile->read (fieldname.Data(), 5000);
	
	fieldname = basename + "hist_hits_bins";
	Int_t hist_hits_bins = fConfigFile->read(fieldname.Data(), 50);

	fieldname = basename + "use_reconstruct";
	mUseReconstruct = fConfigFile->read (fieldname.Data(), false);

	fieldname = basename + "graph_hits_num_points";
	Int_t rolling_graph_num = fConfigFile->read (fieldname.Data(), 10);
	fieldname = basename + "graph_hits_num_average";
	Int_t rolling_graph_average = fConfigFile->read(fieldname.Data(), 100);
	
	
	//-----------------//
	// Make Parameters //
	//-----------------//
	folder = basefolder + "/" + mShortName;
	paraname = "Par " + mShortName + " delayline";
	fParaPos = MakeParam<TRemiDelaylineParameter>(paraname, folder);
	
	if (mUseReconstruct)
	{
		paraname = "Par " + mShortName + " reconstruct";
		fParaReconst = MakeParam<TRemiReconstructParameter>(paraname, folder);
	}

	//-------------------//
	// Create histograms //
	//-------------------//

	// Total counts spectra
	folder = basefolder + "/" + mShortName + "_all/det/counts";
	
	hname = "X1Hits_" + mShortName;
	htitle = "No. of X1 Hits " + mLongName + " detector";
	fX1Hits = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	hname = "X2Hits_" + mShortName;
	htitle = "No. of X2 Hits " + mLongName + " detector";
	fX2Hits = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	hname = "Y1Hits_" + mShortName;
	htitle = "No. of Y1 Hits " + mLongName + " detector";
	fY1Hits = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	hname = "Y2Hits_" + mShortName;
	htitle = "No. of Y2 Hits " + mLongName + " detector";
	fY2Hits = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	hname = "X1HitsVsMcpHits_" + mShortName;
	htitle = "No. of X1 hits vs. MCP hits " + mLongName + " detector";
	fX1McpHits = MakeH2<TH2I> (hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. X1 hits");
	
	hname = "X2HitsVsMcpHits_" + mShortName;
	htitle = "No. of X2 hits vs. MCP hits " + mLongName + " detector";
	fX2McpHits = MakeH2<TH2I> (hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. X2 hits");
	
	hname = "Y1HitsVsMcpHits_" + mShortName;
	htitle = "No. of Y1 hits vs. MCP hits " + mLongName + " detector";
	fY1McpHits = MakeH2<TH2I> (hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. Y1 hits");
	
	hname = "Y2HitsVsMcpHits_" + mShortName;
	htitle = "No. of Y2 hits vs. MCP hits " + mLongName + " detector";
	fY2McpHits = MakeH2<TH2I> (hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. Y2 hits");
	
	
	// Time-sum spectra
	folder = basefolder + "/" + mShortName + "_all/det/tsum";
	
	hname = "Tsum_" + mXlabel + "_all";
	htitle = "Time sum " + mXlabel + " wire all hits";
	fAllTsum_x = MakeH1<TH1I> (hname, htitle, hist_tsum_bins, hist_tsum_min, hist_tsum_max, folder, "[TDC bins]");
	
	hname = "Tsum_" + mYlabel + "_all";
	htitle = "Time sum " + mYlabel + " wire all hits";
	fAllTsum_y = MakeH1<TH1I> (hname, htitle, hist_tsum_bins, hist_tsum_min, hist_tsum_max, folder, "[TDC bins]");
	
	// Time-sum spectra for sorted events
	folder = basefolder + "/" + mShortName + "_sort/det/tsum";
	
	hname = "Tsum_" + mXlabel + "_sort";
	htitle = "Time sum " + mXlabel + " wire sorted hits";
	fSortTsum_x = MakeH1<TH1I> (hname, htitle, hist_tsum_bins, hist_tsum_min, hist_tsum_max, folder, "[TDC bins]");
	
	hname = "Tsum_" + mYlabel + "_sort";
	htitle = "Time sum " + mYlabel + " wire sorted hits";
	fSortTsum_y = MakeH1<TH1I> (hname, htitle, hist_tsum_bins, hist_tsum_min, hist_tsum_max, folder, "[TDC bins]");
	
	hname = "TsumCo_" + mXlabel + "_sort";
	htitle = "Time sum vs. coordinate " + mXlabel + " wire sorted hits";
	hytitle = "Time sum " + mXlabel + " [TDC bins]";
	hxtitle = mXlabel + " [mm]";
	fSortX_Tsum_x = MakeH2<TH2I> (hname, htitle, hist_xy_bins/2, hist_xy_min, hist_xy_max, hist_tsum_bins/4, hist_tsum_min, hist_tsum_max, folder, hxtitle, hytitle);
	
	hname = "TsumCo_" + mYlabel + "_sort";
	htitle = "Time sum vs. coordinate " + mYlabel + " wire sorted hits";
	hytitle = "Time sum " + mYlabel + " [TDC bins]";
	hxtitle = mYlabel + " [mm]";
	fSortY_Tsum_y = MakeH2<TH2I> (hname, htitle, hist_xy_bins/2, hist_xy_min, hist_xy_max, hist_tsum_bins/4, hist_tsum_min, hist_tsum_max, folder, hxtitle, hytitle);
	
	// check histograms for sorting
	folder = basefolder + "/" + mShortName + "_sort/det/counts";
	
	hname = "SortedEvents_" + mShortName;
	htitle = "No. of sorted events " + mLongName + " detector";
	fSortedEvents = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	hname = "SortVsMcp_" + mShortName;
	htitle = "No. sorted events vs No. MCP hits " + mLongName;
	fSortvsMcpHits = MakeH2<TH2I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. sorted events");
	
	hname = "SortedYield_" + mShortName;
	htitle = "No. of sorted events / No. MCP events " + mLongName;
	binsname = basename + "hist_sortedyield_bins";
	minname = basename + "hist_sortedyield_min";
	maxname = basename + "hist_sortedyield_max";
	fSortedYield = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder);
	
	folder = basefolder + "/" + mShortName + "_sort/det/check";
	
	binsname = basename + "hist_sortcheck_bins";
	minname = basename + "hist_sortcheck_min";
	maxname = basename + "hist_sortcheck_max";
	
	hname = "SortCheck_" + mXlabel + "1";
	htitle = mXlabel + "1 signal minus MCP signal " + mLongName + " detector";
	fSortX1Mcp = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder, "X1-MCP [TDC bins]");
	
	hname = "SortCheck_" + mXlabel + "2";
	htitle = mXlabel + "2 signal minus MCP signal " + mLongName + " detector";
	fSortX2Mcp = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder, "X2-MCP [TDC bins]");
	
	hname = "SortCheck_" + mYlabel + "1";
	htitle = mYlabel + "1 signal minus MCP signal " + mLongName + " detector";
	fSortY1Mcp = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder, "Y1-MCP [TDC bins]");
	
	hname = "SortCheck_" + mYlabel + "2";
	htitle = mYlabel + "2 signal minus MCP signal " + mLongName + " detector";
	fSortY2Mcp = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder, "Y2-MCP [TDC bins]");
	
	if (mUseReconstruct)
	{
		// Reconstruction spectra
		folder = basefolder + "/" + mShortName + "_sort/det/reco";
		
		hname = "RecoMcpSortedEvents_" + mShortName;
		htitle = "No. of sorted events " + mLongName + " detector after MCP reconstruction";
		fRecoMcpSorted = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
		
		hname = "RecoMcp_" + mShortName;
		htitle = "Reconstructed MCP signals " + mLongName;
		fRecoMcp = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoMcpSortVsMcp_" + mShortName;
		htitle = "No. sorted events vs No. MCP hits " + mLongName + " after MCP reconstruction";
		fRecoMcpSortvsMcpHits = MakeH2<TH2I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. sorted events");
		
		hname = "RecoMcpSortedYield_" + mShortName;
		htitle = "No. of sorted events / No. MCP events " + mLongName + " after MCP reconstruction";
		binsname = basename + "hist_sortedyield_bins";
		minname = basename + "hist_sortedyield_min";
		maxname = basename + "hist_sortedyield_max";
		fRecoMcpSortedYield = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder);
		
		hname = "RecoDlineSortedEvents_" + mShortName;
		htitle = "No. of sorted events " + mLongName + " detector after Delayline reconstruction";
		fRecoDlineSorted = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
		
		hname = "RecoDline_" + mShortName;
		htitle = "Reconstructed Delayline signals " + mLongName;
		fRecoDline = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoDlineSortVsMcp_" + mShortName;
		htitle = "No. sorted events vs No. Delayline hits " + mLongName + " after Delayline reconstruction";
		fRecoDlineSortvsMcpHits = MakeH2<TH2I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. Delayline hits", "No. sorted events");
		
		hname = "RecoDlineSortedYield_" + mShortName;
		htitle = "No. of sorted events / No. Delayline events " + mLongName + " after Delayline reconstruction";
		binsname = basename + "hist_sortedyield_bins";
		minname = basename + "hist_sortedyield_min";
		maxname = basename + "hist_sortedyield_max";
		fRecoDlineSortedYield = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder);
	}
		
	// Rolling graphs
	folder = basefolder + "/" + mShortName + "/";
	fGraphX1Hits = MakeRollingGraph("CurrentX1Hits_" + mShortName, "No. of X1 Hits " + mLongName + " detector", rolling_graph_num, rolling_graph_average, folder);
	fGraphX2Hits = MakeRollingGraph("CurrentX2Hits_" + mShortName, "No. of X2 Hits " + mLongName + " detector", rolling_graph_num, rolling_graph_average, folder);
	fGraphY1Hits = MakeRollingGraph("CurrentY1Hits_" + mShortName, "No. of Y1 Hits " + mLongName + " detector", rolling_graph_num, rolling_graph_average, folder);
	fGraphY2Hits = MakeRollingGraph("CurrentY2Hits_" + mShortName, "No. of Y2 Hits " + mLongName + " detector", rolling_graph_num, rolling_graph_average, folder);
	fGraphSortedEvents = MakeRollingGraph("CurrentSortedEcents_" + mShortName, "Sorted events for " + mLongName + " detector", rolling_graph_num, rolling_graph_average, folder);
	
	fPictureRollingGraphs = MakePicture("CurrentCounts_" + mShortName, "Counts for " + mLongName + " detector", folder, 2, 3);
	AddGraphToPicture(fGraphMcpHits, fPictureRollingGraphs, 0, 0);
	AddGraphToPicture(fGraphX1Hits, fPictureRollingGraphs, 0, 1);
	AddGraphToPicture(fGraphX2Hits, fPictureRollingGraphs, 0, 2);
	AddGraphToPicture(fGraphY1Hits, fPictureRollingGraphs, 1, 0);
	AddGraphToPicture(fGraphY2Hits, fPictureRollingGraphs, 1, 1);
	AddGraphToPicture(fGraphSortedEvents, fPictureRollingGraphs, 1, 2);
	
	//-------------------//
	// Create conditions //
	//-------------------//

	// Time sum conditions
	folder = basefolder + "/" + mShortName + "/Tsum";
	cname = "c_Tsum_" + mXlabel;
	ctitle = "Time sum condition " + mXlabel + " wire";
	hname = "Tsum_" + mXlabel + "_all";
	fconTsum_x = MakeWinCon(cname, ctitle, hname, folder);

	cname = "c_Tsum_" + mYlabel;
	ctitle = "Time sum condition " + mYlabel + " wire";
	hname = "Tsum_" + mYlabel + "_all";
	fconTsum_y = MakeWinCon(cname, ctitle, hname, folder);
	
	// Condition for the delay line signal ranges
	folder = basefolder + "/" + mShortName + "/SortCheck";
	cname = "c_Sort_" + mXlabel + "1";
	ctitle = "Window for allowed difference between " + mXlabel + "1 signal and MCP";
	hname = "SortCheck_" + mXlabel + "1";
	fconSortX1 = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Sort_" + mXlabel + "2";
	ctitle = "Window for allowed difference between " + mXlabel + "2 signal and MCP";
	hname = "SortCheck_" + mXlabel + "2";
	fconSortX2 = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Sort_" + mYlabel + "1";
	ctitle = "Window for allowed difference between " + mYlabel + "1 signal and MCP";
	hname = "SortCheck_" + mYlabel + "1";
	fconSortY1 = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Sort_" + mYlabel + "2";
	ctitle = "Window for allowed difference between " + mYlabel + "2 signal and MCP";
	hname = "SortCheck_" + mYlabel + "2";
	fconSortY2 = MakeWinCon(cname, ctitle, hname, folder);
	
	//-----------------//
	// Create pictures //
	//-----------------//
	folder = basefolder;
	
	// picture with Time Sums
	pname = "TimeSums_" + mShortName;
	ptitle = "Time sum conditions for " + mLongName;
	fPicTimeSums = MakePicture(pname, ptitle, folder, 1, 2);
	AddCondToPicture(fconTsum_x, fPicTimeSums, 0, 0);
	AddCondToPicture(fconTsum_y, fPicTimeSums, 0, 1);
	
	
	//---------------------------------//
	// Initialise other member objects //
	//---------------------------------//

	if (mUseReconstruct)
		fReconstruct = new TRemiUnpackProcReconstruct (fConfigFile, particle);
	
	mDetectorHits = std::vector<TRemiDetectorHit> ();
	
}

/**
 * \brief Destructor
 */
TRemiUnpackProcDelayline::~TRemiUnpackProcDelayline()
{
	if (mUseReconstruct)
		delete fReconstruct;
}

/**
 * \brief calculate positions of current event
 *
 */
void TRemiUnpackProcDelayline::CalculatePosition()
{
	SortDetecor();
	
	if (mUseReconstruct)
		Reconstruct();
}

/**
 * \brief Sort detector and calculate positions
 */
void TRemiUnpackProcDelayline::SortDetecor()
{
	// Create references to input vectors
	std::vector<Float_t> &VectorMcp = fRawData->fiTDC[mTdcChanMCP];
	std::vector<Float_t> &VectorX1 = fRawData->fiTDC[mTdcChanX1];
	std::vector<Float_t> &VectorX2 = fRawData->fiTDC[mTdcChanX2];
	std::vector<Float_t> &VectorY1 = fRawData->fiTDC[mTdcChanY1];
	std::vector<Float_t> &VectorY2 = fRawData->fiTDC[mTdcChanY2];
	
	// Fill some histograms
	fX1Hits->Fill(VectorX1.size());
	fX2Hits->Fill(VectorX2.size());
	fY1Hits->Fill(VectorY1.size());
	fY2Hits->Fill(VectorY2.size());
	fX1McpHits->Fill(mNumTofHits, VectorX1.size());
	fX2McpHits->Fill(mNumTofHits, VectorX2.size());
	fY1McpHits->Fill(mNumTofHits, VectorY1.size());
	fY2McpHits->Fill(mNumTofHits, VectorY2.size());
	fGraphX1Hits->Fill(VectorX1.size());
	fGraphX2Hits->Fill(VectorX2.size());
	fGraphY1Hits->Fill(VectorY1.size());
	fGraphY2Hits->Fill(VectorY2.size());
	
	mDetectorHits.clear();
	std::vector<Float_t>::iterator x1It, x2It, y1It, y2It;
	Char_t RandomWalk; 
	
	std::vector<Float_t>::iterator McpIt = VectorMcp.begin();
	while (McpIt != VectorMcp.end())
	{
		RandomWalk = fRandom.Integer(4); // Variable is assigned a random value to deside on which side of the detector one should start searching a hit
		if ( ( (RandomWalk & 0x1) ? FindSingleLayerEvent(x1It, x2It, VectorX1, VectorX2, *McpIt, fconTsum_x, fconSortX1, fconSortX2, fAllTsum_x) : FindSingleLayerEvent(x2It, x1It, VectorX2, VectorX1, *McpIt, fconTsum_x, fconSortX2, fconSortX1, fAllTsum_x) ) && ( (RandomWalk & 0x2) ? FindSingleLayerEvent(y1It, y2It, VectorY1, VectorY2, *McpIt, fconTsum_y, fconSortY1, fconSortY2, fAllTsum_y) : FindSingleLayerEvent(y2It, y1It, VectorY2, VectorY1, *McpIt, fconTsum_y, fconSortY2, fconSortY1, fAllTsum_y) ) )
		{
			// We have found a matching time sum on each delay line wire. The iterators x1It, x2It, y1It amd y2It point to the corresponding times in the vectors
			AddPosition( *McpIt, *x1It, *x2It, *y1It, *y2It);
			
			// Erase used times from delay line and MCP hit vectors
			x1It = VectorX1.erase(x1It);
			x2It = VectorX2.erase(x2It);
			y1It = VectorY1.erase(y1It);
			y2It = VectorY2.erase(y2It);
			McpIt = VectorMcp.erase(McpIt);
		}
		else
			++McpIt;
	}
	
	// Fill spectra showing sorting efficiency
	fGraphSortedEvents->Fill(mDetectorHits.size());
	fSortedEvents->Fill(mDetectorHits.size());
	fSortvsMcpHits->Fill(mNumTofHits, mDetectorHits.size());
	if (mNumTofHits)
		fSortedYield->Fill(static_cast<Float_t>(mDetectorHits.size())/static_cast<Float_t>(mNumTofHits));
}

/**
 * \brief Add a new position object and fill some elementary spectra
 
 */
void TRemiUnpackProcDelayline::AddPosition(Float_t McpValue, Float_t X1Value, Float_t X2Value, Float_t Y1Value, Float_t Y2Value)
{
	Float_t det_px2mm = fParaPos->size_mm / fParaPos->size_raw;
	
	Float_t xvalue = det_px2mm * fParaPos->scaleX * ( X1Value - X2Value ) - fParaPos->xo;
	Float_t yvalue = det_px2mm * fParaPos->scaleY * ( Y1Value - Y2Value ) - fParaPos->yo;
	
	TRemiDetectorHit newhit = TRemiDetectorHit(CalculateSingleTof(McpValue), xvalue, yvalue);
	newhit.SetIndex(mDetectorHits.size());
	newhit.Rotate(fParaPos->phi);
	mDetectorHits.push_back(newhit);
	
	// Fill spectra
	Float_t xsum = X1Value + X2Value - 2 * McpValue;
	Float_t ysum = Y1Value + Y2Value - 2 * McpValue;
	fSortTsum_x->Fill(xsum);
	fSortTsum_y->Fill(ysum);
	fSortX_Tsum_x->Fill(xvalue, xsum);
	fSortY_Tsum_y->Fill(yvalue, ysum);
	fSortX1Mcp->Fill(X1Value - McpValue);
	fSortX2Mcp->Fill(X2Value - McpValue);
	fSortY1Mcp->Fill(Y1Value - McpValue);
	fSortY2Mcp->Fill(Y2Value - McpValue);
}

/**
 * \brief Reconstruction routines applied before any position finding
 */
void TRemiUnpackProcDelayline::Reconstruct ()
{
	Int_t NumReconstruct = 0;
	
	// Reconstruct missing MCP signals
	if (fParaReconst->useRecoMCP)
		NumReconstruct += ReconstructMcp();
	
	// Reconstruct missing delayline signals
	if (fParaReconst->useRecoDelayline)
		NumReconstruct += ReconstructDelayline();
	
	// We may have to sort the detector hits now
	if (NumReconstruct)
		std::sort (mDetectorHits.begin(), mDetectorHits.end());
}

/**
 * \brief Reconstruction of MCP signals 
 * \return Number of reconstructed MCP signals
 */
Int_t TRemiUnpackProcDelayline::ReconstructMcp()
{
	// Create references to input vectors
	std::vector<Float_t> &VectorX1 = fRawData->fiTDC[mTdcChanX1];
	std::vector<Float_t> &VectorX2 = fRawData->fiTDC[mTdcChanX2];
	std::vector<Float_t> &VectorY1 = fRawData->fiTDC[mTdcChanY1];
	std::vector<Float_t> &VectorY2 = fRawData->fiTDC[mTdcChanY2];
	
	Int_t NumReconstruct=0;
	Char_t RandomWalk = fRandom.Integer(4); // Variable is assigned a random value to deside on which side of the detector one should start searching a hit
	
	std::vector<Float_t>::iterator x1It=VectorX1.begin(), x2It=VectorX2.begin(), y1It, y2It;
	Float_t	McpValueX, McpValueY;
	Float_t tolerance = ( fconTsum_x->GetXUp() - fconTsum_x->GetXLow() + fconTsum_y->GetXUp() -fconTsum_y->GetXLow() ) / 4.;	// Tolerance is the maximum allowed deviation between the MCP values calculated with each wire
	Bool_t	FoundHit;
	while ( (RandomWalk & 0x1) ? fReconstruct->RecoMCP(x1It, x2It, McpValueX, VectorX1, VectorX2, fconTsum_x, fconSortX1, fconSortX2) : fReconstruct->RecoMCP(x2It, x1It, McpValueX, VectorX2, VectorX1, fconTsum_x, fconSortX2, fconSortX1)) 
	{
		y1It=VectorY1.begin(), y2It=VectorY2.begin(); // Reset iterators for Y
		FoundHit = kFALSE;
		while ( (RandomWalk & 0x2) ? fReconstruct->RecoMCP(y1It, y2It, McpValueY, VectorY1, VectorY2, fconTsum_y, fconSortY1, fconSortY2) : fReconstruct->RecoMCP(y2It, y1It, McpValueY, VectorY2, VectorY1, fconTsum_y, fconSortY2, fconSortY1))  
		{
			if (TMath::Abs(McpValueX - McpValueY) < tolerance) 
			{ 
				// now we have found a MCP value satisfying both layers
				AddPosition( (McpValueX + McpValueY)/2., *x1It, *x2It, *y1It, *y2It );
				
				// Erase used times from delay line and MCP hit vectors
				x1It = VectorX1.erase(x1It);
				x2It = VectorX2.erase(x2It);
				y1It = VectorY1.erase(y1It);
				y2It = VectorY2.erase(y2It);
				if (RandomWalk & 0x1)
					x2It = VectorX2.begin(); 
				else 
					x1It = VectorX1.begin(); // reset second x iterator. Note: always the iterator passed second to fReconstruct->RecoMCP has to be reseted
				FoundHit = kTRUE;
				NumReconstruct++;
				break; // break y loop, because first a new x hit has to be generated
			}
			else
				(RandomWalk & 0x2) ? ++y2It : ++y1It; // If this second y value does not fit, go to next. Note: always the iterator passed second to fReconstruct->RecoMCP has to be incremented
		}
		if (!FoundHit)
			(RandomWalk & 0x1) ? ++x2It : ++x1It; // If no suitable y hit was found for this x combination, go on to the next second hit. Note: always the iterator passed second to fReconstruct->RecoMCP has to be incremented
	}
	
	// Fill some histograms
	fRecoMcp->Fill(NumReconstruct);
	fRecoMcpSorted->Fill(mDetectorHits.size());
	fRecoMcpSortvsMcpHits->Fill(mNumTofHits, mDetectorHits.size());
	if (mNumTofHits)
		fRecoMcpSortedYield->Fill(static_cast<Float_t>(mDetectorHits.size())/static_cast<Float_t>(mNumTofHits));
	
	return NumReconstruct;
}

/**
* \brief Reconstruction of Delayline signals 
* \return Number of reconstructed Delayline signals
*/
Int_t TRemiUnpackProcDelayline::ReconstructDelayline()
{
	// Create references to input vectors
	std::vector<Float_t> &VectorMcp = fRawData->fiTDC[mTdcChanMCP];
	std::vector<Float_t> &VectorX1 = fRawData->fiTDC[mTdcChanX1];
	std::vector<Float_t> &VectorX2 = fRawData->fiTDC[mTdcChanX2];
	std::vector<Float_t> &VectorY1 = fRawData->fiTDC[mTdcChanY1];
	std::vector<Float_t> &VectorY2 = fRawData->fiTDC[mTdcChanY2];
	
	Int_t NumReconstruct = 0;
	Bool_t HaveReconstructed;
	std::vector<Float_t>::iterator McpIt = VectorMcp.begin();
	while (McpIt != VectorMcp.end())
	{
		HaveReconstructed = kFALSE;
		switch(fRandom.Integer(4)) // Random choice where to start reconstruction. Note that for each choice we will always try to reconstruct from either x or y signal, but the order of trials will change
		{
			case 0:
				HaveReconstructed = ReconstructDelaylineSingleHit(VectorY1, VectorY2, fconTsum_y, fconSortY1, fconSortY2, McpIt, VectorX2, fconTsum_x, fconSortX2, fconSortX1, kX1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructDelaylineSingleHit(VectorX1, VectorX2, fconTsum_x, fconSortX1, fconSortX2, McpIt, VectorY2, fconTsum_y, fconSortY2, fconSortY1, kY1);
				break;
			case 1:
				HaveReconstructed = ReconstructDelaylineSingleHit(VectorY1, VectorY2, fconTsum_y, fconSortY1, fconSortY2, McpIt, VectorX1, fconTsum_x, fconSortX1, fconSortX2, kX2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructDelaylineSingleHit(VectorX1, VectorX2, fconTsum_x, fconSortX1, fconSortX2, McpIt, VectorY1, fconTsum_y, fconSortY1, fconSortY2, kY2);
				break;
			case 2:
				HaveReconstructed = ReconstructDelaylineSingleHit(VectorX1, VectorX2, fconTsum_x, fconSortX1, fconSortX2, McpIt, VectorY2, fconTsum_y, fconSortY2, fconSortY1, kY1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructDelaylineSingleHit(VectorY1, VectorY2, fconTsum_y, fconSortY1, fconSortY2, McpIt, VectorX2, fconTsum_x, fconSortX2, fconSortX1, kX1);
				break;
			case 3:
				HaveReconstructed = ReconstructDelaylineSingleHit(VectorX1, VectorX2, fconTsum_x, fconSortX1, fconSortX2, McpIt, VectorY1, fconTsum_y, fconSortY1, fconSortY2, kY2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructDelaylineSingleHit(VectorY1, VectorY2, fconTsum_y, fconSortY1, fconSortY2, McpIt, VectorX1, fconTsum_x, fconSortX1, fconSortX2, kX2);
				break;
			default:
				break;
		}		
		if (!HaveReconstructed)
			++McpIt; // go to next MCP hit
		else
			NumReconstruct++; // No need to increment MCP iterator in this case because this is already done when the hit was found
	}
	
	// Fill some histograms
	fRecoDline->Fill(NumReconstruct);
	fRecoDlineSorted->Fill(mDetectorHits.size());
	fRecoDlineSortvsMcpHits->Fill(mNumTofHits, mDetectorHits.size());
	if (mNumTofHits)
		fRecoDlineSortedYield->Fill( static_cast<Float_t>(mDetectorHits.size())/static_cast<Float_t>(mNumTofHits) );
	
	return NumReconstruct;
}


/**
 * \brief Reconstructs a delay line hit on one wire
 * \param KnownList1 First signal vector of known layer
 * \param KnownList2 Signal signal vector of known layer
 * \param KnownConTimeSum Time sum condition for known layer
 * \param KnownConSort1 Valid range window condition for first signal of known layer
 * \param KnownConSort2 Valid range window condition for second signal of known layer
 * \param McpValue Mcp value
 * \param UnknownList1 One signal layer of unknown layer
 * \param UnknownConTimeSum Time sum condition of unknown layer
 * \param UnknownParaSort Sorting parameter of unknown layer
 * \param UnknownConSort1 Valid range window condition for first signal of unknown layer
 * \param UnknownConSort2 Valid range window condition for second signal of unknown layer
 * \param WhichReconstruct Indicator which signal is being reconstructed (X1, X2, Y1 or Y2)
 */
Bool_t TRemiUnpackProcDelayline::ReconstructDelaylineSingleHit(std::vector< Float_t > &KnownList1, std::vector< Float_t > &KnownList2, TGo4WinCond* KnownConTimeSum, TGo4WinCond* KnownConSort1, TGo4WinCond* KnownConSort2, std::vector<Float_t>::iterator &McpIt, std::vector< Float_t > &UnknownList1, TGo4WinCond* UnknownConTimeSum, TGo4WinCond* UnknownConSort1, TGo4WinCond* UnknownConSort2, TRemiUnpackProcDelayline::eDelaylineReconstruct WhichReconstruct)
{
	std::vector<Float_t>::iterator Known1It, Known2It, Unknown1It;
	Float_t NewValue=0;
	Char_t RandomWalk = fRandom.Integer(2); // Random variable to decide in which direction to look for a valid hit on the known wire
	if ( ( (RandomWalk) ? FindSingleLayerEvent(Known1It, Known2It, KnownList1, KnownList2, *McpIt, KnownConTimeSum, KnownConSort1, KnownConSort2) : FindSingleLayerEvent(Known2It, Known1It, KnownList2, KnownList1, *McpIt, KnownConTimeSum, KnownConSort2, KnownConSort1) ) && (fReconstruct->RecoDelayline(Unknown1It, NewValue, UnknownList1, *McpIt, UnknownConTimeSum, UnknownConSort1, UnknownConSort2)) )
	{
		switch (WhichReconstruct)
		{
			case kX1:
				AddPosition(*McpIt, NewValue, *Unknown1It, *Known1It, *Known2It);
				break;
			case kX2:
				AddPosition(*McpIt, *Unknown1It, NewValue, *Known1It, *Known2It);
				break;
			case kY1:
				AddPosition(*McpIt, *Known1It, *Known2It, NewValue, *Unknown1It);
				break;
			case kY2:
				AddPosition(*McpIt, *Known1It, *Known2It, *Unknown1It, NewValue);
				break;
		}
		
		McpIt = fRawData->fiTDC[mTdcChanMCP].erase(McpIt);
		Known1It = KnownList1.erase(Known1It);
		Known2It = KnownList2.erase(Known2It);
		Unknown1It = UnknownList1.erase(Unknown1It);
		return kTRUE;
	}
	else 
		return kFALSE;
}



ClassImp(TRemiUnpackProcDelayline);
