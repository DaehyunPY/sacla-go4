//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the base class for all position-calculating classes
//----------------------------------------------------------------

#ifndef TREMIUNPACKPROCPOS_H
#define TREMIUNPACKPROCPOS_H

#include "TRemiUnpackProcTof.h"
#include "../Core/Point/TRemiDetectorHit.h"

#include <vector>

class ConfigFile;

class TH1I;
class TH2I;

class TGo4WinCond;
class TGo4PolyCond;

/**
 * \brief Abstract base class for all position-sensitive detector read-out classes
 * 
 * Provides the pure virtual function CalculatePosition() which is used in TRemiUnpackProc::UnpackProcessor() to calculate a particle's position. The implementation of this function has to be made in each extended class for each detector type
 */
class TRemiUnpackProcPos : public TRemiUnpackProcTof
{
	public:
		TRemiUnpackProcPos();
		
		TRemiUnpackProcPos(ConfigFile* UnpackConf, TString particle);
		
		~TRemiUnpackProcPos();
		
		virtual void CalculatePosition() = 0; ///< Function to calculate position. Has to be overwritten by extended classes
		
		Bool_t FindSingleLayerEvent(std::vector<Float_t>::iterator &value1, std::vector<Float_t>::iterator &value2, std::vector<Float_t> &list1, std::vector<Float_t> &list2, Float_t McpValue, TGo4WinCond* ConTimeSum, TGo4WinCond* ConSort1, TGo4WinCond* ConSort2, TH1I* SumHist=0);
		
		void FillPosHistograms() const;
		
		void GetPosition(std::vector<TRemiXY> &Position) const;
		
		void GetHits(std::vector<TRemiDetectorHit> &Hits) const;
		
		void SetMbIndex(Int_t in)	{ mMbIndex = in;}
		
		Bool_t TestPosCondition(TRemiDetectorHit &Hit);
		Bool_t TestTofCondition(TRemiDetectorHit &Hit);
		
		
	protected:
		// Histograms for sorted hits
		TH1I* 	fSortTof;	//!< Histogram for sorted times-of-flight 
		
		TH2I*	fSortXY;	///< Position spectra
		TH1I*	fSortX;	///< X coordinate spectra
		TH1I*	fSortY;	///< Y coordinate spectra
		TH1I*   fSortR;	///< Radius of detector hit spectra
		TH2I*	fSortXY_ct;	///< Position spectra with time-of-flight condition
		TH2I*	fSortX_Tof;	///< Spectra showing X vs. time-of-flight
		TH2I*	fSortY_Tof;	///< Spectra showing Y vs. time-of-flight
		TH2I*	fSortR_Tof;	///< Spectra showing radius vs. time-of-flight
		TH2I*	fSortR_Phi;	///< Spectra showing detector hit radius vs. angle
		
		/** \name Plots with differences between neighbouring particles */
		//@{
		TH1I*	fSortTofDiff;	///< Plot showing Tof difference
		TH2I*	fSortXdiffTofDiff;	///< Plot showing X difference vs. Tof difference
		TH2I*	fSortYdiffTofDiff;	///< Plot showing y difference vs. Tof difference
		//@}
		
		TGo4PolyCond*	fSortConPos;	///< Polygon conditions for the position
		TGo4WinCond*	fConRad;	///< Radius condition
		TGo4WinCond*	fSortConY;		///< Y-position condition
		
		TH1I*	fSortTofConPos;	///< Time-of-flight spectrum for sorted hits with position condition
		TH2I*	fSortDelayVsTofConPos;	///< Histogram for Delay vs. sorted time of flight with position condition
		TH2I*	fSortDelayVsTofConRad;	///< Histogram for Delay vs. sorted time of flight with radius condition
		
		TH2I*	fSortDelayVsX;	///< Histogram for Delay vs. sorted X
		TH2I*	fSortDelayVsY;	///< Histogram for Delay vs. sorted Y
		TH2I*	fSortDelayVsR;	///< Histogram for Delay vs. sorted R
		
		TH2I*	fSortDelayVsXConTof;
		TH2I*	fSortDelayVsYConTof;
		TH2I*	fSortDelayVsXConTofConY;
		
		TH2I*	fSortXvsRealtimeConTof;	///< Histogram for sorted X vs. the real time of reading in the data
		
		std::vector<TH1I*>	fSortTofSepIndividual;
		TH1I*				fSortTofSepRemaining;
		
		std::vector<TRemiDetectorHit>	mDetectorHits;	///< Array of successfully sorted detector hits
		
		Int_t mMbIndex;
		
		TString mDetType;
		
	ClassDef(TRemiUnpackProcPos, 1);
};

#endif
