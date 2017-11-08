//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Parameter class for the automatic export of Histograms to Ascii or Images via Gnuplot
//----------------------------------------------------------------

#include "TRemiHistExportParameter.h"

#include "Riostream.h"


//***********************************************************
TRemiHistExportParameter::TRemiHistExportParameter() 
: TGo4Parameter("TRemiHistExportParameter")
{ 
	useAutoExport=kFALSE;
	SearchString="*";
	AsciiFolder="hist/";
	useGnuplot=kFALSE;
	EpsFolder="hist/";
}
//***********************************************************
TRemiHistExportParameter::TRemiHistExportParameter(const char* name) 
: TGo4Parameter(name)
{ 
	useAutoExport=kFALSE;
	SearchString="*";
	AsciiFolder="hist/";
	useGnuplot=kFALSE;
	EpsFolder="hist/";
}
//***********************************************************
TRemiHistExportParameter::~TRemiHistExportParameter() 
{ 
}
//***********************************************************

ClassImp(TRemiHistExportParameter);




//----------------------------END OF GO4 SOURCE FILE ---------------------


