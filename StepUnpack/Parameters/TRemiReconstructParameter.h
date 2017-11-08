//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Parameter class for position reconstruction
//----------------------------------------------------------------

#ifndef SPARRECONSTRUCT_H
#define SPARRECONSTRUCT_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for new reconstruction routines working with delayline and hex-anode detectors
 */
class TRemiReconstructParameter : public TGo4Parameter 
{
	
	public:
		TRemiReconstructParameter() : TGo4Parameter("RemiReconstructParameter")	{ Init(); }
		TRemiReconstructParameter(const char* name) : TGo4Parameter(name)	{ Init(); }
		virtual ~TRemiReconstructParameter() {}
		
		Bool_t	useRecoMCP; ///< reconstruct MCP signal if missing?
		Bool_t	useRecoDelayline; ///< reconstruct delayline signals only one is available?
	
	private:
		/** \brief Setting default values */
		void Init()
		{
			useRecoDelayline = kTRUE;
			useRecoMCP = kTRUE;
		}
		
	ClassDef(TRemiReconstructParameter, 1);
};

#endif //SPARRECOD_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


