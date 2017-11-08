//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Base class for all event processing classes
//		Contains many functions useful in handling of histograms, parameters, conditions and more
//		
//		This very file contains the implementations of template functions
//----------------------------------------------------------------

#ifndef SINCLUDE_INLINE_H
#define SINCLUDE_INLINE_H

#include "TRemiIncludeFunctions.h"

#include "Extern/ConfigFile.h"

#include "Riostream.h"

/**
* \brief Function to make parameter
* \tparam ParameterType has to be extended class of TGo4Parameter
* \param name Internal name of the parameter object in the object browser
* \param folder (optional) Internal path in the object browser were to add this histogram
* \return Pointer to the desired object. If a parameter object satisfying \p ParameterType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new parameter object is created.
* 
* Example how to create a pointer to a parameter object of class \c TMyParameter using this function:
* \code
* myParameter = MakeParam<TMyParameter> ("myname", "myfolder");
* \endcode
* 
* This had to be implemented inline, because it is a template function.
*/
template <class ParameterType> 
		ParameterType* TRemiIncludeFunctions::MakeParam(TString name, TString folder) const
{
	ParameterType* parameter = dynamic_cast<ParameterType*> (TGo4Analysis::Instance()->GetParameter(name));
	if (!parameter)
	{
		parameter = new ParameterType(name);
		if (folder.Length()!=0)
			TGo4Analysis::Instance()->AddParameter(parameter, folder);
		else
			TGo4Analysis::Instance()->AddParameter(parameter);
	}
	return parameter;
}

