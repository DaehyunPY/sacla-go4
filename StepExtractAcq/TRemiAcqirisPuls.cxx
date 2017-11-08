
#include "TRemiAcqirisPuls.h"

#include "TRemiAcqirisDataStreamError.h"
#include "TRemiAcqirisPeak.h"
#include "TRemiAcqirisChannel.h"
#include "TRemiAcqirisArchive.h"
#include "TRemiAcqirisEvent.h"

#include "Riostream.h"

TRemiAcqirisPuls::TRemiAcqirisPuls(const TRemiAcqirisPuls& in)
{
	fParentChannel = in.getParentChannel();
	mDataSize = in.getLength();
	mData = new Char_t[mDataSize];
	for (UInt_t i=0; i<mDataSize; i++)
		mData[i] = in.getData()[i];
	for (UInt_t i=0; i<in.getNbrPeaks(); i++)
		fPeaks.push_back(in.getPeak(i));
	mIndex = in.mIndex;
	mAdcValue = in.mAdcValue;
}


//______________________________________________________________________________________________________________________
TRemiAcqirisPuls::~TRemiAcqirisPuls()
{
	delete [] mData; 
}

//______________________________________________________________________________________________________________________
/** 
* \brief Clears vector of peaks 
* 
* Contents of peaks are not deleted, as they are to be kept by the owning channel
*/
void TRemiAcqirisPuls::clear()
{
	//clear the peaks vector//
	fPeaks.clear();
}


//______________________________________________________________________________________________________________________
Bool_t TRemiAcqirisPuls::readPuls(TRemiAcqirisIO* input)
{
	Int_t Tempstamp;
	//--read the puls properties from archive--//
	*input >> Tempstamp;
	*input >> mLength;
	//std::cerr<<"length:"<<length<<std::endl

	if ((mLength <= 0) || (MAX_ARRAY_LENGTH < mLength))
	{
		throw TRemiAcqirisDataStreamError(mLength, "mLength", __func__, __FILE__, __LINE__);
	}

	// convert timestamp to ns and correct by horpos
	mTimestamp = static_cast<Double_t>(Tempstamp) * fParentChannel->getSampDistNs() + fParentChannel->getParentEvent()->getHorpos()*1e9;

	//--calculate the size we need for the given length of the puls--//
	size_t newSize = mLength*(fParentChannel->getParentEvent()->getNbrBits()/8);
	//std::cerr<<"newsize:"<<newSize<<std::endl;
	//--if the length of the waveform requires a bigger size we have to increase the size of the buffer

	if(newSize > mDataSize)
	{
		//std::cerr << "RESIZE DataBuffer: " << newSize+5024 <<"> " <<dataSize<<std::endl;
		delete [] mData;
		mData = new Char_t[newSize+1000];
		mDataSize = newSize+1000;
	}
	//--read the waveform into the buffer given by data--//
	input->readArray(mData, newSize); 
	
	return 1;
}

/**
* \brief Adds a Peak to this Pulse
*
* Peak is actually added and allocated by the channel and not by the Puls istseld
*/
TRemiAcqirisPeak* TRemiAcqirisPuls::addPeak()
{
	fPeaks.push_back(fParentChannel->addPeak());

	//--return the added Peak--//
	return fPeaks.back();
}
//______________________________________________________________________________________________________________________
void TRemiAcqirisPuls::writePulsToFile(TRemiAcqirisArchive &ar)
{
	ar << (Int_t)mTimestamp
		<< (Int_t)mLength;
	size_t newSize = mLength*(fParentChannel->getParentEvent()->getNbrBits()/8);
	
	ar.writeArray(mData,newSize);
}

