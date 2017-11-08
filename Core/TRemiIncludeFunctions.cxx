//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Base class for all event processing classes
//		Contains many functions useful in handling of histograms, parameters, conditions and more
//----------------------------------------------------------------
#include "TRemiIncludeFunctions.h"

#include "TRemiBasicParameter.h"
#include "RootMod/TRemiRollingGraph.h"

#include "TGo4PolyCond.h"
#include "TGo4CondArray.h"
#include "TGo4WinCond.h"
#include "TGo4Picture.h"

#ifndef kWINDOW
#define kWINDOW kGO4CONDWINDOW
#endif

#ifndef kPOLYGON
#define kPOLYGON kGO4CONDPOLYGON
#endif

#include "TCutG.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TGraph.h"
#include "TString.h"
#include "TObject.h"
#include "TLine.h"
#include "TArc.h"
#include "TList.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

/**
 * \brief Default constructor
 * 
 * Will lead the config file to be uninitialised!
 */
TRemiIncludeFunctions::TRemiIncludeFunctions()
{
	fHistoStore = std::vector<TH1*> ();
	fConditionStore = std::vector<TGo4Condition*> ();
	fBasicParameterStore = std::vector<TRemiBasicParameter*> ();
	fConfigFile = 0;
}

/**
 * \brief Constructor with initialisation of Config file
 */
TRemiIncludeFunctions::TRemiIncludeFunctions(const char* config_name)
{
	fHistoStore = std::vector<TH1*> ();
	fConditionStore = std::vector<TGo4Condition*> ();
	fBasicParameterStore = std::vector<TRemiBasicParameter*> ();
	fConfigFile = new ConfigFile(config_name);
}

/**
 * \brief Constructor using existing pointer to a Config file
 */
TRemiIncludeFunctions::TRemiIncludeFunctions(ConfigFile* ConfPointer)
{
	fHistoStore = std::vector<TH1*> ();
	fConditionStore = std::vector<TGo4Condition*> ();
	fBasicParameterStore = std::vector<TRemiBasicParameter*> ();
	fConfigFile = ConfPointer;
}


TRemiIncludeFunctions::~TRemiIncludeFunctions()
{
	fConditionStore.clear();
	fBasicParameterStore.clear();
	fHistoStore.clear();
}

/**
* \brief Function to make Root Graph objects
* \param name Internal name of the graph
* \param folder Folder to place the graph in the Go4 browser tree
*/
TGraph* TRemiIncludeFunctions::MakeGraph(TString name, TString title, TString folder) const
{
	return MakeGraph(name, title, 0, 0, 0, folder);
}

/**
* \brief Function to make Root Graph objects
* \param name Internal name of the graph
* \param folder (optional) Folder to place the graph in the Go4 browser tree
*/
TGraph* TRemiIncludeFunctions::MakeGraph(TString name, TString title, Int_t points, Double_t* xvalues, Double_t* yvalues, TString folder) const
{
	TGraph* graph = dynamic_cast<TGraph*> (TGo4Analysis::Instance()->GetObject( name ) );
	if (!graph)
	{
		if (points==0)
			graph = new TGraph ();
		else
			graph = new TGraph (points, xvalues, yvalues);
		graph->SetName(name);
		graph->SetTitle(title);
		if (folder.Length() != 0)
			TGo4Analysis::Instance()->AddObject(graph, folder);
		else
			TGo4Analysis::Instance()->AddObject(graph);
	}
	return graph;
}

/**
* \brief Function to make Root Graph objects using a TF1 function as input
* \param name Internal name of the graph
* \param function Pointer to existing TF1 function object
* \param folder (optional) Folder to place the graph in the Go4 browser tree
*/
TGraph* TRemiIncludeFunctions::MakeGraph(TString name, TString title, TF1* function, TString folder) const
{
	TGraph* graph = dynamic_cast<TGraph*> (TGo4Analysis::Instance()->GetObject( name ) );
	if (!graph)
	{
		graph = new TGraph (function);
		graph->SetName(name);
		graph->SetTitle(title);
		if (folder.Length() != 0)
			TGo4Analysis::Instance()->AddObject(graph, folder);
		else
			TGo4Analysis::Instance()->AddObject(graph);
	}
	return graph;
}

/**
* \brief Function to make a Rolling Graph object (for monitoring of a value)
* \param name Internal name of the graph
* \param title Long name (description) of the graph
* \param points Number of points to display
* \param average Number of values to average over for each point in the graph
* \param folder (optional) Folder to place the graph in the Go4 browser tree
*/
TRemiRollingGraph* TRemiIncludeFunctions::MakeRollingGraph(TString name, TString title, Int_t points, Int_t average, TString folder) const
{
	TRemiRollingGraph* graph = dynamic_cast<TRemiRollingGraph*> (TGo4Analysis::Instance()->GetObject( name ) );
	if (!graph) 
	{
		graph = new TRemiRollingGraph (points, average);
		graph->SetName(name);
		graph->SetTitle(title);
		if (folder.Length() != 0)
			TGo4Analysis::Instance()->AddObject(graph, folder);
		else
			TGo4Analysis::Instance()->AddObject(graph);
	}
	return graph;
}

/**
 * \brief Function to make a basic one-value parameter object
 * \param name Short name of the parameter
 * \param title Long description of the parameter
 * \param folder (optional) Folder to place the parameter in the Go4 browser tree
 */
TRemiBasicParameter* TRemiIncludeFunctions::MakeBasicParam(TString name, TString title, TString folder) const
{
	TRemiBasicParameter* parameter = dynamic_cast<TRemiBasicParameter*> (TGo4Analysis::Instance()->GetParameter( name ));
	if (!parameter)
	{
		parameter = new TRemiBasicParameter(name, title);
		if (folder.Length()!=0)
			TGo4Analysis::Instance()->AddParameter(parameter, folder);
		else
			TGo4Analysis::Instance()->AddParameter(parameter);
	}
	return parameter;
}

/**
 * \brief Allows to directly use the value of a single-value basic parameter
 * \param name Short name of the parameter
 * \param title Long description of the parameter
 * \param folder (optional) Folder to place the parameter in the Go4 browser tree
 * 
 * Parameter will be accessible in the GUI
 */
Double_t TRemiIncludeFunctions::UseBasicParamValue(TString name, TString title, TString folder)
{
	TRemiBasicParameter* parameter = dynamic_cast<TRemiBasicParameter*> (TGo4Analysis::Instance()->GetParameter( MakeName(name, folder) ));
	if (!parameter)
	{
		parameter = new TRemiBasicParameter(name, title);
		if (folder.Length()!=0)
			TGo4Analysis::Instance()->AddParameter(parameter, folder);
		else
			TGo4Analysis::Instance()->AddParameter(parameter);
		fBasicParameterStore.push_back(parameter);
	}
	return parameter->Value;
}

