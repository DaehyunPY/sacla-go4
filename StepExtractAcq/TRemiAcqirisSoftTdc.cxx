#include "TRemiAcqirisSoftTdc.h"

#include "Parameters/TRemiAcqirisCfdParameter.h"
#include "Parameters/TRemiAcqirisRisEParameter.h"
#include "Parameters/TRemiAcqirisCoMParameter.h"
#include "TRemiAcqirisEvent.h"
#include "TRemiAcqirisPuls.h"
#include "TRemiAcqirisPeak.h"
#include "TRemiAcqirisChannel.h"

#include "TGo4Picture.h"

#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TMath.h"
#include "TString.h"
#include "TGraph.h"
#include "TArrayI.h"

#include "Riostream.h"

#include "TSpectrum.h"

#include "Math/Interpolator.h"


/** \brief Default constructor. Not to be used */
TRemiAcqirisSoftTdc::TRemiAcqirisSoftTdc()
{

}


/** \brief Principal constructor */
TRemiAcqirisSoftTdc::TRemiAcqirisSoftTdc(ConfigFile* AcqConf, TRemiAcqirisChannel* ParentChannel) : 
	TRemiIncludeFunctions(AcqConf)
{
	if (!fConfigFile)
	{
		std::cout << "**** TRemiAcqirisSoftTdc: No pointer to Config File";
		return;
	}
	
	fParentChannel = ParentChannel;
	
	// Read Information from Config File
	mMethod = ParentChannel->getTdcMethod();
	TString fieldname;
	
	TString paraname, basefolder="Acq", hname, htitle;
	
	// create parameter
	TString folder = basefolder + "/TDC";
	paraname.Form ("Par Chan %i Acq Tdc ", fParentChannel->getChannelNbr()+1);
	switch (mMethod)
	{
		case kCoM :
			paraname += "CoM";
			fParaSoftTdc = MakeParam<TRemiAcqirisCoMParameter>(paraname, folder);
			break;
			
		case kCFD :
			paraname += "CFD";
			fParaSoftTdc = MakeParam<TRemiAcqirisCfdParameter>(paraname, folder);
			break;
			
		case kRisingEdge :
			paraname += "RisE";
			fParaSoftTdc = MakeParam<TRemiAcqirisRisEParameter>(paraname, folder);
			break;
			
		case kSearchHighRes :
			paraname += "HighR";
			fParaSoftTdc = MakeParam<TRemiAcqirisHighRParameter>(paraname, folder);
			break;
			
		case kExtrema :
			paraname += "Extrema";
			fParaSoftTdc = MakeParam<TRemiAcqirisCfdParameter>(paraname, folder);
			break;
			
		case kCFDrev :
			paraname += "CFDrev";
			fParaSoftTdc = MakeParam<TRemiAcqirisCfdParameter>(paraname, folder);
			break;
			
		case kNone :
		default :
			break;
	}
	
	// Create histograms to show contents of each channel
	if (mMethod != 0)
	{
		folder.Form(basefolder + "/raw/AcqChan%02i", fParentChannel->getChannelNbr()+1);

		hname.Form("Acq_Tdc_%02i", fParentChannel->getChannelNbr()+1);
		htitle.Form("Peaks in Acqiris channel %i", fParentChannel->getChannelNbr()+1);
		fHistRawPeaks = MakeH1<TH1I>(hname, htitle, "acq_hist_tdc_raw_bins", "acq_hist_tdc_raw_min", "acq_hist_tdc_raw_max", folder);

		Int_t HistTdchitsMax = fConfigFile->read("acq_hist_tdchits_max", 100);
		hname.Form("Acq_Tdc_Hits_%02i", fParentChannel->getChannelNbr()+1);
		htitle.Form("No. of Peaks in Acqiris channel %i", fParentChannel->getChannelNbr()+1);
		fHistNumRawPeaks = MakeH1<TH1I>(hname, htitle, HistTdchitsMax, -.5, HistTdchitsMax-.5, folder);

		// Initialise Overview mode if requested
		mOverviewMode = fConfigFile->read("acq_use_tdc_overviewmode", false);
		if (mOverviewMode)
			initOverviewModeSpectra();
	}

	// Initialise Watchmode if requested
	mWatchMode = fConfigFile->read("acq_use_tdc_watchmode", false);
	if (mWatchMode)
		initWatchMode();
}

/** \brief Destructor */
TRemiAcqirisSoftTdc::~TRemiAcqirisSoftTdc()
{
//	if (mWatchMode)
//	{
//		delete fBaseline;

//		for (Int_t i=0; i<mNumPulseGraphs; ++i)
//		{
//			delete fBaselines[i];
//		}
//	}
}

void TRemiAcqirisSoftTdc::clear()
{
	if (mWatchMode)
		clearWatchMode();
}


//______________________________fwhm________________________________________________________________________________________
template <typename T>
void TRemiAcqirisSoftTdc::fwhm(TRemiAcqirisPeak * p, TRemiAcqirisPuls *Pulse)
{
	Double_t baseline = p->getParentChannel()->getBaseline(); // vorher Long_t
// 	Double_t threshold = p->getParentChannel()->getNoise(); // vorher Long_t
// 	Long_t timestamp = p->getParentPuls()->getTimestamp();
// 	threshold = p->getParentChannel()->getNoise();
		//threshold = p->getParentPuls()->getParentChannel()->getCFDSettings()->getThreshold(timestamp);	//this is in mV			// habe die folgenden 3 Zeilen auskommentiert zum einen weils den CFD nicht mehr gibt und zum anderen weil horpos nirgendwo verwendet wird -- Moritz 9.1.09 --
		//threshold /= p->getParentPuls()->getParentChannel()->getVertGain();								//now it is in ADC Bytes
		//Double_t horpos = p->getParentPuls()->getParentChannel()->getParentEvent()->getHorpos()*1.e9;

	T * Data = (T *) Pulse->getData();

	//--get peak fwhm--//
	Int_t fwhm_l = 0;
	Int_t fwhm_r = 0;
	Double_t HalfMax = 0.5*p->getMaximum();
	//--go from right towards extrapoint until 0.5*height--//
	for (Int_t i = p->getStartPos(); i <= p->getMaxPos();++i)
	{
		if (TMath::Abs(Data[i]-baseline) > (HalfMax))
		{
			fwhm_l = i;
			break;
		}
				
	}

	//--go from right towards extrpoint until 0.5*height--//
	for (Int_t i = p->getStopPos(); i >= p->getMaxPos();--i)
	{
		if (TMath::Abs(Data[i]-baseline) > (HalfMax))
		{
			fwhm_r = i;
			break;
		}
	}

	//--if the threshold was too high then the start of the puls is already higher than the half of the puls--//
	//--the same is true for the end of the puls--//
	if ((fwhm_r == p->getStopPos()) || (fwhm_l == p->getStartPos()))
	{p->setFWHMNs(-1.); return;}
	// falls garnix getan wurde und noch die Ausgangswerte drin stehen setze die Halbwertsbreite auf einen negativen Wert und gehe zur�ck
	if ( !fwhm_r || !fwhm_l)
	{p->setFWHMNs(-2.); return;}

	//--if we found a right side and a left side, then--//
	//--compute the fwhm with a linear interpolation--//
	//--between the points that are left and right from--//
	//--where the fwhm is, else return here--//
	

	Double_t lx1=fwhm_l-1;
	Double_t ly1=TMath::Abs(Data[fwhm_l-1]-baseline);
	Double_t ly2=TMath::Abs(Data[fwhm_l]-baseline);

	Double_t rx1=fwhm_r;
	Double_t ry1=TMath::Abs(Data[fwhm_r]-baseline);
	Double_t ry2=TMath::Abs(Data[fwhm_r+1]-baseline);

	Double_t fwhm_L = (HalfMax-ly1)/(ly2-ly1) + lx1;
	Double_t fwhm_R = (HalfMax-ry1)/(ry2-ry1) + rx1;

	Double_t FWHM = fwhm_R - fwhm_L;
	p->setFWHMNs(FWHM*fParentChannel->getSampDistNs());
	
	
	//------------------------ ganzen Teil auskommentiert und wieder auf die urspr�ngliche Variante zur�ckgegangen, die nur anhand von zwei Punkten (statt 4) auf jeder Flanke die FWHM extrapoliert -- Moritz 9.1.09 --
	//Double_t lx[4];
	//Double_t ly[4];
	//lx[0] = fwhm_l-2;	ly[0] = TMath::Abs(Data[fwhm_l-2]-baseline);
	//lx[1] = fwhm_l-1;	ly[1] = TMath::Abs(Data[fwhm_l-1]-baseline);
	//lx[2] = fwhm_l-0;	ly[2] = TMath::Abs(Data[fwhm_l-0]-baseline);
	//lx[3] = fwhm_l+1;	ly[3] = TMath::Abs(Data[fwhm_l+1]-baseline);

	//Double_t rx[4];
	//Double_t ry[4];
	//rx[0] = fwhm_r-1;	ry[0] = TMath::Abs(Data[fwhm_r-1]-baseline);
	//rx[1] = fwhm_r-0;	ry[1] = TMath::Abs(Data[fwhm_r-0]-baseline);
	//rx[2] = fwhm_r+1;	ry[2] = TMath::Abs(Data[fwhm_r+1]-baseline);
	//rx[3] = fwhm_r+2;	ry[3] = TMath::Abs(Data[fwhm_r+2]-baseline);

	//Double_t mLeft,cLeft,mRight,cRight;
	//linearRegression(4,lx,ly,mLeft,cLeft);
	//linearRegression(4,rx,ry,mRight,cRight);

	////y = m*x+c => x = (y-c)/m;
	//Double_t fwhm_L = (HalfMax-cLeft)/mLeft;
	//Double_t fwhm_R = (HalfMax-cRight)/mRight;

	//Double_t fwhm = fwhm_R-fwhm_L;
	////--set all found parameters--//
	//p->setFWHM(fwhm);
	//p->setWidth(p->getStopPos()-p->getStartPos());
	//p->setPosHalfLeft(fwhm_L);
	//p->setPosHalfRight(fwhm_R);

}

