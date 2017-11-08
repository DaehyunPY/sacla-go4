//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		General base class for Event classes defined in GENERiC
//----------------------------------------------------------------
#ifndef TE2EEVENTELEMENT_H
#define TE2EEVENTELEMENT_H

#include "TGo4EventElement.h"

class TGo4FileSource;

/**
 * \brief User class for analysis event elements. 
 * 
 * Extends Go4's TGo4EventElement by some useful functions. Analysis event elements should extend this class
 */
class TRemiEventElement : public TGo4EventElement
{
	public:
		TRemiEventElement();
		TRemiEventElement(const char* name);
		~TRemiEventElement();
		
		void CheckForParameterMacro();
		
	protected:
	
	ClassDef(TRemiEventElement, 1);
};

#endif
