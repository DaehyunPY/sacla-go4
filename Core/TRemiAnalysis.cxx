//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This is the main analysis class were the individual steps are defined
//----------------------------------------------------------------
#include "TRemiAnalysis.h"

#include "HistExport/TRemiHistExport.h"
#include "Rolling/TRemiRollingRatio.h"
#include "Rolling/TRemiRollingValue.h"
#include "../EventElement/TRemiCalcEvent.h"
#include "../EventElement/TRemiFinishEvent.h"
#include "../EventElement/TRemiUnpackEvent.h"
#include "../EventElement/TRemiExtractEvent.h"
#include "Extern/ConfigFile.h"

#include "TGo4AnalysisStep.h"
#include "TGo4StepFactory.h"
#include "Go4EventServer.h"
#include "TGo4ShapedCond.h"

#include "TH1D.h"
#include "TString.h"
#include "Riostream.h"


//***********************************************************
TRemiAnalysis::TRemiAnalysis() :
	TGo4Analysis(),
	TRemiIncludeFunctions(),
	fUserFile(0),
	fMbsEvent(0),
	fAcqEvent(0),
	fExtractEvent(0),
	fUnpackEvent(0),
	fCalcEvent(0),
	fFinishEvent(0)
{
	cout << "Wrong constructor TRemiAnalysis()!" << endl;
}
//***********************************************************
// this constructor is used
TRemiAnalysis::TRemiAnalysis(int argc, char** argv) :
		TGo4Analysis(argc, argv),
		TRemiIncludeFunctions(),
		fUserFile(0),
	fMbsEvent(0),
	fAcqEvent(0),
	fExtractEvent(0),
	fUnpackEvent(0),
	fCalcEvent(0),
	fFinishEvent(0),
	fSize(0),
	fEvents(0),
	fLastEvent(0)
{
	// input: input file name (*.lmd)
	// out1:  output file name of first analysis step  (*.root)
	// out2:  output file name of second analysis step (*.root)
	cout << "**** TRemiAnalysis: Create" << endl;

	// Build Histogram Export Object;
	fExport = new TRemiHistExport("Par Auto HistExport");

	// Create Rolling Ratio and Value
	fRollingRatio = new TRemiRollingRatio();
	fRollingValue = new TRemiRollingValue();

	// Add an unbound circular FreeShapeCondition for drawing circles onto arbitrary histograms
	fCircleCond = MakeCircleCond("CircleCondition", 48, 0, 0, 40, "");

	// Read from config file which Extract step is used
	fConfigFile = new ConfigFile(MAINCONFIG);
	TString ExtractSourceText = fConfigFile->read<TString>("extract_source", "mbs");

	mExtractSource = -1;
	if (ExtractSourceText == TString("mbs"))
	{
		// ----------------------------------------------------------------------------------------
		// ExtractMbs step
		// ----------------------------------------------------------------------------------------
		TGo4StepFactory*	factory01 = new TGo4StepFactory("ExtractMbsFactory");

		factory01->DefEventProcessor("ExtractMbsProc", "TRemiExtractMbsProc");
		factory01->DefOutputEvent("ExtractEvent", "TRemiExtractEvent");

		TGo4MbsFileParameter* source01 = new TGo4MbsFileParameter();
		TGo4AnalysisStep* step01 = new TGo4AnalysisStep("ExMbs", factory01, source01, 0, 0);

		step01->SetSourceEnabled(kTRUE);
		step01->SetStoreEnabled(kFALSE);
		step01->SetProcessEnabled(kFALSE);
		step01->SetErrorStopEnabled(kTRUE);
		AddAnalysisStep(step01);

		mExtractSource = 0;
	}
	else if (ExtractSourceText == TString("acq_file"))
	{
		// ----------------------------------------------------------------------------------------
		// ExtractAcq step
		// ----------------------------------------------------------------------------------------
		TGo4StepFactory*	factory02 = new TGo4StepFactory("ExtractAcqFactory");

		factory02->DefUserEventSource("TRemiAcqirisFileEventSource");
		factory02->DefInputEvent("AcqEvent","TRemiExtractEvent");
		factory02->DefEventProcessor("ExtractAcqProc","TRemiExtractAcqProc");
		factory02->DefOutputEvent("ExtractEvent","TRemiExtractEvent");

		TGo4UserSourceParameter* source02 = new TGo4UserSourceParameter();
		TGo4AnalysisStep* step02 = new TGo4AnalysisStep("ExAcq",factory02,source02,0,0);

		step02->SetSourceEnabled(kTRUE);
		step02->SetStoreEnabled(kFALSE);
		step02->SetProcessEnabled(kTRUE);
		step02->SetErrorStopEnabled(kTRUE);
		AddAnalysisStep(step02);

		mExtractSource = 1;
	}
	else if (ExtractSourceText == TString("acq_online"))
	{
		// ----------------------------------------------------------------------------------------
		// ExtractAcqRemote step
		// ----------------------------------------------------------------------------------------
		TGo4StepFactory*	factory03 = new TGo4StepFactory("ExtractAcqRFactory");

		factory03->DefUserEventSource("TRemiAcqirisSocketEventSource");
		factory03->DefInputEvent("AcqEvent","TRemiExtractEvent");
		factory03->DefEventProcessor("ExtractAcqRProc","TRemiExtractAcqProc");
		factory03->DefOutputEvent("ExtractEvent","TRemiExtractEvent");

		TGo4UserSourceParameter* source03 = new TGo4UserSourceParameter();
		TGo4AnalysisStep* step03 = new TGo4AnalysisStep("ExAcqR",factory03,source03,0,0);

		step03->SetSourceEnabled(kTRUE);
		step03->SetStoreEnabled(kFALSE);
		step03->SetProcessEnabled(kFALSE);
		step03->SetErrorStopEnabled(kTRUE);
		AddAnalysisStep(step03);

		mExtractSource = 2;
	}

	// ----------------------------------------------------------------------------------------
	// Unpack step
	// ----------------------------------------------------------------------------------------
	TGo4StepFactory*	factory1 = new TGo4StepFactory("UnpackFactory");

	factory1->DefInputEvent("ExtractEvent","TRemiExtractEvent");
	factory1->DefEventProcessor("UnpackProc","TRemiUnpackProc");
	factory1->DefOutputEvent("UnpackEvent","TRemiUnpackEvent");

	TGo4AnalysisStep* step1 = new TGo4AnalysisStep("Unpack",factory1,0,0,0);

	step1->SetSourceEnabled(kFALSE);
	step1->SetStoreEnabled(kFALSE);
	step1->SetProcessEnabled(kTRUE);
	step1->SetErrorStopEnabled(kTRUE);
	AddAnalysisStep(step1);

	// ----------------------------------------------------------------------------------------
// Calc step - gets input from Unpack step
// ----------------------------------------------------------------------------------------
	TGo4StepFactory* factory2 = new TGo4StepFactory("CalcFactory");

	factory2->DefInputEvent("UnpackEvent","TRemiUnpackEvent");
	factory2->DefEventProcessor("CalcProc","TRemiCalcProc");
	factory2->DefOutputEvent("CalcEvent","TRemiCalcEvent");

	TGo4AnalysisStep* step2 = new TGo4AnalysisStep("Calc",factory2,0,0,0);

	step2->SetSourceEnabled(kFALSE);
	step2->SetStoreEnabled(kFALSE);
	step2->SetProcessEnabled(kTRUE);
	step2->SetErrorStopEnabled(kTRUE);
	AddAnalysisStep(step2);


// ----------------------------------------------------------------------------------------
// Finish step - gets input from Calc step
// ----------------------------------------------------------------------------------------
	TGo4StepFactory* factory3 = new TGo4StepFactory("FinishFactory");

	factory3->DefInputEvent("CalcEvent","TRemiCalcEvent");
	factory3->DefEventProcessor("FinishProc","TRemiFinishProc");
	factory3->DefOutputEvent("FinishEvent","TRemiFinishEvent");

	TGo4AnalysisStep* step3 = new TGo4AnalysisStep("Finish",factory3,0,0,0);

	step3->SetSourceEnabled(kFALSE);
	step3->SetStoreEnabled(kFALSE);
	step3->SetProcessEnabled(kTRUE);
	step3->SetErrorStopEnabled(kTRUE);
	AddAnalysisStep(step3);

}

