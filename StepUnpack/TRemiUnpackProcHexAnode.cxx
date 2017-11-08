//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class calculates the position of a hit on a hexanode
//----------------------------------------------------------------

#include "TRemiUnpackProcHexAnode.h"

#include "Parameters/TRemiHexAnodeParameter.h"
#include "Parameters/TRemiReconstructParameter.h"
#include "TRemiUnpackProcReconstruct.h"
#include "../EventElement/TRemiExtractEvent.h"
#include "../Core/Point/TRemiXY.h"

#include "../Core/Extern/ConfigFile.h"

#include "TGo4WinCond.h"
#include "TGo4Picture.h"

#include "TH1.h"
#include "TH2.h"

/**
 * \brief Default constructor. Not to be used!
 */
TRemiUnpackProcHexAnode::TRemiUnpackProcHexAnode()
{
}

/**
 * \brief Constructor to directly build an object of this class using configuration file
 */
TRemiUnpackProcHexAnode::TRemiUnpackProcHexAnode(ConfigFile* UnpackConf, TString particle) : 
	TRemiUnpackProcPos(UnpackConf, particle)
{
	TString fieldname, basename="unp_", hname, htitle, folder, basefolder = "Unpack", cname, ctitle, paraname, hxtitle, hytitle, pname, ptitle, binsname, minname, maxname;
	
	//------------------------------------------//
	// Read information from configuration file //
	//------------------------------------------//
	
	basename = basename + particle + "_";
	
	fieldname = basename + "ulabel";
	mUlabel = fConfigFile->read<TString>(fieldname.Data(), "u");
	fieldname = basename + "vlabel";
	mVlabel = fConfigFile->read<TString>(fieldname.Data(), "v");
	fieldname = basename + "wlabel";
	mWlabel = fConfigFile->read<TString>(fieldname.Data(), "w");
	
	fieldname = basename + "tdc_u1";
	mTdcChanU1 = fConfigFile->read (fieldname.Data(), 1);
	fieldname = basename + "tdc_u2";
	mTdcChanU2 = fConfigFile->read (fieldname.Data(), 2);
	fieldname = basename + "tdc_v1";
	mTdcChanV1 = fConfigFile->read (fieldname.Data(), 3);
	fieldname = basename + "tdc_v2";
	mTdcChanV2 = fConfigFile->read (fieldname.Data(), 4);
	fieldname = basename + "tdc_w1";
	mTdcChanW1 = fConfigFile->read (fieldname.Data(), 5);
	fieldname = basename + "tdc_w2";
	mTdcChanW2 = fConfigFile->read (fieldname.Data(), 6);
	
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
	
	fieldname = basename + "use_scalef";
	mUseScalef = fConfigFile->read (fieldname.Data(), false);
	
	if (mUseScalef)
	{
		fieldname = basename + "scalef_min";
		mScalefMin = fConfigFile->read (fieldname.Data(), .9);
		fieldname = basename + "scalef_max";
		mScalefMax = fConfigFile->read (fieldname.Data(), 1.1);
		fieldname = basename + "scalef_stepsize";
		mScalefStepsize = fConfigFile->read (fieldname.Data(), .1);
		fieldname = basename + "scalef_minerror";
		mScalefMinerror = fConfigFile->read (fieldname.Data(), 1.);
	}

	
	//-----------------//
	// Make Parameters //
	//-----------------//
	folder = basefolder + "/" + mShortName;
	paraname = "Par "; paraname = paraname + mShortName + " hexanode";
	fParaPos = MakeParam<TRemiHexAnodeParameter>(paraname, folder);
	
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
	
	hname = "U1Hits_" + mShortName;
	htitle = "No. of U1 Hits " + mLongName + " detector";
	fU1Hits = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	hname = "U2Hits_" + mShortName;
	htitle = "No. of U2 Hits " + mLongName + " detector";
	fU2Hits = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	hname = "V1Hits_" + mShortName;
	htitle = "No. of V1 Hits " + mLongName + " detector";
	fV1Hits = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	hname = "V2Hits_" + mShortName;
	htitle = "No. of V2 Hits " + mLongName + " detector";
	fV2Hits = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	hname = "W1Hits_" + mShortName;
	htitle = "No. of W1 Hits " + mLongName + " detector";
	fW1Hits = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	hname = "W2Hits_" + mShortName;
	htitle = "No. of W2 Hits " + mLongName + " detector";
	fW2Hits = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	hname = "U1HitsVsMcpHits_" + mShortName;
	htitle = "No. of U1 hits vs. MCP hits " + mLongName + " detector";
	fU1McpHits = MakeH2<TH2I> (hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. U1 hits");
	
	hname = "U2HitsVsMcpHits_" + mShortName;
	htitle = "No. of U2 hits vs. MCP hits " + mLongName + " detector";
	fU2McpHits = MakeH2<TH2I> (hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. U2 hits");
	
	hname = "V1HitsVsMcpHits_" + mShortName;
	htitle = "No. of V1 hits vs. MCP hits " + mLongName + " detector";
	fV1McpHits = MakeH2<TH2I> (hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. V1 hits");
	
	hname = "V2HitsVsMcpHits_" + mShortName;
	htitle = "No. of V2 hits vs. MCP hits " + mLongName + " detector";
	fV2McpHits = MakeH2<TH2I> (hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. V2 hits");
	
	hname = "W1HitsWsMcpHits_" + mShortName;
	htitle = "No. of W1 hits vs. MCP hits " + mLongName + " detector";
	fW1McpHits = MakeH2<TH2I> (hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. W1 hits");
	
	hname = "W2HitsWsMcpHits_" + mShortName;
	htitle = "No. of W2 hits vs. MCP hits " + mLongName + " detector";
	fW2McpHits = MakeH2<TH2I> (hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. W2 hits");
	

	// Time-sum spectra
	folder = basefolder + "/" + mShortName + "_all/det/tsum";
	
	hname = "Tsum_" + mUlabel + "_all";
	htitle = "Time sum " + mUlabel + " wire all hits";
	fAllTsum_u = MakeH1<TH1I> (hname, htitle, hist_tsum_bins, hist_tsum_min, hist_tsum_max, folder, "[TDC bins]");
	
	hname = "Tsum_" + mVlabel + "_all";
	htitle = "Time sum " + mVlabel + " wire all hits";
	fAllTsum_v = MakeH1<TH1I> (hname, htitle, hist_tsum_bins, hist_tsum_min, hist_tsum_max, folder, "[TDC bins]");
	
	hname = "Tsum_" + mWlabel + "_all";
	htitle = "Time sum " + mWlabel + " wire all hits";
	fAllTsum_w = MakeH1<TH1I> (hname, htitle, hist_tsum_bins, hist_tsum_min, hist_tsum_max, folder, "[TDC bins]");
	
	// Time-sum spectra for sorted events
	folder = basefolder + "/" + mShortName + "_sort/det/tsum";
	
	hname = "Tsum_" + mUlabel + "_sort";
	htitle = "Time sum " + mUlabel + " wire sorted hits";
	fSortTsum_u = MakeH1<TH1I> (hname, htitle, hist_tsum_bins, hist_tsum_min, hist_tsum_max, folder, "[TDC bins]");
	
	hname = "Tsum_" + mVlabel + "_sort";
	htitle = "Time sum " + mVlabel + " wire sorted hits";
	fSortTsum_v = MakeH1<TH1I> (hname, htitle, hist_tsum_bins, hist_tsum_min, hist_tsum_max, folder, "[TDC bins]");
	
	hname = "Tsum_" + mWlabel + "_sort";
	htitle = "Time sum " + mWlabel + " wire sorted hits";
	fSortTsum_w = MakeH1<TH1I> (hname, htitle, hist_tsum_bins, hist_tsum_min, hist_tsum_max, folder, "[TDC bins]");
	
	hname = "TsumCo_" + mUlabel + "_sort";
	htitle = "Time sum vs. coordinate " + mUlabel + " wire sorted hits";
	hytitle = "Time sum " + mUlabel + " [TDC bins]";
	hxtitle = mUlabel + " [mm]";
	fSortU_Tsum_u = MakeH2<TH2I> (hname, htitle, hist_xy_bins/2, hist_xy_min, hist_xy_max, hist_tsum_bins/4, hist_tsum_min, hist_tsum_max, folder, hxtitle, hytitle);
	
	hname = "TsumCo_" + mVlabel + "_sort";
	htitle = "Time sum vs. coordinate " + mVlabel + " wire sorted hits";
	hytitle = "Time sum " + mVlabel + " [TDC bins]";
	hxtitle = mVlabel + " [mm]";
	fSortV_Tsum_v = MakeH2<TH2I> (hname, htitle, hist_xy_bins/2, hist_xy_min, hist_xy_max, hist_tsum_bins/4, hist_tsum_min, hist_tsum_max, folder, hxtitle, hytitle);
	
	hname = "TsumCo_" + mWlabel + "_sort";
	htitle = "Time sum ws. coordinate " + mWlabel + " wire sorted hits";
	hytitle = "Time sum " + mWlabel + " [TDC bins]";
	hxtitle = mWlabel + " [mm]";
	fSortW_Tsum_w = MakeH2<TH2I> (hname, htitle, hist_xy_bins/2, hist_xy_min, hist_xy_max, hist_tsum_bins/4, hist_tsum_min, hist_tsum_max, folder, hxtitle, hytitle);
	
	// check histograms for sorting
	folder = basefolder + "/" + mShortName + "_sort/det/counts";
	
	hname = "SortedEvents_" + mShortName;
	htitle = "No. of sorted events " + mLongName + " detector";
	fSortedEvents = MakeH1<TH1I>(hname, htitle,hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
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
	
	hname = "SortCheck_" + mUlabel + "1";
	htitle = mUlabel + "1 signal minus MCP signal " + mLongName + " detector";
	fSortU1Mcp = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder, "U1-MCP [TDC bins]");
	
	hname = "SortCheck_" + mUlabel + "2";
	htitle = mUlabel + "2 signal minus MCP signal " + mLongName + " detector";
	fSortU2Mcp = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder, "U2-MCP [TDC bins]");
	
	hname = "SortCheck_" + mVlabel + "1";
	htitle = mVlabel + "1 signal minus MCP signal " + mLongName + " detector";
	fSortV1Mcp = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder, "V1-MCP [TDC bins]");
	
	hname = "SortCheck_" + mVlabel + "2";
	htitle = mVlabel + "2 signal minus MCP signal " + mLongName + " detector";
	fSortV2Mcp = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder, "V2-MCP [TDC bins]");
	
	hname = "SortCheck_" + mWlabel + "1";
	htitle = mWlabel + "1 signal minus MCP signal " + mLongName + " detector";
	fSortW1Mcp = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder, "W1-MCP [TDC bins]");
	
	hname = "SortCheck_" + mWlabel + "2";
	htitle = mWlabel + "2 signal minus MCP signal " + mLongName + " detector";
	fSortW2Mcp = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder, "W2-MCP [TDC bins]");
	
	
	// Layer spectra
	folder = basefolder + "/" + mShortName + "_sort/det/layers";
	hname = mUlabel + "_sort";
	htitle = "Position on " + mUlabel + " wire sorted hits";
	fSortU = MakeH1<TH1I> (hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "[mm]");
	
	hname = mVlabel + "_sort";
	htitle = "Position on " + mVlabel + " wire sorted hits";
	fSortV = MakeH1<TH1I> (hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "[mm]");
	
	hname = mWlabel + "_sort";
	htitle = "Position on " + mWlabel + " wire sorted hits";
	fSortW = MakeH1<TH1I> (hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "[mm]");
	
	hname = "XY_" + mUlabel + mVlabel + "_sort";
	htitle = "XY position from wire " + mUlabel + " and " + mVlabel;
	fSortXY_uv = MakeH2<TH2I> (hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "X [mm]", "Y [mm]");
	
	hname = "XY_" + mUlabel + mWlabel + "_sort";
	htitle = "XY position from wire " + mUlabel + " and " + mWlabel;
	fSortXY_uw = MakeH2<TH2I> (hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "X [mm]", "Y [mm]");
	
	hname = "XY_" + mVlabel + mWlabel + "_sort";
	htitle = "XY position from wire " + mVlabel + " and " + mWlabel;
	fSortXY_vw = MakeH2<TH2I> (hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "X [mm]", "Y [mm]");
	
	
	// Histograms for layer overlay check
	folder = basefolder + "/" + mShortName + "_sort/det/overlay";
	
	hname = "Y" + mUlabel + mVlabel + "_Y" + mUlabel + mWlabel;
	hxtitle = "y(" + mUlabel + mVlabel + ") - y(" + mUlabel + mWlabel + ") [mm]";
	hytitle = "y(" + mUlabel + mWlabel + ") [mm]";
	htitle = hytitle + " vs. " + hxtitle;
	fYuv_yuw = MakeH2<TH2I>(hname, htitle, hist_xy_bins/2, hist_xy_min/10, hist_xy_max/10, hist_xy_bins, hist_xy_min, hist_xy_max, folder, hxtitle, hytitle);
	
	hname = "Y" + mVlabel + mWlabel + "_Y" + mUlabel + mVlabel;
	hxtitle = "y(" + mVlabel + mWlabel + ") - y(" + mUlabel + mVlabel + ") [mm]";
	hytitle = "y(" + mUlabel + mVlabel + ") [mm]";
	htitle = hytitle + " vs. " + hxtitle;
	fYvw_yuv = MakeH2<TH2I>(hname, htitle, hist_xy_bins/2, hist_xy_min/10, hist_xy_max/10, hist_xy_bins, hist_xy_min, hist_xy_max, folder, hxtitle, hytitle);
	
	hname = "Y" + mUlabel + mWlabel + "_Y" + mVlabel + mWlabel;
	hxtitle = "y(" + mUlabel + mWlabel + ") - y(" + mVlabel + mWlabel + ") [mm]";
	hytitle = "y(" + mVlabel + mWlabel + ") [mm]";
	htitle = hytitle + " vs. " + hxtitle;
	fYuw_yvw = MakeH2<TH2I>(hname, htitle, hist_xy_bins/2, hist_xy_min/10, hist_xy_max/10, hist_xy_bins, hist_xy_min, hist_xy_max, folder, hxtitle, hytitle);
	
	hname = "X" + mUlabel + mVlabel + "_X" + mUlabel + mWlabel;
	hxtitle = "x(" + mUlabel + mVlabel + ") - x(" + mUlabel + mWlabel + ") [mm]";
	hytitle = "x(" + mUlabel + mWlabel + ") [mm]";
	htitle = hytitle + " vs. " + hxtitle;
	fXuv_xuw = MakeH2<TH2I>(hname, htitle, hist_xy_bins/2, hist_xy_min/10, hist_xy_max/10, hist_xy_bins, hist_xy_min, hist_xy_max, folder, hxtitle, hytitle);
	
	hname = "X" + mVlabel + mWlabel + "_X" + mUlabel + mVlabel;
	hxtitle = "x(" + mVlabel + mWlabel + ") - x(" + mUlabel + mVlabel + ") [mm]";
	hytitle = "x(" + mUlabel + mVlabel + ") [mm]";
	htitle = hytitle + " vs. " + hxtitle;
	fXvw_xuv = MakeH2<TH2I>(hname, htitle, hist_xy_bins/2, hist_xy_min/10, hist_xy_max/10, hist_xy_bins, hist_xy_min, hist_xy_max, folder, hxtitle, hytitle);
	
	hname = "X" + mUlabel + mWlabel + "_X" + mVlabel + mWlabel;
	hxtitle = "x(" + mUlabel + mWlabel + ") - x(" + mVlabel + mWlabel + ") [mm]";
	hytitle = "x(" + mVlabel + mWlabel + ") [mm]";
	htitle = hytitle + " vs. " + hxtitle;
	fXuw_xvw = MakeH2<TH2I>(hname, htitle, hist_xy_bins/2, hist_xy_min/10, hist_xy_max/10, hist_xy_bins, hist_xy_min, hist_xy_max, folder, hxtitle, hytitle);
	
	if (mUseScalef)
	{
		hname = "Scalef_" + mShortName;
		htitle = "Find best hex anode scalefactors for " + mLongName + ", " + mUlabel + " factor set to 1";
		hxtitle = mVlabel + " scale factor";
		hytitle = mWlabel + " scale factor";
		Int_t bins = static_cast<Int_t> ( (mScalefMax - mScalefMin) / mScalefStepsize );
		fScalefactors = MakeH2<TH2I> (hname, htitle, bins, mScalefMin, mScalefMax, bins, mScalefMin, mScalefMax, folder, hxtitle, hytitle);
	}
	
	if (mUseReconstruct)
	{
		// Reconstruction spectra
		folder = basefolder + "/" + mShortName + "_sort/det/reco/mcp";
		
		hname = "RecoMcp_" + mShortName;
		htitle = "Reconstructed total MCP signals " + mLongName;
		fRecoMcp = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoMcp_" + mUlabel + mVlabel + "_" + mShortName;
		htitle =  mLongName + " MCP signals reconstructed from " + mUlabel + " and " + mVlabel + " layers";
		fRecoMcpUV = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoMcp_" + mUlabel + mWlabel + "_" + mShortName;
		htitle =  mLongName + " MCP signals reconstructed from " + mUlabel + " and " + mWlabel + " layers";
		fRecoMcpUW = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoMcp_" + mVlabel + mWlabel + "_" + mShortName;
		htitle =  mLongName + " MCP signals reconstructed from " + mVlabel + " and " + mWlabel + " layers";
		fRecoMcpVW = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoMcp_" + mUlabel + mVlabel + mWlabel + "_" + mShortName;
		htitle =  mLongName + " MCP signals reconstructed from all three layers";
		fRecoMcpUVW = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoMcpSortedEvents_" + mShortName;
		htitle = "No. of sorted events " + mLongName + " detector after MCP reconstruction";
		fRecoMcpSorted = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
		
		hname = "RecoMcpSortVsMcp_" + mShortName;
		htitle = "No. sorted events vs No. MCP hits " + mLongName + " after MCP reconstruction";
		fRecoMcpSortvsMcpHits = MakeH2<TH2I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. MCP hits", "No. sorted events");
		
		hname = "RecoMcpSortedYield_" + mShortName;
		htitle = "No. of sorted events / No. MCP events " + mLongName + " after MCP reconstruction";
		binsname = basename + "hist_sortedyield_bins";
		minname = basename + "hist_sortedyield_min";
		maxname = basename + "hist_sortedyield_max";
		fRecoMcpSortedYield = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder);
		
		folder = basefolder + "/" + mShortName + "_sort/det/reco/Dline";

		hname = "RecoDline_" + mShortName;
		htitle = "Reconstructed Delayline signals " + mLongName;
		fRecoDline = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoDline_" + mUlabel + "_Single_" + mShortName;
		htitle = mLongName +" Delayline signals reconstructed on a single layer with known " + mUlabel + " layer";
		fRecoDlineSingleKnownU = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoDline_" + mVlabel + "_Single_" + mShortName;
		htitle = mLongName +" Delayline signals reconstructed on a single layer with known " + mVlabel + " layer";
		fRecoDlineSingleKnownV = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoDline_" + mWlabel + "_Single_" + mShortName;
		htitle = mLongName +" Delayline signals reconstructed on a single layer with known " + mWlabel + " layer";
		fRecoDlineSingleKnownW = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoDline_" + mUlabel + "_Double_" + mShortName;
		htitle = mLongName +" Delayline signals reconstructed on a double layer with known " + mUlabel + " layer";
		fRecoDlineDoubleKnownU = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoDline_" + mVlabel + "_Double_" + mShortName;
		htitle = mLongName +" Delayline signals reconstructed on a double layer with known " + mVlabel + " layer";
		fRecoDlineDoubleKnownV = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoDline_" + mWlabel + "_Double_" + mShortName;
		htitle = mLongName +" Delayline signals reconstructed on a double layer with known " + mWlabel + " layer";
		fRecoDlineDoubleKnownW = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder, "No. of reconstructed Hits");
		
		hname = "RecoDlineSortedEvents_" + mShortName;
		htitle = "No. of sorted events " + mLongName + " detector after Delayline reconstruction";
		fRecoDlineSorted = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
		
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
	
	
	//-------------------//
	// Create conditions //
	//-------------------//

	// Time sum conditions
	folder = basefolder + "/" + mShortName + "/Tsum";
	cname = "c_Tsum_" + mUlabel;
	ctitle = "Time sum condition " + mUlabel + " wire";
	hname = "Tsum_" + mUlabel + "_all";
	fconTsum_u = MakeWinCon(cname, ctitle, hist_tsum_min, hist_tsum_max, hname, folder);
	
	cname = "c_Tsum_" + mVlabel;
	ctitle = "Time sum condition " + mVlabel + " wire";
	hname = "Tsum_" + mVlabel + "_all";
	fconTsum_v = MakeWinCon(cname, ctitle, hist_tsum_min, hist_tsum_max, hname, folder);
	
	cname = "c_Tsum_" + mWlabel;
	ctitle = "Time sum condition " + mWlabel + " wire";
	hname = "Tsum_" + mWlabel + "_all";
	fconTsum_w = MakeWinCon(cname, ctitle, hist_tsum_min, hist_tsum_max, hname, folder);

	// Condition for the delay line signal ranges
	folder = basefolder + "/" + mShortName + "/SortCheck";
	cname = "c_Sort_" + mUlabel + "1";
	ctitle = "Window for allowed difference between " + mUlabel + "1 signal and MCP";
	hname = "SortCheck_" + mUlabel + "1";
	fconSortU1 = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Sort_" + mUlabel + "2";
	ctitle = "Window for allowed difference between " + mUlabel + "2 signal and MCP";
	hname = "SortCheck_" + mUlabel + "2";
	fconSortU2 = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Sort_" + mVlabel + "1";
	ctitle = "Window for allowed difference between " + mVlabel + "1 signal and MCP";
	hname = "SortCheck_" + mVlabel + "1";
	fconSortV1 = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Sort_" + mVlabel + "2";
	ctitle = "Window for allowed difference between " + mVlabel + "2 signal and MCP";
	hname = "SortCheck_" + mVlabel + "2";
	fconSortV2 = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Sort_" + mWlabel + "1";
	ctitle = "Window for allowed difference between " + mWlabel + "1 signal and MCP";
	hname = "SortCheck_" + mWlabel + "1";
	fconSortW1 = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Sort_" + mWlabel + "2";
	ctitle = "Window for allowed difference between " + mWlabel + "2 signal and MCP";
	hname = "SortCheck_" + mWlabel + "2";
	fconSortW2 = MakeWinCon(cname, ctitle, hname, folder);
	
	//-----------------//
	// Create pictures //
	//-----------------//
	folder = basefolder;
	
	// picture for layer overlay check
	pname = "Overlap_" + mShortName;
	ptitle = "Check overlap of hex-anode layers for " + mLongName;
	fPicOverlap = MakePicture(pname, ptitle, folder, 2, 3);
	AddHistToPicture(fYuv_yuw, fPicOverlap, 0, 0);
	AddHistToPicture(fYuw_yvw, fPicOverlap, 0, 1);
	AddHistToPicture(fYvw_yuv, fPicOverlap, 0, 2);
	AddHistToPicture(fXuv_xuw, fPicOverlap, 1, 0);
	AddHistToPicture(fXuw_xvw, fPicOverlap, 1, 1);
	AddHistToPicture(fXvw_xuv, fPicOverlap, 1, 2);
	
	// picture with Time Sums
	pname = "TimeSums_" + mShortName;
	ptitle = "Time sum conditions for " + mLongName;
	fPicTimeSums = MakePicture(pname, ptitle, folder, 1, 3);
	AddCondToPicture(fconTsum_u, fPicTimeSums, 0, 0);
	AddCondToPicture(fconTsum_v, fPicTimeSums, 0, 1);
	AddCondToPicture(fconTsum_w, fPicTimeSums, 0, 2);
	
	
	//---------------------------------//
	// Initialise other member objects //
	//---------------------------------//
	if (mUseReconstruct)
		fReconstruct = new TRemiUnpackProcReconstruct (fConfigFile, particle);
	
}

