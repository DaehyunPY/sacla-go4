
//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the base class for all position-calculating classes
//----------------------------------------------------------------

#include "TRemiUnpackProcPos.h"

#include "../Core/Point/TRemiXY.h"
#include "../Core/Extern/ConfigFile.h"

#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"

#include <algorithm>

#include "TSystem.h"
#include "TTime.h"
#include "TH1.h"
#include "TH2.h"

/**
 * \brief Default constructor. Not to be used!
 */
TRemiUnpackProcPos::TRemiUnpackProcPos()
{
}

/**
 * \brief Principal constructor. Uses a configuration file
 */
TRemiUnpackProcPos::TRemiUnpackProcPos(ConfigFile* UnpackConf, TString particle) : 
	TRemiUnpackProcTof(UnpackConf, particle)
{
	TString fieldname, basename="unp_", hname, htitle, folder, basefolder = "Unpack", cname, ctitle, hxtitle, hytitle;


	fieldname = basename + particle + "_dettype";
	mDetType = fConfigFile->read<TString> (fieldname.Data(), "delayline");
	mDetType.ToLower();

	// Skip creation of sort histos and conditions for tofonly detectors
	if (mDetType == "tofonly")
	{
		return;
	}


	//------------------------------------------//
	// Read information from configuration file //
	//------------------------------------------//

	fieldname = basename + "hist_delay_min";
	Double_t hist_delay_min = fConfigFile->read (fieldname.Data(), 0.);
	fieldname = basename + "hist_delay_max";
	Double_t hist_delay_max = fConfigFile->read (fieldname.Data(), 100.);
	fieldname = basename + "hist_delay_bins";
	Int_t hist_delay_bins = fConfigFile->read (fieldname.Data(), 100);

	basename = basename + particle + "_";
	
	fieldname = basename + "hist_xy_min";
	Double_t hist_xy_min = fConfigFile->read (fieldname.Data(), -50);
	fieldname = basename + "hist_xy_max";
	Double_t hist_xy_max = fConfigFile->read (fieldname.Data(), 50);
	fieldname = basename + "hist_xy_bins";
	Int_t hist_xy_bins = fConfigFile->read (fieldname.Data(), 200);
	
	fieldname = basename + "hist_phi_min";
	Double_t hist_phi_min = fConfigFile->read (fieldname.Data(), 0.);
	fieldname = basename + "hist_phi_max";
	Double_t hist_phi_max = fConfigFile->read (fieldname.Data(), 1000.);
	fieldname = basename + "hist_phi_bins";
	Int_t hist_phi_bins = fConfigFile->read (fieldname.Data(), 1000);
	
	fieldname = basename + "hist_tof_min";
	Double_t hist_tof_min = fConfigFile->read (fieldname.Data(), 0.);
	fieldname = basename + "hist_tof_max";
	Double_t hist_tof_max = fConfigFile->read (fieldname.Data(), 1000.);
	fieldname = basename + "hist_tof_bins";
	Int_t hist_tof_bins = fConfigFile->read (fieldname.Data(), 1000);
	
	fieldname = basename + "hist_tofdiff_min";
	Double_t hist_tofdiff_min = fConfigFile->read (fieldname.Data(), 0.);
	fieldname = basename + "hist_tofdiff_max";
	Double_t hist_tofdiff_max = fConfigFile->read (fieldname.Data(), 1000.);
	fieldname = basename + "hist_tofdiff_bins";
	Int_t hist_tofdiff_bins = fConfigFile->read (fieldname.Data(), 1000);
	
	
	//-------------------//
	// Create histograms //
	//-------------------//
	
	// Histograms for sorted hits
	folder = basefolder + "/" + mShortName + "_sort";
	hname = "Tof_" + mShortName + "_sort";
	htitle = mLongName + " time-of-flight sorted hits";
	fSortTof = MakeH1<TH1I> (hname, htitle, hist_tof_bins, hist_tof_min, hist_tof_max, folder, "[ns]");
	
	hname = "Tof_ConPos_" + mShortName + "_sort";
	htitle = mLongName + " time-of-flight sorted hits with position condition";
	fSortTofConPos = MakeH1<TH1I> (hname, htitle, hist_tof_bins, hist_tof_min, hist_tof_max, folder, "[ns]");
	
	hname = "XY_" + mShortName + "_sort";
	htitle = mLongName + " position sorted hits";
	fSortXY = MakeH2<TH2I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "X [mm]", "Y [mm]");
	
	hname = "X_" + mShortName + "_sort";
	htitle = mLongName + " X coordinate sorted hits ";
	fSortX = MakeH1<TH1I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "X [mm]");
	
	
	hname = "Y_" + mShortName + "_sort";
	htitle = mLongName + " Y coordinate sorted hits ";
	fSortY = MakeH1<TH1I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "Y [mm]");
	
	hname = "R_" + mShortName + "_sort";
	htitle = mLongName + " R coordinate sorted hits ";
	fSortR = MakeH1<TH1I>(hname, htitle, hist_xy_bins, 0, hist_xy_max, folder, "R [mm]");

	
	hname = "XY_ConTof_" + mShortName + "_sort";
	htitle = mLongName + " position sorted hits with time-of-flight condition ";
	fSortXY_ct = MakeH2<TH2I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "X [mm]", "Y [mm]");
	

	hname = "X_Tof_" + mShortName + "_sort";
	htitle = mLongName + " X vs. time-of-flight sorted hits";
	fSortX_Tof = MakeH2<TH2I>(hname, htitle, hist_tof_bins/2, hist_tof_min, hist_tof_max, hist_xy_bins/2, hist_xy_min, hist_xy_max, folder, "Tof [ns]", "X [mm]");
	
	hname = "Y_Tof_" + mShortName + "_sort";
	htitle = mLongName + " Y vs. time-of-flight sorted hits";
	fSortY_Tof = MakeH2<TH2I>(hname, htitle, hist_tof_bins/2, hist_tof_min, hist_tof_max, hist_xy_bins/2, hist_xy_min, hist_xy_max, folder, "Tof [ns]", "Y [mm]");
	
	hname = "R_Tof_" + mShortName + "_sort";
	htitle = mLongName + "Radius vs. time-of-flight sorted hits";
	fSortR_Tof = MakeH2<TH2I>(hname, htitle, hist_tof_bins, hist_tof_min, hist_tof_max, hist_xy_bins, 0, hist_xy_max, folder, "Tof [ns]", "R [mm]");

	hname = "Phi_R_" + mShortName + "_sort";
	htitle = mLongName + "Azimutal angle vs. radius sorted hits";
	fSortR_Phi = MakeH2<TH2I>(hname, htitle, hist_xy_bins, 0, hist_xy_max, hist_phi_bins, hist_phi_min, hist_phi_max, folder, "R [mm]", "Phi [deg]");


	// Histograms for individual hits
	folder = basefolder + "/" + mShortName + "_sort_sep";

	fSortTofSepIndividual.resize(5);
	for (UShort_t i=0; i<fSortTofSepIndividual.size(); i++)
	{
		hname.Form("Tof_" + mShortName + "_sort_individual_%i", i+1);
		htitle.Form(mLongName + " time-of-flight for hit %i", i+1);
		fSortTofSepIndividual[i] = MakeH1<TH1I> (hname, htitle, hist_tof_bins, hist_tof_min, hist_tof_max, folder, "");
	}

	hname = "Tof_" + mShortName + "_sort_remaining";
	htitle = mLongName + " time-of-flight for remaining hits";
	fSortTofSepRemaining = MakeH1<TH1I> (hname, htitle, hist_tof_bins, hist_tof_min, hist_tof_max, folder, "");


	// Delay dependent plots
	if (mUseDelay)
	{
		folder = basefolder + "/" + mShortName + "_sort" + "/Delay";
		hname = "DelayTof_ConPos_" + mShortName + "_sort";
		htitle = "Delay vs. " + mLongName + "time-of-flight sorted hits";
		fSortDelayVsTofConPos = MakeH2<TH2I>(hname, htitle, hist_tof_bins/2, hist_tof_min, hist_tof_max, hist_delay_bins/2, hist_delay_min, hist_delay_max, folder, "Tof [ns]", "Delay [fs]");

		
		hname = "DelayX_" + mShortName + "_sort";
		htitle = "Delay vs. " + mLongName + "X sorted hits";
		fSortDelayVsX = MakeH2<TH2I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_delay_bins, hist_delay_min, hist_delay_max, folder, "X [mm]", "Delay [fs]");
		
		
		hname = "DelayY_" + mShortName + "_sort";
		htitle = "Delay vs. " + mLongName + "Y sorted hits";
		fSortDelayVsY = MakeH2<TH2I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_delay_bins, hist_delay_min, hist_delay_max, folder, "Y [mm]", "Delay [fs]");
		
		hname = "DelayR_" + mShortName + "_sort";
		htitle = "Delay vs. " + mLongName + "R sorted hits";
		fSortDelayVsR = MakeH2<TH2I>(hname, htitle, hist_xy_bins, 0, hist_xy_max, hist_delay_bins, hist_delay_min, hist_delay_max, folder, "R [mm]", "Delay [fs]");
		
		hname = "DelayTof_ConRad_" + mShortName + "_sort";
		htitle = "Delay vs. " + mLongName + "time-of-flight sorted hits";
		fSortDelayVsTofConRad = MakeH2<TH2I>(hname, htitle, hist_tof_bins, hist_tof_min, hist_tof_max, hist_delay_bins, hist_delay_min, hist_delay_max, folder, "Tof [ns]", "Delay [fs]");


		hname = "DelayX_ConTof_" + mShortName + "_sort";
		htitle = "Delay vs. X " + mLongName + " with condition on Tof sorted hits";
		fSortDelayVsXConTof = MakeH2<TH2I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_delay_bins, hist_delay_min, hist_delay_max, folder, "X [mm]", "Delay [fs]");

		hname = "DelayY_ConTof_" + mShortName + "_sort";
		htitle = "Delay vs. Y " + mLongName + " with condition on Tof sorted hits";
		fSortDelayVsYConTof = MakeH2<TH2I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_delay_bins, hist_delay_min, hist_delay_max, folder, "Y [mm]", "Delay [fs]");

		hname = "DelayX_ConTof_ConY_" + mShortName + "_sort";
		htitle = "Delay vs. X " + mLongName + " with condition on Tof and position Y sorted hits";
		fSortDelayVsXConTofConY = MakeH2<TH2I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_delay_bins, hist_delay_min, hist_delay_max, folder, "X [mm]", "Delay [fs]");
	}


	folder = basefolder + "/" + mShortName + "_sort";
	hname = "Realtime_X_" + mShortName + "_sort";
	htitle = "Sorted X "+ mLongName +" vs. real time of reading in the data ";
	Double_t starttime = (Long64_t) (gSystem->Now()) / 1000;
	fSortXvsRealtimeConTof = MakeH2<TH2I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_delay_bins, starttime, starttime+1e5, folder, "X [mm]", "Time [s]");


	// Histograms showing differences between neighbouring hits
	folder = basefolder + "/" + mShortName + "_sort" + "/det/diff";
	hname = "TofDiff_" + mShortName + "_sort";
	htitle = "Time-of-flight differences for sorted neighbouring hits on the " + mLongName + " detector";
	fSortTofDiff = MakeH1<TH1I>(hname, htitle, hist_tofdiff_bins, hist_tofdiff_min, hist_tofdiff_max, folder, "Tof 2 - Tof 1 [ns]");
	
	hname = "XdiffVsTofDiff_" + mShortName + "_sort";
	htitle = "X differences vs. time-of-flight differences for sorted neighbouring hits on the " + mLongName + " detector";
	fSortXdiffTofDiff = MakeH2<TH2I>(hname, htitle, hist_tofdiff_bins, hist_tofdiff_min, hist_tofdiff_max, hist_xy_bins, 2*hist_xy_min, 2*hist_xy_max, folder, "Tof 2 - Tof 1 [ns]", "X 2 - X 1 [mm]");
	
	hname = "YdiffVsTofDiff_" + mShortName + "_sort";
	htitle = "Y differences vs. time-of-flight differences for sorted neighbouring hits on the " + mLongName + " detector";
	fSortYdiffTofDiff = MakeH2<TH2I>(hname, htitle, hist_tofdiff_bins, hist_tofdiff_min, hist_tofdiff_max, hist_xy_bins, 2*hist_xy_min, 2*hist_xy_max, folder, "Tof 2 - Tof 1 [ns]", "X 2 - X 1 [mm]");
	
	
	//-------------------//
	// Create Conditions //
	//-------------------//
	
	folder = "Unpack/" + mShortName + "/sorted";
	hname = "XY_" + mShortName + "_sort";
	cname = "cXY_" + mShortName + "_sort";
	ctitle = mLongName + "Position condition sorted hits";
	fSortConPos = MakePolyCon(cname, ctitle, hname, folder);

	cname = "c_Rad_" + mShortName;
	ctitle = "Position radius condition for centering histograms" + mLongName;
	fConRad = MakeWinCon(cname, ctitle, fSortR->GetName(), folder);

	cname = "c_Y_" + mShortName + "_sort";
	ctitle = mLongName + "Position Y condition for sorted hits";
	fSortConY = MakeWinCon(cname, ctitle, fSortY->GetName(), folder);


	// Create other objects and vectors //
	//----------------------------------//
}

