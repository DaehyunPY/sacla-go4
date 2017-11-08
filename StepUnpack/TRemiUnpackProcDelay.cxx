//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class for handling the delay stage
//----------------------------------------------------------------

#include "TRemiUnpackProcDelay.h"

#include "Parameters/TRemiDelayParameter.h"
#include "../EventElement/TRemiExtractEvent.h"

#include "TRandom3.h"
#include "TH1.h"

/**
 * \brief Default constructor. Not to be used
 */
TRemiUnpackProcDelay::TRemiUnpackProcDelay()
{
}

/**
 * \brief Principal constructor using input from config file
 * \param ConfName filename of Config file
 */
TRemiUnpackProcDelay::TRemiUnpackProcDelay(ConfigFile* UnpackConf) :
	TRemiIncludeFunctions(UnpackConf)
{
	// Read information from Config file
	mAdcChanDelay = fConfigFile->read("unp_adc_chan_delay", 0);
	
	mUseRandomizer = fConfigFile->read("unp_delay_use_randomizer", false);
	mRandomizingRange = fConfigFile->read("unp_delay_randomizing_range", 1.);
	
	Int_t randomizerSeed = fConfigFile->read("unp_delay_randomizer_seed", 0);
	fRandom = TRandom3(randomizerSeed);
	std::cout << "unp_delay_randomizer_seed: " << randomizerSeed << std::endl;
	
	// Create parameter
	fParaDelay = MakeParam<TRemiDelayParameter>("Par Delay", "Unpack");
	
	// Create Delay histograms
	fHistDelay = MakeH1<TH1F>("DelayValue", "Delay after calibration", "unp_hist_delay_bins", "unp_hist_delay_min", "unp_hist_delay_max", "Unpack/Delay", "Delay [fs]");
}

/**
 * \brief Sets input event
 */
void TRemiUnpackProcDelay::SetInput(TRemiExtractEvent* Input)
{
	fInput = Input;
}

/** 
 * \brief Calculates delay from raw value
 */
void TRemiUnpackProcDelay::CalculateDelay()
{
	mDelayValue = ( fInput->fiADC[mAdcChanDelay] + (mUseRandomizer ? (fRandom.Rndm()-.5)*mRandomizingRange : 0.) ) * fParaDelay->Fullrange_fs / fParaDelay->Fullrange_raw - fParaDelay->Offset;
	fHistDelay->Fill(mDelayValue);
}

/**
 * \brief Returns AC delay value
 */
Float_t TRemiUnpackProcDelay::GetDelay()
{
	return mDelayValue;
}

ClassImp(TRemiUnpackProcDelay);
