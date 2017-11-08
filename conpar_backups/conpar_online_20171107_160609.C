// Exported 2017-11-07 16:06:09

#include "ConPar.Ch"

Bool_t conpar_online()
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

	x[0] = 40;	y[0] = 0;
	x[1] = 39.6578;	y[1] = 5.22105;
	x[2] = 38.637;	y[2] = 10.3528;
	x[3] = 36.9552;	y[3] = 15.3073;
	x[4] = 34.641;	y[4] = 20;
	x[5] = 31.7341;	y[5] = 24.3505;
	x[6] = 28.2843;	y[6] = 28.2843;
	x[7] = 24.3505;	y[7] = 31.7341;
	x[8] = 20;	y[8] = 34.641;
	x[9] = 15.3073;	y[9] = 36.9552;
	x[10] = 10.3528;	y[10] = 38.637;
	x[11] = 5.22105;	y[11] = 39.6578;
	x[12] = 2.44929e-15;	y[12] = 40;
	x[13] = -5.22105;	y[13] = 39.6578;
	x[14] = -10.3528;	y[14] = 38.637;
	x[15] = -15.3073;	y[15] = 36.9552;
	x[16] = -20;	y[16] = 34.641;
	x[17] = -24.3505;	y[17] = 31.7341;
	x[18] = -28.2843;	y[18] = 28.2843;
	x[19] = -31.7341;	y[19] = 24.3505;
	x[20] = -34.641;	y[20] = 20;
	x[21] = -36.9552;	y[21] = 15.3073;
	x[22] = -38.637;	y[22] = 10.3528;
	x[23] = -39.6578;	y[23] = 5.22105;
	x[24] = -40;	y[24] = 4.89859e-15;
	x[25] = -39.6578;	y[25] = -5.22105;
	x[26] = -38.637;	y[26] = -10.3528;
	x[27] = -36.9552;	y[27] = -15.3073;
	x[28] = -34.641;	y[28] = -20;
	x[29] = -31.7341;	y[29] = -24.3505;
	x[30] = -28.2843;	y[30] = -28.2843;
	x[31] = -24.3505;	y[31] = -31.7341;
	x[32] = -20;	y[32] = -34.641;
	x[33] = -15.3073;	y[33] = -36.9552;
	x[34] = -10.3528;	y[34] = -38.637;
	x[35] = -5.22105;	y[35] = -39.6578;
	x[36] = -7.34788e-15;	y[36] = -40;
	x[37] = 5.22105;	y[37] = -39.6578;
	x[38] = 10.3528;	y[38] = -38.637;
	x[39] = 15.3073;	y[39] = -36.9552;
	x[40] = 20;	y[40] = -34.641;
	x[41] = 24.3505;	y[41] = -31.7341;
	x[42] = 28.2843;	y[42] = -28.2843;
	x[43] = 31.7341;	y[43] = -24.3505;
	x[44] = 34.641;	y[44] = -20;
	x[45] = 36.9552;	y[45] = -15.3073;
	x[46] = 38.637;	y[46] = -10.3528;
	x[47] = 39.6578;	y[47] = -5.22105;
	x[48] = 40;	y[48] = 0;
	SetPolyCondition("CircleCondition", 49, x, y, 0, 0);

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
		SetParameterMember (DataMembers, "xo", "-1.2");
		SetParameterMember (DataMembers, "yo", "0.5");
		SetParameterMember (DataMembers, "phi", "0");
		SetParameterMember (DataMembers, "size_raw", "132");
		SetParameterMember (DataMembers, "size_mm", "80");
	SaveParameter (DataMembers, "Par i hexanode");
	
	return kTRUE;
}

