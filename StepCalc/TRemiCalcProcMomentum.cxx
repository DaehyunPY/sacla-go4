//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This contains the actual momentum calculation class
//----------------------------------------------------------------
#include "TRemiCalcProcMomentum.h"

#include "Parameters/TRemiMomentumParameter.h"

#include "TGo4PolyCond.h"
#include "TGo4WinCond.h"

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"


// non-integer static consts cannot be defined in header
const Double_t TRemiCalcProcMomentum::mEMASS   = 9.10938215e-31;
const Double_t TRemiCalcProcMomentum::mECHARGE = 1.602176487e-19;
const Double_t TRemiCalcProcMomentum::mAUP     = 1.99285179e-24;
const Double_t TRemiCalcProcMomentum::mNANO    = 1e-9;
const Double_t TRemiCalcProcMomentum::mMILLI   = 1e-3;
const Double_t TRemiCalcProcMomentum::mGS2T    = 1e-4;


/**
 * \brief Principal constructor using configuration file
 * \param CalcConf ConfigFile object of the Analysis step
 * \param Particle String to identify particle group (electron or ion) in configuration file
 * \param MassMultiplier (Optional, default=1) Fixed mass multiplier for all species in a group of particles
 * \param Index (Optional, default = 0) No. to identify a species within a group of particles
 * \param HistTof (Optional) Pointer to a input histogram with the particles' time-of-flight
 */
