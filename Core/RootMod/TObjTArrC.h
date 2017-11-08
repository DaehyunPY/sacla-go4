#ifndef TOBJTARRC_H
#define TOBJTARRC_H


#include "TObject.h"
#include "TArrayC.h"


class TObjTArrC : public TObject, public TArrayC
{
public:
	// constructors for TObject default constructor

	TObjTArrC() : 
		TObject(), TArrayC()
	{}

	TObjTArrC(Int_t n) : 
		TObject(), TArrayC(n)
	{}

	TObjTArrC(Int_t n, const Char_t* array) : 
		TObject(), TArrayC(n, array)
	{}

	TObjTArrC(const TArrayC& array) : 
		TObject(), TArrayC(array)
	{}


	// constructors for TObject copy constructor

	TObjTArrC(const TObject& object) : 
		TObject(object), TArrayC()
	{}

	TObjTArrC(const TObject& object, Int_t n) : 
		TObject(object), TArrayC(n)
	{}

	TObjTArrC(const TObject& object, Int_t n, const Char_t* array) : 
		TObject(object), TArrayC(n, array)
	{}

	TObjTArrC(const TObject& object, const TArrayC& array) : 
		TObject(object), TArrayC(array)
	{}

ClassDef(TObjTArrC, 1); //TObjTArrC
};


/*
// alternative implementation
class TObjTArrC : public TObject
{
public:
	TObjTArrC() : 
		fArray(new TArrayC())
	{}

	TObjTArrC(Int_t n) : 
		fArray(new TArrayC(n))
	{}

	TObjTArrC(Int_t n, const Char_t * a) : 
		fArray(new TArrayC(n, a))
	{}

	TObjTArrC(const TArrayC & a) : 
		fArray(new TArrayC(a))
	{}

	virtual ~TObjTArrC() { delete fArray; }

	Char_t * GetArray() { return fArray->GetArray(); }
	Int_t GetSize() { return fArray->GetSize(); }

private:
	TArrayC * fArray;

ClassDef(TObjTArrC, 1); //TObjTArrC
};
*/


#endif

