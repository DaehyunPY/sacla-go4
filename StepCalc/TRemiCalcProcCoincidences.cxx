//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class to process a coincidence channel
//----------------------------------------------------------------

#include "TRemiCalcProcCoincidences.h"

#include "TRemiCalcProcMomentum.h"
//#include "Parameters/TRemiMomentumParameter.h"
#include "../Core/Point/TRemiCoincidence.h"
#include "../Core/Point/TRemiParticle.h"

#include "TGo4WinCond.h"

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"

/** 
 * \brief Default constructor. Not to be used!
 */
TRemiCalcProcCoincidences::TRemiCalcProcCoincidences()
{
}

/**
 * \brief Principal constructor using config file input
 * \param CalcConf ConfigFile object of the Analysis step
 * \param Index (Optional, default = 0) No. to identify a species within a group of particles
 */
TRemiCalcProcCoincidences::TRemiCalcProcCoincidences(ConfigFile* CalcConf, std::vector<TRemiCalcProcMomentum> &MomentumCalculators, Short_t Index): TRemiIncludeFunctions(CalcConf)
{
	TString fieldname, basename="calc_", coincbasename, hname, htitle, folder, basefolder = "Calc", cname, ctitle, hxtitle, hytitle, hztitle, pname, ptitle, SumBinsName, SumMaxName, SumMinName, MomBinsName, MomMinName, MomMaxName, EnergyBinsName, EnergyMinName, EnergyMaxName, Particle;
	
	//------------------------------------------//
	// Read information from configuration file //
	//------------------------------------------//
	
	coincbasename.Form(basename + "coinc_%i_", Index);
	
	fieldname =  coincbasename + "shortname";
	mShortName = fConfigFile->read<std::string> (fieldname.Data(), TString::Format("Coin%i", Index).Data());
	fieldname =  coincbasename + "longname";
	mLongName = fConfigFile->read<std::string> (fieldname.Data(), TString::Format("Coincidence %i", Index).Data());
	
	// Set identifier for this coincidence handler
	mIdentifier = mShortName.Hash();
	
	fieldname = coincbasename + "num_particles";
	mNumParticles = fConfigFile->read (fieldname.Data(), 0);
	
	mHaveSameSpecies = kFALSE;
	
	mParticleNames = std::vector<TString> (mNumParticles);
	mParticleIdentifiers = std::vector<UInt_t> (mNumParticles);
	for (UInt_t i=0; i<mNumParticles; i++)
	{
		fieldname.Form(coincbasename + "particle_%i_shortname", i);
		mParticleNames[i] = fConfigFile->read<TString>(fieldname.Data(), "");
		mParticleIdentifiers[i] = mParticleNames[i].Hash();
		
		// Check for repetition of one or more species in the definition of the coincidence handler
		for (UInt_t j=0; j<i; j++)
			if (mParticleIdentifiers[i] == mParticleIdentifiers[j])
				mHaveSameSpecies = kTRUE;
	}
	
	fieldname = basename + "hist_num_bins";
	Int_t HistNumBins = fConfigFile->read(fieldname.Data(), 50);

	SumBinsName = basename + "hist_psum_bins";
	Int_t HistSumBins = fConfigFile->read(SumBinsName.Data(), 100);
	SumMinName = basename + "hist_psum_min";
	Float_t HistSumMin = fConfigFile->read(SumMinName.Data(), -50.);
	SumMaxName = basename + "hist_psum_max";
	Float_t HistSumMax = fConfigFile->read(SumMaxName.Data(), 50.);
	
	SumBinsName = basename + "hist_esum_bins";
	Int_t HistESumBins = fConfigFile->read(SumBinsName.Data(), 100);
	SumMinName = basename + "hist_esum_min";
	Float_t HistESumMin = fConfigFile->read(SumMinName.Data(), -50.);
	SumMaxName = basename + "hist_esum_max";
	Float_t HistESumMax = fConfigFile->read(SumMaxName.Data(), 50.);
	
	//-----------------------//
	// Create member objects //
	//-----------------------//
	
	fMomentumCalculators = std::vector<TRemiCalcProcMomentum*> ();
	std::vector<TRemiCalcProcMomentum>::iterator ThisMomCalc;
	for (std::vector<UInt_t>::iterator ThisIdentifier = mParticleIdentifiers.begin(); ThisIdentifier != mParticleIdentifiers.end(); ++ThisIdentifier)
	{
		for (ThisMomCalc = MomentumCalculators.begin(); ThisMomCalc != MomentumCalculators.end(); ++ThisMomCalc)
		{
			if (ThisMomCalc->GetIdentifier() == *ThisIdentifier) 
			{
				fMomentumCalculators.push_back(&(*ThisMomCalc));
				break;
			}
		}
		if (ThisMomCalc == MomentumCalculators.end())
		{
			std::cout << "**** " << __FILE__ << ": Did not find a momentum calculator for coincidence " << mLongName << std::endl;
			mNumParticles -= 1;
		}
	}
	
	fPartIts = std::vector< std::vector<TRemiParticle*>::iterator > (mNumParticles);
	fPartBegins = std::vector< std::vector<TRemiParticle*>::iterator > (mNumParticles);
	fPartEnds = std::vector< std::vector<TRemiParticle*>::iterator > (mNumParticles);
	mFoundCoincidences = std::vector<TRemiCoincidence*> ();
	
	//-------------------//
	// Create histograms //
	//-------------------//

	folder = basefolder + "/Coinc/" + mShortName + "/sum";
	
	hname = "PxSum_" + mShortName;
	htitle = "X momentum sum " + mLongName;
	fHistPxSum = MakeH1<TH1I>(hname, htitle, HistSumBins, HistSumMin, HistSumMax, folder, "PxSum (a.u.)");
	
	hname = "PySum_" + mShortName;
	htitle = "Y momentum sum " + mLongName;
	fHistPySum = MakeH1<TH1I>(hname, htitle, HistSumBins, HistSumMin, HistSumMax, folder, "PySum (a.u.)");
	
	hname = "PzSum_" + mShortName;
	htitle = "Z momentum sum " + mLongName;
	fHistPzSum = MakeH1<TH1I>(hname, htitle, HistSumBins, HistSumMin, HistSumMax, folder, "PzSum (a.u.)");
	
	hname = "KER_" + mShortName;
	htitle = "Kin. energy release for " + mLongName + " coincidence";
	fHistKer = MakeH1<TH1I>(hname, htitle, HistESumBins, HistESumMin, HistESumMax, folder, "Energy (eV)");
	
	// Histograms for each individual particle
	fHistPx = std::vector<TH1I*> (mNumParticles);
	fHistPy = std::vector<TH1I*> (mNumParticles);
	fHistPz = std::vector<TH1I*> (mNumParticles);
	fHistPxy = std::vector<TH2I*> (mNumParticles);
	fHistPxz = std::vector<TH2I*> (mNumParticles);
	fHistPyz = std:: vector<TH2I*> (mNumParticles);
	fHistPxyz = std:: vector<TH3I*> (mNumParticles);
	fHistPtranslong = std:: vector<TH2I*> (mNumParticles);
	fHistRadTof = std:: vector<TH2I*> (mNumParticles);
	fHistXTof = std:: vector<TH2I*> (mNumParticles);
	fHistYTof = std:: vector<TH2I*> (mNumParticles);
	fHistEnergy = std::vector<TH1I*> (mNumParticles);
	fHistKerVsEnergy = std::vector<TH2I*> (mNumParticles);
	fHistPhi = std::vector<TH1I*> (mNumParticles);
	fHistTheta = std::vector<TH1I*> (mNumParticles);
	fHistShperical = std::vector<TH2I*> (mNumParticles);
	fHistPxSumVsPx = std::vector<TH2I*> (mNumParticles);
	fHistPySumVsPy = std::vector<TH2I*> (mNumParticles);
	fHistPzSumVsPz = std::vector<TH2I*> (mNumParticles);
	fHistPxVsOther = std::vector<TH2I*> (mNumParticles);
	fHistPyVsOther = std::vector<TH2I*> (mNumParticles);
	fHistPzVsOther = std::vector<TH2I*> (mNumParticles);
	fHistDeltaPhi = std::vector<TH2I*> (mNumParticles);
	fHistKerVsPx = std::vector<TH2I*> (mNumParticles);
	fHistKerVsPy = std::vector<TH2I*> (mNumParticles);
	fHistKerVsPz = std::vector<TH2I*> (mNumParticles);
	fHistMutualAngle = std::vector< std::vector<TH1I*> > (mNumParticles);
	fHistEnergy2Particle = std::vector< std::vector<TH2I*> > (mNumParticles);
	for (UInt_t i=0; i<mNumParticles; i++)
	{
		// decide if we need spectra for electrons or ions
		if (fMomentumCalculators[i]->GetMassAu() == 1.)
			Particle = "electrons";
		else 
			Particle = "ions";
		
		fieldname = basename + Particle + "_hist_phi_bins";
		Int_t HistPhiBins = fConfigFile->read(fieldname.Data(), 360);
		fieldname = basename + Particle + "_hist_theta_bins";
		Int_t HistThetaBins = fConfigFile->read(fieldname.Data(), 180);
		
		MomBinsName = basename + Particle + "_hist_mom_bins";
		Int_t HistMomBins = fConfigFile->read(MomBinsName.Data(), 100);
		MomMinName = basename + Particle + "_hist_mom_min";
		Float_t HistMomMin = fConfigFile->read(MomMinName.Data(), -50.);
		MomMaxName = basename + Particle + "_hist_mom_max";
		Float_t HistMomMax = fConfigFile->read(MomMaxName.Data(), 50.);
		
		EnergyBinsName = basename + Particle + "_hist_energy_bins";
		Int_t HistEnergyBins = fConfigFile->read(EnergyBinsName.Data(), 100);
		EnergyMinName = basename + Particle + "_hist_energy_min";
		Float_t HistEnergyMin = fConfigFile->read(EnergyMinName.Data(), 0.);
		EnergyMaxName = basename + Particle + "_hist_energy_max";
		Float_t HistEnergyMax = fConfigFile->read(EnergyMaxName.Data(), 100.);
		
		fieldname = basename + Particle + "_hist_tof_min";
		Double_t HistTofMin = CalcConf->read (fieldname.Data(), 0.);
		fieldname = basename + Particle + "_hist_tof_max";
		Double_t HistTofMax = CalcConf->read (fieldname.Data(), 1.);
		fieldname = basename + Particle + "_hist_tof_bins";
		Int_t HistTofBins = CalcConf->read (fieldname.Data(), 100);
		
		fieldname = basename + Particle + "_hist_xy_min";
		Double_t HistXyMin = CalcConf->read (fieldname.Data(), -1.);
		fieldname = basename + Particle + "_hist_xy_max";
		Double_t HistXyMax = CalcConf->read (fieldname.Data(), 1.);
		fieldname = basename + Particle + "_hist_xy_bins";
		Int_t HistXyBins = CalcConf->read (fieldname.Data(), 100);
		
		folder = basefolder + "/Coinc/" + mShortName + "/" + mParticleNames[i] + "/";
		
		// Momenta of each particle
		hname = "Px_" + mParticleNames[i] + "_" + mShortName;
		htitle = "X momentum " + mParticleNames[i] + " for " + mLongName + " coincidence";
		hxtitle = "Px " + mParticleNames[i] + " (a.u.)";
		fHistPx[i] = MakeH1<TH1I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, folder, hxtitle);
		
		hname = "Py_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Y momentum " + mParticleNames[i] + " for " + mLongName + " coincidence";
		hxtitle = "Py " + mParticleNames[i] + " (a.u.)";
		fHistPy[i] = MakeH1<TH1I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, folder, hxtitle);
		
		hname = "Pz_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Z momentum " + mParticleNames[i] + " for " + mLongName + " coincidence";
		hxtitle = "Pz " + mParticleNames[i] + " (a.u.)";
		fHistPz[i] = MakeH1<TH1I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, folder, hxtitle);
		
		hname = "Pyx_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Y vs. X momentum " + mParticleNames[i] + " for " + mLongName + " coincidence";
		hxtitle = "Px " + mParticleNames[i] + " (a.u.)";
		hytitle = "Py " + mParticleNames[i] + " (a.u.)";
		fHistPxy[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistMomBins, HistMomMin, HistMomMax, folder, hxtitle, hytitle);
		
		hname = "Pxz_" + mParticleNames[i] + "_" + mShortName;
		htitle = "X vs. Z momentum " + mParticleNames[i] + " for " + mLongName + " coincidence";
		hxtitle = "Pz " + mParticleNames[i] + " (a.u.)";
		hytitle = "Px " + mParticleNames[i] + " (a.u.)";
		fHistPxz[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistMomBins, HistMomMin, HistMomMax, folder, hxtitle, hytitle);
		
		hname = "Pyz_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Y vs. Z momentum " + mParticleNames[i] + " for " + mLongName + " coincidence";
		hxtitle = "Pz " + mParticleNames[i] + " (a.u.)";
		hytitle = "Py " + mParticleNames[i] + " (a.u.)";
		fHistPyz[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistMomBins, HistMomMin, HistMomMax, folder, hxtitle, hytitle);
		
		hname = "Pxyz_" + mParticleNames[i] + "_" + mShortName;
		htitle = "X vs. Y vs. Z momentum " + mParticleNames[i] + " for " + mLongName + " coincidence";
		hxtitle = "Px " + mParticleNames[i] + " (a.u.)";
		hytitle = "Py " + mParticleNames[i] + " (a.u.)";
		hztitle = "Pz " + mParticleNames[i] + " (a.u.)";
		fHistPxyz[i] = MakeH3<TH3I>(hname, htitle, HistMomBins/4, HistMomMin, HistMomMax, HistMomBins/4, HistMomMin, HistMomMax, HistMomBins/4, HistMomMin, HistMomMax, folder, hxtitle, hytitle, hztitle);
		
		hname = "Ptransz_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Transverse vs. Z momentum " + mParticleNames[i] + " for " + mLongName + " coincidence";
		hxtitle = "Pz " + mParticleNames[i] + " (a.u.)";
		hytitle = "Pr " + mParticleNames[i] + " (a.u.)";
		fHistPtranslong[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistMomBins/2, 0., HistMomMax, folder, hxtitle, hytitle);
		
		hname = "RadTof_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Radius vs. Tof " + mParticleNames[i] + " for " + mLongName + " coincidence";
		hxtitle = "Tof_" + mParticleNames[i] + " (ns)";
		hytitle = "Radius " + mParticleNames[i] + " (mm)";
		fHistRadTof[i] = MakeH2<TH2I>(hname, htitle, HistTofBins, HistTofMin, HistTofMax, HistXyBins/2, 0., HistXyMax, folder, hxtitle, hytitle);
		
		hname = "X_Tof_" + mParticleNames[i] + "_" + mShortName;
		htitle = "X vs. Tof " + mParticleNames[i] + " for " + mLongName + " coincidence";
		hxtitle = "Tof_" + mParticleNames[i] + " (ns)";
		hytitle = "X " + mParticleNames[i] + " (mm)";
		fHistXTof[i] = MakeH2<TH2I>(hname, htitle, HistTofBins, HistTofMin, HistTofMax, HistXyBins, HistXyMin, HistXyMax, folder, hxtitle, hytitle);
		
		hname = "Y_Tof_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Y vs. Tof " + mParticleNames[i] + " for " + mLongName + " coincidence";
		hxtitle = "Tof_" + mParticleNames[i] + " (ns)";
		hytitle = "Y " + mParticleNames[i] + " (mm)";
		fHistYTof[i] = MakeH2<TH2I>(hname, htitle, HistTofBins, HistTofMin, HistTofMax, HistXyBins, HistXyMin, HistXyMax, folder, hxtitle, hytitle);
		
		// Angles and energy
		folder = basefolder + "/Coinc/" + mShortName + "/" + mParticleNames[i] + "/energy";
		
		hname = "Energy_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Kin. energy " + mParticleNames[i] + " for " + mLongName + " coincidence";
		fHistEnergy[i] = MakeH1<TH1I>(hname, htitle, HistEnergyBins, HistEnergyMin, HistEnergyMax, folder, "Energy (eV)");
		
		hname = "KerVsEnergy_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Kinetic energy relase vs. " + mParticleNames[i] + " Kin. energy for " + mLongName + " coincidence";
		hxtitle = "Energy " + mParticleNames[i] + " (eV)";
		fHistKerVsEnergy[i] = MakeH2<TH2I>(hname, htitle, HistEnergyBins, HistEnergyMin, HistEnergyMax, HistESumBins, HistESumMin, HistESumMax, folder, hxtitle, "KER (eV)");
		
		fHistEnergy2Particle[i] = std::vector<TH2I*> (mNumParticles);
		for (UInt_t j=0; j<mNumParticles; j++)
			if (j != i)
			{
				// decide if we need spectra for electrons or ions
				TString OtherParticle;
				if (fMomentumCalculators[i]->GetMassAu() == 1.)
					OtherParticle = "electrons";
				else 
					OtherParticle = "ions";
				
				fieldname = basename + OtherParticle + "_hist_energy_bins";
				Int_t HistOtherEnergyBins = fConfigFile->read(fieldname.Data(), 100);
				fieldname = basename + OtherParticle + "_hist_energy_min";
				Float_t HistOtherEnergyMin = fConfigFile->read(fieldname.Data(), 0.);
				fieldname = basename + OtherParticle + "_hist_energy_max";
				Float_t HistOtherEnergyMax = fConfigFile->read(fieldname.Data(), 100.);

				hname = "EnergyCorr_" + mParticleNames[i] + mParticleNames[j] + "_" + mShortName;
				htitle  = "Energy correlation between " + mParticleNames[i] + " and " + mParticleNames[j] + " in " + mShortName + " coincidence";
				hxtitle = "Energy " + mParticleNames[j] + " (eV)";
				hytitle = "Energy " + mParticleNames[i] + " (eV)";
				fHistEnergy2Particle[i][j] = MakeH2<TH2I>(hname, htitle, HistOtherEnergyBins, HistOtherEnergyMin, HistOtherEnergyMax, HistEnergyBins, HistEnergyMin, HistEnergyMax, folder, hxtitle, hytitle);
			}
			
		folder = basefolder + "/Coinc/" + mShortName + "/" + mParticleNames[i] + "/angles";
		
		hname = "Phi_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Azimuthal angle " + mParticleNames[i] + " for " + mLongName + " coincidence";
		fHistPhi[i] = MakeH1<TH1I>(hname, htitle, HistPhiBins, 0, 360, folder, "Phi (degrees)");
		
		hname = "Theta_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Polar angle " + mParticleNames[i] + " for " + mLongName + " coincidence";
		fHistTheta[i] = MakeSinusoidalH1<TH1I>(hname, htitle, HistThetaBins, 0, 180, folder, "Theta (degrees)");
		
		hname = "Spherical_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Solid angle emission pattern " + mParticleNames[i] + " for " + mLongName + " coincidence";
		fHistShperical[i] = MakeSphericalH2<TH2I>(hname, htitle, HistPhiBins/4, 0, 360, HistThetaBins/4, 0, 180, folder, "Phi (degrees)", "Theta (degrees)");
		
		// Mutual angles
		fHistMutualAngle[i] = std::vector<TH1I*> (mNumParticles);
		for (UInt_t j=0; j<mNumParticles; j++)
			if (j != i)
			{
				hname = "Angle_" + mParticleNames[i] + mParticleNames[j] + "_" + mShortName;
				htitle  = "Angle between " + mParticleNames[i] + " and " + mParticleNames[j] + " in " + mShortName + " coincidence";
				fHistMutualAngle[i][j] = MakeSinusoidalH1<TH1I>(hname, htitle, HistThetaBins, 0, 180, folder);
			}
		
		// Spectra showing individual momentum components vs. sum
		folder = basefolder + "/Coinc/" + mShortName + "/" + mParticleNames[i] + "/VsSum";
		
		hname = "PxSum_" + mShortName + "VsPx_" + mParticleNames[i];
		htitle = "X Momentum sum " + mLongName + " vs X momentum " + mParticleNames[i];
		hxtitle = "Px " + mParticleNames[i] + " (a.u.)";
		fHistPxSumVsPx[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistSumBins, HistSumMin, HistSumMax, folder, hxtitle, "PxSum (a.u.)");
		
		hname = "PySum_" + mShortName + "VsPy_" + mParticleNames[i];
		htitle = "Y Momentum sum " + mLongName + " vs Y momentum " + mParticleNames[i];
		hxtitle = "Py " + mParticleNames[i] + " (a.u.)";
		fHistPySumVsPy[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistSumBins, HistSumMin, HistSumMax, folder, hxtitle, "PySum (a.u.)");
		
		hname = "PzSum_" + mShortName + "VsPz_" + mParticleNames[i];
		htitle = "Z Momentum sum " + mLongName + " vs Z momentum " + mParticleNames[i];
		hxtitle = "Pz " + mParticleNames[i] + " (a.u.)";
		fHistPzSumVsPz[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistSumBins, HistSumMin, HistSumMax, folder, hxtitle, "PzSum (a.u.)");
		
		hname = "Px_" + mParticleNames[i] + "VsPxOther_" + mShortName;
		htitle = "X momentum " + mParticleNames[i] + " vs. sum of other X momenta in " + mLongName + " coincidence";
		hxtitle = "PxSum - Px " + mParticleNames[i] + " (a.u.)";
		hytitle = "Px " + mParticleNames[i] + " (a.u.)";
		fHistPxVsOther[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistMomBins, HistMomMin, HistMomMax, folder, hxtitle, hytitle);
		
		hname = "Py_" + mParticleNames[i] + "VsPyOther_" + mShortName;
		htitle = "Y momentum " + mParticleNames[i] + " vs. sum of other Y momenta in " + mLongName + " coincidence";
		hxtitle = "PySum - Py " + mParticleNames[i] + " (a.u.)";
		hytitle = "Py " + mParticleNames[i] + " (a.u.)";
		fHistPyVsOther[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistMomBins, HistMomMin, HistMomMax, folder, hxtitle, hytitle);
		
		hname = "Pz_" + mParticleNames[i] + "VsPzOther_" + mShortName;
		htitle = "Z momentum " + mParticleNames[i] + " vs. sum of other Z momenta in " + mLongName + " coincidence";
		hxtitle = "PzSum - Pz " + mParticleNames[i] + " (a.u.)";
		hytitle = "Pz " + mParticleNames[i] + " (a.u.)";
		fHistPzVsOther[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistMomBins, HistMomMin, HistMomMax, folder, hxtitle, hytitle);
		
		hname = "DeltaPhi_" + mParticleNames[i] + mShortName;
		htitle = "DeltaPhi vs. longitudinal momentum for " + mParticleNames[i] + " in " + mLongName + " conincidence";
		fHistDeltaPhi[i] = MakeH2<TH2I>(hname, htitle, HistMomBins/4, HistMomMin, HistMomMax, 360, -360, 360, folder, "P_long [a.u.]" , "DeltaPhi [degree]");
		
		// Spectra showing Kinetic energy release vs. individual particles' momenta
		hytitle = "KER (eV)";
		hname = "Ker_" + mShortName + "VsPx_" + mParticleNames[i];
		htitle = "Kinetic energie release of " + mLongName + " vs X momentum " + mParticleNames[i];
		hxtitle = "Px " + mParticleNames[i] + " (a.u.)";
		fHistKerVsPx[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistESumBins, HistESumMin, HistESumMax, folder, hxtitle, hytitle);
		
		hname = "Ker_" + mShortName + "VsPy_" + mParticleNames[i];
		htitle = "Kinetic energie release of " + mLongName + " vs Y momentum " + mParticleNames[i];
		hxtitle = "Py " + mParticleNames[i] + " (a.u.)";
		fHistKerVsPy[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistESumBins, HistESumMin, HistESumMax, folder, hxtitle, hytitle);
		
		hname = "Ker_" + mShortName + "VsPz_" + mParticleNames[i];
		htitle = "Kinetic energie release of " + mLongName + " vs Z momentum " + mParticleNames[i];
		hxtitle = "Pz " + mParticleNames[i] + " (a.u.)";
		fHistKerVsPz[i] = MakeH2<TH2I>(hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistESumBins, HistESumMin, HistESumMax, folder, hxtitle, hytitle);
		
		
	}

	folder = basefolder + "/Coinc/" + mShortName;
	
	hname = "NumCoinc_" + mShortName;
	htitle = "No. of " + mLongName + " coincicendences per event";
	fHistNumCoincidences = MakeH1<TH1I>(hname, htitle, HistNumBins, -0.5, static_cast<Double_t>(HistNumBins)-0.5, folder );


	//-------------------//
	// Create conditions //
	//-------------------//
	folder = basefolder + "/Coinc/" + mShortName;
	
	cname = "c_PxSum_" + mShortName;
	ctitle = "Condition for X momentum conservation for " + mLongName;
	hname = "PxSum_" + mShortName;
	fConPxSum = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_PySum_" + mShortName;
	ctitle = "Condition for Y momentum conservation for " + mLongName;
	hname = "PySum_" + mShortName;
	fConPySum = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_PzSum_" + mShortName;
	ctitle = "Condition for Z momentum conservation for " + mLongName;
	hname = "PzSum_" + mShortName;
	fConPzSum = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Ker_" + mShortName;
	ctitle = "Condition on kinetic energy release for " + mLongName;
	hname = fHistKer->GetName();
	fConKer = MakeWinCon(cname, ctitle, hname, folder);
	
	//-----------------//
	// Create Pictures //
	//-----------------//
	
	folder = basefolder + "/Coinc/" + mShortName;
	
	pname = "PicSums_" + mShortName;
	ptitle = "Momentum sums for " + mLongName;
	fPicSums = MakePicture(pname, ptitle, folder, 1, 3);
	AddCondToPicture(fConPxSum, fPicSums, 0, 0);
	AddCondToPicture(fConPySum, fPicSums, 0, 1);
	AddCondToPicture(fConPzSum, fPicSums, 0, 2);
	
	pname = "PicPxVsSum_" + mShortName;
	ptitle = "Calibration check for X momentum using momentum sum for " + mLongName;
	fPicPxSum = MakePicture(pname, ptitle, folder, (mNumParticles-1)/2+1, 2);
	
	pname = "PicPyVsSum_" + mShortName;
	ptitle = "Calibration check for Y momentum using momentum sum for " + mLongName;
	fPicPySum = MakePicture(pname, ptitle, folder, (mNumParticles-1)/2+1, 2);
	
	pname = "PicPzVsSum_" + mShortName;
	ptitle = "Calibration check for Z momentum using momentum sum for " + mLongName;
	fPicPzSum = MakePicture(pname, ptitle, folder, (mNumParticles-1)/2+1, 2);
	
	pname = "PicPxVsOther_" + mShortName;
	ptitle = "Calibration check for X momentum using momenta for " + mLongName;
	fPicPxVsOther = MakePicture(pname, ptitle, folder, (mNumParticles-1)/2+1, 2);
	
	pname = "PicPyVsOther_" + mShortName;
	ptitle = "Calibration check for Y momentum using momenta sum for " + mLongName;
	fPicPyVsOther = MakePicture(pname, ptitle, folder, (mNumParticles-1)/2+1, 2);
	
	pname = "PicPzvsOther_" + mShortName;
	ptitle = "Calibration check for Z momentum using momenta sum for " + mLongName;
	fPicPzVsOther = MakePicture(pname, ptitle, folder, (mNumParticles-1)/2+1, 2);
	
	fPicPartMom = std::vector<TGo4Picture*> (mNumParticles);
	fPicPartSumX = std::vector<TGo4Picture*> (mNumParticles);
	fPicPartSumY = std::vector<TGo4Picture*> (mNumParticles);
	fPicPartSumZ = std::vector<TGo4Picture*> (mNumParticles);
	fPicKerVsMomenta = std::vector<TGo4Picture*> (mNumParticles);
	for (UShort_t i=0; i<mNumParticles; i++)
	{
		// decide if we need spectra for electrons or ions
		if (fMomentumCalculators[i]->GetMassAu() == 1.)
			Particle = "electrons";
		else 
			Particle = "ions";
		
		MomMinName = basename + Particle + "_hist_mom_min";
		Float_t HistMomMin = fConfigFile->read(MomMinName.Data(), -50.);
		MomMaxName = basename + Particle + "_hist_mom_max";
		Float_t HistMomMax = fConfigFile->read(MomMaxName.Data(), 50.);
		
		AddHistToPicture(fHistPxSumVsPx[i], fPicPxSum, i/2, i%2);
		AddLineToPicture(HistMomMin, 0, HistMomMax, 0, fPicPxSum, i/2, i%2);
		AddHistToPicture(fHistPySumVsPy[i], fPicPySum, i/2, i%2);
		AddLineToPicture(HistMomMin, 0, HistMomMax, 0, fPicPySum, i/2, i%2);
		AddHistToPicture(fHistPzSumVsPz[i], fPicPzSum, i/2, i%2);
		AddLineToPicture(HistMomMin, 0, HistMomMax, 0, fPicPzSum, i/2, i%2);
		
		AddHistToPicture(fHistPxVsOther[i], fPicPxVsOther, i/2, i%2);
		AddLineToPicture(HistMomMin, HistMomMax, HistMomMax, HistMomMin, fPicPxVsOther, i/2, i%2);
		AddHistToPicture(fHistPyVsOther[i], fPicPyVsOther, i/2, i%2);
		AddLineToPicture(HistMomMin, HistMomMax, HistMomMax, HistMomMin, fPicPyVsOther, i/2, i%2);
		AddHistToPicture(fHistPzVsOther[i], fPicPzVsOther, i/2, i%2);
		AddLineToPicture(HistMomMin, HistMomMax, HistMomMax, HistMomMin, fPicPzVsOther, i/2, i%2);
		
		folder = basefolder + "/Coinc/" + mShortName + "/" + mParticleNames[i];
		
		pname = "PicMom_" + mParticleNames[i] + "_" + mShortName;
		ptitle = "Momentum overview for " + mParticleNames[i] + " in " + mLongName + " coincidence";
		fPicPartMom[i] = MakePicture(pname, ptitle, folder, 2, 3);
		AddHistToPicture(fHistPx[i], fPicPartMom[i], 0, 0);
		AddHistToPicture(fHistPy[i], fPicPartMom[i], 0, 1);
		AddHistToPicture(fHistPz[i], fPicPartMom[i], 0, 2);
		AddHistToPicture(fHistPxy[i], fPicPartMom[i], 1, 0);
		AddHistToPicture(fHistPxz[i], fPicPartMom[i], 1, 1);
		AddHistToPicture(fHistPyz[i], fPicPartMom[i], 1, 2);
		
		pname = "PicXsum_" + mParticleNames[i] + "_" + mShortName;
		ptitle = "X momentum (sum) overview for  " + mParticleNames[i] + " in " + mLongName + " coincidence";
		fPicPartSumX[i] = MakePicture(pname, ptitle, folder, 2, 2);
		AddHistToPicture(fHistPxSumVsPx[i], fPicPartSumX[i], 0, 0);
		AddLineToPicture(HistMomMin, 0, HistMomMax, 0, fPicPartSumX[i], 0, 0);
		AddHistToPicture(fHistPxVsOther[i], fPicPartSumX[i], 0, 1);
		AddLineToPicture(HistMomMin, HistMomMax, HistMomMax, HistMomMin, fPicPartSumX[i], 0, 1);
		AddCondToPicture(fConPxSum, fPicPartSumX[i], 1, 0);
		AddHistToPicture(fHistPx[i], fPicPartSumX[i], 1, 1);
		
		pname = "PicYsum_" + mParticleNames[i] + "_" + mShortName;
		ptitle = "Y momentum (sum) overview for  " + mParticleNames[i] + " in " + mLongName + " coincidence";
		fPicPartSumY[i] = MakePicture(pname, ptitle, folder, 2, 2);
		AddHistToPicture(fHistPySumVsPy[i], fPicPartSumY[i], 0, 0);
		AddLineToPicture(HistMomMin, 0, HistMomMax, 0, fPicPartSumY[i], 0, 0);
		AddHistToPicture(fHistPyVsOther[i], fPicPartSumY[i], 0, 1);
		AddLineToPicture(HistMomMin, HistMomMax, HistMomMax, HistMomMin, fPicPartSumY[i], 0, 1);
		AddCondToPicture(fConPySum, fPicPartSumY[i], 1, 0);
		AddHistToPicture(fHistPy[i], fPicPartSumY[i], 1, 1);
		
		pname = "PicZsum_" + mParticleNames[i] + "_" + mShortName;
		ptitle = "Z momentum (sum) overview for  " + mParticleNames[i] + " in " + mLongName + " coincidence";
		fPicPartSumZ[i] = MakePicture(pname, ptitle, folder, 2, 2);
		AddHistToPicture(fHistPzSumVsPz[i], fPicPartSumZ[i], 0, 0);
		AddTF1LineToPicture(HistMomMin, 0, HistMomMax, 0, fPicPartSumZ[i], 0, 0);
		AddHistToPicture(fHistPzVsOther[i], fPicPartSumZ[i], 0, 1);
		AddTF1LineToPicture(HistMomMin, HistMomMax, HistMomMax, HistMomMin, fPicPartSumZ[i], 0, 1);
		AddCondToPicture(fConPzSum, fPicPartSumZ[i], 1, 0);
		AddHistToPicture(fHistPz[i], fPicPartSumZ[i], 1, 1);
		
		pname = "PicKer_" + mShortName + "VsMom_" + mParticleNames[i];
		ptitle = "KER vs individual momentum components of " + mParticleNames[i] + " in " + mLongName + " coincidence";
		fPicKerVsMomenta[i] = MakePicture(pname, ptitle, folder, 1, 3);
		AddHistToPicture(fHistKerVsPx[i], fPicKerVsMomenta[i], 0, 0);
		AddHistToPicture(fHistKerVsPy[i], fPicKerVsMomenta[i], 0, 1);
		AddHistToPicture(fHistKerVsPz[i], fPicKerVsMomenta[i], 0, 2);
	}
	
	// Init Newton and Dalitz plots if necessary
	if (mNumParticles == 3)
	{
		InitNewtonPlots(CalcConf);
		InitDalitzPlots(CalcConf);
	}
}

