/**
 * \file
 * \brief Script to export any histogram to an ASCII file
 */

/**
 * \brief Exports a selection of histograms
 * \param search Name of the histogram(s) you want to export. "*" works as a wildcard
 * \param folder Folder to save the histograms
 * 
 * If \p search or \p folder is not specified, it is taken from the "Par Auto HistExport" parameter object in the Go4 GUI
 */
Int_t ExportHist (TString search="", TString folder="")
{

	TRemiHistExport* Exporter = new TRemiHistExport("Par Auto HistExport");
	Exporter->AsciiFiltered(search, folder);
	
	return 1;
}