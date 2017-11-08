//--This file contains macros that were written by Achim Oliver Czasch & Lutz Foucar--//
void help()
{
	cout<<"******Root macros written by Achim Czasch and Lutz Foucar******"<<endl;
	cout<<endl;
	cout<<endl;
	cout<<" 1.)  polar(ErrorBars=true)	(use this when the histogram is already in pad)"<<endl;
	cout<<" 2.)  polar(histname)	(use to polar plot histname)"<<endl;
	cout<<" 3.)  printHistos(const char* Name, dimension=2, format=3)"<<endl;
	cout<<"              format = 1 => ps"<<endl;
	cout<<"              format = 2 => eps"<<endl;
	cout<<"              format = 3 => gif"<<endl;
	cout<<"              format = 4 => svg"<<endl;
	cout<<"              format = 5 => pdf"<<endl;
	cout<<" 4.)  zoom2d(Rectangle=true) (true means that an rectangle is forced)"<<endl;
	cout<<" 5.)  circle(center=0) (draws a circle with center)"<<endl;
	cout<<" 6.)  prox(Nbr+-Bins=-1) (if nbr+-bins is >-1 then it draws the"<<endl;
	cout<<"              projection in a separat canvas online with +- nbrbins"<<endl;
	cout<<"              around the mouse)"<<endl;
	cout<<" 7.)  proy(Mbr+-Bins=-1) (same thing here for projection to Y)"<<endl;
	cout<<" 8.)  int()         (gives U the nbr of entries in the selected Area)"<<endl;
	cout<<" 9.)  add(nbr)      (add nbr to current histogram)"<<endl;
	cout<<"10.)  add(hist,nbr) (add nbr to hist)"<<endl;
	cout<<"11.)  gaus()        (makes a gaus fit to a 1D histo)"<<endl;
	cout<<"12.)  test2d()      (creates a 2d histo for testing)"<<endl;
	cout<<"13.)  test1d()      (creates a 1d histo for testing)"<<endl;
	cout<<"14.)  subHistos(hist1,hist2) (substracts hist1 from hist2)"<<endl;
	cout<<"15.)  drawsum()            (to correct timesums)"<<endl;
	cout<<"16.)  color(int colornbr)  (sets another colorscale (0 for grayscale))"<<endl;
	cout<<"17.)  unzoom()             (unzooms histograms)"<<endl;
	cout<<"18.)  dist()               (measures the distance between two points)"<<endl;
	cout<<"19.)  Canv(int tilehor=1, int tilever=1) (creates a Canvas with nbr of hor"<<endl;
	cout<<"                                          and nbr of ver subpads)"<<endl;
	cout<<"20.)  printCanv() (prints the contents of all open Canvases)"<<endl;
	cout<<"21.)  findMaximum(hist, max=0)   (find the maximum in hist)"<<endl; 
	cout<<"22.)  findMinimum(hist, min=0)   (find the minimum in hist)"<<endl;
	cout<<"23.)  showHist(const char * histname) (for viewing a hist while histogram"<<endl;
	cout<<"                                       is beeing filled)"<<endl;
	cout<<"24.)  update(int seconds)  (updates the histogram, every given seconds."<<endl;
	cout<<"                           If no argument is given, histograms are updated once)"<<endl;
	cout<<"25.)  pipico(Mass_amu, charge_au, EField1_Vpcm, EField2_Vpcm, "<<endl;
	cout<<"             LenReg1_mm, LenReg2_mm, MaxEnergy_ev) (this will draw a pipico line)"<<endl;
	cout<<"26.)  pro3d(\"Plane\",from,to) (projects a 3d histogram onto the chosen plane)"<<endl;
	cout<<"27.)  multiply(TH1D* hist,double factor, const char* newname)	(multiplies Histogramm with a certain factor)"<<endl;
	cout<<"28.)  void OneDHistToTXT(char * filename)   (exports 1D-Histogramm to txt-file)"<<endl;
	cout<<"29.)  void TwoDHistToTXT(char * filename)   (exports 2D-Histogramm to txt-file)"<<endl;
	cout<<"30.)  void Rebin1D(int xbins, char * newname)"<<endl;
	cout<<"31.)  void Divide(TH1D *hist1D, TH2D *hist2D, const char * name)"<<endl;
	cout<<endl;
}





















////////////////////////////////////////////////////////////////////////////////////////
//---------------------create Canvas---------------------------------------------------
TCanvas* Canv(int hor=1, int ver=1)
{
	
	//find out which size is the max, so that all subpads are still rectangluar but not bigger than 367//
	double h = gClient->GetDisplayHeight();
	double w = gClient->GetDisplayWidth();
	h -=26;
	w -=10;
	double hm = h/(double)ver;
	double wm = w/(double)hor;
	double maxsize = (wm>hm)? hm:wm;
	if (maxsize > 367) maxsize = 367;
	double cw = gStyle->GetScreenFactor();	//root multiplies the height and the width by this number
											//so in order to get the wanted size we need to divide the values by this number first

	static int nbrCanv;
	char name[64];
	char title[64];
	sprintf(name,"MyC%i",nbrCanv);
	sprintf(title,"MyCanvas %i",nbrCanv);
	nbrCanv++;
	TCanvas * MyC = new TCanvas(name,title,-1,1,maxsize*(double)hor/cw,maxsize*(double)ver/cw);
	MyC->Divide(hor,ver,0.0001,0.0001);
	MyC->ToggleEventStatus();
	for (int i=1;i<=hor*ver;++i)
	{
		MyC->cd(i);
		gPad->SetRightMargin(0.15);
		gPad->SetLeftMargin(0.15);
		gPad->SetTopMargin(0.15);
		gPad->SetBottomMargin(0.15);
		gPad->SetGridx();
		gPad->SetGridy();
	}
	return MyC;
}
////////////////////////////////////////////////////////////////////////////////////////


















////////////////////////////////////////////////////////////////////////////////////////
//---------------------print all open Canvase---------------------------------------------------
void printCanv()
{
	//go through all open canvase//
	TIter next(gROOT->GetListOfCanvases());
	TCanvas * can;
	while( (can = (TCanvas*) next()) )
	{
		//go through all pads in canvase//
		can->Print(can->GetName());
	}
	return;
}
////////////////////////////////////////////////////////////////////////////////////////





















