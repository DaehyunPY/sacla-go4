//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Base class for all event processing classes
//		Contains many functions useful in handling of histograms, parameters, conditions and more
//----------------------------------------------------------------

#ifndef __IncludeFunction__
#define __IncludeFunction__

#define DEFAULT_NUM_ELEC 1
#define DEFAULT_NUM_ION 1

#define DEFAULT_NBINS 100
#define DEFAULT_LOWER 0.
#define DEFAULT_UPPER 1.

#define DEFAULT_NUM_ADC_CHAN 2
#define DEFAULT_NUM_TDC_CHAN 12
#define DEFAULT_NUM_TDC_HITS 1


#include "TGo4AnalysisImp.h"
#include "TH3.h"

#include "TString.h"
#include "Rtypes.h"

#include <vector>


class TRemiBasicParameter;
class TRemiRollingGraph;
class ConfigFile;
class TGo4PolyCond;
class TGo4Condition;
class TGo4CondArray;
class TGo4WinCond;
class TGo4Picture;
class TGraph;
class TCutG;
class TF1;
class TH1;
class TLine;
class TArc;

//using namespace std;

/**
 * \brief Provides a set of useful functions for the analysis. 
 * 
 * Should be extended by other classes to use the functions
 */
class TRemiIncludeFunctions 
{
	public:
		TRemiIncludeFunctions();
		TRemiIncludeFunctions(const char* config_name);
		TRemiIncludeFunctions(ConfigFile* ConfPointer);
		virtual ~TRemiIncludeFunctions();
	
	protected:
		
		/** \name Condition make functions */
		//@{
		TGo4WinCond* MakeWinCon(TString name, TString title, Double_t left1, Double_t right1, Double_t left2, Double_t right2, TString histo_name="", TString folder = "") const;
		TGo4WinCond* MakeWinCon(TString name, TString title, Double_t left, Double_t right, TString histo_name="", TString folder = "") const;
		TGo4WinCond* MakeWinCon(TString name, TString title="", TString histo_name="", TString folder = "") const;
		TGo4PolyCond* MakePolyCon(TString name, TString title, Int_t points, Double_t* xvalues, Double_t* yvalues, TString histo_name="", TString folder = "") const;
		TGo4PolyCond* MakePolyCon(TString name, TString title, TCutG* cut, TString histo_name="", TString folder = "") const;
		TGo4PolyCond* MakePolyCon(TString name, TString title="", TString histo_name="", TString folder = "") const;
		TGo4PolyCond* MakeCircCon(TString name, TString title, Double_t rad, Double_t x0 = 0, Double_t y0 = 0, TString histo_name = "", TString folder = "") const;
		TGo4CondArray* MakeWinConArray(TString name, Int_t elements, Double_t* lowvalues, Double_t* highvalues, TString histo_name="", TString folder = "") const;
		TGo4CondArray* MakeWinConArray(TString name, Int_t elements, Double_t xlow, Double_t xup, TString histo_name="", TString folder = "") const;
		TGo4CondArray* MakeWinConArray(TString name, Int_t elements, TString histo_name, TString folder = "") const;
		TGo4CondArray* MakeIncWinCondArray(TString name, Int_t elements, Double_t* values, TString histo_name="", TString folder = "") const;
		TGo4CondArray* MakePolyConArray(TString name, Int_t elements, TCutG* cut, TString histo_name="", TString folder = "") const;
		TGo4CondArray* MakePolyConArray(TString name, Int_t elements, Int_t points, Double_t* xvalues, Double_t* yvalues, TString histo_name="", TString folder = "") const;
		//@}
		
		// Parameter make function
		template <class ParameterType> ParameterType* MakeParam(TString name, TString folder = "") const;
		
		// TGraph make function
		TGraph* MakeGraph(TString name, TString title, TString folder) const;
		TGraph* MakeGraph(TString name, TString title, Int_t points=0, Double_t* xvalues=0, Double_t* yvalues=0, TString folder = "") const;
		TGraph* MakeGraph(TString name, TString title, TF1* function, TString folder = "") const;
		TRemiRollingGraph*	MakeRollingGraph (TString name, TString title, Int_t points=0, Int_t average = 1, TString folder = "") const;
		
