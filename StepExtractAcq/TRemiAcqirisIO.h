#ifndef __TRemiAcqirisIO_H_
#define __TRemiAcqirisIO_H_

#include "Rtypes.h"


/**
 * \brief Abstract base class for I/O classes such as TRemiAcqirisArchive or TRemiAcqirisBuffer
 */

class TRemiAcqirisIO
{

	public:
		TRemiAcqirisIO()	{}
		virtual ~TRemiAcqirisIO()	{}

//		virtual Bool_t End() = 0;	///< Function returning if end of input is reached
//		virtual Bool_t New(TString NewName) = 0;	///< Open new I/O object
//		virtual void Close() = 0;	///< Close I/O object

		virtual TRemiAcqirisIO & operator << (Char_t c) = 0;
		virtual TRemiAcqirisIO & operator << (Short_t s) = 0;
		virtual TRemiAcqirisIO & operator << (Int_t s) = 0;
		virtual TRemiAcqirisIO & operator << (Long64_t l) = 0;
		virtual TRemiAcqirisIO & operator << (Float_t d) = 0;
		virtual TRemiAcqirisIO & operator << (Double_t d) = 0;
		virtual void writeArray(void * data, Int_t sizeOfData) = 0;

		virtual TRemiAcqirisIO & operator>>(Char_t &c) = 0;
		virtual TRemiAcqirisIO & operator>>(Short_t &s) = 0;
		virtual TRemiAcqirisIO & operator>>(Int_t &s) = 0;
		virtual TRemiAcqirisIO & operator>>(Long64_t &l) = 0;
		virtual TRemiAcqirisIO & operator>>(Float_t &d) = 0;
		virtual TRemiAcqirisIO & operator>>(Double_t &d) = 0;
		virtual Bool_t readArray(void * data, Int_t sizeOfData) = 0;

	ClassDef(TRemiAcqirisIO, 1);
};

#endif 

