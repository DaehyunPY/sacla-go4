#ifndef __TRemiAcqirisArchive_H_
#define __TRemiAcqirisArchive_H_

#include "TRemiAcqirisIO.h"

#include "TString.h"
#include "Riostream.h"
#include "Rtypes.h"

class TRemiAcqirisArchive : public TRemiAcqirisIO
{

	public:
		enum EWriting {ArReading=false,ArWriting=true};
		
		TRemiAcqirisArchive();
		TRemiAcqirisArchive(EWriting w);
		TRemiAcqirisArchive(TString FileName, EWriting w);
		virtual ~TRemiAcqirisArchive()	{if (fFile.is_open()) fFile.close();}

		Int_t getFileSize()	{return mIsWriting ? static_cast<Int_t>(fFile.tellg()):mFileSize;}
		Bool_t EndOfFile()	{return (mFileSize <= fFile.tellg());}
		Bool_t IsWriting()	{return mIsWriting;}
		Bool_t IsReading()	{return !mIsWriting;}
		Bool_t fileIsOpen()	{return fFile.is_open();}
		Bool_t newFile(TString NewFileName);	
		void closeFile();

		virtual TRemiAcqirisArchive & operator << (Char_t c)	{ fFile.write( &c,sizeof(char)  ); return *this;}
		virtual TRemiAcqirisArchive & operator << (Short_t s)	{ fFile.write((char*)&s,sizeof(Short_t) ); return *this;}
		virtual TRemiAcqirisArchive & operator << (Int_t s)	{ fFile.write((char*)&s,sizeof(Int_t) ); return *this;}
		virtual TRemiAcqirisArchive & operator << (Long64_t l)	{ fFile.write((char*)&l,sizeof(Long64_t)  ); return *this;}
		virtual TRemiAcqirisArchive & operator << (Float_t f)	{ fFile.write((char*)&f,sizeof(Float_t)); return *this;}
		virtual TRemiAcqirisArchive & operator << (Double_t d)	{ fFile.write((char*)&d,sizeof(Double_t)); return *this;}
		virtual void writeArray(void * data, Int_t sizeOfData)		{        fFile.write((char*)data,sizeOfData);}
		//void writeArray(const float* const data, Int_t sizeOfData)		{file.write((char*)data,sizeOfData);}

		virtual TRemiAcqirisArchive & operator>>(Char_t &c)	{ fFile.read( &c,sizeof(char)  ); return *this;}
		virtual TRemiAcqirisArchive & operator>>(Short_t &s)	{ fFile.read((char*)&s,sizeof(Short_t) ); return *this;}
		virtual TRemiAcqirisArchive & operator>>(Int_t &s)	{ fFile.read((char*)&s,sizeof(Int_t) ); return *this;}
		virtual TRemiAcqirisArchive & operator>>(Long64_t &l)	{ fFile.read((char*)&l,sizeof(Long64_t)  ); return *this;}
		virtual TRemiAcqirisArchive & operator>>(Float_t &f)	{ fFile.read((char*)&f,sizeof(Float_t)); return *this;}
		virtual TRemiAcqirisArchive & operator>>(Double_t &d)	{ fFile.read((char*)&d,sizeof(Double_t)); return *this;}
		virtual Bool_t readArray(void * data, Int_t sizeOfData)	{ fFile.read((char*)data,sizeOfData); return kTRUE;}


	private:
		std::fstream fFile;	///< the stream to the file
		Bool_t mIsWriting;	///< Boolean describing wether this Archive is reading or not
		Int_t mFileSize;	///< in case of reading then this is the filesize of the file

	ClassDef(TRemiAcqirisArchive, 1);
};

#endif 

