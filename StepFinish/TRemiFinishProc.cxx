//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the main processing file for the Finish step
//----------------------------------------------------------------
#include "TRemiFinishProc.h"

#include "Parameters/TRemiFinishParameter.h"
#include "../EventElement/TRemiFinishEvent.h"
#include "../EventElement/TRemiCalcEvent.h"

#include "TH1.h"
#include "TClass.h"

#include "Riostream.h"


//***********************************************************
/**
 * \brief Principal constructor, is used in TRemiFinishFact
 */
TRemiFinishProc::TRemiFinishProc(const char* name)
		:TGo4EventProcessor(name), TRemiIncludeFunctions(FINISHCONFIG)
{
	TString hname, htitle, folder, basefolder = "Fin";
	
	std::cout << "**** TRemiFinishProc: Create" << std::endl;
	
	//---------------------------------------//
	// Initialise constants from Config file //
	//---------------------------------------//
	
	
	// here you can read any other property from the Finish.conf config file
	
	std::cout << "**** TRemiFinishProc: Loaded configuration settings" << std::endl;
	
	
	//------------------------//
	// Creation of parameters //
	//------------------------//
	
	fParaFinish = MakeParam<TRemiFinishParameter> ("Par finish", "Finish");
	std::cout << "**** TRemiFinishProc: Created parameter object 'Par finish'" << std::endl;
	
	//------------------------//
	// Creation of Histograms //
	//------------------------//
	
	folder = basefolder + "/numbers";
	
	fHistNumParticles = MakeH1<TH1I>("NumParticles", "Number of particles per Event", "fin_hist_numbers_max", "null", "fin_hist_numbers_max", folder);
	fHistNumCoincidences = MakeH1<TH1I>("NumCoincidences", "Number of coincidences per Event", "fin_hist_numbers_max", "null", "fin_hist_numbers_max", folder);
	
	folder = basefolder + "/asym";	
	fHistAsymEleEnergy = MakeH1<TH1F>("AsymEleEnergy", "Electron asymmetry as function of energy", "fin_hist_electron_energy_bins", "fin_hist_electron_energy_min", "fin_hist_electron_energy_max", folder, "Energy (eV)");
	
	// create any histograms here. Please not that you have to define them in the .h file!
	
	
	std::cout << "**** TRemiFinishProc: Created Histograms" << std::endl;
	
	// ---------------------- //
	// Creation of conditions //
	// ---------------------- //
	
	// Create any conditions here. Please not that you have to define them in the .h file!
	
	std::cout << "**** TRemiFinishProc: Created Conditions" << std::endl;
	
	// Enable rescaling of Fill 'n' Go histograms
	EnableRescaleFillToHist();
	
}
//***********************************************************
TRemiFinishProc::TRemiFinishProc()
        : TGo4EventProcessor("RemiFinishishProc"),fInput(0)
{
}
//***********************************************************
TRemiFinishProc::~TRemiFinishProc()
{
}
//***********************************************************

/**
 * \brief Check Event Class
 */
Bool_t TRemiFinishProc::CheckEventClass(TClass* fl)
{
   return fl->InheritsFrom(TRemiFinishEvent::Class());
}

/**
 * \brief Event processor
 *
 * This functions is called once for every event
 */
Bool_t TRemiFinishProc::BuildEvent(TGo4EventElement* dest)
{
	Bool_t valid=0; // Have a valid output event?

//	fInput  = (TRemiCalcEvent*) GetInputEvent();
	fInput  = dynamic_cast<TRemiCalcEvent*> (GetInputEvent());
	fOutput = dynamic_cast<TRemiFinishEvent*> (dest);

	// Plotting No. of particles and coincidences
	fHistNumParticles->Fill(fInput->fParticles.size());
	fHistNumCoincidences->Fill(fInput->fCoincidences.size());
	
	if (fInput->IsValid())
	{
		UInt_t i;
		
		//-----------------------------------------------------------------//
		// Loop through all non-coincident particles passed from Calc step //
		//-----------------------------------------------------------------//
		
		for (i=0; i<fInput->fParticles.size(); i++)
		{
			FillTo1DHist(fInput->fParticles[i].Mom().Z(), "Plong", "Test plot: longitudinal momentum", 100, -5, 5, "Fin/Particles");
		}

		
		//-------------------------------------------------------------//
		// Loop through all coincident particles passed from Calc step //
		//-------------------------------------------------------------//
		
		for (i=0; i<fInput->fCoincidences.size(); i++)
		{
			FillTo2DHist(fInput->fCoincidences[i].GetParticle(0).Mom().Z(), fInput->fCoincidences[i].GetParticle(1).Mom().Z(), "PlongPlong", "Test plot: longitudinal momentum correlation", 100, -5, 5, 100, -5, 5, "Fin/Coinc" );
			
			// Plot electron asymmetry as a function of energy
			fHistAsymEleEnergy->Fill( fInput->fCoincidences[i].GetParticle(1).Energy(), TMath::Sign(static_cast<Float_t>(1.), fInput->fCoincidences[i].GetParticle(1).Mom().Z()) );
		}

		fOutput->SetValid(kTRUE);       // events are not stored until kTRUE is set
		
	} //  if (fInput->IsValid())
	
	// Disable rescaling of Fill 'n' Go histograms
	DisableRescaleFillToHist();
	
	return valid;
} // Finish Processor