////////////////////////////////polar stuff////////////////////////////////////////////////////////
void polar(bool errorbar=true, const char * tit ="", TPad * pad = 0)
{
    TIter next(gPad->GetListOfPrimitives());
    TObject *obj;
	TH1 *hist1;
	//find histogram in current pad//
	while((obj = next()))
	{
		if( obj->InheritsFrom("TH1") ) 
		{
			hist1 = (TH1*)obj;
			break;
		}
	}
	//check if histogram is a 1D Histogram//
	if (hist1->InheritsFrom("TH2"))
	{
		cout << "This histogram is a 2D Histogram, stopping here"<<endl;
		return;
	}
	polar(hist1,errorbar,tit,pad);
	return;
}
void polar(TH1 * hist1, bool errorbar=true, const char * tit= "", TPad * pad = 0)
{
	//check wether the histo has the right ranges and is in degrees or in cos--//
	bool degrees = false;
	if( ((hist1->GetXaxis()->GetXmin() ==-180) && (hist1->GetXaxis()->GetXmax() == 180)) ||
		((hist1->GetXaxis()->GetXmin() ==   0) && (hist1->GetXaxis()->GetXmax() == 360))
	  )
		degrees = true;
	else if ((hist1->GetXaxis()->GetXmin() ==  -1) && (hist1->GetXaxis()->GetXmax() ==   1))
		degrees = false;
	else
	{
		cout << "The Histogram has the wrong Range:"<<endl;
		cout << "   "<< hist1->GetXaxis()->GetXmin() << " ... "<< hist1->GetXaxis()->GetXmax()<<endl;
		cout << "This Macro only supports Histograms in the following Ranges:"<<endl;
		cout << "   -180 ... 180"<<endl;
		cout << "or    0 ... 360"<<endl;
		cout << "or   -1 ... 1"<<endl;

		return;
	}
	//stuff that we need to draw errorlines and markers//
	TLine * l = new TLine();
	TGraph * gr = new TGraph();
	TMarker * m = new TMarker();
	m->SetMarkerStyle(7);
	m->SetMarkerSize(1);
	gr->SetMarkerStyle(1);

	//write the 360 degree labels and lines//
	preparePad(hist1->GetMaximum(), tit, pad);

	//plot the dots and the errorbars if wanted//
	double conv2NDC = 1./hist1->GetMaximum();
	for (int i=1;i<=hist1->GetNbinsX();i++)
	{	
		//convert polar koordinates to kartesian koordinates//
		double cos_x = (degrees)? cos(hist1->GetBinCenter(i)*3.1415/180.) : hist1->GetBinCenter(i);
		double sin_x = (degrees)? sin(hist1->GetBinCenter(i)*3.1415/180.): sqrt(1.- (hist1->GetBinCenter(i)*hist1->GetBinCenter(i)));
		double y = hist1->GetBinContent(i);
		double error = hist1->GetBinError(i);

		double mx = cos_x * y * conv2NDC;
		double my = sin_x * y * conv2NDC;

		double ex1 = mx - 0.5*error * cos_x * conv2NDC;
		double ex2 = mx + 0.5*error * cos_x * conv2NDC;
		double ey1 = my - 0.5*error * sin_x * conv2NDC;
		double ey2 = my + 0.5*error * sin_x * conv2NDC;
		
		//draw the markers and the errorbars//
		m->DrawMarker(mx,my);
		if (errorbar)
			l->DrawLine(ex1,ey1,ex2,ey2);
	}
	

	//--if we have a fit function draw it--//
	TIter next(hist1->GetListOfFunctions());
	TF1 * func=0;
	while((func = (TF1*) next()))		//find first fit function
	{
		if (func->InheritsFrom("TF1"))
			break;
	}

	if (func)
	{
		gr->Set(func->GetNpx()+1);
		double dx = (func->GetXmax()-func->GetXmin())/func->GetNpx();
		for (int i=0;i<=func->GetNpx();++i)
		{
			double x = i*dx + func->GetXmin();
			double y = func->Eval(x);

			double cos_x = (degrees)? cos(x*3.1415/180.) : x;
			double sin_x = (degrees)? sin(x*3.1415/180.): sqrt(1.-(x*x));

			double mx = cos_x * y * conv2NDC;
			double my = sin_x * y * conv2NDC;

			gr->SetPoint(i,mx,my);
		}
		gr->SetLineColor(func->GetLineColor());
		gr->SetLineStyle(func->GetLineStyle());
		gr->SetLineWidth(func->GetLineWidth());
		gr->SetFillColor(func->GetFillColor());
		gr->SetFillStyle(func->GetFillStyle());
		gr->SetMarkerColor(func->GetMarkerColor());
		gr->SetMarkerStyle(func->GetMarkerStyle());
		gr->SetMarkerSize(func->GetMarkerSize());

		gr->Draw("l");
	}
	return;
}
void preparePad(double rmax, const char * title, TPad * pad =0)
{
	//prepare the pad, if no pad is given create a canvas//
	if(!pad)
	{
		TCanvas * MyPolarCanvas = Canv(1,1);
		MyPolarCanvas->cd();
		pad = gPad;
	}
	pad->cd();
	pad->SetBorderSize(2);
	pad->SetFrameLineColor(19);
	pad->SetFrameBorderMode(0);
	pad->SetLeftMargin(0.2);
	pad->SetBottomMargin(0.2);

	TH1 *hframe = new TH1F("hframe",title,1000,-1,1);
	hframe->SetMinimum(-1);
	hframe->SetMaximum(1);
	hframe->SetDirectory(0);
	hframe->SetStats(0);
	hframe->GetXaxis()->SetAxisColor(0);
	hframe->GetXaxis()->SetLabelOffset(999);
	hframe->GetXaxis()->SetTickLength(0);
	hframe->GetYaxis()->SetAxisColor(0);
	hframe->GetYaxis()->SetLabelOffset(999);
	hframe->GetYaxis()->SetTickLength(0);
	hframe->Draw("a");

	//Draw the axis//
	double rmin = 0;
	TGaxis *axis = new TGaxis(-1.25,0.,-1.25,1,rmin,rmax,510,"");
	axis->SetLabelSize(.03);
	axis->Draw();

	//compute where the ticks in the axis are using the same function the axis is using//
	double binLow,binHigh,binWidth;
	int nbins;
	THLimitsFinder::Optimize(rmin,rmax,10,binLow,binHigh,nbins,binWidth,"");

	double conv2NDC = 1./rmax;

	//Draw the polar circles//
	TEllipse el;
	el.SetFillStyle(0);
	el.SetLineStyle(3);
	Double_t r = (rmin+binWidth)*conv2NDC;
	Double_t dr = binWidth*conv2NDC;
	for (Int_t i=1; i<=nbins; i++) 
	{
		el.DrawEllipse(0.,0.,r,r,0.,360,0);
		r += dr;
	}

	//Draw the radial lines and labels//
	TLine l;
	TLatex t;
	char s[24];
	Double_t theta = 0;
	Double_t dtheta = TMath::Pi()/6.;
	t.SetTextSize(.03);
	l.SetLineStyle(3);
	for (i=1; i<=12; i++)
	{
		l.DrawLine(0.,0.,TMath::Cos(theta),TMath::Sin(theta));
		sprintf(s,"%g^{#circ}",theta*180./TMath::Pi());
		t.SetTextAlign(12);
		if (i>4 && i<10) t.SetTextAlign(32);
		if (i==4) t.SetTextAlign(21);
		if (i==10) t.SetTextAlign(23);
		t.DrawLatex(1.03*TMath::Cos(theta),1.03*TMath::Sin(theta),s);
		theta += dtheta;
	}
	return;
}
//__________________________ Version die Gebrauch von TGraphPolar macht___ Moritz 27.5.2010____________________________________

