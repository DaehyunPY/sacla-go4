#ifndef __TRemiAcqirisSoftAdc_H__
#define __TRemiAcqirisSoftAdc_H__

#include "../Core/TRemiIncludeFunctions.h"

#include "TString.h"

#include <vector>

class TRemiAcqirisPuls;
class TRemiAcqirisChannel;

class ConfigFile;

class TGo4Picture;

class TGraph;
class TH1I;

class TRemiAcqirisSoftAdc : public TRemiIncludeFunctions
{
public:
	TRemiAcqirisSoftAdc();
	TRemiAcqirisSoftAdc(ConfigFile* AcqConf, TRemiAcqirisChannel* ParentChannel);
	virtual ~TRemiAcqirisSoftAdc()		{}
	void runAdc(TRemiAcqirisPuls *Pulse);

	enum eMethod {kNone=0, kIntegral=1, kMaximum=2, kFirstTime=3, kModulusIntegral=4};
	enum eBits {k8Bit=8, k16Bit=16};
// 	enum ePolarity{kNeg=-1, kBad=0, kPos=1};
	
	void fillRawSpectra();
	
	private:
	template<class T> void runAdcTemplate(TRemiAcqirisPuls *Pulse);
	
	template<class T> void Integral(TRemiAcqirisPuls *Pulse);
	template<class T> void Maximum(TRemiAcqirisPuls *Pulse);
	template<class T> void FirstTime(TRemiAcqirisPuls *Pulse);
	template<class T> void ModulusIntegral(TRemiAcqirisPuls *Pulse);
	
	void initWatchMode();
	template<class T> void watchMode(TRemiAcqirisPuls* Pulse);
	
	Int_t mMethod;	///< ADC method for each channel
	
	TH1I*	fHistRawAdc;	///< ADC raw Histogram

	Bool_t mWatchMode;	///< Flag if TDC watching is activated
	std::vector< TGraph* > 	fPulseGraph;	///< Graphs of Pulse traces
	TGo4Picture*	fPicGraphs;	///< Pictures with graphs for each channel
	Int_t mNumPulseGraphs;	///< No. of pulses to show per channel
	Int_t mNumPulseGraphColumns;	///< No. of columns in pulse pictures
	
	TRemiAcqirisChannel* fParentChannel;	///< Channel to which this SoftADC object belongs
	
	ClassDef(TRemiAcqirisSoftAdc, 1);
};


#endif