/**
 * \brief Destructor
 */
TRemiUnpackProcHexAnode::~TRemiUnpackProcHexAnode()
{
	if (mUseReconstruct)
		delete fReconstruct;
}

/**
 * \brief calculate positions of current event
 * 
 */
void TRemiUnpackProcHexAnode::CalculatePosition()
{
	SortDetecor();
	
	if (mUseReconstruct)
		Reconstruct();
}


/**
* \brief Sort detector and calculate positions
*/
void TRemiUnpackProcHexAnode::SortDetecor()
{
	// Create references to input vectors
	std::vector<Float_t> &VectorMcp = fRawData->fiTDC[mTdcChanMCP];
	std::vector<Float_t> &VectorU1 = fRawData->fiTDC[mTdcChanU1];
	std::vector<Float_t> &VectorU2 = fRawData->fiTDC[mTdcChanU2];
	std::vector<Float_t> &VectorV1 = fRawData->fiTDC[mTdcChanV1];
	std::vector<Float_t> &VectorV2 = fRawData->fiTDC[mTdcChanV2];
	std::vector<Float_t> &VectorW1 = fRawData->fiTDC[mTdcChanW1];
	std::vector<Float_t> &VectorW2 = fRawData->fiTDC[mTdcChanW2];
	
	// Fill some histograms
	fU1Hits->Fill(VectorU1.size());
	fU2Hits->Fill(VectorU2.size());
	fV1Hits->Fill(VectorV1.size());
	fV2Hits->Fill(VectorV2.size());
	fW1Hits->Fill(VectorW1.size());
	fW2Hits->Fill(VectorW2.size());
	fU1McpHits->Fill(mNumTofHits, VectorU1.size());
	fU2McpHits->Fill(mNumTofHits, VectorU2.size());
	fV1McpHits->Fill(mNumTofHits, VectorV1.size());
	fV2McpHits->Fill(mNumTofHits, VectorV2.size());
	fW1McpHits->Fill(mNumTofHits, VectorW1.size());
	fW2McpHits->Fill(mNumTofHits, VectorW2.size());
	
	mDetectorHits.clear();
	std::vector<Float_t>::iterator u1It, u2It, v1It, v2It, w1It, w2It;
	Char_t RandomWalk, LayerEventMask; 
	
	std::vector<Float_t>::iterator McpIt = VectorMcp.begin();
	while (McpIt != VectorMcp.end())
	{
		RandomWalk = fRandom.Integer(8); // Variable is assigned a random value to deside on which side of the detector one should start searching a hit
		// Construct a bitmask to see which layers have content fulfilling the timesum to the current McpValue
		LayerEventMask = ( (RandomWalk & 0x1) ? FindSingleLayerEvent(u1It, u2It, VectorU1, VectorU2, *McpIt, fconTsum_u, fconSortU1, fconSortU2, fAllTsum_u) : FindSingleLayerEvent(u2It, u1It, VectorU2, VectorU1, *McpIt, fconTsum_u, fconSortU2, fconSortU1, fAllTsum_u) ) + ( (RandomWalk & 0x2) ? FindSingleLayerEvent(v1It, v2It, VectorV1, VectorV2, *McpIt, fconTsum_v, fconSortV1, fconSortV2, fAllTsum_v) : FindSingleLayerEvent(v2It, v1It, VectorV2, VectorV1, *McpIt, fconTsum_v, fconSortV2, fconSortV1, fAllTsum_v) ) * 2 + ( (RandomWalk & 0x4) ? FindSingleLayerEvent(w1It, w2It, VectorW1, VectorW2, *McpIt, fconTsum_w, fconSortW1, fconSortW2, fAllTsum_w) : FindSingleLayerEvent(w2It, w1It, VectorW2, VectorW1, *McpIt, fconTsum_w, fconSortW2, fconSortW1, fAllTsum_w) ) * 4;
		
		// We have to distiguish which wires have hat valid hits
		switch (LayerEventMask)
		{
			case 7 : // All wires
				AddUVWPosition(*McpIt, *u1It, *u2It, *v1It, *v2It, *w1It, *w2It);
				// Erase used times from delay line and MCP hit vectors
				u1It = VectorU1.erase(u1It);
				u2It = VectorU2.erase(u2It);
				v1It = VectorV1.erase(v1It);
				v2It = VectorV2.erase(v2It);
				w1It = VectorW1.erase(w1It);
				w2It = VectorW2.erase(w2It);
				McpIt = VectorMcp.erase(McpIt);
				break;
			case 6 : // V and W wire
				AddVWPosition(*McpIt, *v1It, *v2It, *w1It, *w2It);
				v1It = VectorV1.erase(v1It);
				v2It = VectorV2.erase(v2It);
				w1It = VectorW1.erase(w1It);
				w2It = VectorW2.erase(w2It);
				McpIt = VectorMcp.erase(McpIt);
				break;
			case 5 : // U and W wire
				AddUWPosition(*McpIt, *u1It, *u2It, *w1It, *w2It);
				// Erase used times from delay line and MCP hit vectors
				u1It = VectorU1.erase(u1It);
				u2It = VectorU2.erase(u2It);
				w1It = VectorW1.erase(w1It);
				w2It = VectorW2.erase(w2It);
				McpIt = VectorMcp.erase(McpIt);
				break;
			case 3 : // U and V wires
				AddUVPosition(*McpIt, *u1It, *u2It, *v1It, *v2It);
				// Erase used times from delay line and MCP hit vectors
				u1It = VectorU1.erase(u1It);
				u2It = VectorU2.erase(u2It);
				v1It = VectorV1.erase(v1It);
				v2It = VectorV2.erase(v2It);
				McpIt = VectorMcp.erase(McpIt);
				break;
			case 4 : // only W wire - not sufficient
			case 2 : // only V wire - not sufficient
			case 1 : // only U wire - not sufficient
			case 0 : // no wire
			default :
				++ McpIt;
				break;
		}
			
	}
	
	// Fill spectra showing sorting efficiency
	fSortedEvents->Fill(mDetectorHits.size());
	fSortvsMcpHits->Fill(mNumTofHits, mDetectorHits.size());
	if (mNumTofHits)
		fSortedYield->Fill(static_cast<Float_t>(mDetectorHits.size())/static_cast<Float_t>(mNumTofHits));
	
}