/**
 * \brief Clear internal values for next event
 */
void TRemiCalcProcCoincidences::Clear()
{	
	mFoundCoincidences.clear();
}

/** 
 * \brief Finds coincidences in the momentum data
 * \param Coincidences Reference to the vector of coincidences owned by the TRemiCalcProc processor
 */
void TRemiCalcProcCoincidences::FindCoincidences(std::vector< TRemiCoincidence >& Coincidences)
{
	// Set all iterators to their respective particle list
	for (UInt_t i=0; i<mNumParticles; i++)
	{
		fPartBegins[i] = fMomentumCalculators[i]->GetCalculatedParticles().begin();
		fPartIts[i] = fPartBegins[i];
		fPartEnds[i] = fMomentumCalculators[i]->GetCalculatedParticles().end();
	}
	
	if (mNumParticles)
		while (GetNextParticle(fPartIts, 0)) // This now loops automatically through all possible combinations of particle coincidences
		{
			// Calculate momentum sum and KER
			mMomentumSum.Null();
			Float_t Ker = 0;
			for (UInt_t i=0; i<mNumParticles; i++)
			{
				mMomentumSum += (*fPartIts[i])->Mom();
				Ker += (*fPartIts[i])->Energy();
			}
			
			// Check momentum sum conditions
			if ( fConPxSum->Test(mMomentumSum.X()) && fConPySum->Test(mMomentumSum.Y()) && fConPzSum->Test(mMomentumSum.Z()) && fConKer->Test(Ker) )
			{
				TRemiCoincidence NewCoincidence (mIdentifier, mNumParticles);
				for (UInt_t i=0; i<mNumParticles; i++)
					NewCoincidence.SetParticle(i, *(*fPartIts[i]));
				NewCoincidence.SetMomentumSum(mMomentumSum);
				NewCoincidence.CalculateKer();
				
				// If we have a species more than once in the definition of this coincidence handler we have to check if we have created a duplicate coincidence and discard this one
				if (mHaveSameSpecies)
				{
					Bool_t IsIdentical = kFALSE;
					for (std::vector<TRemiCoincidence>::iterator CoinIt = Coincidences.begin(); CoinIt != Coincidences.end(); ++CoinIt)
						if (*CoinIt == NewCoincidence)
						{
							IsIdentical = kTRUE;
							break;
						}
					if (!IsIdentical)
					{
						Coincidences.push_back(NewCoincidence);
						mFoundCoincidences.push_back(&Coincidences.back());
					}
					// Note: If IsIdentical is true, nothing will be done!
				}
				else	// This handler is defined with distinct species
				{
					Coincidences.push_back(NewCoincidence);
					mFoundCoincidences.push_back(&Coincidences.back());
				}
			}
			
			++(fPartIts.back()); // Only the iterator of the deepest recursion level is incremented and the surch continues...
		}
}

