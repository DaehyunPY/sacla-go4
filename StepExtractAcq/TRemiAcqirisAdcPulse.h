#ifndef __TRemiAcqirisAdcPulse_H_
#define __TRemiAcqirisAdcPulse_H_

#include "Rtypes.h"

/** \name Stores the output of the software ADC working on a single pulse */
class TRemiAcqirisAdcPulse
{
	public:
		TRemiAcqirisAdcPulse(): mTimestamp(0), mAdcValue(0) {}
		TRemiAcqirisAdcPulse(const TRemiAcqirisAdcPulse &other) : mTimestamp(other.mTimestamp), mAdcValue(other.mAdcValue) {}
		virtual ~TRemiAcqirisAdcPulse() {}
		
		TRemiAcqirisAdcPulse&	operator= (const TRemiAcqirisAdcPulse &other);
		bool	operator< (const TRemiAcqirisAdcPulse &other);
		
		void	setTimestamp(Double_t in)	{ mTimestamp = in; }
		Double_t	getTimestamp() const	{ return mTimestamp; }
		
		void	setAdcValue(Double_t in)	{ mAdcValue = in; }
		Double_t	getAdcValue() const	{ return mAdcValue; }
	
	private:
		Double_t mTimestamp;
		Double_t mAdcValue;
		
	ClassDef(TRemiAcqirisAdcPulse, 0);
};

#endif