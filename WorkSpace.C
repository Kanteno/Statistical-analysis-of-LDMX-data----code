#include "RooStats/HistFactory/MakeModelAndMeasurementsFast.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooRandom.h"
#include "TFile.h"
#include "TAxis.h"
#include "RooHistPdf.h"
#include <cmath>

#include "RooStats/ModelConfig.h"

using namespace std;
using namespace RooStats;
using namespace RooFit;

void WorkSpace(
						const char* infile =  "Data_File/symmUncert_analysisHistograms.root",
						const char* sigName =  "1",
						const char* sigNb =  "1"
		)
{
	
	// open input file 
	TFile *file = TFile::Open(infile);
	if (!file) return;
	
	
	// get the Data and backgrounda and sys up/dn out of the file
	TH1F* Sig = (TH1F*) file->Get(Form("hS%s", sigName));
	TH1F* Bkg = (TH1F*) file->Get("hBkgd");
	TH1F* Data = (TH1F*) file->Get(Form("hData%s", sigNb));
	
	TH1F* SigEUp = (TH1F*) file->Get( Form("%sUp", Sig->GetName()) );
	TH1F* BkgEUp = (TH1F*) file->Get("hBkgdUp");
	TH1F* SigEDn = (TH1F*) file->Get( Form("%sDn", Sig->GetName()) );
	TH1F* BkgEDn = (TH1F*) file->Get("hBkgdDn");
	
	
	if(!Bkg or !Data or !Sig or !BkgEUp or !SigEUp or !BkgEDn or !SigEDn){ printf("Hist not extracted correctly"); return; };
	

	cout<<endl<<"here"<<endl;
	/////////////////////////////////////////////////////////////////////
	/////////Fixing hists by adding a number also sys error//////////////
	/////////////////////////////////////////////////////////////////////
	
	double nuisance = pow(10,-9);
	double nuisanceE = pow(10,-9);
	double ErrorB = 0;
	double ErrorD = 0;
	double ErrorS = 0;
	
	double ErrorBUp = 0;
	double ErrorSUp = 0;
	double ErrorBDn = 0;
	double ErrorSDn = 0;
	
	double AvgB = 0;
	double AvgS = 0;
	
	for(int i=1; i<15; i++)
	{
		
		ErrorB = Bkg->GetBinError(i);
		ErrorD = Data->GetBinError(i); 
		ErrorS = Sig->GetBinError(i); 
		
		ErrorBUp = BkgEUp->GetBinError(i);
		ErrorSUp = SigEUp->GetBinError(i); 
		ErrorBDn = BkgEDn->GetBinError(i);
		ErrorSDn = SigEDn->GetBinError(i); 
		
		AvgB = 0.5*(ErrorBUp+ErrorBDn);
		AvgS = 0.5*(ErrorSUp+ErrorSDn);
		
		ErrorB = pow( pow(ErrorB,2) + pow(AvgB,2) ,0.5);
		ErrorS = pow( pow(ErrorS,2) + pow(AvgS,2) ,0.5);
		
		///// Ensuring min error and sig lest code break /////
		
		Bkg->SetBinError(i,ErrorB);
		Sig->SetBinError(i,ErrorS);
		
		if(ErrorB==0)
		{
			Bkg->SetBinContent(i,nuisance);
			Bkg->SetBinError(i,nuisanceE);
		}
		if(ErrorD == 0)
		{
			Data->SetBinContent(i,nuisance);
			Data->SetBinError(i,nuisanceE);
		}
	}

	//TCanvas *c1 = new TCanvas();
	//Bkg->Draw();
	
	//create the histfactory model
	RooStats::HistFactory::Measurement meas("SimpleNumberCounting", "SimpleNumberCounting"); 
	meas.SetOutputFilePrefix("SimpleNumberCounting/tut");
	meas.SetExportOnly(1);
	meas.SetPOI("mu");

	// this scales the histogram content, which already includes lumi, so set to 1
	meas.SetLumi(1.0);
	meas.SetLumiRelErr(0.000001);
	
	

	//create the parameters for the model

	//first the signal normalization
	RooStats::HistFactory::NormFactor normS;
	normS.SetName("mu");
	normS.SetHigh(100); // maximum value it can take
	normS.SetLow(0); // minimum value it can take
	normS.SetVal(1); // nominal value

	//create the SR
	RooStats::HistFactory::Channel SR("SR");
	SR.SetData(Data);

	//add the signal and background samples
	RooStats::HistFactory::Sample sample_S("S");
	sample_S.SetHisto(Sig);
	sample_S.AddNormFactor(normS);
	SR.AddSample(sample_S);
	
	
	cout<<"hello"<<endl;
	
	
	RooStats::HistFactory::Sample sample_B("B");
	sample_B.SetHisto(Bkg);
	SR.AddSample(sample_B);
	
	cout<<"hi"<<endl;
	
	//add the single region to the measurement
	meas.AddChannel(SR);
	//meas.CollectHistograms();
	
	//meas.PrintTree();
	

	//make the workspace
	RooStats::HistFactory::MakeModelAndMeasurementFast(meas);

	cout<<Bkg->GetBinError(14)<<endl;

}


