/**
 * \brief Recursively searches the momentum vector for coincidences
 * \param PartIts Reference to vector of iterators on the particle vector
 * \param Particles Reference to the vector of particles
 * \param Level Level of recursion
 */
Bool_t TRemiCalcProcCoincidences::GetNextParticle(std::vector< std::vector<TRemiParticle*>::iterator > &PartIts, UInt_t Level)
{
	while (PartIts[Level] != fPartEnds[Level])
	{
		if ( (Level==0) || CheckUnique(PartIts, Level) ) 
		{	// We have found a particle of the species we are looking for. 
			if (Level == mNumParticles-1)
				return kTRUE; // Have reached deepest level of recursion, e.g. all particles have been found!
			else
			{
				if (GetNextParticle(PartIts, Level+1))
					return kTRUE; // Going back the whole recursion chain if a coincidence was found
				else 
					PartIts[Level+1] = fPartBegins[Level+1]; // have to reset next level iterator because we will now continue searching with the next particle on the current level
			}
		}
		// when we get to this point, no return has been made, so we have to increment current iterator
		++(PartIts[Level]);
	}
	// At this point, no particle was found in this level
	return kFALSE;
}

/**
 * \brief Checks uniqueness of a particle in a coincidence
 * \return False if the Particle at Level is already accessed by an iterator of smaller index
 */
