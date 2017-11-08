#ifndef __TRemiAcqirisBuffer_H_
#define __TRemiAcqirisBuffer_H_

#include "TRemiAcqirisIO.h"

#include "TMath.h"
#include "Riostream.h"
#include "Rtypes.h"


class TRemiAcqirisBuffer : public TRemiAcqirisIO
{
	public:
		TRemiAcqirisBuffer():
			buffer(0),bufferCurser(0),
			posAtEndOfBuffer(0),
			isWriting(true),bufferSize(0)		{expand(4*1024);}
		
		TRemiAcqirisBuffer(Char_t* buf, Int_t length):
			buffer(buf),bufferCurser(buf),
			posAtEndOfBuffer(buffer+length),
			isWriting(false),bufferSize(length)		{}

		virtual ~TRemiAcqirisBuffer()			{if(isWriting) delete [] buffer;}

	protected:
		TRemiAcqirisBuffer& operator=(const TRemiAcqirisBuffer& in)	{return *this;}
		TRemiAcqirisBuffer(const TRemiAcqirisBuffer& in)			{}

	public:
		Int_t length()		{return static_cast<Int_t>(bufferCurser - buffer);}
		Bool_t eob()			{return bufferCurser == posAtEndOfBuffer;}
		Char_t* getBuffer()	{writeSize();return buffer;}
		void writeSize()	{Int_t l=length(); memcpy(buffer,&l,sizeof(Int_t));}
		void clear()		{bufferCurser = buffer + sizeof(Int_t);}

		virtual TRemiAcqirisBuffer& operator<<(Char_t c);
		virtual TRemiAcqirisBuffer& operator<<(Short_t s);
		virtual TRemiAcqirisBuffer& operator<<(Int_t s);
		virtual TRemiAcqirisBuffer& operator<<(Long64_t l);
		virtual TRemiAcqirisBuffer& operator<<(UShort_t l);
		virtual TRemiAcqirisBuffer& operator<<(Float_t f);
		virtual TRemiAcqirisBuffer& operator<<(Double_t d);
		virtual TRemiAcqirisBuffer& operator>>(Char_t &c);
		virtual TRemiAcqirisBuffer& operator>>(Short_t &s);
		virtual TRemiAcqirisBuffer& operator>>(Int_t &s);
		virtual TRemiAcqirisBuffer& operator>>(Long64_t &l);
		virtual TRemiAcqirisBuffer& operator>>(UShort_t &l);
		virtual TRemiAcqirisBuffer& operator>>(Float_t &f);
		virtual TRemiAcqirisBuffer& operator>>(Double_t &d);
		virtual void writeArray(void* data, const Int_t lengthInBytes);
		virtual Bool_t readArray(void* data, const Int_t lengthInBytes);

	private:
		void expand(const Int_t newsize);

		Char_t* buffer;
		Char_t* bufferCurser;
		Char_t* posAtEndOfBuffer;
		Bool_t isWriting;
		Int_t bufferSize;
		
	ClassDef(TRemiAcqirisBuffer, 1);
};

