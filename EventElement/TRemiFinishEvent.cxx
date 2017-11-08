//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Describes Output Event of the Finish step which has no real contents
//		It is, however, required by the Go4 Framework
//----------------------------------------------------------------
#include "TRemiFinishEvent.h"

#include "Riostream.h"


//***********************************************************
TRemiFinishEvent::TRemiFinishEvent() 
  :TRemiEventElement()
{ 
}
//***********************************************************
TRemiFinishEvent::TRemiFinishEvent(const char* name) 
  :TRemiEventElement(name)
{ 
}
//***********************************************************
TRemiFinishEvent::~TRemiFinishEvent() 
{ 
}
//***********************************************************
void TRemiFinishEvent::Clear(Option_t *t)
{ 
}


//----------------------------END OF GO4 SOURCE FILE ---------------------