Bool_t TRemiCalcProcCoincidences::CheckUnique(std::vector< __gnu_cxx::__normal_iterator< TRemiParticle**, std::vector< TRemiParticle* > > >& PartIts, UInt_t Level)
{
	for (UInt_t i=0; i<Level; i++)
		if ((*PartIts[Level])->GetTof() == (*PartIts[i])->GetTof())
			return kFALSE;
	return kTRUE;
}


void TRemiCalcProcCoincidences::FillCoincidenceHistograms()
{
	Int_t NumCoincidences = 0; // Count number of coincidences for the current channel
	TRemiPxyz MomOther; // Temporary object to store the momentum of all other particles in the conincidence
	for (std::vector<TRemiCoincidence*>::iterator CoinIt = mFoundCoincidences.begin(); CoinIt != mFoundCoincidences.end(); ++CoinIt) // iterate through all coincidences recently found by this handler
	{
		// Momentum sums
		fHistPxSum->Fill((*CoinIt)->GetMomentumSum().X());
		fHistPySum->Fill((*CoinIt)->GetMomentumSum().Y());
		fHistPzSum->Fill((*CoinIt)->GetMomentumSum().Z());
		
		
		for (UInt_t i=0; i<mNumParticles; i++)
		{
			// Individual particles' momenta
			fHistPx[i]->Fill((*CoinIt)->GetParticle(i).Mom().X());
			fHistPy[i]->Fill((*CoinIt)->GetParticle(i).Mom().Y());
			fHistPz[i]->Fill((*CoinIt)->GetParticle(i).Mom().Z());
			fHistPxy[i]->Fill((*CoinIt)->GetParticle(i).Mom().X(), (*CoinIt)->GetParticle(i).Mom().Y());
			fHistPxz[i]->Fill((*CoinIt)->GetParticle(i).Mom().Z(), (*CoinIt)->GetParticle(i).Mom().X());
			fHistPyz[i]->Fill((*CoinIt)->GetParticle(i).Mom().Z(), (*CoinIt)->GetParticle(i).Mom().Y());
			fHistPxyz[i]->Fill((*CoinIt)->GetParticle(i).Mom().X(), (*CoinIt)->GetParticle(i).Mom().Y(), (*CoinIt)->GetParticle(i).Mom().Z());
			fHistPtranslong[i]->Fill((*CoinIt)->GetParticle(i).Mom().Z(), (*CoinIt)->GetParticle(i).Mom().Rho());
			fHistRadTof[i]->Fill( (*CoinIt)->GetParticle(i).Tof(), (*CoinIt)->GetParticle(i).Length());
			fHistXTof[i]->Fill( (*CoinIt)->GetParticle(i).Tof(), (*CoinIt)->GetParticle(i).X());
			fHistYTof[i]->Fill( (*CoinIt)->GetParticle(i).Tof(), (*CoinIt)->GetParticle(i).Y());
			fHistEnergy[i]->Fill((*CoinIt)->GetParticle(i).Energy());
			fHistPhi[i]->Fill((*CoinIt)->GetParticle(i).Mom().Phi());
			fHistTheta[i]->Fill((*CoinIt)->GetParticle(i).Mom().Theta());
			fHistShperical[i]->Fill((*CoinIt)->GetParticle(i).Mom().Phi(), (*CoinIt)->GetParticle(i).Mom().Theta());
			
			// Momentum sum vs. individual particle
			fHistPxSumVsPx[i]->Fill((*CoinIt)->GetParticle(i).Mom().X(), (*CoinIt)->GetMomentumSum().X());
			fHistPySumVsPy[i]->Fill((*CoinIt)->GetParticle(i).Mom().Y(), (*CoinIt)->GetMomentumSum().Y());
			fHistPzSumVsPz[i]->Fill((*CoinIt)->GetParticle(i).Mom().Z(), (*CoinIt)->GetMomentumSum().Z());
			
			MomOther = (*CoinIt)->GetMomentumSum() - (*CoinIt)->GetParticle(i).Mom();
			fHistPxVsOther[i]->Fill(MomOther.X(), (*CoinIt)->GetParticle(i).Mom().X());
			fHistPyVsOther[i]->Fill(MomOther.Y(), (*CoinIt)->GetParticle(i).Mom().Y());
			fHistPzVsOther[i]->Fill(MomOther.Z(), (*CoinIt)->GetParticle(i).Mom().Z());
			
			fHistDeltaPhi[i]->Fill((*CoinIt)->GetParticle(i).Mom().Z(), (*CoinIt)->GetParticle(i).Mom().Phi() - MomOther.Phi());
			
			fHistKerVsEnergy[i]->Fill((*CoinIt)->GetParticle(i).Energy(), (*CoinIt)->GetKer());
			fHistKerVsPx[i]->Fill((*CoinIt)->GetParticle(i).Mom().X(), (*CoinIt)->GetKer());
			fHistKerVsPy[i]->Fill((*CoinIt)->GetParticle(i).Mom().Y(), (*CoinIt)->GetKer());
			fHistKerVsPz[i]->Fill((*CoinIt)->GetParticle(i).Mom().Z(), (*CoinIt)->GetKer());
			
			// Correlation spectra
			for (UInt_t j=0; j<mNumParticles; j++)
				if (i != j)
				{
					fHistMutualAngle[i][j]->Fill( (*CoinIt)->GetParticle(i).Mom().Angle( (*CoinIt)->GetParticle(j).Mom() ) );
					fHistEnergy2Particle[i][j]->Fill( (*CoinIt)->GetParticle(j).Energy(), (*CoinIt)->GetParticle(i).Energy() );
				}
		}

		// Kinetic energy release spectrum
		fHistKer->Fill((*CoinIt)->GetKer());
		NumCoincidences++;
		
		// Fill Newton and Dalitz plots if necessary
		if (mNumParticles == 3)
		{
			FillCoincidenceNewtonPlots(*(*CoinIt));
			FillCoincidenceDalitzPlots(*(*CoinIt));
		}
	}
	
	fHistNumCoincidences->Fill(NumCoincidences);
}