TGraphPolar *makePolar(TH1D *hist)
{
	//check whether the histo has the right ranges and is in degrees or in cos--//
	bool degrees = false;
	if((hist->GetXaxis()->GetXmin() ==0) && (hist->GetXaxis()->GetXmax() == 180))
		degrees = true;
	else if ((hist->GetXaxis()->GetXmin() ==  -1) && (hist->GetXaxis()->GetXmax() ==   1))
		degrees = false;
	else if((hist->GetXaxis()->GetXmin() ==-180) && (hist->GetXaxis()->GetXmax() == 180))
		degrees = true;
	else {
		cout << "The Histogram has the wrong Range:"<<endl;
		cout << "   "<< hist1->GetXaxis()->GetXmin() << " ... "<< hist1->GetXaxis()->GetXmax()<<endl;
		cout << "This Macro only supports Histograms in the following Ranges:"<<endl;
		cout << "   0° ... 180°"<<endl;
		cout << "or   -1 ... 1"<<endl
		cout << "or   -180° ... +180°"<<endl;
		return;
	}
	
	TCanvas * CPol = new TCanvas("CPol","TGraphPolar Example",500,500);

	//TH1D *hist = new TH1D("test","test",90,0,TMath::Pi());
	//hist->FillRandom("gaus");
	
	
	double nbins = hist->GetNbinsX();
	double *theta = new double[nbins];
	double *etheta  = new double[nbins];
		double thetaMin = hist->GetXaxis()->GetXmin();
		double thetaMax = hist->GetXaxis()->GetXmax();
		double binsize = (thetaMax-thetaMin)/nbins;
	double *radius = new double[nbins];
	double *eradius = new double[nbins];
	

	for(int i=0;i<nbins;i++) {
		theta[i] = thetaMin + i/nbins*(thetaMax-thetaMin) + binsize/2.;
		etheta[i]=1e-3;
		if(degrees)
			theta[i]*=TMath::Pi()/180.;
		else
			theta[i] = acos(theta[i]);
		radius[i] = hist->GetBinContent(i+1);
		eradius[i] = sqrt(radius[i]);
		//std::cout << i << ":\t" << theta[i] << '\t' << radius[i] << '\t' << eradius[i] << std::endl;
	}

	TGraphPolar *gPolar = new TGraphPolar(nbins,theta,radius,0,eradius);
	gPolar->SetTitle("TGraphPolar Example");

	gPolar->SetMarkerStyle(20);
	gPolar->SetMarkerSize(1.2);
	gPolar->SetMarkerColor(1);
	gPolar->SetLineColor(1);
	gPolar->SetLineWidth(3);
	gPolar->Draw("PE");

	// Update, otherwise GetPolargram returns 0
	CPol->Update();
	gPolar->GetPolargram()->SetToRadian();
	//gPolar->GetPolargram()->SetToDegree();
	gPolar->GetPolargram()->SetRangeRadial(0.,hist->GetMaximum()*1.1);
	//gPolar->GetPolargram()->SetRangeRadial(hist->GetMinimum()*0.7,hist->GetMaximum()*1.1);
	//CPol->Update();

	return gPolar;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////










////////////////////////////findmaximum/////////////////////////////////////////////////////////////
void findMaximum(TH1 * hist, double max = 0)
{
	double maximum=0;
	double maxx=0;
	double maxy=0;
	long k=0;

	for (int i=1;i<=hist->GetNbinsX();++i)
	{
		for (int j=1;j<=hist->GetNbinsY();++j)
		{
			bool inRange = (max)? ((hist->GetBinContent(i,j)>maximum) && (hist->GetBinContent(i,j)< max)) : (hist->GetBinContent(i,j)>maximum);
			if (inRange)
			{
				maximum = hist->GetBinContent(i,j);
				maxx=i;
				maxy=j;
			}
		}
	}

	for (int i=1;i<=hist->GetNbinsX();++i)
	{
		for (int j=1;j<=hist->GetNbinsY();++j)
		{
			if (hist->GetBinContent(i,j)==maximum)
			{
				cout << "maximum["<<++k<<"]("<<hist->GetBinContent(i,j)<<") is at ("<<i<<","<<j<<") ("<<hist->GetXaxis()->GetBinCenter(i)<<","<<hist->GetYaxis()->GetBinCenter(j)<<")"<<endl;
			}
		}
	}
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////



















////////////////////////////findminimum/////////////////////////////////////////////////////////////
void findMinimum(TH1 * hist, double min = 0)
{
	double minimum = hist->GetBinContent(1,1);;
	double minx=1;
	double miny=1;
	long k=0;

	for (int i=1;i<=hist->GetNbinsX();++i)
	{
		for (int j=1;j<=hist->GetNbinsY();++j)
		{
			bool inRange = (min)? ((hist->GetBinContent(i,j) < minimum) && (hist->GetBinContent(i,j) < min)) : (hist->GetBinContent(i,j) < minimum);
			if (inRange)
			{
				minimum = hist->GetBinContent(i,j);
				minx=i;
				miny=j;
			}
		}
	}

	for (int i=1;i<=hist->GetNbinsX();++i)
	{
		for (int j=1;j<=hist->GetNbinsY();++j)
		{
			if (hist->GetBinContent(i,j)==minimum)
			{
				cout << "minimum["<<++k<<"]("<<hist->GetBinContent(i,j)<<") is at ("<<i<<","<<j<<") ("<<hist->GetXaxis()->GetBinCenter(i)<<","<<hist->GetYaxis()->GetBinCenter(j)<<")"<<endl;
			}
		}
	}
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////













///////////////////////////////print Histos////////////////////////////////////////
void printHistos(const char* inName, int format=3)
{
	char filename[50];
	TH1 *h1;
	TString name = inName;

	TKey *key;
  
	TCanvas *c0;	
	c0 = gPad->GetCanvas();

	TIter nextkey(gDirectory->GetListOfKeys());
	
	while (key = (TKey*)nextkey())
	{
		TObject * obj = key->ReadObj();
		if( obj->InheritsFrom("TH1") )
		{
			h1 = (TH1*)obj; 
			if( name.Contains(h1->GetName()) )
			{
				h1->Draw();
				c0->Update();
				switch (format)
				{
				case 1:	sprintf(filename,"%s.ps",h1->GetName());
						gPad->Print(filename,"ps"); 
						break;
				case 2: sprintf(filename,"%s.eps",h1->GetName());
						gPad->Print(filename,"eps"); 
						break;
				case 3:	sprintf(filename,"%s.gif",h1->GetName());
						gPad->Print(filename,"gif"); 
						break;
				case 4:	sprintf(filename,"%s.svg",h1->GetName());
						gPad->Print(filename,"svg"); 
						break;
				case 5: sprintf(filename,"%s.pdf",h1->GetName());
						gPad->Print(filename,"pdf"); 
						break;
				default:break;
				}
			}
		}
	}
	return;
}
//////////////////////////////////////////////////////////////////////


















///////////////////////////////////substract two 1d histos/////////////////
TH1D *subHistos(TH1D * hi1, TH1D * hi2)
{
	if ((hi1->GetNbinsX() != hi2->GetNbinsX()))
	{
		cout <<"Histos have to have the same binning"<<endl;
		return;
	}

	TH1D * sub = new TH1D("sub","substracted Hist",hi1->GetNbinsX(),hi1->GetXaxis()->GetXmin(),hi1->GetXaxis()->GetXmax());
	
	sub->Add(hi1,hi2,1,-1);
	return sub;
}
///////////////////////////////////substract two 2d histos/////////////////
TH2D *subHistos(TH2D * hi1, TH2D * hi2)
{
	if ((hi1->GetNbinsX() != hi2->GetNbinsX()) || (hi1->GetNbinsY() != hi2->GetNbinsY()) )
	{
		cout <<"Histos have to have the same binning"<<endl;
		return;
	}

	TH2D * sub = new TH2D("sub","substracted Hist",hi1->GetNbinsX(),hi1->GetXaxis()->GetXmin(),hi1->GetXaxis()->GetXmax(),hi1->GetNbinsY(),hi1->GetYaxis()->GetXmin(),hi1->GetYaxis()->GetXmax());
	sub->SetOption("colz");

	sub->Add(hi1,hi2,1,-1);
	return sub;
}
/////////////////////////////////////////////////////////////////////////////////





















/////////////////////////Add something to histos//////////////////////////////////
void add(double add)
{
    TList   *list = gPad->GetListOfPrimitives();
    TObject *obj  = list->First();

	while(obj)
	{
		if( !strncmp("TH1",obj->ClassName(),3) ) 
		{
			TH1 *hist1 = (TH1*)obj;
			add(hist1,add);
			hist1->Draw();
		}

		if( !strncmp("TH2",obj->ClassName(),3) ) 
		{
			TH2 *hist2 = (TH2*)obj;
			add(hist2,add);
			hist2->Draw("colz");
		}
		obj = list->After(obj);
	}
	return;
}
void add(TH1* hist1,double ad)
{

	for (int i=1;i<=hist1->GetNbinsX();++i)
	{
		double temp = hist1->GetBinContent(i)+ad;
		if (temp<0.) temp=0.;
		hist1->SetBinContent(i,temp);
	}
	return;
}

void add(TH2* hist2,double add)
{
	for (int i=1;i<=hist2->GetNbinsX();++i)
	{
		for (int j=1;j<=hist2->GetNbinsY();++j)
		{
			double temp = hist2->GetBinContent(i,j)+ad;
			if (temp<0.) temp=0.;
			hist2->SetBinContent(i,j,temp);
		}
	}
	return;
}
///////////////////////////////////////////////////////////

/*
void multiply(TH1* hist1,double factor)
{

	char* histname[64];
	strcpy(histname,hist1->GetName());
	strcat(histname,"mult");

	hist1->Clone(histname);

	for (int i=1;i<=hist1->GetNbinsX();++i)
	{
		double temp = hist1->GetBinContent(i)*factor;
		if (temp<0.) temp=0.;
		hist1->SetBinContent(i,temp);
	}
	return;
}
*/


// to do liste:
//
//	1.) Namen des übergebenen Histogramms einlesen
//	2.) Name erweitern um Faktor mit dem multipliziert wird
//	3.) Histogramm mit neu erzeugtem Namen erzeugen und mit multiplizierten Werten füllen 

















/////////////////////ZOOM2D///////////////////////////////
class Zoom2D
{
RQ_OBJECT("Zoom2D")

private:
	Float_t x1;
	Float_t y1;
	Float_t x2;
	Float_t y2;
	Float_t fx1;
	Float_t fy1;
	Float_t rx2;
	Float_t ry2;
	Bool_t rect;

	void getRectangle(Float_t recx1, Float_t recx2, Float_t &recy1, Float_t &recy2);

public:
	Zoom2D(Bool_t r) {x1=0;y1=0;x2=0;y2=0;fx1=0;fy1=0;rx2=0;ry2=0;rect=r;}
	~Zoom2D() {}

	void ExecuteZoom2D(Int_t event, Int_t px, Int_t py, TObject *sel);
};
void Zoom2D::getRectangle(Float_t recx1, Float_t recy1, Float_t &recx2, Float_t &recy2)
{
	Float_t x = recx1-recx2;
	Float_t y = recy1-recy2;

	if ( ((x<0.)&&(y<0.)) || ((x>0.)&&(y>0.)) )
	{
		if (fabs(x)>fabs(y)) recy2=recy1-x;
		if (fabs(x)<fabs(y)) recx2=recx1-y;
	}

	if ( ((x<0.)&&(y>0.)) || ((x>0)&&(y<0.)) )
	{
		if (fabs(x)>fabs(y)) recy2=recy1+x;
		if (fabs(x)<fabs(y)) recx2=recx1+y;
	}
}
void Zoom2D::ExecuteZoom2D(Int_t event, Int_t px, Int_t py, TObject *sel)
{

   TCanvas * c = (TCanvas *) gTQSender;
   TPad * pad = (TPad *) c->GetSelectedPad();
   TH2 * hist2 = (TH2 *) sel;


   if ( event == kButton1Double)
   {
		//--quit--//
		c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
		delete this;
   }
   else if ( event == kButton1Down )
   {
		//--Get x1,y1--//
		fx1 = px;
		fy1 = py; 
	   	x1 = pad->AbsPixeltoX(fx1);
		y1 = pad->AbsPixeltoY(fy1);
   }
   else if ( event == kButton1Up )
   {
		//--Get Coordinates--//
		x2 = pad->AbsPixeltoX(px);
		y2 = pad->AbsPixeltoY(py);
		if (rect) getRectangle(x1,y1,x2,y2);
     
		//--now zoom in--//
		Float_t xmax = (x1 > x2)? x1 : x2;
		Float_t xmin = (x1 < x2)? x1 : x2;
		Float_t ymax = (y1 > y2)? y1 : y2;
		Float_t ymin = (y1 < y2)? y1 : y2;
		hist2->GetXaxis()->SetRangeUser(xmin,xmax);
		hist2->GetYaxis()->SetRangeUser(ymin,ymax);
		hist2->Draw();
		c->Modified();
		c->Update();
     
		//--end program here //
		c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
		delete this;
   }
   else if ( event == kButton1Motion )
   {
		Float_t ix2 = pad->AbsPixeltoX(px);
		Float_t iy2 = pad->AbsPixeltoY(py);
		if (rect) getRectangle(x1,y1,ix2,iy2);

		Float_t fx2 = pad->XtoAbsPixel(ix2);
		Float_t fy2 = pad->YtoAbsPixel(iy2);

		pad->GetCanvas()->FeedbackMode(kTRUE);

     	//--delete old box--//
     	if (rx2 && ry2)
			 gVirtualX->DrawBox(fx1,fy1,rx2,ry2,1);	//left line
		// draw new Box //
		gVirtualX->DrawBox(fx1,fy1,fx2,fy2,1);	 	
		rx2=fx2;
		ry2=fy2;

       /********************box explanation******************
        *           top line                                *
        * #-------------------------------# tempx2,y1       *
        * | x1,y1                         |                 *
        * |                               |                 *
        * |                               |                 *
        * |                               | right line      *
        * | left line                     |                 *
        * |                               |                 *
        * |                               |                 *
        * | x1,tempy2                     |                 *
        * #-------------------------------# tempx2,tempy2   *
        *          bottom line                              *
        *****************************************************/
   }
   return;
}
void zoom2d(Bool_t rect=true)
{
	Zoom2D * z = new Zoom2D(rect);
	TCanvas *c1 = gPad->GetCanvas();
	c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "Zoom2D",
                    z,"ExecuteZoom2D(Int_t,Int_t,Int_t,TObject*)");
	return;
}
////////////////////////////////////////////////////////////////////////////////////














///////////////////////Integral/////////////////////////////////////
class Integral
{
RQ_OBJECT("Integral")

private:
	Int_t dx1;
	Int_t dy1;
	Int_t dx2;
	Int_t dy2;

public:
	Integral() {dx1=0;dy1=0;dx2=0;dy1=0;}
	~Integral() {}

	void ExecuteIntegral(Int_t event, Int_t px, Int_t py, TObject *sel);
};
void Integral::ExecuteIntegral(Int_t event, Int_t px, Int_t py, TObject *sel)
{

	TCanvas * c = (TCanvas *) gTQSender;
	TPad * pad = (TPad *) c->GetSelectedPad();
	
	if ( event == kButton1Double)
	{
		//--quit--//
		c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
		delete this;
		return;
	}

	//--if hist is a 2d histo--//
	if(sel->InheritsFrom("TH2"))
	{
		TH2 * hist2 = (TH2 *) sel;
		if ( event == kButton1Down )
		{
			//--Get x1,y1--//
   			dx1 = px;
			dy1 = py;
		}
		else if ( event == kButton1Up )
		{
			//--Get Coordinates--//
			Double_t x1 = pad->AbsPixeltoX(dx1);
			Double_t y1 = pad->AbsPixeltoY(dy1);
			Double_t x2 = pad->AbsPixeltoX(px);
			Double_t y2 = pad->AbsPixeltoY(py);
	 
			//--find out which one is bigger an which on is smaller--//
			Double_t xmax = (x1 > x2)? x1 : x2;
			Double_t xmin = (x1 < x2)? x1 : x2;
			Double_t ymax = (y1 > y2)? y1 : y2;
			Double_t ymin = (y1 < y2)? y1 : y2;

			//--find the corosponding bins--//
			Int_t binx1 = hist2->GetXaxis()->FindBin(xmin);
			Int_t binx2 = hist2->GetXaxis()->FindBin(xmax);
			Int_t biny1 = hist2->GetYaxis()->FindBin(ymin);
			Int_t biny2 = hist2->GetYaxis()->FindBin(ymax);

			//--get Integral--//
			Double_t integral = hist2->Integral(binx1,binx2,biny1,biny2);
			hist2->Draw();
			c->Modified();
			c->Update();
			cout << "Integral: "<<integral<<endl;
		 
			//--end program here //
			c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
			delete this;
		}
		else if ( event == kButton1Motion )
		{
			pad->GetCanvas()->FeedbackMode(kTRUE);

 			//--delete old box--//
 			if (dx2 && dy2)
				 gVirtualX->DrawBox(dx1,dy1,dx2,dy2,1);	//box
			// draw new Box //
			gVirtualX->DrawBox(dx1,dy1,px,py,1);	 	
			dx2=px;
			dy2=py;
		}

	}
	//--if hist a 1d histo--//
	else
	{
		if ( event == kButton1Down )
		{
			// Get ymin,ymax// 
			Double_t uymin = pad->GetUymin();
			Double_t uymax = pad->GetUymax();
			dy1 = pad->YtoAbsPixel(uymin);
			dy2 = pad->YtoAbsPixel(uymax);

			dx1 = px;
			pad->SetEditable(kFALSE);
			
			pad->GetCanvas()->FeedbackMode(kTRUE);
			gVirtualX->DrawLine(px,dy1,px,dy2);
		}
		else if ( event == kButton1Up )
		{
			TH1* hist1;

			//--get histo--//
			TList * list = pad->GetListOfPrimitives();
			TObject * obj  = list->First();
			while(obj)
			{
				if (obj->InheritsFrom("TH1"))
				{
					hist1 = (TH1 *) obj;
					break;
				}
				obj = list->After(obj);
			}

			//--get coordinates--//
			Double_t x1 = pad->AbsPixeltoX(dx1);
			Double_t x2 = pad->AbsPixeltoX(px);
		    
   			//--get min and max--//     
			Double_t xmin = (x1 >= x2)? x2:x1;
			Double_t xmax = (x1 <= x2)? x2:x1;

			//--find the corosponding bins--//
			Int_t binx1 = hist1->GetXaxis()->FindBin(xmin);
			Int_t binx2 = hist1->GetXaxis()->FindBin(xmax);

			//--get Integral--//
			Double_t integral = hist1->Integral(binx1,binx2);
			c->Modified();
			c->Update();
			cout << "Integral: "<<integral<<endl;

  			// end program here //
 			pad->SetEditable(kTRUE);
			c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
			delete this;
		}
		else if ( event == kButton1Motion )
		{
			//--delete if drawn before--//
			pad->GetCanvas()->FeedbackMode(kTRUE);
			if (dx2)
				gVirtualX->DrawLine(dx2,dy1,dx2,dy2);
				
			gVirtualX->DrawLine(px,dy1,px,dy2);
			dx2=px;
		}
	}
	return;
}
void int()
{
	Integral * z = new Integral();
	TCanvas *c1 = gPad->GetCanvas();
	c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "Integral",z,"ExecuteIntegral(Int_t,Int_t,Int_t,TObject*)");
	return;
}

double int1D(double from, double to)
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

	double temp = 0;

	if(hist)
	{
		int start = hist->FindBin(from);
		int stop =  hist->FindBin(to);
				
		cout << "startBIN: " <<start << endl << "stopBIN: " <<stop<< endl;

		for(int i=start;i<=stop;i++)
			temp+=hist->GetBinContent(i);
		std::cout << "Integral(" << from << "," << to << ") = " << temp << std::endl << std::endl;
	}

	return temp;
}
//____________________________________________________________________________________________________________________
double int1D(TH1D *hist,double from, double to)
{
	int start = hist->FindBin(from);
	int stop =  hist->FindBin(to);

	double temp = 0;
	for(int i=start;i<=stop;i++)
			temp+=hist->GetBinContent(i);
	return temp;
}
//____________________________________________________________________________________________________________________

