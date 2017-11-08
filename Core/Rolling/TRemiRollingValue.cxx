#include "TRemiRollingValue.h"

#include "TMath.h"
#include "TGo4WinCond.h"
#include "../RootMod/TRemiRollingGraph.h"

#include "TRemiRollingParameter.h"


const TString TRemiRollingValue::prefix_condi = "Value Condition: ";
const TString TRemiRollingValue::prefix_graph = "Rolling Value: ";

const TString TRemiRollingValue::suffix_condi = "_value";
const TString TRemiRollingValue::suffix_graph = "_rollingGraph";
const TString TRemiRollingValue::suffix_param = "_parameters";



/**
 * \brief Default Constructor
 * \param name Name, used for the Condition, the RollingGraph, and the Parameters with respective suffixes
 * \param sourceHist Source histogram for the Condition, used to generate the titles of the Condition and the RollingGraph with respective prefixes
 * \param plotError Switch for plotting error bars
 * \param type Select which property of the histogram to plot according to eRollingType
 * \param folder Internal path in the object browser were to add the Condition, the RollingGraph, and the Parameters
 * \param NumPoints Passed to the RollingGraph
 * \param UpdateInterval Passed to the RollingGraph
 */
TRemiRollingValue::TRemiRollingValue(TString name, TString sourceHist, Bool_t plotError, Int_t type, TString folder, Int_t NumPoints, Int_t UpdateInterval) :
TRemiIncludeFunctions(), mName(name), mSourceHist(sourceHist), mPlotError(plotError), mType(type)
{
	fParRolling = MakeParam<TRemiRollingParameter>(mName + suffix_param, folder);
	fParRolling->SetParameter("mSourceHist", mSourceHist);
	fParRolling->SetParameter("mPlotError", mPlotError);
	fParRolling->SetParameter("mType", mType);

	fLinkedHist = TGo4Analysis::Instance()->GetHistogram(mSourceHist);
	fCondition    =       MakeWinCon(mName + suffix_condi, prefix_condi + mSourceHist, mSourceHist, folder);
	fRollingGraph = MakeRollingGraph(mName + suffix_graph, prefix_graph + mSourceHist, NumPoints, UpdateInterval, folder);
}


/**
 * \brief Destructor
 */
TRemiRollingValue::~TRemiRollingValue()
{
}


/**
 * \brief Call update for the sourceHist and the graph
 */
void TRemiRollingValue::Update()
{
	if (! fParRolling->mUpdate)
		return;

	UpdateSourceHist();
	if (fLinkedHist)
		UpdateGraph();
}


/**
 * \brief Update the sourceHist
 */
void TRemiRollingValue::UpdateSourceHist()
{
	TString newSourceHist = fParRolling->mSourceHist;

	if (!IsLinkedTo(newSourceHist))
	{
		fCondition->SetHistogram(newSourceHist);
		fCondition->ResetCounts();
		fCondition->SetTitle(prefix_condi + newSourceHist);

		fRollingGraph->Clear();
		fRollingGraph->SetTitle(prefix_graph + newSourceHist);

		mSourceHist = newSourceHist;
		fLinkedHist = TGo4Analysis::Instance()->GetHistogram(mSourceHist);
	}
}


/**
 * \brief Test whether condition is linked to hist
 */
Bool_t TRemiRollingValue::IsLinkedTo(TString hist)
{
	if (hist.Length() == 0)
		return kFALSE;

	TString linkedHist = fCondition->GetLinkedHistogram();
	return (hist == linkedHist);
}


/**
 * \brief Update the graph
 */
void TRemiRollingValue::UpdateGraph()
{
	Double_t value = GetValue(fCondition);

	// Test the threshold for #counts in the conditions
	if (fParRolling->mThreshold) {
		Int_t thresholdCounts = fParRolling->mThresholdCounts;
		Double_t valueCounts  = fCondition->GetIntegral(fLinkedHist);

		if (valueCounts < thresholdCounts) {
			return;
		}
	}

//	// Dead time?
//	if (mCounter <= 5000) {
//		return;
//	}

	// Test whether the source histogram is to be cleared
	if (fParRolling->mClear) {
		mCounter++;
		if (mCounter >= fParRolling->mClearInterval) {
			std::cout << "Warning: Rolling Value \"" << mName << "\" clearing histogram \"" << mSourceHist << "\"!" << std::endl;
			mCounter = 0;
			fLinkedHist->Reset();
			return;
		}
	}

	// Calculate the error
	Float_t error;
	mPlotError = fParRolling->mPlotError;
	if (mPlotError)
		error = TMath::Sqrt(value);
	else
		error = 0;

	// Plot the graph
	fRollingGraph->Fill(value, 0, error);
}


/**
 * \brief Read the value (as set by the type argument of the constructor) from the given condition
 */
Double_t TRemiRollingValue::GetValue(TGo4WinCond * c)
{
	mType = fParRolling->mType;
	switch (mType)
	{
	case kIntegral:
		return c->GetIntegral(fLinkedHist);
	case kMean:
		return c->GetMean(fLinkedHist);
	case kRMS:
		return c->GetRMS(fLinkedHist);
	case kSkewness:
		return c->GetSkewness(fLinkedHist);
	case kCurtosis:
		return c->GetCurtosis(fLinkedHist);
	case kMax:
		return c->GetCMax(fLinkedHist);
	default:
		return c->GetIntegral(fLinkedHist);
	}
}



ClassImp(TRemiRollingValue);

