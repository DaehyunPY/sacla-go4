//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Parameter class for the automatic export of Histograms to Ascii or Images via Gnuplot
//----------------------------------------------------------------

#ifndef SPAREXPHP_H
#define SPAREXPHP_H

#include "TGo4Parameter.h"
#include "TString.h"

#define __ARRAYSIZE__ 100
#define __MEVX__ 5
#define __MEVY__ 10

#include "Go4StatusBase/TGo4Parameter.h"

/**
 * \brief Parameter class controlling the automatic export of histograms
 */
class TRemiHistExportParameter : public TGo4Parameter 
{

	public:
		TRemiHistExportParameter();  
		TRemiHistExportParameter(const char* name); 
		virtual ~TRemiHistExportParameter();
		
		Bool_t useAutoExport; ///< Export Histograms at the end of analysis run
		TString SearchString; ///< Search string, * finds all histograms
		TString AsciiFolder; ///< Folder to store ASCII files
		Bool_t useGnuplot; ///< Use Gnuplot to directly create EPS graphs
		TString EpsFolder; ///< Folder to store EPS files
	
	ClassDef(TRemiHistExportParameter, 1);
};


#endif //SPAR_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