/**
* \brief Init Histograms for Newton Plots of three-particle coincidences
*/
void TRemiCalcProcCoincidences::InitNewtonPlots(ConfigFile* CalcConf)
{
	Double_t HistNewtonMin = CalcConf->read("calc_coinc_hist_newton_min", -2.);
	Double_t HistNewtonMax = CalcConf->read("calc_coinc_hist_newton_max", 2.);
	Int_t HistNewtonBins = CalcConf->read("calc_coinc_hist_newton_bins", 100);
	
	TString hname, htitle, hxtitle, hytitle, folder = "Calc/Coinc/" + mShortName + "/Newton", Particle, MomBinsName, MomMinName, MomMaxName, basename = "calc_";
	for (UInt_t i=0; i<mNumParticles; i++)
	{
		// Decide if we want electron or ion spectra
		if ( (fMomentumCalculators[(i+1)%mNumParticles]->GetMassAu() == 1.) || (fMomentumCalculators[(i+2)%mNumParticles]->GetMassAu() == 1.) )
			Particle = "electrons";
		else 
			Particle = "ions";
		
		MomBinsName = basename + Particle + "_hist_mom_bins";
		Int_t HistMomBins = fConfigFile->read(MomBinsName.Data(), 100);
		MomMinName = basename + Particle + "_hist_mom_min";
		Float_t HistMomMin = fConfigFile->read(MomMinName.Data(), -50.);
		MomMaxName = basename + Particle + "_hist_mom_max";
		Float_t HistMomMax = fConfigFile->read(MomMaxName.Data(), 50.);
		
		hname = "Newton_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Newton plot referenced to " + mParticleNames[i] + " in " + mLongName + " coincidence";
		hxtitle = mParticleNames[i] + " direction";
		hytitle = "Down: " + mParticleNames[(i+2)%mNumParticles] + ", Up: " + mParticleNames[(i+1)%mNumParticles];
		fHistNewton[i] = MakeH2<TH2I> (hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistMomBins, HistMomMin, HistMomMax, folder, hxtitle, hytitle);
		
		hname = "NewtonN_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Normalised Newton plot referenced to " + mParticleNames[i] + " in " + mLongName + " coincidence";
		fHistNewtonN[i] = MakeH2<TH2I> (hname, htitle, HistNewtonBins, HistNewtonMin, HistNewtonMax, HistNewtonBins, HistNewtonMin, HistNewtonMax, folder, hxtitle, hytitle);
		
		hname = "NewtonS_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Shifted Newton plot referenced to " + mParticleNames[i] + " in " + mLongName + " coincidence";
		fHistNewtonS[i] = MakeH2<TH2I> (hname, htitle, HistMomBins, HistMomMin, HistMomMax, HistMomBins, HistMomMin, HistMomMax, folder, hxtitle, hytitle);
		
		hname = "NewtonNS_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Normalised and shifted Newton plot referenced to " + mParticleNames[i] + " in " + mLongName + " coincidence";
		fHistNewtonNS[i] = MakeH2<TH2I> (hname, htitle, HistNewtonBins, HistNewtonMin, HistNewtonMax, HistNewtonBins, HistNewtonMin, HistNewtonMax, folder, hxtitle, hytitle);
	}
}