int Intcounts(int from, int to)
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

	int temp = 0;

	if(hist)
	{
		int start = hist->FindBin(from);
		int stop =  hist->FindBin(to);
		
		
		for(int i=start;i<stop;i++)
			temp+=hist->GetBinContent(i)*(i-1);
		

		std::cout << "Integral(" << from << "," << to << ") = " << temp << std::endl << std::endl;
	}

	return temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
















/////////////////////Distance///////////////////////////////
class Distance
{
RQ_OBJECT("Distance")

private:
	Float_t x1;
	Float_t y1;
	Float_t x2;
	Float_t y2;
	Float_t fx1;
	Float_t fy1;
	Float_t rx2;
	Float_t ry2;

	void getRectangle(Float_t recx1, Float_t recx2, Float_t &recy1, Float_t &recy2);

public:
	Distance() {x1=0;y1=0;x2=0;y2=0;fx1=0;fy1=0;rx2=0;ry2=0;}
	~Distance() {}

	void ExecuteDistance(Int_t event, Int_t px, Int_t py, TObject *sel);
};
void Distance::ExecuteDistance(Int_t event, Int_t px, Int_t py, TObject *sel)
{

   TCanvas * c = (TCanvas *) gTQSender;
   TPad * pad = (TPad *) c->GetSelectedPad();
   TH2 * hist2 = (TH2 *) sel;

   if ( event == kButton1Double)
   {
		//--quit--//
		cout << endl;
		c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
		delete this;
   }
   else if ( event == kButton1Down )
   {
		//--Get x1,y1--//
		fx1 = px;
		fy1 = py; 
	   	x1 = pad->AbsPixeltoX(fx1);
		y1 = pad->AbsPixeltoY(fy1);
		cout << endl;
   }
   else if ( event == kButton1Up )
   {
		//--Get Coordinates--//
		x2 = pad->AbsPixeltoX(px);
		y2 = pad->AbsPixeltoY(py);
		
		TLine * li = new TLine(x1,y1,x2,y2);
		li->Draw();
		c->Modified();
		c->Update();
 		cout << endl;
    
		//--end program here //
		c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
		delete this;
   }
   else if ( event == kButton1Motion )
   {
		Float_t ix2 = pad->AbsPixeltoX(px);
		Float_t iy2 = pad->AbsPixeltoY(py);

		Float_t fx2 = px;
		Float_t fy2 = py;

		pad->GetCanvas()->FeedbackMode(kTRUE);

     	//--delete old line--//
     	if (rx2 && ry2)
			 gVirtualX->DrawLine(fx1,fy1,rx2,ry2);
		// draw new line //
		gVirtualX->DrawLine(fx1,fy1,fx2,fy2);	 	
		rx2=fx2;
		ry2=fy2;

		cout << "\rDistance: "<< sqrt((x1-ix2)*(x1-ix2)+ (y1-iy2)*(y1-iy2));
   }
   return;
}
void dist()
{
	Distance * z = new Distance();
	TCanvas *c1 = gPad->GetCanvas();
	c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "Distance",
                    z,"ExecuteDistance(Int_t,Int_t,Int_t,TObject*)");
	return;
}
////////////////////////////////////////////////////////////////////////////////////

















