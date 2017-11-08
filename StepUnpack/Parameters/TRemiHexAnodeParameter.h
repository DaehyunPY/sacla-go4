//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Parameter class for the position calculation for Hexanodes
//----------------------------------------------------------------
#ifndef SPARDELAYHEX_H
#define SPARDELAYHEX_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for hexagonal Delay line detectors (Hex Anodes) with three layers
 */

class TRemiHexAnodeParameter : public TGo4Parameter 
{

	public:
		
		TRemiHexAnodeParameter() : TGo4Parameter("RemiHexAnodeParmeter")	{ Init(); }
		TRemiHexAnodeParameter(const char* name) : TGo4Parameter(name)	{ Init(); }
		virtual ~TRemiHexAnodeParameter() {}
		
		Float_t scaleU; ///< scaling factor for delayline U layer
		Float_t scaleV; ///< scaling factor for delayline V layer
		Float_t scaleW; ///< scaling factor for delayline W layer
		Float_t shiftW; ///< [mm] offset of W layer (is only necessary for one layer, see delayline manual by A. Czasch)
		Float_t distancemax; ///< [mm] maximum tolerance for distance between good points
		Float_t xo; ///< [mm] Defines center of distribution 
		Float_t yo; ///< [mm] Defines center of distribution
		Float_t phi; ///< [degrees] Angle to rotate 
		Float_t size_raw; ///< detector size in TDC channels
		Float_t size_mm; ///< [mm] real detector size
		
	private:
		/** \brief Setting default values */
		void Init()
		{
			scaleU=1;
			scaleV=0.97;
			scaleW=0.95;
			shiftW=-2;
			distancemax=1;
			xo=0;
			yo=0;
			phi=0;
			size_raw=9850;
			size_mm=80;
		}

	ClassDef(TRemiHexAnodeParameter, 1);
};

#endif //SPARDELAY_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