/**
* \brief Function to make 2D Window conditions
* \param name Short (internal) name of condition
* \param title Long description of conditions
* \param left1 First lower border of default condition
* \param right1 First upper border of default condition
* \param left2 Second lower border of default condition
* \param right2 Second upper border of default condition
* \param histo_name (optional) Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
TGo4WinCond* TRemiIncludeFunctions::MakeWinCon(TString name, TString title, Double_t left1, Double_t right1, Double_t left2, Double_t right2, TString histo_name, TString folder) const
{
	TGo4WinCond* wincond = dynamic_cast<TGo4WinCond*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (wincond) {
		wincond->ResetCounts();
	} else {
		if (title.Length() == 0) {
			title = name + " window condition";
		}
		wincond = new TGo4WinCond(name, title); 
		wincond->SetValues(left1, right1, left2, right2);
		wincond->Enable();
		AddCondition(wincond, folder);
	}
	if (histo_name.Length() != 0) {
		wincond->SetHistogram(histo_name); // link with histogram
	}
	return wincond;
}

/**
* \brief Function to make Window conditions
* \param name Short (internal) name of condition
* \param title Long description of conditions
* \param left Lower border of default condition
* \param right Upper border of default condition
* \param histo_name (optional) Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
TGo4WinCond* TRemiIncludeFunctions::MakeWinCon(TString name, TString title, Double_t left, Double_t right, TString histo_name, TString folder) const
{
	TGo4WinCond* wincond = dynamic_cast<TGo4WinCond*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (wincond) {
		wincond->ResetCounts();
	} else {
		if (title.Length() == 0) {
			title = name + " window condition";
		}
		wincond = new TGo4WinCond(name, title); 
		wincond->SetValues(left, right);
		wincond->Enable();
		AddCondition(wincond, folder);
	}
	if (histo_name.Length() != 0) {
		wincond->SetHistogram(histo_name); // link with histogram
	}
	return wincond;
}

/**
* \brief Function to make Window conditions initialised with borders of the linked histogram
* \param name Short (internal) name of condition
* \param title Long description of conditions
* \param histo_name Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
TGo4WinCond* TRemiIncludeFunctions::MakeWinCon(TString name, TString title, TString histo_name, TString folder) const
{
	TGo4WinCond* wincond = dynamic_cast<TGo4WinCond*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (wincond) {
		wincond->ResetCounts();
	} else {
		if (title.Length() == 0) {
			title = name + " window condition";
		}
		wincond = new TGo4WinCond(name, title); 

		TH1* hist = TGo4Analysis::Instance()->GetHistogram(histo_name);
		if (hist)
		{
			if (hist->GetDimension() > 1) {
				wincond->SetValues(hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(), hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());
			} else {
				wincond->SetValues(hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
			}
			wincond->SetHistogram(histo_name); // link with histogram
		}

		wincond->Enable();
		AddCondition(wincond, folder);
	}
	return wincond;
}

/**
* \brief Function to make Polygon condition from lists of x and y values				
* \param name Short (internal) name of condition
* \param title Long description of conditions
* \param points (N+1) where N is the no. of points in initial polygon
* \param xvalues X values of polygon points, has to be array of size \c points Last value has to match first
* \param yvalues Y values of polygon points, has to be array of size \c points Last value has to match first
* \param histo_name (optional) Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
TGo4PolyCond* TRemiIncludeFunctions::MakePolyCon(TString name, TString title, Int_t points, Double_t* xvalues, Double_t* yvalues, TString histo_name, TString folder) const
{
	TGo4PolyCond* polycond = dynamic_cast<TGo4PolyCond*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (polycond) {
		polycond->ResetCounts();
	} else {
		if (title.Length() == 0) {
			title = name + " window condition";
		}
		polycond = new TGo4PolyCond(name, title); 
		polycond->SetValues(xvalues, yvalues, points);
		polycond->Enable();
		AddCondition(polycond, folder);
	}
	if (histo_name.Length() != 0) {
		polycond->SetHistogram(histo_name); // link with histogram
	}
	return polycond;
}

/**
* \brief Function to make Polygon condition from a TCutG object				
* \param name Short (internal) name of condition
* \param title Long description of conditions
* \param cut TCutG with the initial polygon for all conditions
* \param histo_name (optional) Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
TGo4PolyCond* TRemiIncludeFunctions::MakePolyCon(TString name, TString title, TCutG* cut, TString histo_name, TString folder) const
{
	TGo4PolyCond* polycond = dynamic_cast<TGo4PolyCond*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (polycond) {
		polycond->ResetCounts();
	} else {
		if (title.Length() == 0) {
			title = name + " window condition";
		}
		polycond = new TGo4PolyCond(name, title); 
		polycond->SetValues(cut);
		polycond->Enable();
		AddCondition(polycond, folder);
	}
	if (histo_name.Length() != 0) {
		polycond->SetHistogram(histo_name); // link with histogram
	}
	return polycond;
}

/**
* \brief Function to make rectanglular Polygon condition initialised with corners of the linked histogram
* \param name Short (internal) name of condition
* \param title Long description of conditions
* \param histo_name (optional) Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
TGo4PolyCond* TRemiIncludeFunctions::MakePolyCon(TString name, TString title, TString histo_name, TString folder) const
{
	TGo4PolyCond* polycond = dynamic_cast<TGo4PolyCond*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (polycond) {
		polycond->ResetCounts();
	} else {
		if (title.Length() == 0) {
			title = name + " window condition";
		}
		polycond = new TGo4PolyCond(name, title);

		TH1* hist = TGo4Analysis::Instance()->GetHistogram(histo_name);
		if (hist)
		{
			Double_t hist_x_min = hist->GetXaxis()->GetXmin();
			Double_t hist_x_max = hist->GetXaxis()->GetXmax();
			Double_t hist_y_min = hist->GetYaxis()->GetXmin();
			Double_t hist_y_max = hist->GetYaxis()->GetXmax();

			const Int_t points = 5;
			Double_t xvalues[points] = {hist_x_min, hist_x_min, hist_x_max, hist_x_max, hist_x_min};
			Double_t yvalues[points] = {hist_y_min, hist_y_max, hist_y_max, hist_y_min, hist_y_min};

			polycond->SetValues(xvalues, yvalues, points);
			polycond->SetHistogram(histo_name); // link with histogram
		}

		polycond->Enable();
		AddCondition(polycond, folder);
	}
	return polycond;
}

/**
* \brief Function to make circular (as 30 points polygon) condition from given radius and central coordinate
* \param name Short (internal) name of condition
* \param title Long description of conditions
* \param rad radius of the circle
* \param x0 X value of the central coordinate
* \param y0 Y value of the central coordinate
* \param histo_name (optional) Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
TGo4PolyCond* TRemiIncludeFunctions::MakeCircCon(TString name, TString title, Double_t rad, Double_t x0, Double_t y0, TString histo_name, TString folder) const
{
	const Int_t n = 30; //i.e., 12 degrees per slice
	Double_t x[n+1], y[n+1];
	Double_t dphi = TMath::TwoPi()/n;
	for (Int_t i=0; i<n; i++)
	{
		x[i] = x0 + rad * TMath::Cos(i*dphi);
		y[i] = y0 + rad * TMath::Sin(i*dphi);
	}
	x[n] = x[0];
	y[n] = y[0];

	return MakePolyCon(name, title, n+1, x, y, histo_name, folder);
}

/**
* \brief Function to make standard window condition array with separated low and high values for each condition
* \param name Short (internal) name of condition
* \param elements No. of conditions in this array
* \param lowvalues Lower borders of default conditions, has to be array of size \c elements
* \param highvalues Upper borders of default conditions, has to be array of size \c elements
* \param histo_name (optional) Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
TGo4CondArray* TRemiIncludeFunctions::MakeWinConArray(TString name, Int_t elements, Double_t* lowvalues, Double_t* highvalues, TString histo_name, TString folder) const
{
	TString fullname = MakeName(name, folder);
	TGo4CondArray* condarray = dynamic_cast<TGo4CondArray*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (condarray) {
		condarray->ResetCounts();
	} else {
		condarray = new TGo4CondArray(name, elements, kWINDOW);
		condarray->SetValues(lowvalues[0], highvalues[elements-1]);
		for (Int_t i=0; i<elements; i++) {
			(condarray->At(i))->SetValues(lowvalues[i],highvalues[i]);
		}
		condarray->Enable();
		AddCondition(condarray, folder);
	}
	if (histo_name.Length() != 0) {
		condarray->SetHistogram(histo_name); // link with histogram
	}
	return condarray;
}

/**
* \brief Function to make standard window condition array initialised with the same borders for each condition
* \param name Short (internal) name of condition
* \param elements No. of conditions in this array
* \param xlow Lower border of default conditions
* \param xup Upper border of default conditions
* \param histo_name (optional) Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
TGo4CondArray* TRemiIncludeFunctions::MakeWinConArray(TString name, Int_t elements, Double_t xlow, Double_t xup, TString histo_name, TString folder) const
{
	TString fullname = MakeName(name, folder);
	TGo4CondArray* condarray = dynamic_cast<TGo4CondArray*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (condarray) {
		condarray->ResetCounts();
	} else {
		condarray = new TGo4CondArray(name, elements, kWINDOW);
		condarray->SetValues(xlow, xup);
		for (Int_t i=0; i<elements; i++) {
			(condarray->At(i))->SetValues(xlow,xup);
		}
		condarray->Enable();
		AddCondition(condarray, folder);
	}
	if (histo_name.Length() != 0) {
		condarray->SetHistogram(histo_name); // link with histogram
	}
	return condarray;
}

/**
* \brief Function to make standard window condition array initialised with the borders of the linked histogram
* \param name Short (internal) name of condition
* \param elements No. of conditions in this array
* \param histo_name Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
TGo4CondArray* TRemiIncludeFunctions::MakeWinConArray(TString name, Int_t elements, TString histo_name, TString folder) const
{
	TString fullname = MakeName(name, folder);
	TGo4CondArray* condarray = dynamic_cast<TGo4CondArray*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (condarray) {
		condarray->ResetCounts();
	} else {
		condarray = new TGo4CondArray(name, elements, kWINDOW);

		TH1* hist = TGo4Analysis::Instance()->GetHistogram(histo_name);
		if (hist)
		{
			condarray->SetValues(hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
			for (Int_t i=0; i<elements; i++) {
				(condarray->At(i))->SetValues(hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
			}
			condarray->SetHistogram(histo_name); // link with histogram
		}

		condarray->Enable();
		AddCondition(condarray, folder);
	}
	return condarray;
}

/**
 * \brief Function to make incremental window condition array where the high value of cond. i is the low value of cond. i+1
 * \param name Short (internal) name of condition
 * \param elements No. of conditions in this array
 * \param values Borders of default conditions, has to be array of size (\c elements + 1)
 * \param histo_name (optional) Name of histogram to link condition with
 * \param folder (optional)  Folder to place the condition in the Go4 browser tree
 */
TGo4CondArray* TRemiIncludeFunctions::MakeIncWinCondArray(TString name, Int_t elements, Double_t* values, TString histo_name, TString folder) const
{
	TGo4CondArray* condarray = dynamic_cast<TGo4CondArray*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (condarray) {
		condarray->ResetCounts();
	} else {
		condarray = new TGo4CondArray(name, elements, kWINDOW);
		condarray->SetValues(values[0], values[elements]);
		for (Int_t i=0; i<elements; i++) {
			(condarray->At(i))->SetValues(values[i],values[i+1]);
		}
		condarray->Enable();
		AddCondition(condarray, folder);
	}
	if (histo_name.Length() != 0) {
		condarray->SetHistogram(histo_name); // link with histogram
	}
	return condarray;
}

/**
 * \brief Function to make polygon condition array where all conditions are initialized with the same cut
 * \param name Short (internal) name of condition
 * \param elements No. of conditions in this array
 * \param cut TCutG with the initial polygon for all conditions
 * \param histo_name (optional) Name of histogram to link condition with
 * \param folder (optional)  Folder to place the condition in the Go4 browser tree
 */
TGo4CondArray* TRemiIncludeFunctions::MakePolyConArray(TString name, Int_t elements, TCutG* cut, TString histo_name, TString folder) const
{
	TString fullname = MakeName(name, folder);
	TGo4CondArray* condarray = dynamic_cast<TGo4CondArray*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (condarray) {
		condarray->ResetCounts();
	} else {
		condarray = new TGo4CondArray(name, elements, kPOLYGON);
		condarray->SetValues(cut);
		condarray->Enable();
		AddCondition(condarray, folder);
	}
	if (histo_name.Length() != 0) {
		condarray->SetHistogram(histo_name); // link with histogram
	}
	return condarray;
}

