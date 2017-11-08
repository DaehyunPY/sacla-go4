//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This describes a generic one-value parameter object
//----------------------------------------------------------------

#include "TRemiBasicParameter.h"


/**
 * \brief Default constructor. Not advisable to be used
 */
TRemiBasicParameter::TRemiBasicParameter() 
: TGo4Parameter("TRemiBasicParameter")
{ 
	Init();
}

/**
 * \brief Principal constructor creating parameter object with name and title
 */
TRemiBasicParameter::TRemiBasicParameter(const char* name, const char* title) 
: TGo4Parameter(name, title)
{ 
	Init();
}

TRemiBasicParameter::~TRemiBasicParameter() 
{ 
}

/**
 * \brief Initialise values
 */
void TRemiBasicParameter::Init()
{
	Value = 0;
}

ClassImp(TRemiBasicParameter);




//----------------------------END OF GO4 SOURCE FILE ---------------------