//***********************************************************
TRemiAnalysis::~TRemiAnalysis()
{
	cout << "**** TRemiAnalysis: Delete" << endl;

	delete fExport;
	delete fRollingRatio;
	delete fRollingValue;
	delete fConfigFile;
}
//***********************************************************

//-----------------------------------------------------------
Int_t TRemiAnalysis::UserPreLoop()
{
	cout << "**** TRemiAnalysis: PreLoop" << endl;
	// we update the pointers to the current event structures here:
	if (mExtractSource == 0)
		fMbsEvent = dynamic_cast<TGo4MbsEvent*>    (GetInputEvent("ExtractMbs"));   // of step "ExtractMbs"
	else if (mExtractSource == 1)
		fAcqEvent = dynamic_cast<TRemiExtractEvent*> (GetInputEvent("ExtractAcq"));
	else if (mExtractSource == 2)
		fAcqEvent = dynamic_cast<TRemiExtractEvent*> (GetInputEvent("ExtractAcqR"));
	else
		cout << "**** TRemiAnalysis: Warning: Source type not specified correctly in cMain.conf" << endl;
	fExtractEvent = dynamic_cast<TRemiExtractEvent*> (GetInputEvent("Unpack"));
	fUnpackEvent = dynamic_cast<TRemiUnpackEvent*> (GetInputEvent("Calc"));
	fCalcEvent = dynamic_cast<TRemiCalcEvent*>    (GetInputEvent("Finish"));
	fFinishEvent = dynamic_cast<TRemiFinishEvent*>   (GetOutputEvent("Finish"));
	
	// Check for Parameter macros when reading Root files
	if (fExtractEvent)
		fExtractEvent->CheckForParameterMacro();
	if (fUnpackEvent)
		fUnpackEvent->CheckForParameterMacro();
	if (fCalcEvent)
		fCalcEvent->CheckForParameterMacro();
	
	fEvents=0;
	fLastEvent=0;

	// create histogram for UserEventFunc
	fSize = MakeH1<TH1D> ("Eventsize", "Event size [b]",160,1,160);

	// Load Root Macros
	gROOT->ProcessLine(".L Core/Macros/ExportSettings.C");
	gROOT->ProcessLine(".L Core/Macros/ExportHist.C");
	gROOT->ProcessLine(".L Core/Macros/GenericMacros.C");

	// To enable or disable the execution of conpar_online.C
	// use the corresponding switch in the cMain.conf
	if (fConfigFile->read("use_conpar_online", false))
	{
		gROOT->ProcessLine(".x conpar_online.C()");
		std::cout << "**** TRemiAnalysis: conpar_online macro read!" << std::endl;
	}

//	// Init asymmetry plots
//	InitAsymPlots();

	return 0;
}
//-----------------------------------------------------------
Int_t TRemiAnalysis::UserPostLoop()
{
	cout << "**** TRemiAnalysis: PostLoop" << endl;
	cout << "Last event: " << fLastEvent << " Total events: " << fEvents << endl;
	if(fMbsEvent)
	{
		// we can check some properties of last event here:
		//fMbsEvent->PrintEvent(); // header and data content

		// fileheader structure:
		s_filhe* fileheader=fMbsEvent->GetMbsSourceHeader();
		if(fileheader)
		{
			cout <<"\nInput file was: "<<fileheader->filhe_file << endl;
			cout <<"Tapelabel:\t" << fileheader->filhe_label<<endl;
			cout <<"UserName:\t" << fileheader->filhe_user<<endl;
			cout <<"RunID:\t" << fileheader->filhe_run<<endl;
			cout <<"\tExplanation: "<<fileheader->filhe_exp <<endl;
			cout <<"\tComments: "<<endl;
			Int_t numlines=fileheader->filhe_lines;
			for(Int_t i=0; i<numlines;++i)
				cout<<"\t\t"<<fileheader->s_strings[i].string << endl;
		}

		// mbs buffer header structure:
		s_bufhe* bufheader=fMbsEvent->GetMbsBufferHeader();
		if(bufheader)
		{
			cout <<"Last Buffer:"<<endl;
			cout <<"\tNumber: "<<bufheader->l_buf << endl;
			cout << "\t\t\t + "<<bufheader->l_time[1] << " us"<<endl;
		}

	}

	// Export Histograms
	fExport->AutoExport();

	// Automatically export conditions and parameters to a script file
	gROOT->ProcessLine(".x Core/Macros/ExportSettings.C(\"conpar_online_auto\", \"conpar_backups\", kFALSE)");

//	// Renormalize Asymmetrie plots
//	RenormalizeAsymPlots();

	fMbsEvent = 0; // reset to avoid invalid pointer if analysis is changed in between
	fAcqEvent = 0;
	fExtractEvent = 0;
	fUnpackEvent = 0;
	fCalcEvent = 0;
	fFinishEvent = 0;
	fEvents=0;
	return 0;
}