		/** \name Funtions for Pictures */
		//@{
		TGo4Picture* MakePicture(TString name, TString title, TString folder="", Int_t numY=1, Int_t numX=1) const;
		void AddHistToPicture(TH1* hist, TGo4Picture* picture, Int_t posY=0, Int_t posX=0) const;
		void AddHistToPicture(TString histname, TGo4Picture* picture, Int_t posY=0, Int_t posX=0) const;
		void AddGraphToPicture(TGraph* graph, TGo4Picture* picture, Int_t posY=0, Int_t posX=0) const;
		void AddCondToPicture(TGo4Condition* cond, TGo4Picture* picture, Int_t posY=0, Int_t posX=0) const;
		void AddCondToPicture(TString condname, TGo4Picture* picture, Int_t posY=0, Int_t posX=0) const;
		void AddLineToPicture(TLine* line, TGo4Picture* picture, Int_t posY=0, Int_t posX=0) const;
		void AddLineToPicture(Double_t x1, Double_t y1, Double_t x2, Double_t y2, TGo4Picture* picture, Int_t posY=0, Int_t posX=0, Color_t colour=1, Style_t style=1, Width_t width=1) const;
		void AddTF1ToPicture(TF1* funct, TGo4Picture* picture, Int_t posY=0, Int_t posX=0) const;
		void AddTF1LineToPicture(Double_t x1, Double_t y1, Double_t x2, Double_t y2, TGo4Picture* picture, Int_t posY=0, Int_t posX=0) const;
		void AddArcToPicture(TArc* arc, TGo4Picture* picture, Int_t posY=0, Int_t posX=0) const;
		void AddArcToPicture(Double_t x1, Double_t y1, Double_t radius, Double_t phi1, Double_t phi2, TGo4Picture* picture, Int_t posY=0, Int_t posX=0, Style_t fillStyle=0) const;
		void AddCircleToPicture(Double_t x1, Double_t y1, Double_t radius, TGo4Picture* picture, Int_t posY=0, Int_t posX=0, Style_t fillStyle=0) const;
		void ClearPicture(TGo4Picture* picture, Int_t posY=0, Int_t posX=0) const;
		//@}
		
		/** \name Functions for the Basic Parameter class */
		//@{
		TRemiBasicParameter* MakeBasicParam(TString name, TString title, TString folder="") const;
		Double_t UseBasicParamValue(TString name, TString title, TString folder="");
		//@}
		
		/** \name Test 'n' Go condition functions */
		//@{
		Bool_t TestWinCond (Double_t value, TString name, TString title, Double_t left, Double_t right, TString histo_name="", TString folder="");
		Bool_t TestWinCond (Double_t value, TString name, TString title, TString histo_name, TString folder="");
		//@}
		
