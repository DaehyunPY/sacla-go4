//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class defines the interface object between the Unpack and the Calculate step
//----------------------------------------------------------------

#include "TRemiUnpackEvent.h"


/** \brief Default constructor */
TRemiUnpackEvent::TRemiUnpackEvent() 
  :TRemiEventElement(),
  fiIonHits(),
  fiElectronHits(),
  fiDelay(0),
  fiIntensity(0),
  fiMinibunchIndex(0)
{ 
}

/** \brief Constructor with object name */
TRemiUnpackEvent::TRemiUnpackEvent(const char* name) 
  :TRemiEventElement(name),
  fiIonHits(),
  fiElectronHits(),
  fiDelay(0),
  fiIntensity(0),
  fiMinibunchIndex(0)
{ 
}
//***********************************************************
TRemiUnpackEvent::~TRemiUnpackEvent() 
{ 
}
//***********************************************************

/**
* \brief Checks if internal arrays are large enough for communication with step processors
* \param numElec Required electron number
* \param numIon Required ion number
*
*/
void TRemiUnpackEvent::CheckSizes(Int_t numElec , Int_t numIon )
{
	fiElectronHits.resize(numElec);
	fiIonHits.resize(numIon);
}


/**
 * \brief Clear event contents
 */
void  TRemiUnpackEvent::Clear(Option_t *t)
{ 
	fiDelay = 0;
	fiIntensity = 0;
	fiMinibunchIndex = 0;
	fiIonHits.clear();
	fiElectronHits.clear();
}

/**
 * \brief Adds ion detector hit to event
 * \param IonHit TRemiDetectorhit object containing the Hit
 */
void TRemiUnpackEvent::AddIon(const TRemiDetectorHit& IonHit)
{
	fiIonHits.push_back(IonHit);
}


/**
* \brief Adds electron detector hit to event
* \param IonHit TRemiDetectorhit object containing the Hit
*/
void TRemiUnpackEvent::AddElectron(const TRemiDetectorHit& ElectronHit)
{
	fiElectronHits.push_back(ElectronHit);
}



//----------------------------END OF GO4 SOURCE FILE ---------------------

