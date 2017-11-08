#include "TRemiAcqirisBuffer.h"

#include <cstring>

//______________________________________________________________________________________________________________________
void TRemiAcqirisBuffer::expand(const Int_t newsize)
{
	//--get old length--//
	Int_t oldLength = length();

	//--create array with increased size--//
	Char_t* tmp = new Char_t[newsize];

	//--copy old array to new array--//
	memcpy(tmp,buffer,bufferSize);

	//--delete old array--//
	delete [] buffer;

	//--set things to new stuff--//
	buffer = tmp;
	bufferCurser = buffer + oldLength;
	bufferSize = newsize;
	posAtEndOfBuffer = buffer + bufferSize;
}

ClassImp(TRemiAcqirisBuffer);