//////////////////////////////////test3d///////////////////////////////////////////
void test3d()
{
	TH3D * threed = new TH3D("hpxpypz","py vs px vs pz",200,-2,2,200,-2,2,200,0,40);
	threed->SetXTitle("x");
	threed->GetXaxis()->CenterTitle(true);
	threed->SetYTitle("y");
	threed->GetYaxis()->CenterTitle(true);
	threed->GetYaxis()->SetTitleOffset(1.5);
	threed->SetZTitle("z");
	threed->GetZaxis()->CenterTitle(true);
	threed->GetZaxis()->SetTitleOffset(1.5);
	threed->SetOption("colz");
	double px,py,pz;
	for (int i = 0; i < 5000; i++)
	{
		gRandom->Rannor(px,py);
		pz = (gRandom->Rndm())*40;
		threed->Fill(px,py,pz,1.);
	}

	
	threed->Draw();
}
///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////test2d///////////////////////////////////////////
void test2d()
{
	TH2D * twod = new TH2D("hpxpy","py vs px",200,-4,4,200,-4,4);
	twod->SetOption("colz");
	Double_t px,py;
	for (Int_t i = 0; i < 500000; i++)
	{
		gRandom->Rannor(px,py);
		twod->Fill(px,py);
	}
	twod->Draw();
	return;
} 
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////test1d///////////////////////////////////////////
void test1d()
{
	TH1D * oned = new TH1D("hpx","px",40,-4,4);
	Double_t px,py;
	for (Int_t i = 0; i < 50000; i++)
	{
		gRandom->Rannor(px,py);
		oned->Fill(px);
	}
	oned->Draw();
	return;
}
/////////////////////////////////////////////////////////////////////////////
















/////////////////////Circle///////////////////////////////
void circle(double cx, double cy, double r)
{
	//--Draw an circle that stayes--//
	TArc * ar = new TArc(cx,cy,r);
	ar->SetLineColor(kRed);
	ar->SetLineWidth(2);
	ar->SetFillStyle(0);
	ar->Draw();
	return;
}
class MyCircle
{
RQ_OBJECT("MyCircle")
public:
	MyCircle(double centerX, double centerY, bool findCenterFirst) 
		    {cx = centerX; cy = centerY;fcf = findCenterFirst;rr=0;
		     rpx=0;rpy=0;bx=0;by=0;}
	~MyCircle() {}

	void ExecuteCircle(Int_t event, Int_t px, Int_t py, TObject *sel);

private:
	void drawArc(double radius);

