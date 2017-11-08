//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Describes Object to contain extracted Raw data
//----------------------------------------------------------------
#include "TRemiExtractEvent.h"


/**
 * \brief Default constructor. 
 */
TRemiExtractEvent::TRemiExtractEvent()
: TRemiEventElement(),
fiADC(DEFAULT_NUM_ADC_CHAN),
fiTDC(DEFAULT_NUM_TDC_CHAN)
{
}

/**
 * \brief Principal constructor with object name
 */
TRemiExtractEvent::TRemiExtractEvent(const char* name)
:TRemiEventElement(name),
fiADC(DEFAULT_NUM_ADC_CHAN),
fiTDC(DEFAULT_NUM_TDC_CHAN)
{
}

TRemiExtractEvent::~TRemiExtractEvent()
{
}

/** \brief Clear event data */
void TRemiExtractEvent::Clear()
{
	for (UInt_t i=0; i<fiADC.size(); i++)
		fiADC[i] = -1;
	for (UInt_t i=0; i<fiTDC.size(); i++)
		fiTDC[i].clear();
}

/** 
* \brief Check arrays to sizes required by step processors
* \param numAdcChan Number of ADC channels required
* \param numTdcChan Number of TDC channels required
*/
void TRemiExtractEvent::CheckSizes ( Int_t numAdcChan, Int_t numTdcChan )
{
	fiADC.resize(numAdcChan);
	fiTDC.resize(numTdcChan);
}

/**
* \brief Adds a TDC hit to the list for one channel
* \param TdcChannel No. of channel where the hit was recorded
* \param data Time of the hit
*/
void TRemiExtractEvent::AddTdcHit(Int_t TdcChannel, Float_t data)
{
	fiTDC[TdcChannel].push_back(data);
}



