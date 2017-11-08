// Exported 2017-11-04 02:48:23

#include "ConPar.Ch"

Bool_t conpar_online_auto()
{
	// #ifndef __GO4ANAMACRO__
	// cout << "Macro can execute only in analysis" << endl;
	// return kFALSE;
	// #endif

	Double_t x[200], y[200];

	// Set Conditions

	SetWindowCondition("RatioExample_numerator", 0, 0, 0, 0);
	SetWindowCondition("RatioExample_denominator", 0, 0, 0, 0);
	SetWindowCondition("ValueExample_value", 0, 0, 0, 0);

	x[0] = 400;	y[0] = 200;
	x[1] = 396.593;	y[1] = 225.882;
	x[2] = 386.603;	y[2] = 250;
	x[3] = 370.711;	y[3] = 270.711;
	x[4] = 350;	y[4] = 286.603;
	x[5] = 325.882;	y[5] = 296.593;
	x[6] = 300;	y[6] = 300;
	x[7] = 274.118;	y[7] = 296.593;
	x[8] = 250;	y[8] = 286.603;
	x[9] = 229.289;	y[9] = 270.711;
	x[10] = 213.397;	y[10] = 250;
	x[11] = 203.407;	y[11] = 225.882;
	x[12] = 200;	y[12] = 200;
	x[13] = 203.407;	y[13] = 174.118;
	x[14] = 213.397;	y[14] = 150;
	x[15] = 229.289;	y[15] = 129.289;
	x[16] = 250;	y[16] = 113.397;
	x[17] = 274.118;	y[17] = 103.407;
	x[18] = 300;	y[18] = 100;
	x[19] = 325.882;	y[19] = 103.407;
	x[20] = 350;	y[20] = 113.397;
	x[21] = 370.711;	y[21] = 129.289;
	x[22] = 386.603;	y[22] = 150;
	x[23] = 396.593;	y[23] = 174.118;
	x[24] = 400;	y[24] = 200;
	SetPolyCondition("CircleCondition", 25, x, y, 0, 0);

	SetWindowCondition("Tdc_veto", 0, 0, 0, 0);

	// Set Parameter

	TObjArray* DataMembers;
	DataMembers = OpenParameter ("Par Auto HistExport");
		SetParameterMember (DataMembers, "useAutoExport", "0");
		SetParameterMember (DataMembers, "SearchString", "*");
		SetParameterMember (DataMembers, "AsciiFolder", "hist/");
		SetParameterMember (DataMembers, "useGnuplot", "0");
		SetParameterMember (DataMembers, "EpsFolder", "hist/");
	SaveParameter (DataMembers, "Par Auto HistExport");
	
	DataMembers = OpenParameter ("RatioExample_parameters");
		SetParameterMember (DataMembers, "mUpdate", "0");
		SetParameterMember (DataMembers, "mSourceHist", "");
		SetParameterMember (DataMembers, "mPlotError", "0");
		SetParameterMember (DataMembers, "mType", "0");
		SetParameterMember (DataMembers, "mClear", "0");
		SetParameterMember (DataMembers, "mClearInterval", "75000");
		SetParameterMember (DataMembers, "mThreshold", "0");
		SetParameterMember (DataMembers, "mThresholdCounts", "5000");
	SaveParameter (DataMembers, "RatioExample_parameters");
	
	DataMembers = OpenParameter ("ValueExample_parameters");
		SetParameterMember (DataMembers, "mUpdate", "0");
		SetParameterMember (DataMembers, "mSourceHist", "");
		SetParameterMember (DataMembers, "mPlotError", "0");
		SetParameterMember (DataMembers, "mType", "0");
		SetParameterMember (DataMembers, "mClear", "0");
		SetParameterMember (DataMembers, "mClearInterval", "75000");
		SetParameterMember (DataMembers, "mThreshold", "0");
		SetParameterMember (DataMembers, "mThresholdCounts", "5000");
	SaveParameter (DataMembers, "ValueExample_parameters");
	
	DataMembers = OpenParameter ("Par SingleShot");
		SetParameterMember (DataMembers, "NumMinibunches", "1");
		SetParameterMember (DataMembers, "MinibunchDistance", "5000");
		SetParameterMember (DataMembers, "Offset", "0");
		SetParameterMember (DataMembers, "Overlap", "0");
	SaveParameter (DataMembers, "Par SingleShot");
	
	DataMembers = OpenParameter ("Par Chan 1 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "131");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 1 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 2 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "131");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 2 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 3 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "131");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 3 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 4 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "131");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 4 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 5 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "131");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 5 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 6 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "131");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 6 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 7 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "131");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 7 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 8 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "131");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 8 Acq Tdc CFD");
	
	return kTRUE;
}

