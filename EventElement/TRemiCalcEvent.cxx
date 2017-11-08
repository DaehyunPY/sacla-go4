//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class describes the interface between the Calc and the finish step
//----------------------------------------------------------------
#include "TRemiCalcEvent.h"



/** \brief Default constructor */
TRemiCalcEvent::TRemiCalcEvent() 
  :TRemiEventElement(),
  fParticles(),
  fCoincidences(),
  fDelayValue(0)
{ 
}

/** \brief Constructor with object name */
TRemiCalcEvent::TRemiCalcEvent(const char* name) 
  :TRemiEventElement(name),
  fParticles(),
  fCoincidences(),
  fDelayValue(0)
{ 
}
//***********************************************************
TRemiCalcEvent::~TRemiCalcEvent() 
{ 
}

/**
* \brief Clear event contents
*/
void TRemiCalcEvent::Clear(Option_t *t)
{ 
	fDelayValue = 0;
	fParticles.clear();
	fCoincidences.clear();
}

/** 
* \brief Check sizes of data arrays
* \param numPart Number of particles to store
*/
void TRemiCalcEvent::CheckSizes ( UInt_t numPart, UInt_t numCoinc)
{
	fParticles.resize(numPart);
	fCoincidences.resize(numCoinc);
}

/**
* \brief Add a particle to the event
* \param NewPart TRemiParticle object to add
*/
void TRemiCalcEvent::AddParticle(const TRemiParticle& NewPart)
{
	fParticles.push_back(NewPart);
}

/**
* \brief Add a coincidence to the event
* \param NewCoinc TRemiCoincidence object to add
*/
void TRemiCalcEvent::AddCoincidence(const TRemiCoincidence& NewCoinc)
{
	fCoincidences.push_back(NewCoinc);
}





//----------------------------END OF GO4 SOURCE FILE ---------------------

