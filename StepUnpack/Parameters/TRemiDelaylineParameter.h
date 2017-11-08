//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This describes the parameters for a two-layer delayline detector
//----------------------------------------------------------------
#ifndef SPARDELAY_H
#define SPARDELAY_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for Delay line detectors with two layers
 */

class TRemiDelaylineParameter : public TGo4Parameter 
{

	public:
		
		TRemiDelaylineParameter() : TGo4Parameter("RemiDelaylineParameter")	{ Init(); }
		TRemiDelaylineParameter(const char* name) : TGo4Parameter(name)	{ Init(); }
		virtual ~TRemiDelaylineParameter() {}
		
		Float_t scaleX; ///< scaling factor for delayline X layer
		Float_t scaleY; ///< scaling factor for delayline Y layer
		Float_t xo; ///< [mm] Defines center of distribution 
		Float_t yo; ///< [mm] Defines center of distribution
		Float_t phi; ///< [degrees] Angle to rotate 
		Float_t size_raw; ///< detector size in elementary pixel
		Float_t size_mm; ///< [mm] real detector size
		
	private:
		/** \brief Setting default values */
		void Init()
		{
			scaleX=1;
			scaleY=1;
			xo=0;
			yo=0;
			phi=0;
			size_raw=5600;
			size_mm=80;
		}

	ClassDef(TRemiDelaylineParameter, 1);
};

#endif //SPARDELAY_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


