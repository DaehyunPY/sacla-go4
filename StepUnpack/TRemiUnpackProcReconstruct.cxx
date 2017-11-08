//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This reconstructs missing delayline and hexanode signals. Note that the detector-specific tasks are cared for in the respective position-calculating classes
//----------------------------------------------------------------

#include "TRemiUnpackProcReconstruct.h"

#include <algorithm>

/**
 * \brief Default constructor. Not to be used!
 */
TRemiUnpackProcReconstruct::TRemiUnpackProcReconstruct()
{
}

/**
 * \brief Principal constructor using a configuration file object
 */
TRemiUnpackProcReconstruct::TRemiUnpackProcReconstruct(ConfigFile* UnpackConf, TString particle) :
	TRemiIncludeFunctions(UnpackConf)
{
	TString basefolder = "Unpack";
	
	//------------------------------------------//
	// Read information from configuration file //
	//------------------------------------------//

	TString basename = "unp_";
	TString fieldname = basename + "condname_tdc";
	
	basename = basename + particle + "_";
	fieldname = basename + "shortname";
	TString shortname = fConfigFile->read<TString> (fieldname.Data(), TString(particle(0,3)));
	
	//------------------//
	// Create Parameter //
	//------------------//
	TString paraname = "Par " + shortname + " reconstruct";
	TString folder = basefolder + "/" + shortname;
	fParaReconst = MakeParam<TRemiReconstructParameter>(paraname, folder); // Gets parameter from analysis if already existing
	
	//------------------//
	// Create condition //
	//------------------//
}


TRemiUnpackProcReconstruct::~TRemiUnpackProcReconstruct()
{
}


/**
 * \brief Finds and calculates a missing delayline signal if the other delayline signal and the MCP signal are existing
 * \param List1It (output) Iterator pointing to the retrieved value from first list
 * \param Value2 (output) Calculated time at second end of wire
 * \param List1 List of time values from one end of the delay-line
 * \param McpValue (output) calculated time of the MCP hit to test
 * \param ConTimeSum Current time sum condition
 * \param ConSort1 Valid range window condition for first signal
 * \param ConSort2 Valid range window condition for second signal
 */
Bool_t TRemiUnpackProcReconstruct::RecoDelayline(std::vector<Float_t>::iterator &List1It, Float_t &Value2, std::vector<Float_t> &List1, Float_t McpValue, TGo4Condition* ConTimeSum, TGo4WinCond* ConSort1, TGo4WinCond* ConSort2)
{
	List1It = std::lower_bound (List1.begin(), List1.end(), McpValue + ConSort1->GetXLow());
	while ( (List1It != List1.end()) && (*List1It < McpValue + ConSort1->GetXUp()) )
	{
		Value2 = (ConTimeSum->GetXLow() + ConTimeSum->GetXUp()) / 2. - *List1It + 2. * McpValue;
		if ( ConSort2->Test(Value2 - McpValue) )
			return kTRUE;
		else
			++List1It;
	}
	// If this point has been reached, no signals were found
	return kFALSE;
}

/**
 * \brief Reconstructs a missing MCP signal if the two delayline signal are existing
 * \param List1It (output) Iterator pointing to the retrieved value from first list
 * \param List2It (output) Iterator pointing to the retrieved value from second list
 * \param McpValue (output) calculated time of the MCP hit to test
 * \param List1 List of time values from one end of the delay-line
 * \param List2 List of time values from the other end of the delay-line
 * \param ConTimeSum Current time sum condition
 * \param ConSort1 Valid range window condition for first signal
 * \param ConSort2 Valid range window condition for second signal
 * 
 * Note that the second iterator has to be incremented externally if the last McpValue calculated did was not used.
 */
Bool_t TRemiUnpackProcReconstruct::RecoMCP(std::vector<Float_t>::iterator &List1It, std::vector<Float_t>::iterator &List2It, Float_t& McpValue, std::vector< Float_t >& List1, std::vector< Float_t >& List2, TGo4WinCond* ConTimeSum, TGo4WinCond* ConSort1, TGo4WinCond* ConSort2)
{
	Float_t maxdiff = ( ConSort1->GetXUp() - ConSort1->GetXLow() + ConSort2->GetXUp() - ConSort2->GetXLow()) /2;	// maximum difference between signals
	while (List1It != List1.end())
	{
		// check if second iterator has been reset
		if (List2It == List2.begin())
			List2It = std::lower_bound(List2.begin(), List2.end(), *List1It - maxdiff);
		
		// If a second signal is in the desired range, calculate MCP signal and return
		while ( (List2It != List2.end()) && (*List2It < *List1It + maxdiff))
		{
			McpValue = .5 * (*List1It + *List2It - (ConTimeSum->GetXUp() - ConTimeSum->GetXLow()) / 2.);
			
			// Check if MCP value is in the allowed range relative to the delayline signals. Only have to check one, other one will be fulfilled automatically.
			if ( ConSort1->Test(*List1It - McpValue) )
				return kTRUE;
			else
				++List2It;
		}
		
		// If no suitable second signal was found, go to next value of first list and reset iterator of second list
		++List1It;
		List2It = List2.begin();
	}
	
	// If this point has been reached, no signals were found
	return kFALSE;
}