TRemiCalcProcMomentum::TRemiCalcProcMomentum(ConfigFile *CalcConf, TString Particle, Short_t Index, TH1I* HistTof) :
	TRemiIncludeFunctions(CalcConf)
{
	TString fieldname, basename="calc_", hname, htitle, folder, basefolder = "Calc/Mom", cname, ctitle, hxtitle, hytitle, paraname;
	
	//------------------------------------------//
	// Read information from configuration file //
	//------------------------------------------//

	basename = basename + Particle + "_";
	
	fieldname = basename + "massfactor";
	mMassMultiplier = fConfigFile->read(fieldname.Data(), 1.);
	fieldname = basename + "acceldirection";
	mAccelDirection = fConfigFile->read(fieldname.Data(), 1);
	mAccelDirection = TMath::Sign(1, mAccelDirection); // fix mAccelDirection o +/- 1

	fieldname.Form( basename + "species_%i_shortname", Index);
	mShortName = CalcConf->read (fieldname.Data(), Particle(0, 1) + TString::Format("%i", Index));
	fieldname.Form( basename + "species_%i_longname", Index);
	mLongName = CalcConf->read (fieldname.Data(), Particle + TString::Format("%i", Index));
	fieldname.Form( basename + "species_%i_mass", Index);
	mMass = CalcConf->read (fieldname.Data(), 1.);
	fieldname.Form( basename + "species_%i_charge", Index);
	mCharge = CalcConf->read (fieldname.Data(), 1.);
	
	//< Convert mass and charge to SI units
	mMass = mEMASS * mMass * mMassMultiplier;
	mCharge = mECHARGE * mCharge;
	mAbsCharge = TMath::Abs(mCharge);
	
	// Set identifier for this momentum calculator
	mIdentifier = mShortName.Hash();
	
	fieldname = basename + "hist_mom_min";
	Double_t hist_mom_min = CalcConf->read(fieldname.Data(), -1.);
	fieldname = basename + "hist_mom_max";
	Double_t hist_mom_max = CalcConf->read(fieldname.Data(), 1.);
	fieldname = basename + "hist_mom_bins";
	Int_t hist_mom_bins = CalcConf->read(fieldname.Data(), 100);

	fieldname = basename + "hist_energy_min";
	Double_t hist_energy_min = CalcConf->read(fieldname.Data(), 0.);
	fieldname = basename + "hist_energy_max";
	Double_t hist_energy_max = CalcConf->read(fieldname.Data(), 1.);
	fieldname = basename + "hist_energy_bins";
	Int_t hist_energy_bins = CalcConf->read(fieldname.Data(), 100);

	fieldname = basename + "hist_newton_bins";
	Int_t hist_newton_bins = CalcConf->read(fieldname.Data(), 21);
	fieldname = basename + "hist_phi_bins";
	Int_t hist_phi_bins = fConfigFile->read(fieldname.Data(), 360);
	fieldname = basename + "hist_theta_bins";
	Int_t hist_theta_bins = fConfigFile->read(fieldname.Data(), 180);
	
	fieldname = basename + "hist_alpha_min";
	Double_t hist_alpha_min = CalcConf->read (fieldname.Data(), 0.);
	fieldname = basename + "hist_alpha_max";
	Double_t hist_alpha_max = CalcConf->read (fieldname.Data(), 1.);
	fieldname = basename + "hist_alpha_bins";
	Int_t hist_alpha_bins = CalcConf->read (fieldname.Data(), 100);
	
	fieldname = basename + "hist_xy_min";
	Double_t hist_xy_min = CalcConf->read (fieldname.Data(), -0.);
	fieldname = basename + "hist_xy_max";
	Double_t hist_xy_max = CalcConf->read (fieldname.Data(), 1.);
	fieldname = basename + "hist_xy_bins";
	Int_t hist_xy_bins = CalcConf->read (fieldname.Data(), 100);
	
	fieldname = basename + "hist_tof_min";
	Double_t hist_tof_min = CalcConf->read (fieldname.Data(), 0.);
	fieldname = basename + "hist_tof_max";
	Double_t hist_tof_max = CalcConf->read (fieldname.Data(), 1.);
	fieldname = basename + "hist_tof_bins";
	Double_t hist_tof_bins = CalcConf->read (fieldname.Data(), 100);
	
	
	Int_t HistNumBins = CalcConf->read("calc_hist_num_bins", 50);
	
	Double_t xPol = CalcConf->read ("calc_polarization_x", 0.);
	Double_t yPol = CalcConf->read ("calc_polarization_y", 0.);
	Double_t zPol = CalcConf->read ("calc_polarization_z", 0.);
	mPolarizationAxis = TRemiPxyz (xPol, yPol, zPol);
	
	
	//-----------------//
	// Make Parameters //
	//-----------------//
	folder = basefolder;
	paraname = "Par " + mShortName + " momentum";
	fParaMom = MakeParam<TRemiMomentumParameter> (paraname, folder);

	//-------------------//
	// Create histograms //
	//-------------------//

	folder = basefolder + "/" + mShortName + "/";

	hname = "Ptrans_x_" + mShortName;
	htitle = "Transverse (x) momentum " + mLongName;
	fPtransX = MakeH1<TH1I>(hname, htitle, hist_mom_bins, hist_mom_min, hist_mom_max, folder, "P_x [a.u.]");

	hname = "Ptrans_y_" + mShortName;
	htitle = "Transverse (y) momentum " + mLongName;
	fPtransY = MakeH1<TH1I>(hname, htitle, hist_mom_bins, hist_mom_min, hist_mom_max, folder, "P_y [a.u.]");

	hname = "Plong_" + mShortName;
	htitle = "Longitudinal (z) momentum " + mLongName;
	fPlong = MakeH1<TH1I>(hname, htitle, hist_mom_bins, hist_mom_min, hist_mom_max, folder, "P_z [a.u.]");

	hname = "Ptrans_" + mShortName;
	htitle = "Transverse momentum " + mLongName ;
	fPxy = MakeH2<TH2I>(hname, htitle, hist_mom_bins, hist_mom_min, hist_mom_max, hist_mom_bins, hist_mom_min, hist_mom_max, folder, "P_x [a.u.]", "P_y [a.u.]");

	hname = "PlongPtrans_" + mShortName;
	htitle = "Transverse (radial) momentum vs. longitudinal momentum " + mLongName;
	fPlongPtrans = MakeH2<TH2I>(hname, htitle, hist_mom_bins, hist_mom_min, hist_mom_max, hist_mom_bins/2, 0, hist_mom_max, folder, "P_z [a.u.]", "P_r [a.u.]");

	hname = "Energy_" + mShortName;
	htitle = "Energy of " + mLongName;
	fEnergy = MakeH1<TH1I>(hname, htitle, hist_energy_bins, hist_energy_min, hist_energy_max, folder, "Energy [eV]");

	hname = "AnglePol_" + mShortName;
	htitle = "Angle between" + mLongName + " and polarization axis";
	fAnglePolarization = MakeSinusoidalH1<TH1I>(hname, htitle, hist_theta_bins, 0, 180, folder, "(deg)");

	folder += "check/";
	
	hname = "PtotalPz_" + mShortName;
	htitle = "Total momentum vs. z momentum " + mLongName;
	fPtotalPlong = MakeH2<TH2I>(hname, htitle, hist_mom_bins, hist_mom_min, hist_mom_max, hist_mom_bins, 0, hist_mom_max, folder, "P_z [a.u.]", "P_total [a.u.]");

	hname = "PtotalPx_" + mShortName;
	htitle = "Total momentum vs. x momentum " + mLongName;
	fPtotalPx = MakeH2<TH2I>(hname, htitle, hist_mom_bins, hist_mom_min, hist_mom_max, hist_mom_bins, 0, hist_mom_max, folder, "P_x [a.u.]", "P_total [a.u.]");

	hname = "PtotalPy_" + mShortName;
	htitle = "Total momentum vs. y momentum " + mLongName;
	fPtotalPy = MakeH2<TH2I>(hname, htitle, hist_mom_bins, hist_mom_min, hist_mom_max, hist_mom_bins, 0, hist_mom_max, folder, "P_y [a.u.]", "P_total [a.u.]");
	
	hname = "NewtonI_" + mShortName;
	htitle = "Newton iterations to get " + mLongName + " longitudinal momentum";
	fNewtonI = MakeH1<TH1I> (hname, htitle, hist_newton_bins, 0, hist_newton_bins-1, folder);
	
	folder += "input/";
	
	hname = "Num_" + mShortName;
	htitle = "Number of particles selected for momentum calculation as " + mLongName;
	fNumParticles = MakeH1<TH1I>(hname, htitle, HistNumBins, -0.5, static_cast<Double_t>(HistNumBins)-0.5, folder);
	
	hname = "XY_ConTof_" + mShortName;
	htitle = "Position with Tof condition for " + mLongName + " input";
	fXY_ConTof = MakeH2<TH2I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "X [mm]", "Y [mm]");
	
	hname = "XY_shifted_" + mShortName;
	htitle = "Position after shifting and scaling for " + mLongName + " input";
	fXY_shifted = MakeH2<TH2I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "X [mm]", "Y [mm]");
	
	hname = "Radius_" + mShortName;
	htitle = "Radius after shifting and scaling for " + mLongName + " input";
	fRadius = MakeH1<TH1I>(hname, htitle, hist_xy_bins/2, 0, hist_xy_max, folder, "Radius [mm]");
	
	hname = "XY_ConRad_" + mShortName;
	htitle = "Position after shifting and scaling for " + mLongName + " input with radius condition";
	fXY_ConRad = MakeH2<TH2I>(hname, htitle, hist_xy_bins, hist_xy_min, hist_xy_max, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "X [mm]", "Y [mm]");
	
	hname = "XY_Centering_" + mShortName;
	htitle = "Position after shifting and scaling for " + mLongName + " input with radius condition";
	fXY_centering = MakeH2<TH2I>(hname, htitle, 1, -1., 1., 1, -1., 1., folder, "X", "Y");
	
	hname = "X_Tof_" + mShortName;
	htitle = "X vs. Time of flight " + mLongName;
	fX_TOF = MakeH2<TH2I>(hname, htitle, hist_tof_bins, hist_tof_min, hist_tof_max, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "TOF[ns]","X[mm]");

	hname = "Y_Tof_" + mShortName;
	htitle = "Y vs. Time of flight " + mLongName;
	fY_TOF = MakeH2<TH2I>(hname, htitle, hist_tof_bins, hist_tof_min, hist_tof_max, hist_xy_bins, hist_xy_min, hist_xy_max, folder, "TOF[ns]","Y[mm]");	
	
	hname = "Rad_alpha_" + mShortName;
	htitle = "Radius vs. moved portion of cyclotron trajectory of " + mLongName;
	hxtitle = "Tof_" + mShortName + " / T_c = alpha / (2 Pi)";
	hytitle = "Rad_" + mShortName + " [mm]";
	fRad_alpha = MakeH2<TH2I> (hname, htitle, hist_alpha_bins, hist_alpha_min, hist_alpha_max, hist_xy_bins, 0, hist_xy_max, folder, hxtitle, hytitle);

	folder = basefolder + "/" + mShortName + "/check";
	
	hname = "PxPyPz_" + mShortName;
	htitle = "3D Momentum " + mLongName;
	fFull3DMomentum = MakeH3<TH3I>(hname, htitle, hist_mom_bins/4, hist_mom_min, hist_mom_max, hist_mom_bins/4, hist_mom_min, hist_mom_max, hist_mom_bins/4, hist_mom_min, hist_mom_max, folder, "P_x [a.u.]", "P_y [a.u.]", "P_z [a.u.]");
	
	folder = basefolder + "/" + mShortName + "/check/energy";
	
	hname = "EnergyPz_" + mShortName;
	htitle = "Energy vs. z momentum " + mLongName;
	fEnergyPlong = MakeH2<TH2I>(hname, htitle, hist_mom_bins, hist_mom_min, hist_mom_max, hist_energy_bins, hist_energy_min, hist_energy_max, folder, "P_z [a.u.]", "Energy [eV]");
	
	hname = "EnergyPx_" + mShortName;
	htitle = "Energy vs. x momentum " + mLongName;
	fEnergyPx = MakeH2<TH2I>(hname, htitle, hist_mom_bins, hist_mom_min, hist_mom_max, hist_energy_bins, hist_energy_min, hist_energy_max, folder, "P_x [a.u.]", "Energy [eV]");
	
	hname = "EnergyPy_" + mShortName;
	htitle = "Energy vs. y momentum " + mLongName;
	fEnergyPy = MakeH2<TH2I>(hname, htitle, hist_mom_bins, hist_mom_min, hist_mom_max, hist_energy_bins, hist_energy_min, hist_energy_max, folder, "P_y [a.u.]", "Energy [eV]");
	
	hname = "EnergyPhi_" + mShortName;
	htitle = "Energy vs. momentum azimuthal angle " + mLongName;
	fEnergyPhi = MakeH2<TH2I>(hname, htitle, hist_phi_bins, 0, 360, hist_energy_bins, hist_energy_min, hist_energy_max, folder, "Phi [degrees]", "Energy [eV]");
	
	hname = "EnergyTheta_" + mShortName;
	htitle = "Energy vs. momentum polar angle " + mLongName;
	fEnergyTheta = MakeH2<TH2I>(hname, htitle, hist_theta_bins, 0, 180, hist_energy_bins, hist_energy_min, hist_energy_max, folder, "Theta [degrees]", "Energy [eV]");
	
	hname = "EnergyCosTh_" + mShortName;
	htitle = "Energy vs. cosine of momentum polar angle " + mLongName;
	fEnergyCosTheta = MakeH2<TH2I>(hname, htitle, hist_theta_bins, -1, 1, hist_energy_bins, hist_energy_min, hist_energy_max, folder, "cos (Theta)", "Energy [eV]");
	
	
	//-------------------//
	// Create conditions //
	//-------------------//
	
	folder = basefolder + "/" + mShortName;
	cname = "c_Tof_" + mShortName;
	ctitle = "Time-of-flight condition to select " + mLongName;
	if (HistTof)
		fConTof = MakeWinCon(cname, ctitle, HistTof->GetName(), folder);
	else 
		fConTof = MakeWinCon(cname, ctitle, hist_tof_min, hist_tof_max, "", folder);
	
	cname = "c_Pos_" + mShortName;
	ctitle = "Position condition to select " + mLongName;
	hname = fXY_ConTof->GetName();
	fConPos = MakePolyCon(cname, ctitle, hname, folder);
	
	cname = "c_Rad_" + mShortName;
	ctitle = "Position radius condition for centering histograms" + mLongName;
	hname = fRadius->GetName();
	fConRad = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Energy_" + mShortName;
	ctitle = "Condition on Energy of " + mLongName;
	hname = fEnergy->GetName();
	fConEnergy = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_AnglePol_" + mShortName;
	ctitle = "Angle between Polarization and " + mLongName +  "condition";
	hname = fAnglePolarization->GetName();
	fConAnglePol = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Px_" + mShortName;
	ctitle = "Condition on X-Momentum of " + mLongName;
	hname = fPtransX->GetName();
	fConPx = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Py_" + mShortName;
	ctitle = "Condition on Y-Momentum of " + mLongName;
	hname = fPtransY->GetName();
	fConPy = MakeWinCon(cname, ctitle, hname, folder);
	
	cname = "c_Pz_" + mShortName;
	ctitle = "Condition on Z-Momentum of " + mLongName;
	hname = fPlong->GetName();
	fConPz = MakeWinCon(cname, ctitle, hname, folder);
}

