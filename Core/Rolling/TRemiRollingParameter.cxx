#include "TRemiRollingParameter.h"

#include "TGo4Parameter.h"
#include "TObjArray.h"
#include "TGo4ParameterMember.h"


/**
 * \brief Default constructor with optional name
 */
TRemiRollingParameter::TRemiRollingParameter(const char* name) : TGo4Parameter(name)
{
	Init();
}

/**
 * \brief Destructor
 */
TRemiRollingParameter::~TRemiRollingParameter()
{
}

/**
 * \brief Set string parameter name to value (will be overwritten if macro produced by ExportSettings.C is called)
 */
void TRemiRollingParameter::SetParameter(TString name, TString value)
{
	TObjArray * DataMembers = new TObjArray();
	DataMembers->SetOwner(kTRUE);
	GetMemberValues(DataMembers);

	TGo4ParameterMember * member = dynamic_cast<TGo4ParameterMember*>(DataMembers->FindObject(name));
	if (member) {
		member->SetStrValue(value);
		SetMemberValues(DataMembers);
	}

	delete DataMembers;
}

/**
 * \brief Set integer parameter name to value (will be overwritten if macro produced by ExportSettings.C is called)
 */
void TRemiRollingParameter::SetParameter(TString name, Int_t value)
{
	TObjArray * DataMembers = new TObjArray();
	DataMembers->SetOwner(kTRUE);
	GetMemberValues(DataMembers);

	TGo4ParameterMember * member = dynamic_cast<TGo4ParameterMember*>(DataMembers->FindObject(name));
	if (member) {
		member->SetIntValue(value);
		SetMemberValues(DataMembers);
	}

	delete DataMembers;
}

ClassImp(TRemiRollingParameter);

