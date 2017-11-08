//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Global Parameter class for the Unpack step
//----------------------------------------------------------------

#ifndef SPARUNPACK_H
#define SPARUNPACK_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for global Unpack step settings
 */
class TRemiUnpackParameter : public TGo4Parameter 
{
	
	public:
		TRemiUnpackParameter() : TGo4Parameter("RemiUnpackParameter")	{ Init(); }
		TRemiUnpackParameter(const char* name) : TGo4Parameter(name)	{ Init(); }
		virtual ~TRemiUnpackParameter() {}
		
		Double_t	to;	///< [ns] Global time origin. This is substracted from all Time-of-flights
		
	private:
		/** \brief Setting default values */
		void Init()
		{
			to = 0.;
		}
	
	ClassDef(TRemiUnpackParameter, 1);
};


#endif //SPAR_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