/**
 * \brief Default constructor. Not to be used.
 */
TRemiCalcProcMomentum::TRemiCalcProcMomentum()
{
}


TRemiCalcProcMomentum::~TRemiCalcProcMomentum()
{
}

/**
 * \brief Clears internal variables for momentum calculation
 */
void TRemiCalcProcMomentum::Clear()
{
	mCalculatedParticles.clear();
}


/**
 * \brief Calculates momentum
 * \param Particles (Output) list of calculated particles
 * \param Hits list of detector hits to start calculation with
 *
 * For the longitudinal momentum the iterative Newton method is used. Its performance can be controlled by the three parameter elements NewtonPo, NewtonEpsilon and NewtonMaxI in the fParaMom parameter element for this kind of particle. If the drift length of the spectrometer is set to zero, the longitudinal momentum is calculated analytically. In this case the Newton parameter elements have no effect.
 * Both the Newton method and the analytic calculation requires a absolute time-of-flight. If this has not been set properly during the unpack step, the additional parameter element shiftT in fParaMom can be used. For further calibration, scaleT and scalePz scale the time-of-flight and the longitudinal momentum, respectively.
 *
 * The transverse momentum is calculated including the magnetic field and the cyclotron movement as described for electrons in Fischer's PhD thesis (2003). However, if the parameter element MagneticField is set to 0, the linear approximation described for ions is used.
 */