//__________________________________________Center of Mass_______________________________________
template <typename T>
void TRemiAcqirisSoftTdc::com(TRemiAcqirisPeak * p, TRemiAcqirisPuls *Pulse)
{
	T * Data = (T *) Pulse->getData();
	Int_t baseline = p->getParentChannel()->getBaseline();
	Int_t threshold = dynamic_cast<TRemiAcqirisCoMParameter*>(fParaSoftTdc)->Threshold;
	Int_t timestamp = Pulse->getTimestamp();

	//--this function goes through the puls from start to stop and finds the center of mass--//
	Double_t integral = 0;
	Double_t wichtung = 0;
	Int_t start = p->getStartPos();
	Int_t stop = p->getStopPos();
	
	for (Int_t i = start; i<=stop;++i)
	{
		integral +=  (TMath::Abs( (Double_t)(Data[i]-baseline) )-threshold);			//calc integral
		wichtung += ((TMath::Abs( (Double_t)(Data[i]-baseline) )-threshold)*i);		//calc weight
	}
	p->setPositionNs(wichtung/integral*fParentChannel->getSampDistNs() + timestamp);
}



//______________________________________find start and stop of pulse___________________________________________
template <typename T>
void TRemiAcqirisSoftTdc::startstop(TRemiAcqirisPeak * p, TRemiAcqirisPuls *Pulse)
{
	//--this function will find the start and the stop of the peak--//
	T * Data = (T *) Pulse->getData();
	Int_t baseline = p->getParentChannel()->getBaseline();
	Int_t threshold = p->getParentChannel()->getNoise();
	Int_t timestamp = Pulse->getTimestamp();
	Double_t horpos = p->getParentChannel()->getParentEvent()->getHorpos()*1.e9;

	Int_t pulslength = Pulse->getLength();
	Double_t center = (p->getPosition() - timestamp - horpos); // vorher Int_t
	

	//go left from center until either i == 0, or the datapoint is inside the noise
	//or we go from the previous one (i+1) to the actual one (i) through the baseline
	Int_t i;
	for (i = center; i>=0; --i)				
		if ((TMath::Abs(Data[i]-baseline) < threshold) || (((Data[i]-baseline) * (Data[i+1]-baseline)) < 0) )
			break;
	Int_t start = i;

	//go right from center until either i < pulslength, or the datapoint is inside the noise
	//or we go from the previous one (i-1) to the actual one (i) through the baseline
	for (i = center; i< pulslength; ++i)				
		if ((TMath::Abs(Data[i]-baseline) < threshold) || (((Data[i]-baseline) * (Data[i-1]-baseline)) < 0) )
			break;
	Int_t stop = i;


	p->setStartPos(start);
	p->setStopPos(stop);

}


//___________________________________find Maximum of puls and calcs the height_____________________________________________
template <typename T>
void TRemiAcqirisSoftTdc::maximum(TRemiAcqirisPeak * p, TRemiAcqirisPuls *Pulse)
{
	//--this function will find the maximum of the peak and its position--//
	T * Data = (T *) Pulse->getData();
	Int_t baseline = p->getParentChannel()->getBaseline();
	Int_t start = p->getStartPos();
	Int_t stop = p->getStopPos();
// 	Double_t verticalGain = Pulse->getParentChannel()->getVertGain();
	Int_t maximum=0;
	Int_t maxpos = 0;

	for (Int_t i = start; i<=stop;++i)
	{
		if (TMath::Abs(Data[i]-baseline) > maximum)
		{
			maximum = Data[i];
			maxpos = i;
		}
	}
	p->setMaxPos(maxpos);
	p->setMaximum(maximum);
}


/** 
 * \brief Fits a linear model into a range of points
 * \param Slope Output value: Slope of the resulting linear functions
 * \param Offset Output value: y-axis offset of the resulting linear functions
 * \param nbrPoints No. of points in the fitting range
 * \param x List of x values
 * \param y List of y values
 * 
 * getting a line that follows the form: y(x) = Slope * x + Offset--//
 */
template <typename T> void TRemiAcqirisSoftTdc::linearRegression(Double_t &Slope, Double_t &Offset, Short_t nbrPoints, Int_t *x, T *y)
{
	Double_t MeanX = TMath::Mean(nbrPoints, x);
	Double_t MeanY = TMath::Mean(nbrPoints, y);
	
	Double_t VarianceX = 0., CovarianceXY = 0., DeltaX=0;
	for (Int_t i=0;i<nbrPoints;++i)
	{
		DeltaX = static_cast<Double_t>(x[i]) - MeanX;
		VarianceX += DeltaX * DeltaX;
		CovarianceXY += DeltaX * (static_cast<Double_t>(y[i]) - MeanY);
	}
	
	Slope = CovarianceXY / VarianceX;
	Offset = MeanY - Slope * MeanX;
}

/**
 * \brief Plots the raw spectrum for this channel
 */
void TRemiAcqirisSoftTdc::fillRawSpectra()
{
	for (UInt_t i=0; i<fParentChannel->getNbrPeaks(); i++)
		fHistRawPeaks->Fill(fParentChannel->getPeak(i)->getPosition());

	fHistNumRawPeaks->Fill(fParentChannel->getNbrPeaks());

	// Fill Overview mode spectra if requested
	if (mOverviewMode)
		fillOverviewModeSpectra();
}


