#include <fstream>
#include "TGo4CondArray.h"
#include "TGo4ParameterMember.h"

#include "BackupSettings.C"


/**
 * \file
 * \brief Script to export conditions and parameters and saves them to a script file
 */

Bool_t ExportSettings(TString name="conpar_online", TString folder="", Bool_t verbose=kTRUE)///*Todo more advanced string handling
{
	std::ostream cnotout(NULL);
	std::ostream & print_out = (verbose ? cout : cnotout);

	TObject*		obj;
	TObject*		pobj = 0;
	TGo4WinCond*   	win;
	TGo4PolyCond*  	pol;
	TGo4CondArray* 	ar;
	TCutG*			cut;
	TGo4Parameter*	param;
	TClass*			paraclass;
	TDataMember*	member;
	TGo4ParameterMember* info;

	TString 		filename, cName, pName, pClass, pInt, mName, mValue, mType, oldName;
	Int_t			points, dim, ia, ib=0, ip, ii, indx1, indx2, adim;
	Double_t*		xs;
	Double_t*		ys;
	Double_t		xl, xu, yl, yu, x[200], y[200];
	Bool_t			reset = kTRUE;

	// Check, if folder ends with slash. If not, append one!
	if ( (folder.Length() > 0) && !(folder.EndsWith("/")) )
		folder += "/";
	filename = folder + name + ".C";
	std::ofstream file_out (filename.Data());
	if (file_out.is_open()) print_out << "Exporting settings macro \"" << filename << "\":" << endl;

	file_out << "// Exported " << TDatime().AsSQLString() << endl << endl;
	file_out << "#include \"ConPar.Ch\"\n";
	file_out << endl;
	file_out << "Bool_t " << name << "()\n";
	file_out << "{\n";
	file_out << "\t// #ifndef __GO4ANAMACRO__\n";
	file_out << "\t// cout << \"Macro can execute only in analysis\" << endl;\n";
	file_out << "\t// return kFALSE;\n";
	file_out << "\t// #endif\n";
	file_out << endl;
	file_out << "\tDouble_t x[200], y[200];\n";
	file_out << endl;
	file_out << "\t// Set Conditions\n";
	file_out << endl;

	while ( (obj = go4->NextMatchingObject("*","Conditions",reset)) != 0 ){
		reset = kFALSE;
		if ( obj->InheritsFrom("TGo4CondArray") )
		{
			file_out << endl;
			ar = dynamic_cast<TGo4CondArray*>(obj);
			cName = ar->GetName();
			if ( ar->IsPolygonType() ) for(ia=0; ia<ar->GetNumber(); ia++)
			{
				pol = dynamic_cast<TGo4PolyCond*>(ar->At(ia));
				cut = pol->GetCut(kFALSE); // no owner
				xs = cut->GetX();
				ys = cut->GetY();
				points = cut->GetN();
				for (ip=0; ip<points; ip++)
				{
					file_out << "\tx[" << ip << "] = " << *xs << ";\ty[" << ip << "] = " << *ys << ";\n";
					x[ip] = *xs;
					y[ip] = *ys;
					xs++; ys++;
				}
				file_out << "\tSetPolyConditionInArray(\"" << cName << "\", " << ia << ", " << points << ", x, y, " << !(pol->IsEnabled()) <<", " << ( (!(pol->FixedResult())) ^ (!(pol->IsTrue())) ) << ");\n";
				file_out << endl;
			}
			else for (ia=0; ia<ar->GetNumber(); ia++)
			{
				win = dynamic_cast<TGo4WinCond*>(ar->At(ia));
				win->GetValues(dim, xl, xu, yl, yu);
				file_out << "\tSetWindowConditionInArray(\"" << cName << "\", " << ia << ", " << xl << ", " << xu << ", " << !(win->IsEnabled()) << ", " << ( (!(win->FixedResult())) ^ (!(win->IsTrue())) ) << ");\n";
			}
			file_out << endl;
			print_out << "*** " << cName << " was appended\n";
		}// end condition array

		if ( obj->InheritsFrom("TGo4WinCond") )
		{
			win = dynamic_cast<TGo4WinCond*>(obj);
			cName = win->GetName();
			win->GetValues(dim, xl, xu, yl, yu);
			file_out << "\tSetWindowCondition(\"" << cName << "\", " << xl << ", " << xu << ", " << !(win->IsEnabled()) << ", " << ( (!(win->FixedResult())) ^ (!(win->IsTrue())) ) << ");\n";
			print_out << "*** " << cName << " was appended\n";
		}// end window condition

		if ( obj->InheritsFrom("TGo4PolyCond") )
		{
			file_out << endl;
			pol = dynamic_cast<TGo4PolyCond*>(obj);
			cName = pol->GetName();
			cut = pol->GetCut(kFALSE); // no owner

			// if poly condition is defined but never used GetCut can return a null pointer
			if (cut)
			{
				xs = cut->GetX();
				ys = cut->GetY();
				points = cut->GetN();
			}
			else
			{
				points = 0;
			}

			for(ip=0; ip<points;ip++)
			{
				file_out << "\tx[" << ip << "] = " << *xs << ";\ty[" << ip << "] = " << *ys << ";\n";
				x[ip] = *xs;
				y[ip] = *ys;
				xs++; ys++;
			}
			file_out << "\tSetPolyCondition(\"" << cName << "\", " << points << ", x, y, " << !(pol->IsEnabled()) << ", " << ( (!(pol->FixedResult())) ^ (!(pol->IsTrue())) ) << ");\n";
			file_out << endl;
			print_out << "*** " << cName << " was appended\n";
		}// end poly condition
	}// end while(NextMatchingObject) conditions

	file_out << endl;
	file_out << "\t// Set Parameter\n";
	file_out << endl;
	file_out << "\tTObjArray* DataMembers;" << endl;
	reset = kTRUE;
	while ( (obj = go4->NextMatchingObject("*", "Parameters", reset)) != 0 ){
		reset = kFALSE;
		param = dynamic_cast<TGo4Parameter*>(obj);
		paraclass = param->IsA();
		pName = param->GetName();
		pClass = paraclass->GetName();
		// ------------------------------------------
		TObjArray *fitems = new TObjArray();
		fitems->SetOwner(kTRUE);
		param->GetMemberValues(fitems);
		TIter iter(fitems);
		// ------------------------------------------
		ib++;
		file_out << "\tDataMembers = OpenParameter (\"" << pName << "\");" << endl;
		ii = 0;
		while ( (info = dynamic_cast<TGo4ParameterMember*>(iter())) != 0){
				oldName = mName;
				mName = info->GetName();
				if (mName != oldName) ii = 0;
				mValue = info->GetStrValue();
				if (info->IsArrayItem())
				{ // member type is array item
					file_out << "\t\t" << "SetParameterArrayMember (DataMembers, \"" << mName << "\", \"" << ii << "\", " << mValue << "\");\n";
					ii++;
				}
				else
					file_out << "\t\t" << "SetParameterMember (DataMembers, \"" << mName << "\", \"" << mValue << "\");\n";
		}
		file_out << "\tSaveParameter (DataMembers, \"" << pName << "\");" << endl;
		file_out << "\t\n";
		print_out << "*** " << pName << " was appended\n";
	}// end while(NextMatchingObject) parameter

	file_out << "\treturn kTRUE;\n" << "}\n\n";
	file_out.close();

	cout << "Exported settings macro \"" << filename << "\"" << endl;

	return BackupSettings(name, folder);
}// end ExportSettings


