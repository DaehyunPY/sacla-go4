//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the main analysis class header
//----------------------------------------------------------------
#ifndef TRemiANALYSIS_H
#define TRemiANALYSIS_H

#define MAINCONFIG "cMain.conf"

#include "TRemiIncludeFunctions.h"

#include "TGo4Analysis.h"

class TFile;
class TRemiExtractEvent;
class TRemiUnpackEvent;
class TRemiCalcEvent;
class TRemiFinishEvent;
class TRemiHistExport;
class TRemiRollingRatio;
class TRemiRollingValue;
class TH1D;
class ConfigFile;

/**
 * \brief Main analysis class
 */

class TRemiAnalysis : public TGo4Analysis, protected TRemiIncludeFunctions
{
	public:
		TRemiAnalysis();
		TRemiAnalysis(int argc, char** argv);
		virtual       ~TRemiAnalysis() ;
		virtual Int_t UserPreLoop();
		virtual Int_t UserEventFunc();
		virtual Int_t UserPostLoop();
	private:
		TFile              *fUserFile;
		TGo4MbsEvent       *fMbsEvent;
		TRemiExtractEvent  *fAcqEvent;
		TRemiExtractEvent  *fExtractEvent;
		TRemiUnpackEvent   *fUnpackEvent;
		TRemiCalcEvent     *fCalcEvent;
		TRemiFinishEvent   *fFinishEvent;
		TH1D               *fSize;
		Int_t              fEvents;
		Int_t              fLastEvent;
		TRemiHistExport    *fExport;
		TRemiRollingRatio  *fRollingRatio;
		TRemiRollingValue  *fRollingValue;
		TGo4ShapedCond     *fCircleCond;

		ConfigFile	*fConfigFile;
		Int_t mExtractSource;	///< Source of the Extract step: 0 - MBS, 1 - Acq_file, 2 - Acq_online
		
		
//		void	InitAsymPlots ();
//		void	RenormalizeAsymPlots ();
		
		
	
	ClassDef(TRemiAnalysis, 1);
};
#endif //TRemiANALYSIS_H

//----------------------------END OF GO4 SOURCE FILE ---------------------