/**
* \brief Function to make polygon condition array where all conditions are initialized with the same list of x and y values
* \param name Short (internal) name of condition
* \param elements No. of conditions in this array
* \param points (N+1) where N is the no. of points in initial polygon
* \param xvalues X values of polygon points, has to be array of size \c points Last value has to match first
* \param yvalues Y values of polygon points, has to be array of size \c points Last value has to match first
* \param histo_name (optional) Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
TGo4CondArray* TRemiIncludeFunctions::MakePolyConArray(TString name, Int_t elements, Int_t points, Double_t* xvalues, Double_t* yvalues, TString histo_name, TString folder) const
{
	TString fullname = MakeName(name, folder);
	TGo4CondArray* condarray = dynamic_cast<TGo4CondArray*> (TGo4Analysis::Instance()->GetAnalysisCondition( name ));
	if (condarray) {
		condarray->ResetCounts();
	} else {
		condarray = new TGo4CondArray(name, elements, kPOLYGON);
		TCutG *cut = new TCutG ("polycondcut", points, xvalues, yvalues);
		condarray->SetValues(cut);
		delete cut;
		condarray->Enable();
		AddCondition(condarray, folder);
	}
	if (histo_name.Length() != 0) {
		condarray->SetHistogram(histo_name); // link with histogram
	}
	return condarray;
}

/**
 * \brief Function to make a Go4Picture
 * \param name Internal name of the picture
 * \param title Internal description
 * \param folder Folder in the object tree
 * \param numY (optional) number of pads in y direction. Preset value is 1
 * \param numX (optional) number of pads in x direction. Preset value is 1
 */
TGo4Picture* TRemiIncludeFunctions::MakePicture(TString name, TString title, TString folder, Int_t numY, Int_t numX) const
{
	TGo4Picture* picture = TGo4Analysis::Instance()->GetPicture( name );
	if (!picture)
	{
		picture = new TGo4Picture (name, title, numY, numX);
		if (folder.Length() != 0 )
			TGo4Analysis::Instance()->AddPicture(picture, folder);
		else
			TGo4Analysis::Instance()->AddPicture(picture);
	}
	return picture;
}

/**
 * \brief Adds a histogram to a TGo4Picture
 * \param picture Pointer to the Picture
 * \param hist Pointer to the histogram
 * \param posY (Default=0) Row of the slot to add the histogram to
 * \param posX (Default=0) Column of the slot to add the histogram to
 * 
 * If there already exists an histogram in the picture, a superposition of histograms will be created.
 * 
 * A check is performed that the same histogram will not be added again
 */
void TRemiIncludeFunctions::AddHistToPicture(TH1* hist, TGo4Picture* picture, Int_t posY, Int_t posX) const
{
	if ( (picture != 0) && (picture->Pic(posY, posX) != 0) && (hist != 0) )
	{
		Int_t numobj = picture->Pic(posY, posX)->GetNumObjNames();
		if (numobj==0)
			picture->Pic(posY, posX)->AddObject(hist);
		else
		{
			Bool_t existing = kFALSE;
			for (Int_t i=0; i<numobj; i++)
				if (TString(hist->GetName()) == TString(picture->Pic(posY, posX)->GetObjName(i)) )
					existing = kTRUE;
			
			if ( existing == kFALSE )
			{
				picture->SetSuperimpose(true);
				picture->Pic(posY, posX)->AddObject(hist);
			}
		}
	}
}

/**
* \brief Adds a histogram identified by its name to a TGo4Picture
* \param picture Pointer to the Picture
* \param histname Name of the histogram
* \param posY (Default=0) Row of the slot to add the histogram to
* \param posX (Default=0) Column of the slot to add the histogram to
* 
* If there already exists an histogram in the picture, a superposition of histograms will be created.
* 
* A check is performed that the same histogram will not be added again
*/
void TRemiIncludeFunctions::AddHistToPicture(TString histname, TGo4Picture* picture, Int_t posY, Int_t posX) const
{
	TH1* hist = TGo4Analysis::Instance()->GetHistogram(histname);
	if (hist)
		AddHistToPicture(hist, picture, posY, posX);
}

/**
* \brief Adds a Root graph to a TGo4Picture
* \param picture Pointer to the Picture
* \param graph Pointer to the graph
* \param posY (Default=0) Row of the slot to add the histogram to
* \param posX (Default=0) Column of the slot to add the histogram to
* 
* If there already exists an histogram in the picture, a superposition of histograms will be created.
* 
* A check is performed that the same graph will not be added again
*/
void TRemiIncludeFunctions::AddGraphToPicture(TGraph* graph, TGo4Picture* picture, Int_t posY, Int_t posX) const
{
	if ( (picture != 0) && (picture->Pic(posY, posX) != 0) && (graph != 0) )
	{
		Int_t numobj = picture->Pic(posY, posX)->GetNumObjNames();
		if (numobj==0)
			picture->Pic(posY, posX)->AddObject(graph);
		else
		{
			Bool_t existing = kFALSE;
			for (Int_t i=0; i<numobj; i++)
				if (TString(graph->GetName()) == TString(picture->Pic(posY, posX)->GetObjName(i)) )
					existing = kTRUE;
				
				if ( existing == kFALSE )
				{
					picture->SetSuperimpose(true);
					picture->Pic(posY, posX)->AddObject(graph);
				}
		}
	}
}

/**
 * \brief Adds a combination of histogram and condition to a Go4Picture
 * \param picture Pointer to the Picture
 * \param cond Pointer to the condition
 * \param posY (Default=0) Row of the slot to add the histogram to
 * \param posX (Default=0) Column of the slot to add the histogram to
 * 
 * The condition \c cond has to have a histogram linked to it. This will be shown in the picture. If no linked histogram is found for the condition, nothing will be added to the picture.
 * 
 * If there already exists any object in the desired slot, nothing will be added.
 */
void TRemiIncludeFunctions::AddCondToPicture(TGo4Condition* cond, TGo4Picture* picture, Int_t posY, Int_t posX) const
{
	if (picture != 0 && (picture->Pic(posY, posX) != 0)) 
		if (picture->Pic(posY, posX)->GetObjName(0)==0) 
			if (cond != 0) 
				if (cond->GetLinkedHistogram() != 0) 
	{
		TH1* hist = TGo4Analysis::Instance()->GetHistogram(cond->GetLinkedHistogram());
		if (hist != 0)
		{
			picture->Pic(posY, posX)->AddObject(cond);
			picture->Pic(posY, posX)->AddObject(hist);
		}
	}
}

/**
* \brief Adds a combination of histogram and condition to a Go4Picture by condition name
* \param picture Pointer to the Picture
* \param condname Name of the condition
* \param posY (Default=0) Row of the slot to add the histogram to
* \param posX (Default=0) Column of the slot to add the histogram to
* 
* The condition \c cond has to have a histogram linked to it. This will be shown in the picture. If no linked histogram is found for the condition, nothing will be added to the picture.
* 
* If there already exists any object in the desired slot, nothing will be added.
*/
void TRemiIncludeFunctions::AddCondToPicture(TString condname, TGo4Picture* picture, Int_t posY, Int_t posX) const
{
	TGo4Condition* cond = TGo4Analysis::Instance()->GetAnalysisCondition(condname);
	if (cond)
		AddCondToPicture(cond, picture, posY, posX);
}


/**
* \brief Adds a simple Line from a TLine object to a Go4Picture
* \param picture Pointer to the Picture
* \param line Pointer to the line object
* \param posY (Default=0) Row of the slot to add the histogram to
* \param posX (Default=0) Column of the slot to add the histogram to
*
* No check for existence of the line is performed
* Ownership of line is transferred to picture, no delete necessary
*/
void TRemiIncludeFunctions::AddLineToPicture(TLine* line, TGo4Picture* picture, Int_t posY, Int_t posX) const
{
	if (picture != 0 && (picture->Pic(posY, posX) != 0) && line != 0)
		picture->Pic(posY, posX)->AddSpecialObject(line, "same");
}

/**
* \brief Adds a simple Line entered as two points to a Go4Picture
* \param picture Pointer to the Picture
* \param x1 x-value of first point
* \param y1 y-value of first point
* \param x2 x-value of second point
* \param y2 y-value of second point
* \param posY (Default=0) Row of the slot to add the histogram to
* \param posX (Default=0) Column of the slot to add the histogram to
* \param colour (Default=1) Colour of the line, see http://root.cern.ch/root/html/TAttLine.html#L1
* \param style (Default=1) Line style, see http://root.cern.ch/root/html/TAttLine.html#L3
* \param width (Default=1) Line width, see http://root.cern.ch/root/html/TAttLine.html#L2
*
* No check for existence of the line is performed
*/
void TRemiIncludeFunctions::AddLineToPicture(Double_t x1, Double_t y1, Double_t x2, Double_t y2, TGo4Picture* picture, Int_t posY, Int_t posX, Color_t colour, Style_t style, Width_t width) const
{
	TLine *line = new TLine(x1, y1, x2, y2); // Ownership of line is transferred to picture, no delete necessary
	line->SetLineColor(colour);
	line->SetLineStyle(style);
	line->SetLineWidth(width);
	AddLineToPicture(line, picture, posY, posX);
}

/**
* \brief Adds a 1D function to a Go4Picture
* \param funct Pointer to the TF1 function (Root) object
* \param picture Pointer to the Picture
* \param posY (Default=0) Row of the slot to add the histogram to
* \param posX (Default=0) Column of the slot to add the histogram to
* 
* Only adds function if no function of the same name exists in the picture
* Ownership of funct is transferred to picture, no delete necessary
*/
void TRemiIncludeFunctions::AddTF1ToPicture ( TF1* funct, TGo4Picture* picture, Int_t posY, Int_t posX ) const
{
	if (picture != 0 && (picture->Pic(posY, posX) != 0) && funct != 0)
	{
		TList* specialObjects = picture->Pic(posY, posX)->GetSpecialObjects();
		if (!( (specialObjects) && (specialObjects->FindObject(funct->GetName())) ))
			picture->Pic(posY, posX)->AddSpecialObject(funct, "same");
	}
}

