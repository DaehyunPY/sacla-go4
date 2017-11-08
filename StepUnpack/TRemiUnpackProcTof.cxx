//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Base class for time-of-flight retrieval of a hit. This is the base class for position calculation
//----------------------------------------------------------------

#include "TRemiUnpackProcTof.h"

#include "../EventElement/TRemiExtractEvent.h"
#include "Parameters/TRemiUnpackParameter.h"

#include "../Core/Extern/ConfigFile.h"
#include "../Core/RootMod/TRemiRollingGraph.h"

#include "TGo4WinCond.h"

#include "TString.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TH2.h"

/**
 * \brief Default constructor. Not to be used!
 * 
 * Is used by objects of derived classes
 */
TRemiUnpackProcTof::TRemiUnpackProcTof() :
	fRandom(0)
{
}

/**
 * \brief Constructor to build a object of this class using configuration file
 */
TRemiUnpackProcTof::TRemiUnpackProcTof(ConfigFile * UnpackConf, TString particle) :
	TRemiIncludeFunctions(UnpackConf)
{
	TString fieldname, basename="unp_", hname, htitle, folder, basefolder = "Unpack", cname, ctitle;

	//------------------------------------------//
	// Read information from configuration file //
	//------------------------------------------//
	
	fieldname = basename + "use_pulser";
	mUsePulser = fConfigFile->read (fieldname.Data(), true);
	
	fieldname = basename + "use_delay";
	mUseDelay = fConfigFile->read (fieldname.Data(), false);
	fieldname = basename + "hist_delay_min";
	Double_t hist_delay_min = fConfigFile->read (fieldname.Data(), 0.);
	fieldname = basename + "hist_delay_max";
	Double_t hist_delay_max = fConfigFile->read (fieldname.Data(), 100.);
	fieldname = basename + "hist_delay_bins";
	Int_t hist_delay_bins = fConfigFile->read (fieldname.Data(), 100);
	
	basename = basename + particle + "_";
	
	fieldname = basename + "shortname";
	mShortName = fConfigFile->read (fieldname.Data(), TString(particle(0, 3)));
	fieldname = basename + "longname";
	mLongName = fConfigFile->read<std::string> (fieldname.Data(), particle.Data());
	
	fieldname = basename + "tdc_mcp";
	mTdcChanMCP = fConfigFile->read (fieldname.Data(), 0);
	mTdcBins2ns = fConfigFile->read ("unp_tdc_bin2ns", 1.);
	mTdcChanPulser = fConfigFile-> read("unp_tdc_pulser", 0);
	TString parname_global = fConfigFile->read<std::string> ("unp_parname_global", "Par unpack global");
	
	fieldname = basename + "hist_tof_min";
	Double_t hist_tof_min = fConfigFile->read (fieldname.Data(), 0.);
	fieldname = basename + "hist_tof_max";
	Double_t hist_tof_max = fConfigFile->read (fieldname.Data(), 1000.);
	fieldname = basename + "hist_tof_bins";
	Int_t hist_tof_bins = fConfigFile->read (fieldname.Data(), 1000);
	
	fieldname = basename + "graph_hits_num_points";
	Int_t rolling_graph_num = fConfigFile->read (fieldname.Data(), 10);
	fieldname = basename + "graph_hits_num_average";
	Int_t rolling_graph_average = fConfigFile->read(fieldname.Data(), 100);
	
	fieldname = basename + "hist_hits_bins";
	Int_t hist_hits_bins = fConfigFile->read(fieldname.Data(), 50);
	
	fieldname = basename + "tof_randomizer_seed";
	Int_t randomizerSeed = fConfigFile->read(fieldname.Data(), 0);
	fRandom = TRandom3(randomizerSeed);
	std::cout << fieldname << ": " << randomizerSeed << std::endl;
	
	//-------------------//
	// Create Parameters //
	//-------------------//
	fParaGlobal = MakeParam<TRemiUnpackParameter> (parname_global, basefolder); // This parameter should usually already be created by TRemiUnpackProc. In this case, no new parameter will be created, but the existing parameter will be taken from the analysis thread
	
	
	//-------------------//
	// Create histograms //
	//-------------------//


	folder = basefolder + "/" + mShortName + "_all";
	hname = "Tof_" + mShortName + "_all";
	htitle = mLongName + " time-of-flight all hits";
	fTofAll = MakeH1<TH1I> (hname, htitle, hist_tof_bins, hist_tof_min, hist_tof_max, folder, "[ns]");

	hname = "P" + mShortName + "P" + mShortName + "Co_all";
	htitle = "Tof1 vs. Tof2 " + mLongName + " all hits";
	fPiPiCoAll = MakeH2<TH2I>(hname, htitle, hist_tof_bins/4, hist_tof_min, hist_tof_max, hist_tof_bins/4, hist_tof_min, hist_tof_max, folder, "Tof1 [ns]", "Tof2 [ns]");

	hname = "P" + mShortName + "P" + mShortName + "No_all";
	htitle = "Tof1 vs. Tof2 " + mLongName + " all hits for subsequent bunches";
	fPiPiNoAll = MakeH2<TH2I>(hname, htitle, hist_tof_bins/4, hist_tof_min, hist_tof_max, hist_tof_bins/4, hist_tof_min, hist_tof_max, folder, "Tof1 [ns]", "Tof2 [ns]");

	if (mUseDelay)
	{
		folder = basefolder + "/Delay/" + mShortName;
		
		hname = "DelayTof_" + mShortName + "_all";
		htitle = "Delay vs. " + mLongName + "time-of-flight all hits";
		fDelayTofAll = MakeH2<TH2I>(hname, htitle, hist_tof_bins/2, hist_tof_min, hist_tof_max, hist_delay_bins/2, hist_delay_min, hist_delay_max, folder, "Tof [ns]", "Delay [fs]");
		
		hname = "Delay_ct_" + mShortName + "_all";
		htitle = "Delay with " + mLongName + " time-of-flight condition for all hits";
		fDelayCTofAll = MakeH1<TH1I>(hname, htitle, hist_delay_bins, hist_delay_min, hist_delay_max, folder, "Delay [fs]");
	}


	
	// Total counts spectra
	folder = basefolder + "/" + mShortName + "_all/det/counts";
	
	hname = "McpHits_" + mShortName;
	htitle = "No. of MCP Hits " + mLongName + " detector";
	fMcpHits = MakeH1<TH1I>(hname, htitle, hist_hits_bins, -.5, static_cast<Float_t>(hist_hits_bins)-.5, folder);
	
	// Rolling Graph for no. of MCP hits
	folder = basefolder + "/" + mShortName + "/";
	fGraphMcpHits = MakeRollingGraph("CurrentMcpHits_" + mShortName, "No. of MCP Hits " + mLongName + " detector", rolling_graph_num, rolling_graph_average, folder);
	
	// Neighbouring hits difference spectra
	folder = basefolder + "/" + mShortName + "_all/det/diff";
	
	hname = "TofDiff_" + mShortName + "_all";
	htitle = "Time-of-flight differences for neighbouring hits on the " + mLongName + " detector";
	TString binsname = basename + "hist_tofdiff_bins";
	TString minname = basename + "hist_tofdiff_min";
	TString maxname = basename + "hist_tofdiff_max";
	fTofDiffAll = MakeH1<TH1I>(hname, htitle, binsname, minname, maxname, folder,  "Tof 2 - Tof 1 [ns]");
	
	//-------------------//
	// Create conditions //
	//-------------------//
	
	// Conditions for the particles' times-of-flight
	
	folder = basefolder + "/" + mShortName + "/all";
	cname = "c_Tof_" + mShortName + "_all";
	ctitle = mLongName + " Time-of-flight condition for all hits";
	hname = "Tof_" + mShortName + "_all";
	fcon_TofAll = MakeWinCon(cname, ctitle, hist_tof_min, hist_tof_max, hname, folder);
	
	
	//-----------------------//
	// Create member vectors //
	//-----------------------//
	mTof = std::vector<Double_t>();
	mFlagTofAll = std::vector<Bool_t>();
}