/** 
* \brief Calculates position for a hit where we only have a valid hit on the U and V wire
*/
void TRemiUnpackProcHexAnode::AddUVPosition(Float_t McpValue, Float_t U1Value, Float_t U2Value, Float_t V1Value, Float_t V2Value)
{
	Float_t uvalue = GetScaledUvalue ( U1Value - U2Value );
	Float_t vvalue = GetScaledVvalue ( V1Value - V2Value );
	TRemiXY XYvalues( CalculateXYfromUV(uvalue, vvalue) );
	
	AddXYPosition(McpValue, XYvalues);
	
	// Fill spectra
	fSortU->Fill(uvalue);
	fSortV->Fill(vvalue);
	fSortXY_uv->Fill(XYvalues.X(), XYvalues.Y());
	Float_t usum = U1Value + U2Value - 2 * McpValue;
	Float_t vsum = V1Value + V2Value - 2 * McpValue;
	fSortTsum_u->Fill(usum);
	fSortTsum_v->Fill(vsum);
	fSortU_Tsum_u->Fill(uvalue, usum);
	fSortV_Tsum_v->Fill(vvalue, vsum);
	fSortU1Mcp->Fill(U1Value - McpValue);
	fSortU2Mcp->Fill(U2Value - McpValue);
	fSortV1Mcp->Fill(V1Value - McpValue);
	fSortV2Mcp->Fill(V2Value - McpValue);
	
}

