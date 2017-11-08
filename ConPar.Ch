#ifndef CONPARHEADER
#define CONPARHEADER

#include "Riostream.h"
/**
 * \file 
 * \brief Include header for condition and parameter setting macros
 * 
 * File extension .h is avoided to avoid compilation of this file when "make" is called
 */

/**
 * \brief Extracts a Window condition object from the analysis and assigns new values to it if the object is found and correct
 */
void SetWindowCondition(const Text_t* name, Double_t leftvalue, Double_t rightvalue, Bool_t return_true=kFALSE, Bool_t negate=kFALSE)
{
	TGo4Condition* con;
	TGo4WinCond*   win;

	con = dynamic_cast<TGo4Condition *>(go4->GetObject(name,"Go4"));
	if(!con)
	{
		cout << __FILE__ << ": could not find condition " << name << endl;
		return; 
	}
	if(strcmp(con->ClassName(),"TGo4WinCond"))
	{
		cout << __FILE__ << ": Condition " << name << " has wrong class " << con->ClassName() << endl;
		return; 
	}

	win = dynamic_cast<TGo4WinCond *>(con);
	win->ResetCounts();
	win->SetValues(leftvalue, rightvalue);

	// switches for condition behaviour
	// return_true negate
	// 				0				0			=> Condition works
	//				1				0			=> Return always ture
	//				0				1			=> Invert condition
	//				1				1			=> Return always false
	win->Invert(negate);
	if(return_true) win->Disable(!negate);
		else win->Enable();

	cout << __FILE__ << ": Set window condition " << name << " to new values\n";
}

/**
 * \brief Extracts a Window condition inside a condition array object from the analysis and assigns new values to it if the object is found and correct
 */
void SetWindowConditionInArray(const Text_t* name, Int_t index, Double_t leftvalue, Double_t rightvalue,  Bool_t return_true=kFALSE, Bool_t negate=kFALSE)
{
	TGo4Condition *con;
	TGo4CondArray *conar;
	TGo4WinCond   *win;

	con = dynamic_cast<TGo4Condition *>(go4->GetObject(name,"Go4"));
	if(!con)
	{
		cout << __FILE__ << ": could not find condition " << name << endl;
		return; 
	}
	if(strcmp(con->ClassName(),"TGo4CondArray"))
	{
		cout << __FILE__ << ": Condition " << name << " has wrong class " << con->ClassName() << endl;
		return; 
	}

	conar = dynamic_cast<TGo4CondArray *>(con);
	con=conar->At(index);
	if (!con)
	{
		cout << __FILE__ << ": No element " << index << " in condition " << name << endl;
		return;
	}
	if(strcmp(con->ClassName(),"TGo4WinCond"))
	{
		cout << __FILE__ << ": Element " << index << " in condition " << name << " has wrong class " << con->ClassName() << endl;
		return; 
	}

	win = dynamic_cast<TGo4WinCond *>(con);
	win->ResetCounts();
	win->SetValues(leftvalue, rightvalue);
	
	win->Invert(negate);
	if(return_true) win->Disable(!negate);
		else win->Enable();

	cout << __FILE__ << ": Set window condition " << index << " in array " << name << " to new values\n";
}

/**
 * \brief Extracts a Polygon condition object from the analysis and assigns new values to it if the object is found and correct
 */
void SetPolyCondition(const Text_t* name, Int_t points, Double_t* xvalues, Double_t* yvalues, Bool_t return_true=kFALSE, Bool_t negate=kFALSE)
{
	TGo4Condition* con;
	TGo4PolyCond*  pol;

	con = dynamic_cast<TGo4Condition *>(go4->GetObject(name,"Go4"));
	if(!con)
	{
		cout << __FILE__ << ": could not find condition " << name << endl;
		return; 
	}
	if(strcmp(con->ClassName(),"TGo4PolyCond"))
	{
		cout << __FILE__ << ": Condition " << name << " has wrong class " << con->ClassName() << endl;
		return; 
	}

	pol = dynamic_cast<TGo4PolyCond *>(con);
	pol->ResetCounts();
	pol->SetValues(xvalues, yvalues, points);
	
	pol->Invert(negate);
	if(return_true) pol->Disable(!negate);
		else pol->Enable();

	cout << __FILE__ << ": Set polygon condition " << name << " to new values\n";
}

/**
 * \brief Extracts a Polygon condition inside a condition array object from the analysis and assigns new values to it if the object is found and correct
 */
void SetPolyConditionInArray(const Text_t* name, Int_t index,  Int_t points, Double_t* xvalues, Double_t* yvalues, Bool_t return_true=kFALSE, Bool_t negate=kFALSE)
{
	TGo4Condition *con;
	TGo4CondArray *conar;
	TGo4PolyCond   *pol;

	con = dynamic_cast<TGo4Condition *>(go4->GetObject(name,"Go4"));
	if(!con)
	{
		cout << __FILE__ << ": could not find condition " << name << endl;
		return; 
	}
	if(strcmp(con->ClassName(),"TGo4CondArray"))
	{
		cout << __FILE__ << ": Condition " << name << " has wrong class " << con->ClassName() << endl;
		return; 
	}

	conar = dynamic_cast<TGo4CondArray *>(con);
	con=conar->At(index);
	if (!con)
	{
		cout << __FILE__ << ": No element " << index << " in condition " << name << endl;
		return;
	}
	if(strcmp(con->ClassName(),"TGo4PolyCond"))
	{
		cout << __FILE__ << ": Condition " << name << " has wrong class " << con->ClassName() << endl;
		return; 
	}

	pol = dynamic_cast<TGo4PolyCond *>(con);
	pol->ResetCounts();
	pol->SetValues(xvalues, yvalues, points);
	
	pol->Invert(negate);
	if(return_true) pol->Disable(!negate);
		else pol->Enable();

	cout << __FILE__ << ": Set polygon condition " << index << " in array " << name << " to new values\n";
}

