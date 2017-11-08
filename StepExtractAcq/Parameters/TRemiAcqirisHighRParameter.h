//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This describes the parameter object for a SearchHighes  channel
//----------------------------------------------------------------
#ifndef SPARACQIRISHIGHR_H
#define SPARACQIRISHIGHR_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for a HighR channel
 */
class TRemiAcqirisHighRParameter : public TGo4Parameter 
{
	
	public:
		TRemiAcqirisHighRParameter() : TGo4Parameter("RemiAcqirisHighRParameter")	{ Init(); }
		TRemiAcqirisHighRParameter(const char* name) : TGo4Parameter(name)	{ Init(); }
		virtual ~TRemiAcqirisHighRParameter()	{}
		
		Float_t Sigma;				///< expected sigma of peak 
		Float_t	Threshold;			///< threshold value in % for selected peaks
		Bool_t	BackgroundRemove;	///< set if removal of background before deconvolution is desired
		Int_t DeconIterations;		///< number of iterations in deconvolution operation
		Bool_t Markov;				///< if true first the source soectrum is replaced by new spectrum using Markov chains
		Float_t	AverWindow;			///< averaging window of searched peaks, only true for Markov method
		
	private:
		/** \brief Setting default values */
		void Init()
		{
			Sigma = 4.;
			Threshold = 20.;
			BackgroundRemove = kTRUE;
			DeconIterations = 3;
			Markov = kFALSE;
			AverWindow = 3;
		}
	
	ClassDef(TRemiAcqirisHighRParameter, 1);
};


#endif //SPAR_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


