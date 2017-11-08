// Exported 2017-10-24 19:32:06

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

	SetWindowCondition("Tdc_veto", 0, 10000, 0, 0);
	SetWindowCondition("c_Tof_e_all", 0, 500, 0, 0);

	x[0] = -45;	y[0] = -45;
	x[1] = -45;	y[1] = 45;
	x[2] = 45;	y[2] = 45;
	x[3] = 45;	y[3] = -45;
	x[4] = -45;	y[4] = -45;
	SetPolyCondition("cXY_e_sort", 5, x, y, 0, 0);

	SetWindowCondition("c_Rad_e", 0, 45, 0, 0);
	SetWindowCondition("c_Y_e_sort", -45, 45, 0, 0);
	SetWindowCondition("c_Tsum_u", 0, 300, 0, 0);
	SetWindowCondition("c_Tsum_v", 0, 300, 0, 0);
	SetWindowCondition("c_Tsum_w", 0, 300, 0, 0);
	SetWindowCondition("c_Sort_u1", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_u2", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_v1", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_v2", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_w1", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_w2", -100, 900, 0, 0);
	SetWindowCondition("c_Tof_i_all", 0, 10000, 0, 0);

	x[0] = -65;	y[0] = -65;
	x[1] = -65;	y[1] = 65;
	x[2] = 65;	y[2] = 65;
	x[3] = 65;	y[3] = -65;
	x[4] = -65;	y[4] = -65;
	SetPolyCondition("cXY_i_sort", 5, x, y, 0, 0);

	SetWindowCondition("c_Rad_i", 0, 65, 0, 0);
	SetWindowCondition("c_Y_i_sort", -65, 65, 0, 0);
	SetWindowCondition("c_Tsum_xi", 0, 300, 0, 0);
	SetWindowCondition("c_Tsum_yi", 0, 300, 0, 0);
	SetWindowCondition("c_Sort_xi1", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_xi2", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_yi1", -100, 900, 0, 0);
	SetWindowCondition("c_Sort_yi2", -100, 900, 0, 0);

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

	SetWindowCondition("c_Tof_e0", 0, 500, 0, 0);

	x[0] = -45;	y[0] = -45;
	x[1] = -45;	y[1] = 45;
	x[2] = 45;	y[2] = 45;
	x[3] = 45;	y[3] = -45;
	x[4] = -45;	y[4] = -45;
	SetPolyCondition("c_Pos_e0", 5, x, y, 0, 0);

	SetWindowCondition("c_Rad_e0", 0, 45, 0, 0);
	SetWindowCondition("c_Energy_e0", 0, 200, 0, 0);
	SetWindowCondition("c_AnglePol_e0", 0, 180, 0, 0);
	SetWindowCondition("c_Px_e0", -5, 5, 0, 0);
	SetWindowCondition("c_Py_e0", -5, 5, 0, 0);
	SetWindowCondition("c_Pz_e0", -5, 5, 0, 0);
	SetWindowCondition("c_Tof_e1", 0, 500, 0, 0);

	x[0] = -45;	y[0] = -45;
	x[1] = -45;	y[1] = 45;
	x[2] = 45;	y[2] = 45;
	x[3] = 45;	y[3] = -45;
	x[4] = -45;	y[4] = -45;
	SetPolyCondition("c_Pos_e1", 5, x, y, 0, 0);

	SetWindowCondition("c_Rad_e1", 0, 45, 0, 0);
	SetWindowCondition("c_Energy_e1", 0, 200, 0, 0);
	SetWindowCondition("c_AnglePol_e1", 0, 180, 0, 0);
	SetWindowCondition("c_Px_e1", -5, 5, 0, 0);
	SetWindowCondition("c_Py_e1", -5, 5, 0, 0);
	SetWindowCondition("c_Pz_e1", -5, 5, 0, 0);
	SetWindowCondition("c_Tof_Ne+", 0, 10000, 0, 0);

	x[0] = -65;	y[0] = -65;
	x[1] = -65;	y[1] = 65;
	x[2] = 65;	y[2] = 65;
	x[3] = 65;	y[3] = -65;
	x[4] = -65;	y[4] = -65;
	SetPolyCondition("c_Pos_Ne+", 5, x, y, 0, 0);

	SetWindowCondition("c_Rad_Ne+", 0, 65, 0, 0);
	SetWindowCondition("c_Energy_Ne+", 0, 10, 0, 0);
	SetWindowCondition("c_AnglePol_Ne+", 0, 180, 0, 0);
	SetWindowCondition("c_Px_Ne+", -50, 50, 0, 0);
	SetWindowCondition("c_Py_Ne+", -50, 50, 0, 0);
	SetWindowCondition("c_Pz_Ne+", -50, 50, 0, 0);
	SetWindowCondition("c_Tof_Ne++", 0, 10000, 0, 0);

	x[0] = -65;	y[0] = -65;
	x[1] = -65;	y[1] = 65;
	x[2] = 65;	y[2] = 65;
	x[3] = 65;	y[3] = -65;
	x[4] = -65;	y[4] = -65;
	SetPolyCondition("c_Pos_Ne++", 5, x, y, 0, 0);

	SetWindowCondition("c_Rad_Ne++", 0, 65, 0, 0);
	SetWindowCondition("c_Energy_Ne++", 0, 10, 0, 0);
	SetWindowCondition("c_AnglePol_Ne++", 0, 180, 0, 0);
	SetWindowCondition("c_Px_Ne++", -50, 50, 0, 0);
	SetWindowCondition("c_Py_Ne++", -50, 50, 0, 0);
	SetWindowCondition("c_Pz_Ne++", -50, 50, 0, 0);
	SetWindowCondition("c_Tof_NeCoul+", 0, 10000, 0, 0);

	x[0] = -65;	y[0] = -65;
	x[1] = -65;	y[1] = 65;
	x[2] = 65;	y[2] = 65;
	x[3] = 65;	y[3] = -65;
	x[4] = -65;	y[4] = -65;
	SetPolyCondition("c_Pos_NeCoul+", 5, x, y, 0, 0);

	SetWindowCondition("c_Rad_NeCoul+", 0, 65, 0, 0);
	SetWindowCondition("c_Energy_NeCoul+", 0, 10, 0, 0);
	SetWindowCondition("c_AnglePol_NeCoul+", 0, 180, 0, 0);
	SetWindowCondition("c_Px_NeCoul+", -50, 50, 0, 0);
	SetWindowCondition("c_Py_NeCoul+", -50, 50, 0, 0);
	SetWindowCondition("c_Pz_NeCoul+", -50, 50, 0, 0);
	SetWindowCondition("c_Tof_N+", 0, 10000, 0, 0);

	x[0] = -65;	y[0] = -65;
	x[1] = -65;	y[1] = 65;
	x[2] = 65;	y[2] = 65;
	x[3] = 65;	y[3] = -65;
	x[4] = -65;	y[4] = -65;
	SetPolyCondition("c_Pos_N+", 5, x, y, 0, 0);

	SetWindowCondition("c_Rad_N+", 0, 65, 0, 0);
	SetWindowCondition("c_Energy_N+", 0, 10, 0, 0);
	SetWindowCondition("c_AnglePol_N+", 0, 180, 0, 0);
	SetWindowCondition("c_Px_N+", -50, 50, 0, 0);
	SetWindowCondition("c_Py_N+", -50, 50, 0, 0);
	SetWindowCondition("c_Pz_N+", -50, 50, 0, 0);
	SetWindowCondition("c_Tof_i4", 0, 10000, 0, 0);

	x[0] = -65;	y[0] = -65;
	x[1] = -65;	y[1] = 65;
	x[2] = 65;	y[2] = 65;
	x[3] = 65;	y[3] = -65;
	x[4] = -65;	y[4] = -65;
	SetPolyCondition("c_Pos_i4", 5, x, y, 0, 0);

	SetWindowCondition("c_Rad_i4", 0, 65, 0, 0);
	SetWindowCondition("c_Energy_i4", 0, 10, 0, 0);
	SetWindowCondition("c_AnglePol_i4", 0, 180, 0, 0);
	SetWindowCondition("c_Px_i4", -50, 50, 0, 0);
	SetWindowCondition("c_Py_i4", -50, 50, 0, 0);
	SetWindowCondition("c_Pz_i4", -50, 50, 0, 0);
	SetWindowCondition("c_Tof_i5", 0, 10000, 0, 0);

	x[0] = -65;	y[0] = -65;
	x[1] = -65;	y[1] = 65;
	x[2] = 65;	y[2] = 65;
	x[3] = 65;	y[3] = -65;
	x[4] = -65;	y[4] = -65;
	SetPolyCondition("c_Pos_i5", 5, x, y, 0, 0);

	SetWindowCondition("c_Rad_i5", 0, 65, 0, 0);
	SetWindowCondition("c_Energy_i5", 0, 10, 0, 0);
	SetWindowCondition("c_AnglePol_i5", 0, 180, 0, 0);
	SetWindowCondition("c_Px_i5", -50, 50, 0, 0);
	SetWindowCondition("c_Py_i5", -50, 50, 0, 0);
	SetWindowCondition("c_Pz_i5", -50, 50, 0, 0);
	SetWindowCondition("c_PxSum_NeCoulNeCoul", -10, 10, 0, 0);
	SetWindowCondition("c_PySum_NeCoulNeCoul", -10, 10, 0, 0);
	SetWindowCondition("c_PzSum_NeCoulNeCoul", -10, 10, 0, 0);
	SetWindowCondition("c_Ker_NeCoulNeCoul", -0, 100, 0, 0);
	SetWindowCondition("c_PxSum_Nee", -10, 10, 0, 0);
	SetWindowCondition("c_PySum_Nee", -10, 10, 0, 0);
	SetWindowCondition("c_PzSum_Nee", -10, 10, 0, 0);
	SetWindowCondition("c_Ker_Nee", -0, 100, 0, 0);
	SetWindowCondition("c_PxSum_NN", -10, 10, 0, 0);
	SetWindowCondition("c_PySum_NN", -10, 10, 0, 0);
	SetWindowCondition("c_PzSum_NN", -10, 10, 0, 0);
	SetWindowCondition("c_Ker_NN", -0, 100, 0, 0);
	SetWindowCondition("c_PxSum_Ne", -10, 10, 0, 0);
	SetWindowCondition("c_PySum_Ne", -10, 10, 0, 0);
	SetWindowCondition("c_PzSum_Ne", -10, 10, 0, 0);
	SetWindowCondition("c_Ker_Ne", -0, 100, 0, 0);
	SetWindowCondition("c_PxSum_Coin4", -10, 10, 0, 0);
	SetWindowCondition("c_PySum_Coin4", -10, 10, 0, 0);
	SetWindowCondition("c_PzSum_Coin4", -10, 10, 0, 0);
	SetWindowCondition("c_Ker_Coin4", -0, 100, 0, 0);
	SetWindowCondition("c_PxSum_Coin5", -10, 10, 0, 0);
	SetWindowCondition("c_PySum_Coin5", -10, 10, 0, 0);
	SetWindowCondition("c_PzSum_Coin5", -10, 10, 0, 0);
	SetWindowCondition("c_Ker_Coin5", -0, 100, 0, 0);

	SetWindowConditionInArray("c_Intensity", 0, 0, 100, 0, 0);
	SetWindowConditionInArray("c_Intensity", 1, 0, 100, 0, 0);
	SetWindowConditionInArray("c_Intensity", 2, 0, 100, 0, 0);
	SetWindowConditionInArray("c_Intensity", 3, 0, 100, 0, 0);
	SetWindowConditionInArray("c_Intensity", 4, 0, 100, 0, 0);


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
	
	DataMembers = OpenParameter ("Par Chan 9 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "30");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 9 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 10 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "30");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 10 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 11 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "30");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 11 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 12 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "30");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 12 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 13 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "30");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 13 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 14 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "30");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 14 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par Chan 15 Acq Tdc CFD");
		SetParameterMember (DataMembers, "Threshold", "30");
		SetParameterMember (DataMembers, "Delay", "4");
		SetParameterMember (DataMembers, "Fraction", "0.5");
		SetParameterMember (DataMembers, "Walk", "0");
	SaveParameter (DataMembers, "Par Chan 15 Acq Tdc CFD");
	
	DataMembers = OpenParameter ("Par unpack global");
		SetParameterMember (DataMembers, "to", "0");
	SaveParameter (DataMembers, "Par unpack global");
	
	DataMembers = OpenParameter ("Par e hexanode");
		SetParameterMember (DataMembers, "scaleU", "1");
		SetParameterMember (DataMembers, "scaleV", "0.97");
		SetParameterMember (DataMembers, "scaleW", "0.95");
		SetParameterMember (DataMembers, "shiftW", "-2");
		SetParameterMember (DataMembers, "distancemax", "1");
		SetParameterMember (DataMembers, "xo", "0");
		SetParameterMember (DataMembers, "yo", "0");
		SetParameterMember (DataMembers, "phi", "0");
		SetParameterMember (DataMembers, "size_raw", "9850");
		SetParameterMember (DataMembers, "size_mm", "80");
	SaveParameter (DataMembers, "Par e hexanode");
	
	DataMembers = OpenParameter ("Par i delayline");
		SetParameterMember (DataMembers, "scaleX", "1");
		SetParameterMember (DataMembers, "scaleY", "1");
		SetParameterMember (DataMembers, "xo", "0");
		SetParameterMember (DataMembers, "yo", "0");
		SetParameterMember (DataMembers, "phi", "0");
		SetParameterMember (DataMembers, "size_raw", "5600");
		SetParameterMember (DataMembers, "size_mm", "80");
	SaveParameter (DataMembers, "Par i delayline");
	
	DataMembers = OpenParameter ("Par i hexanode");
		SetParameterMember (DataMembers, "scaleU", "1");
		SetParameterMember (DataMembers, "scaleV", "0.97");
		SetParameterMember (DataMembers, "scaleW", "0.95");
		SetParameterMember (DataMembers, "shiftW", "-2");
		SetParameterMember (DataMembers, "distancemax", "1");
		SetParameterMember (DataMembers, "xo", "0");
		SetParameterMember (DataMembers, "yo", "0");
		SetParameterMember (DataMembers, "phi", "0");
		SetParameterMember (DataMembers, "size_raw", "9850");
		SetParameterMember (DataMembers, "size_mm", "80");
	SaveParameter (DataMembers, "Par i hexanode");
	
	DataMembers = OpenParameter ("Par Delay");
		SetParameterMember (DataMembers, "Fullrange_raw", "1000");
		SetParameterMember (DataMembers, "Fullrange_fs", "1000");
		SetParameterMember (DataMembers, "Offset", "0");
	SaveParameter (DataMembers, "Par Delay");
	
	DataMembers = OpenParameter ("Par e0 momentum");
		SetParameterMember (DataMembers, "scaleX", "1");
		SetParameterMember (DataMembers, "scaleY", "1");
		SetParameterMember (DataMembers, "scaleT", "1");
		SetParameterMember (DataMembers, "shiftX", "0");
		SetParameterMember (DataMembers, "shiftY", "0");
		SetParameterMember (DataMembers, "shiftT", "0");
		SetParameterMember (DataMembers, "lincorshiftX", "0");
		SetParameterMember (DataMembers, "lincorshiftY", "0");
		SetParameterMember (DataMembers, "scalePx", "1");
		SetParameterMember (DataMembers, "scalePy", "1");
		SetParameterMember (DataMembers, "scalePz", "1");
		SetParameterMember (DataMembers, "rotatePhi", "0");
		SetParameterMember (DataMembers, "shiftVx", "0");
		SetParameterMember (DataMembers, "shiftVy", "0");
		SetParameterMember (DataMembers, "scaleEn", "1");
		SetParameterMember (DataMembers, "MagneticField", "0");
		SetParameterMember (DataMembers, "u", "20");
		SetParameterMember (DataMembers, "accel", "0.11");
		SetParameterMember (DataMembers, "drift", "0");
		SetParameterMember (DataMembers, "NewtonPo", "2");
		SetParameterMember (DataMembers, "NewtonEpsilon", "0.0001");
		SetParameterMember (DataMembers, "NewtonMaxI", "20");
	SaveParameter (DataMembers, "Par e0 momentum");
	
	DataMembers = OpenParameter ("Par e1 momentum");
		SetParameterMember (DataMembers, "scaleX", "1");
		SetParameterMember (DataMembers, "scaleY", "1");
		SetParameterMember (DataMembers, "scaleT", "1");
		SetParameterMember (DataMembers, "shiftX", "0");
		SetParameterMember (DataMembers, "shiftY", "0");
		SetParameterMember (DataMembers, "shiftT", "0");
		SetParameterMember (DataMembers, "lincorshiftX", "0");
		SetParameterMember (DataMembers, "lincorshiftY", "0");
		SetParameterMember (DataMembers, "scalePx", "1");
		SetParameterMember (DataMembers, "scalePy", "1");
		SetParameterMember (DataMembers, "scalePz", "1");
		SetParameterMember (DataMembers, "rotatePhi", "0");
		SetParameterMember (DataMembers, "shiftVx", "0");
		SetParameterMember (DataMembers, "shiftVy", "0");
		SetParameterMember (DataMembers, "scaleEn", "1");
		SetParameterMember (DataMembers, "MagneticField", "0");
		SetParameterMember (DataMembers, "u", "20");
		SetParameterMember (DataMembers, "accel", "0.11");
		SetParameterMember (DataMembers, "drift", "0");
		SetParameterMember (DataMembers, "NewtonPo", "2");
		SetParameterMember (DataMembers, "NewtonEpsilon", "0.0001");
		SetParameterMember (DataMembers, "NewtonMaxI", "20");
	SaveParameter (DataMembers, "Par e1 momentum");
	
	DataMembers = OpenParameter ("Par Ne+ momentum");
		SetParameterMember (DataMembers, "scaleX", "1");
		SetParameterMember (DataMembers, "scaleY", "1");
		SetParameterMember (DataMembers, "scaleT", "1");
		SetParameterMember (DataMembers, "shiftX", "0");
		SetParameterMember (DataMembers, "shiftY", "0");
		SetParameterMember (DataMembers, "shiftT", "0");
		SetParameterMember (DataMembers, "lincorshiftX", "0");
		SetParameterMember (DataMembers, "lincorshiftY", "0");
		SetParameterMember (DataMembers, "scalePx", "1");
		SetParameterMember (DataMembers, "scalePy", "1");
		SetParameterMember (DataMembers, "scalePz", "1");
		SetParameterMember (DataMembers, "rotatePhi", "0");
		SetParameterMember (DataMembers, "shiftVx", "0");
		SetParameterMember (DataMembers, "shiftVy", "0");
		SetParameterMember (DataMembers, "scaleEn", "1");
		SetParameterMember (DataMembers, "MagneticField", "0");
		SetParameterMember (DataMembers, "u", "20");
		SetParameterMember (DataMembers, "accel", "0.11");
		SetParameterMember (DataMembers, "drift", "0");
		SetParameterMember (DataMembers, "NewtonPo", "2");
		SetParameterMember (DataMembers, "NewtonEpsilon", "0.0001");
		SetParameterMember (DataMembers, "NewtonMaxI", "20");
	SaveParameter (DataMembers, "Par Ne+ momentum");
	
	DataMembers = OpenParameter ("Par Ne++ momentum");
		SetParameterMember (DataMembers, "scaleX", "1");
		SetParameterMember (DataMembers, "scaleY", "1");
		SetParameterMember (DataMembers, "scaleT", "1");
		SetParameterMember (DataMembers, "shiftX", "0");
		SetParameterMember (DataMembers, "shiftY", "0");
		SetParameterMember (DataMembers, "shiftT", "0");
		SetParameterMember (DataMembers, "lincorshiftX", "0");
		SetParameterMember (DataMembers, "lincorshiftY", "0");
		SetParameterMember (DataMembers, "scalePx", "1");
		SetParameterMember (DataMembers, "scalePy", "1");
		SetParameterMember (DataMembers, "scalePz", "1");
		SetParameterMember (DataMembers, "rotatePhi", "0");
		SetParameterMember (DataMembers, "shiftVx", "0");
		SetParameterMember (DataMembers, "shiftVy", "0");
		SetParameterMember (DataMembers, "scaleEn", "1");
		SetParameterMember (DataMembers, "MagneticField", "0");
		SetParameterMember (DataMembers, "u", "20");
		SetParameterMember (DataMembers, "accel", "0.11");
		SetParameterMember (DataMembers, "drift", "0");
		SetParameterMember (DataMembers, "NewtonPo", "2");
		SetParameterMember (DataMembers, "NewtonEpsilon", "0.0001");
		SetParameterMember (DataMembers, "NewtonMaxI", "20");
	SaveParameter (DataMembers, "Par Ne++ momentum");
	
	DataMembers = OpenParameter ("Par NeCoul+ momentum");
		SetParameterMember (DataMembers, "scaleX", "1");
		SetParameterMember (DataMembers, "scaleY", "1");
		SetParameterMember (DataMembers, "scaleT", "1");
		SetParameterMember (DataMembers, "shiftX", "0");
		SetParameterMember (DataMembers, "shiftY", "0");
		SetParameterMember (DataMembers, "shiftT", "0");
		SetParameterMember (DataMembers, "lincorshiftX", "0");
		SetParameterMember (DataMembers, "lincorshiftY", "0");
		SetParameterMember (DataMembers, "scalePx", "1");
		SetParameterMember (DataMembers, "scalePy", "1");
		SetParameterMember (DataMembers, "scalePz", "1");
		SetParameterMember (DataMembers, "rotatePhi", "0");
		SetParameterMember (DataMembers, "shiftVx", "0");
		SetParameterMember (DataMembers, "shiftVy", "0");
		SetParameterMember (DataMembers, "scaleEn", "1");
		SetParameterMember (DataMembers, "MagneticField", "0");
		SetParameterMember (DataMembers, "u", "20");
		SetParameterMember (DataMembers, "accel", "0.11");
		SetParameterMember (DataMembers, "drift", "0");
		SetParameterMember (DataMembers, "NewtonPo", "2");
		SetParameterMember (DataMembers, "NewtonEpsilon", "0.0001");
		SetParameterMember (DataMembers, "NewtonMaxI", "20");
	SaveParameter (DataMembers, "Par NeCoul+ momentum");
	
	DataMembers = OpenParameter ("Par N+ momentum");
		SetParameterMember (DataMembers, "scaleX", "1");
		SetParameterMember (DataMembers, "scaleY", "1");
		SetParameterMember (DataMembers, "scaleT", "1");
		SetParameterMember (DataMembers, "shiftX", "0");
		SetParameterMember (DataMembers, "shiftY", "0");
		SetParameterMember (DataMembers, "shiftT", "0");
		SetParameterMember (DataMembers, "lincorshiftX", "0");
		SetParameterMember (DataMembers, "lincorshiftY", "0");
		SetParameterMember (DataMembers, "scalePx", "1");
		SetParameterMember (DataMembers, "scalePy", "1");
		SetParameterMember (DataMembers, "scalePz", "1");
		SetParameterMember (DataMembers, "rotatePhi", "0");
		SetParameterMember (DataMembers, "shiftVx", "0");
		SetParameterMember (DataMembers, "shiftVy", "0");
		SetParameterMember (DataMembers, "scaleEn", "1");
		SetParameterMember (DataMembers, "MagneticField", "0");
		SetParameterMember (DataMembers, "u", "20");
		SetParameterMember (DataMembers, "accel", "0.11");
		SetParameterMember (DataMembers, "drift", "0");
		SetParameterMember (DataMembers, "NewtonPo", "2");
		SetParameterMember (DataMembers, "NewtonEpsilon", "0.0001");
		SetParameterMember (DataMembers, "NewtonMaxI", "20");
	SaveParameter (DataMembers, "Par N+ momentum");
	
	DataMembers = OpenParameter ("Par i4 momentum");
		SetParameterMember (DataMembers, "scaleX", "1");
		SetParameterMember (DataMembers, "scaleY", "1");
		SetParameterMember (DataMembers, "scaleT", "1");
		SetParameterMember (DataMembers, "shiftX", "0");
		SetParameterMember (DataMembers, "shiftY", "0");
		SetParameterMember (DataMembers, "shiftT", "0");
		SetParameterMember (DataMembers, "lincorshiftX", "0");
		SetParameterMember (DataMembers, "lincorshiftY", "0");
		SetParameterMember (DataMembers, "scalePx", "1");
		SetParameterMember (DataMembers, "scalePy", "1");
		SetParameterMember (DataMembers, "scalePz", "1");
		SetParameterMember (DataMembers, "rotatePhi", "0");
		SetParameterMember (DataMembers, "shiftVx", "0");
		SetParameterMember (DataMembers, "shiftVy", "0");
		SetParameterMember (DataMembers, "scaleEn", "1");
		SetParameterMember (DataMembers, "MagneticField", "0");
		SetParameterMember (DataMembers, "u", "20");
		SetParameterMember (DataMembers, "accel", "0.11");
		SetParameterMember (DataMembers, "drift", "0");
		SetParameterMember (DataMembers, "NewtonPo", "2");
		SetParameterMember (DataMembers, "NewtonEpsilon", "0.0001");
		SetParameterMember (DataMembers, "NewtonMaxI", "20");
	SaveParameter (DataMembers, "Par i4 momentum");
	
	DataMembers = OpenParameter ("Par i5 momentum");
		SetParameterMember (DataMembers, "scaleX", "1");
		SetParameterMember (DataMembers, "scaleY", "1");
		SetParameterMember (DataMembers, "scaleT", "1");
		SetParameterMember (DataMembers, "shiftX", "0");
		SetParameterMember (DataMembers, "shiftY", "0");
		SetParameterMember (DataMembers, "shiftT", "0");
		SetParameterMember (DataMembers, "lincorshiftX", "0");
		SetParameterMember (DataMembers, "lincorshiftY", "0");
		SetParameterMember (DataMembers, "scalePx", "1");
		SetParameterMember (DataMembers, "scalePy", "1");
		SetParameterMember (DataMembers, "scalePz", "1");
		SetParameterMember (DataMembers, "rotatePhi", "0");
		SetParameterMember (DataMembers, "shiftVx", "0");
		SetParameterMember (DataMembers, "shiftVy", "0");
		SetParameterMember (DataMembers, "scaleEn", "1");
		SetParameterMember (DataMembers, "MagneticField", "0");
		SetParameterMember (DataMembers, "u", "20");
		SetParameterMember (DataMembers, "accel", "0.11");
		SetParameterMember (DataMembers, "drift", "0");
		SetParameterMember (DataMembers, "NewtonPo", "2");
		SetParameterMember (DataMembers, "NewtonEpsilon", "0.0001");
		SetParameterMember (DataMembers, "NewtonMaxI", "20");
	SaveParameter (DataMembers, "Par i5 momentum");
	
	DataMembers = OpenParameter ("Par Delay Shift");
		SetParameterMember (DataMembers, "Value", "0");
	SaveParameter (DataMembers, "Par Delay Shift");
	
	DataMembers = OpenParameter ("Par finish");
		SetParameterMember (DataMembers, "P_Proj", "0");
	SaveParameter (DataMembers, "Par finish");
	
	return kTRUE;
}