/** 
* \brief Calculates position for a hit where we only have a valid hit on the U and W wire
*/
void TRemiUnpackProcHexAnode::AddUWPosition(Float_t McpValue, Float_t U1Value, Float_t U2Value, Float_t W1Value, Float_t W2Value)
{
	Float_t uvalue = GetScaledUvalue ( U1Value - U2Value );
	Float_t wvalue = GetScaledWvalue ( W1Value - W2Value );
	TRemiXY XYvalues( CalculateXYfromUW(uvalue, wvalue) );
	
	AddXYPosition(McpValue, XYvalues);
	
	// Fill spectra
	fSortU->Fill(uvalue);
	fSortW->Fill(wvalue);
	fSortXY_uw->Fill(XYvalues.X(), XYvalues.Y());
	Float_t usum = U1Value + U2Value - 2 * McpValue;
	Float_t wsum = W1Value + W2Value - 2 * McpValue;
	fSortTsum_u->Fill(usum);
	fSortTsum_w->Fill(wsum);
	fSortU_Tsum_u->Fill(uvalue, usum);
	fSortW_Tsum_w->Fill(wvalue, wsum);
	fSortU1Mcp->Fill(U1Value - McpValue);
	fSortU2Mcp->Fill(U2Value - McpValue);
	fSortW1Mcp->Fill(W1Value - McpValue);
	fSortW2Mcp->Fill(W2Value - McpValue);
	
}

/** 
* \brief Calculates position for a hit where we only have a valid hit on the V and W wire
*/
void TRemiUnpackProcHexAnode::AddVWPosition(Float_t McpValue, Float_t V1Value, Float_t V2Value, Float_t W1Value, Float_t W2Value)
{
	Float_t vvalue = GetScaledVvalue ( V1Value - V2Value );
	Float_t wvalue = GetScaledWvalue ( W1Value - W2Value );
	TRemiXY XYvalues( CalculateXYfromVW(vvalue, wvalue) );
	
	AddXYPosition(McpValue, XYvalues);
	
	// Fill spectra
	fSortV->Fill(vvalue);
	fSortW->Fill(wvalue);
	fSortXY_vw->Fill(XYvalues.X(), XYvalues.Y());
	Float_t vsum = V1Value + V2Value - 2 * McpValue;
	Float_t wsum = W1Value + W2Value - 2 * McpValue;
	fSortTsum_v->Fill(vsum);
	fSortTsum_w->Fill(wsum);
	fSortV_Tsum_v->Fill(vvalue, vsum);
	fSortW_Tsum_w->Fill(wvalue, wsum);
	fSortV1Mcp->Fill(V1Value - McpValue);
	fSortV2Mcp->Fill(V2Value - McpValue);
	fSortW1Mcp->Fill(W1Value - McpValue);
	fSortW2Mcp->Fill(W2Value - McpValue);
	
}

/**
* \brief Calculates position for a hit where he have a valid hit on each wire
*/
void TRemiUnpackProcHexAnode::AddUVWPosition(Float_t McpValue, Float_t U1Value, Float_t U2Value, Float_t V1Value, Float_t V2Value, Float_t W1Value, Float_t W2Value)
{
	Float_t uvalue = GetScaledUvalue ( U1Value - U2Value );
	Float_t vvalue = GetScaledVvalue ( V1Value - V2Value );
	Float_t wvalue = GetScaledWvalue ( W1Value - W2Value );
	
	if (mUseScalef)
		FindBestScalefactors(uvalue, vvalue, wvalue);
	
	TRemiXY XY_uv ( CalculateXYfromUV(uvalue, vvalue) );
	TRemiXY XY_uw ( CalculateXYfromUW(uvalue, wvalue) );
	TRemiXY XY_vw ( CalculateXYfromVW(vvalue, wvalue) );
	TRemiXY XY_final (0,0);
	
	// Now we have three different XY positions and combine them to find the final values
	
	// Generate a bitmask that contains the information which distances between points are in the limit
	Short_t DistancesInLimit = (XY_uv.Distance(XY_uw) < fParaPos->distancemax) + (XY_uv.Distance(XY_vw) < fParaPos->distancemax) * 2 + (XY_uw.Distance(XY_vw) < fParaPos->distancemax) * 4; 
	
	switch (DistancesInLimit)
	{
		case 7 :	// All distances in limit
			XY_final = (XY_uv + XY_uw + XY_vw) / 3.;
			break;
		case 6 :	// Distances uv-vw and uw-vw in limits
			XY_final = (XY_uv + XY_uw + XY_vw * 2.) / 4.;
			break;
		case 5 :	// Distances uv-uw and uw-vw in limits
			XY_final = (XY_uv + XY_uw * 2. + XY_vw) / 4.;
			break;
		case 3 :	// Distances uv-uw and uv-vw in limits
			XY_final = (XY_uv * 2. + XY_uw + XY_vw) / 4.;
			break;
		case 4 :	// Only distance uw-vw in limit
			XY_final = (XY_uw + XY_vw) / 2.;
			break;
		case 2 :	// Only distance uv-vw in limit
			XY_final = (XY_uv + XY_vw) / 2.;
			break;
		case 1 :	// Only distance uv-uw in limit
			XY_final = (XY_uv + XY_uw) / 2.;
			break;
		case 0 :	// No distance in limit: Take a random position
		default :
			Int_t choice = fRandom.Integer(3);
			switch (choice)
			{
				case 0 : XY_final = XY_uv; break;
				case 1 : XY_final = XY_uw; break;
				case 2 : XY_final = XY_vw; break;
				default : break;
			}
			break;
	}
	
	AddXYPosition(McpValue, XY_final);
	
	// Fill spectra
	fSortU->Fill(uvalue);
	fSortV->Fill(vvalue);
	fSortW->Fill(wvalue);
	fSortXY_uv->Fill(XY_uv.X(), XY_uv.Y());
	fSortXY_uw->Fill(XY_uw.X(), XY_uw.Y());
	fSortXY_vw->Fill(XY_vw.X(), XY_vw.Y());
	fYuv_yuw->Fill(XY_uv.Y() - XY_uw.Y(), XY_uw.Y());
	fXuv_xuw->Fill(XY_uv.X() - XY_uw.X(), XY_uw.X());
	fYuw_yvw->Fill(XY_uw.Y() - XY_vw.Y(), XY_vw.Y());
	fXuw_xvw->Fill(XY_uw.X() - XY_vw.X(), XY_vw.X());
	fYvw_yuv->Fill(XY_vw.Y() - XY_uv.Y(), XY_uv.Y());
	fXvw_xuv->Fill(XY_vw.X() - XY_uv.X(), XY_uv.X());
	Float_t usum = U1Value + U2Value - 2 * McpValue;
	Float_t vsum = V1Value + V2Value - 2 * McpValue;
	Float_t wsum = W1Value + W2Value - 2 * McpValue;
	fSortTsum_u->Fill(usum);
	fSortTsum_v->Fill(vsum);
	fSortTsum_w->Fill(wsum);
	fSortU_Tsum_u->Fill(uvalue, usum);
	fSortV_Tsum_v->Fill(vvalue, vsum);
	fSortW_Tsum_w->Fill(wvalue, wsum);
	fSortU1Mcp->Fill(U1Value - McpValue);
	fSortU2Mcp->Fill(U2Value - McpValue);
	fSortV1Mcp->Fill(V1Value - McpValue);
	fSortV2Mcp->Fill(V2Value - McpValue);
	fSortW1Mcp->Fill(W1Value - McpValue);
	fSortW2Mcp->Fill(W2Value - McpValue);
}