/** 
* \brief General function to call to find all peaks in a given pulse
* 
* Actually passes the call to search_for_Peaks_template() with the correct template argument
*/
void TRemiAcqirisSoftTdc::search_for_Peaks(TRemiAcqirisPuls *Pulse)
{
	Short_t nbrOfBits = fParentChannel->getParentEvent()->getNbrBits();
	if(nbrOfBits==TRemiAcqirisSoftTdc::k8Bit)
		search_for_Peaks_template<Char_t>(Pulse);
	else if(nbrOfBits==TRemiAcqirisSoftTdc::k16Bit)
		search_for_Peaks_template<Short_t>(Pulse);
}


/** 
* \brief Internally used function to find all peaks in a given pulse
* \tparam T Data type
*/
template<class T> void TRemiAcqirisSoftTdc::search_for_Peaks_template(TRemiAcqirisPuls *Pulse)
{
	switch(mMethod)
	{
		case kCoM :
			findPeaks_CoM<T>(Pulse);
			break;
			
		case kCFD :
			findPeaks_CFD<T>(Pulse);
			break;
			
		case kRisingEdge :
			findPeaks_RisE<T>(Pulse);
			break;
			
		case kSearchHighRes :
			findPeaks_HighR<T>(Pulse);
			break;
			
		case kExtrema :
			findPeaks_Extrema<T>(Pulse);
			break;
			
		case kCFDrev :
			findPeaks_CFD_rev<T>(Pulse);
			break;
			
		case kNone :
		default :
			break;
	}
	
	// create graphs to watch TDC performance
	if (mWatchMode)
		watchMode<T> (Pulse);
}

/**
 * \brief Find peak position by its center of mass
 * \param Pulse Data object to perform the analysis on
 * \tparam T Type of the raw data values
 * 
 * Peaks are identified by a rising edge
 * NOTE: Added noise in rising edge consideration 2010-12-03
 */
template<class T> void TRemiAcqirisSoftTdc::findPeaks_CoM(TRemiAcqirisPuls *Pulse)
{
	Bool_t risingEdge=false;
	Bool_t firsttime=true;
	Int_t startpos=-1;

	Double_t threshold = dynamic_cast<TRemiAcqirisCoMParameter*>(fParaSoftTdc)->Threshold;
	threshold /= fParentChannel->getVertGain();

	Double_t baseli = fParentChannel->getBaseline(); // vorher Int_t

	T * Data = (T*) Pulse->getData();
// 				Long_t timestamp = Event.getChannel(chan)->getPuls(puls)->getTimestamp();
	risingEdge=false;
	
	//--go through the puls--//
	for (Int_t i=3; i<Pulse->getLength();i++)
		if (   (TMath::Abs(Data[i] - baseli) <= threshold)							//checking for inside noise
		|| ( i == Pulse->getLength()-1)	//check if we are at the second to last index
		|| ( ( (Double_t)(Data[i]-baseli)*(Double_t)(Data[i-1]-baseli) ) < 0.))				//check wether we go through the zero
		{
			if (risingEdge)			//if we had a rising edge before we know that it was a real peak
			{
				TRemiAcqirisPeak * Peak = Pulse->addPeak();
				Peak->setStartPos(startpos);
				Peak->setStopPos(i-1);
				//--height stuff--//
				maximum<T>(Peak, Pulse);
				//--fwhm stuff--//
				fwhm<T>(Peak, Pulse);
				//--center of mass stuff--//
				com<T>(Peak, Pulse);

				//--check the polarity--//
				if (Data[Peak->getMaxPos()] > baseli)		//positive
					Peak->setPolarity(kPositive);
				else if (Data[Peak->getMaxPos()] < baseli)	//negative
					Peak->setPolarity(kNegative);
				else															//error: polarity found
					Peak->setPolarity(kBad);				
			}
			risingEdge = false;
			firsttime=true;
		}
		else		//if the above is not true then we are outside the noise
		{
			if(firsttime)	//if it is the firsttime we are outside the noise
			{
				firsttime = false;
				startpos= i;			//remember the position
			}
			//--if we haven't found a risingEdge before check if we have--//
			if (!risingEdge)
				if ((TMath::Abs(Data[i-2]-baseli) < TMath::Abs(Data[i-1]-baseli) + threshold) && 
					(TMath::Abs(Data[i-1]-baseli) < TMath::Abs(Data[i  ]-baseli) + threshold)  )
				
						risingEdge=true;
		}

}
//______________________________________________________________________________________________________________________
template<class T> void TRemiAcqirisSoftTdc::findPeaks_CFD(TRemiAcqirisPuls *Pulse)
{
	Double_t baseline = fParentChannel->getBaseline();
	//--get the right cfd settings--//
	TRemiAcqirisCfdParameter* CfdParameter = dynamic_cast<TRemiAcqirisCfdParameter*> (fParaSoftTdc);
	Int_t delay = CfdParameter->Delay;
	Double_t walk = CfdParameter->Walk;
	walk /= fParentChannel->getVertGain();
	Double_t threshold = CfdParameter->Threshold;
	threshold /= fParentChannel->getVertGain();
	Double_t fraction = CfdParameter->Fraction;

	Int_t length = Pulse->getLength();
	T * data = (T*) Pulse->getData();


/*

	Float_t *source = new float[length];

	for (Int_t i=0; i<length; i++)
	{
		source[i]  = (Float_t) data[i];
	}

	delete data;

	Float_t *data = new float[length];

	TSpectrum *s = new TSpectrum;
	s->Deconvolution(source, data, length, 1000, 1, 1);

	delete source;
	delete s;

*/


	for(Int_t i=(Int_t)delay+1; i<length-2;i++)
	{

		Double_t fx  = (Double_t)data[i] - baseline;		//the original Point at i
		Double_t fxd = (Double_t)data[i-delay] - baseline;	//the delayed Point	at i 
		Double_t fsx = -fx*fraction + fxd;		//the calculated CFPoint at i

		Double_t fx_1  = (Double_t)data[i+1] - baseline;		//original Point at i+1
		Double_t fxd_1 = (Double_t)data[i+1-delay] - baseline;	//delayed Point at i+1
		Double_t fsx_1 = -fx_1*fraction + fxd_1;		//calculated CFPoint at i+1

		//check whether the criteria for a Peak are fullfilled
		if (((fsx-walk) * (fsx_1-walk)) <= 0 )	//one point above one below the walk
		if (fabs(fx) > threshold)			//original point above the threshold
		{
			// tests for cases where (fsx-walk)*(fsx_1-walk) == 0
			if (fabs(fsx-fsx_1) < 1e-8)	//both points are on the walk	-> not a Signal
				continue;
			else if ((fsx-walk) == 0)	//only first is on walk			-> Signal
				;
			else if ((fsx_1-walk) == 0)	//only second is on walk		-> might be a Signal but at a later point in time
				continue;

			// check if polarity of the signal and the value of the original puls fit together
			Int_t polarity=0;
			if (fsx > fsx_1) {		//neg polarity
				if (data[i] > baseline)	//but pos Puls .. skip
					continue;
				polarity=kNegative;
			}
			if (fsx < fsx_1) {		//pos polarity
				if (data[i] < baseline)	//but neg Puls .. skip
					continue;
				polarity=kPositive;			
			}

			//--later we need two more points, create them here--//
			Double_t fx_m1 = data[i-1] - baseline;		//the original Point at i-1
			Double_t fxd_m1 = data[i-1-delay] - baseline;	    //the delayed Point	at i-1 
			Double_t fsx_m1 = -fx_m1*fraction + fxd_m1;			//the calculated CFPoint at i-1

			Double_t fx_2 = data[i+2] - baseline;			//original Point at i+2
			Double_t fxd_2 = data[i+2-delay] - baseline;		//delayed Point at i+2
			Double_t fsx_2 = -fx_2*fraction + fxd_2;				//calculated CFPoint at i+2

			//--find x with a linear interpolation between the two points--//
			Double_t m = fsx_1-fsx;					//(fsx-fsx_1)/(i-(i+1));
			Double_t xLin = i + (walk - fsx)/m;		//PSF fx = (x - i)*m + cfs[i]

			//--make a linear regression to find the slope of the leading edge--//
			//Double_t mslope,cslope;
			//const Double_t xslope[3] = {i-delay,i+1-delay,i+2-delay};
			//const Double_t yslope[3] = {fxd,fxd_1,fxd_2};
			//linearRegression(3,xslope,yslope,mslope,cslope);

			//--find x with a cubic polynomial interpolation between four points--//
			//--do this with the Newtons interpolation Polynomial--//
			Double_t x[4] = {(Double_t) i-1, (Double_t) i, (Double_t) i+1, (Double_t) i+2};				//x vector
			Double_t y[4] = {fsx_m1,fsx,fsx_1,fsx_2};		//y vector
			Double_t coeff[4] = {0,0,0,0};				//Newton coeff vector
			createNewtonPolynomial(x,y,coeff);

			//--numericaly solve the Newton Polynomial--//
			Double_t xPoly = findXForGivenY(x,coeff,walk,xLin);
			Double_t pos = xPoly*fParentChannel->getSampDistNs() + Pulse->getTimestamp();

			// Add Peak
			TRemiAcqirisPeak * Peak = Pulse->addPeak();
			Peak->setPositionNs(pos);
			Peak->setPolarity(polarity);

			// These are only needed for the Acq/Overview plots
			if (mOverviewMode)
			{
				startstop<T>(Peak, Pulse);
				maximum<T>(Peak, Pulse);
				fwhm<T>(Peak, Pulse);
			}

			// to prevent the same Signal to be found twice increase the index i by the value of delay
			i+=delay;
		}
	}
}

