#include "TH2Fsmooth.h"
#include "Riostream.h"

/**
 * \brief Default constructor
 */
TH2Fsmooth::TH2Fsmooth()
 : TH2F()
{
}

/**
 * \brief Copy constructor
 */
TH2Fsmooth::TH2Fsmooth(const TH2Fsmooth &h2f) : TH2F()
{
	((TH2Fsmooth&)h2f).Copy(*this);
}

/**
 * \brief Constructor with x and y equidistant bins
 * \param name Short name of the histogram
 * \param title Long name (description) of the histogram
 * \param nbinsx No. of bins along \c x axis
 * \param xlow Lower boundary of first \c x bin (included)
 * \param xup Upper boundary of last \c x bin (excluded)
 * \param nbinsy No. of bins along \c y axis
 * \param ylow Lower boundary of first \c y bin (included)
 * \param yup Upper boundary of last \c y bin (excluded)
 * \param nsmooth Degree of smoothing: Sets the maximum distance of neighbouring bins to be used for smoothing
 * \param xperiod Use periodic boundary conditions for smoothing in \c x direction?
 * \param yperiod Use periodic boundary conditions for smoothing in \c y direction?
 */
TH2Fsmooth::TH2Fsmooth(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nsmooth, Bool_t xperiod, Bool_t yperiod)
	: TH2F(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup)
{
	SetInternals(nsmooth, xperiod, yperiod);
}

/**
 * \brief Constructor with variable bins in x
 * \param name Short name of the histogram
 * \param title Long name (description) of the histogram
 * \param nbinsx No. of bins along \c x axis
 * \param xbins Array with \p nbinsx <tt>+ 1</tt> elements containing the lower boundaries (included) of all \c x bins and the (excluded) upper boundary of the last bin
 * \param nbinsy No. of bins along \c y axis
 * \param ylow Lower boundary of first \c y bin (included)
 * \param yup Upper boundary of last \c y bin (excluded)
 * \param nsmooth Degree of smoothing: Sets the maximum distance of neighbouring bins to be used for smoothing
 * \param xperiod Use periodic boundary conditions for smoothing in \c x direction?
 * \param yperiod Use periodic boundary conditions for smoothing in \c y direction?
 */
TH2Fsmooth::TH2Fsmooth(const char *name, const char *title, Int_t nbinsx, const Double_t *xbins, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nsmooth, Bool_t xperiod, Bool_t yperiod)
	: TH2F(name, title, nbinsx, xbins, nbinsy, ylow, yup)
{
	SetInternals(nsmooth, xperiod, yperiod);
}

/**
 * \brief Constructor with variable bins in y 
 * \param name Short name of the histogram
 * \param title Long name (description) of the histogram
 * \param nbinsx No. of bins along \c x axis
 * \param xlow Lower boundary of first \c x bin (included)
 * \param xup Upper boundary of last \c x bin (excluded)
 * \param nbinsy No. of bins along \c y axis
 * \param ybins Array with \p nbinsy <tt>+ 1</tt> elements containing the lower boundaries (included) of all \c y bins and the (excluded) upper boundary of the last bin
 * \param nsmooth Degree of smoothing: Sets the maximum distance of neighbouring bins to be used for smoothing
 * \param xperiod Use periodic boundary conditions for smoothing in \c x direction?
 * \param yperiod Use periodic boundary conditions for smoothing in \c y direction?
 */
TH2Fsmooth::TH2Fsmooth(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, const Double_t *ybins, Int_t nsmooth, Bool_t xperiod, Bool_t yperiod)
	: TH2F(name, title, nbinsx, xlow, xup, nbinsy, ybins)
{
	SetInternals(nsmooth, xperiod, yperiod);
}

/**
 * \brief Destructor
 */
TH2Fsmooth::~TH2Fsmooth()
{
}

/**
 * \brief returns the "smoothed" bin content
 */