/**
 * \brief Destructor
 */
TRemiUnpackProcTof::~TRemiUnpackProcTof()
{
}

/**
 * \brief Set the pointer to the current input object
 */
void TRemiUnpackProcTof::SetInput(TRemiExtractEvent *Input)
{
	fRawData = Input;
	mNumTofHits = fRawData->fiTDC[mTdcChanMCP].size();
	mFlagTofAll.resize(mNumTofHits);
	mTof.resize(mNumTofHits);
}

/** 
 * \brief Set Delay value
 */
void TRemiUnpackProcTof::SetDelay(Float_t delay)
{
	mDelayValue = delay;
}

/**
 * \brief return number of ToF hits
 */
UInt_t TRemiUnpackProcTof::GetNumTofHits()
{
	return mNumTofHits;
}


/**
 * \brief calculate Tofs of current event
 */
void TRemiUnpackProcTof::CalculateTof()
{
	mTofPrevious = mTof;

	for (UInt_t i=0; i < mNumTofHits; i++)
	{
		mTof[i] = CalculateSingleTof (fRawData->fiTDC[mTdcChanMCP][i]);
	}

}

/**
* \brief calculates single Tof in current event
*/
Double_t TRemiUnpackProcTof::CalculateSingleTof(Float_t McpTime)
{
	if ( mUsePulser && fRawData->fiTDC[mTdcChanPulser].size() )
		return ( mTdcBins2ns * static_cast<Double_t>(McpTime - fRawData->fiTDC[mTdcChanPulser].front()) - fParaGlobal->to );
	else 
		return ( mTdcBins2ns * static_cast<Float_t>(McpTime) - fParaGlobal->to );
	
	
}