/**
	* \brief Function to make general one-dimensional histograms with equidistant bins
	* \tparam HistoType has to be extended class of TH1
	* \param name Internal name of the histogram in the object browser
	* \param title Long title for the histogram
	* \param nbinsx No. of bins to create 
	* \param xlow Lower boundary of first bin (included)
	* \param xup Upper boundary of last bin (excluded)
	* \param folder (optional) Internal path in the object browser were to add this histogram
	* \param xtitle (optional) Title for the X axis
	* \param ytitle (optional) Title for the Y axis
	* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
	* 
	* This function only creates histograms with \p nbinsx equidistant bins between \p xlow and \p xup.
	* 
	* Example how to create a pointer to a histogram of class \c TH1I using this function:
	* \code
	* myHistogram = MakeH1<TH1I> ("myname", "mytitle", 10, 0, 10, "myfolder", "xaxis");
	* \endcode
	* This had to be implemented inline, because it is a template function.
	*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeH1 (TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder, TString xtitle, TString ytitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, xlow, xup)) )
		 return histo;
	if (title.Length() != 0) 
		histo = new HistoType(name, title, nbinsx, xlow, xup);
	else 
		histo = new HistoType(name, TString(name) + " histogram", nbinsx, xlow, xup);
	AddHistogram (histo, folder, xtitle, ytitle);
	return histo;
}

/**
* \brief Function to make general one-dimensional histograms with equidistant bins using input from a config file
* \tparam HistoType has to be extended class of TH1
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx name of ConfigFile field that contains No. of bins to create 
* \param field_xlow name of ConfigFile field that contains Lower boundary of first bin (included)
* \param field_xup name of ConfigFile field that contains Upper boundary of last bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* This function only creates histograms with \p nbinsx equidistant bins between \p xlow and \p xup.
* 
* Example how to create a pointer to a histogram of class \c TH1I using this function:
* \code
* myHistogram = MakeH1<TH1I> ("myname", "mytitle", 10, 0, 10, "myfolder", "xaxis");
* \endcode
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeH1 (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder, TString xtitle, TString ytitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, xlow, xup)) )
		return histo;
	if (title.Length() != 0) 
		histo = new HistoType(name, title, nbinsx, xlow, xup);
	else 
		histo = new HistoType(name, TString(name) + " histogram", nbinsx, xlow, xup);
	AddHistogram (histo, folder, xtitle, ytitle);
	return histo;
}

/**
* \brief Function to make general one-dimensional histograms with variable bin sizes
* \tparam HistoType has to be extended class of TH1
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins to create 
* \param borders Array with \p nbinsx <tt>+ 1</tt> elements containing the lower boundaries (included) of all bins and the (excluded) upper boundary of the last bin
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeH1 (TString name, TString title, Int_t nbinsx, Double_t* borders, TString folder, TString xtitle, TString ytitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, borders[0], borders[nbinsx])) )
		return histo;
	if (title.Length() != 0) 
		histo = new HistoType(name, title, nbinsx, borders);
	else 
		histo = new HistoType(name, TString(name) + " histogram", nbinsx, borders);
	AddHistogram (histo, folder, xtitle, ytitle);
	return histo;
}

/**
* \brief Function to make one-dimensional histograms with sinusoidal binning
* \tparam HistoType has to be extended class of TH1
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins to create 
* \param xlow Lower boundary of first bin in degrees (included) 
* \param xup Upper boundary of last bin in degrees (excluded) 
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* Filling this histogram with the distribution \f$\sin(x)\f$ will look like a uniform distribution. This is especially useful if you want to plot the \f$\theta\f$-angle of a vector in spherical coordinates with collecting equal parts of the solid angle into each bin.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeSinusoidalH1 (TString name, TString title, Int_t nbinsx, Float_t xlow, Float_t xup, TString folder, TString xtitle, TString ytitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, xlow, xup)) )
		return histo;
	Double_t borders[nbinsx+1];
	if (CreateSinusoidalBins(borders, nbinsx, xlow, xup) == kFALSE)
		return 0;
	if (title.Length() != 0) 
		histo = new HistoType(name, title, nbinsx, borders);
	else 
		histo = new HistoType(name, TString(name) + " histogram", nbinsx, borders);
	AddHistogram (histo, folder, xtitle, ytitle);
	return histo;
}

/**
* \brief Function to make one-dimensional histograms with sinusoidal binning using input from a config file
* \tparam HistoType has to be extended class of TH1
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx name of ConfigFile field that contains No. of bins to create 
* \param field_xlow name of ConfigFile field that contains Lower boundary of first bin in degrees (included)
* \param field_xup name of ConfigFile field that contains Upper boundary of last bin in degrees (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* Filling this histogram with the distribution \f$\sin(x)\f$ will look like a uniform distribution. This is especially useful if you want to plot the \f$\theta\f$-angle of a vector in spherical coordinates with collecting equal parts of the solid angle into each bin.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeSinusoidalH1 (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder, TString xtitle, TString ytitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, xlow, xup)) )
		return histo;
	Double_t borders[nbinsx+1];
	if (CreateSinusoidalBins(borders, nbinsx, xlow, xup) == kFALSE)
		return 0;
	if (title.Length() != 0) 
		histo = new HistoType(name, title, nbinsx, borders);
	else 
		histo = new HistoType(name, TString(name) + " histogram", nbinsx, borders);
	AddHistogram (histo, folder, xtitle, ytitle);
	return histo;
}

/**
* \brief Function to make general two-dimensional histograms with equidistant bins
* \tparam HistoType has to be extended class of TH2
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
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* This function only creates histograms with equidistant bins both in \c x and \c y direction.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeH2 (TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder, TString xtitle, TString ytitle, TString ztitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup)) )
		return histo;
	if (title.Length() != 0) 
		histo = new HistoType (name, title, nbinsx, xlow, xup, nbinsy, ylow, yup);
	else 
		histo = new HistoType (name, TString(name) + " histogram", nbinsx, xlow, xup, nbinsy, ylow, yup);
	AddHistogram (histo, folder, xtitle, ytitle, ztitle);
	return histo;
}

/**
* \brief Function to make general two-dimensional histograms with equidistant bins using input from input file
* \tparam HistoType has to be extended class of TH2
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
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* This function only creates histograms with equidistant bins both in \c x and \c y direction.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeH2 (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder, TString xtitle, TString ytitle, TString ztitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	Int_t nbinsy = ReadFromConfigFile<Int_t>(field_nbinsy, DEFAULT_NBINS);
	Double_t ylow = ReadFromConfigFile<Double_t>(field_ylow, DEFAULT_LOWER);
	Double_t yup = ReadFromConfigFile<Double_t>(field_yup, DEFAULT_UPPER);
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup)) )
		return histo;
	if (title.Length() != 0) 
		histo = new HistoType (name, title, nbinsx, xlow, xup, nbinsy, ylow, yup);
	else 
		histo = new HistoType (name, TString(name) + " histogram", nbinsx, xlow, xup, nbinsy, ylow, yup);
	AddHistogram (histo, folder, xtitle, ytitle, ztitle);
	return histo;
}

/**
* \brief Function to make two-dimensional histograms with spherical binning
* \tparam HistoType has to be extended class of TH2
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
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* This function was made to create spherical histograms depending on the two angles \f$ \theta \f$ (the y-axis of the histogram) and \f$ \phi \f$ (the x-axis of the histogram). Filling this histogram with isotropically distributed points on the unit sphere will create a uniform distribution.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeSphericalH2 (TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder, TString xtitle, TString ytitle, TString ztitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup)) )
		return histo;
	Double_t borders[nbinsy+1];
	if (CreateSinusoidalBins(borders, nbinsy, ylow, yup) == kFALSE)
		return 0;
	if (title.Length() != 0) 
		histo = new HistoType (name, title, nbinsx, xlow, xup, nbinsy, borders);
	else 
		histo = new HistoType (name, TString(name) + " histogram", nbinsx, xlow, xup, nbinsy, borders);
	if (xtitle.Length() == 0)
		xtitle = "Phi";
	if (ytitle.Length() == 0)
		ytitle = "Theta";
	AddHistogram (histo, folder, xtitle, ytitle, ztitle);
	return histo;
}

/**
* \brief Function to make two-dimensional histograms with spherical binning using input from Config File
* \tparam HistoType has to be extended class of TH2
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
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* This function was made to create spherical histograms depending on the two angles \f$ \theta \f$ (the y-axis of the histogram) and \f$ \phi \f$ (the x-axis of the histogram). Filling this histogram with isotropically distributed points on the unit sphere will create a uniform distribution.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeSphericalH2 (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder, TString xtitle, TString ytitle, TString ztitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	Int_t nbinsy = ReadFromConfigFile<Int_t>(field_nbinsy, DEFAULT_NBINS);
	Double_t ylow = ReadFromConfigFile<Double_t>(field_ylow, DEFAULT_LOWER);
	Double_t yup = ReadFromConfigFile<Double_t>(field_yup, DEFAULT_UPPER);
	Double_t borders[nbinsy+1];
	if (CreateSinusoidalBins(borders, nbinsy, ylow, yup) == kFALSE)
		return 0;
	if (title.Length() != 0) 
		histo = new HistoType (name, title, nbinsx, xlow, xup, nbinsy, borders);
	else 
		histo = new HistoType (name, TString(name) + " histogram", nbinsx, xlow, xup, nbinsy, borders);
	if (xtitle.Length() == 0)
		xtitle = "Phi";
	if (ytitle.Length() == 0)
		ytitle = "Theta";
	AddHistogram (histo, folder, xtitle, ytitle, ztitle);
	return histo;
}

/**
* \brief Function to make two-dimensional smoothed histograms with spherical binning
* \tparam HistoType has to be extended class of TH2 that supports smoothing, at the moment only TH2Fsmooth exists
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of azimuthal bins
* \param xlow Lower boundary of first azimuthal bin (included)
* \param xup Upper boundary of last azimuthal bin (excluded)
* \param nbinsy No. of polar bins
* \param ylow Lower boundary of first polar bin in degrees (included) Minimum: 0
* \param yup Upper boundary of last polar bin in degrees (excluded) Maximum: 180
* \param nsmooth (optional) Degree of smoothing
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* This function was made to create spherical histograms depending on the two angles \f$ \theta \f$ (the y-axis of the histogram) and \f$ \phi \f$ (the x-axis of the histogram). Filling this histogram with isotropically distributed points on the unit sphere will create a uniform distribution.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeSphericalH2smooth (TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nsmooth, TString folder, TString xtitle, TString ytitle, TString ztitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup)) )
		return histo;
	Double_t borders[nbinsy+1];
	if (CreateSinusoidalBins(borders, nbinsy, ylow, yup) == kFALSE)
		return 0;
	if (title.Length() != 0) 
		histo = new HistoType (name, title, nbinsx, xlow, xup, nbinsy, borders, nsmooth, kFALSE, kTRUE);
	else 
		histo = new HistoType (name, TString(name) + " histogram", nbinsx, xlow, xup, nbinsy, borders, nsmooth, kFALSE, kTRUE);
	if (histo)
	{
		if (xtitle.Length() == 0)
			xtitle = "Phi";
		if (ytitle.Length() == 0)
			ytitle = "Theta";
		AddHistogram (dynamic_cast<TH1*>(histo), folder, xtitle, ytitle, ztitle);
		return histo;
	}
	else return 0;
}

/**
* \brief Function to make two-dimensional smoothed histograms with spherical binning using input from Config File
* \tparam HistoType has to be extended class of TH2 that supports smoothing, at the moment only TH2Fsmooth exists
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx ConfigFile field that contains No. of azimuthal bins
* \param field_xlow ConfigFile field that contains Lower boundary of first azimuthal bin (included)
* \param field_xup ConfigFile field that contains Upper boundary of last azimuthal bin (excluded)
* \param field_nbinsy ConfigFile field that contains no. of polar bins
* \param field_ylow ConfigFile field that contains Lower boundary of first polar bin in degrees (included) Minimum: 0
* \param field_yup ConfigFile field that contains Upper boundary of last polar bin in degrees (excluded) Maximum: 180
* \param nsmooth (optional) Degree of smoothing
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \param ztitle (optional) Title for the Z axis
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* This function was made to create spherical histograms depending on the two angles \f$ \theta \f$ (the y-axis of the histogram) and \f$ \phi \f$ (the x-axis of the histogram). Filling this histogram with isotropically distributed points on the unit sphere will create a uniform distribution.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeSphericalH2smooth (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, Int_t nsmooth, TString folder, TString xtitle, TString ytitle, TString ztitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	Int_t nbinsy = ReadFromConfigFile<Int_t>(field_nbinsy, DEFAULT_NBINS);
	Double_t ylow = ReadFromConfigFile<Double_t>(field_ylow, DEFAULT_LOWER);
	Double_t yup = ReadFromConfigFile<Double_t>(field_yup, DEFAULT_UPPER);
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup)) )
		return histo;
	Double_t borders[nbinsy+1];
	if (CreateSinusoidalBins(borders, nbinsy, ylow, yup) == kFALSE)
		return 0;
	if (title.Length() != 0) 
		histo = new HistoType (name, title, nbinsx, xlow, xup, nbinsy, borders, nsmooth, kFALSE, kTRUE);
	else 
		histo = new HistoType (name, TString(name) + " histogram", nbinsx, xlow, xup, nbinsy, borders, nsmooth, kFALSE, kTRUE);
	if (histo)
	{
		if (xtitle.Length() == 0)
			xtitle = "Phi";
		if (ytitle.Length() == 0)
			ytitle = "Theta";
		AddHistogram (dynamic_cast<TH1*>(histo), folder, xtitle, ytitle, ztitle);
		return histo;
	}
	else return 0;
}

/**
* \brief Function to make general three-dimensional histograms with equidistant bins
* \tparam HistoType has to be extended class of TH3
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
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* This function only creates histograms with equidistant bins both in \c x, \c y, and \c z direction.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeH3 (TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup, TString folder, TString xtitle, TString ytitle, TString ztitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup)) )
		return histo;
	if (title.Length() != 0) 
		histo = new HistoType (name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup);
	else 
		histo = new HistoType (name, TString(name) + " histogram", nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup);
	AddHistogram (histo, folder, xtitle, ytitle, ztitle);
	return histo;
}

/**
* \brief Function to make general three-dimensional histograms with equidistant bins using input from input file
* \tparam HistoType has to be extended class of TH3
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
* \return Pointer to the desired object. If a histogram satisfying \p HistoType, \p name  and \p folder is found in the object tree, a pointer to this object will be returned. If no matching object is found, a new histogram according to the given parameters is created.
* 
* This function only creates histograms with equidistant bins both in \c x, \c y, and \c z direction.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::MakeH3 (TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString field_nbinsz, TString field_zlow, TString field_zup, TString folder, TString xtitle, TString ytitle, TString ztitle) const
{
	HistoType* histo = dynamic_cast<HistoType*> (TGo4Analysis::Instance()->GetHistogram(name));
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	Int_t nbinsy = ReadFromConfigFile<Int_t>(field_nbinsy, DEFAULT_NBINS);
	Double_t ylow = ReadFromConfigFile<Double_t>(field_ylow, DEFAULT_LOWER);
	Double_t yup = ReadFromConfigFile<Double_t>(field_yup, DEFAULT_UPPER);
	Int_t nbinsz = ReadFromConfigFile<Int_t>(field_nbinsz, DEFAULT_NBINS);
	Double_t zlow = ReadFromConfigFile<Double_t>(field_zlow, DEFAULT_LOWER);
	Double_t zup = ReadFromConfigFile<Double_t>(field_zup, DEFAULT_UPPER);
	if ( (histo) && (CheckUnchangedHist(histo, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup)) )
		return histo;
	if (title.Length() != 0) 
		histo = new HistoType (name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup);
	else 
		histo = new HistoType (name, TString(name) + " histogram", nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup);
	AddHistogram (histo, folder, xtitle, ytitle, ztitle);
	return histo;
}


// Private functions

/**
* \brief Function to create general one-dimensional histograms with equidistant bins for internal use in the Fill & Go functions
* \tparam HistoType has to be extended class of TH1
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins to create 
* \param xlow Lower boundary of first bin (included)
* \param xup Upper boundary of last bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \return Pointer to the desired object. 
* 
* NOTE: This function cannot be called directly in any Event processing class. Use FillTo1DHist() or FillToWeighted1DHist() instead.
* 
* This function only creates histograms with \p nbinsx equidistant bins between \p xlow and \p xup.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::CreateH1(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder, TString xtitle, TString ytitle)
{
	HistoType* histo;
	if (title.Length() != 0) 
		histo = new HistoType(name, title, nbinsx, xlow, xup);
	else 
		histo = new HistoType(name, TString(name) + " histogram", nbinsx, xlow, xup);
	AddHistogram (histo, folder, xtitle, ytitle);
	fHistoStore.push_back(histo);
	return histo;
}

/**
* \brief Function to create general one-dimensional histograms with equidistant bins for internal use in the Fill & Go functions
* \tparam HistoType has to be extended class of TH1
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx name of ConfigFile field that contains No. of bins to create 
* \param field_xlow name of ConfigFile field that contains Lower boundary of first bin in degrees (included)
* \param field_xup name of ConfigFile field that contains Upper boundary of last bin in degrees (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \return Pointer to the desired object. 
* 
* NOTE: This function cannot be called directly in any Event processing class. Use FillTo1DHist() or FillToWeighted1DHist() instead.
* 
* This function only creates histograms with \p nbinsx equidistant bins between \p xlow and \p xup.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::CreateH1(TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder, TString xtitle, TString ytitle)
{
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	return CreateH1<HistoType>(name, title, nbinsx, xlow, xup, folder, xtitle, ytitle);
}

/**
* \brief Function to create general one-dimensional histograms with sinusoidal bins for internal use in the Fill & Go functions
* \tparam HistoType has to be extended class of TH1
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param nbinsx No. of bins to create 
* \param xlow Lower boundary of first bin (included)
* \param xup Upper boundary of last bin (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \return Pointer to the desired object. 
* 
* NOTE: This function cannot be called directly in any Event processing class. Use FillToSinusoidal1DHist() or FillToWeightedSinusoidal1DHist() instead.
* 
* This function only creates histograms with \p nbinsx equidistant bins between \p xlow and \p xup.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::CreateSinusoidalH1(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString folder, TString xtitle, TString ytitle)
{
	HistoType* histo;
	Double_t borders[nbinsx+1];
	if (CreateSinusoidalBins(borders, nbinsx, xlow, xup) == kFALSE)
		return 0;
	if (title.Length() != 0) 
		histo = new HistoType(name, title, nbinsx, borders);
	else 
		histo = new HistoType(name, TString(name) + " histogram", nbinsx, borders);
	AddHistogram (histo, folder, xtitle, ytitle);
	fHistoStore.push_back(histo);
	return histo;
}

/**
* \brief Function to create general one-dimensional histograms with sinusoidal bins for internal use in the Fill & Go functions
* \tparam HistoType has to be extended class of TH1
* \param name Internal name of the histogram in the object browser
* \param title Long title for the histogram
* \param field_nbinsx name of ConfigFile field that contains No. of bins to create 
* \param field_xlow name of ConfigFile field that contains Lower boundary of first bin in degrees (included)
* \param field_xup name of ConfigFile field that contains Upper boundary of last bin in degrees (excluded)
* \param folder (optional) Internal path in the object browser were to add this histogram
* \param xtitle (optional) Title for the X axis
* \param ytitle (optional) Title for the Y axis
* \return Pointer to the desired object. 
* 
* NOTE: This function cannot be called directly in any Event processing class. Use FillToSinusoidal1DHist() or FillToWeightedSinusoidal1DHist() instead.
* 
* This function only creates histograms with \p nbinsx equidistant bins between \p xlow and \p xup.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::CreateSinusoidalH1(TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString folder, TString xtitle, TString ytitle)
{
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	return CreateH1<HistoType>(name, title, nbinsx, xlow, xup, folder, xtitle, ytitle);
}

/**
* \brief Function to create general two-dimensional histograms with equidistant bins for internal use in the Fill & Go functions
* \tparam HistoType has to be extended class of TH2
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
* \return Pointer to the desired object. 
* 
* NOTE: This function cannot be called directly in any Event processing class. Use FillTo2DHist() or FillToWeighted2DHist() instead.
* 
* This function only creates histograms with equidistant bins both in \c x and \c y direction.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::CreateH2(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	HistoType* histo;
	if (title.Length() != 0) 
		histo = new HistoType(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup);
	else 
		histo = new HistoType(name, TString(name) + " histogram", nbinsx, xlow, xup, nbinsy, ylow, yup);
	AddHistogram (histo, folder, xtitle, ytitle, ztitle);
	fHistoStore.push_back(histo);
	return histo;
}

/**
* \brief Function to create general two-dimensional histograms with equidistant bins for internal use in the Fill & Go functions
* \tparam HistoType has to be extended class of TH2
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
* \return Pointer to the desired object. 
* 
* NOTE: This function cannot be called directly in any Event processing class. Use FillTo2DHist() or FillToWeighted2DHist() instead.
* 
* This function only creates histograms with equidistant bins both in \c x and \c y direction.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::CreateH2(TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	Int_t nbinsy = ReadFromConfigFile<Int_t>(field_nbinsy, DEFAULT_NBINS);
	Double_t ylow = ReadFromConfigFile<Double_t>(field_ylow, DEFAULT_LOWER);
	Double_t yup = ReadFromConfigFile<Double_t>(field_yup, DEFAULT_UPPER);
	return CreateH2<HistoType>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, folder, xtitle, ytitle, ztitle);
}

/**
* \brief Function to create general two-dimensional histograms with spherical bins for internal use in the Fill & Go functions
* \tparam HistoType has to be extended class of TH2
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
* \return Pointer to the desired object. 
* 
* NOTE: This function cannot be called directly in any Event processing class. Use FillToSpherical2DHist() or FillToWeightedSpherical2DHist() instead.
* 
* This function only creates histograms with equidistant bins both in \c x and \c y direction.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::CreateSphericalH2(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	HistoType* histo;
	Double_t borders[nbinsy+1];
	if (CreateSinusoidalBins(borders, nbinsy, ylow, yup) == kFALSE)
		return 0;
	if (title.Length() != 0) 
		histo = new HistoType (name, title, nbinsx, xlow, xup, nbinsy, borders);
	else 
		histo = new HistoType (name, TString(name) + " histogram", nbinsx, xlow, xup, nbinsy, borders);
	if (xtitle.Length() == 0)
		xtitle = "Phi";
	if (ytitle.Length() == 0)
		ytitle = "Theta";
	AddHistogram (histo, folder, xtitle, ytitle, ztitle);
	fHistoStore.push_back(histo);
	return histo;
}

/**
* \brief Function to create general two-dimensional histograms with spherical bins for internal use in the Fill & Go functions
* \tparam HistoType has to be extended class of TH2
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
* \return Pointer to the desired object. 
* 
* NOTE: This function cannot be called directly in any Event processing class. Use FillToSpherical2DHist() or FillToWeightedSpherical2DHist() instead.
* 
* This function only creates histograms with equidistant bins both in \c x and \c y direction.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::CreateSphericalH2(TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	Int_t nbinsx = ReadFromConfigFile<Int_t>(field_nbinsx, DEFAULT_NBINS);
	Double_t xlow = ReadFromConfigFile<Double_t>(field_xlow, DEFAULT_LOWER);
	Double_t xup = ReadFromConfigFile<Double_t>(field_xup, DEFAULT_UPPER);
	Int_t nbinsy = ReadFromConfigFile<Int_t>(field_nbinsy, DEFAULT_NBINS);
	Double_t ylow = ReadFromConfigFile<Double_t>(field_ylow, DEFAULT_LOWER);
	Double_t yup = ReadFromConfigFile<Double_t>(field_yup, DEFAULT_UPPER);
	return CreateSphericalH2<HistoType>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, folder, xtitle, ytitle, ztitle);
}

/**
* \brief Function to create general three-dimensional histograms with equidistant bins for internal use in the Fill & Go functions
* \tparam HistoType has to be extended class of TH3
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
* \return Pointer to the desired object. 
* 
* NOTE: This function cannot be called directly in any Event processing class. Use FillTo3DHist() or FillToWeighted3DHist() instead.
* 
* This function only creates histograms with equidistant bins both in \c x, \c y, and \c z direction.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::CreateH3(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup, TString folder, TString xtitle, TString ytitle, TString ztitle)
{
	HistoType* histo;
	if (title.Length() != 0) 
		histo = new HistoType(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup);
	else 
		histo = new HistoType(name, TString(name) + " histogram", nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup);
	AddHistogram (histo, folder, xtitle, ytitle, ztitle);
	fHistoStore.push_back(histo);
	return histo;
}

/**
* \brief Function to create general three-dimensional histograms with equidistant bins for internal use in the Fill & Go functions
* \tparam HistoType has to be extended class of TH3
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
* \return Pointer to the desired object. 
* 
* NOTE: This function cannot be called directly in any Event processing class. Use FillTo3DHist() or FillToWeighted3DHist() instead.
* 
* This function only creates histograms with equidistant bins both in \c x, \c y, and \c z direction.
* 
* This had to be implemented inline, because it is a template function.
*/
template <class HistoType>
HistoType* TRemiIncludeFunctions::CreateH3(TString name, TString title, TString field_nbinsx, TString field_xlow, TString field_xup, TString field_nbinsy, TString field_ylow, TString field_yup, TString field_nbinsz, TString field_zlow, TString field_zup, TString folder, TString xtitle, TString ytitle, TString ztitle)
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
	return CreateH3<HistoType>(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup, nbinsz, zlow, zup, folder, xtitle, ytitle, ztitle);
}

/** 
* \brief Reads a field value from current ConfigFile and checks if this is initialised
* \tparam Type Datatype of field value
* \param field Field name
* \param Default Default value
* This had to be implemented inline, because it is a template function.
*/
template <class Type>
Type TRemiIncludeFunctions::ReadFromConfigFile(TString field, Type Default) const
{
	Type value;
	if (fConfigFile)
		value = fConfigFile->read<Type>(field.Data(), Default);
	else
	{
		std::cout << __FILE__ << ": Not initialised for internal Config file use" << std::endl;
		value = Default;
	}
	return value;
}


#endif
