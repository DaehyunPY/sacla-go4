//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Position-calculating class for Wedge-and-strip detectors
//----------------------------------------------------------------

#ifndef SWASANODE_W
#define SWASANODE_W

#include "TRemiUnpackProcPos.h"

class TRemiWaSParameter;
class ConfigFile;

class TString;
class TH1I;

/**
 * \brief Extended class to get event information from single detector
 * 
 * Provides method for position calculation and inherits method for time-of-flight calculation from class TRemiUnpackProcTof
 */

class TRemiUnpackProcWaSAnode : public TRemiUnpackProcPos
{
	public :
		TRemiUnpackProcWaSAnode(); 
		TRemiUnpackProcWaSAnode(ConfigFile* UnpackConf, TString particle);
		virtual ~TRemiUnpackProcWaSAnode();
		void CalculatePosition();
		
		
	protected :
		void CrossTalkComp (Float_t &wedge, Float_t &strip, Float_t &meander);
		
		Int_t mAdcChanWe; ///< ADC-Channel for wedge electrode
		Int_t mAdcChanSt; ///< ADC-Channel for strip electrode
		Int_t mAdcChanMe; ///< ADC-Channel for meander electrode
		
		TRemiWaSParameter* fParaPos; /**< User parameter for the position calculation */
		
		TH1I*	fXr;	///< X coordinate spectrum
		TH1I*	fYr;	///< Y coordinate spectrum
		
		TH1I*	fPattern;	///< Signal pattern
		
	ClassDef (TRemiUnpackProcWaSAnode, 1);
};

#endif