//______________________________________________________________________________________________________________________
template<class T> void TRemiAcqirisSoftTdc::findPeaks_Extrema(TRemiAcqirisPuls *Pulse)
{
	// use the CFD parameters for the threshold
	TRemiAcqirisCfdParameter* CfdParameter = dynamic_cast<TRemiAcqirisCfdParameter*> (fParaSoftTdc);
	Double_t threshold = CfdParameter->Threshold;
	threshold /= fParentChannel->getVertGain();

	// get some parameters and a pointer to the data arrays from channel and pulse
	Double_t baseline = fParentChannel->getBaseline();
	Int_t ni = Pulse->getLength();
	T * data = (T*) Pulse->getData();


	// feed the data into the interpolator
	Double_t xi[ni], yi[ni];

	for ( Int_t i = 0; i < ni; ++i )
	{
		xi[i] = (Double_t) i;
		yi[i] = (Double_t) data[i] - baseline;
	}

	ROOT::Math::Interpolator * inter = new ROOT::Math::Interpolator(ni, ROOT::Math::Interpolation::kLINEAR);
	inter->SetData(ni, xi, yi);


	// get the derivative
	Double_t di[ni];

	for ( Int_t i = 0; i < ni; ++i )
	{
		di[i] = inter->Deriv(xi[i]);
	}


	// find the zero crossings of the derivative
	Double_t d, d2, x, x2, y, y2;
	Int_t polarity;
	Double_t pos, m;

	for ( Int_t i = 0; i < ni-1; ++i )
	{
		d  = di[i];
		d2 = di[i+1];

		x  = xi[i];
		x2 = xi[i+1];

		y  = yi[i];
		y2 = yi[i+1];


		// test wether the current point should be skipped
		// every point that gets through these tests is a signal

		// this is noise
		if ((fabs(y) < threshold) && (fabs(y2) < threshold))
			continue;

		// a and b have the same sign -> not a crossing
		if (d * d2 > 0)
			continue;

		// a and b are equal -> slope is constant
		if (fabs(d - d2) < 1e-8)
			continue;

		// second point is the crossing, will find it in the next iteration
		if ((d != 0) && (d2 == 0))
			continue;

		// ignore maxima (minima) in the negative (positive)
		polarity = 0;
		if (d > d2) { // maximum
			if (y < 0) // below zero
				continue;
			polarity = kPositive;
		}
		if (d < d2) { // minimum
			if (y > 0) // above zero
				continue;
			polarity = kNegative;
		}


		// calculate the crossing
		m = d2 / (d - d2);
		pos = x2 - (x - x2) * m;

		// convert the position from indices to time
		pos = pos * fParentChannel->getSampDistNs() + Pulse->getTimestamp();

		// add the postion as found peak to the pulse
		TRemiAcqirisPeak * Peak = Pulse->addPeak();
		Peak->setPositionNs(pos);
		Peak->setPolarity(polarity);

		// These are only needed for the Acq/Overview plots
		if (mOverviewMode)
		{
			startstop<T>(Peak, Pulse);
			maximum<T>(Peak, Pulse);
			fwhm<T>(Peak, Pulse);
		}
	}

	// clean up
	delete inter;
}