/**
* \brief Adds a line defined by points as a TF1 object to a Go4Picture
* \param x1 x-value of first point
* \param y1 y-value of first point
* \param x2 x-value of second point
* \param y2 y-value of second point
* \param picture Pointer to the Picture
* \param posY (Default=0) Row of the slot to add the histogram to
* \param posX (Default=0) Column of the slot to add the histogram to
* 
* TF1 lines have a few advantages over conventional lines, therefore this might be useful
*/
void TRemiIncludeFunctions::AddTF1LineToPicture ( Double_t x1, Double_t y1, Double_t x2, Double_t y2, TGo4Picture* picture, Int_t posY, Int_t posX ) const
{
	if ( picture != 0) 
	{
		if (x1==x2) // avoid zero division
			x2 = x1 + (y2-y1)*1e-5;
		Double_t sloap = (y2-y1) / (x2-x1);
		Double_t offset = y1 - sloap*x1;
		Double_t xlow = (x1<x2) ? x1 : x2;
		Double_t xup = (x1<x2) ? x2 : x1;
		Int_t newpos;
		TList* specialObjects = picture->Pic(posY, posX)->GetSpecialObjects();
		if (specialObjects)
			newpos = specialObjects->GetSize();
		else
			newpos = 0;
		TString name = "TF1funct"; name += newpos;
		TF1* funct = new TF1(name, "[0]+[1]*x", xlow, xup);
		funct->SetParameter(0, offset);
		funct->SetParameter(1, sloap);
		funct->SetLineWidth(1);
		AddTF1ToPicture(funct, picture, posY, posX);
	}
}

/**
* \brief Adds a simple Arc from a TArc object to a Go4Picture
* \param picture Pointer to the Picture
* \param arc Pointer to the arc object
* \param posY (Default=0) Row of the slot to add the histogram to
* \param posX (Default=0) Column of the slot to add the histogram to
*
* No check for existence of the arc is performed
* Ownership of arc is transferred to picture, no delete necessary
*/
void TRemiIncludeFunctions::AddArcToPicture(TArc* arc, TGo4Picture* picture, Int_t posY, Int_t posX) const
{
	if (picture != 0 && (picture->Pic(posY, posX) != 0) && arc != 0)
		picture->Pic(posY, posX)->AddSpecialObject(arc, "same");
}

/**
 * \brief Adds a simple arc entered by center, radius and angular range to a Go4Picture
 * \param picture Pointer to the Picture
 * \param x1 x-value of center
 * \param y1 y-value of center
 * \param radius radius of circle
 * \param phi1 starting angle of the arc
 * \param phi2 terminating angle of the arc
 * \param posY (Default=0) Row of the slot to add the histogram to
 * \param posX (Default=0) Column of the slot to add the histogram to
 * \param fillStyle (Default=0) Style to fill the object, see http://root.cern.ch/root/html/TAttFill.html#F2
 * 
 * No check for existence of the arc is performed
 */
void TRemiIncludeFunctions::AddArcToPicture(Double_t x1, Double_t y1, Double_t radius, Double_t phi1, Double_t phi2, TGo4Picture* picture, Int_t posY, Int_t posX, Style_t fillStyle) const
{
	TArc *arc = new TArc(x1, y1, radius, phi1, phi2);
	arc->SetFillStyle(fillStyle);
	AddArcToPicture(arc, picture, posY, posX);
}

/**
 * \brief Adds a simple circle entered by center and radius to a Go4Picture
 * \param picture Pointer to the Picture
 * \param x1 x-value of center
 * \param y1 y-value of center
 * \param radius radius of circle
 * \param posY (Default=0) Row of the slot to add the histogram to
 * \param posX (Default=0) Column of the slot to add the histogram to
 * \param fillStyle (Default=0) Style to fill the object, see http://root.cern.ch/root/html/TAttFill.html#F2
 * 
 * No check for existence of the arc is performed
 */
void TRemiIncludeFunctions::AddCircleToPicture(Double_t x1, Double_t y1, Double_t radius, TGo4Picture* picture, Int_t posY, Int_t posX, Style_t fillStyle) const
{
	TArc *arc = new TArc(x1, y1, radius);
	arc->SetFillStyle(fillStyle);
	AddArcToPicture(arc, picture, posY, posX);
}

/**
* \brief Removes all special objects from a picture, i.e. all Lines, Circles etc,
* \param picture Pointer to the Picture
* \param posY (Default=0) Row of the slot to add the histogram to
* \param posX (Default=0) Column of the slot to add the histogram to
* 
* Only acts on the specified (or default) slot. Does not remove histograms and/or conditions
*/
void TRemiIncludeFunctions::ClearPicture(TGo4Picture* picture, Int_t posY, Int_t posX) const
{
	TList* specialObjects = picture->Pic(posY, posX)->GetSpecialObjects();
	if (specialObjects)
		specialObjects->Delete();
}