/**
 * \brief Adds a calculated position to the list of detector hits
 * \param McpValue Mcp hit time
 * \param XValue X position
 * \param YValue Y position
 */
void TRemiUnpackProcHexAnode::AddXYPosition(Float_t McpValue, TRemiXY &XYvalues)
{
	TRemiDetectorHit newhit = TRemiDetectorHit(CalculateSingleTof(McpValue), XYvalues);
	newhit.SetIndex(mDetectorHits.size());
	newhit.Rotate(fParaPos->phi);
	mDetectorHits.push_back(newhit);
	
}

/**
* \brief Reconstruction routines applied before any position finding
*/
void TRemiUnpackProcHexAnode::Reconstruct()
{
	Int_t NumReconstruct = 0;
	
	// Reconstruct missing MCP signals
	if (fParaReconst->useRecoMCP)
	{
		NumReconstruct += ReconstructMcpUVW();
		NumReconstruct += ReconstructMcpUV();
		NumReconstruct += ReconstructMcpUW();
		NumReconstruct += ReconstructMcpVW();
		
		// Fill some histograms
		fRecoMcp->Fill(NumReconstruct);
		fRecoMcpSorted->Fill(mDetectorHits.size());
		fRecoMcpSortvsMcpHits->Fill(mNumTofHits, mDetectorHits.size());
		if (mNumTofHits)
			fRecoMcpSortedYield->Fill(static_cast<Float_t>(mDetectorHits.size())/static_cast<Float_t>(mNumTofHits));
	}
	
	// Reconstruct missing delayline signals
	if (fParaReconst->useRecoDelayline)
		NumReconstruct += ReconstructHexAnode();
	
	// We may have to sort the detector hits now
	if (NumReconstruct)
		std::sort (mDetectorHits.begin(), mDetectorHits.end());
	
}

/**
* \brief Reconstruction of MCP signals from U, V and W layers
* \return Number of reconstructed MCP signals
*/
Int_t TRemiUnpackProcHexAnode::ReconstructMcpUVW()
{
	// Create references to input vectors
	std::vector<Float_t> &VectorU1 = fRawData->fiTDC[mTdcChanU1];
	std::vector<Float_t> &VectorU2 = fRawData->fiTDC[mTdcChanU2];
	std::vector<Float_t> &VectorV1 = fRawData->fiTDC[mTdcChanV1];
	std::vector<Float_t> &VectorV2 = fRawData->fiTDC[mTdcChanV2];
	std::vector<Float_t> &VectorW1 = fRawData->fiTDC[mTdcChanW1];
	std::vector<Float_t> &VectorW2 = fRawData->fiTDC[mTdcChanW2];
	
	Int_t NumReconstruct=0;
	Char_t RandomWalk = fRandom.Integer(8); // Variable is assigned a random value to deside on which side of the detector one should start searching a hit
	
	std::vector<Float_t>::iterator u1It=VectorU1.begin(), u2It=VectorU2.begin(), v1It, v2It, w1It, w2It;
	Float_t	McpValueU, McpValueV, McpValueW;
	Float_t toleranceUV = ( fconTsum_u->GetXUp() - fconTsum_u->GetXLow() + fconTsum_v->GetXUp() -fconTsum_v->GetXLow() ) / 4.;	// Tolerance is the maximum allowed deviation between the MCP values calculated with each wire
	Float_t toleranceUW = ( fconTsum_u->GetXUp() - fconTsum_u->GetXLow() + fconTsum_w->GetXUp() -fconTsum_w->GetXLow() ) / 4.;	// Tolerance is the maximum allowed deviation between the MCP values calculated with each wire
	Float_t toleranceVW = ( fconTsum_v->GetXUp() - fconTsum_v->GetXLow() + fconTsum_w->GetXUp() -fconTsum_w->GetXLow() ) / 4.;	// Tolerance is the maximum allowed deviation between the MCP values calculated with each wire
	Bool_t	FoundHit;
	while ( (RandomWalk & 0x1) ? fReconstruct->RecoMCP(u1It, u2It, McpValueU, VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2) : fReconstruct->RecoMCP(u2It, u1It, McpValueU, VectorU2, VectorU1, fconTsum_u, fconSortU2, fconSortU1)) 
	{
		v1It=VectorV1.begin(), v2It=VectorV2.begin(); // Reset iterators for V
		FoundHit = kFALSE;
		while ( (RandomWalk & 0x2) ? fReconstruct->RecoMCP(v1It, v2It, McpValueV, VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2) : fReconstruct->RecoMCP(v2It, v1It, McpValueV, VectorV2, VectorV1, fconTsum_v, fconSortV2, fconSortV1))  
		{
			w1It=VectorW1.begin(), w2It=VectorW2.begin(); // Reset iterators for W
			while ( (RandomWalk & 0x4) ? fReconstruct->RecoMCP(w1It, w2It, McpValueW, VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2) : fReconstruct->RecoMCP(w2It, w1It, McpValueW, VectorW2, VectorW1, fconTsum_w, fconSortW2, fconSortW1))  
			{
				if ( (TMath::Abs(McpValueU - McpValueV) < toleranceUV) && (TMath::Abs(McpValueU - McpValueW) < toleranceUW)  && (TMath::Abs(McpValueV - McpValueW) < toleranceVW) ) 
				{ 
					// now we have found a MCP value satisfying all three layers
					AddUVWPosition( (McpValueU + McpValueV + McpValueW)/3., *u1It, *u2It, *v1It, *v2It, *w1It, *w2It );
					
					// Erase used times from delay line and MCP hit vectors
					u1It = VectorU1.erase(u1It);
					u2It = VectorU2.erase(u2It);
					v1It = VectorV1.erase(v1It);
					v2It = VectorV2.erase(v2It);
					w1It = VectorW1.erase(w1It);
					w2It = VectorW2.erase(w2It);
					if (RandomWalk & 0x1)
						u2It = VectorU2.begin(); 
					else 
						u1It = VectorU1.begin(); // reset second u iterator. Note: always the iterator passed second to fReconstruct->RecoMCP has to be reseted
					if (RandomWalk & 0x1)
						v2It = VectorV2.begin(); 
					else 
						v1It = VectorV1.begin(); // reset second v iterator. Note: always the iterator passed second to fReconstruct->RecoMCP has to be reseted
						FoundHit = kTRUE;
					NumReconstruct++;
					break; // break w loop, because first a new u and w hit have to be generated
				}
				else
					(RandomWalk & 0x4) ? ++w2It : ++w1It; // If this second w value does not fit, go to next. Note: always the iterator passed second to fReconstruct->RecoMCP has to be incremented
			}
			if (FoundHit)
				break; // if we had a hit also break v loop because first a new u hit has to be found
			else
				(RandomWalk & 0x2) ? ++v2It : ++v1It; // If no suitable w hit was found for this u and v combination, go on to the next second hit. Note: always the iterator passed second to fReconstruct->RecoMCP has to be incremented
		}
		if (!FoundHit)
			(RandomWalk & 0x1) ? ++u2It : ++u1It; // If no suitable v and w hit was found for this u combination, go on to the next second hit. Note: always the iterator passed second to fReconstruct->RecoMCP has to be incremented
	}
	
	// Fill some histograms
	fRecoMcpUVW->Fill(NumReconstruct);
	
	return NumReconstruct;
}

