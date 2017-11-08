//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Global parameter class for te Finish step
//----------------------------------------------------------------
#ifndef SPARFDCS_H
#define SPARFDCS_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for the Finish step
 *
 * It is rather empty at the beginning. You can add properties here as you modify the Finish step to ypur needs.
 */
class TRemiFinishParameter : public TGo4Parameter
{

	public:
		TRemiFinishParameter() : TGo4Parameter("RemiFinishParameter")	{ Init(); }
		TRemiFinishParameter(const char* name) : TGo4Parameter(name)	{ Init(); }
		virtual ~TRemiFinishParameter() {}

		// This is where you can add more parameters. They are just defined in the following manner:
		// Type	name	///< Description

		Float_t	P_Proj;	///< [a.u.] Momentum of the projectile
		
	private:
		/** \brief Setting default values */
		void Init()
		{
			P_Proj = 0.;
		}
		
	ClassDef(TRemiFinishParameter, 1);
};

#endif //SPARFDCS_H