/**
* \brief Test & Go function to easily make and test a 1D window condition
* \param value Value to test
* \param name Short (internal) name of condition
* \param title Long description of conditions
* \param left Lower border of default condition
* \param right Upper border of default condition
* \param histo_name (optional) Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
Bool_t TRemiIncludeFunctions::TestWinCond (Double_t value, TString name, TString title, Double_t left, Double_t right, TString histo_name, TString folder)
{
	TGo4WinCond* wincond = dynamic_cast<TGo4WinCond*> ( TGo4Analysis::Instance()->GetAnalysisCondition( MakeName(name, folder)  ) );
	if (!wincond)
	{
		if (title.Length() == 0) 
			title = name + " window condition";
		wincond = new TGo4WinCond(name, title); 
		wincond->SetValues(left, right);
		wincond->Enable();
		fConditionStore.push_back(wincond);
		AddCondition(wincond, folder);
		if (histo_name.Length() != 0)
			wincond->SetHistogram(histo_name); // link with histogram
	}
	return wincond->Test(value);
}

/**
* \brief Test & Go function to easily make and test a 1D window condition which is initialised via its linked histogram
* \param value Value to test
* \param name Short (internal) name of condition
* \param title Long description of conditions
* \param histo_name Name of histogram to link condition with
* \param folder (optional)  Folder to place the condition in the Go4 browser tree
*/
Bool_t TRemiIncludeFunctions::TestWinCond (Double_t value, TString name, TString title,  TString histo_name, TString folder)
{
	TGo4WinCond* wincond = dynamic_cast<TGo4WinCond*> ( TGo4Analysis::Instance()->GetAnalysisCondition( MakeName(name, folder)  ) );
	if (!wincond)
	{
		TH1* hist = TGo4Analysis::Instance()->GetHistogram(histo_name);
		if (hist)
		{
			if (title.Length() == 0) 
				title = name + " window condition";
			wincond = new TGo4WinCond(name, title); 
			wincond->SetValues(hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
			wincond->Enable();
			fConditionStore.push_back(wincond);
			AddCondition(wincond, folder);
			wincond->SetHistogram(histo_name); // link with histogram
		}
	}
	return wincond->Test(value);
}

/**
* \brief Fill & Go function to easily make and fill unweighted one-dimensional histograms with equidistant bins
* \param value Value to fill into the histogram
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins to create 
* \param xlow Lower boundary of first bin (included)
* \param xup Upper boundary of last bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with \p nbinsx equidistant bins between \p xlow and \p xup.
* 
* Example how to make and fill a histogram using this function:
* \code
* FillTo1DHist (myvalue, "myname", "mytitle", 10, 0, 10, "myfolder", "xaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillTo1DHist (Double_t value, TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder, TString xtitle, TString ytitle)
{
	TH1I* histo = dynamic_cast<TH1I*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH1<TH1I>(name, title, nbinsx, xlow, xup, folder, xtitle, ytitle);
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, nbinsx, xlow, xup) ) )
		histo = CreateH1<TH1I>(name, title, nbinsx, xlow, xup, folder, xtitle, ytitle);
	histo->Fill(value);
}

/**
* \brief Fill & Go function to easily make and fill unweighted one-dimensional histograms with equidistant bins using input from a Config File
* \param value Value to fill into the histogram
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx name of ConfigFile field that contains No. of bins to create 
* \param field_xlow name of ConfigFile field that contains Lower boundary of first bin (included)
* \param field_xup name of ConfigFile field that contains Upper boundary of last bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with \p nbinsx equidistant bins between \p xlow and \p xup.
* 
* Example how to make and fill a histogram using this function:
* \code
* FillTo1DHist (myvalue, "myname", "mytitle", "hist_bins", "hist_min", "hist_max", "myfolder", "xaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillTo1DHist (Double_t value, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder, TString xtitle, TString ytitle)
{
	TH1I* histo = dynamic_cast<TH1I*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH1<TH1I>(name, title, field_nbinsx, field_xlow, field_xup, folder, xtitle, ytitle);
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, field_nbinsx, field_xlow, field_xup) ) )
		histo = CreateH1<TH1I>(name, title, field_nbinsx, field_xlow, field_xup, folder, xtitle, ytitle);
	histo->Fill(value);
}


/**
* \brief Fill & Go function to easily make and fill weighted one-dimensional histograms with equidistant bins
* \param value Value to fill into the histogram
* \param weight Weight for this value
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins to create 
* \param xlow Lower boundary of first bin (included)
* \param xup Upper boundary of last bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with \p nbinsx equidistant bins between \p xlow and \p xup.
* 
* Errorbars for this histogram will be the square root of the sum of squared weights, i.e. weight-corrected errors
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToWeighted1DHist (myvalue, myweight, "myname", "mytitle", 10, 0, 10, "myfolder", "xaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToWeighted1DHist (Double_t value, Double_t weight, TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder, TString xtitle, TString ytitle)
{
	TH1F* histo = dynamic_cast<TH1F*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH1<TH1F>(name, title, nbinsx, xlow, xup, folder, xtitle, ytitle);
		histo->Sumw2();
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, nbinsx, xlow, xup) ) )
		histo = CreateH1<TH1F>(name, title, nbinsx, xlow, xup, folder, xtitle, ytitle);
	histo->Fill(value, weight);
}

/**
* \brief Fill & Go function to easily make and fill weighted one-dimensional histograms with equidistant bins using input from a Config File
* \param value Value to fill into the histogram
* \param weight Weight for this value
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx name of ConfigFile field that contains No. of bins to create 
* \param field_xlow name of ConfigFile field that contains Lower boundary of first bin (included)
* \param field_xup name of ConfigFile field that contains Upper boundary of last bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with \p nbinsx equidistant bins between \p xlow and \p xup.
* 
* Errorbars for this histogram will be the square root of the sum of squared weights, i.e. weight-corrected errors
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToWeighted1DHist (myvalue, myweight, "myname", "mytitle", "hist_bins", "hist_min", "hist_max", "myfolder", "xaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToWeighted1DHist (Double_t value, Double_t weight, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder, TString xtitle, TString ytitle)
{
	TH1F* histo = dynamic_cast<TH1F*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH1<TH1F>(name, title, field_nbinsx, field_xlow, field_xup, folder, xtitle, ytitle);
		histo->Sumw2();
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, field_nbinsx, field_xlow, field_xup) ) )
		histo = CreateH1<TH1F>(name, title, field_nbinsx, field_xlow, field_xup, folder, xtitle, ytitle);
	histo->Fill(value, weight);
}


/**
* \brief Fill & Go function to easily make and fill unweighted one-dimensional histograms with sinusoidal bins
* \param value Value to fill into the histogram
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins to create 
* \param xlow Lower boundary of first bin in degrees (included) 
* \param xup Upper boundary of last bin in degrees (excluded) 
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* Filling this histogram with the distribution \f$\sin(x)\f$ will look like a uniform distribution. This is especially useful if you want to plot the \f$\theta\f$-angle of a vector in spherical coordinates with collecting equal parts of the solid angle into each bin.
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToSinusoidal1DHist (myvalue, "myname", "mytitle", 90, 0, 180, "myfolder", "xaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToSinusoidal1DHist (Double_t value, TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder, TString xtitle, TString ytitle)
{
	TH1I* histo = dynamic_cast<TH1I*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateSinusoidalH1<TH1I>(name, title, nbinsx, xlow, xup, folder, xtitle, ytitle);
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, nbinsx, xlow, xup) ) )
		histo = CreateSinusoidalH1<TH1I>(name, title, nbinsx, xlow, xup, folder, xtitle, ytitle);
	histo->Fill(value);
}

/**
* \brief Fill & Go function to easily make and fill unweighted one-dimensional histograms with sinusoidal bins using input from a Config File
* \param value Value to fill into the histogram
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx name of ConfigFile field that contains No. of bins to create 
* \param field_xlow name of ConfigFile field that contains Lower boundary of first bin in degrees (included)
* \param field_xup name of ConfigFile field that contains Upper boundary of last bin in degrees (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* Filling this histogram with the distribution \f$\sin(x)\f$ will look like a uniform distribution. This is especially useful if you want to plot the \f$\theta\f$-angle of a vector in spherical coordinates with collecting equal parts of the solid angle into each bin.
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToSinusoidal1DHist (myvalue, "myname", "mytitle", "hist_bins", "hist_min", "hist_max", "myfolder", "xaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToSinusoidal1DHist (Double_t value, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder, TString xtitle, TString ytitle)
{
	TH1I* histo = dynamic_cast<TH1I*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateSinusoidalH1<TH1I>(name, title, field_nbinsx, field_xlow, field_xup, folder, xtitle, ytitle);
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, field_nbinsx, field_xlow, field_xup) ) )
		histo = CreateSinusoidalH1<TH1I>(name, title, field_nbinsx, field_xlow, field_xup, folder, xtitle, ytitle);
	histo->Fill(value);
}


/**
* \brief Fill & Go function to easily make and fill weighted one-dimensional histograms with sinusoidal bins
* \param value Value to fill into the histogram
* \param weight Weight for this value
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins to create 
* \param xlow Lower boundary of first bin in degrees (included) 
* \param xup Upper boundary of last bin in degrees (excluded) 
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* Filling this histogram with the distribution \f$\sin(x)\f$ will look like a uniform distribution. This is especially useful if you want to plot the \f$\theta\f$-angle of a vector in spherical coordinates with collecting equal parts of the solid angle into each bin.
* 
* Errorbars for this histogram will be the square root of the sum of squared weights, i.e. weight-corrected errors
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToWeightedSinusoidal1DHist (myvalue, myweight, "myname", "mytitle", 90, 0, 180, "myfolder", "xaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToWeightedSinusoidal1DHist (Double_t value, Double_t weight, TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder, TString xtitle, TString ytitle)
{
	TH1F* histo = dynamic_cast<TH1F*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateSinusoidalH1<TH1F>(name, title, nbinsx, xlow, xup, folder, xtitle, ytitle);
		histo->Sumw2();
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, nbinsx, xlow, xup) ) )
		histo = CreateSinusoidalH1<TH1F>(name, title, nbinsx, xlow, xup, folder, xtitle, ytitle);
	histo->Fill(value, weight);
}

/**
* \brief Fill & Go function to easily make and fill weighted one-dimensional histograms with sinusoidal bins using input from a Config File
* \param value Value to fill into the histogram
* \param weight Weight for this value
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx name of ConfigFile field that contains No. of bins to create 
* \param field_xlow name of ConfigFile field that contains Lower boundary of first bin in degrees (included)
* \param field_xup name of ConfigFile field that contains Upper boundary of last bin in degrees (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* Filling this histogram with the distribution \f$\sin(x)\f$ will look like a uniform distribution. This is especially useful if you want to plot the \f$\theta\f$-angle of a vector in spherical coordinates with collecting equal parts of the solid angle into each bin.
* 
* Errorbars for this histogram will be the square root of the sum of squared weights, i.e. weight-corrected errors
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToWeightedSinusoidal1DHist (myvalue, myweight, "myname", "mytitle", "hist_bins", "hist_min", "hist_max", "myfolder", "xaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToWeightedSinusoidal1DHist (Double_t value, Double_t weight, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder, TString xtitle, TString ytitle)
{
	TH1F* histo = dynamic_cast<TH1F*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateSinusoidalH1<TH1F>(name, title, field_nbinsx, field_xlow, field_xup, folder, xtitle, ytitle);
		histo->Sumw2();
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, field_nbinsx, field_xlow, field_xup) ) )
		histo = CreateSinusoidalH1<TH1F>(name, title, field_nbinsx, field_xlow, field_xup, folder, xtitle, ytitle);
	histo->Fill(value, weight);
}


/**
* \brief Fill & Go function to easily make and fill unweighted two-dimensional histograms with equidistant bins
* \param xvalue X-value to fill into the histogram
* \param yvalue Y-value to fill into the histogram
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins along \c x axis
* \param xlow Lower boundary of first \c x bin (included)
* \param xup Upper boundary of last \c x bin (excluded)
* \param nbinsy No. of bins along \c y axis
* \param ylow Lower boundary of first \c y bin (included)
* \param yup Upper boundary of last \c y bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with equidistant bins both in \c x and \c y direction.
* 
* Example how to make and fill a histogram using this function:
* \code
* FillTo2DHist (myxvalue, myyvalue, "myname", "mytitle", 10, 0, 10, 20, 0, 20, "myfolder", "xaxis", "yaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillTo2DHist (Double_t xvalue, Double_t yvalue, TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH2I* histo = dynamic_cast<TH2I*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH2<TH2I>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, folder, xtitle, ytitle, ztitle);
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup) ) )
		histo = CreateH2<TH2I>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue);
}

/**
* \brief Fill & Go function to easily make and fill unweighted two-dimensional histograms with equidistant bins using input from a Config File
* \param xvalue X-value to fill into the histogram
* \param yvalue Y-value to fill into the histogram
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx Name of ConfigFile filed that contains No. of bins along \c x axis
* \param field_xlow Name of ConfigFile filed that contains Lower boundary of first \c x bin (included)
* \param field_xup Name of ConfigFile filed that contains Upper boundary of last \c x bin (excluded)
* \param field_nbinsy Name of ConfigFile filed that contains No. of bins along \c y axis
* \param field_ylow Name of ConfigFile filed that contains Lower boundary of first \c y bin (included)
* \param field_yup Name of ConfigFile filed that contains Upper boundary of last \c y bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with equidistant bins both in \c x and \c y direction.
* 
* Example how to make and fill a histogram using this function:
* \code
* FillTo2DHist (myxvalue, myyvalue, "myname", "mytitle", "hist_xbins", "hist_xmin", "hist_xmax", "hist_ybins", "hist_ymin", "hist_ymax", "myfolder", "xaxis", "yaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillTo2DHist (Double_t xvalue, Double_t yvalue, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH2I* histo = dynamic_cast<TH2I*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH2<TH2I>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, folder, xtitle, ytitle, ztitle);
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup) ) )
		histo = CreateH2<TH2I>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue);
}


/**
* \brief Fill & Go function to easily make and fill weighted two-dimensional histograms with equidistant bins
* \param xvalue X-value to fill into the histogram
* \param yvalue Y-value to fill into the histogram
* \param weight Weight for this value
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins along \c x axis
* \param xlow Lower boundary of first \c x bin (included)
* \param xup Upper boundary of last \c x bin (excluded)
* \param nbinsy No. of bins along \c y axis
* \param ylow Lower boundary of first \c y bin (included)
* \param yup Upper boundary of last \c y bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with equidistant bins both in \c x and \c y direction.
* 
* Errorbars for this histogram will be the square root of the sum of squared weights, i.e. weight-corrected errors
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToWeighted2DHist (myxvalue, myyvalue, myweight, "myname", "mytitle", 10, 0, 10, 20, 0, 20, "myfolder", "xaxis", "yaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToWeighted2DHist (Double_t xvalue, Double_t yvalue, Double_t weight, TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH2F* histo = dynamic_cast<TH2F*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH2<TH2F>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, folder, xtitle, ytitle, ztitle);
		histo->Sumw2();
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup) ) )
		histo = CreateH2<TH2F>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue, weight);
}

/**
* \brief Fill & Go function to easily make and fill weighted two-dimensional histograms with equidistant bins using input from a Config File
* \param xvalue X-value to fill into the histogram
* \param yvalue Y-value to fill into the histogram
* \param weight Weight for this value
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx Name of ConfigFile filed that contains No. of bins along \c x axis
* \param field_xlow Name of ConfigFile filed that contains Lower boundary of first \c x bin (included)
* \param field_xup Name of ConfigFile filed that contains Upper boundary of last \c x bin (excluded)
* \param field_nbinsy Name of ConfigFile filed that contains No. of bins along \c y axis
* \param field_ylow Name of ConfigFile filed that contains Lower boundary of first \c y bin (included)
* \param field_yup Name of ConfigFile filed that contains Upper boundary of last \c y bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with equidistant bins both in \c x and \c y direction.
* 
* Errorbars for this histogram will be the square root of the sum of squared weights, i.e. weight-corrected errors
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToWeighted2DHist (myxvalue, myyvalue, myweight, "myname", "mytitle", "hist_xbins", "hist_xmin", "hist_xmax", "hist_ybins", "hist_ymin", "hist_ymax", "myfolder", "xaxis", "yaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToWeighted2DHist (Double_t xvalue, Double_t yvalue, Double_t weight, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH2F* histo = dynamic_cast<TH2F*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH2<TH2F>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, folder, xtitle, ytitle, ztitle);
		histo->Sumw2();
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup) ) )
		histo = CreateH2<TH2F>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue, weight);
}


/**
* \brief Fill & Go function to easily make and fill unweighted two-dimensional histograms with spherical bins
* \param xvalue X-value (Azimuthal angle phi) to fill into the histogram
* \param yvalue Y-value (Polar angle theta) to fill into the histogram
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of azimuthal bins
* \param xlow Lower boundary of first azimuthal bin (included)
* \param xup Upper boundary of last azimuthal bin (excluded)
* \param nbinsy No. of polar bins
* \param ylow Lower boundary of first polar bin in degrees (included) Minimum: 0
* \param yup Upper boundary of last polar bin in degrees (excluded) Maximum: 180
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function was made to create spherical histograms depending on the two angles \f$ \theta \f$ (the y-axis of the histogram) and \f$ \phi \f$ (the x-axis of the histogram). Filling this histogram with isotropically distributed points on the unit sphere will create a uniform distribution.
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToSpherical2DHist (myxvalue, myyvalue, "myname", "mytitle", 90, 0, 180, 180, 0, 360, "myfolder", "xaxis", "yaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToSpherical2DHist (Double_t xvalue, Double_t yvalue, TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH2I* histo = dynamic_cast<TH2I*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateSphericalH2<TH2I>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, folder, xtitle, ytitle, ztitle);
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup) ) )
		histo = CreateSphericalH2<TH2I>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue);
}

/**
* \brief Fill & Go function to easily make and fill unweighted two-dimensional histograms with spherical bins using input from a Config File
* \param xvalue X-value (Azimuthal angle phi) to fill into the histogram
* \param yvalue Y-value (Polar angle theta) to fill into the histogram
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx ConfigFile field that contains No. of azimuthal bins
* \param field_xlow ConfigFile field that contains Lower boundary of first azimuthal bin (included)
* \param field_xup ConfigFile field that contains Upper boundary of last azimuthal bin (excluded)
* \param field_nbinsy ConfigFile field that contains no. of polar bins
* \param field_ylow ConfigFile field that contains Lower boundary of first polar bin in degrees (included) Minimum: 0
* \param field_yup ConfigFile field that contains Upper boundary of last polar bin in degrees (excluded) Maximum: 180
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function was made to create spherical histograms depending on the two angles \f$ \theta \f$ (the y-axis of the histogram) and \f$ \phi \f$ (the x-axis of the histogram). Filling this histogram with isotropically distributed points on the unit sphere will create a uniform distribution.
* 
* Example how to make and fill a histogram using this function:
* \code
* FillTo2SphericalDHist (myxvalue, myyvalue, "myname", "mytitle", "hist_xbins", "hist_xmin", "hist_xmax", "hist_ybins", "hist_ymin", "hist_ymax", "myfolder", "xaxis", "yaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToSpherical2DHist (Double_t xvalue, Double_t yvalue, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH2I* histo = dynamic_cast<TH2I*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateSphericalH2<TH2I>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, folder, xtitle, ytitle, ztitle);
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup) ) )
		histo = CreateSphericalH2<TH2I>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue);
}


/**
* \brief Fill & Go function to easily make and fill weighted two-dimensional histograms with spherical bins
* \param xvalue X-value (Azimuthal angle phi) to fill into the histogram
* \param yvalue Y-value (Polar angle theta) to fill into the histogram
* \param weight Weight for this value
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of azimuthal bins
* \param xlow Lower boundary of first azimuthal bin (included)
* \param xup Upper boundary of last azimuthal bin (excluded)
* \param nbinsy No. of polar bins
* \param ylow Lower boundary of first polar bin in degrees (included) Minimum: 0
* \param yup Upper boundary of last polar bin in degrees (excluded) Maximum: 180
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function was made to create spherical histograms depending on the two angles \f$ \theta \f$ (the y-axis of the histogram) and \f$ \phi \f$ (the x-axis of the histogram). Filling this histogram with isotropically distributed points on the unit sphere will create a uniform distribution.
* 
* Errorbars for this histogram will be the square root of the sum of squared weights, i.e. weight-corrected errors
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToWeightedSpherical2DHist (myxvalue, myyvalue, myweight, "myname", "mytitle", 90, 0, 180, 180, 0, 360, "myfolder", "xaxis", "yaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToWeightedSpherical2DHist (Double_t xvalue, Double_t yvalue, Double_t weight, TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH2F* histo = dynamic_cast<TH2F*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateSphericalH2<TH2F>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, folder, xtitle, ytitle, ztitle);
		histo->Sumw2();
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup) ) )
		histo = CreateH2<TH2F>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue, weight);
}

/**
* \brief Fill & Go function to easily make and fill weighted two-dimensional histograms with spherical bins using input from a Config File
* \param xvalue X-value (Azimuthal angle phi) to fill into the histogram
* \param yvalue Y-value (Polar angle theta) to fill into the histogram
* \param weight Weight for this value
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx ConfigFile field that contains No. of azimuthal bins
* \param field_xlow ConfigFile field that contains Lower boundary of first azimuthal bin (included)
* \param field_xup ConfigFile field that contains Upper boundary of last azimuthal bin (excluded)
* \param field_nbinsy ConfigFile field that contains no. of polar bins
* \param field_ylow ConfigFile field that contains Lower boundary of first polar bin in degrees (included) Minimum: 0
* \param field_yup ConfigFile field that contains Upper boundary of last polar bin in degrees (excluded) Maximum: 180
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function was made to create spherical histograms depending on the two angles \f$ \theta \f$ (the y-axis of the histogram) and \f$ \phi \f$ (the x-axis of the histogram). Filling this histogram with isotropically distributed points on the unit sphere will create a uniform distribution.
* 
* Errorbars for this histogram will be the square root of the sum of squared weights, i.e. weight-corrected errors
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToWeightedSpherical2DHist (myxvalue, myyvalue, myweight, "myname", "mytitle", "hist_xbins", "hist_xmin", "hist_xmax", "hist_ybins", "hist_ymin", "hist_ymax", "myfolder", "xaxis", "yaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToWeightedSpherical2DHist (Double_t xvalue, Double_t yvalue, Double_t weight, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH2F* histo = dynamic_cast<TH2F*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateSphericalH2<TH2F>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, folder, xtitle, ytitle, ztitle);
		histo->Sumw2();
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup) ) )
		histo = CreateH2<TH2F>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue, weight);
}


/**
* \brief Fill & Go function to easily make and fill unweighted three-dimensional histograms with equidistant bins
* \param xvalue X-value to fill into the histogram
* \param yvalue Y-value to fill into the histogram
* \param zvalue Z-value to fill into the histogram
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins along \c x axis
* \param xlow Lower boundary of first \c x bin (included)
* \param xup Upper boundary of last \c x bin (excluded)
* \param nbinsy No. of bins along \c y axis
* \param ylow Lower boundary of first \c y bin (included)
* \param yup Upper boundary of last \c y bin (excluded)
* \param nbinsz No. of bins along \c z axis
* \param zlow Lower boundary of first \c z bin (included)
* \param zup Upper boundary of last \c z bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with equidistant bins both in \c x, \c y, and \c z direction.
* 
* Example how to make and fill a histogram using this function:
* \code
* FillTo3DHist (myxvalue, myyvalue, myzvalue, "myname", "mytitle", 10, 0, 10, 20, 0, 20, "myfolder", "xaxis", "yaxis", "zaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillTo3DHist (Double_t xvalue, Double_t yvalue, Double_t zvalue, TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH3I* histo = dynamic_cast<TH3I*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH3<TH3I>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup, folder, xtitle, ytitle, ztitle);
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup) ) )
		histo = CreateH3<TH3I>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue, zvalue);
}

/**
* \brief Fill & Go function to easily make and fill unweighted three-dimensional histograms with equidistant bins using input from a Config File
* \param xvalue X-value to fill into the histogram
* \param yvalue Y-value to fill into the histogram
* \param zvalue Z-value to fill into the histogram
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx Name of ConfigFile filed that contains No. of bins along \c x axis
* \param field_xlow Name of ConfigFile filed that contains Lower boundary of first \c x bin (included)
* \param field_xup Name of ConfigFile filed that contains Upper boundary of last \c x bin (excluded)
* \param field_nbinsy Name of ConfigFile filed that contains No. of bins along \c y axis
* \param field_ylow Name of ConfigFile filed that contains Lower boundary of first \c y bin (included)
* \param field_yup Name of ConfigFile filed that contains Upper boundary of last \c y bin (excluded)
* \param field_nbinsz Name of ConfigFile filed that contains No. of bins along \c z axis
* \param field_zlow Name of ConfigFile filed that contains Lower boundary of first \c z bin (included)
* \param field_zup Name of ConfigFile filed that contains Upper boundary of last \c z bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with equidistant bins both in \c x, \c y, and \c z direction.
* 
* Example how to make and fill a histogram using this function:
* \code
* FillTo3DHist (myxvalue, myyvalue, myzvalue, "myname", "mytitle", "hist_xbins", "hist_xmin", "hist_xmax", "hist_ybins", "hist_ymin", "hist_ymax", "hist_zbins", "hist_zmin", "hist_zmax", "myfolder", "xaxis", "yaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillTo3DHist (Double_t xvalue, Double_t yvalue, Double_t zvalue, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString field_nbinsz, TString field_zlow, TString field_zup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH3I* histo = dynamic_cast<TH3I*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH3<TH3I>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, field_nbinsz, field_zlow, field_zup, folder, xtitle, ytitle, ztitle);
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, field_nbinsz, field_zlow, field_zup) ) )
		histo = CreateH3<TH3I>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, field_nbinsz, field_zlow, field_zup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue, zvalue);
}


/**
* \brief Fill & Go function to easily make and fill weighted three-dimensional histograms with equidistant bins
* \param xvalue X-value to fill into the histogram
* \param yvalue Y-value to fill into the histogram
* \param zvalue Z-value to fill into the histogram
* \param weight Weight for this value
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins along \c x axis
* \param xlow Lower boundary of first \c x bin (included)
* \param xup Upper boundary of last \c x bin (excluded)
* \param nbinsy No. of bins along \c y axis
* \param ylow Lower boundary of first \c y bin (included)
* \param yup Upper boundary of last \c y bin (excluded)
* \param nbinsz No. of bins along \c z axis
* \param zlow Lower boundary of first \c z bin (included)
* \param zup Upper boundary of last \c z bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with equidistant bins both in \c x, \c y, and \c z direction.
* 
* Errorbars for this histogram will be the square root of the sum of squared weights, i.e. weight-corrected errors
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToWeighted3DHist (myxvalue, myyvalue, myzvalue, myweight, "myname", "mytitle", 10, 0, 10, 20, 0, 20, "myfolder", "xaxis", "yaxis", "zaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToWeighted3DHist (Double_t xvalue, Double_t yvalue, Double_t zvalue, Double_t weight, TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH3F* histo = dynamic_cast<TH3F*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH3<TH3F>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup, folder, xtitle, ytitle, ztitle);
		histo->Sumw2();
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup) ) )
		histo = CreateH3<TH3F>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue, zvalue, weight);
}

/**
* \brief Fill & Go function to easily make and fill weighted three-dimensional histograms with equidistant bins using input from a Config File
* \param xvalue X-value to fill into the histogram
* \param yvalue Y-value to fill into the histogram
* \param zvalue Z-value to fill into the histogram
* \param weight Weight for this value
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx Name of ConfigFile filed that contains No. of bins along \c x axis
* \param field_xlow Name of ConfigFile filed that contains Lower boundary of first \c x bin (included)
* \param field_xup Name of ConfigFile filed that contains Upper boundary of last \c x bin (excluded)
* \param field_nbinsy Name of ConfigFile filed that contains No. of bins along \c y axis
* \param field_ylow Name of ConfigFile filed that contains Lower boundary of first \c y bin (included)
* \param field_yup Name of ConfigFile filed that contains Upper boundary of last \c y bin (excluded)
* \param field_nbinsz Name of ConfigFile filed that contains No. of bins along \c z axis
* \param field_zlow Name of ConfigFile filed that contains Lower boundary of first \c z bin (included)
* \param field_zup Name of ConfigFile filed that contains Upper boundary of last \c z bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
*
* Can be used to fill data into a histogram without building an object for it. Uses the internal name to check if a histogram exists or not. If the histogram is not existing (Usually the case if you call this function for a histogram the first time), a new histogram object will be created and safed internally.
* 
* This function only creates histograms with equidistant bins both in \c x, \c y, and \c z direction.
* 
* Errorbars for this histogram will be the square root of the sum of squared weights, i.e. weight-corrected errors
* 
* Example how to make and fill a histogram using this function:
* \code
* FillToWeighted3DHist (myxvalue, myyvalue, myzvalue, myweight, "myname", "mytitle", "hist_xbins", "hist_xmin", "hist_xmax", "hist_ybins", "hist_ymin", "hist_ymax", "hist_zbins", "hist_zmin", "hist_zmax", "myfolder", "xaxis", "yaxis", "zaxis");
* \endcode
*/
void TRemiIncludeFunctions::FillToWeighted3DHist (Double_t xvalue, Double_t yvalue, Double_t zvalue, Double_t weight, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString field_nbinsz, TString field_zlow, TString field_zup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	TH3F* histo = dynamic_cast<TH3F*> (TGo4Analysis::Instance()->GetHistogram( MakeName(name, folder) ));
	if (!histo) 
	{
		histo = CreateH3<TH3F>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, field_nbinsz, field_zlow, field_zup, folder, xtitle, ytitle, ztitle);
		histo->Sumw2();
	}
	if ( (mRescaleFillnGoHist) && (!CheckUnchangedHist(histo, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, field_nbinsz, field_zlow, field_zup) ) )
		histo = CreateH3<TH3F>(name, title, field_nbinsx, field_xlow, field_xup, field_nbinsy, field_ylow, field_yup, field_nbinsz, field_zlow, field_zup, folder, xtitle, ytitle, ztitle);
	histo->Fill(xvalue, yvalue, zvalue, weight);
}