/**
* \brief Reconstruction of MCP signals from U and V signal
* \return Number of reconstructed MCP signals
*/
Int_t TRemiUnpackProcHexAnode::ReconstructMcpUV()
{
	// Create references to input vectors
	std::vector<Float_t> &VectorU1 = fRawData->fiTDC[mTdcChanU1];
	std::vector<Float_t> &VectorU2 = fRawData->fiTDC[mTdcChanU2];
	std::vector<Float_t> &VectorV1 = fRawData->fiTDC[mTdcChanV1];
	std::vector<Float_t> &VectorV2 = fRawData->fiTDC[mTdcChanV2];
	
	Int_t NumReconstruct=0;
	Char_t RandomWalk = fRandom.Integer(4); // Variable is assigned a random value to deside on which side of the detector one should start searching a hit
	
	std::vector<Float_t>::iterator u1It=VectorU1.begin(), u2It=VectorU2.begin(), v1It, v2It;
	Float_t	McpValueU, McpValueV;
	Float_t tolerance = ( fconTsum_u->GetXUp() - fconTsum_u->GetXLow() + fconTsum_v->GetXUp() -fconTsum_v->GetXLow() ) / 4.;	// Tolerance is the maximum allowed deviation between the MCP values calculated with each wire
	Bool_t	FoundHit;
	while ( (RandomWalk & 0x1) ? fReconstruct->RecoMCP(u1It, u2It, McpValueU, VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2) : fReconstruct->RecoMCP(u2It, u1It, McpValueU, VectorU2, VectorU1, fconTsum_u, fconSortU2, fconSortU1)) 
	{
		v1It=VectorV1.begin(), v2It=VectorV2.begin(); // Reset iterators for V
		FoundHit = kFALSE;
		while ( (RandomWalk & 0x2) ? fReconstruct->RecoMCP(v1It, v2It, McpValueV, VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2) : fReconstruct->RecoMCP(v2It, v1It, McpValueV, VectorV2, VectorV1, fconTsum_v, fconSortV2, fconSortV1))  
		{
			if (TMath::Abs(McpValueU - McpValueV) < tolerance) 
			{ 
				// now we have found a MCP value satisfying both layers
				AddUVPosition( (McpValueU + McpValueV)/2., *u1It, *u2It, *v1It, *v2It );
				
				// Erase used times from delay line and MCP hit vectors
				u1It = VectorU1.erase(u1It);
				u2It = VectorU2.erase(u2It);
				v1It = VectorV1.erase(v1It);
				v2It = VectorV2.erase(v2It);
				if (RandomWalk & 0x1)
					u2It = VectorU2.begin(); 
				else 
					u1It = VectorU1.begin(); // reset second u iterator. Note: always the iterator passed second to fReconstruct->RecoMCP has to be reseted
					FoundHit = kTRUE;
				NumReconstruct++;
				break; // break v loop, because first a new u hit has to be generated
			}
			else
				(RandomWalk & 0x2) ? ++v2It : ++v1It; // If this second v value does not fit, go to next. Note: always the iterator passed second to fReconstruct->RecoMCP has to be incremented
		}
		if (!FoundHit)
			(RandomWalk & 0x1) ? ++u2It : ++u1It; // If no suitable v hit was found for this u combination, go on to the next second hit. Note: always the iterator passed second to fReconstruct->RecoMCP has to be incremented
	}
	
	// Fill some histograms
	fRecoMcpUV->Fill(NumReconstruct);
	
	return NumReconstruct;
}


/**
* \brief Reconstruction of MCP signals from U and W signal
* \return Number of reconstructed MCP signals
*/
Int_t TRemiUnpackProcHexAnode::ReconstructMcpUW()
{
	// Create references to input vectors
	std::vector<Float_t> &VectorU1 = fRawData->fiTDC[mTdcChanU1];
	std::vector<Float_t> &VectorU2 = fRawData->fiTDC[mTdcChanU2];
	std::vector<Float_t> &VectorW1 = fRawData->fiTDC[mTdcChanW1];
	std::vector<Float_t> &VectorW2 = fRawData->fiTDC[mTdcChanW2];
	
	Int_t NumReconstruct=0;
	Char_t RandomWalk = fRandom.Integer(4); // Variable is assigned a random value to deside on which side of the detector one should start searching a hit
	
	std::vector<Float_t>::iterator u1It=VectorU1.begin(), u2It=VectorU2.begin(), w1It, w2It;
	Float_t	McpValueU, McpValueW;
	Float_t tolerance = ( fconTsum_u->GetXUp() - fconTsum_u->GetXLow() + fconTsum_w->GetXUp() -fconTsum_w->GetXLow() ) / 4.;	// Tolerance is the maximum allowed deviation between the MCP values calculated with each wire
	Bool_t	FoundHit;
	while ( (RandomWalk & 0x1) ? fReconstruct->RecoMCP(u1It, u2It, McpValueU, VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2) : fReconstruct->RecoMCP(u2It, u1It, McpValueU, VectorU2, VectorU1, fconTsum_u, fconSortU2, fconSortU1)) 
	{
		w1It=VectorW1.begin(), w2It=VectorW2.begin(); // Reset iterators for W
		FoundHit = kFALSE;
		while ( (RandomWalk & 0x2) ? fReconstruct->RecoMCP(w1It, w2It, McpValueW, VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2) : fReconstruct->RecoMCP(w2It, w1It, McpValueW, VectorW2, VectorW1, fconTsum_w, fconSortW2, fconSortW1))  
		{
			if (TMath::Abs(McpValueU - McpValueW) < tolerance) 
			{ 
				// now we have found a MCP value satisfying both layers
				AddUWPosition( (McpValueU + McpValueW)/2., *u1It, *u2It, *w1It, *w2It );
				
				// Erase used times from delay line and MCP hit vectors
				u1It = VectorU1.erase(u1It);
				u2It = VectorU2.erase(u2It);
				w1It = VectorW1.erase(w1It);
				w2It = VectorW2.erase(w2It);
				if (RandomWalk & 0x1)
					u2It = VectorU2.begin(); 
				else 
					u1It = VectorU1.begin(); // reset second u iterator. Note: always the iterator passed second to fReconstruct->RecoMCP has to be reseted
					FoundHit = kTRUE;
				NumReconstruct++;
				break; // break w loop, because first a new u hit has to be generated
			}
			else
				(RandomWalk & 0x2) ? ++w2It : ++w1It; // If this second w value does not fit, go to next. Note: always the iterator passed second to fReconstruct->RecoMCP has to be incremented
		}
		if (!FoundHit)
			(RandomWalk & 0x1) ? ++u2It : ++u1It; // If no suitable w hit was found for this u combination, go on to the next second hit. Note: always the iterator passed second to fReconstruct->RecoMCP has to be incremented
	}
	
	// Fill some histograms
	fRecoMcpUW->Fill(NumReconstruct);
	
	return NumReconstruct;
}


/**
* \brief Reconstruction of MCP signals from V and W signal
* \return Number of reconstructed MCP signals
*/
Int_t TRemiUnpackProcHexAnode::ReconstructMcpVW()
{
	// Create references to input vectors
	std::vector<Float_t> &VectorV1 = fRawData->fiTDC[mTdcChanV1];
	std::vector<Float_t> &VectorV2 = fRawData->fiTDC[mTdcChanV2];
	std::vector<Float_t> &VectorW1 = fRawData->fiTDC[mTdcChanW1];
	std::vector<Float_t> &VectorW2 = fRawData->fiTDC[mTdcChanW2];
	
	Int_t NumReconstruct=0;
	Char_t RandomWalk = fRandom.Integer(4); // Variable is assigned a random value to deside on which side of the detector one should start searching a hit
	
	std::vector<Float_t>::iterator v1It=VectorV1.begin(), v2It=VectorV2.begin(), w1It, w2It;
	Float_t	McpValueV, McpValueW;
	Float_t tolerance = ( fconTsum_v->GetXUp() - fconTsum_v->GetXLow() + fconTsum_w->GetXUp() -fconTsum_w->GetXLow() ) / 4.;	// Tolerance is the maximum allowed deviation between the MCP values calculated with each wire
	Bool_t	FoundHit;
	while ( (RandomWalk & 0x1) ? fReconstruct->RecoMCP(v1It, v2It, McpValueV, VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2) : fReconstruct->RecoMCP(v2It, v1It, McpValueV, VectorV2, VectorV1, fconTsum_v, fconSortV2, fconSortV1)) 
	{
		w1It=VectorW1.begin(), w2It=VectorW2.begin(); // Reset iterators for W
		FoundHit = kFALSE;
		while ( (RandomWalk & 0x2) ? fReconstruct->RecoMCP(w1It, w2It, McpValueW, VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2) : fReconstruct->RecoMCP(w2It, w1It, McpValueW, VectorW2, VectorW1, fconTsum_w, fconSortW2, fconSortW1))  
		{
			if (TMath::Abs(McpValueV - McpValueW) < tolerance) 
			{ 
				// now we have found a MCP value satisfying both layers
				AddVWPosition( (McpValueV + McpValueW)/2., *v1It, *v2It, *w1It, *w2It );
				
				// Erase used times from delay line and MCP hit vectors
				v1It = VectorV1.erase(v1It);
				v2It = VectorV2.erase(v2It);
				w1It = VectorW1.erase(w1It);
				w2It = VectorW2.erase(w2It);
				if (RandomWalk & 0x1)
					v2It = VectorV2.begin(); 
				else 
					v1It = VectorV1.begin(); // reset second u iterator. Note: always the iterator passed second to fReconstruct->RecoMCP has to be reseted
					FoundHit = kTRUE;
				NumReconstruct++;
				break; // break w loop, because first a new u hit has to be generated
			}
			else
				(RandomWalk & 0x2) ? ++w2It : ++w1It; // If this second w value does not fit, go to next. Note: always the iterator passed second to fReconstruct->RecoMCP has to be incremented
		}
		if (!FoundHit)
			(RandomWalk & 0x1) ? ++v2It : ++v1It; // If no suitable w hit was found for this u combination, go on to the next second hit. Note: always the iterator passed second to fReconstruct->RecoMCP has to be incremented
	}
	
	// Fill some histograms
	fRecoMcpVW->Fill(NumReconstruct);
	
	return NumReconstruct;
}


