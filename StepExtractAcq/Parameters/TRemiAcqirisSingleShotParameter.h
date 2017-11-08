//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This describes the parameter object for the extraction of a single shot event
//----------------------------------------------------------------
#ifndef SPARACQIRISSINGLESHOT_H
#define SPARACQIRISSINGLESHOT_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for retrieval of single shot events
 */
class TRemiAcqirisSingleShotParameter : public TGo4Parameter 
{
	
	public:
		TRemiAcqirisSingleShotParameter() : TGo4Parameter("RemiAcqirisSingleShotParameter")	{ Init(); }  
		TRemiAcqirisSingleShotParameter(const char* name) : TGo4Parameter(name)	{ Init(); }
		virtual ~TRemiAcqirisSingleShotParameter()	{}
		
		Int_t	NumMinibunches;	///< Number of minibunches in the puls train
		Double_t	MinibunchDistance;	///< [ns] Time between two mininbunches
		Double_t	Offset;	///< [ns] Offset of the first minibunch with respect to the start of acqisition window
		Double_t	Overlap;	///< [ns] Range to include from the following minibunch window
		
	private:
		/** \brief Setting default values */
		void Init()
		{
			NumMinibunches = 1;
			MinibunchDistance = 5000.;
			Offset = 0.;
			Overlap = 0.;
		}
	
	ClassDef(TRemiAcqirisSingleShotParameter, 1);
};


#endif //SPAR_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


