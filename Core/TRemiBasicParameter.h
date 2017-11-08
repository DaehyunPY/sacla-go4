//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This describes a generic one-value parameter object
//----------------------------------------------------------------
#ifndef SPARBASIC_H
#define SPARBASIC_H

#include "TGo4Parameter.h"

/**
 * \brief Basic parameter class for a generic parameter object
 * 
 * This class allows the creation of simple parameter objects in the analysis that contain a single adjustable value. To build such a parameter object use the function MakeBasicParameter() in TRemiIncludeFunctions
 */
class TRemiBasicParameter : public TGo4Parameter 
{
	
	public:
		TRemiBasicParameter();  
		TRemiBasicParameter(const char* name, const char* title); 
		virtual ~TRemiBasicParameter();
		
		Double_t	Value;	///< Value of this parameter
		
	private:
		void Init();
	
	ClassDef(TRemiBasicParameter, 1);
};


#endif //SPAR_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