/**
* \brief Init Histograms for Dalitz Plots of three-particle coincidences
*/
void TRemiCalcProcCoincidences::InitDalitzPlots(ConfigFile* CalcConf)
{
	Double_t HistDalitzMin = CalcConf->read("calc_coinc_hist_dalitz_min", -2.);
	Double_t HistDalitzMax = CalcConf->read("calc_coinc_hist_dalitz_max", 2.);
	Int_t HistDalitzBins = CalcConf->read("calc_coinc_hist_dalitz_bins", 100);
	
	TString hname, htitle, hxtitle, hytitle, folder = "Calc/Coinc/" + mShortName + "/Dalitz", Particle, basename = "calc_";
	for (UInt_t i=0; i<mNumParticles; i++)
	{
		// Decide if we want electron or ion spectra
		if ( (fMomentumCalculators[(i+1)%mNumParticles]->GetMassAu() == 1.) || (fMomentumCalculators[(i+2)%mNumParticles]->GetMassAu() == 1.) )
			Particle = "electrons";
		else 
			Particle = "ions";
		
		hname = "DalitzP_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Momentum Dalitz plot referenced to " + mParticleNames[i] + " in " + mLongName + " coincidence";
		hxtitle = "(p2 - p3 ) / sqrt(3)";
		hytitle = "p1 - 1/3";
		fHistDalitzP[i] = MakeH2<TH2I> (hname, htitle, HistDalitzBins, HistDalitzMin, HistDalitzMax, HistDalitzBins, HistDalitzMin, HistDalitzMax, folder, hxtitle, hytitle);
		
		hname = "DalitzE_" + mParticleNames[i] + "_" + mShortName;
		htitle = "Energy Dalitz plot referenced to " + mParticleNames[i] + " in " + mLongName + " coincidence";
		hxtitle = "(E2 - E3) / sqrt(3)";
		hytitle = "E1 - 1/3";
		fHistDalitzE[i] = MakeH2<TH2I> (hname, htitle, HistDalitzBins, HistDalitzMin, HistDalitzMax, HistDalitzBins, HistDalitzMin, HistDalitzMax, folder, hxtitle, hytitle);
	}
}


