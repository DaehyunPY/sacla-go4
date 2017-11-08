//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Parameter class for delay stage settings
//----------------------------------------------------------------

#ifndef SPARSUNPACK_H
#define SPARSUNPACK_H

#include "TGo4Parameter.h"


/**
 * \brief Parameter class for Delay stage settings
 */
class TRemiDelayParameter : public TGo4Parameter 
{
	
	public:
		TRemiDelayParameter() : TGo4Parameter ("TRemiDelayParameter")	 { Init(); }  
		TRemiDelayParameter(const char* name) : TGo4Parameter (name)	 { Init(); }
		virtual ~TRemiDelayParameter() {}
		
		Float_t Fullrange_raw;	///< full scan range in ADC raw bins
		Float_t	Fullrange_fs;	///< [fs] full scan range in fs
		Float_t	Offset;	///< [fs] Zero point offset
		
	private:
		void Init()
		{
			Fullrange_raw = 1000;
			Fullrange_fs = 1000;
			Offset = 0;
		}
	
	ClassDef(TRemiDelayParameter, 1);
};


#endif





//----------------------------END OF GO4 SOURCE FILE ---------------------


