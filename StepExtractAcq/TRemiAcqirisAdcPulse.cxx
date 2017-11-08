#include "TRemiAcqirisAdcPulse.h"

TRemiAcqirisAdcPulse& TRemiAcqirisAdcPulse::operator=(const TRemiAcqirisAdcPulse& other)
{
	mTimestamp = other.mTimestamp;
	mAdcValue = other.mAdcValue;
	return *this;
}

bool TRemiAcqirisAdcPulse::operator<(const TRemiAcqirisAdcPulse& other)
{
	return (mTimestamp < other.mTimestamp);
}