/**
* \brief Fill Newton plots for the complete coincidence
*/
void TRemiCalcProcCoincidences::FillCoincidenceNewtonPlots(TRemiCoincidence& ThreeParticles)
{
	for (UInt_t i=0; i<mNumParticles; i++)
		FillParticleNewtonPlots(i, ThreeParticles.GetParticle(i), ThreeParticles.GetParticle((i+1)%mNumParticles), ThreeParticles.GetParticle((i+2)%mNumParticles));
}


/**
* \brief Fill Dalitz plots for the complete coincidence
*/
void TRemiCalcProcCoincidences::FillCoincidenceDalitzPlots(TRemiCoincidence& ThreeParticles)
{
	for (UInt_t i=0; i<mNumParticles; i++)
		FillParticleDalitzPlots(i, ThreeParticles.GetParticle(i), ThreeParticles.GetParticle((i+1)%mNumParticles), ThreeParticles.GetParticle((i+2)%mNumParticles));
}


/**
* \brief Fill Newton Plots referenced to one particle in a three-body coincidence
* \param Index Running No. of reference particle
* \param Fixed Reference particle, will be rotated onto the z-axis
* \param Plotted1 Particle which momentum will be plotted to upper half
* \param Plotted1 Particle which momentum will be plotted to lower half
*/
void TRemiCalcProcCoincidences::FillParticleNewtonPlots(Int_t Index, TRemiParticle Fixed, TRemiParticle Plotted1, TRemiParticle Plotted2)
{
	// We want to rotate the reaction plane into the xz plane
	// First of all make momentum of 1st particle coincide with z axis
	Float_t thetaF = Fixed.Mom().Theta();
	Float_t phiF = Fixed.Mom().Phi();
	
	Fixed.Mom().RotateZ(-phiF);
	Plotted1.Mom().RotateZ(-phiF);
	Plotted2.Mom().RotateZ(-phiF);
	
	Fixed.Mom().RotateY(-thetaF);
	Plotted1.Mom().RotateY(-thetaF);
	Plotted2.Mom().RotateY(-thetaF);
	
	// Now rotate the remaining two particles' momenta into the xz plane
	Float_t phi1 = Plotted1.Mom().Phi();
	Plotted1.Mom().RotateZ(-phi1);
	Plotted2.Mom().RotateZ(-phi1);
	
	// Fill unnormalised and unshifted plots
	fHistNewton[Index]->Fill(Plotted1.Mom().Z(), Plotted1.Mom().X());
	fHistNewton[Index]->Fill(Plotted2.Mom().Z(), Plotted2.Mom().X());
	
	// Subtract 1st particle's recoil momentum from the other twos'
	Float_t mass1 = Plotted1.GetMass();
	Float_t mass2 = Plotted2.GetMass() ;
	TRemiPxyz Shifted1 = Plotted1.Mom() + Fixed.Mom() * ( mass1 / (mass1+mass2) ) ;
	TRemiPxyz Shifted2 = Plotted2.Mom() + Fixed.Mom() * ( mass2 / (mass1+mass2) );
	
	// Fill shifted plots
	fHistNewtonS[Index]->Fill(Shifted1.Z(), Shifted1.X());
	fHistNewtonS[Index]->Fill(Shifted2.Z(), Shifted2.X());
	
	// Normalise vectors to Length of 1st particle momentum
	Float_t scale = Fixed.Mom().Length();
	Plotted1.Mom() /= scale;
	Plotted2.Mom() /= scale;
	Shifted1 /= scale;
	Shifted2 /= scale;
	
	// Fill normalised plots
	fHistNewtonN[Index]->Fill(Plotted1.Mom().Z(), Plotted1.Mom().X());
	fHistNewtonN[Index]->Fill(Plotted2.Mom().Z(), Plotted2.Mom().X());
	
	// Fill shifted and normalised plots
	fHistNewtonNS[Index]->Fill(Shifted1.Z(), Shifted1.X());
	fHistNewtonNS[Index]->Fill(Shifted2.Z(), Shifted2.X());
}


