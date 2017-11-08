//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Pseudo-detector type to processor pure time information
//----------------------------------------------------------------

#ifndef TREMIUNPACKPROCTOFONLY_H
#define TREMIUNPACKPROCTOFONLY_H

#include "TRemiUnpackProcPos.h"

#include "TString.h"

class TRemiUnpackProcTofOnly : public TRemiUnpackProcPos
{

public:
	
    TRemiUnpackProcTofOnly(ConfigFile* UnpackConf, TString particle);
	
    virtual ~TRemiUnpackProcTofOnly() {}
    virtual void CalculatePosition();
	
	ClassDef(TRemiUnpackProcTofOnly, 1);
};

#endif // TREMIUNPACKPROCTOFONLY_H
