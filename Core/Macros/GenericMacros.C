//--This file contains macros that were written by Achim Oliver Czasch & Lutz Foucar--//
#include <Rtypes.h>
//#include "../TRemiIncludeFunctions.inline.h"

void help()
{
	cout<<"******Root macros written for use with the GENERiC code within Go4******"<<endl;
	cout<<endl;
	cout<<endl;
/*	cout<<" 1.)  circle(center=0) (draws a circle with center)"<<endl;
	cout<<" 2.)  prox(Nbr+-Bins=-1) (if nbr+-bins is >-1 then it draws the"<<endl;
	cout<<"              projection in a separat canvas online with +- nbrbins"<<endl;
	cout<<"              around the mouse)"<<endl;*/
// 	cout<<" 3.)  proy(Mbr+-Bins=-1) (same thing here for projection to Y)"<<endl;
	cout<<" 1.)  int1D(); int2D()        (gives U the nbr of entries in the selected Area)"<<endl;
	cout<<" 2.)  ratio1D(hist, xmin1, xmax1, xmin2, xmax2) (gives the ratio between two" <<endl;
	cout<<"               integrals in the same histogram" << endl;
	cout<<" 3.)  findMaximum(hist, max=0)   (find the maximum in hist)"<<endl; 
	cout<<" 4.)  findMinimum(hist, min=0)   (find the minimum in hist)"<<endl;
	cout<<endl;
}