//-----------------------------------------------------------
Int_t TRemiAnalysis::UserEventFunc()
{
//// This function is called once for each event.
	Int_t value = 0;
	Int_t count = 0;

	if(fMbsEvent) value = fMbsEvent->GetDlen()/2+2; // total longwords
	fSize->Fill(value); // fill histogram
	fEvents++;
	if(fEvents == 1 || IsNewInputFile())
	{
		if(fMbsEvent)
		{
			count=fMbsEvent->GetCount();
			cout << "\nFirst event #: " << count  << endl;
			s_bufhe* bufheader=fMbsEvent->GetMbsBufferHeader();
			if(bufheader)
			{
				cout <<"First Buffer:"<<endl;
				cout <<"\tNumber: "<<bufheader->l_buf << endl;
				cout << "\t\t\t + "<<bufheader->l_time[1] << " us"<<endl;
			}
	// Search for Makros to be executed before the reading Datafile
			s_filhe* fileheader=fMbsEvent->GetMbsSourceHeader();
			if(fileheader)
			{
				TString filename = (TString)fileheader->filhe_file;
				cout <<"\nInput file is: "<< filename << endl;
				Int_t a=-1,b;
				do
				{
					b = a; // b gets last value of a
					a = filename.Index(".", b + 1); // start searching for next dot starting from last occurence
				}
				while (a != -1);
				filename.Resize(b); // truncate filename up to the last dot
				filename += ".C";
				cout << "Looking for makro " << filename  << endl;
				ifstream fFile(filename);
				if(fFile)
				{
					fFile.close();
					cout << "*** TRemiUnpackEvent: Found macro " << filename << endl;
					TString command = ".x " + filename;
					gROOT->ProcessLine(command);
				}
			}
		}
		SetNewInputFile(kFALSE); // we have to reset the newfile flag
	}

	fRollingRatio->Update();
	fRollingValue->Update();

	fLastEvent = count;
	return 0;
}


