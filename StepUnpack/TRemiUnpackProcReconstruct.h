//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This reconstructs missing delayline and hexanode signals. Note that the detector-specific tasks are cared for in the respective position-calculating classes
//----------------------------------------------------------------

#ifndef TREMIUNPACKPROCRECONSTRUCT_H
#define TREMIUNPACKPROCRECONSTRUCT_H

#include "../Core/TRemiIncludeFunctions.h"
#include "Parameters/TRemiReconstructParameter.h"

#include "../Core/Extern/ConfigFile.h"

#include "TGo4WinCond.h"

#include "TString.h"
#include "TArrayI.h"

#include <vector>

/**
 * \brief Handles reconstruction of lost events in the unpack step
 */
class TRemiUnpackProcReconstruct : public TRemiIncludeFunctions
{
	public:
		TRemiUnpackProcReconstruct();
		TRemiUnpackProcReconstruct(ConfigFile* UnpackConf, TString particle);
		~TRemiUnpackProcReconstruct();

		Bool_t RecoDelayline(std::vector<Float_t>::iterator &List1It, Float_t &Value2, std::vector<Float_t> &List1, Float_t McpValue, TGo4Condition* ConTimeSum, TGo4WinCond* ConSort1, TGo4WinCond* ConSort2);
		Bool_t RecoMCP(std::vector<Float_t>::iterator &List1It, std::vector<Float_t>::iterator &List2It, Float_t& McpValue, std::vector< Float_t >& List1, std::vector< Float_t >& List2, TGo4WinCond* ConTimeSum, TGo4WinCond* ConSort1, TGo4WinCond* ConSort2);

	private:
		TRemiReconstructParameter*	fParaReconst; ///< Parameter for reconstruction of this particle

	ClassDef(TRemiUnpackProcReconstruct, 1);
};



#endif