		/** \name Fill 'n' Go histogram functions 
		* Allow the creation and filling of a histogram in a single line. No additional definition to this is necessary.
		*/
		//@{
		// 1D
		void FillTo1DHist (Double_t value, TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		void FillTo1DHist (Double_t value, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		void FillToWeighted1DHist (Double_t value, Double_t weight, TString name, TString, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		void FillToWeighted1DHist (Double_t value, Double_t weight, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		
		// 1D Sinusoidal
		void FillToSinusoidal1DHist (Double_t value, TString name, TString, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		void FillToSinusoidal1DHist (Double_t value, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		void FillToWeightedSinusoidal1DHist (Double_t value, Double_t weight, TString name, TString, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		void FillToWeightedSinusoidal1DHist (Double_t value, Double_t weight, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		
		// 2D
		void FillTo2DHist (Double_t xvalue, Double_t yvalue, TString name, TString, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		void FillTo2DHist (Double_t xvalue, Double_t yvalue, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		void FillToWeighted2DHist (Double_t xvalue, Double_t yvalue, Double_t weight, TString name, TString, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		void FillToWeighted2DHist (Double_t xvalue, Double_t yvalue, Double_t weight, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		
		// 2D spherical
		void FillToSpherical2DHist (Double_t xvalue, Double_t yvalue, TString name, TString, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		void FillToSpherical2DHist (Double_t xvalue, Double_t yvalue, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		void FillToWeightedSpherical2DHist (Double_t xvalue, Double_t yvalue, Double_t weight, TString name, TString, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		void FillToWeightedSpherical2DHist (Double_t xvalue, Double_t yvalue, Double_t weight, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");

		// 3D
		void FillTo3DHist (Double_t xvalue, Double_t yvalue, Double_t zvalue, TString name, TString, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		void FillTo3DHist (Double_t xvalue, Double_t yvalue, Double_t zvalue, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString field_nbinsz, TString field_zlow, TString field_zup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		void FillToWeighted3DHist (Double_t xvalue, Double_t yvalue, Double_t zvalue, Double_t weight, TString name, TString, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		void FillToWeighted3DHist (Double_t xvalue, Double_t yvalue, Double_t zvalue, Double_t weight, TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString field_nbinsz, TString field_zlow, TString field_zup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		//@}
		
		
		/** \name Histogram make functions
		* Are to be used in the constructor of an event processing class to create a pointer to a new histograms
		*/
		//@{
		// 1D
		template <class HistoType> HistoType* MakeH1 (TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder = "", TString xtitle = "", TString ytitle = "") const;
		template <class HistoType> HistoType* MakeH1 (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder = "", TString xtitle = "", TString ytitle = "") const;
		template <class HistoType> HistoType* MakeH1 (TString name, TString title, Int_t nbinsx, Double_t* borders, TString folder = "", TString xtitle = "", TString ytitle = "") const;
		template <class HistoType> HistoType* MakeSinusoidalH1 (TString name, TString title, Int_t nbinsx, Float_t xlow, Float_t xup, TString folder = "", TString xtitle = "", TString ytitle = "") const;
		template <class HistoType> HistoType* MakeSinusoidalH1 (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder = "", TString xtitle = "", TString ytitle = "") const;
		
		// 2D
		template <class HistoType> HistoType* MakeH2 (TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "") const;
		template <class HistoType> HistoType* MakeH2 (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "") const;
		template <class HistoType> HistoType* MakeSphericalH2 (TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "") const;
		template <class HistoType> HistoType* MakeSphericalH2 (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "") const;
		template <class HistoType> HistoType* MakeSphericalH2smooth (TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nsmooth=0, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "") const;
		template <class HistoType> HistoType* MakeSphericalH2smooth (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, Int_t nsmooth=0, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "") const;

		// 3D
		template <class HistoType> HistoType* MakeH3 (TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "") const;
		template <class HistoType> HistoType* MakeH3 (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString field_nbinsz, TString field_zlow, TString field_zup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "") const;
		//@}
		
		ConfigFile* fConfigFile;	///< Config file object for this processor. Can be accessed in the step itself
		
		
	private:
		TString MakeName(TString name, TString folder = "") const;
		Bool_t CreateSinusoidalBins (Double_t *borders, Int_t nbins, Double_t xlow, Double_t xup) const;
		void AddHistogram (TH1* histo, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "") const;
		void AddCondition (TGo4Condition* cond, TString folder) const;
		Bool_t CheckUnchangedHist (TH1* hist, Int_t nbins, Double_t xlow, Double_t xup) const;
		Bool_t CheckUnchangedHist (TH2* hist, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup) const;
		Bool_t CheckUnchangedHist (TH3* hist, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup) const;
		Bool_t CheckUnchangedHist (TH1* hist, TString field_nbins, TString field_xlow, TString field_xup) const;
		Bool_t CheckUnchangedHist (TH2* hist, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup) const;
		Bool_t CheckUnchangedHist (TH3* hist, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString field_nbinsz, TString field_zlow, TString field_zup) const;
		
		/** \name Internal histogram creators for Fill 'n' Go functions */
		//@{
		template <class HistoType> HistoType* CreateH1(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		template <class HistoType> HistoType* CreateH1(TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		
		template <class HistoType> HistoType* CreateSinusoidalH1(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		template <class HistoType> HistoType* CreateSinusoidalH1(TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder = "", TString xtitle = "", TString ytitle = "");
		
		template <class HistoType> HistoType* CreateH2(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		template <class HistoType> HistoType* CreateH2(TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		
		template <class HistoType> HistoType* CreateSphericalH2(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		template <class HistoType> HistoType* CreateSphericalH2(TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");

		template <class HistoType> HistoType* CreateH3(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		template <class HistoType> HistoType* CreateH3(TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString field_nbinsz, TString field_zlow, TString field_zup, TString folder = "", TString xtitle = "", TString ytitle = "", TString ztitle = "");
		//@}
		
		/** \name Rescaling functions for Fill 'n' Go histograms */
		//@{
// 		void RescaleH1 (TH1* hist, Int_t nbinsy, Double_t xlow, Double_t xup) const;
		//@}
		
		template <class Type> Type ReadFromConfigFile (TString field, Type Default) const;
		
		// Class Members
		
		std::vector<TH1*> fHistoStore; ///< Dynamic array to store histograms created with Fill & Go functions
		std::vector<TRemiBasicParameter*> fBasicParameterStore;	///< Dynamic array to store histograms created with UseBasicParamValue()
		std::vector<TGo4Condition*>	fConditionStore;	///< Dynamic array to store conditions created with Test & Go functions
		
	protected :
		void EnableRescaleFillToHist()	{ mRescaleFillnGoHist = kTRUE; }	///< Activate check for rescale of Fill 'n' Go histograms. Should be called in the constructor of derived class
		void DisableRescaleFillToHist()	{ mRescaleFillnGoHist = kFALSE; }	///< Deactivate check for rescale of Fill 'n' Go histograms. Should be called at the end of BuildEvent() of derived class
		
	private :
		Bool_t mRescaleFillnGoHist;	///< Switch if Fill 'n' Go histogams are to be checked for rebinning
		

	ClassDef(TRemiIncludeFunctions, 1);
};
	
// Include file with the implementation of template functions
#include "TRemiIncludeFunctions.inline.h"

#endif