///**
//* \brief Initialize asymmetry plots
//*
//* Activates the saving of square sum of weights for all histograms containing the string "Asym" in their name. This is necessary to correctly renormalize those plots to relative asymmetries.
//* Additionally, the asymmetry plots are cleared.
//*/
//void TRemiAnalysis::InitAsymPlots()
//{
//	TObject *obj = 0;
//	TH1 *hist = 0;
//	Bool_t reset=kTRUE;
//	while ((obj = TGo4Analysis::Instance()->NextMatchingObject("*Asym*", "Histograms", reset)))
//	{
//		reset = kFALSE;
//		hist = dynamic_cast<TH1*>(obj);
//		if (hist)
//		{
//			std::cout << "**** " << __FILE__ << ": Found asymmetry histogram " << hist->GetName() << std::endl;
//			hist->Sumw2();
//			hist->Reset("M");
//		}
//	}
//}


///**
//* \brief Renormalizes assymmetry plots
//*
//* Renormalizes each bin in all existing asymmetry (those that contain the string "Asym" in their name) plots by dividing it by the saved sum of squared weights. Then each bin will contain a relative asymmetry defined as A- - A+ / (A- + A+) if it was filled with weights -1 and +1 for negative and positive incidents, respectively.
//*/
//void TRemiAnalysis::RenormalizeAsymPlots()
//{
//	TObject *obj = 0;
//	TH1  *hist =0;
//	Bool_t reset=kTRUE;
//	while ((obj = TGo4Analysis::Instance()->NextMatchingObject("*Asym*", "Histograms", reset)))
//	{
//		reset = kFALSE;
//		hist = dynamic_cast<TH1*>(obj);
//		if (hist)
//		{
//			UInt_t NTotalBins = dynamic_cast<TArray*>(obj)->GetSize();
//			Double_t scalefactor;
//			for (UInt_t i=0; i<NTotalBins; i++)
//			{
//				scalefactor = TMath::Power(hist->GetBinError(i),2);
//				if (scalefactor != 0) // avoid division by zero. If bin error is zero (means also bin content is zero) it will be unchanged
//				{
//					hist->SetBinContent(i, hist->GetBinContent(i) / scalefactor);
//					hist->SetBinError(i, hist->GetBinError(i) / scalefactor);
//				}
//			}
//			std::cout << "**** " << __FILE__ << ": Renormalized asymmetry histogram " << hist->GetName() << std::endl;
//		}
//	}
//}


//----------------------------END OF GO4 SOURCE FILE ---------------------


