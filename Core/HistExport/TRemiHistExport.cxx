//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Class that handles automatic export of Histograms to Ascii or Images via Gnuplot
//----------------------------------------------------------------
#include "TRemiHistExport.h"

#include "TRemiHistExportParameter.h"
#include "../Extern/Gnuplot.h"

#include "TGo4Analysis.h"

#include "TH1.h"
#include "TH2.h"
#include "TString.h"
#include "TMath.h"
#include "Riostream.h"


/**
 * \brief constructor with no Parameter object
 */
TRemiHistExport::TRemiHistExport()
{
}

/**
 * \brief constructor with Parameter object
 */
TRemiHistExport::TRemiHistExport(TString parameter_name)
{
	// Check if HistExport Parameter exists or create new
	
	fParAutoExp = MakeParam<TRemiHistExportParameter>(parameter_name);
}

TRemiHistExport::~TRemiHistExport()
{
	cout << "deleting TRemiHistExport Member" << endl;
}

/**
 * \brief Export Single Histogram into ASCII file
 * \param hist Pointer to histogram that shall be exported
 * \param filename (Output) Returns filename of generated file
 * \param folder (optional) Folder to locate exported file. Default is none, i.e. the current working directory
 * \param separator (otional) String used to separate values in the exported file. Default is a tabulator 
 */
Bool_t TRemiHistExport::SingleExportAscii(TH1* hist, TString &filename, TString folder, TString separator) 
{
	Int_t i,j;
	Double_t xcenter, xwidth;
	Bool_t success=kFALSE;
	CheckFolderString(folder);
	filename = folder + hist->GetName() + ".dat";
	ofstream file_out(filename);
	file_out << "# Output " << hist->ClassName() << ": " << hist->GetName() << " (" << hist->GetTitle() << ")\n";
	file_out << "# Total count: " << hist->Integral() << endl;
	if (hist->GetDimension()==1)
	{
		file_out << "# BinCenter" << separator << "Content" << separator << "BinHalfWidth" << separator << "Error\n";
		for (i=1; i<=hist->GetNbinsX(); i++)
			file_out << hist->GetBinCenter(i) << separator << hist->GetBinContent(i) << separator << hist->GetBinWidth(i)/2 << separator << hist->GetBinError(i) << endl;
		if (i>1)
			success=kTRUE;
	}
	else if (hist->GetDimension()==2)
	{
		file_out << "# xBinCenter" << separator << "yBinCenter" << separator << "Content" << separator << "xBinHalfWidth" << separator << "yBinHalfWidth" << separator << "Error" << endl;
		for (i=1; i <= hist->GetNbinsX(); i++)
		{
			xcenter = hist->GetXaxis()->GetBinCenter(i);
			xwidth = hist->GetXaxis()->GetBinWidth(i)/2;
			for (j=1; j <= hist->GetNbinsY(); j++)
				file_out << xcenter << separator << hist->GetYaxis()->GetBinCenter(j) << separator << hist->GetBinContent(i,j) << separator << xwidth << separator << hist->GetYaxis()->GetBinWidth(j)/2 << separator << hist->GetBinError(i,j) << endl;
			if (j>1)
				file_out << endl; // produce a blank line after each set of Ybins for a certain Xbin
		}
		if (i>1)
			success=kTRUE;
	}
	file_out.close();
	if (success == kTRUE)
		cout << "*** TRemiHistExport: Histogram " << hist->GetName() << " written to " << filename << endl;
	return success;
}


/**
 * \brief Export Single Histogram to EPS via Gnuplot
 * \param hist Pointer to histogram that shall be exported
 * \param datafile Path to and name of ASCII file to use for plot
 * \param filename (Output) Returns filename of generated file
 * \param targetfolder (optional) Folder to locate exported file. Default is none, i.e. the current working directory
 * \param labelIntensity (optional) label to use on the Y-axis
 * \param addText (optional) text to add to the plot's title
 */
