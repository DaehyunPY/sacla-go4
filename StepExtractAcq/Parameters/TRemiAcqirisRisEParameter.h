//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This describes a parameter object for the risng-edge Soft TDC method
//----------------------------------------------------------------
#ifndef SPARRISE_H
#define SPARRISE_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for the Rising edge TDC method
 */
class TRemiAcqirisRisEParameter : public TGo4Parameter 
{
	
	public:
		TRemiAcqirisRisEParameter(): TGo4Parameter("TRemiAcqirisRisEParameter")	{ Init(); }
		TRemiAcqirisRisEParameter(const char* name): TGo4Parameter(name) { Init(); }
		virtual ~TRemiAcqirisRisEParameter() {}
		
		Short_t	MinPointsRising;	///< Minimum no. of points to define a rising edge
		Short_t	MinPointsFalling;	///< Minimum no. of points to define a falling edge
		Short_t	MaxPointsBetween;	///< Maximum no. of points allowed between a rising and a falling edge
		Float_t	Fraction;	///< Fraction of peak height at which the trigger should come
		Short_t Noise;	///< Manually defined noise level if positive
		
	private:
		/** \brief Setting default values */
		void Init()
		{
			
			MinPointsRising = 5;
			MinPointsFalling = 4;
			MaxPointsBetween = 20;
			Fraction = .5;
			Noise = 0;
		}
		

	
	ClassDef(TRemiAcqirisRisEParameter, 1);
};



#endif //SPAR_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


