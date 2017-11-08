//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the main processing file for the Unpack step
//----------------------------------------------------------------

#include "TRemiUnpackProc.h"

#include "Parameters/TRemiUnpackParameter.h"
#include "TRemiUnpackProcHexAnode.h"
#include "TRemiUnpackProcDelayline.h"
#include "TRemiUnpackProcWaSAnode.h"
#include "TRemiUnpackProcTofOnly.h"
#include "TRemiUnpackProcDelay.h"
#include "TRemiUnpackProcPos.h"
#include "TRemiUnpackProcTof.h"
#include "../EventElement/TRemiUnpackEvent.h"
#include "../EventElement/TRemiExtractEvent.h"

#include "TGo4Log.h"
#include "TGo4PolyCond.h"
#include "TGo4WinCond.h"

#include "TH1.h"
#include "TH2.h"
#include "TClass.h"
#include "TROOT.h"

/**
 * \brief Default constructor, not used
 */
TRemiUnpackProc::TRemiUnpackProc() :
  TGo4EventProcessor()
{
}

/**
 * \brief Principal constructor, used in TRemiUnpackFact.cxx
 */
TRemiUnpackProc::TRemiUnpackProc(const char* name) :
TGo4EventProcessor(name), TRemiIncludeFunctions(UNPACKCONFIG)
{
	TGo4Log::LogfileEnable(kTRUE);
	std::cout << "**** TRemiUnpackProc: Create" << std::endl;

	// Definitions
	TString basefolder = "Unpack", folder, hname, htitle, cname, ctitle, fieldname, basename;


	//-----------------------------------//
	// Read information from config file //
	//-----------------------------------//

	mDetIon = fConfigFile->read<TString> ("unp_ions_dettype", "delayline");
	mDetElec = fConfigFile->read<TString> ("unp_electrons_dettype", "delayline");

	mDetIon.ToLower();
	mDetElec.ToLower();

	TString parname_global = fConfigFile->read<std::string> ("unp_parname_global", "Par unpack global");

	mUseDelay = fConfigFile->read ("unp_use_delay", false);

	// Constants for ions
	basename = "unp_ions_";

	fieldname = basename + "shortname";
	TString ion_short = fConfigFile->read<TString> (fieldname.Data(), "r");
	fieldname = basename + "longname";
	TString ion_long = fConfigFile->read<std::string> (fieldname.Data(), "recoil ion");

	fieldname = basename + "hist_tof_min";
	Double_t ion_hist_tof_min = fConfigFile->read(fieldname.Data(), 0.);
	fieldname = basename + "hist_tof_max";
	Double_t ion_hist_tof_max = fConfigFile->read(fieldname.Data(), 5000.);
	fieldname = basename + "hist_tof_bins";
	Int_t ion_hist_tof_bins = fConfigFile->read(fieldname.Data(), 1000);

	fieldname = basename + "hist_xy_min";
	Double_t ion_hist_xy_min = fConfigFile->read(fieldname.Data(), 0.);
	fieldname = basename + "hist_xy_max";
	Double_t ion_hist_xy_max = fConfigFile->read(fieldname.Data(), 5000.);
	fieldname = basename + "hist_xy_bins";
	Int_t ion_hist_xy_bins = fConfigFile->read(fieldname.Data(), 1000);

	// Constants for electrons
	basename = "unp_electrons_";

	fieldname = basename + "shortname";
	TString electron_short = fConfigFile->read<TString> (fieldname.Data(), "e");
	fieldname = basename + "longname";
	TString electron_long = fConfigFile->read<std::string> (fieldname.Data(), "electron");

	fieldname = basename + "hist_tof_min";
	Double_t electron_hist_tof_min = fConfigFile->read(fieldname.Data(), 0.);
	fieldname = basename + "hist_tof_max";
	Double_t electron_hist_tof_max = fConfigFile->read(fieldname.Data(), 5000.);
	fieldname = basename + "hist_tof_bins";
	Int_t electron_hist_tof_bins = fConfigFile->read(fieldname.Data(), 1000);

	fieldname = basename + "hist_xy_min";
	Double_t electron_hist_xy_min = fConfigFile->read(fieldname.Data(), 0.);
	fieldname = basename + "hist_xy_max";
	Double_t electron_hist_xy_max = fConfigFile->read(fieldname.Data(), 5000.);
	fieldname = basename + "hist_xy_bins";
	Int_t electron_hist_xy_bins = fConfigFile->read(fieldname.Data(), 1000);

	//Minibunch Channel
	
	mAdcChanMbIndex = fConfigFile->read("unp_adc_chan_mbindex", 0);
	
	// Intensity
	mUseIntensity = fConfigFile->read("unp_use_intensity", false);
	mAdcChanIntensity = fConfigFile->read("unp_adc_chan_intensity", -1);


	
	std::cout << "**** TRemiUnpackProc: Read information from Config File" << std::endl;
	
	//------------------------//
	// Creation of parameters //
	//------------------------//

	fParaUnpack = MakeParam<TRemiUnpackParameter>(parname_global, basefolder);

	std::cout << "**** TRemiUnpackProc: Created Parameter" << std::endl;
	
	//--------------------------------//
	// Initialise objects and vectors //
	//--------------------------------//
	
	// Electron hit vector
	mHit_e = std::vector<TRemiDetectorHit> ();

	// Electron detector object
	if (mDetElec == "hexanode")
	{
		TRemiUnpackProcHexAnode* ElectronHex = new TRemiUnpackProcHexAnode (fConfigFile, "electrons");
		fElectron = ElectronHex;
	}
	else if (mDetElec == "delayline")
	{
		TRemiUnpackProcDelayline* ElectronDel = new TRemiUnpackProcDelayline (fConfigFile, "electrons");
		fElectron = ElectronDel;
	}
	else if (mDetElec == "wedgeandstrip")
	{
		TRemiUnpackProcWaSAnode* ElectronWaS = new TRemiUnpackProcWaSAnode (fConfigFile, "electrons");
		fElectron = ElectronWaS;
	}
	else if (mDetElec == "tofonly")
	{
		TRemiUnpackProcTofOnly* ElectronTof = new TRemiUnpackProcTofOnly (fConfigFile, "electrons");
		fElectron = ElectronTof;
	}
	
	std::cout << "**** TRemiUnpackProc: Created Electron objects" << std::endl;
	
	
	// Recoil ion hit vector
	mHit_r = std::vector<TRemiDetectorHit> ();
	
	// Ion detector object
	if (mDetIon == "hexanode")
	{
		TRemiUnpackProcHexAnode* IonHex = new TRemiUnpackProcHexAnode (fConfigFile, "ions");
		fRecoil = IonHex;
	}
	else if (mDetIon == "delayline")
	{
		TRemiUnpackProcDelayline* IonDel = new TRemiUnpackProcDelayline (fConfigFile, "ions");
		fRecoil = IonDel;
	}
	else if (mDetIon == "wedgeandstrip")
	{
		TRemiUnpackProcWaSAnode* IonWaS = new TRemiUnpackProcWaSAnode (fConfigFile, "ions");
		fRecoil = IonWaS;
	}
	else if (mDetIon == "tofonly")
	{
		TRemiUnpackProcTofOnly* IonTof = new TRemiUnpackProcTofOnly (fConfigFile, "ions");
		fRecoil = IonTof;
	}
	std::cout << "**** TRemiUnpackProc: Created ion objects" << std::endl;
	
	// Delay stage objects
	if (mUseDelay)
		fDelay = new TRemiUnpackProcDelay(fConfigFile);

	// ------------------------//
	// Creation of histograms  //
	// ------------------------//
	// Here special histograms for the unpack step can be created
	// This is not necessary if Fill & Go is used for al5l histograms filled here
	

	// other Histograms
	fOutputValid = MakeH1<TH1I>("OutputValid", "Output events set to valid", 2, 0, 2, "Unpack");
	
	//Minibunches
	fHistMbIndex = MakeH1<TH1I>("MbIndexValue", "Mb-Index of single shot event", "unp_hist_mbindex_bins", "unp_hist_mbindex_min", "unp_hist_mbindex_max", "Unpack/Minibunch", "Mb-index");

	// Delay with conditions
	folder = basefolder + "/Delay";
	hname = "Delay_ConTof";
	hname = "unp_" + hname;
	htitle = "Delay with " + ion_long + " and " + electron_long + " Tof condition";
	fDelay_conTof = MakeH1<TH1I>(hname, htitle, "unp_hist_delay_bins", "unp_hist_delay_min", "unp_hist_delay_max", folder, "Delay [fs]");

	hname = "Delay_ConP" + ion_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = "Delay with " + ion_long + " P" + ion_short + "P" + ion_short + "Co condition";
	fDelay_conPiPiCo = MakeH1<TH1I>(hname, htitle, "unp_hist_delay_bins", "unp_hist_delay_min", "unp_hist_delay_max", folder, "Delay [fs]");

	hname = "Delay_ConP" + electron_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = "Delay with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + ion_short + "Co condition";
	fDelay_conPePiCo = MakeH1<TH1I>(hname, htitle, "unp_hist_delay_bins", "unp_hist_delay_min", "unp_hist_delay_max", folder, "Delay [fs]");

	hname = "Delay_ConP" + electron_short + "P" + ion_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = "Delay with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + ion_short + "P" + ion_short + "Co condition";
	fDelay_conPePiPiCo = MakeH1<TH1I>(hname, htitle, "unp_hist_delay_bins", "unp_hist_delay_min", "unp_hist_delay_max", folder, "Delay [fs]");

	hname = "Delay_ConP" + electron_short + "P" + electron_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = "Delay with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + electron_short + "P" + ion_short + "Co condition";
	fDelay_conPePePiCo = MakeH1<TH1I>(hname, htitle, "unp_hist_delay_bins", "unp_hist_delay_min", "unp_hist_delay_max", folder, "Delay [fs]");

	// XY position with conditions
	folder = basefolder + "/XY";

	hname = "XY_" + ion_short + "_sort" + "_ConP" + ion_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = ion_long + " position sorted hits with " + ion_long + " P" + ion_short + "P" + ion_short + "Co condition ";
	fPos_conPiPiCo = MakeH2<TH2I>(hname, htitle, ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, folder, "X [mm]", "Y [mm]");

	// ions
	hname = "XY_" + ion_short + "_sort" + "_ConTof";
	hname = "unp_" + hname;
	htitle = ion_long + " position sorted hits with " + ion_long + " and " + electron_long + " Tof condition";
	fPos_r_conTof = MakeH2<TH2I>(hname, htitle, 4*ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, 4*ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, folder, "X [mm]", "Y [mm]");

	hname = "XY_" + ion_short + "_sort" + "_ConP" + electron_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = ion_long + " position sorted hits with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + ion_short + "Co condition";
	fPos_r_conPePiCo = MakeH2<TH2I>(hname, htitle, ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, folder, "X [mm]", "Y [mm]");

	hname = "XY_" + ion_short + "_sort" + "_ConP" + electron_short + "P" + electron_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = ion_long + " position sorted hits with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + electron_short + "P" + ion_short + "Co condition";
	fPos_r_conPePePiCo = MakeH2<TH2I>(hname, htitle, ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, folder, "X [mm]", "Y [mm]");

	hname = "XY_" + ion_short + "_sort" + "_ConP" + electron_short + "P" + ion_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = ion_long + " position sorted hits with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + ion_short + "P" + ion_short + "Co condition";
	fPos_r_conPePiPiCo = MakeH2<TH2I>(hname, htitle, ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, folder, "X [mm]", "Y [mm]");

	// electrons
	hname = "XY_" + electron_short + "_sort" + "_ConTof";
	hname = "unp_" + hname;
	htitle = electron_long + " position sorted hits with " + ion_long + " and " + electron_long + " Tof condition";
	fPos_e_conTof = MakeH2<TH2I>(hname, htitle, electron_hist_xy_bins, electron_hist_xy_min, electron_hist_xy_max, electron_hist_xy_bins, electron_hist_xy_min, electron_hist_xy_max, folder, "X [mm]", "Y [mm]");

	hname = "XY_" + electron_short + "_sort" + "_ConP" + electron_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = electron_long + " position sorted hits with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + ion_short + "Co condition";
	fPos_e_conPePiCo = MakeH2<TH2I>(hname, htitle, electron_hist_xy_bins, electron_hist_xy_min, electron_hist_xy_max, electron_hist_xy_bins, electron_hist_xy_min, electron_hist_xy_max, folder, "X [mm]", "Y [mm]");

	hname = "XY_" + electron_short + "_sort" + "_ConP" + electron_short + "P" + electron_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = electron_long + " position sorted hits with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + electron_short + "P" + ion_short + "Co condition";
	fPos_e_conPePePiCo = MakeH2<TH2I>(hname, htitle, electron_hist_xy_bins, electron_hist_xy_min, electron_hist_xy_max, electron_hist_xy_bins, electron_hist_xy_min, electron_hist_xy_max, folder, "X [mm]", "Y [mm]");

	hname = "XY_" + electron_short + "_sort" + "_ConP" + electron_short + "P" + ion_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = electron_long + " position sorted hits with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + ion_short + "P" + ion_short + "Co condition";
	fPos_e_conPePiPiCo = MakeH2<TH2I>(hname, htitle, electron_hist_xy_bins, electron_hist_xy_min, electron_hist_xy_max, electron_hist_xy_bins, electron_hist_xy_min, electron_hist_xy_max, folder, "X [mm]", "Y [mm]");

	// X-position vs. Delay histograms with conditions
	folder = basefolder + "/X_Delay";

	hname = "X_Delay_" + ion_short + "_sort" + "_ConTof";
	hname = "unp_" + hname;
	htitle = ion_long + " X vs Delay sorted hits with " + ion_long + " and " + electron_long + " Tof condition";
	fX_r_Delay_conTof = MakeH2<TH2I>(hname, htitle, "unp_electrons_hist_xy_bins", "unp_electrons_hist_xy_min", "unp_electrons_hist_xy_max", "unp_hist_delay_bins", "unp_hist_delay_min", "unp_hist_delay_max", folder, "X [mm]", "Delay");

	hname = "X_Delay_" + ion_short + "_sort" + "_ConP" + electron_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = ion_long + " X vs Delay sorted hits with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + ion_short + "Co condition";
	fX_r_Delay_conPePiCo = MakeH2<TH2I>(hname, htitle, "unp_ions_hist_xy_bins", "unp_ions_hist_xy_min", "unp_ions_hist_xy_max", "unp_hist_delay_bins", "unp_hist_delay_min", "unp_hist_delay_max", folder, "X [mm]", "Delay");

	hname = "X_Delay_" + electron_short + "_sort" + "_ConTof";
	hname = "unp_" + hname;
	htitle = electron_long + " X vs Delay sorted hits with " + ion_long + " and " + electron_long + " Tof condition";
	fX_e_Delay_conTof = MakeH2<TH2I>(hname, htitle, "unp_electrons_hist_xy_bins", "unp_electrons_hist_xy_min", "unp_electrons_hist_xy_max", "unp_hist_delay_bins", "unp_hist_delay_min", "unp_hist_delay_max", folder, "X [mm]", "Delay");

	hname = "X_Delay_" + electron_short + "_sort" + "_ConP" + electron_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = electron_long + " X vs Delay sorted hits with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + ion_short + "Co condition";
	fX_e_Delay_conPePiCo = MakeH2<TH2I>(hname, htitle, "unp_electrons_hist_xy_bins", "unp_electrons_hist_xy_min", "unp_electrons_hist_xy_max", "unp_hist_delay_bins", "unp_hist_delay_min", "unp_hist_delay_max", folder, "X [mm]", "Delay");

	hname = "X_Delay_" + electron_short + "_sort" + "_ConTof_ConY";
	hname = "unp_" + hname;
	htitle = electron_long + " X vs Delay sorted hits with " + ion_long + " and " + electron_long + " P" + electron_short + "P" + ion_short + "Co condition + Y";
	fX_e_Delay_conTofconY = MakeH2<TH2I>(hname, htitle, "unp_electrons_hist_xy_bins", "unp_electrons_hist_xy_min", "unp_electrons_hist_xy_max", "unp_hist_delay_bins", "unp_hist_delay_min", "unp_hist_delay_max", folder, "X [mm]", "Delay");

	// Ion-Ion Coincidence histograms
	folder = basefolder + "/P" + ion_short + "P" + ion_short + "Coin";

	hname = "P" + ion_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = ion_long + " Tof 1 vs. Tof 2";
	fPiPiCo = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, folder, "Tof 1 [ns]", "Tof 2 [ns]");

	hname = "P" + ion_short + "P" + ion_short + "Co";
	hname += "_ConPos";
	hname = "unp_" + hname;
	htitle = ion_long + " Tof 1 vs. Tof 2 with " + ion_long + " position condition";
	fPiPiCo_conPos = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, folder, "Tof 1 [ns]", "Tof 2 [ns]");

	// Electron-Ion Coincidence histograms
	folder = basefolder + "/P" + electron_short + "P" + ion_short + "Coin";

	hname = "P" + electron_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = electron_long + " Tof vs. " + ion_long + " Tof";
	fPePiCo = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, electron_hist_tof_bins/4, electron_hist_tof_min, electron_hist_tof_max, folder, ion_long + "Tof [ns]", electron_long + "Tof 2 [ns]");

	hname = "P" + electron_short + "P" + ion_short + "Co";
	hname += "_ConPos";
	hname = "unp_" + hname;
	htitle = electron_long + " Tof vs. " + ion_long + " Tof with " + electron_long + " and " + ion_long + " position condition";
	fPePiCo_conPos = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins, ion_hist_tof_min, ion_hist_tof_max, electron_hist_tof_bins, electron_hist_tof_min, electron_hist_tof_max, folder, "Tof 1 [ns]", "Tof 2 [ns]");

	// Electron-Electron-Ion Coincidence histograms
	folder = basefolder + "/P" + electron_short + "P" + electron_short + "P" + ion_short + "Coin";

	hname = "P" + electron_short + "P" + electron_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = electron_long + " Tof 1 + Tof 2 vs. " + ion_long + " Tof";
	fPePePiCo = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, electron_hist_tof_bins/4, electron_hist_tof_min, electron_hist_tof_max, folder, ion_long + "Tof [ns]", electron_long + "Tof 1 + Tof 2 [ns]");

	hname = "P" + electron_short + "P" + electron_short + "P" + ion_short + "Co";
	hname += "_ConPos";
	hname = "unp_" + hname;
	htitle = electron_long + " Tof 1 + Tof 2 vs. " + ion_long + " Tof with " + electron_long + " and " + ion_long + " position condition";
	fPePePiCo_conPos = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins, ion_hist_tof_min, ion_hist_tof_max, electron_hist_tof_bins, electron_hist_tof_min, electron_hist_tof_max, folder, ion_long + "Tof [ns]", electron_long + "Tof 1 + Tof 2 [ns]");

	hname = "P" + electron_short + "P" + electron_short + "P" + ion_short + "Co";
	hname += "_ConTof";
	hname = "unp_" + hname;
	htitle = electron_long + " Tof 1 + Tof 2 vs. " + ion_long + " Tof with " + electron_long + " and " + ion_long + " Tof condition";
	fPePePiCo_conTof = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, electron_hist_tof_bins/4, electron_hist_tof_min, electron_hist_tof_max, folder, ion_long + "Tof [ns]", electron_long + "Tof 1 + Tof 2 [ns]");

	// Electron-Ion-Ion Coincidence histograms
	folder = basefolder + "/P" + electron_short + "P" + ion_short + "P" + ion_short + "Coin";

	hname = "P" + electron_short + "P" + ion_short + "P" + ion_short + "Co";
	hname = "unp_" + hname;
	htitle = ion_long + " Tof 1 + Tof 2 vs. " + electron_long + " Tof";
	fPePiPiCo = MakeH2<TH2I>(hname, htitle, electron_hist_tof_bins/4, electron_hist_tof_min, electron_hist_tof_max, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, folder, electron_long + "Tof [ns]", ion_long + "Tof 1 + Tof 2 [ns]");

	hname = "P" + electron_short + "P" + ion_short + "P" + ion_short + "Co";
	hname += "_ConPos";
	hname = "unp_" + hname;
	htitle = ion_long + " Tof 1 + Tof 2 vs. " + electron_long + " Tof with " + electron_long + " and " + ion_long + " position condition";
	fPePiPiCo_conPos = MakeH2<TH2I>(hname, htitle, electron_hist_tof_bins, electron_hist_tof_min, electron_hist_tof_max, ion_hist_tof_bins, ion_hist_tof_min, ion_hist_tof_max, folder, electron_long + "Tof [ns]", ion_long + "Tof 1 + Tof 2 [ns]");

	hname = "P" + electron_short + "P" + ion_short + "P" + ion_short + "Co";
	hname += "_ConTof";
	hname = "unp_" + hname;
	htitle = ion_long + " Tof 1 + Tof 2 vs. " + electron_long + " Tof with " + electron_long + " and " + ion_long + " Tof condition";
	fPePiPiCo_conTof = MakeH2<TH2I>(hname, htitle, electron_hist_tof_bins/4, electron_hist_tof_min, electron_hist_tof_max, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, folder, electron_long + "Tof [ns]", ion_long + "Tof 1 + Tof 2 [ns]");

	// Triple Ion Coincidence histograms
	folder = basefolder + "/TriP" + ion_short + "Coin";

	hname = "TriP" + ion_short + "Co1";
	hname = "unp_" + hname;
	htitle = ion_long + " Tof 1 vs. Tof 2 + Tof 3";
	fTriPiCo1 = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, ion_hist_tof_bins/4, ion_hist_tof_min, 2*ion_hist_tof_max, folder, "Tof 1 [ns]", "Tof 2 + Tof 3 [ns]");

	hname = "TriP" + ion_short + "Co2";
	hname = "unp_" + hname;
	htitle = ion_long + " Tof 2 vs. Tof 1 + Tof 3";
	fTriPiCo2 = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins/4, ion_hist_tof_min, 2*ion_hist_tof_max, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max/2, folder, "Tof 2 [ns]", "Tof 1 + Tof 3 [ns]");

	hname = "TriP" + ion_short + "Co3";
	hname = "unp_" + hname;
	htitle = ion_long + " Tof 3 vs. Tof 1 + Tof 2";
	fTriPiCo3 = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins/4, ion_hist_tof_min, 2*ion_hist_tof_max, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max/2, folder, "Tof 3 [ns]", "Tof 1 + Tof 2 [ns]");

	std::cout << "**** TRemiUnpackProc: Created histograms" << std::endl;


	//------------------------//
	// Creation of conditions //
	//------------------------//
	folder = basefolder + "/P" + ion_short + electron_short + "Coin";

	hname = fPiPiCo->GetName();
	cname = "c_" + hname;
	htitle = fPiPiCo->GetTitle();
	ctitle = htitle + " condition";
	fconPiPiCo = MakePolyCon(cname, ctitle, hname, folder);

	hname = fPePiCo->GetName();
	cname = "c_" + hname;
	htitle = fPePiCo->GetTitle();
	ctitle = htitle + " condition";
	fconPePiCo = MakePolyCon(cname, ctitle, hname, folder);

	hname = fPePePiCo->GetName();
	cname = "c_" + hname;
	htitle = fPePePiCo->GetTitle();
	ctitle = htitle + " condition";
	fconPePePiCo = MakePolyCon(cname, ctitle, hname, folder);

	hname = fPePiPiCo->GetName();
	cname = "c_" + hname;
	htitle = fPePiPiCo->GetTitle();
	ctitle = htitle + " condition";
	fconPePiPiCo = MakePolyCon(cname, ctitle, hname, folder);


	// Load already defined conditions
	// if any of these end up in the Conditions root folder, there's a typo in the name here

	cname = "cXY_" + ion_short + "_sort";
	fconPos_r = MakePolyCon(cname);

	cname = "c_Tof_" + ion_short + "_all";
	fconTof_r = MakeWinCon(cname);

	cname = "cXY_" + electron_short + "_sort";
	fconPos_e = MakePolyCon(cname);

	cname = "c_Tof_" + electron_short + "_all";
	fconTof_e = MakeWinCon(cname);

	cname = "c_Y_" + electron_short + "_sort";
	fconY_e = MakeWinCon(cname);

	std::cout << "**** TRemiUnpackProc: Created conditions" << std::endl;


	// To enable or disable the execution of conpar_online.C
	// use the corresponding switch in the cUnpack.conf
	if (fConfigFile->read("use_conpar_online", false))
	{
		gROOT->ProcessLine(".x conpar_online.C()");
		std::cout << "**** TRemiUnpackProc: conpar_online macro read!" << std::endl;
	}


	// Enable rescaling of Fill 'n' Go histograms
	EnableRescaleFillToHist();

} // end of constructor

