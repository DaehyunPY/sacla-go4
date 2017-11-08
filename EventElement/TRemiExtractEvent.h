//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Describes Object to contain extracted Raw data
//----------------------------------------------------------------
#ifndef TREMIEXTRACTEVENT_H
#define TREMIEXTRACTEVENT_H

#include "../Core/TRemiEventElement.h"
#include "../Core/TRemiIncludeFunctions.h"

#include "TArrayS.h"
#include "TArrayF.h"

/**
 * \brief Object class for TDC/ADC raw data
 */
class TRemiExtractEvent : public TRemiEventElement
{
	public:
		TRemiExtractEvent();
		TRemiExtractEvent(const char* name);
	
		~TRemiExtractEvent();
		
		void Clear();
		void CheckSizes(Int_t numAdcChan, Int_t numTdcChan);
		
		void AddTdcHit(Int_t TdcChannel, Float_t data);
		
		std::vector<Float_t>	 fiADC;	///< contents of the ADC channels. Size of the array given by value num_adc_chan in configuration file
		std::vector< std::vector<Float_t> >	fiTDC;	//[fiNumTdcChan] Array of fiNumTdcChan TArrayF arrays
		
	private:
		
	ClassDef(TRemiExtractEvent, 1);
};


#endif