//______________________________________________________________________________________________________________________
template<class T> void TRemiAcqirisSoftTdc::findPeaks_CFD_rev(TRemiAcqirisPuls *Pulse)
{
	Double_t baseline = fParentChannel->getBaseline();
	//--get the right cfd settings--//
	TRemiAcqirisCfdParameter* CfdParameter = dynamic_cast<TRemiAcqirisCfdParameter*> (fParaSoftTdc);
	Int_t delay = CfdParameter->Delay;
	Double_t walk = CfdParameter->Walk;
	walk /= fParentChannel->getVertGain();
	Double_t threshold = CfdParameter->Threshold;
	threshold /= fParentChannel->getVertGain();
	Double_t fraction = CfdParameter->Fraction;

	Int_t length = Pulse->getLength();
	T * data_orig = (T*) Pulse->getData();
	T data[length];
	for (Int_t i=0; i<length; i++)
		data[i] = data_orig[length-1-i];


	for(Int_t i=(Int_t)delay+1; i<length-2;i++)
	{

		Double_t fx  = (Double_t)data[i] - baseline;		//the original Point at i
		Double_t fxd = (Double_t)data[i-delay] - baseline;	//the delayed Point	at i 
		Double_t fsx = -fx*fraction + fxd;		//the calculated CFPoint at i

		Double_t fx_1  = (Double_t)data[i+1] - baseline;		//original Point at i+1
		Double_t fxd_1 = (Double_t)data[i+1-delay] - baseline;	//delayed Point at i+1
		Double_t fsx_1 = -fx_1*fraction + fxd_1;		//calculated CFPoint at i+1

		//check whether the criteria for a Peak are fullfilled
		if (((fsx-walk) * (fsx_1-walk)) <= 0 )	//one point above one below the walk
		if (fabs(fx) > threshold)			//original point above the threshold
		{
			// tests for cases where (fsx-walk)*(fsx_1-walk) == 0
			if (fabs(fsx-fsx_1) < 1e-8)	//both points are on the walk	-> not a Signal
				continue;
			else if ((fsx-walk) == 0)	//only first is on walk			-> Signal
				;
			else if ((fsx_1-walk) == 0)	//only second is on walk		-> might be a Signal but at a later point in time
				continue;

			// check if polarity of the signal and the value of the original puls fit together
			Int_t polarity=0;
			if (fsx > fsx_1) {		//neg polarity
				if (data[i] > baseline)	//but pos Puls .. skip
					continue;
				polarity=kNegative;
			}
			if (fsx < fsx_1) {		//pos polarity
				if (data[i] < baseline)	//but neg Puls .. skip
					continue;
				polarity=kPositive;
			}

			//--later we need two more points, create them here--//
			Double_t fx_m1 = data[i-1] - baseline;		//the original Point at i-1
			Double_t fxd_m1 = data[i-1-delay] - baseline;	    //the delayed Point	at i-1 
			Double_t fsx_m1 = -fx_m1*fraction + fxd_m1;			//the calculated CFPoint at i-1

			Double_t fx_2 = data[i+2] - baseline;			//original Point at i+2
			Double_t fxd_2 = data[i+2-delay] - baseline;		//delayed Point at i+2
			Double_t fsx_2 = -fx_2*fraction + fxd_2;				//calculated CFPoint at i+2

			//--find x with a linear interpolation between the two points--//
			Double_t m = fsx_1-fsx;					//(fsx-fsx_1)/(i-(i+1));
			Double_t xLin = i + (walk - fsx)/m;		//PSF fx = (x - i)*m + cfs[i]

			//--make a linear regression to find the slope of the leading edge--//
			//Double_t mslope,cslope;
			//const Double_t xslope[3] = {i-delay,i+1-delay,i+2-delay};
			//const Double_t yslope[3] = {fxd,fxd_1,fxd_2};
			//linearRegression(3,xslope,yslope,mslope,cslope);

			//--find x with a cubic polynomial interpolation between four points--//
			//--do this with the Newtons interpolation Polynomial--//
			Double_t x[4] = {(Double_t) i-1, (Double_t) i, (Double_t) i+1, (Double_t) i+2};				//x vector
			Double_t y[4] = {fsx_m1,fsx,fsx_1,fsx_2};		//y vector
			Double_t coeff[4] = {0,0,0,0};				//Newton coeff vector
			createNewtonPolynomial(x,y,coeff);

			//--numericaly solve the Newton Polynomial--//
			Double_t xPoly = findXForGivenY(x,coeff,walk,xLin);
			xPoly = length - xPoly; // reverse the reversal
			Double_t pos = xPoly*fParentChannel->getSampDistNs() + Pulse->getTimestamp();

			// Add Peak
			TRemiAcqirisPeak * Peak = Pulse->addPeak();
			Peak->setPositionNs(pos);
			Peak->setPolarity(polarity);

			// These are only needed for the Acq/Overview plots
			if (mOverviewMode)
			{
				startstop<T>(Peak, Pulse);
				maximum<T>(Peak, Pulse);
				fwhm<T>(Peak, Pulse);
			}

			// to prevent the same Signal to be found twice increase the index i by the value of delay
			i+=delay;
		}
	}
}

//______________________________________________________________________________________________________________________
template<class T> void TRemiAcqirisSoftTdc::findPeaks_HighR(TRemiAcqirisPuls *Pulse)
{
    // TSpectrum API uses double instead of float in ROOT6
    #if ROOT_VERSION_CODE < ROOT_VERSION(5,99,0)
        typedef Float_t TSPEC_TYPE;
    #else
        typedef Double_t TSPEC_TYPE;
    #endif

//	Double_t baseline = fParentChannel->getBaseline();
	TRemiAcqirisHighRParameter* HighRParameter = dynamic_cast<TRemiAcqirisHighRParameter*> (fParaSoftTdc);

	Float_t sigma           = HighRParameter->Sigma;
	Float_t threshold       = HighRParameter->Threshold;
	Bool_t backgroundremove = HighRParameter->BackgroundRemove;
	Int_t deconiterations   = HighRParameter->DeconIterations;
	Bool_t markov           = HighRParameter->Markov;
	Int_t averwindow        = HighRParameter->AverWindow;

	Int_t length = Pulse->getLength();
	T * data = (T*) Pulse->getData();

	TSPEC_TYPE* source = new TSPEC_TYPE[length];
//	TSPEC_TYPE* source = new TSPEC_TYPE[2*length-1];
//	TSPEC_TYPE* dest   = new TSPEC_TYPE[length];

	for (Int_t i=0; i<length; i++)
	{
		source[i] = -((float) data[i]);
	}

//	for (Int_t i=1; i<=(2*length-1); i++)
//	{
//		source[2*i-2] = -((float) data[i-1]);
//		source[2*i-1] = -((float) data[i-1] + (float) data[i]) / 2;
////		cout << "source: " << source[i] << endl;
//	}

	TSpectrum* s = new TSpectrum;

	Int_t nfound = s->SearchHighRes(source, source, length, sigma, threshold, backgroundremove, deconiterations, markov, averwindow);
//	Int_t nfound = s->SearchHighRes(source, source, 2*length-1, sigma, threshold, backgroundremove, deconiterations, markov, averwindow);
//	Int_t nfound = s->SearchHighRes(source, source, length, 4, 15, kTRUE, 3, kFALSE, 3);
//	Int_t nfound = s->SearchHighRes(source, source, length, sigma, threshold, 1, deconiterations, 0, averwindow);

	TSPEC_TYPE* xpeaks = s->GetPositionX();
	Float_t pos;

	for (Int_t i=0; i<nfound; i++)
	{
		pos = xpeaks[i] * fParentChannel->getSampDistNs() + Pulse->getTimestamp();
//		cout << fixed << setprecision(9) << "Found peak pos: " << pos << "	xpeaks: " << xpeaks[i]<< endl;

		// Add Peak
		TRemiAcqirisPeak* Peak = Pulse->addPeak();
		Peak->setPositionNs(pos);
		Peak->setPolarity(kNegative);

		// These are only needed for the Acq/Overview plots
		if (mOverviewMode)
		{
			startstop<T>(Peak, Pulse);
			maximum<T>(Peak, Pulse);
			fwhm<T>(Peak, Pulse);
		}
	}

	delete source;
	delete s;
}

/**
 * \brief Find peaks using a "numerical" CFD approach that considers only the rising edge of a signal
 */