/**
 * \brief Test time-of-flight window conditions
 */
void TRemiUnpackProcTof::TestTofCond()
{
	for (UInt_t i=0; i<mNumTofHits; i++)
		mFlagTofAll[i] = fcon_TofAll->Test(mTof[i]);
}

/**
 * \brief Fill time-of-flight histograms
 */
void TRemiUnpackProcTof::FillTofHistograms()
{
	fMcpHits->Fill(mNumTofHits);
	fGraphMcpHits->Fill(mNumTofHits);

	for (UInt_t i=0; i<mNumTofHits; ++i)
	{
		fTofAll->Fill(mTof[i]);

		for (UInt_t j=i+1; j<mNumTofHits; ++j)
		{
			fPiPiCoAll->Fill(mTof[i], mTof[j]);
		}

		for (UInt_t j=0; j<mTofPrevious.size(); ++j)
		{
			if (mTof[i] < mTofPrevious[j])
			{
				fPiPiNoAll->Fill(mTof[i], mTofPrevious[j]);
			}
		}

		if (mUseDelay)
		{
			fDelayTofAll->Fill(mTof[i], mDelayValue);
				
			if (mFlagTofAll[i])
			{
				fDelayCTofAll->Fill(mDelayValue);
			}
		}
	}

	for (UInt_t i=1; i<mNumTofHits; ++i)
		fTofDiffAll->Fill(mTof[i] - mTof[i-1]);
}

/**
 * \brief returns Tofs of current event
 */
void TRemiUnpackProcTof::GetTof(std::vector<Double_t> &Tof) const
{
	Tof.resize(mNumTofHits);
	for (UInt_t i=0; i<mNumTofHits; i++)
		Tof[i]=mTof[i];
}

/**
 * \brief returns the vector of boolean values for the Time-of-flight condition flags
 */
void TRemiUnpackProcTof::GetCondTof(std::vector<Bool_t> &CondTof) const
{
	CondTof.resize(mNumTofHits);
	for (UInt_t i=0; i<mNumTofHits; i++)
		CondTof[i]=mFlagTofAll[i];
}

/**
 * \brief Set the internal vector of time-of-flight values to a new value
 */
void TRemiUnpackProcTof::SetTof(std::vector<Double_t> tof)
{
	mTof = tof;
}

/**
* \brief Test if a single Tof value fulfills the time-of-flight condition
*/
Bool_t TRemiUnpackProcTof::TestTofCondition(Float_t Tof)
{
	return fcon_TofAll->Test(Tof);
}

ClassImp(TRemiUnpackProcTof);
