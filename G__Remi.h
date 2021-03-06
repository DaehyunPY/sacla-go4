/********************************************************************
* G__Remi.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error G__Remi.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
#include "FastAllocString.h"
extern "C" {
extern void G__cpp_setup_tagtableG__Remi();
extern void G__cpp_setup_inheritanceG__Remi();
extern void G__cpp_setup_typetableG__Remi();
extern void G__cpp_setup_memvarG__Remi();
extern void G__cpp_setup_globalG__Remi();
extern void G__cpp_setup_memfuncG__Remi();
extern void G__cpp_setup_funcG__Remi();
extern void G__set_cpp_environmentG__Remi();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "./Core/TRemiAnalysis.h"
#include "./Core/TRemiBasicParameter.h"
#include "./Core/TRemiEventElement.h"
#include "./Core/TRemiIncludeFunctions.h"
#include "./Core/TRemiIncludeFunctions.inline.h"
#include "./EventElement/TRemiCalcEvent.h"
#include "./EventElement/TRemiExtractEvent.h"
#include "./EventElement/TRemiFinishEvent.h"
#include "./EventElement/TRemiUnpackEvent.h"
#include "./StepCalc/TRemiCalcProcCoincidences.h"
#include "./StepCalc/TRemiCalcProc.h"
#include "./StepCalc/TRemiCalcProcMomentum.h"
#include "./StepExtractAcq/TRemiAcqirisAdcPulse.h"
#include "./StepExtractAcq/TRemiAcqirisArchive.h"
#include "./StepExtractAcq/TRemiAcqirisBuffer.h"
#include "./StepExtractAcq/TRemiAcqirisChannel.h"
#include "./StepExtractAcq/TRemiAcqirisDataStreamError.h"
#include "./StepExtractAcq/TRemiAcqirisEvent.h"
#include "./StepExtractAcq/TRemiAcqirisEventSource.h"
#include "./StepExtractAcq/TRemiAcqirisFileEventSource.h"
#include "./StepExtractAcq/TRemiAcqirisIO.h"
#include "./StepExtractAcq/TRemiAcqirisPeak.h"
#include "./StepExtractAcq/TRemiAcqirisPuls.h"
#include "./StepExtractAcq/TRemiAcqirisSocketEventSource.h"
#include "./StepExtractAcq/TRemiAcqirisSoftAdc.h"
#include "./StepExtractAcq/TRemiAcqirisSoftTdc.h"
#include "./StepExtractAcq/TRemiExtractAcqProc.h"
#include "./StepExtractMbs/TRemiExtractMbsProc.h"
#include "./StepFinish/TRemiFinishProc.h"
#include "./StepUnpack/TRemiUnpackProcDelay.h"
#include "./StepUnpack/TRemiUnpackProcDelayline.h"
#include "./StepUnpack/TRemiUnpackProc.h"
#include "./StepUnpack/TRemiUnpackProcHexAnode.h"
#include "./StepUnpack/TRemiUnpackProcPos.h"
#include "./StepUnpack/TRemiUnpackProcReconstruct.h"
#include "./StepUnpack/TRemiUnpackProcTof.h"
#include "./StepUnpack/TRemiUnpackProcTofOnly.h"
#include "./StepUnpack/TRemiUnpackProcWaSAnode.h"
#include "./Core/Extern/ConfigFile.h"
#include "./Core/Extern/Gnuplot.h"
#include "./Core/HistExport/TRemiHistExport.h"
#include "./Core/HistExport/TRemiHistExportParameter.h"
#include "./Core/Point/TRemiCoincidence.h"
#include "./Core/Point/TRemiDetectorHit.h"
#include "./Core/Point/TRemiParticle.h"
#include "./Core/Point/TRemiPoint.h"
#include "./Core/Point/TRemiPoint.inline.h"
#include "./Core/Point/TRemiPxyz.h"
#include "./Core/Point/TRemiXY.h"
#include "./Core/Rolling/TRemiRollingParameter.h"
#include "./Core/Rolling/TRemiRollingRatio.h"
#include "./Core/Rolling/TRemiRollingValue.h"
#include "./Core/RootMod/InterpolatorTF1.h"
#include "./Core/RootMod/TH2Fsmooth.h"
#include "./Core/RootMod/TObjTArrC.h"
#include "./Core/RootMod/TRemiRollingGraph.h"
#include "./StepCalc/Parameters/TRemiMomentumParameter.h"
#include "./StepExtractAcq/Parameters/TRemiAcqirisCfdParameter.h"
#include "./StepExtractAcq/Parameters/TRemiAcqirisCoMParameter.h"
#include "./StepExtractAcq/Parameters/TRemiAcqirisHighRParameter.h"
#include "./StepExtractAcq/Parameters/TRemiAcqirisRisEParameter.h"
#include "./StepExtractAcq/Parameters/TRemiAcqirisSingleShotParameter.h"
#include "./StepFinish/Parameters/TRemiFinishParameter.h"
#include "./StepUnpack/Parameters/TRemiDelaylineParameter.h"
#include "./StepUnpack/Parameters/TRemiDelayParameter.h"
#include "./StepUnpack/Parameters/TRemiHexAnodeHoleParameter.h"
#include "./StepUnpack/Parameters/TRemiHexAnodeParameter.h"
#include "./StepUnpack/Parameters/TRemiReconstructParameter.h"
#include "./StepUnpack/Parameters/TRemiUnpackParameter.h"
#include "./StepUnpack/Parameters/TRemiWaSParameter.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__G__RemiLN_TClass;
extern G__linked_taginfo G__G__RemiLN_TBuffer;
extern G__linked_taginfo G__G__RemiLN_TMemberInspector;
extern G__linked_taginfo G__G__RemiLN_TObject;
extern G__linked_taginfo G__G__RemiLN_TNamed;
extern G__linked_taginfo G__G__RemiLN_TString;
extern G__linked_taginfo G__G__RemiLN_ROOT;
extern G__linked_taginfo G__G__RemiLN_vectorlEshortcOallocatorlEshortgRsPgR;
extern G__linked_taginfo G__G__RemiLN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR;
extern G__linked_taginfo G__G__RemiLN_vectorlEfloatcOallocatorlEfloatgRsPgR;
extern G__linked_taginfo G__G__RemiLN_vectorlEfloatcOallocatorlEfloatgRsPgRcLcLiterator;
extern G__linked_taginfo G__G__RemiLN_vectorlEdoublecOallocatorlEdoublegRsPgR;
extern G__linked_taginfo G__G__RemiLN_vectorlEboolcOallocatorlEboolgRsPgR;
extern G__linked_taginfo G__G__RemiLN_basic_istreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__RemiLN_basic_ostreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__RemiLN_basic_ifstreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__RemiLN_basic_ofstreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__RemiLN_basic_fstreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__RemiLN_string;
extern G__linked_taginfo G__G__RemiLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TGo4CommandReceiver;
extern G__linked_taginfo G__G__RemiLN_TH1;
extern G__linked_taginfo G__G__RemiLN_TH2;
extern G__linked_taginfo G__G__RemiLN_TFile;
extern G__linked_taginfo G__G__RemiLN_TGo4Condition;
extern G__linked_taginfo G__G__RemiLN_TGo4WinCond;
extern G__linked_taginfo G__G__RemiLN_TGo4PolyCond;
extern G__linked_taginfo G__G__RemiLN_TGo4ShapedCond;
extern G__linked_taginfo G__G__RemiLN_TGo4Parameter;
extern G__linked_taginfo G__G__RemiLN_TGo4Picture;
extern G__linked_taginfo G__G__RemiLN_TGo4EventElement;
extern G__linked_taginfo G__G__RemiLN_TGo4EventSource;
extern G__linked_taginfo G__G__RemiLN_TGo4EventProcessor;
extern G__linked_taginfo G__G__RemiLN_TGo4Analysis;
extern G__linked_taginfo G__G__RemiLN_TArray;
extern G__linked_taginfo G__G__RemiLN_TAttLine;
extern G__linked_taginfo G__G__RemiLN_TAttFill;
extern G__linked_taginfo G__G__RemiLN_TAttMarker;
extern G__linked_taginfo G__G__RemiLN_TArrayC;
extern G__linked_taginfo G__G__RemiLN_TArrayF;
extern G__linked_taginfo G__G__RemiLN_TVectorTlEfloatgR;
extern G__linked_taginfo G__G__RemiLN_TVectorTlEdoublegR;
extern G__linked_taginfo G__G__RemiLN_TF1;
extern G__linked_taginfo G__G__RemiLN_TH1D;
extern G__linked_taginfo G__G__RemiLN_TH1I;
extern G__linked_taginfo G__G__RemiLN_TH1F;
extern G__linked_taginfo G__G__RemiLN_TH3;
extern G__linked_taginfo G__G__RemiLN_TH3I;
extern G__linked_taginfo G__G__RemiLN_TRemiBasicParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiRollingGraph;
extern G__linked_taginfo G__G__RemiLN_ConfigFile;
extern G__linked_taginfo G__G__RemiLN_TGo4CondArray;
extern G__linked_taginfo G__G__RemiLN_TGraph;
extern G__linked_taginfo G__G__RemiLN_TCutG;
extern G__linked_taginfo G__G__RemiLN_TLine;
extern G__linked_taginfo G__G__RemiLN_TArc;
extern G__linked_taginfo G__G__RemiLN_TRemiIncludeFunctions;
extern G__linked_taginfo G__G__RemiLN_vectorlETH1mUcOallocatorlETH1mUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETH1mUcOallocatorlETH1mUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiBasicParametermUcOallocatorlETRemiBasicParametermUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiBasicParametermUcOallocatorlETRemiBasicParametermUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETGo4ConditionmUcOallocatorlETGo4ConditionmUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETGo4ConditionmUcOallocatorlETGo4ConditionmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_maplEstringcOstringcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOstringgRsPgRsPgR;
extern G__linked_taginfo G__G__RemiLN_maplEstringcOstringcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOstringgRsPgRsPgRcLcLiterator;
extern G__linked_taginfo G__G__RemiLN_ConfigFilecLcLfile_not_found;
extern G__linked_taginfo G__G__RemiLN_ConfigFilecLcLkey_not_found;
extern G__linked_taginfo G__G__RemiLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR;
extern G__linked_taginfo G__G__RemiLN_TH2F;
extern G__linked_taginfo G__G__RemiLN_TH2I;
extern G__linked_taginfo G__G__RemiLN_TGo4MbsEvent;
extern G__linked_taginfo G__G__RemiLN_TGo4MbsSubEvent;
extern G__linked_taginfo G__G__RemiLN_TGo4UserSourceParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiExtractEvent;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackEvent;
extern G__linked_taginfo G__G__RemiLN_TRemiCalcEvent;
extern G__linked_taginfo G__G__RemiLN_TRemiFinishEvent;
extern G__linked_taginfo G__G__RemiLN_TRemiHistExport;
extern G__linked_taginfo G__G__RemiLN_TRemiRollingRatio;
extern G__linked_taginfo G__G__RemiLN_TRemiRollingValue;
extern G__linked_taginfo G__G__RemiLN_TRemiAnalysis;
extern G__linked_taginfo G__G__RemiLN_TRemiEventElement;
extern G__linked_taginfo G__G__RemiLN_TRemiXY;
extern G__linked_taginfo G__G__RemiLN_TRemiPointlEfloatcO2gR;
extern G__linked_taginfo G__G__RemiLN_TRemiDetectorHit;
extern G__linked_taginfo G__G__RemiLN_TRemiPxyz;
extern G__linked_taginfo G__G__RemiLN_TRemiPointlEfloatcO3gR;
extern G__linked_taginfo G__G__RemiLN_TRemiParticle;
extern G__linked_taginfo G__G__RemiLN_TRemiCoincidence;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiParticlecOallocatorlETRemiParticlegRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiParticlecOallocatorlETRemiParticlegRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiCoincidencecOallocatorlETRemiCoincidencegRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiCoincidencecOallocatorlETRemiCoincidencegRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlEvectorlEfloatcOallocatorlEfloatgRsPgRcOallocatorlEvectorlEfloatcOallocatorlEfloatgRsPgRsPgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlEvectorlEfloatcOallocatorlEfloatgRsPgRcOallocatorlEvectorlEfloatcOallocatorlEfloatgRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TRemiFinishProc;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiDetectorHitcOallocatorlETRemiDetectorHitgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiDetectorHitcOallocatorlETRemiDetectorHitgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TRemiCalcProcMomentum;
extern G__linked_taginfo G__G__RemiLN_TRemiCalcProcCoincidences;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiCalcProcMomentumcOallocatorlETRemiCalcProcMomentumgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiCalcProcMomentumcOallocatorlETRemiCalcProcMomentumgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiCoincidencemUcOallocatorlETRemiCoincidencemUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiCoincidencemUcOallocatorlETRemiCoincidencemUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgRcLcLiterator;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlEvectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgRcLcLiteratorcOallocatorlEvectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgRcLcLiteratorgRsPgR;
extern G__linked_taginfo G__G__RemiLN_vectorlEvectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgRcLcLiteratorcOallocatorlEvectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgRcLcLiteratorgRsPgRcLcLiterator;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlEvectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgRcLcLiteratorcOallocatorlEvectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgRcLcLiteratorgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETStringcOallocatorlETStringgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETStringcOallocatorlETStringgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETH2ImUcOallocatorlETH2ImUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETGo4PicturemUcOallocatorlETGo4PicturemUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETGo4PicturemUcOallocatorlETGo4PicturemUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETH1ImUcOallocatorlETH1ImUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETH1ImUcOallocatorlETH1ImUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETH3ImUcOallocatorlETH3ImUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETH3ImUcOallocatorlETH3ImUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlEvectorlETH1ImUcOallocatorlETH1ImUgRsPgRcOallocatorlEvectorlETH1ImUcOallocatorlETH1ImUgRsPgRsPgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlEvectorlETH1ImUcOallocatorlETH1ImUgRsPgRcOallocatorlEvectorlETH1ImUcOallocatorlETH1ImUgRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRcOallocatorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRsPgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRcOallocatorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiCalcProcMomentummUcOallocatorlETRemiCalcProcMomentummUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiCalcProcMomentummUcOallocatorlETRemiCalcProcMomentummUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TRemiMomentumParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiCalcProc;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiCalcProcCoincidencescOallocatorlETRemiCalcProcCoincidencesgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiCalcProcCoincidencescOallocatorlETRemiCalcProcCoincidencesgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlEvectorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRcOallocatorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRsPgRsPgRcOallocatorlEvectorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRcOallocatorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRsPgRsPgRsPgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlEvectorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRcOallocatorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRsPgRsPgRcOallocatorlEvectorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRcOallocatorlEvectorlETH2ImUcOallocatorlETH2ImUgRsPgRsPgRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiCalcProcCoincidencesmUcOallocatorlETRemiCalcProcCoincidencesmUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiCalcProcCoincidencesmUcOallocatorlETRemiCalcProcCoincidencesmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisAdcPulse;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisIO;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisArchive;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisArchivecLcLEWriting;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisBuffer;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisChannel;
extern G__linked_taginfo G__G__RemiLN_EPolarity;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisPeak;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisPeakSorter;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisPuls;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisEvent;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisSoftAdc;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisSoftTdc;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiAcqirisPeakcOallocatorlETRemiAcqirisPeakgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiAcqirisPeakcOallocatorlETRemiAcqirisPeakgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiAcqirisAdcPulsecOallocatorlETRemiAcqirisAdcPulsegRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiAcqirisAdcPulsecOallocatorlETRemiAcqirisAdcPulsegRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_exception;
extern G__linked_taginfo G__G__RemiLN_logic_error;
extern G__linked_taginfo G__G__RemiLN_runtime_error;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisDataStreamError;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiAcqirisChannelmUcOallocatorlETRemiAcqirisChannelmUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiAcqirisChannelmUcOallocatorlETRemiAcqirisChannelmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisSingleShotParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisEventSource;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisEventSourcecLcLEInputType;
extern G__linked_taginfo G__G__RemiLN_vectorlEintcOallocatorlEintgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisFileEventSource;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiAcqirisPeakmUcOallocatorlETRemiAcqirisPeakmUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiAcqirisPeakmUcOallocatorlETRemiAcqirisPeakmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TMonitor;
extern G__linked_taginfo G__G__RemiLN_TSocket;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisSocketEventSource;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiAcqirisEventmUcOallocatorlETRemiAcqirisEventmUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiAcqirisEventmUcOallocatorlETRemiAcqirisEventmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisSoftAdccLcLeMethod;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisSoftAdccLcLeBits;
extern G__linked_taginfo G__G__RemiLN_vectorlETGraphmUcOallocatorlETGraphmUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETGraphmUcOallocatorlETGraphmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisHighRParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisCfdParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisRisEParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisPunkt;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisSoftTdccLcLeMethod;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisSoftTdccLcLeBits;
extern G__linked_taginfo G__G__RemiLN_vectorlETF1mUcOallocatorlETF1mUgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETF1mUcOallocatorlETF1mUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TRemiExtractAcqProc;
extern G__linked_taginfo G__G__RemiLN_TRandom3;
extern G__linked_taginfo G__G__RemiLN_TRemiExtractMbsProc;
extern G__linked_taginfo G__G__RemiLN_TRemiFinishParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiDelayParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackProcDelay;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackProcTof;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackProcPos;
extern G__linked_taginfo G__G__RemiLN_vectorlETRemiXYcOallocatorlETRemiXYgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlETRemiXYcOallocatorlETRemiXYgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TRemiDelaylineParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiReconstructParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackProcReconstruct;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackProcDelayline;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackProcDelaylinecLcLeDelaylineReconstruct;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackProc;
extern G__linked_taginfo G__G__RemiLN_TRemiHexAnodeParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackProcHexAnode;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackProcHexAnodecLcLeHexAnodeReconstruct;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackProcTofOnly;
extern G__linked_taginfo G__G__RemiLN_TRemiWaSParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiUnpackProcWaSAnode;
extern G__linked_taginfo G__G__RemiLN_GnuplotException;
extern G__linked_taginfo G__G__RemiLN_Gnuplot;
extern G__linked_taginfo G__G__RemiLN_vectorlEstringcOallocatorlEstringgRsPgR;
extern G__linked_taginfo G__G__RemiLN_reverse_iteratorlEvectorlEstringcOallocatorlEstringgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__RemiLN_TRemiHistExportParameter;
extern G__linked_taginfo G__G__RemiLN_eRollingType;
extern G__linked_taginfo G__G__RemiLN_TRemiRollingParameter;
extern G__linked_taginfo G__G__RemiLN_ROOTcLcLMath;
extern G__linked_taginfo G__G__RemiLN_ROOTcLcLMathcLcLInterpolation;
extern G__linked_taginfo G__G__RemiLN_ROOTcLcLMathcLcLInterpolationcLcLType;
extern G__linked_taginfo G__G__RemiLN_ROOTcLcLMathcLcLInterpolator;
extern G__linked_taginfo G__G__RemiLN_ROOTcLcLMathcLcLWrappedTF1;
extern G__linked_taginfo G__G__RemiLN_InterpolatorTF1;
extern G__linked_taginfo G__G__RemiLN_TMatrixTBaselEfloatgR;
extern G__linked_taginfo G__G__RemiLN_TMatrixTBaselEdoublegR;
extern G__linked_taginfo G__G__RemiLN_TH2Fsmooth;
extern G__linked_taginfo G__G__RemiLN_TObjTArrC;
extern G__linked_taginfo G__G__RemiLN_TGraphErrors;
extern G__linked_taginfo G__G__RemiLN_TRemiAcqirisCoMParameter;
extern G__linked_taginfo G__G__RemiLN_TRemiHexAnodeHoleParameter;

/* STUB derived class for protected member access */
typedef TRemiPoint<float,2> G__TRemiPointlEfloatcO2gR;
typedef TRemiPoint<float,3> G__TRemiPointlEfloatcO3gR;
typedef vector<TRemiParticle*,allocator<TRemiParticle*> >::iterator G__vectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgRcLcLiterator;
typedef vector<vector<TRemiParticle*,allocator<TRemiParticle*> >::iterator,allocator<vector<TRemiParticle*,allocator<TRemiParticle*> >::iterator> > G__vectorlEvectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgRcLcLiteratorcOallocatorlEvectorlETRemiParticlemUcOallocatorlETRemiParticlemUgRsPgRcLcLiteratorgRsPgR;