template <class T> void TRemiAcqirisSoftTdc::findPeaks_RisE(TRemiAcqirisPuls *Pulse)
{
	TRemiAcqirisRisEParameter* RisEParameter = dynamic_cast<TRemiAcqirisRisEParameter*> (fParaSoftTdc);
	T* data; // Pointer to the data in the pulse
	TArrayI xPoints; // Pointer to the list of correspondent xvalues
	Int_t lengthXpoints = 0; // Length of current xPoints Lists
	Int_t* xRising (0); // Pointer to the x-values of a rising edge
	T* yRising (0); // Pointer to the y-values of a rising edge
	Short_t polarity = 0;
	Short_t polarityRising = 0; // Polarity of the current rising edge
	Short_t noise = ( RisEParameter->Noise >= 0 ) ? RisEParameter->Noise : fParentChannel->getNoise();
	Short_t lengthRising = 2; // Length of the current rising edge
	Short_t lengthFalling = 2; // Length of the current falling edge
	Short_t lengthBetween = 2; // Current length between rising and falling edge
	Bool_t	inpeak = kFALSE; // flag set if we are currently in a pulse
	Int_t lengthPulse;
	
	lengthPulse = Pulse->getLength();
	
	// Check length of list containing x-values
	if (lengthPulse > lengthXpoints)
	{
		xPoints.Set(lengthPulse);
		for (Int_t k=lengthXpoints; k<lengthPulse; k++)
			xPoints[k] = k;
		lengthXpoints = lengthPulse;
	}
	
	// go through the pulse's data
	data = (T*) Pulse->getData();
	for(Int_t i=1; i<lengthPulse; i++)
	{
		if (polarity == 0) // no previous edge existant
		{
			polarity = TMath::Sign(1, data[i]-data[i-1]);
			if (inpeak) // We are in a peak, i.e. between a rising and a falling edge
			{
				lengthBetween++;
			}
		}
		else if ( ((polarity == -1) && (data[i] < data[i-1] + noise)) || ((polarity == +1) && (data[i] + noise > data[i-1])) ) // continued edge
		{
			if (inpeak) // We are in a pulse, i.e. this must be a falling edge
				lengthFalling++;
			else // This must be a rising edge
				lengthRising++;
		}
		else // we have reached the end of a continuous edge
		{
			if (inpeak) // This must have been a falling edge, because we were in a peak
			{
				if (lengthFalling >= RisEParameter->MinPointsFalling)
				{
					if (polarityRising * polarity == -1) // Check for opposing signs in the edges: this is a peak!
					{
						calculatePeak_RisE<T>(Pulse, data, lengthRising, xRising, yRising, lengthFalling, &xPoints[i-lengthFalling], &data[i-lengthFalling], polarityRising);
						inpeak = kFALSE;
						lengthRising = 2;
					}
					else // edges have the same polarity 
					{
						if (lengthFalling >= RisEParameter->MinPointsRising) // set "falling" edge as new rising edge if it is long enough
						{
							lengthRising = lengthFalling;
							xRising = xPoints.fArray + (i-lengthRising);
							yRising = data + (i-lengthRising);
							lengthFalling = 2;
							lengthBetween = 0;
						}
						else // reset peak completely
						{
							inpeak = kFALSE;
							lengthRising = 2;
						}
					}
				}
				else // Falling edge was too short, go on looking for a new one
				{
					lengthBetween += lengthFalling;
					lengthFalling = 2;
				}
			}
			else // This must have been a rising edge
			{
				if (lengthRising >= RisEParameter->MinPointsRising) // Edge is long enough to classify it as a rising one
				{
					lengthBetween=0;
					lengthFalling=2;
					inpeak = kTRUE;
					xRising = xPoints.fArray + (i-lengthRising);
					yRising = data + (i-lengthRising);
					polarityRising = polarity;
				}
				else // Edge was too short, prepare to start a new one
					lengthRising = 2; 
			}
			polarity = TMath::Sign(1, data[i]-data[i-1]); // Set new polarity
		}
		
		// check if we are locked in a peak that has no propper falling edge
		if ( inpeak && (lengthBetween > RisEParameter->MaxPointsBetween) ) 
		{
			inpeak = kFALSE;
			lengthRising = 2;
		}
		
	}
}

/**
 * \brief calculates a peak found with the rising edge method
 * \param Pulse Pointer to the current Pulse
 * \param T	Pointer to the data trace
 * \param lengthRising length of rising edge
 * \param pointsRising pointer to the rising edge
 * \param lengthFalling length of falling edge
 * \param polarity designated polarity of the peak
 */
template <class T> void TRemiAcqirisSoftTdc::calculatePeak_RisE (TRemiAcqirisPuls* Pulse, T* data, Short_t lengthRising, Int_t* xRising, T* yRising, Short_t lengthFalling, Int_t* xFalling, T* yFalling, Short_t polarity)
{
	TRemiAcqirisPeak* Peak = Pulse->addPeak();
	
	// set first and last point of the peak
	Peak->setStartPos(xRising[0]);
	Peak->setStopPos(xFalling[lengthFalling-1]);
	Peak->setPolarity(polarity);
	
	// Find linear regression of rising edge
	Double_t slopeRising = 0, offsetRising = 0, maxRising, minRising, Position;
	linearRegression(slopeRising, offsetRising, lengthRising, xRising, yRising);
// 	maxRising = offsetRising + static_cast<Double_t>(xRising[lengthRising-1]) * slopeRising;
// 	minRising = offsetRising + static_cast<Double_t>(xRising[0]) * slopeRising;
 	maxRising = static_cast<Double_t>(yRising[lengthRising-1]);
 	minRising = static_cast<Double_t>(yRising[0]);
	Peak->setMaximum(maxRising);
	Position = ( minRising + (maxRising - minRising)/2 - offsetRising) / slopeRising;
	Peak->setPositionNs(Pulse->getTimestamp() + Position*fParentChannel->getSampDistNs());
	
	// to determine the FWHM, do a linear regression of the falling edge
	Double_t slopeFalling = 0, offsetFalling = 0, maxFalling, minFalling, Marker;
	maxFalling = offsetFalling + static_cast<Double_t>(xFalling[0]) * slopeFalling;
	minFalling = offsetFalling + static_cast<Double_t>(xFalling[lengthFalling-1]) * slopeFalling;
	Marker = ( minFalling + (maxFalling - minFalling)/2 - offsetRising) / slopeRising;
	Peak->setFWHMNs((Marker - Position)*fParentChannel->getSampDistNs());
}