	double cx,cy,rr;
	bool fcf;
	int rpx,rpy,bx,by;
};
void MyCircle::drawArc(double radius)
{
	const int nbrp = 360;
	TPoint * tp = new TPoint[nbrp+1];
	double angle;

	for (int i=0; i<= nbrp;++i)
	{
		angle = 2.*TMath::Pi()*(double)i/(double)nbrp;
		tp[i].SetX(gPad->XtoAbsPixel((TMath::Cos(angle)*radius)+cx));
		tp[i].SetY(gPad->YtoAbsPixel((TMath::Sin(angle)*radius)+cy));
	}
	gVirtualX->DrawPolyLine(nbrp+1,tp);
	delete[] tp;
	return;
}
void MyCircle::ExecuteCircle(Int_t event, Int_t px, Int_t py, TObject *sel)
{
   TCanvas * c = (TCanvas *) gTQSender;
   TPad * pad = (TPad *) c->GetSelectedPad();

   //--if we need to find the center for the circle first, do it--//
   if (fcf)
   {
	   if ( event == kButton1Down)
	   {
		    //--remember x and y position--//
		    bx = px;
		    by = py;
	   }
	   else if ( event == kButton1Up)
	   {
		   fcf = false;
		   //--calculate the center--//
   		   double x1 = pad->AbsPixeltoX(px);
   		   double x2 = pad->AbsPixeltoX(bx);
   		   double xbmin = (x1>x2)?x2:x1;
   		   double xbmax = (x1<x2)?x2:x1;
   		   cx = xbmin + (0.5 * fabs(x1-x2));
   		   
		   double y1 = pad->AbsPixeltoY(py);
		   double y2 = pad->AbsPixeltoY(by);
   		   double ybmin = (y1>y2)?y2:y1;
   		   double ybmax = (y1<y2)?y2:y1;
   		   cy = ybmin +(0.5 * fabs(y1-y2));
	   }
	   else if ( (event == kMouseMotion)  || (event == kButton1Motion) )
	   {
	   		pad->GetCanvas()->FeedbackMode(kTRUE);
	   		//--get xmin,xmax,ymin,ymax--//
			double uxmin = pad->GetUxmin();
			double uxmax = pad->GetUxmax();
			int xmin = pad->XtoAbsPixel(uxmin);
			int xmax = pad->XtoAbsPixel(uxmax);

			double uymin = pad->GetUymin();
			double uymax = pad->GetUymax();
			int ymin = pad->YtoAbsPixel(uymin);
			int ymax = pad->YtoAbsPixel(uymax);
				   		
			//--delete old crosshair--//
			if(rpx && rpy)
			{
				gVirtualX->DrawLine(xmin,rpy,xmax,rpy);		//horizontal line
				gVirtualX->DrawLine(rpx,ymin,rpx,ymax);		//vertical line
			}
			//--draw crosshair--//
			gVirtualX->DrawLine(xmin,py,xmax,py);	//horizontal line
			gVirtualX->DrawLine(px,ymin,px,ymax);	//vertical line
			
			//--if we now one point of the box (mouse button is clicked)--//
			if(event == kButton1Motion)
			{ 
				//--first delete the old box--//
				gVirtualX->DrawBox(bx,by,rpx,rpy,1);
				//--Draw Box--//
				gVirtualX->DrawBox(bx,by,px,py,1);
			}
			
			//--remember values--//			
			rpy = py;
			rpx = px;
	   }
   }
   //--if we have the center, draw the circle around it--//
   else
   {
	   if ( event == kButton1Down)
	   {
		    //--draw permanent circle--//
			double x = pad->AbsPixeltoX(px)-cx;
			double y = pad->AbsPixeltoY(py)-cy;
			double r = TMath::Sqrt(x*x+y*y);
			circle(cx,cy,r);
			
			//--draw angle line--//
			TLine * l = new TLine(cx,cy,pad->AbsPixeltoX(px),pad->AbsPixeltoY(py));
			l->SetLineColor(kRed);
			l->SetLineWidth(2);
			l->Draw();

			double phi = TMath::ATan2(y,x)*180./3.14;
			phi = (phi < 0)?phi+360.:phi;
			
			//--quit--//
		    cout << "Center is: x=" << cx << " y="<<cy<<endl;
			cout << "Radius: "<<r<<endl;
			cout << "Phi: "<<phi<<endl;
			cout << "exiting circle"<<endl;
			c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
			delete this;
	   }
	   else if ( event == kMouseMotion )
	   {
	   		pad->GetCanvas()->FeedbackMode(kTRUE);
	   		//--get radius and phi--//
			double x = pad->AbsPixeltoX(px)-cx;
			double y = pad->AbsPixeltoY(py)-cy;
			double r = TMath::Sqrt(x*x+y*y);
			
			int pxl = pad->XtoAbsPixel(cx);
			int pyl = pad->YtoAbsPixel(cy);
			double phi = TMath::ATan2(y,x)*180./3.14;
			phi = (phi < 0)?phi+360.:phi;
			
			//--delete old circle--//
			if(rr) drawArc(rr);
			//--draw circle--//
		    drawArc(r);
		    
		    //--delete old angle line--//
			if (rr) gVirtualX->DrawLine(pxl,pyl,rpx,rpy);
		    //--draw angle line--//
			gVirtualX->DrawLine(pxl,pyl,px,py);
		    
		    //--remember values--//
		    rpx=px;
		    rpy=py;
			rr=r;
	   }
   }
   return;
}
void circle(double centerX, double centerY, bool findCenterFirst)
{
	MyCircle * circ = new MyCircle(centerX,centerY,findCenterFirst);
	TCanvas *c1 = gPad->GetCanvas();
	c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "MyCircle",
                    circ,"ExecuteCircle(Int_t,Int_t,Int_t,TObject*)");
	return;
}
void circle()
{
	circle(0,0,false);
	return;
}
void circle(double cx, double cy)
{
	circle(cx,cy,false);
	return;
}
void circle(bool findCfirst)
{
	circle(0,0,findCfirst);
	return;
}
/////////////////////////////////////////////////////////////////////////////















///////////////////////projection for 3d histograms/////////////////////////////////////
TH1* pro3d(char * option, double fr, double to,char * title="", TPad * pad=0)
{
	TH3 * hist3=0;
	
	//find histogram in pad//
	TIter next(gPad->GetListOfPrimitives());
	TObject * obj;
	while((obj = next()))
	{
		if (obj->InheritsFrom("TH3"))
		{
			hist3 = (TH3*) obj;
			break;
		}
	}
	if (!hist3){cout <<"No 3D-Histogram in Pad"<<endl; return 0;}

	return (pro3d(hist3,option,fr,to,title,pad));
}
TH1* pro3d(TH3 * hist, char * option, double fr, double to, char * title="", TPad * pad=0)
{
	TH1 * h;
	TAxis * ax;
	char tmp[64];
	TString opt = option; opt.ToLower();

	//first unzoom all other axes one can zoom later in the 2d histogram//
	hist->GetXaxis()->UnZoom();
	hist->GetYaxis()->UnZoom();
	hist->GetZaxis()->UnZoom();
	static int count;
	if ((opt.Contains("xy")) || (opt.Contains("yx")))
		ax = hist->GetZaxis();
	if ((opt.Contains("xz")) || (opt.Contains("zx")))
		ax = hist->GetYaxis();
	if ((opt.Contains("zy")) || (opt.Contains("yz")))
		ax = hist->GetXaxis();
	
	//set the range of the axis//
	double min = (fr > to)? to:fr;
	double max = (fr > to)? fr:to;
	ax->SetRangeUser(min,max);
	if ((ax->GetXmin() > fr) || (ax->GetXmax() < to))
	{
		cout << "The given Range is out of Limits for Axis \""<< ax->GetTitle()<< "\""<<endl;
		int fi = ax->GetFirst();
		int la = ax->GetLast();
		cout << "The range is set to "<< ax->GetBinLowEdge(fi)<< " ... "<<ax->GetBinUpEdge(la)<<endl;
	}

	//project the histogram//
	h = hist->Project3D(option);

	//unzoom the axis range//
	ax->UnZoom();
	
	//set the attributes to the projected histo//
	h->SetOption("colz");
	sprintf(tmp,"project_%s_%i",h->GetName(),count++);
	h->SetName(tmp);
	TString tit = title;
	if(tit == "") title = hist->GetTitle();
	h->SetTitle(title);
	if (!pad)
	{
		TCanvas * c2 = Canv(1,1);
		c2->cd();
		pad = gPad;
	}
	pad->cd();
	h->Draw();

	return h;
}
/////////////////////////////////////////////////////////////////////////