////////////////////////////findmaximum/////////////////////////////////////////////////////////////
void findMaximum(const char * histname)
{
	Bool_t reset = kTRUE;
	TH1* hist;
	while ((hist = dynamic_cast<TH1*> (go4->NextMatchingObject(histname, "Histograms", reset)) ))
	{
		reset = kFALSE;
		TString maxpostext;
		if (hist->GetDimension() == 1)
			maxpostext.Form("x = %8.2f", hist->GetBinCenter(hist->GetMaximumBin()) );
		else if (hist->GetDimension() == 2)
		{
			Int_t x,y,z;
			hist->GetBinXYZ(hist->GetMaximumBin(), x, y, z);
			maxpostext.Form("x = %8.2f, y = %8.2f", hist->GetXaxis()->GetBinCenter(x), hist->GetYaxis()->GetBinCenter(y) );
		}
		std::cout << "Histogram " << hist->GetName() << " has maximum value of " << hist->GetBinContent(hist->GetMaximumBin()) << " at position " << maxpostext << std::endl;
	}
	if (reset) 
		std::cout << "Histogram " << histname << " not found!" << std::endl;
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////findminimum/////////////////////////////////////////////////////////////
void findMinimum(const char * histname)
{
	Bool_t reset = kTRUE;
	TH1* hist;
	while ((hist = dynamic_cast<TH1*> (go4->NextMatchingObject(histname, "Histograms", reset)) ))
	{
		reset = kFALSE;
		TString minposstext;
		if (hist->GetDimension() == 1)
			minposstext.Form("x = %8.2f", hist->GetBinCenter(hist->GetMinimumBin()) );
		else if (hist->GetDimension() == 2)
		{
			Int_t x,y,z;
			hist->GetBinXYZ(hist->GetMinimumBin(), x, y, z);
			minposstext.Form("x = %8.2f, y = %8.2f", hist->GetXaxis()->GetBinCenter(x), hist->GetYaxis()->GetBinCenter(y) );
		}
		std::cout << "Histogram " << hist->GetName() << " has minimum value of " << hist->GetBinContent(hist->GetMinimumBin()) << " at position " << minposstext << std::endl;
	}
	if (reset) 
		std::cout << "Histogram " << histname << " not found!" << std::endl;
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////


////// Integral 1D
void int1D (const char * histname, Double_t xmin, Double_t xmax)
{
	Bool_t reset = kTRUE;
	TH1* hist;
	while ((hist = dynamic_cast<TH1*> (go4->NextMatchingObject(histname, "Histograms", reset)) ))
	{
		reset = kFALSE;
		if (hist->GetDimension() == 1)
		{
			Int_t xbin1 = hist->FindFixBin(xmin);
			Int_t xbin2 = hist->FindFixBin(xmax);
			std::cout << "Histogram " << hist->GetName() << " from " << hist->GetBinLowEdge(xbin1) << " to " << hist->GetBinLowEdge(xbin2) + hist->GetBinWidth(xbin2) << " has integral " << hist->Integral(xbin1, xbin2) << std::endl;
		}
	
	}
	if (reset) 
		std::cout << "Histogram " << histname << " not found!" << std::endl;
	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////// Integral 2D
void int2D (const char * histname, Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax)
{
	Bool_t reset = kTRUE;
	TH1* hist;
	while ((hist = dynamic_cast<TH1*> (go4->NextMatchingObject(histname, "Histograms", reset)) ))
	{
		reset = kFALSE;
		if (hist->GetDimension() == 2)
		{
			TH2* hist2 = dynamic_cast<TH2*>(hist);
			Int_t x1, y1, z1, x2, y2, z2;
			hist->GetBinXYZ(hist->FindFixBin(xmin, ymin), x1, y1, z1);
			hist->GetBinXYZ(hist->FindFixBin(xmax, ymax), x2, y2, z2);
			std::cout << "Histogram " << hist->GetName() << " from (x) " << hist->GetBinLowEdge(x1) << " to " << hist->GetBinLowEdge(x2) + hist->GetBinWidth(x2) << ", (y) " << hist->GetBinLowEdge(y1) << " to " << hist->GetBinLowEdge(y2) + hist->GetBinWidth(y2) << " has integral " << hist2->Integral(x1, x2, y1, y2) << std::endl;
		}
	
	}
	if (reset) 
		std::cout << "Histogram " << histname << " not found!" << std::endl;
	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ratio1D(const char* histname, Double_t xmin1, Double_t xmax1, Double_t xmin2, Double_t xmax2)
{
	Bool_t reset = kTRUE;
	TH1* hist;
	while ((hist = dynamic_cast<TH1*> (go4->NextMatchingObject(histname, "Histograms", reset)) ))
	{
		reset = kFALSE;
		if (hist->GetDimension() == 1)
		{
			Int_t xminbin1 = hist->FindFixBin(xmin1);
			Int_t xmaxbin1 = hist->FindFixBin(xmax1);
			Int_t xminbin2 = hist->FindFixBin(xmin2);
			Int_t xmaxbin2 = hist->FindFixBin(xmax2);
			Double_t integral1 = hist->Integral(xminbin1, xmaxbin1);
			Double_t integral2 = hist->Integral(xminbin2, xmaxbin2);	
			if (!integral1)
				integral1 = 1e-4;
			if (!integral2)
				integral2 = 1e-4;
			std::cout << "Histogram " << hist->GetName() << " from " << hist->GetBinLowEdge(xminbin1) << " to " << hist->GetBinLowEdge(xmaxbin1) + hist->GetBinWidth(xmaxbin1) << " has integral " << integral1 << std::endl << "\tand from " << hist->GetBinLowEdge(xminbin2) << " to " << hist->GetBinLowEdge(xmaxbin2) + hist->GetBinWidth(xmaxbin2) << " has integral " << integral2 << std::endl << "\tThe ratio is " << integral1/integral2 << " +/- " << integral1/integral2 * TMath::Sqrt(1/integral1 + 1/integral2 )  << std::endl;
		}
		
	}
	if (reset) 
		std::cout << "Histogram " << histname << " not found!" << std::endl;
	return;
}


void duplicateHist(const char* histname)
{
	Bool_t reset = kTRUE;
	TH1* hist;
	while ((hist = dynamic_cast<TH1*> (go4->NextMatchingObject(histname, "Histograms", reset)) ))
	{
		TString duplicateName = TString(hist->GetName()) + "Dup";
		TH1* duplicate = dynamic_cast<TH1*>(hist->Clone(duplicateName));
		go4->AddHistogram(duplicate);
		reset = kFALSE;
	}
	if (reset) 
		std::cout << "Histogram " << histname << " not found!" << std::endl;
	return;
}


/////////////////////////projection to X/////////////////////////////////////
//TH1* prox(double fr, double to, TPad * pad = 0)
//{
//	TH2 * hist2=0;
//	
//	//find histogram in pad//
//	TIter next(gPad->GetListOfPrimitives());
//	TObject * obj;
//	while((obj = next()))
//	{
//		if (obj->InheritsFrom("TH2"))
//		{
//			hist2 = (TH2*) obj;
//			break;
//		}
//	}
//	if (!hist2){cout <<"No 2D-Histogram in Pad"<<endl; return 0;}

//	return (prox(hist2,fr,to,pad));
//}
//TH1 * prox(TH2* hist2, double fr, double to, TPad *pad = 0, bool d=true)
//{
//	static int count;
//	char title[80];

//	int biny1 = hist2->GetYaxis()->FindBin(fr);
//	int biny2 = hist2->GetYaxis()->FindBin(to);
//	
//	int binymin = (biny1 >= biny2)? biny2:biny1;
//	int binymax = (biny1 <= biny2)? biny2:biny1;
//	
//	TH1D * pro = hist2->ProjectionX("",binymin,binymax);
//	double ymin = hist2->GetYaxis()->GetBinLowEdge(binymin);
//	double ymax = hist2->GetYaxis()->GetBinUpEdge(binymax);
//	
//	sprintf(title,"Pro to X from y=%f to %f",ymin,ymax);
//	pro->SetTitle(title);
//	sprintf(title,"projectx_%s%i",hist2->GetName(),count++);
//	pro->SetName(title);
//	if (d)
//	{
//		if (!pad)
//		{
//			TCanvas * c2 = Canv(1,1);
//			c2->cd();
//			pad = gPad;
//		}
//		pad->cd();
//		pro->Draw();
//	}
//	
//	return ((TH1*)pro);
//}
class ProX
{
RQ_OBJECT("ProX")

private:
	Float_t xmin;
	Float_t xmax;
	Float_t remy;
	Float_t remy1;
	Float_t remy2;
	Float_t ymin;
	Float_t ymax;
	Float_t uxmin;
	Float_t uxmax;
	Float_t y;
	double y2;
	double y1;
	int binymin;
	int binymax;
	int py1;
	int py2;
	char title[80];
	int biny1;
	int biny2;
	int pmbins;
	TCanvas *c2;
	TH1D * pro;

public:
	ProX(int PMBins) {pmbins=PMBins;xmin=0;xmax=0;remy=0;remy1=0;remy2=0;biny1=0;biny2=0;c2=0;
					 ymin=0;ymax=0;uxmax=0;uxmin=0;y=0;binymin=0;binymax=0;py1=0;py2=0;pro=0;}
	~ProX() {}

	void ExecuteProx(Int_t event, Int_t px, Int_t py, TObject *sel);
};

//void ProX::ExecuteProx(Int_t event, Int_t px, Int_t py, TObject *sel)
//{
//	//--if selected is not the 2d histo remove the lines & return--//
//	if(!sel->InheritsFrom("TH2"))
//	{
//		if (remy)
//			gVirtualX->DrawLine(xmin,remy,xmax,remy);
//		if (remy1 && remy2)
//		{
//			gVirtualX->DrawLine(xmin,remy1,xmax,remy1);
//			gVirtualX->DrawLine(xmin,remy2,xmax,remy2);
//		}

//		remy=0;
//		remy1=0;
//		remy2=0;
//		return;
//	}

//	TCanvas * c = (TCanvas *) gTQSender;
//	TPad * pad = (TPad *) c->GetSelectedPad();
//	TH2 * hist2 = (TH2 *) sel;

//	
//	if (pmbins== -1)
//	{
//	   if ( event == kButton1Double)
//	   {
//			//--quit--//
//			c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
//			delete this;
//	   }
//	   else if ( event == kButton1Down )
//	   {
//			// Get xmin,xmax// 
//			uxmin = pad->GetUxmin();
//			uxmax = pad->GetUxmax();
//			xmin = pad->XtoAbsPixel(uxmin);
//			xmax = pad->XtoAbsPixel(uxmax);
//			
//			//--get y--//
//			y1 = pad->AbsPixeltoY(py);
//	
//			pad->GetCanvas()->FeedbackMode(kTRUE);
//			gVirtualX->DrawLine(xmin,py,xmax,py);
//	   }
//	   else if ( event == kButton1Up )
//	   {	
//		   	//--get y--//     
//			y2 = pad->AbsPixeltoY(py);
//			
//			prox(y1,y2);

//		   	// end program here //
//	    	c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
//			delete this;
//	   }
//	   else if ( event == kButton1Motion )
//	   {
//			//--delete if drawn before--//
//			pad->GetCanvas()->FeedbackMode(kTRUE);
//			if (remy)
//				gVirtualX->DrawLine(xmin,remy,xmax,remy);
//				
//			gVirtualX->DrawLine(xmin,py,xmax,py);
//			remy=py;
//		}
//	
//	}
//	else
//	{
//		if ( event == kMouseMotion )
//		{
//			//--check if we are inside the histo--//
//			
//			// Get xmin,xmax// 
//			uxmin = pad->GetUxmin();
//			uxmax = pad->GetUxmax();
//			xmin = pad->XtoAbsPixel(uxmin);
//			xmax = pad->XtoAbsPixel(uxmax);
//			
//			//--get biny--//
//			y = pad->AbsPixeltoY(py);
//			biny1 = hist2->GetYaxis()->FindBin(y);
//	
//			
//			binymin = biny1 - pmbins;
//	    	binymax = biny1 + pmbins;
//		   	ymin = hist2->GetYaxis()->GetBinLowEdge(binymin);
//		   	ymax = hist2->GetYaxis()->GetBinUpEdge(binymax);
//			py1 = pad->YtoAbsPixel(ymin);
//			py2 = pad->YtoAbsPixel(ymax);
//	    	
//	    	
//		   	pro = hist2->ProjectionX("",binymin,binymax);
//		   	
//		   	if (!c2)
//		   	{
//			   	c2 = Canv(1,1);
//				c2->cd();
//				pro->Draw();
//	   		}
//	   		c2->Modified();
//			c2->Update();
//			
//			pad->GetCanvas()->FeedbackMode(kTRUE);
//			if (remy1 && remy2)
//			{
//				gVirtualX->DrawLine(xmin,remy1,xmax,remy1);
//				gVirtualX->DrawLine(xmin,remy2,xmax,remy2);
//			}
//			gVirtualX->DrawLine(xmin,py1,xmax,py1);
//			gVirtualX->DrawLine(xmin,py2,xmax,py2);
//	
//			remy1 = py1;
//			remy2 = py2;
//		}
//		else if ( event == kButton1Down)
//		{
//		   	sprintf(title,"Pro to X from y=%f to %f",ymin,ymax);
//			pro->SetTitle(title);
//			static int count;
//			sprintf(title,"projectx_%s%i",hist2->GetName(),count++);
//			pro->SetName(title);
//			c2->ToggleEventStatus();
//			c2->Modified();
//			c2->Update();

//			//--quit--//
//			c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
//			delete this;
//		}
//	}
//}

void prox(int pmbins=-1)
{
	ProX * z = new ProX(pmbins);
	TCanvas *c1 = gPad->GetCanvas();
	c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "ProX",z,"ExecuteProx(Int_t,Int_t,Int_t,TObject*)");
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




















//////////////////////////////////projection to y/////////////////////////////////////////////////////
//TH1* proy(double fr, double to, TPad * pad = 0)
//{
//	TH2* hist2=0;
//	
//	//find histogram in pad//
//	TIter next(gPad->GetListOfPrimitives());
//	TObject *obj;
//	while((obj = next()))
//	{
//		if (obj->InheritsFrom("TH2"))
//		{
//			hist2 = (TH2*) obj;
//			break;
//		}
//	}
//	if (!hist2){cout <<"No 2D-Histogram in Pad"<<endl; return 0;}

//	return (proy(hist2,fr,to,pad));
//}

//TH1* proy(TH2 * hist2, double fr, double to, TPad * pad = 0)
//{
//	static int count;
//	char title[80];

//	int binx1 = hist2->GetXaxis()->FindBin(fr);
//	int binx2 = hist2->GetXaxis()->FindBin(to);
//	
//	int binxmin = (binx1 >= binx2)? binx2:binx1;
//	int binxmax = (binx1 <= binx2)? binx2:binx1;
//	double xmin = hist2->GetXaxis()->GetBinLowEdge(binxmin);
//	double xmax = hist2->GetXaxis()->GetBinUpEdge(binxmax);
//	
//	TH1D * pro = hist2->ProjectionY("",binxmin,binxmax);
//	
//	sprintf(title,"Pro to Y from x=%f to %f",xmin,xmax);
//	pro->SetTitle(title);
//	sprintf(title,"projecty_%s%i",hist2->GetName(),count++);
//	pro->SetName(title);
//	if (!pad)
//	{
//		TCanvas * c2 = Canv(1,1);
//		c2->cd();
//		pad = gPad;
//	}
//	pad->cd();
//	pro->Draw();
//	

//	return ((TH1*) pro);
//}


class ProY
{
RQ_OBJECT("ProY")

private:
	Float_t xmin;
	Float_t xmax;
	Float_t ymin;
	Float_t ymax;
	Float_t remx;
	Float_t remx1;
	Float_t remx2;
	Float_t uymin;
	Float_t uymax;
	Float_t x;
	double x1;
	double x2;
	int binxmin;
	int binxmax;
	int px1;
	int px2;
	char title[80];
	int binx1;
	int binx2;
	int pmbins;
	TCanvas *c2;
	TH1D * pro;

public:
	ProY(int PMBins) {pmbins=PMBins;xmin=0;xmax=0;remx=0;remx1=0;remx2=0;binx1=0;binx2=0;c2=0;
					 ymin=0;ymax=0;uymax=0;uymin=0;x=0;binxmin=0;binxmax=0;px1=0;px2=0;pro=0;}
	~ProY() {}

	void ExecuteProy(Int_t event, Int_t px, Int_t py, TObject *sel);
};


//void ProY::ExecuteProy(Int_t event, Int_t px, Int_t py, TObject *sel)
//{
//	//--if selected is not the 2d histo remove the lines & return--//
//	if(!sel->InheritsFrom("TH2"))
//	{
//		if (remx)
//			gVirtualX->DrawLine(remx,ymin,remx,ymax);
//		if (remx1 && remx2)
//		{
//			gVirtualX->DrawLine(remx1,ymin,remx1,ymax);
//			gVirtualX->DrawLine(remx2,ymin,remx2,ymax);
//		}

//		remx=0;
//		remx1=0;
//		remx2=0;
//		return;
//	}

//	TCanvas * c = (TCanvas *) gTQSender;
//	TPad * pad = (TPad *) c->GetSelectedPad();
//	TH2 * hist2 = (TH2 *) sel;

//	
//	if (pmbins== -1)
//	{
//	   if ( event == kButton1Double)
//	   {
//			//--quit--//
//			c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
//			delete this;
//	   }
//	   else if ( event == kButton1Down )
//	   {
//			// Get xmin,xmax// 
//			uymin = pad->GetUymin();
//			uymax = pad->GetUymax();
//			ymin = pad->YtoAbsPixel(uymin);
//			ymax = pad->YtoAbsPixel(uymax);
//			
//			//--get biny--//
//			x1 = pad->AbsPixeltoX(px);

//			pad->GetCanvas()->FeedbackMode(kTRUE);
//			gVirtualX->DrawLine(px,ymin,px,ymax);
//	   }
//	   else if ( event == kButton1Up )
//	   {	
//		   	//--get biny--//     
//			x2 = pad->AbsPixeltoX(px);
//			proy(x1,x2);
//		   	
//		   	// end program here //
//	    	c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
//			delete this;
//	   }
//	   else if ( event == kButton1Motion )
//	   {
//			//--delete if drawn before--//
//			pad->GetCanvas()->FeedbackMode(kTRUE);
//			if (remx)
//				gVirtualX->DrawLine(remx,ymin,remx,ymax);
//				
//			gVirtualX->DrawLine(px,ymin,px,ymax);
//			remx=px;
//		}
//	
//	}
//	else
//	{
//		if ( event == kMouseMotion )
//		{
//			// Get xmin,xmax// 
//			uymin = pad->GetUymin();
//			uymax = pad->GetUymax();
//			ymin = pad->YtoAbsPixel(uymin);
//			ymax = pad->YtoAbsPixel(uymax);
//			
//			//--get biny--//
//			x = pad->AbsPixeltoX(px);
//			binx1 = hist2->GetYaxis()->FindBin(x);
//	
//			
//			binxmin = binx1 - pmbins;
//	    	binxmax = binx1 + pmbins;
//		   	xmin = hist2->GetXaxis()->GetBinLowEdge(binxmin);
//		   	xmax = hist2->GetXaxis()->GetBinUpEdge(binxmax);
//			px1 = pad->XtoAbsPixel(xmin);
//			px2 = pad->XtoAbsPixel(xmax);
//	    	
//	    	
//		   	pro = hist2->ProjectionY("",binxmin,binxmax);
//		   	
//		   	if (!c2)
//		   	{
//			   	c2 = Canv(1,1);
//				c2->cd();
//				pro->Draw();
//	   		}
//	   		c2->Modified();
//			c2->Update();
//			
//			pad->GetCanvas()->FeedbackMode(kTRUE);
//			if (remx1 && remx2)
//			{
//				gVirtualX->DrawLine(remx1,ymin,remx1,ymax);
//				gVirtualX->DrawLine(remx2,ymin,remx2,ymax);
//			}
//			gVirtualX->DrawLine(px1,ymin,px1,ymax);
//			gVirtualX->DrawLine(px2,ymin,px2,ymax);
//	
//			remx1 = px1;
//			remx2 = px2;
//		}
//		else if ( event == kButton1Down)
//		{
//		   	sprintf(title,"Pro to Y from x=%f to %f",xmin,xmax);
//			pro->SetTitle(title);
//			static int count;
//			sprintf(title,"project_%s%i",hist2->GetName(),count++);
//			pro->SetName(title);
//			c2->Modified();
//			c2->Update();

//			//--quit--//
//			c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
//			delete this;
//		}
//	}
//}

void proy(int pmbins=-1)
{
	ProY * z = new ProY(pmbins);
	TCanvas *c1 = gPad->GetCanvas();
	c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "ProY",z,"ExecuteProy(Int_t,Int_t,Int_t,TObject*)");
	return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////




















/////////////////////////////////////Drawsum///////////////////////////////////////////////////////////////
class DrawSum
{
RQ_OBJECT("DrawSum")

public:
	DrawSum(char * lay, char * v) {var = v;layer = lay;cntr=0;firsttime=true;}
	~DrawSum() {}

	void ExecuteDrawSum(Int_t event, Int_t px, Int_t py, TObject *sel);

private:
	long cntr;
	bool firsttime;
	char * layer;
	char * var;
};

void DrawSum::ExecuteDrawSum(Int_t event, Int_t px, Int_t py, TObject *sel)
{
	TCanvas * c = (TCanvas *) gTQSender;
	TPad * pad = (TPad *) c->GetSelectedPad();
	
	
	if ( event == kButton1Double)
	{
		//--quit--//
		double x1 = pad->AbsPixeltoX(px);
		double y1 = pad->AbsPixeltoY(py);

		cout <<var<<".SumCorrection_"<<layer<<".pos["<<cntr<<"] = 400;";
		cout << "\t"<<var<<".SumCorrection_"<<layer<<".sum["<<cntr++<<"] = "<<y1<<";"<<endl;
		cout <<var<<".SumCorrection_"<<layer<<".number_of_points = "<<cntr<<";"<<endl;

		c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
		delete this;
	}
	else if ( event == kButton1Down )
	{
		double x1 = pad->AbsPixeltoX(px);
		double y1 = pad->AbsPixeltoY(py);

		if (firsttime)
		{
			cout << endl;
			cout <<var<<".SumCorrection_"<<layer<<".pos["<<cntr<<"] = -400;";
			cout << "\t"<<var<<".SumCorrection_"<<layer<<".sum["<<cntr++<<"] = "<<y1<<";"<<endl;
			firsttime = false;
		}
		cout <<var<<".SumCorrection_"<<layer<<".pos["<<cntr<<"] = "<<x1<<";";
		cout << "\t"<<var<<".SumCorrection_"<<layer<<".sum["<<cntr++<<"] = "<<y1<<";"<<endl;
	}
	return;
}

void drawsum(char * v, char * layer)
{
	DrawSum * ds = new DrawSum(layer,v);
	TCanvas *c1 = gPad->GetCanvas();
	c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "DrawSum",ds,"ExecuteDrawSum(Int_t,Int_t,Int_t,TObject*)");
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////


















/////////////////////////////////////////////gaus()/////////////////////////////////////////////////////////////

TF1 * gaus(TH1D * hist, double fr, double to)
{
	double xmin = (fr >= to)? to:fr;
	double xmax = (fr <= to)? to:fr;
	
	hist->Fit("gaus","qw","",xmin,xmax);
	
	hist->GetFunction("gaus")->SetLineColor(kRed);
	gPad->Update();
	TPaveStats * s = (TPaveStats*) hist->GetFunction("stats");
	s->SetX1NDC(0.62);
	s->SetY1NDC(0.675);
	gPad->Modified();
	gPad->Update();
	return (hist->GetFunction("gaus"));
}

TF1 * gaus(double fr, double to)
{
	//--find hist first--//
	TH1D*hist1;
    TIter next(gPad->GetListOfPrimitives());
	TObject* obj;
    while((obj = next()))
    {
        if (obj->InheritsFrom("TH1"))
        {
	        hist1 = (TH1D *) obj;
	        break;
        }
    }

    //--if we did not find a histhaven't found anything--//
	if (!hist1)
		return 0;

	return (gaus(hist1,fr,to));
}

class Gaus
{
RQ_OBJECT("Gaus")

private:
	Float_t xmin;
	Float_t xmax;
	Float_t ymin;
	Float_t ymax;
	Float_t remx;
	Float_t uymin;
	Float_t uymax;
	Float_t x1;
	Float_t x2;
	int binxmin;
	int binxmax;
	int binx1;
	int binx2;
	TH1 * hist1; 

public:
	Gaus() {xmin=0;xmax=0;remx=0;ymin=0;ymax=0;uymax=0;uymin=0;x1=0;x2=0;hist1=NULL;}
	~Gaus() {}

	void ExecuteGaus(Int_t event, Int_t px, Int_t py, TObject *sel);
};

void Gaus::ExecuteGaus(Int_t event, Int_t px, Int_t py, TObject *sel)
{
	TCanvas * c = (TCanvas *) gTQSender;
	TPad * pad = (TPad *) c->GetSelectedPad();
	
	if (sel->InheritsFrom("TH1"))
		hist1 = (TH1 *) sel;

	
	if ( event == kButton1Double)
	{
		//--quit--//
		c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
		delete this;
	}
	else if ( event == kButton1Down )
	{
		// Get ymin,ymax// 
		uymin = pad->GetUymin();
		uymax = pad->GetUymax();
		ymin = pad->YtoAbsPixel(uymin);
		ymax = pad->YtoAbsPixel(uymax);

		x1 = pad->AbsPixeltoX(px);
		pad->SetEditable(kFALSE);
		
		pad->GetCanvas()->FeedbackMode(kTRUE);
		gVirtualX->DrawLine(px,ymin,px,ymax);
	}
	else if ( event == kButton1Up )
	{
	   	//--get biny--//     
		x2 = pad->AbsPixeltoX(px);
	    pad->SetEditable(kTRUE);
		    
		gaus(x1,x2);
	   	c->Update();
	
	   	// end program here //
		c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
		delete this;
	}
	else if ( event == kButton1Motion )
	{
		//--delete if drawn before--//
		pad->GetCanvas()->FeedbackMode(kTRUE);
		if (remx)
			gVirtualX->DrawLine(remx,ymin,remx,ymax);
			
		gVirtualX->DrawLine(px,ymin,px,ymax);
		remx=px;
	}
	return;
}

void gaus()
{
	Gaus * z = new Gaus();
	TCanvas *c1 = gPad->GetCanvas();
	c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "Gaus",z,"ExecuteGaus(Int_t,Int_t,Int_t,TObject*)");
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

TF1 * betafit(TH1D * hist, double fr, double to, double beta)
{
	double xmin = (fr >= to)? to:fr;
	double xmax = (fr <= to)? to:fr;
	
	TF1 *f1= new TF1("f1","[0]/4/3.14159*(1+[1]/2*(3*x*x-1))",-1,1);
		
	f1->SetParLimits(0,0,500000);
	f1->SetParameter(0,1000);
	if(beta == -2.)
	{
		f1->SetParLimits(1,-1.,2.);
		f1->SetParameter(1,0.9);
	}
	else
	{
		f1->FixParameter(1,beta);
	}

	
	hist->Fit("f1","qw","",xmin,xmax);
	
	hist->GetFunction("f1")->SetLineColor(kRed);
	gPad->Update();
	TPaveStats * s = (TPaveStats*) hist->GetFunction("stats");
	s->SetX1NDC(0.62);
	s->SetY1NDC(0.675);
	gPad->Modified();
	gPad->Update();
	return (hist->GetFunction("f1"));
}


TF1 * betafit(double fr=-1., double to=1.)
{
	//--find hist first--//
	TH1D*hist1;
    TIter next(gPad->GetListOfPrimitives());
	TObject* obj;
    while((obj = next()))
    {
        if (obj->InheritsFrom("TH1"))
        {
	        hist1 = (TH1D *) obj;
	        break;
        }
    }

    //--if we did not find a histhaven't found anything--//
	if (!hist1)
		return 0;

	return (betafit(hist1,fr,to,-2.));
}


TF1 * betafit(double beta, double fr, double to)
{
	//--find hist first--//
	TH1D*hist1;
    TIter next(gPad->GetListOfPrimitives());
	TObject* obj;
    while((obj = next()))
    {
        if (obj->InheritsFrom("TH1"))
        {
	        hist1 = (TH1D *) obj;
	        break;
        }
    }

    //--if we did not find a histhaven't found anything--//
	if (!hist1)
		return 0;

	return (betafit(hist1,fr,to,beta));
}


//__________________________________________________________________________________________________________________________________________________
TF1 * beta4fit(TH1D * hist, double fr, double to)
{
	double xmin = (fr >= to)? to:fr;
	double xmax = (fr <= to)? to:fr;
	
	TF1 *f1= new TF1("f1","[0]/4/3.14159*(1+[1]/2*(3*x*x-1)+[2]/8*(35*x*x*x*x-30*x*x+3))",-1,1);
		
	f1->SetParLimits(0,0,500000);
	f1->SetParameter(0,1000);
	
	
		f1->SetParLimits(1,-1.,2.);
		f1->SetParameter(1,0.9);
		f1->SetParameter(2,0.);
	

	
	hist->Fit("f1","qw","",xmin,xmax);
	
	hist->GetFunction("f1")->SetLineColor(kRed);
	gPad->Update();
	TPaveStats * s = (TPaveStats*) hist->GetFunction("stats");
	s->SetX1NDC(0.62);
	s->SetY1NDC(0.675);
	gPad->Modified();
	gPad->Update();
	return (hist->GetFunction("f1"));
}

TF1 * beta4fit(double fr=-1., double to=1.)
{
	//--find hist first--//
	TH1D*hist1;
    TIter next(gPad->GetListOfPrimitives());
	TObject* obj;
    while((obj = next()))
    {
        if (obj->InheritsFrom("TH1"))
        {
	        hist1 = (TH1D *) obj;
	        break;
        }
    }

    //--if we did not find a histhaven't found anything--//
	if (!hist1)
		return 0;

	return (beta4fit(hist1,fr,to));
}












/////////////////////////////////////////ColorScales/////////////////////////////////////////////////////////////////////////
void color(int coltable=1)
{ 
	
	int HighestIndex = 1000-230;
	//remove every color above 228 because otherwise this function cannot be called very often//
	TColor *color;

	TIter next(gROOT->GetListOfColors());
	TSeqCollection * coltab = gROOT->GetListOfColors();
	while ((color = (TColor *) next()))
	{
		if (color->GetNumber() > HighestIndex)
		{
			coltab->Remove(color);
			delete color;
		}
	}
	
	
	
	if (coltable == 0)			//grayscale
	{
		UInt_t Number = 2;
		Double_t Red[2]   = { 0.85, 0.15};
		Double_t Green[2] = { 0.85, 0.15};
		Double_t Blue[2]  = { 0.85, 0.15};
		Double_t Stops[2] = { 0.0, 1.0};

		//gStyle->CreateGradientColorTable(Number,Stops,Red,Green,Blue,230);
		TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,230);
	}
	else if( coltable == 1)		//standart cobold palette
	{		
		UInt_t Number = 7;
		Double_t Red[7]   = { 0.00, 0.00, 0.00, 0.00, 0.65, 0.99, 0.99};
		Double_t Green[7] = { 0.00, 0.00, 0.75, 0.99, 0.00, 0.00, 0.99};
		Double_t Blue[7]  = { 0.33, 0.99, 0.75, 0.00, 0.65, 0.00, 0.00};
		//Define the length of the (color)-interval between this points
		Double_t Stops[7] = { 0.00, 0.10, 0.30, 0.45, 0.70, 0.80, 1.00};

		//gStyle->CreateGradientColorTable(Number,Stops,Red,Green,Blue,230);
		TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,230);
	}
	else if (coltable == 2)		//tills palette
	{
		UInt_t Number = 4;
		Double_t Red[4]   = { 0.50, 0.00, 0.99, 0.99};
		Double_t Green[4] = { 0.99, 0.00, 0.00, 0.99};
		Double_t Blue[4]  = { 0.99, 0.99, 0.00, 0.00};
		Double_t Stops[4] = { 0.00, 0.30, 0.70, 1.00};

		//gStyle->CreateGradientColorTable(Number,Stops,Red,Green,Blue,230);
		TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,230);
	}
	else if (coltable == 3)		// Moritz Palette
	{
		UInt_t Number = 5;
		Double_t Red[5]   = { 0.00, 0.00, 0.93, 1.00, 0.93};
		Double_t Green[5] = { 0.00, 0.86, 0.13, 0.00, 0.75};
		Double_t Blue[5]  = { 0.99, 0.00, 0.53, 0.00, 0.13};
		Double_t Stops[5] = { 0.00, 0.40, 0.7, 0.8, 1.00};
		TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,230);
	}
}
/////////////////////////////////////////////////////////////////////



















