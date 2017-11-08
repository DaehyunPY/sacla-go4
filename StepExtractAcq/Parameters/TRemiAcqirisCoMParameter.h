//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This describes the parameter object for a CFD channel
//----------------------------------------------------------------
#ifndef SPARACQIRISCOM_H
#define SPARACQIRISCOM_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for a CFD channel
 */
class TRemiAcqirisCoMParameter : public TGo4Parameter 
{
	
	public:
		TRemiAcqirisCoMParameter() : TGo4Parameter("RemiAcqirisCfdParameter")	{ Init(); }
		TRemiAcqirisCoMParameter(const char* name) : TGo4Parameter(name)	{ Init(); }
		virtual ~TRemiAcqirisCoMParameter()	{}
		
		Float_t	Threshold;	///< Threshold for peak
		
	private:
		/** \brief Setting default values */
		void Init()
		{
			Threshold = 30.;
		}
	
	ClassDef(TRemiAcqirisCoMParameter, 1);
};


#endif //SPAR_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