TRemiUnpackProcPos::~TRemiUnpackProcPos()
{
}

/**
 * \brief Fills position histograms
 */
void TRemiUnpackProcPos::FillPosHistograms() const
{
	// Skip filling sort histos and conditions for tofonly detectors
	if (mDetType == "tofonly")
	{
		return;
	}

	// Sorted hits histograms
	UShort_t i=0;
	for (std::vector<TRemiDetectorHit>::const_iterator HitsIt = mDetectorHits.begin(); HitsIt != mDetectorHits.end(); ++HitsIt, ++i)
	{
		if (i < fSortTofSepIndividual.size())
		{
			fSortTofSepIndividual[i]->Fill(HitsIt->Tof());
		}
		else
		{
			fSortTofSepRemaining->Fill(HitsIt->Tof());
		}

		fSortTof->Fill(HitsIt->Tof());
		fSortX->Fill(HitsIt->X());
		fSortY->Fill(HitsIt->Y());

		fSortR->Fill(HitsIt->R());
		fSortR_Phi->Fill(HitsIt->R() , HitsIt->Phi());
		
		fSortXY->Fill(HitsIt->X(), HitsIt->Y());
		fSortX_Tof->Fill(HitsIt->Tof(), HitsIt->X());
		fSortY_Tof->Fill(HitsIt->Tof(), HitsIt->Y());
		fSortR_Tof->Fill(HitsIt->Tof(), HitsIt->R());
		
		
		// Spectra with Delay
		if(mUseDelay)
		{
			fSortDelayVsX->Fill(HitsIt->X(), mDelayValue);
			fSortDelayVsY->Fill(HitsIt->Y(), mDelayValue);
			fSortDelayVsR->Fill(HitsIt->R(), mDelayValue);
		}

		// Spectra with Tof condition
		if (fcon_TofAll->Test(HitsIt->Tof()))
		{
			fSortXY_ct->Fill(HitsIt->X(), HitsIt->Y());

			if (mUseDelay)
			{
				fSortDelayVsXConTof->Fill(HitsIt->X(), mDelayValue);
				fSortDelayVsYConTof->Fill(HitsIt->Y(), mDelayValue);

				if (fSortConY->Test(HitsIt->Y()))
				{
					fSortDelayVsXConTofConY->Fill(HitsIt->X(), mDelayValue);
				}
			}

			Double_t realtime = (Long64_t) (gSystem->Now()) / 1e3;
			fSortXvsRealtimeConTof->Fill(HitsIt->X(), realtime);
		}

		// Spectra with Position condition
		if (fSortConPos->Test(HitsIt->X(), HitsIt->Y()) )
		{
			fSortTofConPos->Fill(HitsIt->Tof());

			if (mUseDelay)
			{
				fSortDelayVsTofConPos->Fill(HitsIt->Tof(), mDelayValue);
			}
		}

		// Spectra with radius condition
		if (fConRad->Test(HitsIt->R()))
		{
			if (mUseDelay)
			{
				fSortDelayVsTofConRad->Fill(HitsIt->Tof(), mDelayValue);
			}
		}
	}

	// Histograms with differences of neighbouring hits
	for (UInt_t i=1; i<mDetectorHits.size(); i++)
	{
		fSortTofDiff->Fill(mDetectorHits[i].Tof()-mDetectorHits[i-1].Tof());
		fSortXdiffTofDiff->Fill(mDetectorHits[i].Tof()-mDetectorHits[i-1].Tof(), mDetectorHits[i].X()-mDetectorHits[i-1].X());
		fSortYdiffTofDiff->Fill(mDetectorHits[i].Tof()-mDetectorHits[i-1].Tof(), mDetectorHits[i].Y()-mDetectorHits[i-1].Y());
	}
}