/**
* \brief Fill Dalitz Plots referenced to one particle in a three-body coincidence
* \param Index Running No. of reference particle
*/
void TRemiCalcProcCoincidences::FillParticleDalitzPlots(Int_t Index, TRemiParticle Particle1, TRemiParticle Particle2, TRemiParticle Particle3)
{
	// Momentum Dalitz plots
	Float_t p1, p2, p3, ptot, px, py;

	p1 = Particle1.Mom() ^ Particle1.Mom(); // scalar product impl. by ^ overload
	p2 = Particle2.Mom() ^ Particle2.Mom();
	p3 = Particle3.Mom() ^ Particle3.Mom();

	ptot = p1 + p2 + p3;

	p1 /= ptot;
	p2 /= ptot;
	p3 /= ptot;

	px = (p2 - p3) / sqrt(3);
	py = p1 - 1/3.;

	fHistDalitzP[Index]->Fill(px, py);


	// Energy Dalitz plots
	Float_t E1, E2, E3, Etot, Ex, Ey;

	E1 = Particle1.Energy();
	E2 = Particle2.Energy();
	E3 = Particle3.Energy();

	Etot = E1 + E2 + E3;

	E1 /= Etot;
	E2 /= Etot;
	E3 /= Etot;

	Ex = (E2 - E3) / sqrt(3);
	Ey = E1 - 1/3.;

	fHistDalitzE[Index]->Fill(Ex, Ey);
}


