//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Describes Output Event of the Finish step which has no real contents
//		It is, however, required by the Go4 Framework
//----------------------------------------------------------------
#ifndef TRemiFDCSEVENT_H
#define TRemiFDCSEVENT_H

#include "../Core/TRemiIncludeFunctions.h"
#include "../Core/TRemiEventElement.h"

class TRemiFinishProc; // This declaration is unavoidable because of a loop inclusion between TRemiFinishProc and TRemiFinishEvent

/**
 * \brief Defines the event produced by the Fin step
 */
class TRemiFinishEvent : public TRemiEventElement
{
	public:
		TRemiFinishEvent();
		TRemiFinishEvent(const char* name) ;
		virtual ~TRemiFinishEvent();
		virtual void  Clear(Option_t *t="");  
	
	private:
	
	ClassDef(TRemiFinishEvent, 1);
};
#endif //TRemiFDCSEVENT_H




//----------------------------END OF GO4 SOURCE FILE ---------------------

