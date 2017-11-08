#ifndef __TRemiAcqirisSoftTdc_H__
#define __TRemiAcqirisSoftTdc_H__

#include "Parameters/TRemiAcqirisHighRParameter.h"
#include "../Core/TRemiIncludeFunctions.h"

#include "TString.h"

#include <vector>

class TRemiAcqirisCfdParameter;
class TRemiAcqirisRisEParameter;
class TRemiAcqirisEvent;
class TRemiAcqirisPuls;
class TRemiAcqirisChannel;
class TRemiAcqirisPeak;
class TDCSetting;

class ConfigFile;

class TGo4Picture;
class TGo4Parameter;

class TGraph;
class TF1;
class TH1I;
class TH2I;


class TRemiAcqirisPunkt
{
	public: 
		TRemiAcqirisPunkt():fX(0),fY(0)				{}
		TRemiAcqirisPunkt(Double_t x,Double_t y):
		fX(x),fY(y)					{}
		virtual ~TRemiAcqirisPunkt(){}

		Double_t		&x()				{return fX;}
		Double_t		 x()const			{return fX;}
		Double_t		&y()				{return fY;}
		Double_t		 y()const			{return fY;}

	private:
		Double_t fX; 
		Double_t fY;

	ClassDef(TRemiAcqirisPunkt, 1);
};



class TRemiAcqirisSoftTdc : public TRemiIncludeFunctions
{
	public:
		TRemiAcqirisSoftTdc();
		TRemiAcqirisSoftTdc(ConfigFile* AcqConf, TRemiAcqirisChannel* ParentChannel);
		virtual ~TRemiAcqirisSoftTdc();
		void search_for_Peaks(TRemiAcqirisPuls *Pulse);

		enum eMethod{kNone=0, kCoM=1, kCFD=2, kRisingEdge=3, kSearchHighRes=4, kExtrema=5, kCFDrev=6};
		enum eBits{k8Bit=8,k16Bit=16};
	// 	enum ePolarity{kNeg=-1, kBad=0, kPos=1};

		void fillRawSpectra();

		void clear();

		private:
		template<class T> void search_for_Peaks_template(TRemiAcqirisPuls *Pulse);

		template<class T> void findPeaks_CoM(TRemiAcqirisPuls *Pulse);
		template<class T> void findPeaks_CFD(TRemiAcqirisPuls *Pulse);
		template<class T> void findPeaks_Extrema(TRemiAcqirisPuls *Pulse); // Extrema via derivative
		template<class T> void findPeaks_CFD_rev(TRemiAcqirisPuls *Pulse);
		template<class T> void findPeaks_HighR(TRemiAcqirisPuls *Pulse);
		template<class T> void findPeaks_RisE(TRemiAcqirisPuls *Pulse);
		template<class T> void calculatePeak_RisE(TRemiAcqirisPuls* Pulse, T* data, Short_t lengthRising, Int_t* xRising, T* yRising, Short_t lengthFalling, Int_t* xFalling, T* yFalling, Short_t polarity);

		template <typename T> void fwhm(TRemiAcqirisPeak * p, TRemiAcqirisPuls *Pulse);
		template <typename T> void com(TRemiAcqirisPeak * p, TRemiAcqirisPuls *Pulse);
		template <typename T> void startstop(TRemiAcqirisPeak * p, TRemiAcqirisPuls *Pulse);
		template <typename T> void maximum(TRemiAcqirisPeak * p, TRemiAcqirisPuls *Pulse);

		void createNewtonPolynomial(Double_t * x, Double_t * y, Double_t * coeff);
		Double_t evalNewtonPolynomial(Double_t * x,Double_t * coeff, Double_t X);
		Double_t findXForGivenY(Double_t * x, Double_t * coeff, Double_t Y, Double_t Start);

		template <typename T> void linearRegression(Double_t &Slope, Double_t &Offset, Short_t nbrPoints, Int_t *x, T *y);

		void initWatchMode();
		template<class T> void watchMode(TRemiAcqirisPuls *Pulse);
		void clearWatchMode();

		void initOverviewModeSpectra();
		void fillOverviewModeSpectra();

		Int_t	mMethod;	///< TDC method for each channel

		TGo4Parameter* 	fParaSoftTdc;	///< TDC parameters for each channel. The class of each individual parameter object depends on the selected TDC method
		TH1I*	fHistRawPeaks;	///< Histograms showing all peaks per Acqiris channel
		TH1I*	fHistNumRawPeaks;	///< Histogram showing number of hits per Acqiris channel

		Bool_t mOverviewMode;	///< Flag if TDC overview is activated
		TH1I* fHistMax;	///< Histograms showing the peak height
		TH1I* fHistFWHM;	///< Histograms showing the peak width
		TH1I* fHistPos;	///< Histograms showing the peak position
		TH2I* fHistFWHMvsMax;	///< Histograms showing peak width vs. height
		TH2I* fHistPosVsFWHM;	///< Histograms showing peak position vs. width
		TH2I* fHistPosVsMax;	///< Histograms showing peak position vs. height
		TH2I* fHistSvsS;	///< Histograms showing peak start vs. stop
		TH1I* fHistSoverS;	///< Histograms showing peak start / stop

		Bool_t mWatchMode;	///< Flag if TDC watching is activated
		std::vector< TGraph* > 	fPulseGraph;	///< Graphs of Pulse traces
		std::vector< TGraph* > 	fPeakGraph;	///< Graphs with peak markers
		TGo4Picture* 	fPicGraphs;	///< Pictures with graphs for each channel
		Int_t mNumPulseGraphs;	///< No. of pulses to show per channel
		Int_t mNumPulseGraphColumns;	///< No. of columns in pulse pictures
		std::vector< TGraph* > 	fPulseGraphOv;	///< Graphs of Pulse traces for Overlay pictures
		TGo4Picture*	fPicOverlay;	///< Picture that overlays many pulse traces for one channel
		Int_t mNumPulsesOverlay;	///< Number of pulses in overlay picture

		TF1*	fBaseline;	///< Function to show baseline in Watchmode overlay spectrum
		std::vector<TF1*>	fBaselines; ///< Functions to show baseline in Watchmode spectra

		TRemiAcqirisChannel* fParentChannel;	///< Channel to which this SoftADC object belongs

	ClassDef(TRemiAcqirisSoftTdc, 1);
};


#endif
