//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		General base class for Event classes defined in GENERiC
//----------------------------------------------------------------
#include "TRemiEventElement.h"

#include "TGo4EventElement.h"
#include "TGo4FileSource.h"
#include "TString.h"
#include "Riostream.h"
#include "TROOT.h"

/**
 * \brief Default constructor
 */
TRemiEventElement::TRemiEventElement() : TGo4EventElement()
{
}

/**
 * \brief Constructor with a name
 * 
 * Just puts through the name to the corresponding constructor of TGo4EventElement
 */
TRemiEventElement::TRemiEventElement(const char* name) : TGo4EventElement(name)
{
}


TRemiEventElement::~TRemiEventElement()
{
}

/**
 * \brief Checks if a parameter file for a Root-file exists and executes it if found
 * 
 * If you are working with intermediate .root-files saved between analysis steps you can define a parameter macro with the same name than the root file, but the file ending '.C' instead of '.root'. Therefore, this function is called upon initialisation of the analysis for each event object.
 */
void TRemiEventElement::CheckForParameterMacro()
{
	if (CheckEventSource("TGo4FileSource"))
	{
//		TGo4FileSource* rootfile = (TGo4FileSource*)GetEventSource();
		TGo4FileSource* rootfile = dynamic_cast<TGo4FileSource*> (GetEventSource());
		if (rootfile)
		{
			std::cout << "*** " << ClassName() << ": Input file is " << rootfile->GetActiveName() << std::endl;
			TString filename = rootfile->GetActiveName();
			Int_t a=-1,b;
			do
			{
				b = a; // b gets last value of a
				a = filename.Index(".", b + 1); // start searching for next dot starting from last occurence
			}
			while (a != -1);
			filename.Resize(b); // truncate filename up to the last dot
			filename += ".C";
			std::ifstream fFile(filename);
			if(fFile) 
			{
				fFile.close();
				std::cout << "*** TRemiEventElement: Found macro " << filename << std::endl;
				TString command = ".x " + filename;
				gROOT->ProcessLine(command);
			}
			
		}
	}
}

ClassImp(TRemiEventElement);