void TRemiCalcProcMomentum::CalculateMomentum(std::vector<TRemiParticle> &Particles, std::vector<TRemiDetectorHit> &Hits)
{
	// Variables used in Newton algorithm
	Float_t n_epsilon, n_tof, n_dtof, n_tof_input, n_pz, n_arg, n_sqrtarg, scalePall, n_dm, n_twoam, n_twomq;
	Int_t n_i;

	// Variables for transverse momentum
	Float_t omega, alpha, pr, phi, b_field, PtransConst;
	
	// Constant for transverse momentum
	if (fParaMom->MagneticField)
		PtransConst = mAbsCharge * mMILLI / (2 * mAUP);

	// Constants for Newton algorithm
	if (fParaMom->drift)
	{
		n_dm = fParaMom->drift * static_cast<Float_t> (mMass / mNANO / mAUP);
		n_twoam = 2 * fParaMom->accel * static_cast<Float_t> (mMass / mNANO / mAUP);
		n_twomq = 2 * TMath::Abs(fParaMom->u) * static_cast<Float_t> (mMass * mAbsCharge / (mAUP*mAUP) );
	}


	for(std::vector<TRemiDetectorHit>::iterator HitsIt = Hits.begin(); HitsIt != Hits.end(); ++HitsIt)
	{
		if ( fConTof->Test(HitsIt->Tof()) ) 
		{
			// Fill Position histogram with Tof condition
			fXY_ConTof->Fill(HitsIt->X(), HitsIt->Y());
			
			if (fConPos->Test(HitsIt->X(), HitsIt->Y()))
			{
				mNewParticle = *HitsIt;
				mNewParticle.SetIdentifier(mIdentifier);
				mNewParticle.SetMass(mMass / mEMASS);
				
				// Scaling and shifting of input values
				mNewParticle.SetTof( mNewParticle.Tof() - fParaMom->shiftT );
				mNewParticle.SetX( (mNewParticle.X() - fParaMom->shiftX + fParaMom->lincorshiftX * mNewParticle.Tof()) * fParaMom->scaleX );
				mNewParticle.SetY( (mNewParticle.Y() - fParaMom->shiftY + fParaMom->lincorshiftY * mNewParticle.Tof()) * fParaMom->scaleY );
				
				// Apply radius condition
				if (!fConRad->Test(mNewParticle.Length()))
					continue;

				// Longitudinal momentum in au
				n_tof_input = mNewParticle.Tof() * fParaMom->scaleT;
				if (fParaMom->drift == 0.)
				{ // if drift length is 0, calculate momentum analytically
					n_pz = -mAccelDirection * ( n_tof_input*mNANO*n_tof_input*mNANO * TMath::Abs(fParaMom->u*mCharge) - 2*mMass*fParaMom->accel*fParaMom->accel ) / (2*n_tof_input*mNANO*fParaMom->accel) / mAUP;
				}
				else
				{ // use Newton iteration
					n_epsilon = n_tof_input * fParaMom->NewtonEpsilon; // threshold value at which the equation should be fulfille
					n_pz = fParaMom->NewtonPo; // set momentum to start value;
					n_arg = n_pz*n_pz + n_twomq;
					n_sqrtarg = sqrt(n_arg);
					n_tof = - n_tof_input + n_dm / n_sqrtarg + n_twoam / ( mAccelDirection * n_pz + n_sqrtarg ); // value of the function tof at initial pz
					n_i = 0;
					while ( (n_i < fParaMom->NewtonMaxI) && (n_epsilon < fabs(n_tof) ) )
					{
						n_dtof = - n_dm * n_pz / pow(n_arg, 1.5) - mAccelDirection * n_twoam / (n_arg + mAccelDirection * n_pz * n_sqrtarg); // value of the derivative dtof/dpz at current pz
						n_pz = n_pz - n_tof / n_dtof; // Newton step to get new pz
						n_i++;
						n_arg = n_pz*n_pz + n_twomq;
						n_sqrtarg = sqrt(n_arg);
						n_tof = - n_tof_input + n_dm / n_sqrtarg + n_twoam / ( mAccelDirection * n_pz + n_sqrtarg ); // value of the funktion tof at current pz
					}
					fNewtonI->Fill(n_i); // iterations spectrum
				}

				// Transverse momentum in au
				if (fParaMom->MagneticField == 0.) // don't use magnetic field -> linear approximation
				{
					pr = mNewParticle.R() / mNewParticle.Tof() * static_cast<Float_t>( mMILLI * mMass / (mNANO * mAUP) );
					phi = mNewParticle.Phi(); // phi is in degrees
				}
				else // full calculation using magnetic field
				{
					b_field = TMath::Abs(mGS2T * fParaMom->MagneticField); // get absolute value of magnetic field in Tesla
					omega = mAbsCharge * b_field / mMass;	// Cyclotron angular frequency
					alpha = mNewParticle.Tof() * mNANO * omega;	// moved angular portion of cyclotron trajectory
					alpha = fmod(alpha, static_cast<Float_t>(TMath::TwoPi()) ); // NOTE: Alpha is in radians
					if (alpha == 0.) alpha = 0.001;
					pr = PtransConst * b_field * mNewParticle.R() / ( TMath::Abs(TMath::Sin(0.5*alpha)) );
					phi = fmod(mNewParticle.Phi() +  TMath::RadToDeg() * TMath::Sign( alpha / 2,  fParaMom->MagneticField * static_cast<Float_t>(mMass) ), 360); // calculation of phi (in degrees). The sign in front of alpha/2 is determined by the rotational direction of the cyclotron trajectory, which we derive from the product of B * q
				}
				mNewParticle.Mom().SetRhoPhiZ(pr, phi, n_pz);

				// Before adding the particle, check if we actually calculated momentum values
				
				if ( !TMath::IsNaN(mNewParticle.Mom().X()) && !TMath::IsNaN(mNewParticle.Mom().Y()) && !TMath::IsNaN(mNewParticle.Mom().Z()) && (n_i != fParaMom->NewtonMaxI) )
				{
					// Final scaling of momenta
					scalePall = TMath::Sqrt(fParaMom->scaleEn);
					mNewParticle.Mom().SetXYZ(mNewParticle.Mom().X() * fParaMom->scalePx * scalePall - fParaMom->shiftVx * mMass/mEMASS, mNewParticle.Mom().Y() * fParaMom->scalePy * scalePall - fParaMom->shiftVy * mMass/mEMASS, mNewParticle.Mom().Z() * fParaMom->scalePz * scalePall);
					mNewParticle.Mom().RotateZ(fParaMom->rotatePhi);
					
					// Check energy, momentum, and angle conditions and add new particle to list
					if (fConEnergy->Test(mNewParticle.Energy()) &&
						fConPx->Test(mNewParticle.Mom().X()) &&
						fConPy->Test(mNewParticle.Mom().Y()) &&
						fConPz->Test(mNewParticle.Mom().Z()) &&
						fConAnglePol->Test(mNewParticle.Mom().Angle(mPolarizationAxis)))
					{
						Particles.push_back(mNewParticle);
						mCalculatedParticles.push_back(&Particles.back());
					}
				}
			}
		}
	}

}