//_______________________________________________________________________________________________________________________
void TRemiAcqirisSoftTdc::createNewtonPolynomial(Double_t * x, Double_t * y, Double_t * coeff)
{
	//**this function creates the coefficients for Newton interpolating Polynomials	**//
	//**Newton Polynomial are Created from n Points and have the form				**//
	//**p(x) = c0 + c1(x-x0) + c2(x-x0)(x-x1)+...+c_(n-1)(x-x0)(x-x1)...(x-x_(n-2))	**//
	//**given that you have n Points (x0,y0), (x1,y1), ..., (x_(n-1),y_(n-1))		**//

	Double_t f_x0_x1 = (y[1]-y[0]) / (x[1]-x[0]);
	Double_t f_x1_x2 = (y[2]-y[1]) / (x[2]-x[1]);
	Double_t f_x2_x3 = (y[3]-y[2]) / (x[3]-x[2]);

	Double_t f_x0_x1_x2 = (f_x1_x2 - f_x0_x1) / (x[2]-x[0]);
	Double_t f_x1_x2_x3 = (f_x2_x3 - f_x1_x2) / (x[3]-x[1]);

	Double_t f_x0_x1_x2_x3 = (f_x1_x2_x3 - f_x0_x1_x2) / (x[3]-x[0]);

	coeff[0] = y[0];
	coeff[1] = f_x0_x1;
	coeff[2] = f_x0_x1_x2;
	coeff[3] = f_x0_x1_x2_x3;
}
//_________________________________evaluate Newton Polynomial____________________________
Double_t TRemiAcqirisSoftTdc::evalNewtonPolynomial(Double_t * x,Double_t * coeff, Double_t X)
{
	//**this function evaluates the Newton Polynomial that was created from n Points**//
	//** (x0,y0),..., (x(n-1),y(n-1)) with coefficients (c0,...,c(n-1))				**//
	//**using Horner's Rule															**//

	Double_t returnValue = coeff[3];
	returnValue = returnValue * (X - x[2]) + coeff[2];
	returnValue = returnValue * (X - x[1]) + coeff[1];
	returnValue = returnValue * (X - x[0]) + coeff[0];

	return returnValue;
}
//_________________________________Achims Numerical Approximation______________________
Double_t TRemiAcqirisSoftTdc::findXForGivenY(Double_t * x, Double_t * coeff, Double_t Y, Double_t Start)
{
	//initialisiere die Grenzen//
	TRemiAcqirisPunkt Low(x[1], evalNewtonPolynomial(x,coeff,x[1]));
	TRemiAcqirisPunkt Up (x[2], evalNewtonPolynomial(x,coeff,x[2]));

	//initialisiere den iterierenden Punkt mit dem Startwert//
	TRemiAcqirisPunkt p (Start, evalNewtonPolynomial(x,coeff,Start));
	
	//ist der Startpunkt schon der richtige Punkt//
	//liefere den dazugeh�rigen x-Wert zur�ck//
	if (p.y() == Y)
		return p.x();

	//finde heraus ob es ein positiver oder ein negativer Durchgang ist//
	Bool_t Neg = (Low.y() > Up.y())?true:false;

	//der Startpunkt soll die richtige neue Grenze bilden//
	if (Neg)	//wenn es ein negativer Druchgang ist
	{
		if (p.y() > Y)		//ist der y-Wert gr�sser als der gewollte
			Low = p;		//bildet der Punkt die neue untere Grenze
		else if (p.y() < Y)	//ist der y-Wert ist kleiner als der gewollte
			Up = p;			//bildet der Punkt die neue obere Grenze
		else				//ist der Punkt genau getroffen
			return p.x();	//liefer den dazugeh�rigen x-Wert zur�ck
	}
	else		//wenn es ein positiver Druchgang ist
	{
		if (p.y() > Y)		//und der y-Wert gr�sser als der gewollte
			Up = p;			//bildet der Punkt die neue obere Grenze
		else if (p.y() < Y)	//und y-Wert ist kleiner als der gewollte
			Low = p;		//bildet der Punkt die neue untere Grenze
		else				//ist der Punkt genau getroffen
			return p.x();	//liefer den dazugeh�rigen x-Wert zur�ck
	}


	while((Up.x()-Low.x()) > 0.005) //iteriere solange bis der Abstand zwischen den x-Werten
								//kleiner als 0.005
	{
		//bilde das arithmetische Mittel zwischen beiden Grenzen//
		//das ist der neue x-Wert unseres Punktes//
		p.x() = 0.5 * (Up.x()+Low.x());
		//finde den dazugeh�rigen y-Wert//
		p.y() = evalNewtonPolynomial(x,coeff,p.x());
	
		if (Neg)	//wenn es ein negativer Druchgang ist
		{
			if (p.y() > Y)		//und der y-Wert gr�sser als der gewollte
				Low = p;		//bildet der Punkt die neue untere Grenze
			else if (p.y() < Y)	//und der y-Wert ist kleiner als der gewollte
				Up = p;			//bildet der Punkt die neue obere Grenze
			else				//ist der Punkt genau getroffen
				return p.x();		//liefer den dazugeh�rigen x-Wert zur�ck
		}
		else		//wenn es ein positiver Druchgang ist
		{
			if (p.y() > Y)		//und der y-Wert gr�sser als der gewollte
				Up = p;			//bildet der Punkt die neue obere Grenze
			else if (p.y() < Y)	//und y-Wert ist kleiner als der gewollte
				Low = p;		//bildet der Punkt die neue untere Grenze
			else				//ist der Punkt genau getroffen
				return p.x();		//liefer den dazugeh�rigen x-Wert zur�ck
		}
		//std::cout<<"("<<Low.x<<","<<Low.y<<")   ("<<p.x<<","<<p.y<<")   ("<<Up.x<<","<<Up.y<<") "<<Y<<std::endl;
	}
	return ((Up.x() + Low.x())*0.5);
}


/**
* \brief Initialise watchmode component
*/
void TRemiAcqirisSoftTdc::initWatchMode()
{
	TString picname, pictitle, gname, gtitle, folder, basefolder="Acq";
	mNumPulseGraphColumns = fConfigFile->read("acq_tdc_watchmode_num_columns", 3);
	mNumPulseGraphs = fConfigFile->read("acq_tdc_watchmode_num_pulses", 5);
	mNumPulsesOverlay = fConfigFile->read("acq_tdc_watchmode_num_pulses_overlay", 10);

	// Baseline function
	fBaseline = new TF1 ("baseline", "[0]", -100, 1e6);
	fBaseline->SetLineWidth(1);
	
	fBaselines = std::vector<TF1*> (mNumPulseGraphs);
	for (Int_t i=0; i<mNumPulseGraphs; ++i)
	{
		fBaselines[i] = new TF1 ("baseline_" + TString::Format("%i", i), "[0]", -100, 1e6);
		fBaselines[i]->SetLineWidth(1);
	}

	Int_t row, col;
	if (mMethod != 0)
	{
		picname.Form("TdcPicAcqChan%i", fParentChannel->getChannelNbr()+1);
		pictitle.Form("Watching Tdc performance for Acq channel %i", fParentChannel->getChannelNbr()+1);
		folder = basefolder + "/TDCwatch";
		fPicGraphs = MakePicture(picname, pictitle, folder, ((mNumPulseGraphs-1)/mNumPulseGraphColumns)+1, mNumPulseGraphColumns);
		fPicGraphs->SetSuperimpose(kTRUE);
		fPicGraphs->SetAutoScale(kTRUE);

		// Form overlay picture
		picname.Form("TdcPicAcqChan%iOver", fParentChannel->getChannelNbr()+1);
		pictitle.Form("Watching centering performance of SoftTDC for Acq Channel %i", fParentChannel->getChannelNbr()+1);
		fPicOverlay = MakePicture(picname, pictitle, folder);
		AddLineToPicture(0, -1000, 0, 1000, fPicOverlay);
		fPicOverlay->SetSuperimpose(kTRUE);
		fPicOverlay->SetAutoScale(kTRUE);
		fPicOverlay->SetHisTitle(kFALSE);
		fPicOverlay->SetLegendDraw(kFALSE);

		// Add line for baseline
		AddTF1ToPicture(fBaseline, fPicOverlay);

		fPulseGraph = std::vector<TGraph*> (mNumPulseGraphs);
		fPeakGraph = std::vector<TGraph*> (mNumPulseGraphs);
		for (Int_t j=0; j<mNumPulseGraphs; j++)
		{
			row = j/mNumPulseGraphColumns; // Current row in picture
			col = j%mNumPulseGraphColumns; // Current column in picture

			// First add graph with pulse
			gname.Form("PulseGraph_%i_%i", fParentChannel->getChannelNbr()+1, j);
			gtitle.Form("Pulse No. %i in Channel %i", j, fParentChannel->getChannelNbr()+1);
			fPulseGraph[j] = MakeGraph(gname, gtitle, folder);
			AddGraphToPicture(fPulseGraph[j], fPicGraphs, row, col);
			fPicGraphs->Pic(row, col)->SetDrawOption("AL"); // Draw lines
			fPicGraphs->Pic(row, col)->SetLegendDraw(kFALSE);

			// Than add graph with peak markers
			gname.Form("PeakGraph_%i_%i", fParentChannel->getChannelNbr()+1, j);
			gtitle.Form("Peaks in Pulse No. %i in Channel %i", j, fParentChannel->getChannelNbr()+1);
			fPeakGraph[j] = MakeGraph(gname, gtitle, folder);
			AddGraphToPicture(fPeakGraph[j], fPicGraphs, row, col);
			fPicGraphs->Pic(row, col)->SetDrawOption("P"); // Draw points (markers)
			fPicGraphs->Pic(row, col)->SetMarkerAtt(2, 2, 33);

			// Add line for baseline (cannot be the identical TF1!)
			AddTF1ToPicture(fBaselines[j], fPicGraphs, row, col);
		}

		// Graphs for overlay picture
		fPulseGraphOv = std::vector<TGraph*> (mNumPulsesOverlay);
		for (Int_t j=0; j<mNumPulsesOverlay; j++)
		{
			gname.Form("PulseGraphOv_%i_%i", fParentChannel->getChannelNbr()+1, j);
			gtitle.Form("Pulse No. %i in Channel %i", j, fParentChannel->getChannelNbr()+1);
			fPulseGraphOv[j] = MakeGraph(gname, gtitle, folder);
			AddGraphToPicture(fPulseGraphOv[j], fPicOverlay);
			fPicOverlay->Pic(0,0)->SetDrawOption("AL"); // Draw lines
			fPicOverlay->Pic(0,0)->SetAutoScale(kTRUE);
		}
	}
}



