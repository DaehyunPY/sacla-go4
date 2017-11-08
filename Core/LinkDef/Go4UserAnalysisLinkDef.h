//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 1.5
//
//		Opening part for the automatically generated Go4UserAnalysisLinkDef.h
//
//		In some cases, i.e. if template class specializations are used in the code
//		it might be necessary to add pragma links here or in the closing part
//----------------------------------------------------------------

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ class TRemiPoint<Float_t,2>+;
#pragma link C++ class TRemiPoint<Float_t,3>+;
#pragma link C++ class vector<TRemiParticle*>::iterator>+; //this causes a warning from roocint -v (iterator has no Streamer()), is it needed? anyway, is the last '>' correct?
#pragma link C++ class vector< vector<TRemiParticle*>::iterator >+;

#pragma link C++ defined_in "./Core/TRemiAnalysis.h"; 
#pragma link C++ defined_in "./Core/TRemiBasicParameter.h"; 
#pragma link C++ defined_in "./Core/TRemiEventElement.h"; 
#pragma link C++ defined_in "./Core/TRemiIncludeFunctions.h"; 
#pragma link C++ defined_in "./Core/TRemiIncludeFunctions.inline.h"; 
#pragma link C++ defined_in "./EventElement/TRemiCalcEvent.h"; 
#pragma link C++ defined_in "./EventElement/TRemiExtractEvent.h"; 
#pragma link C++ defined_in "./EventElement/TRemiFinishEvent.h"; 
#pragma link C++ defined_in "./EventElement/TRemiUnpackEvent.h"; 
#pragma link C++ defined_in "./StepCalc/TRemiCalcProcCoincidences.h"; 
#pragma link C++ defined_in "./StepCalc/TRemiCalcProc.h"; 
#pragma link C++ defined_in "./StepCalc/TRemiCalcProcMomentum.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisAdcPulse.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisArchive.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisBuffer.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisChannel.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisDataStreamError.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisEvent.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisEventSource.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisFileEventSource.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisIO.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisPeak.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisPuls.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisSocketEventSource.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisSoftAdc.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiAcqirisSoftTdc.h"; 
#pragma link C++ defined_in "./StepExtractAcq/TRemiExtractAcqProc.h"; 
#pragma link C++ defined_in "./StepExtractMbs/TRemiExtractMbsProc.h"; 
#pragma link C++ defined_in "./StepFinish/TRemiFinishProc.h"; 
#pragma link C++ defined_in "./StepUnpack/TRemiUnpackProcDelay.h"; 
#pragma link C++ defined_in "./StepUnpack/TRemiUnpackProcDelayline.h"; 
#pragma link C++ defined_in "./StepUnpack/TRemiUnpackProc.h"; 
#pragma link C++ defined_in "./StepUnpack/TRemiUnpackProcHexAnode.h"; 
#pragma link C++ defined_in "./StepUnpack/TRemiUnpackProcPos.h"; 
#pragma link C++ defined_in "./StepUnpack/TRemiUnpackProcReconstruct.h"; 
#pragma link C++ defined_in "./StepUnpack/TRemiUnpackProcTof.h"; 
#pragma link C++ defined_in "./StepUnpack/TRemiUnpackProcTofOnly.h"; 
#pragma link C++ defined_in "./StepUnpack/TRemiUnpackProcWaSAnode.h"; 
#pragma link C++ defined_in "./Core/Extern/ConfigFile.h"; 
#pragma link C++ defined_in "./Core/Extern/Gnuplot.h"; 
#pragma link C++ defined_in "./Core/HistExport/TRemiHistExport.h"; 
#pragma link C++ defined_in "./Core/HistExport/TRemiHistExportParameter.h"; 
#pragma link C++ defined_in "./Core/Point/TRemiCoincidence.h"; 
#pragma link C++ defined_in "./Core/Point/TRemiDetectorHit.h"; 
#pragma link C++ defined_in "./Core/Point/TRemiParticle.h"; 
#pragma link C++ defined_in "./Core/Point/TRemiPoint.h"; 
#pragma link C++ defined_in "./Core/Point/TRemiPoint.inline.h"; 
#pragma link C++ defined_in "./Core/Point/TRemiPxyz.h"; 
#pragma link C++ defined_in "./Core/Point/TRemiXY.h"; 
#pragma link C++ defined_in "./Core/Rolling/TRemiRollingParameter.h"; 
#pragma link C++ defined_in "./Core/Rolling/TRemiRollingRatio.h"; 
#pragma link C++ defined_in "./Core/Rolling/TRemiRollingValue.h"; 
#pragma link C++ defined_in "./Core/RootMod/InterpolatorTF1.h"; 
#pragma link C++ defined_in "./Core/RootMod/TH2Fsmooth.h"; 
#pragma link C++ defined_in "./Core/RootMod/TObjTArrC.h"; 
#pragma link C++ defined_in "./Core/RootMod/TRemiRollingGraph.h"; 
#pragma link C++ defined_in "./StepCalc/Parameters/TRemiMomentumParameter.h"; 
#pragma link C++ defined_in "./StepExtractAcq/Parameters/TRemiAcqirisCfdParameter.h"; 
#pragma link C++ defined_in "./StepExtractAcq/Parameters/TRemiAcqirisCoMParameter.h"; 
#pragma link C++ defined_in "./StepExtractAcq/Parameters/TRemiAcqirisHighRParameter.h"; 
#pragma link C++ defined_in "./StepExtractAcq/Parameters/TRemiAcqirisRisEParameter.h"; 
#pragma link C++ defined_in "./StepExtractAcq/Parameters/TRemiAcqirisSingleShotParameter.h"; 
#pragma link C++ defined_in "./StepFinish/Parameters/TRemiFinishParameter.h"; 
#pragma link C++ defined_in "./StepUnpack/Parameters/TRemiDelaylineParameter.h"; 
#pragma link C++ defined_in "./StepUnpack/Parameters/TRemiDelayParameter.h"; 
#pragma link C++ defined_in "./StepUnpack/Parameters/TRemiHexAnodeHoleParameter.h"; 
#pragma link C++ defined_in "./StepUnpack/Parameters/TRemiHexAnodeParameter.h"; 
#pragma link C++ defined_in "./StepUnpack/Parameters/TRemiReconstructParameter.h"; 
#pragma link C++ defined_in "./StepUnpack/Parameters/TRemiUnpackParameter.h"; 
#pragma link C++ defined_in "./StepUnpack/Parameters/TRemiWaSParameter.h";
//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 1.5
//
//		Closing part for the automatically generated Go4UserAnalysisLinkDef.h
//
//		In some cases, i.e. if template class specializations are used in the code
//		it might be necessary to add pragma links here or in the opening part
//----------------------------------------------------------------


#endif