/**
 * \brief Adds a histogram to the object tree and sets axis titles if requested
 */
void TRemiIncludeFunctions::AddHistogram(TH1* histo, TString folder, TString xtitle, TString ytitle, TString ztitle) const
{
	if (xtitle.Length() != 0)
		histo->SetXTitle(xtitle);
	if (ytitle.Length() != 0)
		histo->SetYTitle(ytitle);
	if (ztitle.Length() != 0)
		histo->SetZTitle(ztitle);
	if (folder.Length() != 0)
		TGo4Analysis::Instance()->AddHistogram(histo, folder);
	else
		TGo4Analysis::Instance()->AddHistogram(histo);
}

/**
 * \brief Adds a condition to the object tree
 */
void TRemiIncludeFunctions::AddCondition(TGo4Condition* cond, TString folder) const
{
	if (folder.Length() != 0)
		TGo4Analysis::Instance()->AddAnalysisCondition(cond, folder);
	else
		TGo4Analysis::Instance()->AddAnalysisCondition(cond);
}

/**
 * \brief Creates a full name from a object name and a folder
 */
TString TRemiIncludeFunctions::MakeName(TString name, TString folder) const
{
	TString fullname;
	if (folder.Length() != 0) 
	{
		fullname=folder;
		fullname+="/";
	}
	return (fullname+name);
}

