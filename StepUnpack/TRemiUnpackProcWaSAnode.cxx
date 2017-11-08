//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Position-calculating class for Wedge-and-strip detectors
//----------------------------------------------------------------

#include "TRemiUnpackProcWaSAnode.h"

#include "Parameters/TRemiWaSParameter.h"
#include "../EventElement/TRemiExtractEvent.h"

#include "TString.h"
#include "TH1.h"

/**
 * \brief Default constructor. Not to be used!
 */
TRemiUnpackProcWaSAnode::TRemiUnpackProcWaSAnode()
{
}

/**
 * \brief Constructor to directly build an object of this class using configuration file
 */
TRemiUnpackProcWaSAnode::TRemiUnpackProcWaSAnode(ConfigFile* UnpackConf, TString particle) :
	TRemiUnpackProcPos(UnpackConf, particle)
{
	TString fieldname, basename="unp_", hname, htitle, folder, basefolder = "Unpack", cname, ctitle, paraname, hxtitle, hytitle;
	
	//------------------------------------------//
	// Read information from configuration file //
	//------------------------------------------//
	
	basename = basename + particle + "_";
	
	fieldname  = basename + "adc_we";
	mAdcChanWe = UnpackConf->read (fieldname.Data(), 0);
	fieldname  = basename + "adc_st";
	mAdcChanSt = UnpackConf->read (fieldname.Data(), 1);
	fieldname  = basename + "adc_me";
	mAdcChanMe = UnpackConf->read (fieldname.Data(), 2);
	
	//-----------------//
	// Make Parameters //
	//-----------------//
	folder = basefolder + "/" + mShortName;
	paraname = "Par "; paraname = paraname + mShortName + " WaS";
	fParaPos = MakeParam<TRemiWaSParameter>(paraname, folder);
	
	//-------------------//
	// Create histograms //
	//-------------------//
	folder = basefolder + "/" + mShortName + "_sort/det";
	
	hname = "Pat_" + mShortName;
	htitle = "Pattern " + mLongName;
	fPattern = MakeH1<TH1I> (hname, htitle, 8, 0, 8, folder);
}

/**
 * \brief Destructor
 */
TRemiUnpackProcWaSAnode::~TRemiUnpackProcWaSAnode()
{
}

/**
 * \brief calculate positions of current event
 * 
 * As this is not a multi-hit detector, we will always assign the position to the first MCP hit
 */
void TRemiUnpackProcWaSAnode::CalculatePosition()
{
	// Definitions
	Float_t xr, yr, q[3], qsum;
	Int_t pattern;
	Bool_t flag_pos;
	
	Double_t det_px2mm = fParaPos->size_mm / fParaPos->size_raw;
	
	mDetectorHits.clear();
	
	// Create pattern
	pattern=
			(fRawData->fiADC[mAdcChanWe]>0 && fRawData->fiADC[mAdcChanWe]<3840 ? 0x1 : 0) +
			(fRawData->fiADC[mAdcChanSt]>0 && fRawData->fiADC[mAdcChanSt]<3840 ? 0x2 : 0) +
			(fRawData->fiADC[mAdcChanMe]>0 && fRawData->fiADC[mAdcChanMe]<3840 ? 0x4 : 0);
	if( pattern==7 ) flag_pos = kTRUE;
	fPattern->Fill(pattern);
	
	// Calculate position
	if ( flag_pos && fRawData->fiTDC[mTdcChanMCP].size())
	{
		q[mAdcChanWe] = fParaPos->ADCscale[mAdcChanWe] * ((Float_t)fRawData->fiADC[mAdcChanWe] + static_cast<Float_t>(fRandom.Rndm()) - 0.5 );
		q[mAdcChanSt] = fParaPos->ADCscale[mAdcChanSt] * ((Float_t)fRawData->fiADC[mAdcChanSt] + static_cast<Float_t>(fRandom.Rndm()) - 0.5 );
		q[mAdcChanMe] = fParaPos->ADCscale[mAdcChanMe] * ((Float_t)fRawData->fiADC[mAdcChanMe] + static_cast<Float_t>(fRandom.Rndm()) - 0.5 );
		if ( fParaPos->useCR ) CrossTalkComp(q[mAdcChanWe],q[mAdcChanSt],q[mAdcChanMe]);
		qsum = q[mAdcChanWe]+q[mAdcChanSt]+q[mAdcChanMe];
		xr = det_px2mm*q[mAdcChanWe]*fParaPos->scalex/qsum - fParaPos->xo;
		yr = det_px2mm*q[mAdcChanSt]*fParaPos->scaley/qsum - fParaPos->yo;
		
		if ( pow(xr,2)+pow(yr,2)-pow((fParaPos->size_mm-5)/2,2)<0 ) 
		{
			TRemiDetectorHit NewHit = TRemiDetectorHit(CalculateSingleTof(fRawData->fiTDC[mTdcChanMCP][0]), xr, yr);
			NewHit.Rotate(fParaPos->phi);
			mDetectorHits.push_back(NewHit);
		}
	}
}

/**
 * \brief Cross-Talk compensation routine (after RoentDek)
 */
void TRemiUnpackProcWaSAnode::CrossTalkComp(Float_t &wedge, Float_t &strip, Float_t &meander)
{
	Float_t a, b, det, msCS, mwCS, wsCS, norm=1.;
	Float_t nwedge, nstrip, nmeander;
	// get cross-talk strengths
	msCS = fParaPos->ms;
	mwCS = fParaPos->mw;
	wsCS = fParaPos->ws;
	
	a = mwCS + msCS + wsCS;
	b = (mwCS*msCS) + (mwCS*wsCS) + (msCS*wsCS);
	det = 1. - 2.*a + 3.*b;
	if (det!=0) norm = 1./det;
	// rescale charges
	nmeander = norm*(meander*(1.-wsCS-a+b)+wedge*(b-mwCS)+strip*(b-msCS));
	nwedge   = norm*(wedge*(1.-msCS-a+b)+strip*(b-wsCS)+meander*(b-mwCS));
	nstrip   = norm*(strip*(1.-mwCS-a+b)+meander*(b-msCS)+wedge*(b-wsCS));
	// put back to initial variables
	meander	= nmeander;
	strip	= nstrip;
	wedge	= nwedge;
}

ClassImp(TRemiUnpackProcWaSAnode);