/**
 * \brief Returns a valid (good time sum) hit on a single layer for a given MCP value
 * \param List1It (output) Iterator pointing to the retrieved value from first list
 * \param List2It (output) Iterator pointing to the retrieved value from second list
 * \param TimeSum (output) calculated timesum
 * \param List1 List of time values from one end of the delay-line
 * \param List2 List of time values from the other end of the delay-line
 * \param McpValue Time of the MCP hit to test
 * \param ConTimeSum Current time sum condition
 * \param ConSort1 Valid range window condition for first signal
 * \param ConSort2 Valid range window condition for second signal
 * \param SumHist Current histogram to fill with time sum values
 */
Bool_t TRemiUnpackProcPos::FindSingleLayerEvent(std::vector<Float_t>::iterator &List1It, std::vector<Float_t>::iterator &List2It, std::vector< Float_t >& List1, std::vector< Float_t >& List2, Float_t McpValue, TGo4WinCond* ConTimeSum, TGo4WinCond* ConSort1, TGo4WinCond* ConSort2, TH1I* SumHist)
{
	if (List1.empty() || List2.empty())
		return kFALSE;
	
	std::vector<Float_t>::iterator List2Start;
	
	// Find iterator start point for first list
	List1It = std::lower_bound(List1.begin(), List1.end(), McpValue + ConSort1->GetXLow());
	if (List1It == List1.end())
		return kFALSE;
	
	// Find iterator start point for second list
	List2Start = std::lower_bound(List2.begin(), List2.end(), McpValue + ConSort2->GetXLow());
	if (List2Start == List2.end())
		return kFALSE;
	
	Float_t sum;
	Float_t EndValue1 = McpValue + ConSort1->GetXUp();
	Float_t EndValue2 = McpValue + ConSort2->GetXUp();
	do
	{
		List2It = List2Start;
		do 
		{
			sum = *List1It + *List2It - 2*McpValue;
			if (SumHist)
				SumHist->Fill(sum);
			if (ConTimeSum->Test(sum)) // We have found an event
			{
				return kTRUE; // we leave this function because an event was found. The iterators List1It and List2it point to the found delay line values
			}
			++ List2It;
		}
		while ( (List2It != List2.end()) && (*List2It < EndValue2));
		++ List1It;
	}
	while ( (List1It != List1.end()) && (*List1It < EndValue1));
	
	// If we have reached this point, no event was found
	return kFALSE;
}


/**
 * \brief Returns positions of current event
 * \param Position Output vector for the positions
 */
void TRemiUnpackProcPos::GetPosition(std::vector<TRemiXY> &Position) const
{ 
	Position.resize(mDetectorHits.size());
	for (UInt_t i=0; i<mDetectorHits.size(); i++)
	{
		Position[i].SetXY(mDetectorHits[i].X(), mDetectorHits[i].Y());
	}
}

/**
* \brief Returns Hits of current event
* \param Position Output vector for the hitss
*/
void TRemiUnpackProcPos::GetHits(std::vector< TRemiDetectorHit >& Hits) const
{
	Hits = mDetectorHits;
	
}

/** 
* \brief Tests if a hit fulfills the position condition
*/
Bool_t TRemiUnpackProcPos::TestPosCondition(TRemiDetectorHit& Hit)
{
	return fSortConPos->Test(Hit.X(), Hit.Y());
}

/** 
* \brief Tests if a hit fulfills the time-of-flight condition
*/
Bool_t TRemiUnpackProcPos::TestTofCondition(TRemiDetectorHit& Hit)
{
	return fcon_TofAll->Test(Hit.Tof());
}




ClassImp(TRemiUnpackProcPos);
