#ifndef TREMIROLLINGPARAMETER_H
#define TREMIROLLINGPARAMETER_H

#include "TGo4Parameter.h"

enum eRollingType {kIntegral, kMean, kRMS, kSkewness, kCurtosis, kMax};


/**
 * \brief Parameter class controlling the rolling graphs
 */
class TRemiRollingParameter : public TGo4Parameter
{
	public:
		TRemiRollingParameter(const char* name="TRemiRollingParameter");
		virtual ~TRemiRollingParameter();

		void SetParameter(TString name, TString value);
		void SetParameter(TString name, Int_t value);

		Bool_t  mUpdate;		///< Update this rolling graph, mapped as {0: No, 1: Yes}
		TString mSourceHist;	///< Source histogram for the rolling graph
		Bool_t  mPlotError;		///< Plot error bars, mapped as {0: No, 1: Yes}
		Int_t   mType;			///< Type of information, mapped as {0: Integral, 1: Mean, 2: RMS, 3: Skewness, 4: Curtosis, 5: Max}

		Bool_t  mClear;				///< Clear the source histogram, mapped as {0: No, 1: Yes}
		Int_t   mClearInterval;		///< Clearing interval

		Bool_t  mThreshold;			///< Plot only if mThresholdCounts is overcome, mapped as {0: No, 1: Yes}
		Int_t   mThresholdCounts;	///< Minimum counts in source histogram


	private:
		/**
		 * \brief Set default values
		 */
		void Init()
		{
			mUpdate = kFALSE;
			mSourceHist = "";
			mPlotError = kFALSE;
			mType = kIntegral;

			mClear = kFALSE;
			mClearInterval = 75000;

			mThreshold = kFALSE;
			mThresholdCounts = 5000;
		}


	ClassDef(TRemiRollingParameter, 1);
};

#endif