/**
 * \brief Extracts a parameter object from the analysis and checks if it exists and has the correct type
 */
template <class ParameterType> 
		ParameterType* GetParameter(TString name, TString classname)
{
	TGo4Parameter* par;
	par = dynamic_cast<TGo4Parameter*>(go4->GetObject(name, "Go4"));
	if(!par)
	{
		cout << __FILE__ << ": could not find parameter " << name << endl;
		return;
	}
	if (!par->InheritsFrom(classname))
	{
		cout << __FILE__ << ": Parameter " << name << " has wrong class " << par->ClassName() << endl;
		return;
	}
	ParameterType* thispar = dynamic_cast<ParameterType*>(par);
	if (thispar)
	{
		cout << __FILE__ << ": Got parameter " << name << endl;
		return thispar;
	}
	else
		cout << __FILE__ << ": Could not assign parameter " << name << ". Maybe the Classname " << classname << " doesn't match the desired type\n";
}

/**
 * \brief Opens a parameter object for changing of parameter values
 * \param ParaName Internal name of the parameter
 * \return List of parameter data members to be used by SetParameterMember() and SaveParameter()
 */
TObjArray* OpenParameter(TString ParaName)
{
	TGo4Parameter* par = go4->GetParameter(ParaName);
	if(!par)
	{
		cout << __FILE__ << ": could not find parameter " << ParaName << endl;
		return 0;
	}
	TObjArray *DataMembers = new TObjArray();
	DataMembers->SetOwner(kTRUE);
	par->GetMemberValues(DataMembers);
	cout << __FILE__ << ": Opened parameter " << ParaName << endl;
	return DataMembers;
}

/**
* \brief Sets one value in a parameter object
* \param DataMembers List of parameter data members as generated by OpenParameter()
* \param MemberName Name of the parameter member to change
* \param Value New value for membername as string
*
* Value will not saved into the parameter before SaveParameter() is called.
*/
void SetParameterMember (TObjArray* DataMembers, TString MemberName, TString Value)
{
	if (!DataMembers)
		return;
	TGo4ParameterMember* member = dynamic_cast<TGo4ParameterMember*>(DataMembers->FindObject(MemberName));
	if (!member)
	{
		cout << __FILE__ << ": could not find member " << MemberName << " in current parameter!" << endl;
		return;
	}
	if (member->IsArrayItem())
	{
		cout << __FILE__ << ": Use SetParameterArrayMember function to set array value " << MemberName << endl;
		return;
	}
	member->SetStrValue(Value);
	cout << __FILE__ << ": Set member " << MemberName << " to new value " << member->GetStrValue() << endl;
}

/**
* \brief Sets one array value in a parameter object
* \param DataMembers List of parameter data members as generated by OpenParameter()
* \param MemberName Name of the parameter member to change
* \param Index Index in the array
* \param Value New value for membername as string
*
* Value will not saved into the parameter before SaveParameter() is called.
*/
void SetParameterArrayMember (TObjArray* DataMembers, TString MemberName, Int_t index, TString Value)
{
	if (!DataMembers)
		return;
	TIter iter(DataMembers);
	TGo4ParameterMember* member;
	while( (member = dynamic_cast<TGo4ParameterMember*>(iter())) && (member->GetName() != MemberName) ) {}
	if (!member)
	{
		cout << __FILE__ << ": could not find member " << MemberName << " in current parameter!" << endl;
		return;
	}
	if (!member->IsArrayItem())
	{
		cout << __FILE__ << ": Use SetParameterMember function to set non-array value " << MemberName << endl;
		return;
	}
	for (Int_t i=0; i<index; i++) // iterate to the correct position
		member = dynamic_cast<TGo4ParameterMember*>(iter());
	if (!member)
	{
		cout << __FILE__ << ": Out of array range at index " << index << " for member " << MemberName << " in current parameter!" << endl;
		return;
	}
	member->SetStrValue(Value);
	cout << __FILE__ << ": Set member " << MemberName << "[" << index << "] to new value " << member->GetStrValue() << endl;
}

/**
 * \brief Saves an edited list of data members to a parameter object
 * \param DataMembers List of parameter data members as generated by OpenParameter() and maybe altered by SetParameterMember() and SetParameterArrayMember()
 * \param ParaName Internal name of the parameter
 * \param DeleteArray (Default = true) switch if DataMembers shall be deleted after saving the parameters
 */
void SaveParameter (TObjArray* DataMembers, TString ParaName, Bool_t DeleteArray=kTRUE)
{
	TGo4Parameter* par = go4->GetParameter(ParaName);
	if(!par)
	{
		cout << __FILE__ << ": could not find parameter " << ParaName << ". New values have not been saved!" << endl;
		return;
	}
	par->SetMemberValues(DataMembers);
	cout << __FILE__ << ": Saved changes to parameter " << ParaName << endl << endl;
	if (DeleteArray)
		delete DataMembers;
}

#endif