////////////////////////////unzoom///////////////////////////////////////
void unzoom()
{
	TIter next(gPad->GetListOfPrimitives());
	TObject *obj;
    while((obj = next()))
	{
		if( obj->InheritsFrom("TGraph"))
		{
			TGraph *graph = (TGraph*)obj;
			graph->GetXaxis()->UnZoom();
			graph->GetYaxis()->UnZoom();
		}
		if( obj->InheritsFrom("TH1"))
		{
			TH1 *hist1 = (TH1*)obj;
			hist1->GetXaxis()->UnZoom();
			hist1->GetYaxis()->UnZoom();
			hist1->GetZaxis()->UnZoom();
		}
    }
	gPad->Modified();
	gPad->Update();
}
///////////////////////////////////////////////////////////////////















///////////////////////////////stuff that is needed when the root file is still beeing filled while watched////////////////////////////////
//void showHist(const char *name, TPad *pad = 0, bool upd=false) 
//{
//	Int_t xfirst,xlast,yfirst,ylast;
//	TH1* hist;

//	// if no pad is assigned, choose the currently highlighted pad //
//	if(!pad) pad = gPad;


//	TObject *obj = _file0->FindObject(name);
//	//if histo was there before remember axis settings before deleting//
//	if (upd && obj)
//	{
//		hist = (TH1*) obj;
//		xfirst = hist->GetXaxis()->GetFirst();
//		xlast = hist->GetXaxis()->GetLast();
//		if (obj->InheritsFrom("TH2"))	//if it is a 2d hist
//		{
//			yfirst = hist->GetYaxis()->GetFirst();
//			ylast = hist->GetYaxis()->GetLast();
//		}
//	}