/** 
 * \brief Reconstruct missing delay signals on the Hex anode
 */
Int_t TRemiUnpackProcHexAnode::ReconstructHexAnode()
{
	// Create references to input vectors
	std::vector<Float_t> &VectorMcp = fRawData->fiTDC[mTdcChanMCP];
	std::vector<Float_t> &VectorU1 = fRawData->fiTDC[mTdcChanU1];
	std::vector<Float_t> &VectorU2 = fRawData->fiTDC[mTdcChanU2];
	std::vector<Float_t> &VectorV1 = fRawData->fiTDC[mTdcChanV1];
	std::vector<Float_t> &VectorV2 = fRawData->fiTDC[mTdcChanV2];
	std::vector<Float_t> &VectorW1 = fRawData->fiTDC[mTdcChanW1];
	std::vector<Float_t> &VectorW2 = fRawData->fiTDC[mTdcChanW2];
	
	Int_t NumReconstruct = 0;
	mNumReconstructSingleKnownU = 0;
	mNumReconstructSingleKnownV = 0;
	mNumReconstructSingleKnownW = 0;
	mNumReconstructDoubleKnownU = 0;
	mNumReconstructDoubleKnownV = 0;
	mNumReconstructDoubleKnownW = 0;
	Bool_t HaveReconstructed;
	std::vector<Float_t>::iterator McpIt = VectorMcp.begin();
	while (McpIt != VectorMcp.end())
	{
		HaveReconstructed = kFALSE;
		switch(fRandom.Integer(12)) // Random choice where to start reconstruction. Note that for each choice we will always try to reconstruct from either u, v or w signal, but the order of trials will change
		{
			case 0:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1);
				break;
			case 1:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2);
				break;
			case 2:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1);
				break;
			case 3:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2);
				break;
			case 4:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1);
				break;
			case 5:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2);
				break;
			case 6:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1);
				break;
			case 7:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2);
				break;
			case 8:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1);
				break;
			case 9:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2);
				break;
			case 10:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2, VectorU2, fconTsum_u, fconSortU2, fconSortU1, kU1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2, VectorV2, fconTsum_v, fconSortV2, fconSortV1, kV1);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2, VectorW2, fconTsum_w, fconSortW2, fconSortW1, kW1);
				break;
			case 11:
				HaveReconstructed = ReconstructHexAnodeSingleHit(VectorV1, VectorV2, fconTsum_v, fconSortV1, fconSortV2, McpIt, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorW1, VectorW2, fconTsum_w, fconSortW1, fconSortW2, McpIt, VectorU1, fconTsum_u, fconSortU1, fconSortU2, kU2, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2);
				if (!HaveReconstructed)
					HaveReconstructed = ReconstructHexAnodeSingleHit(VectorU1, VectorU2, fconTsum_u, fconSortU1, fconSortU2, McpIt, VectorV1, fconTsum_v, fconSortV1, fconSortV2, kV2, VectorW1, fconTsum_w, fconSortW1, fconSortW2, kW2);
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
	fRecoDlineSingleKnownU->Fill(mNumReconstructSingleKnownU);
	fRecoDlineSingleKnownV->Fill(mNumReconstructSingleKnownV);
	fRecoDlineSingleKnownW->Fill(mNumReconstructSingleKnownW);
	fRecoDlineDoubleKnownU->Fill(mNumReconstructDoubleKnownU);
	fRecoDlineDoubleKnownV->Fill(mNumReconstructDoubleKnownV);
	fRecoDlineDoubleKnownW->Fill(mNumReconstructDoubleKnownW);
	fRecoDlineSorted->Fill(mDetectorHits.size());
	fRecoDlineSortvsMcpHits->Fill(mNumTofHits, mDetectorHits.size());
	if (mNumTofHits)
		fRecoDlineSortedYield->Fill( static_cast<Float_t>(mDetectorHits.size())/static_cast<Float_t>(mNumTofHits) );
	
	return NumReconstruct;
}


/**
* \brief Reconstructs a delay line hit on one or two wires
* \param KnownList1 First signal vector of known layer
* \param KnownList2 Signal signal vector of known layer
* \param KnownConTimeSum Time sum condition for known layer
* \param KnownConSort1 Valid range window condition for first signal of known layer
* \param KnownConSort2 Valid range window condition for second signal of known layer
* \param McpValue Mcp value
* \param UnknownAList1 One signal layer of unknown layer A
* \param UnknownAConTimeSum Time sum condition of unknown layer A
* \param UnknownAParaSort Sorting parameter of unknown layer A
* \param UnknownAConSort1 Valid range window condition for first signal of unknown layer A
* \param UnknownAConSort2 Valid range window condition for second signal of unknown layer A
* \param WhichReconstructA Indicator which signal is being reconstructed in the first place (U1, U2, V1, V2, W1 or W2)
* \param UnknownBList1 One signal layer of unknown layer B
* \param UnknownBConTimeSum Time sum condition of unknown layer B
* \param UnknownBParaSort Sorting parameter of unknown layer B
* \param UnknownBConSort1 Valid range window condition for first signal of unknown layer B
* \param UnknownBConSort2 Valid range window condition for second signal of unknown layer B
* \param WhichReconstructB Indicator which signal is being reconstructed in the second place (U1, U2, V1, V2, W1 or W2)
*/
Bool_t TRemiUnpackProcHexAnode::ReconstructHexAnodeSingleHit(std::vector< Float_t >& KnownList1, std::vector< Float_t >& KnownList2, TGo4WinCond* KnownConTimeSum, TGo4WinCond* KnownConSort1, TGo4WinCond* KnownConSort2, std::vector< Float_t >::iterator& McpIt, std::vector< Float_t >& UnknownAList1, TGo4WinCond* UnknownAConTimeSum, TGo4WinCond* UnknownAConSort1, TGo4WinCond* UnknownAConSort2, TRemiUnpackProcHexAnode::eHexAnodeReconstruct WhichReconstructA, std::vector< Float_t >& UnknownBList1, TGo4WinCond* UnknownBConTimeSum, TGo4WinCond* UnknownBConSort1, TGo4WinCond* UnknownBConSort2, TRemiUnpackProcHexAnode::eHexAnodeReconstruct WhichReconstructB)
{
	std::vector<Float_t>::iterator Known1It, Known2It, UnknownA1It, UnknownB1It;
	Char_t RandomWalk = fRandom.Integer(2); // Random variable to decide in which direction to look for a valid hit on the known wire
	if ( (RandomWalk) ? FindSingleLayerEvent(Known1It, Known2It, KnownList1, KnownList2, *McpIt, KnownConTimeSum, KnownConSort1, KnownConSort2) : FindSingleLayerEvent(Known2It, Known1It, KnownList2, KnownList1, *McpIt, KnownConTimeSum, KnownConSort2, KnownConSort1) )
	{
		Float_t NewValueA=0, NewValueB=0;
		// Bitmask encoding the results from the two unknown layers
		Char_t Result = (fReconstruct->RecoDelayline(UnknownA1It, NewValueA, UnknownAList1, *McpIt, UnknownAConTimeSum, UnknownAConSort1, UnknownAConSort2)) + (fReconstruct->RecoDelayline(UnknownB1It, NewValueB, UnknownBList1, *McpIt, UnknownBConTimeSum, UnknownBConSort1, UnknownBConSort2)) * 2;
		
		if (Result)
		{
			switch (Result)
			{
				case 1: // only unknown layer A
					AddReconstructedHexanodeHit(McpIt, Known1It, Known2It, UnknownA1It, NewValueA, WhichReconstructA, WhichReconstructB);
					UnknownA1It = UnknownAList1.erase(UnknownA1It);
					break;
					
				case 2: // only unknown layer B
					AddReconstructedHexanodeHit(McpIt, Known1It, Known2It, UnknownB1It, NewValueB, WhichReconstructB, WhichReconstructA);
					UnknownB1It = UnknownBList1.erase(UnknownB1It);
					break;
					
				case 3: // both unknown layers
					switch (WhichReconstructA)
					{
						case kU1:
							switch (WhichReconstructB)
							{
								case kV1:
									AddUVWPosition(*McpIt, NewValueA, *UnknownA1It, NewValueB, *UnknownB1It, *Known1It, *Known2It);
									break;
								case kV2:
									AddUVWPosition(*McpIt, NewValueA, *UnknownA1It, *UnknownB1It, NewValueB, *Known1It, *Known2It);
									break;
								case kW1:
								case kW2:
									std::cout << "Invalid reconstruction" << std::endl;
								default:
									break;
							}
							mNumReconstructDoubleKnownW++;
							break;
						case kU2:
							switch (WhichReconstructB)
							{
								case kV1:
									AddUVWPosition(*McpIt, *UnknownA1It, NewValueA, NewValueB, *UnknownB1It, *Known1It, *Known2It);
									break;
								case kV2:
									AddUVWPosition(*McpIt, *UnknownA1It, NewValueA, *UnknownB1It, NewValueB, *Known1It, *Known2It);
									break;
								case kW1:
								case kW2:
									std::cout << "Invalid reconstruction" << std::endl;
								default:
									break;
							}
							mNumReconstructDoubleKnownW++;
							break;
						case kV1:
							switch (WhichReconstructB)
							{
								case kW1:
									AddUVWPosition(*McpIt, *Known1It, *Known2It, NewValueA, *UnknownA1It, NewValueB, *UnknownB1It);
									break;
								case kW2:
									AddUVWPosition(*McpIt, *Known1It, *Known2It, NewValueA, *UnknownA1It, *UnknownB1It, NewValueB);
									break;
								case kU1:
								case kU2:
									std::cout << "Invalid reconstruction" << std::endl;
								default:
									break;
							}
							mNumReconstructDoubleKnownU++;
							break;
						case kV2:
							switch (WhichReconstructB)
							{
								case kW1:
									AddUVWPosition(*McpIt, *Known1It, *Known2It, *UnknownA1It, NewValueA, NewValueB, *UnknownB1It);
									break;
								case kW2:
									AddUVWPosition(*McpIt, *Known1It, *Known2It, *UnknownA1It, NewValueA, *UnknownB1It, NewValueB);
									break;
								case kU1:
								case kU2:
									std::cout << "Invalid reconstruction" << std::endl;
								default:
									break;
							}
							mNumReconstructDoubleKnownU++;
							break;
						case kW1:
							switch (WhichReconstructB)
							{
								case kU1:
									AddUVWPosition(*McpIt, NewValueB, *UnknownB1It, *Known1It, *Known2It, NewValueA, *UnknownA1It);
									break;
								case kU2:
									AddUVWPosition(*McpIt, *UnknownB1It, NewValueB, *Known1It, *Known2It, NewValueA, *UnknownA1It);
									break;
								case kV1:
								case kV2:
									std::cout << "Invalid reconstruction" << std::endl;
								default:
									break;
							}
							mNumReconstructDoubleKnownV++;
							break;
						case kW2:
							switch (WhichReconstructB)
							{
								case kU1:
									AddUVWPosition(*McpIt, NewValueB, *UnknownB1It, *Known1It, *Known2It, *UnknownA1It, NewValueA);
									break;
								case kU2:
									AddUVWPosition(*McpIt, *UnknownB1It, NewValueB, *Known1It, *Known2It, *UnknownA1It, NewValueA);
									break;
								case kV1:
								case kV2:
									std::cout << "Invalid reconstruction" << std::endl;
								default:
									break;
							}
							mNumReconstructDoubleKnownV++;
							break;
						default:
							break;
					}
					UnknownA1It = UnknownAList1.erase(UnknownA1It);
					UnknownB1It = UnknownBList1.erase(UnknownB1It);
					break;
				default:
					break;
			}
			
			McpIt = fRawData->fiTDC[mTdcChanMCP].erase(McpIt);
			Known1It = KnownList1.erase(Known1It);
			Known2It = KnownList2.erase(Known2It);
		return kTRUE;
		}
		else 
			return kFALSE;
	}
	else 
		return kFALSE;
}