Bool_t TRemiHistExport::SingleExportGnuplot(TH1* hist, TString datafile, TString &filename, TString targetfolder, TString labelIntensity, TString addText)
{
	TString command;
	Bool_t success=kFALSE;
	CheckFolderString(targetfolder);
	filename = targetfolder + hist->GetName();
//	datafile = "/home/xuser/go4code/branches/export_histograms/" + datafile;
	cout << "*** TRemiHistExport: Gnuplot uses file " << datafile << endl;
	Gnuplot gp = Gnuplot();
	if (hist->GetDimension()==1)
	{
		command = "set terminal postscript enhanced";
		gp << command.Data();
		command = "set output \"" + filename + "\"";
		gp << command.Data();
		command = "set ylabel \"" + labelIntensity + "\"";
		gp << command.Data();
		command = "plot \"" + datafile + "\" title \"" + hist->GetTitle() + addText + "\" w xyerrorbars";
		gp << command.Data();
//		command="";
//		gp.cmd(command);
		success=kTRUE;
	}
	if (success == kTRUE)
		cout << "*** TRemiHistExport: Histogram " << hist->GetName() << " exported to " << filename << ".eps\n";
	return success;
}

/**
 * \brief Export Single Histogram to standard output
 */
void TRemiHistExport::SingleExportCout(TH1* hist)  
{
	Int_t i;
	if (hist->GetDimension()==1)
	{
		cout << "# BinCenter\tBinWidth\tContent\tError\n";
		for (i=1; i<=hist->GetNbinsX(); i++)
			cout << hist->GetBinCenter(i) << "\t" << hist->GetBinWidth(i) << "\t" << hist->GetBinContent(i) << "\t" << hist->GetBinError(i) << endl;
	}
}

/**
 * \brief Export histograms filtered by string "search" into given folder in ASCII format
 */
void TRemiHistExport::AsciiFiltered(TString search, TString folder)
{
	Int_t i=0;
	Bool_t reset=kTRUE;
	TObject *obj;
	TString AsciiFilename="";
	if (search.Length()==0)
		search = fParAutoExp->SearchString;
	if (folder.Length()==0)
		folder = fParAutoExp->AsciiFolder;
	while ( (obj = TGo4Analysis::Instance()->NextMatchingObject(search, "Histograms", reset) ) != 0 )
	{
		reset=kFALSE;
		if ( (obj->InheritsFrom("TH1") == kTRUE) &&  (SingleExportAscii((TH1*)obj, AsciiFilename, folder) == kTRUE) )
			i++;
	}
	cout << "*** TRemiHistExport: Saved " << i << " histograms to files\n";
}

/**
 * \brief Export histograms filtered by string "search" into given folder to Gnuplot
 */
void TRemiHistExport::GnuplotFiltered(TString search, TString AsciiFolder, TString EpsFolder)
{
	Int_t i=0;
	Bool_t reset=kTRUE;
	TString AsciiFilename="", EpsFilename="";
	TObject *obj;
	while ( (obj = TGo4Analysis::Instance()->NextMatchingObject(search, "Histograms", reset) ) != 0 )
	{
		reset=kFALSE;
		if ( (obj->InheritsFrom("TH1") == kTRUE) &&  (SingleExportAscii((TH1*)obj, AsciiFilename, AsciiFolder) == kTRUE) && (SingleExportGnuplot((TH1*)obj, AsciiFilename, EpsFilename , EpsFolder) == kTRUE) )
			i++;
	}
	cout << "*** TRemiHistExport: Saved " << i << " histograms to files and produced graphs\n";
}

/**
 * \brief Automatic Export of histograms in the current scope as configured by a parameter
 */
void TRemiHistExport::AutoExport()  
{
	if (fParAutoExp->useAutoExport==kTRUE)
	{
		cout << "*** TRemiHistExport: Starting Histogram export routine\n";
		if (fParAutoExp->useGnuplot==kTRUE)
			GnuplotFiltered (fParAutoExp->SearchString, fParAutoExp->AsciiFolder, fParAutoExp->EpsFolder);
		else
			AsciiFiltered (fParAutoExp->SearchString, fParAutoExp->AsciiFolder);
	}
}

/**
 * \brief Checks if a folder string ends with a slash
 * 
 * If there is no slash at the end of the string, it is appended
 * \param folder String to work on, is used as input and output
 */
void TRemiHistExport::CheckFolderString(TString& folder)
{
	if ( (folder.Length() > 0) && !(folder.EndsWith("/")) )
		folder += "/";
}


ClassImp(TRemiHistExport);