/**
 * \brief Creates the list of borders for sinudoidally binned histograms
 * \param borders Array in which the resulting border values are written to. Has to be an array of size \p nbinsx + 1 
 * \param nbinsx Number of bins to generate
 * \param xlow Lower boundary of first bin in degrees (included) 
 * \param xup Upper boundary of last bin in degrees (excluded) 
 * \return true if list could be generated
 */
Bool_t TRemiIncludeFunctions::CreateSinusoidalBins(Double_t *borders, Int_t nbinsx, Double_t xlow, Double_t xup) const
{
	if (xup <= xlow)
		return kFALSE; // stop if the upper border is not smaller than the lower one
	TRandom3 random(0);
	Double_t squeeze, fullrange, this_xlow, this_xup, this_range, this_cxlow, this_cxup;
	Int_t j=0, past_bins=0, this_nbinsx, halfperiods;
	fullrange = xup - xlow;
	halfperiods = static_cast<Int_t>(fullrange/180.)+1;
	Double_t tempborders[nbinsx+halfperiods+1]; // create tempborders array to avoid segfault on original array if initial partioning is larger than intended
	while (j != nbinsx+1)
	{
		squeeze = random.Rndm();
		this_xlow = xlow;
		j=0;
		past_bins=0;
		do
		{
			this_xup = static_cast<Int_t>( (this_xlow + 180) / 180) * 180.;
			if (this_xup > xup)
				this_xup = xup;
			this_range = this_xup - this_xlow;
			this_nbinsx = static_cast<Int_t>(this_range / fullrange * nbinsx + squeeze);
			this_cxlow = TMath::Cos(TMath::DegToRad() * fmod(this_xlow,180.) );
			if (fmod(this_xup,180.)==0)
				this_cxup = -1;
			else
				this_cxup = TMath::Cos(TMath::DegToRad() * fmod(this_xup,180.));
			while (j<past_bins+this_nbinsx+1)
			{
				tempborders[j] = this_xlow - TMath::RadToDeg() * TMath::ACos( this_cxlow ) + TMath::RadToDeg() * TMath::ACos( this_cxlow - (j-past_bins) * (this_cxlow - this_cxup) / this_nbinsx );
				j++;
			}
			this_xlow = this_xup;
			past_bins += this_nbinsx;
		}
		while (this_xup < xup);
	}
	// Transfer borders into output array
	for (Int_t i=0; i<nbinsx+1; i++)
		borders[i]=tempborders[i];
	return kTRUE;
}

