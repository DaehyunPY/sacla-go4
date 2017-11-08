#include "TRemiRollingGraph.h"

#include "TMath.h"

#include <cstring>

/** \brief Default constructor
*
* Creates a rolling graph with an update interval of 1, i.e. no averaging of the input values
* Graph will have no size, either. Without setting size, nothing will be drawn!
*/
TRemiRollingGraph::TRemiRollingGraph() : 
TGraphErrors(),
mNumPoints(0),
mUpdateInterval(1),
mCounter(0),
mSum(0.)
{
}

/** \brief Constructor with size and update interval
* \param NumPoints Size of the graph, i.e. number of points
* \param UpdateInterval No. of calls to the Fill() routine after which a new bin will started to be filled. For UpdateInterval=1 every value will be displayed directly on the rolling graph, for values > 1 an average will be shown
*/
TRemiRollingGraph::TRemiRollingGraph(Int_t NumPoints, Int_t UpdateInterval): 
TGraphErrors(),
mNumPoints(NumPoints),
mUpdateInterval(UpdateInterval),
mCounter(0),
mSum(0.)
{
}

/** \brief Destructor */
TRemiRollingGraph::~TRemiRollingGraph()
{
}

/** \brief Fills a new value into the rolling graph
* \param value The value to fill
*
* If the Update Interval is larger than one, the value will not be displayed directly in the graph but added to the averaging
*/
void TRemiRollingGraph::Fill(Double_t value, Double_t xerror, Double_t yerror)
{
	if (mNumPoints)
	{
		Int_t NumAverage = mCounter % mUpdateInterval + 1;
		// Create a new bin and reset internal value for averaging if counter is a multiple of mUpdateInterval
		if (NumAverage == 1)
		{
			NextBin();
			mSum = 0.;
		}

		// Add value for averaging
		mSum += value;
		mSumAveraged = mSum / static_cast<Double_t>(NumAverage);

		// Draw new averaged value
		SetPoint(fNpoints-1, static_cast<Double_t>(mCounter / mUpdateInterval), mSumAveraged);
		if (xerror < 0)
			xerror = 0.5;
		if (yerror < 0)
			yerror = mSumAveraged / TMath::Sqrt(NumAverage);
		SetPointError(fNpoints-1, xerror, yerror);
	}
	mCounter++;
}

/** \brief Create a new bin 
*
* Internal function that implements the "rolling" of the graph: The first bin is deleted while a new one is appended at the end
*/
void TRemiRollingGraph::NextBin()
{
	if (mNumPoints)
	{
		if (mNumPoints <= fNpoints)
		{
			// Move points to the left
			memmove (&fX[0],  &fX[1],  (fNpoints-1)*sizeof(Double_t));
			memmove (&fEX[0], &fEX[1], (fNpoints-1)*sizeof(Double_t));
			memmove (&fY[0],  &fY[1],  (fNpoints-1)*sizeof(Double_t));
			memmove (&fEY[0], &fEY[1], (fNpoints-1)*sizeof(Double_t));
		}
		else
		{
			// Create a new point
			Set(fNpoints+1);
		}
	}
}

/** \brief Clear all points
*
* Remove all points from the graph, set number of points to zero, and reset internal value for averaging
*/
void TRemiRollingGraph::Clear()
{
	for (Int_t i=0; i<mNumPoints; ++i)
		RemovePoint(i);

	Set(0);
	mSum = 0.;
}


ClassImp(TRemiRollingGraph);
