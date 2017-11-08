#include "TRemiRollingRatio.h"

#include "TMath.h"
#include "TGo4WinCond.h"
#include "../RootMod/TRemiRollingGraph.h"

#include "TRemiRollingParameter.h"


const TString TRemiRollingRatio::prefix_numer = "Ratio Numerator Condition: ";
const TString TRemiRollingRatio::prefix_denom = "Ratio Denominator Condition: ";
const TString TRemiRollingRatio::prefix_graph = "Rolling Ratio: ";

const TString TRemiRollingRatio::suffix_numer = "_numerator";
const TString TRemiRollingRatio::suffix_denom = "_denominator";
const TString TRemiRollingRatio::suffix_graph = "_rollingGraph";
const TString TRemiRollingRatio::suffix_param = "_parameters";



/**
 * \brief Default Constructor
 * \param name Name, used for the Conditions, the RollingGraph, and the Parameters with respective suffixes
 * \param sourceHist Source histogram for the Conditions, used to generate the titles of the Conditions and the RollingGraph with respective prefixes
 * \param plotError Switch for plotting error bars
 * \param type Select which property of the histogram to use for calculating the ratio according to eRollingType
 * \param folder Internal path in the object browser were to add the Conditions, the RollingGraph, and the Parameters
 * \param NumPoints Passed to the RollingGraph
 * \param UpdateInterval Passed to the RollingGraph
 */
TRemiRollingRatio::TRemiRollingRatio(TString name, TString sourceHist, Bool_t plotError, Int_t type, TString folder, Int_t NumPoints, Int_t UpdateInterval) :
TRemiIncludeFunctions(), mName(name), mSourceHist(sourceHist), mPlotError(plotError), mType(type)
{
	fParRolling = MakeParam<TRemiRollingParameter>(mName + suffix_param, folder);
	fParRolling->SetParameter("mSourceHist", mSourceHist);
	fParRolling->SetParameter("mPlotError", mPlotError);
	fParRolling->SetParameter("mType", mType);

	fLinkedHist = TGo4Analysis::Instance()->GetHistogram(mSourceHist);
	fConRatioNumerator   = MakeWinCon(mName + suffix_numer, prefix_numer + mSourceHist, mSourceHist, folder);
	fConRatioDenominator = MakeWinCon(mName + suffix_denom, prefix_denom + mSourceHist, mSourceHist, folder);
	fRollingGraph  = MakeRollingGraph(mName + suffix_graph, prefix_graph + mSourceHist, NumPoints, UpdateInterval, folder);
}


/**
 * \brief Destructor
 */
TRemiRollingRatio::~TRemiRollingRatio()
{
}


/**
 * \brief Call update for the sourceHist and the graph
 */
void TRemiRollingRatio::Update()
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
void TRemiRollingRatio::UpdateSourceHist()
{
	TString newSourceHist = fParRolling->mSourceHist;

	if (!IsLinkedTo(newSourceHist))
	{
		fConRatioNumerator->SetHistogram(newSourceHist);
		fConRatioNumerator->ResetCounts();
		fConRatioNumerator->SetTitle(prefix_numer + newSourceHist);

		fConRatioDenominator->SetHistogram(newSourceHist);
		fConRatioDenominator->ResetCounts();
		fConRatioDenominator->SetTitle(prefix_denom + newSourceHist);

		fRollingGraph->Clear();
		fRollingGraph->SetTitle(prefix_graph + newSourceHist);

		mSourceHist = newSourceHist;
		fLinkedHist = TGo4Analysis::Instance()->GetHistogram(mSourceHist);
	}
}


/**
 * \brief Test whether numerator and denominator conditions are linked to hist
 */
Bool_t TRemiRollingRatio::IsLinkedTo(TString hist)
{
	if (hist.Length() == 0)
		return kFALSE;

	TString linkedHistNumer = fConRatioNumerator->GetLinkedHistogram();
	TString linkedHistDenom = fConRatioDenominator->GetLinkedHistogram();
	return ((hist == linkedHistNumer) && (hist == linkedHistDenom));
}


/**
 * \brief Update the graph
 */
void TRemiRollingRatio::UpdateGraph()
{
	Double_t numerator   = GetValue(fConRatioNumerator);
	Double_t denominator = GetValue(fConRatioDenominator);

	// Test the threshold for #counts in the conditions
	if (fParRolling->mThreshold) {
		Int_t thresholdCounts      = fParRolling->mThresholdCounts;
		Double_t numeratorCounts   = fConRatioNumerator->GetIntegral(fLinkedHist);
		Double_t denominatorCounts = fConRatioDenominator->GetIntegral(fLinkedHist);

		if ((numeratorCounts < thresholdCounts) && (denominatorCounts < thresholdCounts)) {
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
			std::cout << "Warning: Rolling Ratio \"" << mName << "\" clearing histogram \"" << mSourceHist << "\"!" << std::endl;
			mCounter = 0;
			fLinkedHist->Reset();
			return;
		}
	}

	// Calculate the ratio
	Float_t ratio;
	if (denominator != 0)
		ratio = (Float_t) numerator / denominator;
	else
		ratio = 0;

	// Calculate the error
	Float_t error;
	mPlotError = fParRolling->mPlotError;
	if (mPlotError && (denominator != 0) && (numerator != 0))
		error = ratio * TMath::Sqrt(1/numerator + 1/denominator);
	else
		error = 0;

	// Plot the graph
	fRollingGraph->Fill(ratio, 0, error);
}


/**
 * \brief Read the value (as set by the type argument of the constructor) from the given condition
 */
Double_t TRemiRollingRatio::GetValue(TGo4WinCond * c)
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



ClassImp(TRemiRollingRatio);