/**
* \brief Checks if the borders and number of bins of a 1D hist have been changed by input
* \param histo Pointer to a 1D histogram
* \param nbins New No. of bins
* \param xlow New lower boundary
* \param xup New upper boundary
* \return true if input values are the same than the current histogram values
* 
* If the values have been changed, histogram is deleted.
*/ 
Bool_t TRemiIncludeFunctions::CheckUnchangedHist (TH1* histo, Int_t nbins, Double_t xlow, Double_t xup) const
{
	if ( 
		( histo->GetNbinsX() != nbins ) ||
		( histo->GetXaxis()->GetXmin() != xlow ) ||
		( histo->GetXaxis()->GetXmax() != xup) )
	{	// something has been changed
		TGo4Analysis::Instance()->RemoveObject(histo->GetName(), kTRUE);
		return kFALSE;
	}
	else
		return kTRUE;
}

/**
* \brief Checks if the borders and number of bins of a 2D hist have been changed by input
* \param histo Pointer to a 2D histogram
* \param nbinsx New No. X of bins
* \param xlow New X lower boundary
* \param xup New X upper boundary
* \param nbinsy New No. Y of bins
* \param ylow New Y lower boundary
* \param yup New Y upper boundary
* \return true if input values are the same than the current histogram values
* 
* If the values have been changed, histogram is deleted.
*/ 
Bool_t TRemiIncludeFunctions::CheckUnchangedHist (TH2* histo, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup) const
{
	if ( 
		( histo->GetNbinsX() != nbinsx ) ||
		( histo->GetXaxis()->GetXmin() != xlow ) ||
		( histo->GetXaxis()->GetXmax() != xup) ||
		( histo->GetNbinsY() != nbinsy ) ||
		( histo->GetYaxis()->GetXmin() != ylow ) ||
		( histo->GetYaxis()->GetXmax() != yup) )
	{	// something has been changed
		TGo4Analysis::Instance()->RemoveObject(histo->GetName(), kTRUE);
		return kFALSE;
	}
	else
		return kTRUE;
}

/**
* \brief Checks if the borders and number of bins of a 3D hist have been changed by input
* \param histo Pointer to a 3D histogram
* \param nbinsx New No. X of bins
* \param xlow New X lower boundary
* \param xup New X upper boundary
* \param nbinsy New No. Y of bins
* \param ylow New Y lower boundary
* \param yup New Y upper boundary
* \param nbinsz New No. Z of bins
* \param zlow New Z lower boundary
* \param zup New Z upper boundary
* \return true if input values are the same than the current histogram values
* 
* If the values have been changed, histogram is deleted.
*/ 
Bool_t TRemiIncludeFunctions::CheckUnchangedHist (TH3* histo, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup) const
{
	if ( 
		( histo->GetNbinsX() != nbinsx ) ||
		( histo->GetXaxis()->GetXmin() != xlow ) ||
		( histo->GetXaxis()->GetXmax() != xup) ||
		( histo->GetNbinsY() != nbinsy ) ||
		( histo->GetYaxis()->GetXmin() != ylow ) ||
		( histo->GetYaxis()->GetXmax() != yup) ||
		( histo->GetNbinsY() != nbinsz ) ||
		( histo->GetYaxis()->GetXmin() != zlow ) ||
		( histo->GetYaxis()->GetXmax() != zup) )
	{	// something has been changed
		TGo4Analysis::Instance()->RemoveObject(histo->GetName(), kTRUE);
		return kFALSE;
	}
	else
		return kTRUE;
}

/**
* \brief Checks if the borders and number of bins of a 1D hist have been changed in the config file
* \param histo Pointer to a 1D histogram
* \param field_nbinsx Name of ConfigFile filed that contains No. of bins along \c x axis
* \param field_xlow Name of ConfigFile filed that contains Lower boundary of first \c x bin (included)
* \param field_xup Name of ConfigFile filed that contains Upper boundary of last \c x bin (excluded)
* \return true if input values are the same than the current histogram values
* 
* If the values have been changed, histogram is deleted.
*/ 
Bool_t TRemiIncludeFunctions::CheckUnchangedHist (TH1* histo, TString field_nbins, TString field_xlow, TString field_xup) const
{
	Int_t nbins = ReadFromConfigFile<Int_t>(field_nbins, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	if ( 
		( histo->GetNbinsX() != nbins ) ||
		( histo->GetXaxis()->GetXmin() != xlow ) ||
		( histo->GetXaxis()->GetXmax() != xup) )
	{	// something has been changed
		TGo4Analysis::Instance()->RemoveObject(histo->GetName(), kTRUE);
		return kFALSE;
	}
	else
		return kTRUE;
}

/**
* \brief Checks if the borders and number of bins of a 2D hist have been changed by input
* \param histo Pointer to a 2D histogram
* \param field_nbinsx Name of ConfigFile filed that contains No. of bins along \c x axis
* \param field_xlow Name of ConfigFile filed that contains Lower boundary of first \c x bin (included)
* \param field_xup Name of ConfigFile filed that contains Upper boundary of last \c x bin (excluded)
* \param field_nbinsy Name of ConfigFile filed that contains No. of bins along \c y axis
* \param field_ylow Name of ConfigFile filed that contains Lower boundary of first \c y bin (included)
* \param field_yup Name of ConfigFile filed that contains Upper boundary of last \c y bin (excluded)
* \return true if input values are the same than the current histogram values
* 
* If the values have been changed, histogram is deleted.
*/ 
Bool_t TRemiIncludeFunctions::CheckUnchangedHist (TH2* histo, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup) const
{
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	Int_t nbinsy = ReadFromConfigFile<Int_t>(field_nbinsy, DEFAULT_NBINS);
	Double_t ylow = ReadFromConfigFile<Double_t>(field_ylow, DEFAULT_LOWER);
	Double_t yup = ReadFromConfigFile<Double_t>(field_yup, DEFAULT_UPPER);
	if ( 
		( histo->GetNbinsX() != nbinsx ) ||
		( histo->GetXaxis()->GetXmin() != xlow ) ||
		( histo->GetXaxis()->GetXmax() != xup) ||
		( histo->GetNbinsY() != nbinsy ) ||
		( histo->GetYaxis()->GetXmin() != ylow ) ||
		( histo->GetYaxis()->GetXmax() != yup) )
	{	// something has been changed
		TGo4Analysis::Instance()->RemoveObject(histo->GetName(), kTRUE);
		return kFALSE;
	}
	else
		return kTRUE;
}

/**
* \brief Checks if the borders and number of bins of a 3D hist have been changed by input
* \param histo Pointer to a 3D histogram
* \param field_nbinsx Name of ConfigFile filed that contains No. of bins along \c x axis
* \param field_xlow Name of ConfigFile filed that contains Lower boundary of first \c x bin (included)
* \param field_xup Name of ConfigFile filed that contains Upper boundary of last \c x bin (excluded)
* \param field_nbinsy Name of ConfigFile filed that contains No. of bins along \c y axis
* \param field_ylow Name of ConfigFile filed that contains Lower boundary of first \c y bin (included)
* \param field_yup Name of ConfigFile filed that contains Upper boundary of last \c y bin (excluded)
* \param field_nbinsz Name of ConfigFile filed that contains No. of bins along \c z axis
* \param field_zlow Name of ConfigFile filed that contains Lower boundary of first \c z bin (included)
* \param field_zup Name of ConfigFile filed that contains Upper boundary of last \c z bin (excluded)
* \return true if input values are the same than the current histogram values
* 
* If the values have been changed, histogram is deleted.
*/ 
Bool_t TRemiIncludeFunctions::CheckUnchangedHist (TH3* histo, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString field_nbinsz, TString field_zlow, TString field_zup) const
{
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	Int_t nbinsy = ReadFromConfigFile<Int_t>(field_nbinsy, DEFAULT_NBINS);
	Double_t ylow = ReadFromConfigFile<Double_t>(field_ylow, DEFAULT_LOWER);
	Double_t yup = ReadFromConfigFile<Double_t>(field_yup, DEFAULT_UPPER);
	Int_t nbinsz = ReadFromConfigFile<Int_t>(field_nbinsz, DEFAULT_NBINS);
	Double_t zlow = ReadFromConfigFile<Double_t>(field_zlow, DEFAULT_LOWER);
	Double_t zup = ReadFromConfigFile<Double_t>(field_zup, DEFAULT_UPPER);
	if ( 
		( histo->GetNbinsX() != nbinsx ) ||
		( histo->GetXaxis()->GetXmin() != xlow ) ||
		( histo->GetXaxis()->GetXmax() != xup) ||
		( histo->GetNbinsY() != nbinsy ) ||
		( histo->GetYaxis()->GetXmin() != ylow ) ||
		( histo->GetYaxis()->GetXmax() != yup) ||
		( histo->GetNbinsZ() != nbinsz ) ||
		( histo->GetZaxis()->GetXmin() != zlow ) ||
		( histo->GetZaxis()->GetXmax() != zup) )
	{	// something has been changed
		TGo4Analysis::Instance()->RemoveObject(histo->GetName(), kTRUE);
		return kFALSE;
	}
	else
		return kTRUE;
}

ClassImp(TRemiIncludeFunctions);
