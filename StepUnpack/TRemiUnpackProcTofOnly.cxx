#include "TRemiUnpackProcTofOnly.h"

#include "../Core/Extern/ConfigFile.h"
#include "../Core/Point/TRemiDetectorHit.h"
#include "../EventElement/TRemiExtractEvent.h"

/**
 * \brief Constructor using particle name and Config File
 */
TRemiUnpackProcTofOnly::TRemiUnpackProcTofOnly(ConfigFile* UnpackConf, TString particle): TRemiUnpackProcPos(UnpackConf, particle)
{

}


/**
 * \brief "Position" calculation
 * 
 * This function actually does not calculate a position, but only a time of flight. However, it appears to the rest of the prorgam that a position was calculated. In result, a new DetectorHit will be created
 */
void TRemiUnpackProcTofOnly::CalculatePosition()
{
	mDetectorHits.clear();
	
	// Create references to input vector
	std::vector<Float_t> &VectorMcp = fRawData->fiTDC[mTdcChanMCP];

	std::vector<Float_t>::iterator McpIt = VectorMcp.begin();
	while (McpIt != VectorMcp.end())
	{
		TRemiDetectorHit newhit = TRemiDetectorHit(CalculateSingleTof(*McpIt), 0., 0.);
		newhit.SetIndex(mDetectorHits.size());
		mDetectorHits.push_back(newhit);
		++McpIt;
	}
	return;
}