Double_t TH2Fsmooth::GetBinContent(Int_t bin) const
{
	if (fBuffer) ((TH2C*)this)->BufferEmpty();
	if (bin < 0) bin = 0;
	if (bin >= fNcells) bin = fNcells-1;
	if (!fArray) return 0;
	Double_t result = 0;
	for (Int_t i=-fSmooth; i<=fSmooth; i++)
		for (Int_t j=-fSmooth; j<=fSmooth; j++)
			result += static_cast<Double_t>(fArray[GetXYneighbour(bin, i, j)]);
	return result;
}


/**
 * \brief set internals
 * \param nsmooth New smoothing factor
 * \param xperiod New flag for periodic boundary conditions in \c x
 * \param yperiod New flag for periodic boundary conditions in \c y
 */
void TH2Fsmooth::SetInternals(Int_t nsmooth, Bool_t xperiod, Bool_t yperiod)
{
	fSmooth = nsmooth;
	fXperiod = xperiod;
	fYperiod = yperiod;
}

/**
 * \brief Finds the n-th x-neighbour of a bin
 * \param bin Global bin number of the current bin
 * \param neighbour \c x -distance of the neighbour to bin
 * \return Global bin number of the neighbour
 */
Int_t TH2Fsmooth::GetXneighbour(Int_t bin, Int_t neighbour) const
{
	Int_t x,y,z;
	GetBinXYZ (bin, x, y, z);
	if ( (x==0) || (y==0) || (x==(GetNbinsX()+1)) || (y==(GetNbinsY()+1)) )
		return bin;
	x+=neighbour;
	if (x<1)
	{
		if (fXperiod)
			x+=GetNbinsX();
		else
			x=1;
	}
	else if (x>GetNbinsX())
	{
		if (fXperiod)
			x-=GetNbinsX();
		else
			x=GetNbinsX();
	}
	return GetBin(x,y,z);
}

/**
 * \brief Finds the n-th y-neighbour of a bin
 * \param bin Global bin number of the current bin
 * \param neighbour \c y -distance of the neighbour to bin
 * \return Global bin number of the neighbour
 */
Int_t TH2Fsmooth::GetYneighbour(Int_t bin, Int_t neighbour) const
{
	Int_t x,y,z;
	GetBinXYZ (bin, x, y, z);
	if ( (x==0) || (y==0) || (x==(GetNbinsX()+1)) || (y==(GetNbinsY()+1)) )
		return bin;
	y+=neighbour;
	if (y<1)
	{
		if (fYperiod)
			y+=GetNbinsY();
		else
			y=1;
	}
	else if (y>GetNbinsY())
	{
		if (fYperiod)
			y-=GetNbinsY();
		else
			y=GetNbinsY();
	}
	return GetBin(x,y,z);
}

/**
 * \brief Finds neighbour of a bin
 * \param bin Global bin number of the current bin
 * \param xneighbour \c x -distance of the neighbour to bin
 * \param yneighbour \c y -distance of the neighbour to bin
 * \return Global bin number of the neighbour
 */
Int_t TH2Fsmooth::GetXYneighbour(Int_t bin, Int_t xneighbour, Int_t yneighbour) const
{
	Int_t x,y,z;
	GetBinXYZ (bin, x, y, z);
	if ( (x==0) || (y==0) || (x==(GetNbinsX()+1)) || (y==(GetNbinsY()+1)) ) // Checks if bin is underflow or overflow bin
		return bin;
	
	x+=xneighbour;
	if (x<1)
	{
		if (fXperiod)
			x+=GetNbinsX();
		else
			x=1;
	}
	else if (x>GetNbinsX())
	{
		if (fXperiod)
			x-=GetNbinsX();
		else
			x=GetNbinsX();
	}
	
	y+=yneighbour;
	if (y<1)
	{
		if (fYperiod)
			y+=GetNbinsY();
		else
			y=1;
	}
	else if (y>GetNbinsY())
	{
		if (fYperiod)
			y-=GetNbinsY();
		else
			y=GetNbinsY();
	}
	return GetBin(x,y,z);
}



ClassImp(TH2Fsmooth);
