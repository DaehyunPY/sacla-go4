//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This exception class should be thrown when an illogical
//		(negative or huge) length is read from the data stream
//----------------------------------------------------------------

#ifndef TREMIACQIRISDATASTREAMERROR_H
#define TREMIACQIRISDATASTREAMERROR_H


#include <stdexcept>
#include <TString.h>


#define MAX_ARRAY_LENGTH 1e6


class TRemiAcqirisDataStreamError : public std::runtime_error
{
	public:
		TRemiAcqirisDataStreamError(Int_t Length=0, TString Name="length", TString Msg="TRemiAcqirisDataStreamError", TString File=__FILE__, Int_t Line=__LINE__) :
			std::runtime_error( TString::Format("Length error (%s == %i) in %s() at %s:%i", Name.Data(), Length, Msg.Data(), File.Data(), Line).Data() ) {}

	ClassDef(TRemiAcqirisDataStreamError, 1);
};


#endif //TREMIACQIRISDATASTREAMERROR_H