/**
 * \brief Fills momentum related histograms
 */
void TRemiCalcProcMomentum::FillMomentumHistograms()
{
	Int_t NumParticles = 0;
	for (std::vector<TRemiParticle*>::iterator PartIt = mCalculatedParticles.begin(), End = mCalculatedParticles.end(); PartIt != End; ++PartIt)
	{
		fPtransX->Fill((*PartIt)->Mom().X());
		fPtransY->Fill((*PartIt)->Mom().Y());
		fPlong->Fill((*PartIt)->Mom().Z());
		fPlongPtrans->Fill((*PartIt)->Mom().Z(), (*PartIt)->Mom().Rho());
		fPxy->Fill((*PartIt)->Mom().X(), (*PartIt)->Mom().Y());
		fEnergy->Fill( (*PartIt)->Energy() );
		fEnergyPlong->Fill( (*PartIt)->Mom().Z(), (*PartIt)->Energy());
		fEnergyPx->Fill( (*PartIt)->Mom().X(), (*PartIt)->Energy());
		fEnergyPy->Fill( (*PartIt)->Mom().Y(), (*PartIt)->Energy());
		fEnergyPhi->Fill( (*PartIt)->Mom().Phi(), (*PartIt)->Energy());
		fEnergyTheta->Fill( (*PartIt)->Mom().Theta(), (*PartIt)->Energy());
		fEnergyCosTheta->Fill( (*PartIt)->Mom().Z()/(*PartIt)->Mom().Length(), (*PartIt)->Energy());
		fPtotalPlong->Fill( (*PartIt)->Mom().Z(), (*PartIt)->Mom().Length());
		fPtotalPx->Fill( (*PartIt)->Mom().X(), (*PartIt)->Mom().Length());
		fPtotalPy->Fill( (*PartIt)->Mom().Y(), (*PartIt)->Mom().Length()); 
		fAnglePolarization->Fill( (*PartIt)->Mom().Angle(mPolarizationAxis) );
		
		fFull3DMomentum->Fill( (*PartIt)->Mom().X(), (*PartIt)->Mom().Y(), (*PartIt)->Mom().Z() );

		// Also fill some check histograms with the scaled and shifted position values
		fRad_alpha->Fill( ((*PartIt)->Tof()  * mNANO * mAbsCharge * TMath::Abs(mGS2T * fParaMom->MagneticField))/TMath::TwoPi() / mMass, (*PartIt)->R() );
		fXY_shifted->Fill( (*PartIt)->X(), (*PartIt)->Y() );
		
		// check histograms for LinCor correction of electron wiggles
		fX_TOF->Fill( (*PartIt)->Tof() , (*PartIt)->X() );
		fY_TOF->Fill( (*PartIt)->Tof() , (*PartIt)->Y() );
		
		// Centering histograms
		fRadius->Fill( (*PartIt)->Length() );
		fXY_ConRad->Fill( (*PartIt)->X(), (*PartIt)->Y() );
		fXY_centering->Fill( (*PartIt)->X(), (*PartIt)->Y() );
		
		
		
		NumParticles++;
	}
	fNumParticles->Fill(NumParticles);
}



ClassImp(TRemiCalcProcMomentum);