//	//delete the old histogram and load the new one from file//
//	delete obj;
//	_file0->ReadKeys();
//	hist = (TH1*)_file0->Get(name);
//	//if we are in update mode and it was not able to load the histo, retry until you succeed//
//	while(upd && !hist)
//	{	
//		_file0->ReadKeys();
//		hist = (TH1*)_file0->Get(name);
//		gSystem->Wait(10);
//	}

//	if (hist)
//	{
//		//set the axis settings to the remembered values//
//		hist->GetXaxis()->SetRange(xfirst,xlast);
//		if (hist->InheritsFrom("TH2"))
//			hist->GetYaxis()->SetRange(yfirst,ylast);

//		//draw the histogram into the pad//
//		pad->cd();
//		hist->Draw();
//		pad->Modified();
//		pad->GetCanvas()->Update();
//		
//	}
//	return;
//}
//void UPDATE()
//{
//	TString upHi[20];
//	TPad* pads[20];
//	long upHicnt=0;
//	//loop through all canvases
//	TIter next(gROOT->GetListOfCanvases());
//	TCanvas * can;
//	TPad * pad;
//	TObject * obj;
//	while((can = (TCanvas*) next()))
//	{
//		//loop through all pads in the canvases
//		TIter next2(can->GetListOfPrimitives());
//		while((pad = (TPad*)next2()))
//		{
//			//loop through all objects of this pad
//			TIter next3(pad->GetListOfPrimitives());
//			while((obj = next3()))
//			{
//				//if it is a histogram, remember its name and the pad it was in
//				if (obj->InheritsFrom("TH1"))
//				{
//					pads[upHicnt] = pad;
//					upHi[upHicnt++] = obj->GetName();
//					if (upHicnt == 20){cout <<"cannot upate more then 20 histograms at a time"<<endl; upHicnt=0;}
//				}
//			}
//		}
//	}