void TRemiUnpackProcHexAnode::AddReconstructedHexanodeHit( std::vector<Float_t>::iterator &McpIt, std::vector<Float_t>::iterator &Known1It, std::vector<Float_t>::iterator &Known2It, std::vector<Float_t>::iterator &UnknownA1It, Float_t &NewValueA, TRemiUnpackProcHexAnode::eHexAnodeReconstruct WhichReconstructA, TRemiUnpackProcHexAnode::eHexAnodeReconstruct WhichReconstructB ) 
{
	switch (WhichReconstructA)
	{
		case kU1:
			switch (WhichReconstructB)
			{
				case kV1:
				case kV2: // Known layer is W
					AddUWPosition(*McpIt, NewValueA, *UnknownA1It, *Known1It, *Known2It);
					mNumReconstructSingleKnownW++;
					break;
				case kW1:
				case kW2: // Known layer is V
					AddUVPosition(*McpIt, NewValueA, *UnknownA1It, *Known1It, *Known2It);
					mNumReconstructSingleKnownV++;
					break;
				default:
					break;
			}
			break;
		case kU2:
			switch (WhichReconstructB)
			{
				case kV1:
				case kV2: // Known layer is W
					AddUWPosition(*McpIt, *UnknownA1It, NewValueA, *Known1It, *Known2It);
					mNumReconstructSingleKnownW++;
					break;
				case kW1:
				case kW2: // Known layer is V
					AddUVPosition(*McpIt, *UnknownA1It, NewValueA, *Known1It, *Known2It);
					mNumReconstructSingleKnownV++;
					break;
				default:
					break;
			}
			break;
		case kV1:
			switch (WhichReconstructB)
			{
				case kW1:
				case kW2: // Known layer is U
					AddUVPosition(*McpIt, *Known1It, *Known2It, NewValueA, *UnknownA1It);
					mNumReconstructSingleKnownU++;
					break;
				case kU1:
				case kU2: // Known layer is W
					AddVWPosition(*McpIt, NewValueA, *UnknownA1It, *Known1It, *Known2It);
					mNumReconstructSingleKnownW++;
					break;
				default:
					break;
			}
			break;
		case kV2:
			switch (WhichReconstructB)
			{
				case kW1:
				case kW2: // Known layer is U
					AddUVPosition(*McpIt, *Known1It, *Known2It, *UnknownA1It, NewValueA);
					mNumReconstructSingleKnownU++;
					break;
				case kU1:
				case kU2: // Known layer is W
					AddVWPosition(*McpIt, *UnknownA1It, NewValueA, *Known1It, *Known2It);
					mNumReconstructSingleKnownW++;
					break;
				default:
					break;
			}
			break;
		case kW1:
			switch (WhichReconstructB)
			{
				case kU1:
				case kU2: // Known layer is V
					AddVWPosition(*McpIt, *Known1It, *Known2It, NewValueA, *UnknownA1It);
					mNumReconstructSingleKnownV++;
					break;
				case kV1:
				case kV2: // Known layer is U
					AddUWPosition(*McpIt, *Known1It, *Known2It, NewValueA, *UnknownA1It);
					mNumReconstructSingleKnownU++;
					break;
				default:
					break;
			}
			break;
		case kW2:
			switch (WhichReconstructB)
			{
				case kU1:
				case kU2: // Known layer is V
					AddVWPosition(*McpIt, *Known1It, *Known2It, *UnknownA1It, NewValueA);
					mNumReconstructSingleKnownV++;
					break;
				case kV1:
				case kV2: // Known layer is U
					AddUWPosition(*McpIt, *Known1It, *Known2It, *UnknownA1It, NewValueA);
					mNumReconstructSingleKnownU++;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

/**
 * \brief Produces histogram to see distribution of best scale factors
 * 
 * This bases on the section "How to obtain scalefactors" on page 7 of Achim Czasch documentation for hexanodes
 * 
 * Can only be called if all three timesums are fulfilled.
 */
void TRemiUnpackProcHexAnode::FindBestScalefactors(Float_t &uvalue, Float_t &vvalue, Float_t &wvalue)
{
	Float_t this_u, this_v, this_w, scale_v, scale_w, best_scale_v=0, best_scale_w=0, error, minerror = mScalefMinerror;
	this_u = uvalue / fParaPos->scaleU; // un-apply current scale factor for u
	for (scale_v = mScalefMin; scale_v < mScalefMax; scale_v += mScalefStepsize)
		for (scale_w = mScalefMin; scale_w < mScalefMax; scale_w += mScalefStepsize)
		{
			this_v = vvalue / fParaPos->scaleV * scale_v;
			this_w = wvalue / fParaPos->scaleW * scale_w;
			error = TMath::Abs(this_v - this_u - this_w);
			if (error < minerror)
			{
				minerror = error;
				best_scale_v = scale_v;
				best_scale_w = scale_w;
			}
		}
	fScalefactors->Fill(best_scale_v, best_scale_w);
}

/** \brief Scales the raw difference U1 - U2 to the final U value */
Float_t TRemiUnpackProcHexAnode::GetScaledUvalue(Float_t Udiff)
{
	return Udiff * fParaPos->scaleU * fParaPos->size_mm / fParaPos->size_raw;
}

/** \brief Scales the raw difference V1 - V2 to the final V value */
Float_t TRemiUnpackProcHexAnode::GetScaledVvalue(Float_t Vdiff)
{
	return Vdiff * fParaPos->scaleV * fParaPos->size_mm / fParaPos->size_raw;
}

/** \brief Scales the raw difference W1 - W2 to the final W value */
Float_t TRemiUnpackProcHexAnode::GetScaledWvalue(Float_t Wdiff)
{
	return Wdiff * fParaPos->scaleW * fParaPos->size_mm / fParaPos->size_raw + fParaPos->shiftW;
}

/** \brief Calculates the XY position from U and V values */
TRemiXY TRemiUnpackProcHexAnode::CalculateXYfromUV(Float_t Uvalue, Float_t Vvalue)
{
	return TRemiXY (Uvalue - fParaPos->xo, 0.57735 * Uvalue - 1.1547 * Vvalue - fParaPos->yo);
}

/** \brief Calculates the XY position from U and W values */
TRemiXY TRemiUnpackProcHexAnode::CalculateXYfromUW(Float_t Uvalue, Float_t Wvalue)
{
	return TRemiXY (Uvalue - fParaPos->xo, - 0.57735 * Uvalue - 1.1547 * Wvalue - fParaPos->yo);
}

/** \brief Calculates the XY position from V and W values */
TRemiXY TRemiUnpackProcHexAnode::CalculateXYfromVW(Float_t Vvalue, Float_t Wvalue)
{
	return TRemiXY (Vvalue - Wvalue - fParaPos->xo, - 0.57735 * Vvalue - 0.57735 * Wvalue - fParaPos->yo);
}




ClassImp(TRemiUnpackProcHexAnode);
