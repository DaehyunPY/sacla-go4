//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class for handling the delay stage
//----------------------------------------------------------------

#ifndef TREMIUNPACKPROCDELAY_H
#define TREMIUNPACKPROCDELAY_H

#include "../Core/TRemiIncludeFunctions.h"

#include "TRandom3.h"

#include <vector>

class TRemiDelayParameter;
class TRemiExtractEvent;

class ConfigFile;

class TH1F;

class TRemiUnpackProcDelay : public TRemiIncludeFunctions
{
	public :
		TRemiUnpackProcDelay();
		TRemiUnpackProcDelay(ConfigFile* UnpackConf);
		virtual ~TRemiUnpackProcDelay() {}
		
		void	SetInput(TRemiExtractEvent* Input);
		void	CalculateDelay();
		Float_t	GetDelay();
		
		
	protected :
		TRemiExtractEvent*	fInput;	 ///< Input event
		Float_t	mDelayValue;	///< Value of the delay in femtoseconds
		
		Int_t	mAdcChanDelay;	///< ADC channel of the delay stage
		
		TRemiDelayParameter*	fParaDelay;	///< Parameter Object
		
		TH1F*	fHistDelay;	///< Histogram of the delay without any conditions
		
		TRandom3	fRandom;	///< Randomizer
		Bool_t	mUseRandomizer;	///< Flag if randomizer is used
		Float_t	mRandomizingRange;	///< Range of the random number added to input
		
	ClassDef (TRemiUnpackProcDelay, 1);
};

#endif
