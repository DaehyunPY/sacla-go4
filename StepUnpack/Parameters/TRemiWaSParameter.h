//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Parameter class for Wedge and Strip detectors
//----------------------------------------------------------------

#ifndef SPARWAS_H
#define SPARWAS_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for a wedge and strip detector
 */
class TRemiWaSParameter : public TGo4Parameter 
{
	
	public:
		TRemiWaSParameter() : TGo4Parameter("TRemiWaSParameter")	{ Init(); }  
		TRemiWaSParameter(const char* name): TGo4Parameter(name)	{ Init(); } 
		virtual ~TRemiWaSParameter() {}
		
		Float_t ADCscale[3]; ///< Scaling factors for the electrodes Wedge, Strip and Meander
		Float_t scalex; ///< Scales X coordinate
		Float_t scaley; ///< Scales Y coordinate
		Float_t phi; ///< Angle to rotate image
		Float_t xo; ///< X origin
		Float_t yo; ///< Y origin
		Float_t size_mm; ///< [mm] real detector size
		Float_t size_raw; ///< pixel size
		Bool_t useCR; ///< Wether to use cross-talk reconstruction
		Float_t mw; ///< cross-talk strength factor meander-wedge 
		Float_t ms; ///< cross-talk strength factor meander-strip
		Float_t ws; ///< cross-talk strength factor wedge-strip
		
	private:
		/** \brief Set default values */
		void Init()
		{
			ADCscale[0]=1;
			ADCscale[1]=1;
			ADCscale[2]=1;
			scalex=1;
			scaley=1;
			phi = 85.000000;
			xo = 125;
			yo = 124;
			size_mm = 80.000000;
			size_raw = 0.200000;
			useCR = 1;
			mw = 0.095000;
			ms = 0.095000;
			ws = 0.040000;
		}
		
	
	ClassDef(TRemiWaSParameter, 1);
};


#endif //SPAR_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