///////////////////////projection to X/////////////////////////////////////
TH1* prox(double fr, double to, TPad * pad = 0)
{
	TH2 * hist2=0;
	
	//find histogram in pad//
	TIter next(gPad->GetListOfPrimitives());
	TObject * obj;
	while((obj = next()))
	{
		if (obj->InheritsFrom("TH2"))
		{
			hist2 = (TH2*) obj;
			break;
		}
	}
	if (!hist2){cout <<"No 2D-Histogram in Pad"<<endl; return 0;}

	return (prox(hist2,fr,to,pad));
}
TH1 * prox(TH2* hist2, double fr, double to, TPad *pad = 0, bool d=true)
{
	static int count;
	char title[80];

	int biny1 = hist2->GetYaxis()->FindBin(fr);
	int biny2 = hist2->GetYaxis()->FindBin(to);
	
	int binymin = (biny1 >= biny2)? biny2:biny1;
	int binymax = (biny1 <= biny2)? biny2:biny1;
	
	TH1D * pro = hist2->ProjectionX("",binymin,binymax);
	double ymin = hist2->GetYaxis()->GetBinLowEdge(binymin);
	double ymax = hist2->GetYaxis()->GetBinUpEdge(binymax);
	
	sprintf(title,"Pro to X from y=%f to %f",ymin,ymax);
	pro->SetTitle(title);
	sprintf(title,"projectx_%s%i",hist2->GetName(),count++);
	pro->SetName(title);
	if (d)
	{
		if (!pad)
		{
			TCanvas * c2 = Canv(1,1);
			c2->cd();
			pad = gPad;
		}
		pad->cd();
		pro->Draw();
	}
	
	return ((TH1*)pro);
}
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

void ProX::ExecuteProx(Int_t event, Int_t px, Int_t py, TObject *sel)
{
	//--if selected is not the 2d histo remove the lines & return--//
	if(!sel->InheritsFrom("TH2"))
	{
		if (remy)
			gVirtualX->DrawLine(xmin,remy,xmax,remy);
		if (remy1 && remy2)
		{
			gVirtualX->DrawLine(xmin,remy1,xmax,remy1);
			gVirtualX->DrawLine(xmin,remy2,xmax,remy2);
		}

		remy=0;
		remy1=0;
		remy2=0;
		return;
	}

	TCanvas * c = (TCanvas *) gTQSender;
	TPad * pad = (TPad *) c->GetSelectedPad();
	TH2 * hist2 = (TH2 *) sel;

	
	if (pmbins== -1)
	{
	   if ( event == kButton1Double)
	   {
			//--quit--//
			c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
			delete this;
	   }
	   else if ( event == kButton1Down )
	   {
			// Get xmin,xmax// 
			uxmin = pad->GetUxmin();
			uxmax = pad->GetUxmax();
			xmin = pad->XtoAbsPixel(uxmin);
			xmax = pad->XtoAbsPixel(uxmax);
			
			//--get y--//
			y1 = pad->AbsPixeltoY(py);
	
			pad->GetCanvas()->FeedbackMode(kTRUE);
			gVirtualX->DrawLine(xmin,py,xmax,py);
	   }
	   else if ( event == kButton1Up )
	   {	
		   	//--get y--//     
			y2 = pad->AbsPixeltoY(py);
			
			prox(y1,y2);

		   	// end program here //
	    	c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
			delete this;
	   }
	   else if ( event == kButton1Motion )
	   {
			//--delete if drawn before--//
			pad->GetCanvas()->FeedbackMode(kTRUE);
			if (remy)
				gVirtualX->DrawLine(xmin,remy,xmax,remy);
				
			gVirtualX->DrawLine(xmin,py,xmax,py);
			remy=py;
		}
	
	}
	else
	{
		if ( event == kMouseMotion )
		{
			//--check if we are inside the histo--//
			
			// Get xmin,xmax// 
			uxmin = pad->GetUxmin();
			uxmax = pad->GetUxmax();
			xmin = pad->XtoAbsPixel(uxmin);
			xmax = pad->XtoAbsPixel(uxmax);
			
			//--get biny--//
			y = pad->AbsPixeltoY(py);
			biny1 = hist2->GetYaxis()->FindBin(y);
	
			
			binymin = biny1 - pmbins;
	    	binymax = biny1 + pmbins;
		   	ymin = hist2->GetYaxis()->GetBinLowEdge(binymin);
		   	ymax = hist2->GetYaxis()->GetBinUpEdge(binymax);
			py1 = pad->YtoAbsPixel(ymin);
			py2 = pad->YtoAbsPixel(ymax);
	    	
	    	
		   	pro = hist2->ProjectionX("",binymin,binymax);
		   	
		   	if (!c2)
		   	{
			   	c2 = Canv(1,1);
				c2->cd();
				pro->Draw();
	   		}
	   		c2->Modified();
			c2->Update();
			
			pad->GetCanvas()->FeedbackMode(kTRUE);
			if (remy1 && remy2)
			{
				gVirtualX->DrawLine(xmin,remy1,xmax,remy1);
				gVirtualX->DrawLine(xmin,remy2,xmax,remy2);
			}
			gVirtualX->DrawLine(xmin,py1,xmax,py1);
			gVirtualX->DrawLine(xmin,py2,xmax,py2);
	
			remy1 = py1;
			remy2 = py2;
		}
		else if ( event == kButton1Down)
		{
		   	sprintf(title,"Pro to X from y=%f to %f",ymin,ymax);
			pro->SetTitle(title);
			static int count;
			sprintf(title,"projectx_%s%i",hist2->GetName(),count++);
			pro->SetName(title);
			c2->ToggleEventStatus();
			c2->Modified();
			c2->Update();

			//--quit--//
			c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
			delete this;
		}
	}
}

void prox(int pmbins=-1)
{
	ProX * z = new ProX(pmbins);
	TCanvas *c1 = gPad->GetCanvas();
	c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "ProX",z,"ExecuteProx(Int_t,Int_t,Int_t,TObject*)");
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




















////////////////////////////////projection to y/////////////////////////////////////////////////////
TH1* proy(double fr, double to, TPad * pad = 0)
{
	TH2* hist2=0;
	
	//find histogram in pad//
	TIter next(gPad->GetListOfPrimitives());
	TObject *obj;
	while((obj = next()))
	{
		if (obj->InheritsFrom("TH2"))
		{
			hist2 = (TH2*) obj;
			break;
		}
	}
	if (!hist2){cout <<"No 2D-Histogram in Pad"<<endl; return 0;}

	return (proy(hist2,fr,to,pad));
}

TH1* proy(TH2 * hist2, double fr, double to, TPad * pad = 0)
{
	static int count;
	char title[80];

	int binx1 = hist2->GetXaxis()->FindBin(fr);
	int binx2 = hist2->GetXaxis()->FindBin(to);
	
	int binxmin = (binx1 >= binx2)? binx2:binx1;
	int binxmax = (binx1 <= binx2)? binx2:binx1;
	double xmin = hist2->GetXaxis()->GetBinLowEdge(binxmin);
	double xmax = hist2->GetXaxis()->GetBinUpEdge(binxmax);
	
	TH1D * pro = hist2->ProjectionY("",binxmin,binxmax);
	
	sprintf(title,"Pro to Y from x=%f to %f",xmin,xmax);
	pro->SetTitle(title);
	sprintf(title,"projecty_%s%i",hist2->GetName(),count++);
	pro->SetName(title);
	if (!pad)
	{
		TCanvas * c2 = Canv(1,1);
		c2->cd();
		pad = gPad;
	}
	pad->cd();
	pro->Draw();
	

	return ((TH1*) pro);
}


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


