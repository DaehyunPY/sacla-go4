//
// C++ Interface: TH2Fsmooth
//
// Description: 
//
//
// Author: Arne Senftleben <arne.senftleben@mpi-hd.mpg.de>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TH2FSMOOTH_H
#define TH2FSMOOTH_H

#include "TH2.h"

/**
 * \brief Class providing "smoothed histograms"
 * \author Arne Senftleben <arne.senftleben@mpi-hd.mpg.de>
*/
class TH2Fsmooth : public TH2F
{
public:
	TH2Fsmooth();
	
	TH2Fsmooth(const TH2Fsmooth& h2f);
TH2Fsmooth(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nsmooth=0, Bool_t xperiod=kFALSE, Bool_t yperiod=kFALSE);
	TH2Fsmooth(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, const Double_t* ybins, Int_t nsmooth=0, Bool_t xperiod=kFALSE, Bool_t yperiod=kFALSE);
	TH2Fsmooth(const char* name, const char* title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nsmooth=0, Bool_t xperiod=kFALSE, Bool_t yperiod=kFALSE);
	
	virtual Double_t GetBinContent(Int_t bin) const;

    virtual ~TH2Fsmooth();
	
protected:
	void SetInternals (Int_t nsmooth, Bool_t xperiod, Bool_t yperiod);
	Int_t GetXneighbour (Int_t bin, Int_t neighbour) const;
	Int_t GetYneighbour (Int_t bin, Int_t neighbour) const;
	Int_t GetXYneighbour (Int_t bin, Int_t xneighbour, Int_t yneighbour) const;
	
	Int_t	fSmooth;	///< Smoothing degree
	Bool_t	fXperiod;	///< Use periodic boundary conditions in X ?
	Bool_t	fYperiod;	///< Use periodic boundary conditions in Y ?

ClassDef(TH2Fsmooth, 1);
};

#endif
