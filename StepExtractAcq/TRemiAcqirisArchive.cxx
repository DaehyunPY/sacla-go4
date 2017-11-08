#include "TRemiAcqirisArchive.h"

#include "TRemiAcqirisEvent.h"

/** \brief Default constructor. Not to be used */
TRemiAcqirisArchive::TRemiAcqirisArchive()
{
}


/** \brief Constructor for writing */
TRemiAcqirisArchive::TRemiAcqirisArchive(EWriting w):
mIsWriting(w),
mFileSize(0)
{}

/** \brief Constructor for reading */
TRemiAcqirisArchive::TRemiAcqirisArchive(TString FileName, EWriting w):
mIsWriting(w)	
{
	newFile(FileName);
}


//______________________________________________________________________________________________________________________
Bool_t TRemiAcqirisArchive::newFile(TString NewFileName)
{
	//--first check wether there is a file already open--//
	if (fFile.is_open())
	{
		//if this Archive is writing we need to flush before we close the file
		if(mIsWriting)
		{
			fFile.flush();
			fFile.close();
		}
		//otherwise we just close the file here
		else
		{
			fFile.close();
		}
	}

	//--if this Archive is reading open the file for reading--//
	if (!mIsWriting)
	{	
		fFile.open(NewFileName.Data(),std::ios::in | std::ios::binary);
		//if the file has not been opened give an error message and return false
		if(!fFile.is_open())
		{
			std::cerr << "**** TRemiAcqirisArchive: something went wrong opening the file "<<NewFileName<<std::endl;
			return false;
		}

		//--get the filesize--//
		fFile.seekg(0, std::ios::end);
		mFileSize = fFile.tellg();
		fFile.seekg(0, std::ios::beg);
	}
	
	//--if the Archive is writing open the file for writing--//
	else if (mIsWriting)
	{	
		fFile.open(NewFileName,std::ios::out | std::ios::binary);
		//if the file has not been opened give an error message and return false
		if(!fFile.is_open())
		{
			std::cerr << "something went wrong opening the file "<<NewFileName<<std::endl;
			return false;
		}
	}
	
	//if the file was opened fine return true
	return true;
}
//____________________________________________________________________________________________________________________________________
void TRemiAcqirisArchive::closeFile()
{
	if (fFile.is_open())
	{
		if(mIsWriting)
		{
			fFile.flush();
			fFile.close();
			std::cout << "closing file!" << std::endl;
		}
		//otherwise we just close the file here
		else
		{
			fFile.close();
		}
	}
	else
		std::cout << "tried to close the file but failed!" << std::endl;
}

ClassImp(TRemiAcqirisArchive);
