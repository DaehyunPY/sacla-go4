// Exported 2017-11-07 15:55:44

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
	SetWindowCondition("c_Tof_e_all", 0, 500, 0, 0);
	SetWindowCondition("c_Tof_i_all", 0, 500, 0, 0);

	x[0] = -65;	y[0] = -65;
	x[1] = -65;	y[1] = 65;
	x[2] = 65;	y[2] = 65;
	x[3] = 65;	y[3] = -65;
	x[4] = -65;	y[4] = -65;
	SetPolyCondition("cXY_i_sort", 5, x, y, 0, 0);

	SetWindowCondition("c_Rad_i", 0, 65, 0, 0);
	SetWindowCondition("c_Y_i_sort", -65, 65, 0, 0);
	SetWindowCondition("c_Tsum_ui", 59.3274, 73.4212, 0, 0);
	SetWindowCondition("c_Tsum_vi", 67.2469, 78.6168, 0, 0);
	SetWindowCondition("c_Tsum_wi", 67.0721, 79.3871, 0, 0);
	SetWindowCondition("c_Sort_ui1", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_ui2", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_vi1", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_vi2", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_wi1", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_wi2", -100, 900, 0, 0);

	x[0] = 0;	y[0] = 0;
	x[1] = 0;	y[1] = 10000;
	x[2] = 10000;	y[2] = 10000;
	x[3] = 10000;	y[3] = 0;
	x[4] = 0;	y[4] = 0;
	SetPolyCondition("c_unp_PiPiCo", 5, x, y, 0, 0);


	x[0] = 0;	y[0] = 0;
	x[1] = 0;	y[1] = 500;
	x[2] = 10000;	y[2] = 500;
	x[3] = 10000;	y[3] = 0;
	x[4] = 0;	y[4] = 0;
	SetPolyCondition("c_unp_PePiCo", 5, x, y, 0, 0);


	x[0] = 0;	y[0] = 0;
	x[1] = 0;	y[1] = 500;
	x[2] = 10000;	y[2] = 500;
	x[3] = 10000;	y[3] = 0;
	x[4] = 0;	y[4] = 0;
	SetPolyCondition("c_unp_PePePiCo", 5, x, y, 0, 0);


	x[0] = 0;	y[0] = 0;
	x[1] = 0;	y[1] = 10000;
	x[2] = 500;	y[2] = 10000;
	x[3] = 500;	y[3] = 0;
	x[4] = 0;	y[4] = 0;
	SetPolyCondition("c_unp_PePiPiCo", 5, x, y, 0, 0);


	x[0] = -45;	y[0] = -45;
	x[1] = -45;	y[1] = 45;
	x[2] = 45;	y[2] = 45;
	x[3] = 45;	y[3] = -45;
	x[4] = -45;	y[4] = -45;
	SetPolyCondition("cXY_e_sort", 5, x, y, 0, 0);

	SetWindowCondition("c_Y_e_sort", -45, 45, 0, 0);

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
		SetParameterMember (DataMembers, "Threshold", "20");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 1 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 2 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "20");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 2 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 3 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "20");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 3 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 4 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "20");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 4 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 5 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "20");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 5 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 6 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "20");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 6 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 7 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "20");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 7 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 8 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "20");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 8 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par unpack global");
		SetParameterMember (DataMembers, "to", "0");
	SaveParameter (DataMembers, "Par unpack global");
	
	DataMembers = OpenParameter ("Par i hexanode");
		SetParameterMember (DataMembers, "scaleU", "1");
		SetParameterMember (DataMembers, "scaleV", "0.92");
		SetParameterMember (DataMembers, "scaleW", "0.885");
		SetParameterMember (DataMembers, "shiftW", "0");
		SetParameterMember (DataMembers, "distancemax", "1");
		SetParameterMember (DataMembers, "xo", "-0.5");
		SetParameterMember (DataMembers, "yo", "0");
		SetParameterMember (DataMembers, "phi", "0");
		SetParameterMember (DataMembers, "size_raw", "132");
		SetParameterMember (DataMembers, "size_mm", "80");
	SaveParameter (DataMembers, "Par i hexanode");
	
	return kTRUE;
}