//------------------add things to buffer functions------------------
inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator<<(Char_t c)
{
	if(bufferCurser + sizeof(Char_t) > posAtEndOfBuffer) 
		expand(2*bufferSize);
	memcpy(bufferCurser,&c,sizeof(Char_t));
	bufferCurser += sizeof(Char_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator<<(Short_t s)
{
	if(bufferCurser + sizeof(Short_t) > posAtEndOfBuffer) 
		expand(2*bufferSize);
	memcpy(bufferCurser,&s,sizeof(Short_t));
	bufferCurser += sizeof(Short_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator<<(Int_t s)
{
	if(bufferCurser + sizeof(Int_t) > posAtEndOfBuffer) 
		expand(2*bufferSize);
	memcpy(bufferCurser,&s,sizeof(Int_t));
	bufferCurser += sizeof(Int_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator<<(Long64_t l)
{
	if(bufferCurser + sizeof(Long64_t) > posAtEndOfBuffer) 
		expand(2*bufferSize);
	memcpy(bufferCurser,&l,sizeof(Long64_t));
	bufferCurser += sizeof(Long64_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator<<(UShort_t l)
{
	if(bufferCurser + sizeof(UShort_t) > posAtEndOfBuffer) 
		expand(2*bufferSize);
	memcpy(bufferCurser,&l,sizeof(UShort_t));
	bufferCurser += sizeof(UShort_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator<<(Float_t f)
{
	if(bufferCurser + sizeof(Float_t) > posAtEndOfBuffer) 
		expand(2*bufferSize);
	memcpy(bufferCurser,&f,sizeof(Float_t));
	bufferCurser += sizeof(Float_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator<<(Double_t d)
{
	if(bufferCurser + sizeof(Double_t) > posAtEndOfBuffer) 
		expand(2*bufferSize);
	memcpy(bufferCurser,&d,sizeof(Double_t));
	bufferCurser += sizeof(Double_t);
	return *this;
}

inline
void TRemiAcqirisBuffer::writeArray(void* data, const Int_t len)
{
	if (bufferCurser+len > posAtEndOfBuffer)
		expand(TMath::Max(bufferSize+len,2*bufferSize));
	memcpy(bufferCurser,data,len);
	bufferCurser += len;
}


//-------------------read from Buffer functions--------------------------------
inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator>>(Char_t& c)
{
	if(bufferCurser + sizeof(Char_t) > posAtEndOfBuffer) 
	{
		std::cerr<< "Buffer not large enough to read Char_t"<<std::endl;
		return *this;
	}
	memcpy(&c,bufferCurser,sizeof(Char_t));
	bufferCurser += sizeof(Char_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator>>(Short_t& s)
{
	if(bufferCurser + sizeof(Short_t) > posAtEndOfBuffer) 
	{
		std::cerr<< "Buffer not large enough to read Short_t"<<std::endl;
		return *this;
	}
	memcpy(&s,bufferCurser,sizeof(Short_t));
	bufferCurser += sizeof(Short_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator>>(Int_t& s)
{
	if(bufferCurser + sizeof(Int_t) > posAtEndOfBuffer) 
	{
		std::cerr<< "Buffer not large enough to read Int_t"<<std::endl;
		return *this;
	}
	memcpy(&s,bufferCurser,sizeof(Int_t));
	bufferCurser += sizeof(Int_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator>>(UShort_t& l)
{
	if(bufferCurser + sizeof(UShort_t) > posAtEndOfBuffer) 
	{
		std::cerr<< "Buffer not large enough to read UShort_t"<<std::endl;
		return *this;
	}
	memcpy(&l,bufferCurser,sizeof(UShort_t));
	bufferCurser += sizeof(UShort_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator>>(Long64_t& l)
{
	if(bufferCurser + sizeof(Long64_t) > posAtEndOfBuffer) 
	{
		std::cerr<< "Buffer not large enough to read Long64_t"<<std::endl;
		return *this;
	}
	memcpy(&l,bufferCurser,sizeof(Long64_t));
	bufferCurser += sizeof(Long64_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator>>(Float_t& f)
{
	if(bufferCurser + sizeof(Float_t) > posAtEndOfBuffer) 
	{
		std::cerr<< "Buffer not large enough to read Float_t"<<std::endl;
		return *this;
	}
	memcpy(&f,bufferCurser,sizeof(Float_t));
	bufferCurser += sizeof(Float_t);
	return *this;
}

inline
TRemiAcqirisBuffer& TRemiAcqirisBuffer::operator>>(Double_t& d)
{
	if(bufferCurser + sizeof(Double_t) > posAtEndOfBuffer) 
	{
		std::cerr<< "Buffer not large enough to read Double_t"<<std::endl;
		return *this;
	}
	memcpy(&d,bufferCurser,sizeof(Double_t));
	bufferCurser += sizeof(Double_t);
	return *this;
}

inline
Bool_t TRemiAcqirisBuffer::readArray(void* data, const Int_t len)
{
	if (bufferCurser+len > posAtEndOfBuffer)
	{
		std::cerr<< "trying to read to much from Buffer: " << len <<std::endl;
		return true;
	}
	memcpy(data,bufferCurser,len);
	bufferCurser += len;
	return false;
}

#endif 