/**
 * \brief Destructor
 */
TRemiUnpackProc::~TRemiUnpackProc()
{
	if (fElectron)
		delete fElectron;
	if (fRecoil)
		delete fRecoil;
	if (mUseDelay)
		delete fDelay;
}

/**
 * \brief Check Event Class
 */
Bool_t TRemiUnpackProc::CheckEventClass(TClass* ul)
{
   return ul->InheritsFrom(TRemiUnpackEvent::Class());
}

/**
 * \brief Main event processing routine of the Unpack step
 */
Bool_t TRemiUnpackProc::BuildEvent(TGo4EventElement* dest)
{

	UInt_t j;

	fOutput = dynamic_cast<TRemiUnpackEvent*> (dest);

	// Read input from MBS
//	fInput = (TRemiExtractEvent* ) GetInputEvent();
	fInput = dynamic_cast<TRemiExtractEvent*> (GetInputEvent());

	// Calculate Delay, if used
	if (mUseDelay)
	{
		fDelay->SetInput(fInput);
		fDelay->CalculateDelay();
		mDelay = fDelay->GetDelay();
	}
	
	//-------------------------------//
	// Get information for electrons //
	//-------------------------------//
	
	fElectron->SetInput(fInput);
	if (mUseDelay)
	{
		fElectron->SetDelay(mDelay);
	}

	// Time of flight
	fElectron->CalculateTof();
	fElectron->TestTofCond();
	fElectron->FillTofHistograms();

	// Position
	fElectron->CalculatePosition();
	fElectron->FillPosHistograms();
	
	fElectron->GetHits(mHit_e);

	//--------------------------//
	// Get information for ions //
	//--------------------------//

	fRecoil->SetInput(fInput);
	if (mUseDelay)
	{
		fRecoil->SetDelay(mDelay);
	}
	fRecoil->SetMbIndex(mMbIndex);
	
	// Time of flight
	fRecoil->CalculateTof();
	fRecoil->TestTofCond();
	fRecoil->FillTofHistograms();

	// Position
	fRecoil->CalculatePosition();
	fRecoil->FillPosHistograms();
	
	fRecoil->GetHits(mHit_r);
	

	//-------------------//
	// Create histograms //
	//-------------------//

	// Coincidence histograms
	for (UShort_t i=0;   i<mHit_r.size(); i++)
	for (UShort_t j=i+1; j<mHit_r.size(); j++)
	{
		fPiPiCo->Fill(mHit_r[i].Tof(), mHit_r[j].Tof());

		for (UShort_t k=j+1; k<mHit_r.size(); k++)
		{
			fTriPiCo1->Fill(mHit_r[i].Tof(), mHit_r[j].Tof() + mHit_r[k].Tof());
		}

		for (UShort_t k=j+1; k<mHit_r.size(); k++)
		{
			fTriPiCo2->Fill(mHit_r[i].Tof() + mHit_r[j].Tof(), mHit_r[k].Tof());
		}

		if (fconPos_r->Test(mHit_r[i].X(), mHit_r[i].Y()) && fconPos_r->Test(mHit_r[j].X(), mHit_r[j].Y()))
		{
			fPiPiCo_conPos->Fill(mHit_r[i].Tof(), mHit_r[j].Tof());
		}

		if (fconPiPiCo->Test(mHit_r[i].Tof(), mHit_r[j].Tof()))
		{
			fDelay_conPiPiCo->Fill(mDelay);
			fPos_conPiPiCo->Fill(mHit_r[i].X(), mHit_r[i].Y());
			fPos_conPiPiCo->Fill(mHit_r[j].X(), mHit_r[j].Y());
		}
	}

	// Electron-ion coincidence
	for (UShort_t i=0; i<mHit_r.size(); i++)
	for (UShort_t j=0; j<mHit_e.size(); j++)
	{
		fPePiCo->Fill(mHit_r[i].Tof(), mHit_e[j].Tof());

		if (fconPos_r->Test(mHit_r[i].X(), mHit_r[i].Y()) && fconPos_e->Test(mHit_e[j].X(), mHit_e[j].Y()))
		{
			fPePiCo_conPos->Fill(mHit_r[i].Tof(), mHit_e[j].Tof());
		}

		if (fconPePiCo->Test(mHit_r[i].Tof(), mHit_e[j].Tof()))
		{
			fDelay_conPePiCo->Fill(mDelay);
			fPos_r_conPePiCo->Fill(mHit_r[i].X(), mHit_r[i].Y());
			fPos_e_conPePiCo->Fill(mHit_e[j].X(), mHit_e[j].Y());
			fX_e_Delay_conPePiCo->Fill(mHit_e[j].X(), mDelay);
		}

		if (fconTof_r->Test(mHit_r[i].Tof()) && fconTof_e->Test(mHit_e[j].Tof()))
		{
			fDelay_conTof->Fill(mDelay);
			fPos_r_conTof->Fill(mHit_r[i].X(), mHit_r[i].Y());
			fPos_e_conTof->Fill(mHit_e[j].X(), mHit_e[j].Y());
			fX_e_Delay_conTof->Fill(mHit_e[j].X(), mDelay);
		}

		if(fconTof_e->Test(mHit_e[j].Tof()) && fconY_e->Test(mHit_e[j].Y()))
		{
			fX_e_Delay_conTofconY->Fill(mHit_e[j].X(), mDelay);
		}
	}

	// Electron-Electron-Ion coincidence
	for (UShort_t i=0;   i<mHit_r.size(); i++)
	for (UShort_t j=0;   j<mHit_e.size(); j++)
	for (UShort_t k=j+1; k<mHit_e.size(); k++)
	{
		fPePePiCo->Fill(mHit_r[i].Tof(), mHit_e[j].Tof() + mHit_e[k].Tof());

		if (fconPos_r->Test(mHit_r[i].X(), mHit_r[i].Y()) && fconPos_e->Test(mHit_e[j].X(), mHit_e[j].Y()) && fconPos_e->Test(mHit_e[k].X(), mHit_e[k].Y()))
		{
			fPePePiCo_conPos->Fill(mHit_r[i].Tof(), mHit_e[j].Tof() + mHit_e[k].Tof());
		}

		if (fconTof_r->Test(mHit_r[i].Tof()) && fconTof_e->Test(mHit_e[j].Tof()) && fconTof_e->Test(mHit_e[k].Tof()))
		{
			fPePePiCo_conTof->Fill(mHit_r[i].Tof(), mHit_e[j].Tof() + mHit_e[k].Tof());
		}

		if (fconPePePiCo->Test(mHit_r[i].Tof(), mHit_e[j].Tof() + mHit_e[k].Tof()))
		{
			fDelay_conPePePiCo->Fill(mDelay);
			fPos_r_conPePePiCo->Fill(mHit_r[i].X(), mHit_r[i].Y());
			fPos_e_conPePePiCo->Fill(mHit_e[j].X(), mHit_e[j].Y());
			fPos_e_conPePePiCo->Fill(mHit_e[k].X(), mHit_e[k].Y());
		}
	}

	// Electron-Ion-Ion coincidence
	for (UShort_t i=0;   i<mHit_e.size(); i++)
	for (UShort_t j=0;   j<mHit_r.size(); j++)
	for (UShort_t k=j+1; k<mHit_r.size(); k++)
	{
		fPePiPiCo->Fill(mHit_e[i].Tof(), mHit_r[j].Tof() + mHit_r[k].Tof());

		if (fconPos_e->Test(mHit_e[i].X(), mHit_e[i].Y()) && fconPos_r->Test(mHit_r[j].X(), mHit_r[j].Y()) && fconPos_r->Test(mHit_r[k].X(), mHit_r[k].Y()))
		{
			fPePiPiCo_conPos->Fill(mHit_e[i].Tof(), mHit_r[j].Tof() + mHit_r[k].Tof());
		}

		if (fconTof_e->Test(mHit_e[i].Tof()) && fconTof_r->Test(mHit_r[j].Tof()) && fconTof_r->Test(mHit_r[k].Tof()))
		{
			fPePiPiCo_conTof->Fill(mHit_e[i].Tof(), mHit_r[j].Tof() + mHit_r[k].Tof());
		}

		if (fconPePiPiCo->Test(mHit_e[i].Tof(), mHit_r[j].Tof() + mHit_r[k].Tof()))
		{
			fDelay_conPePiPiCo->Fill(mDelay);
			fPos_e_conPePiPiCo->Fill(mHit_e[i].X(), mHit_e[i].Y());
			fPos_r_conPePiPiCo->Fill(mHit_r[j].X(), mHit_r[j].Y());
			fPos_r_conPePiPiCo->Fill(mHit_r[k].X(), mHit_r[k].Y());
		}
	}


	//-----------------------------------------------------------------//
	// NOTE: Here one can start playing around and fill new histograms //
	//-----------------------------------------------------------------//

	mMbIndex = fInput->fiADC[mAdcChanMbIndex];
	fHistMbIndex->Fill(mMbIndex);

//	EXAMPLE: How to create the same histogram as above via "Fill & Go"
//	FillTo1DHist(mMbIndex, "Test", "Test Test", "unp_hist_mbindex_bins", "unp_hist_mbindex_min", "unp_hist_mbindex_max", "Folder");

	// Fill histograms for all sorted ion hits
	for (j=0; j<mHit_r.size(); j++)
	{
//		if (mMbIndex <10)
//		{
//			FillTo1DHist(mHit_r[j].Tof(), "NewTofMB", "", "unp_ions_hist_tof_bins", "unp_ions_hist_tof_min", "unp_ions_hist_tof_max", "Folder");

//			FillTo2DHist(mHit_r[j].Tof(), mDelay, "NewTofvsDelay", "", "unp_ions_hist_tof_bins", "unp_ions_hist_tof_min", "unp_ions_hist_tof_max","unp_ions_hist_""", "Folder");
//		}
		FillTo1DHist(mMbIndex, "MbIndex_i_sort", "Minibunch index for each sorted ion", "unp_hist_mbindex_bins", "unp_hist_mbindex_min", "unp_hist_mbindex_max", "Unpack/Minibunch");
		FillTo2DHist(mHit_r[j].Tof(), mMbIndex, "MbIndexTof_i_sort", "Minibunch index vs. Tof for sorted ions", "unp_ions_hist_tof_bins", "unp_ions_hist_tof_min", "unp_ions_hist_tof_max", "unp_hist_mbindex_bins", "unp_hist_mbindex_min", "unp_hist_mbindex_max", "Unpack/Minibunch");

/// EXAMPLE: How to include tof and position conditions:
/// if ( fRecoil->TestTofCondition(Hit_r[j]) )
/// if ( fRecoil->TestPosCondition(Hit_r[j]) )
	}

	for (j=0; j<mHit_e.size(); j++)
	{
		FillTo2DHist(mHit_e[j].Tof(), mMbIndex, "MbIndexTof_e_sort", "Minibunch index vs. Tof for sorted electrons", "unp_electrons_hist_tof_bins", "unp_electrons_hist_tof_min", "unp_electrons_hist_tof_max", "unp_hist_mbindex_bins", "unp_hist_mbindex_min", "unp_hist_mbindex_max", "Unpack/Minibunch");
	}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//                  write data to output event                       //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

	Bool_t valid=kTRUE;

	
	// Here set condition for event to be transferred to next step
	// Current setting is: at least one ion hit has to be found in this event 

	if (!mHit_r.empty())
		valid = kTRUE;
	
	if (valid)
	{
		for(UInt_t i=0; i<mHit_e.size(); i++)
		{
			fOutput->AddElectron(mHit_e[i]);
		}
		for(UInt_t j=0; j<mHit_r.size(); j++)
		{
			fOutput->AddIon(mHit_r[j]);
		}
		fOutput->fiDelay = mDelay;
		
		if (mUseIntensity)
			fOutput->fiIntensity = fInput->fiADC[mAdcChanIntensity];
		fOutput->fiMinibunchIndex = mMbIndex;
	}
	
	fOutput->SetValid(valid);
	fOutputValid->Fill(valid);
	
	// Disable rescaling of Fill 'n' Go histograms
	DisableRescaleFillToHist();

  return valid;
}

//----------------------------END OF GO4 SOURCE FILE ---------------------