//	//update all found histograms
//	for (int i=0;i<upHicnt;++i)
//		showHist(upHi[i].Data(),pads[i],true);

//	return;
//} 
//TTimer *timer = new TTimer();
//void update(int time=-2)
//{
//	if (time == -2)
//	{
//		timer->Stop();
//		UPDATE();
//		return;
//	}

//	if (time == -1)
//	{
//		timer->Stop();
//		return;
//	}

//	timer->SetCommand("UPDATE()");
//	timer->Start(time*1000);
//	return;
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////












///////////////////////////////////////////pipico stuff///////////////////////////////////////////////////////////////////
double CalcTof(double Mass_amu, double charge_au, double EField1_Vpcm, double EField2_Vpcm, double LenReg1_mm, double LenReg2_mm, double Mom_au)
{
	double Charge_au = charge_au;
	double Mass_au = Mass_amu*1836.15;
	double ElectricFieldRegion1_Vpcm = EField1_Vpcm;
	double ElectricFieldRegion2_Vpcm = EField2_Vpcm;
	double LengthRegion1_mm = LenReg1_mm;
	double LengthRegion2_mm = LenReg2_mm;

	double racc1_au = (ElectricFieldRegion1_Vpcm * 1.9446905050536652707924548855431e-10* Charge_au)/(Mass_au);
	double racc2_au = (ElectricFieldRegion2_Vpcm * 1.9446905050536652707924548855431e-10* Charge_au)/(Mass_au);
	double rs1_au = LengthRegion1_mm *1.8897261249935897655251029944769e+7;
	double rs2_au = LengthRegion2_mm *1.8897261249935897655251029944769e+7;

	double rvz_au = Mom_au / Mass_au;

	double rt1_au = -(rvz_au/racc1_au)+sqrt((rvz_au/racc1_au)*(rvz_au/racc1_au)+((2.*rs1_au)/racc1_au));
	double rv_au = racc1_au * rt1_au + rvz_au;
	double rt2_au;
	if (racc2_au)
		rt2_au = -(rv_au/racc2_au)+sqrt((rv_au/racc2_au)*(rv_au/racc2_au)+((2.*rs2_au)/racc2_au));
	else 
		rt2_au = 0;
	double rtof_au = rt1_au + rt2_au;
	double rtof_ns = rtof_au * 2.418884326505e-8;

	return rtof_ns;
}

