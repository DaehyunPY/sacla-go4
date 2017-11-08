#ifndef TREMIROLLINGRATIO_H
#define TREMIROLLINGRATIO_H

#include "../TRemiIncludeFunctions.h"
#include "TRemiRollingParameter.h"

class TH1;
class TGo4WinCond;
class TRemiRollingGraph;


/**
 * \brief RollingGraph of the ratio of the information given by type under two conditions on the same histogramm
 */
class TRemiRollingRatio : protected TRemiIncludeFunctions
{
	public:
		TRemiRollingRatio(TString name="RatioExample", TString sourceHist="", 
			Bool_t plotError=kFALSE, Int_t type=kIntegral, TString folder="Rolling",
			Int_t NumPoints=30000, Int_t UpdateInterval=10);
		virtual ~TRemiRollingRatio();

		void Update();


	protected:
		static const TString prefix_numer, prefix_denom, prefix_graph;
		static const TString suffix_numer, suffix_denom, suffix_graph, suffix_param;

		void UpdateGraph();
		void UpdateSourceHist();
		Bool_t IsLinkedTo(TString sourceHist);
		Double_t GetValue(TGo4WinCond * c);

		TRemiRollingGraph * fRollingGraph;
		TGo4WinCond * fConRatioNumerator;
		TGo4WinCond * fConRatioDenominator;
		TH1 * fLinkedHist;

		TString mName;
		TString mSourceHist;
		Bool_t mPlotError;
		Int_t mType;

		TRemiRollingParameter * fParRolling;

		Int_t mCounter;


	ClassDef(TRemiRollingRatio, 1);
};

#endif

