//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Parameter class for the position calculation for Hexanodes with a central hole
//----------------------------------------------------------------
#ifndef SPARDELAYH_H
#define SPARDELAYH_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for Delay line detectors with hole
 */

class TRemiHexAnodeHoleParameter : public TGo4Parameter
{

	public:
		TRemiHexAnodeHoleParameter() : TGo4Parameter("RemiHexAnodeHoleParmeter")	{ Init(); }  
		TRemiHexAnodeHoleParameter(const char* name) : TGo4Parameter(name)	{ Init(); }
		virtual ~TRemiHexAnodeHoleParameter() {}
  
		Float_t U_WIDTH; ///< [mm] width of U layer
		Float_t U_HOLE_LEFT; ///< [mm] left edge of hole in U layer
		Float_t U_HOLE_RIGHT; ///< [mm] right edge of hole in U layer
		Float_t V_WIDTH; ///< [mm] width of V layer
		Float_t V_HOLE_LEFT; ///< [mm] left edge of hole in V layer
		Float_t V_HOLE_RIGHT; ///< [mm] right edge of hole in V layer
		Float_t W_WIDTH; ///< [mm] width of W layer
		Float_t W_HOLE_LEFT; ///< [mm] left edge of hole in W layer
		Float_t W_HOLE_RIGHT; ///< [mm] right edge of hole in W layer
		Float_t edge_remove; ///< [mm] artficial dead region along the gaps
		Float_t distancemax; ///< [mm] maximum distance between two points of one layer
		Int_t Prio_uv; ///< Priority of the uv position 
		Int_t Prio_uw; ///< Priority of the uw position
		Int_t Prio_vw; ///< Priority of the vw position
  

	private:
		/** \brief Setting default values */
		void Init()
		{
			U_WIDTH=79;
			U_HOLE_LEFT=-2.7;
			U_HOLE_RIGHT=4.9;
			V_WIDTH=80.6;
			V_HOLE_LEFT=-3.95;
			V_HOLE_RIGHT=3.95;
			W_WIDTH=80.6;
			W_HOLE_LEFT=-3;
			W_HOLE_RIGHT=5;
			edge_remove=3;
			distancemax=1;
			Prio_uv=3;
			Prio_uw=1;
			Prio_vw=2;
		}

ClassDef(TRemiHexAnodeHoleParameter, 1);
};

#endif //SPARDELAY_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