void pipico(double Mass_amu, double charge_au, double EField1_Vpcm, double EField2_Vpcm, double LenReg1_mm, double LenReg2_mm, double MaxEnergy_ev)
{
	double tof1[200];
	double tof2[200];

	double Mom_au = sqrt(MaxEnergy_ev*0.036871796762656244238781755834962*2*Mass_amu*1836.15);

	for (int i=0;i<200;++i)
	{
		double MomofInterest = (double) (i-100) * Mom_au / 200.;
		tof1[i] = CalcTof(Mass_amu,charge_au,EField1_Vpcm,EField2_Vpcm,LenReg1_mm,LenReg2_mm,MomofInterest);
		tof2[i] = CalcTof(Mass_amu,charge_au,EField1_Vpcm,EField2_Vpcm,LenReg1_mm,LenReg2_mm,-MomofInterest);
//		cout <<"tof1 "<< tof1[i]<<endl;
//		cout <<"tof2 "<< tof2[i]<<endl;
	}

	TPolyLine *pline = new TPolyLine(200,tof1,tof2);
	pline->SetLineColor(kBlack);
	pline->SetLineWidth(2);
	pline->Draw();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Divide(char * formula, Double_t xmin, Double_t xmax)
{
	//--find hist first--//
	TH1D* hist1;
    TIter next(gPad->GetListOfPrimitives());
	TObject* obj;
    while((obj = next()))
    {
        if (obj->InheritsFrom("TH1"))
        {
			cout << "hier bin ich schonmal" << endl;
	        hist1 = (TH1D *) obj;
	        break;
        }
    }

	TF1* f1 = new TF1("f1",formula,xmin,xmax);

    hist1->Divide(f1);
	hist1->Draw();
	f1->Delete();
//	hist1->Delete();

	return;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OneDHistToTXT(char * filename)
{
	//--find hist first--//
	TH1D*hist1;
    TIter next(gPad->GetListOfPrimitives());
	TObject* obj;
    while((obj = next()))
    {
        if (obj->InheritsFrom("TH1"))
        {
	        hist1 = (TH1D *) obj;
	        break;
        }
    }
	// Daten auslesen und in txt-file schreiben
	std::ofstream tempstream(filename,std::ios::out);
	
	for(int i = 1;i <= hist1->GetNbinsX() ; i++)
	{
	//	tempstream << (hist1->GetXaxis()->GetXmin() + (double)((i)*hist1->GetXaxis()->GetBinWidth(i)) ) << '\t' << hist1->GetBinContent(i+1) << endl;
		tempstream << hist1->GetBinCenter(i) << '\t' << hist1->GetBinContent(i) << endl;
	}

	return;

}

void TwoDHistToTXT(char * filename)
{
	//--find hist first--//
	TH2D*hist1;
    TIter next(gPad->GetListOfPrimitives());
	TObject* obj;
    while((obj = next()))
    {
        if (obj->InheritsFrom("TH2"))
        {
	        hist1 = (TH2D *) obj;
	        break;
        }
    }
	// Daten auslesen und in txt-file schreiben
	std::ofstream tempstream(filename,std::ios::out);

	double x=0,y=0,z=0;
	
	for(int ix = 1; ix <= hist1->GetNbinsX(); ix++)
    {
		for(int iy = 1; iy <= hist1->GetNbinsY(); iy++)
		{
		//	x = hist1->GetXaxis()->GetXmin() + (ix)* (hist1->GetXaxis()->GetBinWidth(ix));
		//	y = hist1->GetYaxis()->GetXmin() + (iy)* (hist1->GetYaxis()->GetBinWidth(iy)); // !!!! IMPORTANT trotz y-achse getXMIN!!!!
			x = hist1->GetXaxis()->GetBinCenter(ix);
			y = hist1->GetYaxis()->GetBinCenter(iy);
			z = hist1->GetBinContent(ix,iy);
 		//	cout << x << y << z << endl;
			tempstream << x << '\t' << y << '\t' << z << endl;
		}
	}

	return;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Rebin1D(int xbins, char * newname)
{
	//--find hist first--//
	TH1D*hist1;
    TIter next(gPad->GetListOfPrimitives());
	TObject* obj;
    while((obj = next()))
    {
        if (obj->InheritsFrom("TH1"))
        {
	        hist1 = (TH1D *) obj;
	        break;
        }
    }

	Double_t * arrxbins = new Double_t[xbins+1];

	for(int i = 0; i <= xbins; i++)
	{
		arrxbins[i] = hist1->GetXaxis()->GetXmin() + i * (double)(hist1->GetXaxis()->GetBinWidth(i)) * (hist1->GetNbinsX()/xbins);
		cout << arrxbins[i] << endl;
	}

	hist1->Rebin(xbins,newname);
//	hist1->Rebin(xbins,newname,arrxbins);
	delete[] arrxbins;

	return;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void substractBackground(double backgroundlevel)
{
	TH1D * hist;
	TIter next(gPad->GetListOfPrimitives());
	TObject *obj;
	while((obj = next()))
	{
		if(obj->InheritsFrom("TH1"))
		{
			hist = (TH1D *) obj;
			break;
		}
	}

	// create new Histogram which is initialised as copy of the original Histogramm
	if(hist)
	{
		TString s = hist->GetName();
		s.Append("_woBackground");
		TH1D *hnew = (TH1D*)hist->Clone(s.Data());
		hnew->SetNameTitle(s.Data(),s.Data());
		
		
		for(int i=0;i<hist->GetNbinsX();i++)
		{
			double temp;
			temp = hnew->GetBinContent(i);
			hnew->SetBinContent(i,temp-backgroundlevel);
		}
	}
	
	return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void Multiply_SINE()
//{
//	TH2D * hist;
//	TIter next(gPad->GetListOfPrimitives());
//	TObject *obj;
//	while((obj = next()))
//	{
//		if(obj->InheritsFrom("TH2"))
//		{
//			hist = (TH2D *) obj;
//			break;
//		}
//	}

//	if(hist)
//	{
//		// create SINEfunction
//		TF1 sine("sine","sin(x/180.*3.14159)",hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
//		// make a copy of the old histogramm
//		TH2D *histcopy = hist->Clone("newname");
//		histcopy->SetOption("colz");
//		// multiply it by the SINEfunction
//		//histcopy->Multiply(&f1);	

//		for(int iy=0;iy <= histcopy->GetNbinsY(); iy++) // go through all KERs
//		{
//			for(int ix=0;ix <= histcopy->GetNbinsX();ix++) // go through all angles
//			{
//				if(sine.Eval(histcopy->GetXaxis()->GetBinCenter(ix)))
//				{
//					double sinx = sine.Eval(histcopy->GetXaxis()->GetBinCenter(ix));
//					histcopy->SetBinContent(ix,iy,hist->GetBinContent(ix,iy)/sinx);
//				}
//			}
//		}
//		
//		//open new Canvas and plot the new Histogramm onto it
//		TCanvas * c1 = Canv(1,1);
//		c1->cd();
//		histcopy->Draw();

//	}

//	return;
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void Multiply_SINE_1D()
//{
//	TH1D * hist;
//	TIter next(gPad->GetListOfPrimitives());
//	TObject *obj;
//	while((obj = next()))
//	{
//		if(obj->InheritsFrom("TH1"))
//		{
//			hist = (TH1D *) obj;
//			break;
//		}
//	}

//	if(hist)
//	{
//		// create SINEfunction
//		TF1 sine("sine","sin(x/180.*3.14159)",hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
//		// make a copy of the old histogramm
//		TH1D *histcopy = hist->Clone("newname");
//		//histcopy->SetOption("colz");
//		// multiply it by the SINEfunction
//		//histcopy->Multiply(&f1);

//		for(int ix=0;ix <= histcopy->GetNbinsX();ix++) // go through all angles
//		{
//				double sinx = sine.Eval(histcopy->GetXaxis()->GetBinCenter(ix));
//				if(sinx == 0)	sinx = 1.e-10;
//				histcopy->SetBinContent(ix,hist->GetBinContent(ix)/sinx);
//		}
//		
//		
//		//open new Canvas and plot the new Histogramm onto it
//		TCanvas * c1 = Canv(1,1);
//		c1->cd();
//		histcopy->Draw();

//	}

//	return;
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void normiereHist_a_auf_b_alt(TH1D *a,TH1D *b, const char* newname)
{
	// �berpr�fe auf gleiche Anzahl an bins
	if(a->GetNbinsX() != b->GetNbinsX())
	{
		std::cout << "Histogramme muessen gleiche Anzahl an bins haben!" << std::endl;
		return;
	}


    TH1D* h_neu = new TH1D(newname,newname,a->GetNbinsX(),a->GetXaxis()->GetXmin(),a->GetXaxis()->GetXmax());

	double temp;

	for(int iBin=0; iBin < h_neu->GetNbinsX(); iBin++)
	{
		
			if(b->GetBinContent(iBin)!=0)
				temp = a->GetBinContent(iBin)/b->GetBinContent(iBin);
			else
				temp=0;
		
		h_neu->SetBinContent(iBin,temp);
	}


	
	return;
}

void normiereHist_a_auf_b(const char* name_a, const char* name_b, const char* newname)
{
	
	Bool_t reset = kTRUE;
	
	//TString fullname_a = go4->FindItem(name_a);
	//TString fullname_b = go4->FindItem(name_b);
	
	TH1D* a = dynamic_cast<TH1D*> (go4->NextMatchingObject(name_a, "Histograms", reset));
	TH1D* b = dynamic_cast<TH1D*> (go4->NextMatchingObject(name_b, "Histograms", reset));
	
//	TH1D* a = dynamic_cast<TH1*> (go4->GetObject(name_a,"gROOT"));
//	TH1D* b = dynamic_cast<TH1*> (go4->GetObject(name_b,"gROOT"));


	// ï¿½berprï¿½fe auf gleiche Anzahl an bins
	if(a->GetNbinsX() != b->GetNbinsX())
	{
		std::cout << "Histogramme muessen gleiche Anzahl an bins haben!" << std::endl;
		return;
	}
	
	//TH1D* h_neu = MakeH1<TH1D>(newname, newname,a->GetNbinsX(),a->GetXaxis()->GetXmin(),a->GetXaxis()->GetXmax(), "Makros", " ");
	//FillTo1DHist(mMbIndex, "MbIndex_i_sort", "Minibunch index for each sorted ion", "unp_hist_mbindex_bins", "unp_hist_mbindex_min", "unp_hist_mbindex_max", "Unpack/Minibunch");
		
    TH1D* h_neu = new TH1D(newname,newname,a->GetNbinsX(),a->GetXaxis()->GetXmin(),a->GetXaxis()->GetXmax());
    
    go4->AddHistogram (h_neu, "Makro");

	double temp;

	for(int iBin=0; iBin < h_neu->GetNbinsX(); iBin++)
	{
		
			if(b->GetBinContent(iBin)!=0)
				temp = a->GetBinContent(iBin)/b->GetBinContent(iBin);
			else
				temp=0;
		
		h_neu->SetBinContent(iBin,temp);
	}


	
	return;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Divide(TH1D *hist1D, TH2D *hist2D, const char * name)
{
	// check whether number of bins fit
	int nbins1D = hist1D->GetXaxis()->GetNbins();
	int nbins2D = hist2D->GetYaxis()->GetNbins();
	if(nbins1D != nbins2D)
	{cout << "Not equal amount of bins!" << endl; return;}

	// create new Histogram
	TH2D *newHist = new TH2D(name,name,hist2D->GetXaxis()->GetNbins(),hist2D->GetXaxis()->GetXmin(),hist2D->GetXaxis()->GetXmax(),
										hist2D->GetYaxis()->GetNbins(),hist2D->GetYaxis()->GetXmin(),hist2D->GetYaxis()->GetXmax());
	newHist->SetOption("colz");
	
	// fill new Histogramm
	for(int ix=0; ix <= hist2D->GetXaxis()->GetNbins(); ix++)
		for(int iy=0; iy <= hist2D->GetXaxis()->GetNbins(); iy++)
			if(hist1D->GetBinContent(iy))
				newHist->SetBinContent(ix,iy,hist2D->GetBinContent(ix,iy)/hist1D->GetBinContent(iy));
			else
				newHist->SetBinContent(ix,iy,0);
}	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
TH1D *multiply(TH1D* hist,double factor, const char* newname)
{

	TH1D* h_neu = new TH1D(newname,"vllt jetzt",hist->GetNbinsX(),hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
	
	
	for (int i=1;i<=h_neu->GetNbinsX();++i)
				{
					double temp = hist->GetBinContent(i)*factor;
					if (temp<0.) temp=0.;
					h_neu->SetBinContent(i,temp);
				}
		
	return h_neu;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

