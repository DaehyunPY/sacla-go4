//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class that handles automatic export of Histograms to Ascii or Images via Gnuplot
//----------------------------------------------------------------
#ifndef __TRemiHistExport__
#define __TRemiHistExport__

#include "../TRemiIncludeFunctions.h"

#include "TString.h"
#include "Rtypes.h"

class TH1I;
class TH2I;
class TRemiHistExportParameter;

//using namespace std;

/**
 * \brief Class definition to provide export functions for histograms
 */

class TRemiHistExport : protected TRemiIncludeFunctions
{
	public:
		TRemiHistExport();
		TRemiHistExport(TString parameter_name);
		virtual ~TRemiHistExport();
		Bool_t SingleExportAscii (TH1* hist, TString &filename, TString folder="", TString separator="\t");
		Bool_t SingleFdcsExportAscii (TH1* hist, TString &filename, Int_t binary_end, TString folder="", TString separator="\t");
		Bool_t SingleExportGnuplot (TH1* hist, TString datafile, TString &filename, TString targetfolder="", TString labelIntensity="Counts", TString addText="");
		void SingleExportCout (TH1* hist);
		void AsciiFiltered (TString search="", TString folder="");
		void AsciiFdcsFiltered (TString search, Int_t binary_end, TString folder="");
		void GnuplotFiltered(TString search, TString AsciiFolder, TString EpsFolder);
		void AutoExport();
	private:
		void CheckFolderString(TString &folder);
		TRemiHistExportParameter *fParAutoExp;
		
	ClassDef(TRemiHistExport, 1);
};
#endif
