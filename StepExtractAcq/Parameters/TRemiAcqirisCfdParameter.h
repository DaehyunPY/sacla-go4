//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This describes the parameter object for a CFD channel
//----------------------------------------------------------------
#ifndef SPARACQIRISCFD_H
#define SPARACQIRISCFD_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for a CFD channel
 */
class TRemiAcqirisCfdParameter : public TGo4Parameter 
{
	
	public:
		TRemiAcqirisCfdParameter() : TGo4Parameter("RemiAcqirisCfdParameter")	{ Init(); }
		TRemiAcqirisCfdParameter(const char* name) : TGo4Parameter(name)	{ Init(); }
		virtual ~TRemiAcqirisCfdParameter()	{}
		
		Float_t	Threshold;	///< CFD threshold
		Int_t	Delay;	///< [Samples] CFD internal delay
		Float_t	Fraction;	///< CFD fraction
		Float_t	Walk;	///< CFD walk
		
	private:
		/** \brief Setting default values */
		void Init()
		{
			Threshold = 30.;
			Delay = 4.;
			Fraction = .5;
			Walk = 0;
		}
	
	ClassDef(TRemiAcqirisCfdParameter, 1);
};


#endif //SPAR_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


