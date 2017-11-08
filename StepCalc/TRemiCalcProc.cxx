//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the main processing file for the Calculate step
//----------------------------------------------------------------

#include "TRemiCalcProc.h"

#include "../EventElement/TRemiCalcEvent.h"
#include "../EventElement/TRemiUnpackEvent.h"
#include "../Core/TRemiBasicParameter.h"

#include "../Core/Extern/ConfigFile.h"

#include "TGo4PolyCond.h"
#include "TGo4CondArray.h"

#include "TROOT.h"
#include "Riostream.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TH2.h"
#include "TClass.h"

/**
* \brief Principal constructor, used by TRemiCalcFact
*/
TRemiCalcProc::TRemiCalcProc(const char* name)
		:TGo4EventProcessor(name),
		TRemiIncludeFunctions(CALCCONFIG)
{
	TString basefolder = "Calc", folder, hname, htitle, hxtitle, hytitle, basename, fieldname, paraname, cname, ctitle, binsname, minname, maxname;

	std::cout << "**** TRemiCalcProc: Create" << std::endl;

	//----------------------------------------------------//
	// Load options and constants from configuration file //
	//----------------------------------------------------//

	mNumCoincidenceChannels = fConfigFile->read ("calc_num_coincidences", 1);

	TString parname_global = fConfigFile->read<std::string> ("calc_parname_global", "Par analysis global");

	Int_t HistNumBins = fConfigFile->read("calc_hist_num_bins", 20);
	Int_t HistMbIndexBins = fConfigFile->read("calc_hist_mbindex_bins", 30);

	mUseDelay = fConfigFile->read("calc_use_delay", false);
	mSymmetricDelay = fConfigFile->read ("calc_delay_symmetric", false);
	
	mNumConIntensity = fConfigFile->read("calc_num_intensity_conditions", 5);

	// Constants for ions
	basename = "calc_ions_";

	fieldname = basename + "shortname";
	TString ion_short = fConfigFile->read<TString> (fieldname.Data(), "r");
	fieldname = basename + "longname";
	TString ion_long = fConfigFile->read<std::string> (fieldname.Data(), "recoil ion");

	fieldname = basename + "num_species";
	mNumIonSpecies = fConfigFile->read (fieldname.Data(), 1);

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
	basename = "calc_electrons_";

	fieldname = basename + "num_species";
	mNumElec = fConfigFile->read (fieldname.Data(), 1);

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

	// Constants for Output objects
	UInt_t NumOutputPart = fConfigFile->read("calc_output_num_species", 0);
	UInt_t NumOutputCoinc = fConfigFile->read("calc_output_num_coincidences", 0);


	//-------------------------------//
	// Create input histograms first //
	//-------------------------------//

	folder = basefolder + "/Input";

	hname = "Tof_" + ion_short + "_input";
	htitle = ion_long + " time-of-flight as come from Unpack step";
	fTr_input = MakeH1<TH1I> (hname, htitle, ion_hist_tof_bins, ion_hist_tof_min, ion_hist_tof_max, folder, "Tof [ns]");

	hname = "XY_" + ion_short + "_input";
	htitle = ion_long + " position as come from Unpack step";
	fXYr_input = MakeH2<TH2I> (hname, htitle, ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, ion_hist_xy_bins, ion_hist_xy_min, ion_hist_xy_max, folder, "X [mm]", "Y [mm]");

	hname = "Tof_" + electron_short + "_input";
	htitle = electron_long + " time-of-flight as come from Unpack step";
	fTe_input = MakeH1<TH1I> (hname, htitle, electron_hist_tof_bins, electron_hist_tof_min, electron_hist_tof_max, folder, "Tof [ns]");

	hname = "XY_" + electron_short + "_input";
	htitle = electron_long + " position as come from Unpack step";
	fXYe_input = MakeH2<TH2I> (hname, htitle, electron_hist_xy_bins, electron_hist_xy_min, electron_hist_xy_max, electron_hist_xy_bins, electron_hist_xy_min, electron_hist_xy_max, folder, "X [mm]", "Y [mm]");

	folder = basefolder + "/Input/Intensity";
	
	fIntensity_input = MakeH1<TH1I> ("Intensity_input", "Intensity value from Unpack step", "calc_hist_intensity_bins", "calc_hist_intensity_min", "calc_hist_intensity_max", folder);
	
	fTr_inputConIntensity = std::vector<TH1I*> (mNumConIntensity);
	for (UShort_t i=0; i<mNumConIntensity; i++)
	{
		hname.Form("Tof_" + ion_short + "_ConIntensity%i", i);
		htitle.Form(ion_long + " position as come from Unpack step with intensity condition %i", i);
		fTr_inputConIntensity[i] = MakeH1<TH1I>(hname, htitle, ion_hist_tof_bins, ion_hist_tof_min, ion_hist_tof_max, folder);
	}



	//-------------------------//
	// Create member objects //
	//-------------------------//

	mParticles = std::vector<TRemiParticle> ();

	fMomCalc = std::vector<TRemiCalcProcMomentum> (mNumElec+mNumIonSpecies);
	for (UShort_t i=0; i<mNumElec; i++)
		fMomCalc[i] = TRemiCalcProcMomentum (fConfigFile, electron_long + "s", i, fTe_input);
	for (UShort_t i=mNumElec; i<mNumElec+mNumIonSpecies; i++)
		fMomCalc[i] = TRemiCalcProcMomentum (fConfigFile, ion_long + "s", i-mNumElec, fTr_input);

	mCoincidences = std::vector<TRemiCoincidence> ();
	fCoincidenceHandler = std::vector<TRemiCalcProcCoincidences> (mNumCoincidenceChannels);
	for (UShort_t i=0; i<mNumCoincidenceChannels; i++)
		fCoincidenceHandler[i] = TRemiCalcProcCoincidences(fConfigFile, fMomCalc, i);

	// Output objects
	TString SpeciesShortName, CoincChannelShortName;
	UInt_t SpeciesIdendifier, CoincChannelIdentifier;
	for (UInt_t i=0; i<NumOutputPart; i++)
	{
		fieldname.Form("calc_output_species_%i_shortname", i);
		SpeciesShortName = fConfigFile->read<TString>(fieldname.Data(), "");
		SpeciesIdendifier = SpeciesShortName.Hash();
		std::vector<TRemiCalcProcMomentum>::iterator MomCalcIt;
		for (MomCalcIt=fMomCalc.begin(); MomCalcIt!=fMomCalc.end(); ++MomCalcIt)
			if (MomCalcIt->GetIdentifier() == SpeciesIdendifier)
			{
				fOutputMomCalc.push_back(&(*MomCalcIt));
				break;
			}
		if (MomCalcIt == fMomCalc.end())
			std::cout << "**** " << __FILE__ << ": Could not locate species " << SpeciesShortName << " to use for Calc step output" << std::endl;
	}

	for (UInt_t i=0; i<NumOutputCoinc; i++)
	{
		fieldname.Form("calc_output_coincidence_%i_shortname", i);
		CoincChannelShortName = fConfigFile->read<TString>(fieldname.Data(), "");
		CoincChannelIdentifier = CoincChannelShortName.Hash();
		std::vector<TRemiCalcProcCoincidences>::iterator CoincFinderIt;
		for (CoincFinderIt=fCoincidenceHandler.begin(); CoincFinderIt!=fCoincidenceHandler.end(); ++CoincFinderIt)
			if (CoincFinderIt->GetIdentifier() == CoincChannelIdentifier)
			{
				fOutputCoincHandler.push_back(&(*CoincFinderIt));
				break;
			}
		if (CoincFinderIt == fCoincidenceHandler.end())
			std::cout << "**** " << __FILE__ << ": Could not locate coincidence " << CoincChannelShortName << " to use for Calc step output" << std::endl;
	}


	//-----------------------------------------------------------------//
	// Creation of parameters (check if restored from auto save file): //
	//-----------------------------------------------------------------//

	folder = basefolder;

	// Delay shift parameter
	if (mUseDelay)
		fParaDelayShift = MakeBasicParam("Par Delay Shift", "Shifts the delay value to correct the zero position", folder);


	//-------------------//
	// Create histograms //
	//-------------------//

/* // moved to unpack step
	// Ion-Ion Coincidence histograms
	folder = basefolder + "/P" + ion_short + "P" + ion_short + "Coin";
	hname = "P" + ion_short + "P" + ion_short + "Co";
	htitle = ion_long + " Tof 2 vs. Tof 1";
	fPiPiCo = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, ion_hist_tof_bins/4, ion_hist_tof_min, ion_hist_tof_max, folder, "Tof 1 [ns]", "Tof 2 [ns]");

	hname = "DelayP" + ion_short + "P" + ion_short + "Co";
	htitle = "Delay with " + ion_long + " Tof-Tof coincidence condition";
	fDelayPiPiCo = MakeH1<TH1I>(hname, htitle, "calc_hist_delay_bins", "calc_hist_delay_min", "calc_hist_delay_max", folder, "Delay [fs]");

	// Electron-Ion Coincidence histograms
	folder = basefolder + "/P" + electron_short + "P" + ion_short + "Coin";
	hname = "P" + electron_short + "P" + ion_short + "Co";
	htitle = electron_long + " Tof vs. " + ion_long + " Tof";
	fPePiCo = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins, ion_hist_tof_min, ion_hist_tof_max, electron_hist_tof_bins, electron_hist_tof_min, electron_hist_tof_max, folder, ion_long + "Tof [ns]", electron_long + "Tof 2 [ns]");

	folder = basefolder + "/P" + electron_short+ "P" + electron_short + "P" + ion_short + "Coin";
	hname = "P" + electron_short + "P" + electron_short + "P" + ion_short + "Co";
	htitle = electron_long + " Tof 1 + Tof 2 vs. " + ion_long + " Tof";
	fPePePiCo = MakeH2<TH2I>(hname, htitle, ion_hist_tof_bins, ion_hist_tof_min, ion_hist_tof_max, electron_hist_tof_bins, electron_hist_tof_min, electron_hist_tof_max, folder, ion_long + "Tof [ns]", electron_long + "Tof 1 + Tof 2 [ns]");
*/


	// Count histograms
	folder = basefolder + "/Counts";

	hname = "TotalNum_Part";
	htitle = "Total number of particles per event";
	fNumTotalParticles = MakeH1<TH1I>(hname, htitle, HistNumBins, -0.5, static_cast<Double_t>(HistNumBins)-0.5, folder);

	hname = "TotalNum_Coinc";
	htitle = "Total number of coincidences per event";
	fNumTotalCoincidences = MakeH1<TH1I>(hname, htitle, HistNumBins, -0.5, static_cast<Double_t>(HistNumBins)-0.5, folder);

	// Non-coincident particle histograms
	UShort_t NumParticles = mNumElec + mNumIonSpecies;
	fHistIonDelay = std::vector<TH1I*> (NumParticles);
	fHistIonEnergyDelay = std::vector<TH2I*> (NumParticles);
	fHistIonPxDelay = std::vector<TH2I*> (NumParticles);
	fHistIonPyDelay = std::vector<TH2I*> (NumParticles);
	fHistIonPzDelay = std::vector<TH2I*> (NumParticles);
	fHistIonIntensity = std::vector<TH1I*> (NumParticles);
	fHistIonMinibunch = std::vector<TH1I*> (NumParticles);
	
	
	for (UShort_t i=0; i<NumParticles; i++)
	{
		if (mUseDelay)
		{
			folder = "Calc/Mom/" + fMomCalc[i].GetShortName() + "/Delay/";

			hname = "Delay_" + fMomCalc[i].GetShortName();
			htitle = "Delay dependence for " + fMomCalc[i].GetLongName();
			fHistIonDelay[i] = MakeH1<TH1I>(hname, htitle, "calc_hist_delay_bins", "calc_hist_delay_min", "calc_hist_delay_max", folder, "Delay (fs)");

			hname = "EnergyDelay_" + fMomCalc[i].GetShortName();
			htitle = "Energy vs. Delay for " + fMomCalc[i].GetLongName();
			if (fMomCalc[i].GetMassAu()==1.)
			{
				binsname = "calc_electrons_hist_energy_bins";
				minname = "calc_electrons_hist_energy_min";
				maxname = "calc_electrons_hist_energy_max";
			}
			else
			{
				binsname = "calc_ions_hist_energy_bins";
				minname = "calc_ions_hist_energy_min";
				maxname = "calc_ions_hist_energy_max";
			}
			fHistIonEnergyDelay[i] = MakeH2<TH2I>(hname, htitle, "calc_hist_delay_bins", "calc_hist_delay_min", "calc_hist_delay_max", binsname, minname, maxname, folder, "Delay (fs)", "Energy (eV)");
			
			if (fMomCalc[i].GetMassAu()==1.)
			{
				binsname = "calc_electrons_hist_mom_bins";
				minname = "calc_electrons_hist_mom_min";
				maxname = "calc_electrons_hist_mom_max";
			}
			else
			{
				binsname = "calc_ions_hist_mom_bins";
				minname = "calc_ions_hist_mom_min";
				maxname = "calc_ions_hist_mom_max";
			}
			hname = "PxDelay_" + fMomCalc[i].GetShortName();
			htitle = "Px vs. Delay for " + fMomCalc[i].GetLongName();
			fHistIonPxDelay[i] = MakeH2<TH2I>(hname, htitle, "calc_hist_delay_bins", "calc_hist_delay_min", "calc_hist_delay_max", binsname, minname, maxname, folder, "Delay (fs)", "Px (a.u.)");
			hname = "PyDelay_" + fMomCalc[i].GetShortName();
			htitle = "Py vs. Delay for " + fMomCalc[i].GetLongName();
			fHistIonPyDelay[i] = MakeH2<TH2I>(hname, htitle, "calc_hist_delay_bins", "calc_hist_delay_min", "calc_hist_delay_max", binsname, minname, maxname, folder, "Delay (fs)", "Py (a.u.)");
			hname = "PzDelay_" + fMomCalc[i].GetShortName();
			htitle = "Pz vs. Delay for " + fMomCalc[i].GetLongName();
			fHistIonPzDelay[i] = MakeH2<TH2I>(hname, htitle, "calc_hist_delay_bins", "calc_hist_delay_min", "calc_hist_delay_max", binsname, minname, maxname, folder, "Delay (fs)", "Pz (a.u.)");
		}

		folder = "Calc/Mom/" + fMomCalc[i].GetShortName() + "/";

		hname = "Intensity_" + fMomCalc[i].GetShortName();
		htitle = "Intensity dependence for " + fMomCalc[i].GetLongName();
		fHistIonIntensity[i] = MakeH1<TH1I>(hname, htitle, "calc_hist_intensity_bins", "calc_hist_intensity_min", "calc_hist_intensity_max", folder, "Intensity (arb. units)");

		hname = "MbIndex_" + fMomCalc[i].GetShortName();
		htitle = "Minibunch index dependence for " + fMomCalc[i].GetLongName();
		fHistIonMinibunch[i] = MakeH1<TH1I>(hname, htitle, HistMbIndexBins, -0.5, static_cast<Float_t>(HistMbIndexBins) - 0.5, folder, "Minibunch index");
	}

	// Coincidence channel histograms
	fHistCoinDelay = std::vector<TH1I*> (mNumCoincidenceChannels);
	fHistCoinKerDelay = std::vector<TH2I*> (mNumCoincidenceChannels);
	fHistCoinIntensity = std::vector<TH1I*> (mNumCoincidenceChannels);
	fHistCoinMinibunch = std::vector<TH1I*> (mNumCoincidenceChannels);
	fHistCoinEnergyDelay = std::vector< std::vector <TH2I*> > (mNumCoincidenceChannels);
	fHistCoinAngleDelay = std::vector< std::vector<std::vector <TH2I*> > > (mNumCoincidenceChannels);
	for (UInt_t i=0; i<mNumCoincidenceChannels; i++)
	{
		if (mUseDelay)
		{
			folder = "Calc/Coinc/" + fCoincidenceHandler[i].GetShortName() + "/Delay/";

			hname = "Delay_" + fCoincidenceHandler[i].GetShortName();
			htitle = "Delay dependence for " + fCoincidenceHandler[i].GetLongName();
			fHistCoinDelay[i] = MakeH1<TH1I>(hname, htitle, "calc_hist_delay_bins", "calc_hist_delay_min", "calc_hist_delay_max", folder, "Delay (fs)");

			hname = "KerDelay_" + fCoincidenceHandler[i].GetShortName();
			htitle = "Kinetic energy Release vs. Delay for " + fCoincidenceHandler[i].GetLongName();
			fHistCoinKerDelay[i] = MakeH2<TH2I>(hname, htitle, "calc_hist_delay_bins", "calc_hist_delay_min", "calc_hist_delay_max", "calc_hist_esum_bins", "calc_hist_esum_min", "calc_hist_esum_max", folder, "Delay (fs)", "KER (eV)");
			
			fHistCoinEnergyDelay[i] = std::vector<TH2I*> (fCoincidenceHandler[i].GetNumParticles());
			fHistCoinAngleDelay[i] = std::vector< std::vector<TH2I*> > (fCoincidenceHandler[i].GetNumParticles());
			for (UInt_t j=0; j<fCoincidenceHandler[i].GetNumParticles(); j++)
			{
				folder = "Calc/Coinc/" + fCoincidenceHandler[i].GetShortName() + "/" + fCoincidenceHandler[i].GetParticleName(j) + "/Delay/";
				
				if (fCoincidenceHandler[i].GetMomentumCalculator(j)->GetMassAu()==1.)
				{
					binsname = "calc_electrons_hist_energy_bins";
					minname = "calc_electrons_hist_energy_min";
					maxname = "calc_electrons_hist_energy_max";
				}
				else
				{
					binsname = "calc_ions_hist_energy_bins";
					minname = "calc_ions_hist_energy_min";
					maxname = "calc_ions_hist_energy_max";
				}
				hname = "EnergyDelay_" + fCoincidenceHandler[i].GetParticleName(j) + "_" + fCoincidenceHandler[i].GetShortName();
				htitle = "Energy vs. Delay for " + fCoincidenceHandler[i].GetParticleName(j) + " in " + fCoincidenceHandler[i].GetShortName() + " coincidence";
				fHistCoinEnergyDelay[i][j] = MakeH2<TH2I>(hname, htitle, "calc_hist_delay_bins", "calc_hist_delay_min", "calc_hist_delay_max", binsname, minname, maxname, folder, "Delay (fs)", "Energy (eV)" );
				
				fHistCoinAngleDelay[i][j] = std::vector<TH2I*> (fCoincidenceHandler[i].GetNumParticles());
				for (UInt_t k=0; k<fCoincidenceHandler[i].GetNumParticles(); k++)
				{
					if(j!=k)
					{
						hname = "AngleDelay_" + fCoincidenceHandler[i].GetParticleName(j) + fCoincidenceHandler[i].GetParticleName(k) +"_" + fCoincidenceHandler[i].GetShortName();
						htitle = "Angle between " + fCoincidenceHandler[i].GetParticleName(j) + fCoincidenceHandler[i].GetParticleName(k) + " in " + fCoincidenceHandler[i].GetShortName() + " coincidence vs. Delay";
						fHistCoinAngleDelay[i][j][k] = MakeH2<TH2I>(hname, htitle, "calc_hist_delay_bins", "calc_hist_delay_min", "calc_hist_delay_max", "calc_ions_hist_theta_bins", "calc_ions_hist_theta_min", "calc_ions_hist_theta_max", folder, "Minibunch index", "Angle (deg)" );
					}
				}
				
			}
			
			
		}

		folder = "Calc/Coinc/" + fCoincidenceHandler[i].GetShortName();

		hname = "Intensity_" + fCoincidenceHandler[i].GetShortName();
		htitle = "Intensity dependence for " + fCoincidenceHandler[i].GetLongName();
		fHistCoinIntensity[i] = MakeH1<TH1I>(hname, htitle, "calc_hist_intensity_bins", "calc_hist_intensity_min", "calc_hist_intensity_max", folder, "Intensity (arb. units)");

		hname = "MbIndex_" + fCoincidenceHandler[i].GetShortName();
		htitle = "Minibunch index dependence for " + fCoincidenceHandler[i].GetLongName();
		fHistCoinMinibunch[i] = MakeH1<TH1I>(hname, htitle, HistMbIndexBins, -0.5, static_cast<Float_t>(HistMbIndexBins) - 0.5, folder, "Delay (fs)");
	}

	std::cout << "**** TRemiCalcProc: Created histograms" << std::endl;


	//------------------------//
	// Creation of conditions //
	//------------------------//

/* // moved to unpack step
	folder = basefolder + "/P" + ion_short + "P" + ion_short + "Coin";
	cname = "c_P" + ion_short + "P" + ion_short + "Co";
	ctitle = ion_long + " Tof-Tof-Coincidence condition";
	hname = "P" + ion_short + "P" + ion_short + "Co";
	fconPiPiCo = MakePolyCon(cname, ctitle, hname, folder);
*/

	folder = basefolder;
	fConArIntensity = MakeWinConArray("c_Intensity", mNumConIntensity, "Intensity_input", folder);

	std::cout << "**** TRemiCalcProc: Created conditions" << std::endl;


	// To enable or disable the execution of conpar_online.C
	// use the corresponding switch in the cCalc.conf
	if (fConfigFile->read("use_conpar_online", false))
	{
		gROOT->ProcessLine(".x conpar_online.C()");
		std::cout << "**** TRemiCalcProc: conpar_online macro read!" << std::endl;
	}


	// Enable rescaling of Fill 'n' Go histograms
	EnableRescaleFillToHist();

}
//***********************************************************
TRemiCalcProc::TRemiCalcProc()
		: TGo4EventProcessor("RemiCalcProc"),fInput(0)
{
}
//***********************************************************
TRemiCalcProc::~TRemiCalcProc()
{
}