void ProY::ExecuteProy(Int_t event, Int_t px, Int_t py, TObject *sel)
{
	//--if selected is not the 2d histo remove the lines & return--//
	if(!sel->InheritsFrom("TH2"))
	{
		if (remx)
			gVirtualX->DrawLine(remx,ymin,remx,ymax);
		if (remx1 && remx2)
		{
			gVirtualX->DrawLine(remx1,ymin,remx1,ymax);
			gVirtualX->DrawLine(remx2,ymin,remx2,ymax);
		}

		remx=0;
		remx1=0;
		remx2=0;
		return;
	}

	TCanvas * c = (TCanvas *) gTQSender;
	TPad * pad = (TPad *) c->GetSelectedPad();
	TH2 * hist2 = (TH2 *) sel;

	
	if (pmbins== -1)
	{
	   if ( event == kButton1Double)
	   {
			//--quit--//
			c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
			delete this;
	   }
	   else if ( event == kButton1Down )
	   {
			// Get xmin,xmax// 
			uymin = pad->GetUymin();
			uymax = pad->GetUymax();
			ymin = pad->YtoAbsPixel(uymin);
			ymax = pad->YtoAbsPixel(uymax);
			
			//--get biny--//
			x1 = pad->AbsPixeltoX(px);

			pad->GetCanvas()->FeedbackMode(kTRUE);
			gVirtualX->DrawLine(px,ymin,px,ymax);
	   }
	   else if ( event == kButton1Up )
	   {	
		   	//--get biny--//     
			x2 = pad->AbsPixeltoX(px);
			proy(x1,x2);
		   	
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
	
	}
	else
	{
		if ( event == kMouseMotion )
		{
			// Get xmin,xmax// 
			uymin = pad->GetUymin();
			uymax = pad->GetUymax();
			ymin = pad->YtoAbsPixel(uymin);
			ymax = pad->YtoAbsPixel(uymax);
			
			//--get biny--//
			x = pad->AbsPixeltoX(px);
			binx1 = hist2->GetYaxis()->FindBin(x);
	
			
			binxmin = binx1 - pmbins;
	    	binxmax = binx1 + pmbins;
		   	xmin = hist2->GetXaxis()->GetBinLowEdge(binxmin);
		   	xmax = hist2->GetXaxis()->GetBinUpEdge(binxmax);
			px1 = pad->XtoAbsPixel(xmin);
			px2 = pad->XtoAbsPixel(xmax);
	    	
	    	
		   	pro = hist2->ProjectionY("",binxmin,binxmax);
		   	
		   	if (!c2)
		   	{
			   	c2 = Canv(1,1);
				c2->cd();
				pro->Draw();
	   		}
	   		c2->Modified();
			c2->Update();
			
			pad->GetCanvas()->FeedbackMode(kTRUE);
			if (remx1 && remx2)
			{
				gVirtualX->DrawLine(remx1,ymin,remx1,ymax);
				gVirtualX->DrawLine(remx2,ymin,remx2,ymax);
			}
			gVirtualX->DrawLine(px1,ymin,px1,ymax);
			gVirtualX->DrawLine(px2,ymin,px2,ymax);
	
			remx1 = px1;
			remx2 = px2;
		}
		else if ( event == kButton1Down)
		{
		   	sprintf(title,"Pro to Y from x=%f to %f",xmin,xmax);
			pro->SetTitle(title);
			static int count;
			sprintf(title,"project_%s%i",hist2->GetName(),count++);
			pro->SetName(title);
			c2->Modified();
			c2->Update();

			//--quit--//
			c->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
			delete this;
		}
	}
}

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
	DrawSum(const char * lay, const char * v) {var = v;layer = lay;cntr=0;firsttime=true;}
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
void drawsum(const char * v, const char * layer)
{
	DrawSum * ds = new DrawSum(layer,v);
	TCanvas *c1 = gPad->GetCanvas();
	c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "DrawSum",ds,"ExecuteDrawSum(Int_t,Int_t,Int_t,TObject*)");
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////


















/////////////////////////////////////////////gaus()/////////////////////////////////////////////////////////////
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















/////////////////////////////stuff that is needed when the root file is still beeing filled while watched////////////////////////////////
void showHist(char *name, TPad *pad = 0, bool upd=false) 
{
	Int_t xfirst,xlast,yfirst,ylast;
	TH1* hist;

	// if no pad is assigned, choose the currently highlighted pad //
	if(!pad) pad = gPad;


	TObject *obj = _file0->FindObject(name);
	//if histo was there before remember axis settings before deleting//
	if (upd && obj)
	{
		hist = (TH1*) obj;
		xfirst = hist->GetXaxis()->GetFirst();
		xlast = hist->GetXaxis()->GetLast();
		if (obj->InheritsFrom("TH2"))	//if it is a 2d hist
		{
			yfirst = hist->GetYaxis()->GetFirst();
			ylast = hist->GetYaxis()->GetLast();
		}
	}

	//delete the old histogram and load the new one from file//
	delete obj;
	_file0->ReadKeys();
	hist = (TH1*)_file0->Get(name);
	//if we are in update mode and it was not able to load the histo, retry until you succeed//
	while(upd && !hist)
	{	
		_file0->ReadKeys();
		hist = (TH1*)_file0->Get(name);
		gSystem->Wait(10);
	}

	if (hist)
	{
		//set the axis settings to the remembered values//
		hist->GetXaxis()->SetRange(xfirst,xlast);
		if (hist->InheritsFrom("TH2"))
			hist->GetYaxis()->SetRange(yfirst,ylast);

		//draw the histogram into the pad//
		pad->cd();
		hist->Draw();
		pad->Modified();
		pad->GetCanvas()->Update();
		
	}
	return;
}
void UPDATE()
{
	TString upHi[20];
	TPad* pads[20];
	long upHicnt=0;
	//loop through all canvases
	TIter next(gROOT->GetListOfCanvases());
	TCanvas * can;
	TPad * pad;
	TObject * obj;
	while((can = (TCanvas*) next()))
	{
		//loop through all pads in the canvases
		TIter next2(can->GetListOfPrimitives());
		while((pad = (TPad*)next2()))
		{
			//loop through all objects of this pad
			TIter next3(pad->GetListOfPrimitives());
			while((obj = next3()))
			{
				//if it is a histogram, remember its name and the pad it was in
				if (obj->InheritsFrom("TH1"))
				{
					pads[upHicnt] = pad;
					upHi[upHicnt++] = obj->GetName();
					if (upHicnt == 20){cout <<"cannot upate more then 20 histograms at a time"<<endl; upHicnt=0;}
				}
			}
		}
	}

	//update all found histograms
	for (int i=0;i<upHicnt;++i)
		showHist(upHi[i].Data(),pads[i],true);

	return;
} 
TTimer *timer = new TTimer();
void update(int time=-2)
{
	if (time == -2)
	{
		timer->Stop();
		UPDATE();
		return;
	}

	if (time == -1)
	{
		timer->Stop();
		return;
	}

	timer->SetCommand("UPDATE()");
	timer->Start(time*1000);
	return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////












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

double pipico(double Mass_amu, double charge_au, double EField1_Vpcm, double EField2_Vpcm, double LenReg1_mm, double LenReg2_mm, double MaxEnergy_ev)
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
	TH1D*hist1;
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

	f1 = new TF1("f1",formula,xmin,xmax);

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
	ofstream tempstream(filename,ios::out);
	
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
	ofstream tempstream(filename,ios::out);

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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multiply_SINE()
{
	TH2D * hist;
	TIter next(gPad->GetListOfPrimitives());
	TObject *obj;
	while((obj = next()))
	{
		if(obj->InheritsFrom("TH2"))
		{
			hist = (TH2D *) obj;
			break;
		}
	}

	if(hist)
	{
		// create SINEfunction
		TF1 sine("sine","sin(x/180.*3.14159)",hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
		// make a copy of the old histogramm
		TH2D *histcopy = hist->Clone("newname");
		histcopy->SetOption("colz");
		// multiply it by the SINEfunction
		//histcopy->Multiply(&f1);	

		for(int iy=0;iy <= histcopy->GetNbinsY(); iy++) // go through all KERs
		{
			for(int ix=0;ix <= histcopy->GetNbinsX();ix++) // go through all angles
			{
				if(sine.Eval(histcopy->GetXaxis()->GetBinCenter(ix)))
				{
					double sinx = sine.Eval(histcopy->GetXaxis()->GetBinCenter(ix));
					histcopy->SetBinContent(ix,iy,hist->GetBinContent(ix,iy)/sinx);
				}
			}
		}
		
		//open new Canvas and plot the new Histogramm onto it
		TCanvas * c1 = Canv(1,1);
		c1->cd();
		histcopy->Draw();

	}

	return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multiply_SINE_1D()
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

	if(hist)
	{
		// create SINEfunction
		TF1 sine("sine","sin(x/180.*3.14159)",hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
		// make a copy of the old histogramm
		TH1D *histcopy = hist->Clone("newname");
		//histcopy->SetOption("colz");
		// multiply it by the SINEfunction
		//histcopy->Multiply(&f1);

		for(int ix=0;ix <= histcopy->GetNbinsX();ix++) // go through all angles
		{
				double sinx = sine.Eval(histcopy->GetXaxis()->GetBinCenter(ix));
				if(sinx == 0)	sinx = 1.e-10;
				histcopy->SetBinContent(ix,hist->GetBinContent(ix)/sinx);
		}
		
		
		//open new Canvas and plot the new Histogramm onto it
		TCanvas * c1 = Canv(1,1);
		c1->cd();
		histcopy->Draw();

	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void normiereHist_a_auf_b(TH1D *a,TH1D *b, const char* newname)
{
	// überprüfe auf gleiche Anzahl an bins
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