/**
 * \brief Creates graphs to watch TDC performance
 * \param Event Current TRemiAcqirisEvent
 */
template<class T> void TRemiAcqirisSoftTdc::watchMode(TRemiAcqirisPuls* Pulse)
{
	if ( (Pulse->getIndex() < mNumPulseGraphs) || (Pulse->getIndex() < mNumPulsesOverlay) )
	{
		TRemiAcqirisPeak* ThisPeak(0);

		Double_t vertgain = fParentChannel->getVertGain();
		Double_t baseline = fParentChannel->getBaseline();

		fBaseline->SetParameter(0, baseline*vertgain);

		for (Int_t i=0; i<mNumPulseGraphs; ++i)
		{
			fBaselines[i]->SetParameter(0, baseline*vertgain);
		}

		T * Data = (T*) Pulse->getData();
		
		// Create graph of pulse
		Int_t psize = Pulse->getLength();
		Int_t pindex = Pulse->getIndex();
		if (pindex < mNumPulseGraphs)
		{
			fPulseGraph[pindex]->Set(psize);
			for (Int_t i=0; i<psize; i++)
				fPulseGraph[pindex]->SetPoint(i, fParentChannel->getParentEvent()->getHorpos() + static_cast<Float_t>( Pulse->getTimestamp()) + fParentChannel->getParentEvent()->getSampleInterval() * 1e9 * static_cast<Float_t>(i), Data[i]*vertgain);

			// Create peak markers
			fPeakGraph[pindex]->Set(Pulse->getNbrPeaks());
			for (UInt_t peak=0; peak<Pulse->getNbrPeaks(); peak++)
			{
				ThisPeak = Pulse->getPeak(peak);
				fPeakGraph[pindex]->SetPoint(peak, ThisPeak->getPosition(), baseline*vertgain);
			}
		}
		// Now create shifted graph of pulse for overlay picture
		if ( (Pulse->getIndex() < mNumPulsesOverlay) && (Pulse->getNbrPeaks()) )
		{
			fPulseGraphOv[pindex]->Set(psize);
			for (Int_t i=0; i<psize; i++)
				fPulseGraphOv[pindex]->SetPoint(i, fParentChannel->getParentEvent()->getHorpos() + static_cast<Float_t>( Pulse->getTimestamp() ) + fParentChannel->getParentEvent()->getSampleInterval() * 1e9 * static_cast<Float_t>(i) - Pulse->getPeak(0)->getPosition(), Data[i]*vertgain);
		}
	}
}

void TRemiAcqirisSoftTdc::clearWatchMode()
{
}


/**
 * \brief Creates histograms for TDC performance overview
 */
void TRemiAcqirisSoftTdc::initOverviewModeSpectra()
{
	TString hname, htitle, folder;
	folder.Form("Acq/Overview/AcqChan%02i", fParentChannel->getChannelNbr()+1);


	hname.Form("Acq_Tdc_FWHM_%02i", fParentChannel->getChannelNbr()+1);
	htitle.Form("FWHM of Acqiris channel %i", fParentChannel->getChannelNbr()+1);
	fHistFWHM = MakeH1<TH1I>(hname, htitle, 100, -2, 50, folder);

	hname.Form("Acq_Tdc_Height_%02i", fParentChannel->getChannelNbr()+1);
	htitle.Form("Height of Acqiris channel %i", fParentChannel->getChannelNbr()+1);
	fHistMax = MakeH1<TH1I>(hname, htitle, 100, 0, 5000, folder);

	hname.Form("Acq_Tdc_Pos_%02i", fParentChannel->getChannelNbr()+1);
	htitle.Form("Position of Acqiris channel %i", fParentChannel->getChannelNbr()+1);
	fHistPos = MakeH1<TH1I>(hname, htitle, 100, 0, 2000, folder);


	hname.Form("Acq_Tdc_FWHM_Height_%02i", fParentChannel->getChannelNbr()+1);
	htitle.Form("FWHM vs. Height of Acqiris channel %i", fParentChannel->getChannelNbr()+1);
	fHistFWHMvsMax = MakeH2<TH2I>(hname, htitle, 300, -2, 50, 300, 0, 5000, folder, "FWHM", "Height");

	hname.Form("Acq_Tdc_Pos_FWHM_%02i", fParentChannel->getChannelNbr()+1);
	htitle.Form("Position vs. FWHM of Acqiris channel %i", fParentChannel->getChannelNbr()+1);
	fHistPosVsFWHM = MakeH2<TH2I>(hname, htitle, 300, 0, 2000, 300, -2, 50, folder, "Position", "FWHM");

	hname.Form("Acq_Tdc_Pos_Height_%02i", fParentChannel->getChannelNbr()+1);
	htitle.Form("Position vs. Height of Acqiris channel %i", fParentChannel->getChannelNbr()+1);
	fHistPosVsMax = MakeH2<TH2I>(hname, htitle, 300, 0, 2000, 300, 0, 5000, folder, "Position", "Height");


	hname.Form("Acq_Tdc_Start_vs_Stop_%02i", fParentChannel->getChannelNbr()+1);
	htitle.Form("Start vs. Stop of Acqiris channel %i", fParentChannel->getChannelNbr()+1);
	fHistSvsS = MakeH2<TH2I>(hname, htitle, 500, 0, 500, 500, 0, 500, folder, "Start", "Stop");

	hname.Form("Acq_Tdc_Start_over_Stop_%02i", fParentChannel->getChannelNbr()+1);
	htitle.Form("Start / Stop of Acqiris channel %i", fParentChannel->getChannelNbr()+1);
	fHistSoverS = MakeH1<TH1I>(hname, htitle, 500, 0, 1, folder);
}

/**
 * \brief Fills histograms for TDC performance overview
 */
void TRemiAcqirisSoftTdc::fillOverviewModeSpectra()
{
	for (UInt_t i=0; i<fParentChannel->getNbrPeaks(); i++)
	{
		fHistMax->Fill(fParentChannel->getPeak(i)->getMaximum());
		fHistFWHM->Fill(fParentChannel->getPeak(i)->getFWHM());
		fHistPos->Fill(fParentChannel->getPeak(i)->getPosition());

		fHistFWHMvsMax->Fill(fParentChannel->getPeak(i)->getFWHM(), fParentChannel->getPeak(i)->getMaximum());
		fHistPosVsFWHM->Fill(fParentChannel->getPeak(i)->getPosition(), fParentChannel->getPeak(i)->getFWHM());
		fHistPosVsMax->Fill(fParentChannel->getPeak(i)->getPosition(), fParentChannel->getPeak(i)->getMaximum());

		fHistSvsS->Fill(fParentChannel->getPeak(i)->getStartPos(), fParentChannel->getPeak(i)->getStopPos());
		if ( (fParentChannel->getPeak(i)->getStartPos() != fParentChannel->getPeak(i)->getStopPos()) && (fParentChannel->getPeak(i)->getStopPos() != 0) )
			fHistSoverS->Fill(fParentChannel->getPeak(i)->getStartPos() / Double_t(fParentChannel->getPeak(i)->getStopPos()));
	}
}


ClassImp(TRemiAcqirisSoftTdc);