/**
* \brief Check Event Class
*/
Bool_t TRemiCalcProc::CheckEventClass(TClass* cl)
{
return cl->InheritsFrom(TRemiCalcEvent::Class());
}

//******************************//
// Start of the Event processor //
//------------------------------//

Bool_t TRemiCalcProc::BuildEvent(TGo4EventElement* dest)
{
//	fInput  = (TRemiUnpackEvent*) GetInputEvent();
	fInput  = dynamic_cast<TRemiUnpackEvent*> (GetInputEvent());

	fOutput = dynamic_cast<TRemiCalcEvent*> (dest);

	Bool_t valid = 0; // Have a valid output event?

	if (fInput->IsValid())
	{
		UShort_t i;
		mParticles.clear();
		mCoincidences.clear();

		//-----------------//
		// Read input data //
		//-----------------//

		mNumElecHits = fInput->fiElectronHits.size();
		mNumIonHits = fInput->fiIonHits.size();

		// make sure enough memory is allocated for particles and coincidences
		if (static_cast<UShort_t>(mNumIonHits*mNumIonSpecies*2 + mNumElecHits*mNumElec) > mParticles.capacity())
			mParticles.reserve(mNumIonHits*mNumIonSpecies*2 + mNumElecHits*mNumElec);

		if (static_cast<UShort_t>(mNumCoincidenceChannels * (mNumElecHits + mNumIonHits)) > mCoincidences.capacity())
			mCoincidences.reserve(mNumCoincidenceChannels * (mNumElecHits + mNumIonHits));

		// Correct the delay value and symmetrize, if necessary
		if (mUseDelay)
		{
			mDelay = fInput->fiDelay - fParaDelayShift->Value;
			if (mSymmetricDelay)
				mDelay = TMath::Abs(mDelay);
		}
		else
			mDelay = 0;

		// Input histograms
		for (i=0; i<mNumIonHits; i++)
		{
			fTr_input->Fill(fInput->fiIonHits[i].Tof());
			fXYr_input->Fill(fInput->fiIonHits[i].X(), fInput->fiIonHits[i].Y());
		}

		for (i=0; i<mNumElecHits; i++)
		{
			fTe_input->Fill(fInput->fiElectronHits[i].Tof());
			fXYe_input->Fill(fInput->fiElectronHits[i].X(), fInput->fiElectronHits[i].Y());
		}
		fIntensity_input->Fill(fInput->fiIntensity);

/* // moved to unpack step
		// Coincidence histograms
		for (i=0; i<mNumIonHits; i++)
			for (UShort_t j=i+1; j<mNumIonHits; j++)
		{
			fPiPiCo->Fill(fInput->fiIonHits[i].Tof(), fInput->fiIonHits[j].Tof());
			if (fconPiPiCo->Test(fInput->fiIonHits[i].Tof(), fInput->fiIonHits[j].Tof()))
			{
				fDelayPiPiCo->Fill(mDelay);
			}
		}

		// Electron-ion coincidence
		for (i=0; i<mNumIonHits; i++)
			for (UShort_t j=0; j<mNumElecHits; j++)
			{
				fPePiCo->Fill(fInput->fiIonHits[i].Tof(), fInput->fiElectronHits[j].Tof());
			}

		// Electron-Electron-ion coincidence
		for (i=0; i<mNumIonHits; i++)
			for (UShort_t j=0; j<mNumElecHits; j++)
			{
				for (UShort_t k=j+1; k<mNumElecHits; k++)
				{
					if (fInput->fiElectronHits[j].Tof() > 30 && fInput->fiElectronHits[j].Tof() < 90)
					{
						if(fInput->fiElectronHits[k].Tof() > 30 && fInput->fiElectronHits[k].Tof() < 90)
						{	
							fPePePiCo->Fill(fInput->fiIonHits[i].Tof(), fInput->fiElectronHits[j].Tof() + fInput->fiElectronHits[k].Tof());
						}
					}
				}	
				
			}
*/

		// Intensity conditions
		for (i=0; i<mNumConIntensity; i++)
			if ( fConArIntensity->At(i)->Test(fInput->fiIntensity) )
				for (UShort_t j=0; j<mNumIonHits; j++)
					fTr_inputConIntensity[i]->Fill(fInput->fiIonHits[j].Tof());


		//------------------------//
		// calculation of momenta //
		//------------------------//

		for (i=0; i<mNumElec; i++)
		{
			fMomCalc[i].Clear();
			fMomCalc[i].CalculateMomentum(mParticles, fInput->fiElectronHits);
			fMomCalc[i].FillMomentumHistograms();
			
			for (std::vector<TRemiParticle*>::iterator ThisParticle = fMomCalc[i].GetCalculatedParticles().begin(), End = fMomCalc[i].GetCalculatedParticles().end(); ThisParticle != End; ++ThisParticle)
			{
				
				// Fill spectra with Delay
				if (mUseDelay)
				{
					fHistIonDelay[i]->Fill(mDelay);
					fHistIonEnergyDelay[i]->Fill(mDelay, (*ThisParticle)->GetEnergy());
					fHistIonPxDelay[i]->Fill(mDelay, (*ThisParticle)->Mom().X());
					fHistIonPyDelay[i]->Fill(mDelay, (*ThisParticle)->Mom().Y());
					fHistIonPzDelay[i]->Fill(mDelay, (*ThisParticle)->Mom().Z());
				}
				
			}
			
		}

		for (i=mNumElec; i<mNumElec+mNumIonSpecies; i++)
		{
			fMomCalc[i].Clear();
			fMomCalc[i].CalculateMomentum(mParticles, fInput->fiIonHits);
			fMomCalc[i].FillMomentumHistograms();

			// Here one can loop through all momenta calculated for a given species in order to fill histograms
			for (std::vector<TRemiParticle*>::iterator ThisParticle = fMomCalc[i].GetCalculatedParticles().begin(), End = fMomCalc[i].GetCalculatedParticles().end(); ThisParticle != End; ++ThisParticle)
			{

				
				// Fill spectra with Delay
				if (mUseDelay)
				{
					fHistIonDelay[i]->Fill(mDelay);
					fHistIonEnergyDelay[i]->Fill(mDelay, (*ThisParticle)->GetEnergy());
					fHistIonPxDelay[i]->Fill(mDelay, (*ThisParticle)->Mom().X());
					fHistIonPyDelay[i]->Fill(mDelay, (*ThisParticle)->Mom().Y());
					fHistIonPzDelay[i]->Fill(mDelay, (*ThisParticle)->Mom().Z());
				}
				fHistIonIntensity[i]->Fill(fInput->fiIntensity);
				fHistIonMinibunch[i]->Fill(fInput->fiMinibunchIndex);
			}
		}


		//-------------------------------------------//
		// spectra with coincidences and sum spectra //
		//-------------------------------------------//
		for (i=0; i<mNumCoincidenceChannels; i++)
		{
			fCoincidenceHandler[i].Clear();
			fCoincidenceHandler[i].FindCoincidences(mCoincidences);
			fCoincidenceHandler[i].FillCoincidenceHistograms();

			// Showing two ways to loop through coincidences to create spectra
			// 1st
/*			for (UInt_t j=0; j<fCoincidenceHandler[i].GetFoundCoincidences().size(); j++)
			{
				FillTo1DHist(fCoincidenceHandler[i].GetFoundCoincidences()[j]->GetKer(), "Example_" + fCoincidenceHandler[i].GetShortName() , "This is a very dirty example to plot KER of " + fCoincidenceHandler[i].GetLongName(), "calc_hist_esum_bins", "calc_hist_esum_min", "calc_hist_esum_max", "Calc/Examples");
			}*/
			// 2nd
/*			for (std::vector<TRemiCoincidence*>::iterator ThisCoincidence = fCoincidenceHandler[i].GetFoundCoincidences().begin(); ThisCoincidence != fCoincidenceHandler[i].GetFoundCoincidences().end(); ++ThisCoincidence)
			{
				FillTo1DHist((*ThisCoincidence)->GetKer(), "Example2_" + fCoincidenceHandler[i].GetShortName() , "This is a very dirty example to plot KER of " + fCoincidenceHandler[i].GetLongName(), "calc_hist_esum_bins", "calc_hist_esum_min", "calc_hist_esum_max", "Calc/Examples");
			}*/
			for (std::vector<TRemiCoincidence*>::iterator ThisCoincidence = fCoincidenceHandler[i].GetFoundCoincidences().begin(); ThisCoincidence != fCoincidenceHandler[i].GetFoundCoincidences().end(); ++ThisCoincidence)
			{
				if (mUseDelay)
				{
					fHistCoinDelay[i]->Fill(mDelay);
					fHistCoinKerDelay[i]->Fill(mDelay, (*ThisCoincidence)->GetKer());
					for (UInt_t j=0; j<(*ThisCoincidence)->GetSize(); j++)
					{
						fHistCoinEnergyDelay[i][j]->Fill(mDelay, (*ThisCoincidence)->GetParticle(j).GetEnergy());
					}
					for (UInt_t j=0; j<(*ThisCoincidence)->GetSize(); j++)
					{
						for (UInt_t k=0; k<(*ThisCoincidence)->GetSize(); k++)
						{
							if(j!=k)
							{
								fHistCoinAngleDelay[i][j][k]->Fill( mDelay, (*ThisCoincidence)->GetParticle(j).Mom().Angle( (*ThisCoincidence)->GetParticle(k).Mom() ) );
							}
						}
					}
				}
				fHistCoinIntensity[i]->Fill(fInput->fiIntensity);
				fHistCoinMinibunch[i]->Fill(fInput->fiMinibunchIndex);
			}
		}

		// Fill number spectra
		fNumTotalParticles->Fill(mParticles.size());
		fNumTotalCoincidences->Fill(mCoincidences.size());


		//----------------------------//
		// Write data to output event //
		//----------------------------//
		UInt_t OutputCounter = 0;

		// Loop through all momentum species selected for output
		for (UInt_t i=0; i<fOutputMomCalc.size(); i++)
		{
			// Now loop through all calculated particles for the given species
			for (UInt_t j=0; j<fOutputMomCalc[i]->GetCalculatedParticles().size(); j++)
			{
				fOutput->AddParticle(*(fOutputMomCalc[i]->GetCalculatedParticles()[j]));
				OutputCounter++;
			}
		}

		// Loop through all coincidence channels selected for output
		for (UInt_t i=0; i<fOutputCoincHandler.size(); i++)
		{
			// Now loop through all actual events for the gicen channel
			for (UInt_t j=0; j<fOutputCoincHandler[i]->GetFoundCoincidences().size(); j++)
			{
				fOutput->AddCoincidence(*(fOutputCoincHandler[i]->GetFoundCoincidences()[j]));
				OutputCounter++;
			}
		}

		if (OutputCounter)
			valid = kTRUE;       // events are not stored until kTRUE is set

	} //  if (inputevt->IsValid())

	// Disable rescaling of Fill 'n' Go histograms
	DisableRescaleFillToHist();

	return valid;
} // Calc Event Processor

//----------------------------END OF GO4 SOURCE FILE ---------------------



