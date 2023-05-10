#include "RooWorkspace.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooRandom.h"
#include "TFile.h"
#include "TCanvas.h"

#include "RooStats/ModelConfig.h"
#include "RooStats/AsymptoticCalculator.h"
#include "RooStats/FrequentistCalculator.h"
#include "RooStats/ProfileLikelihoodTestStat.h"
#include "RooStats/ToyMCSampler.h"
#include "RooStats/HypoTestPlot.h"


using namespace RooFit; 
using namespace RooStats;

void HistHypoF(
	const char* infile =  "GausExpModel1.root", 
	const char* workspaceName = "w",
	const char* modelConfigName = "ModelConfig",
	const char* dataName = "data")
{
	
	TFile* f = new TFile(Form("SimpleNumberCounting/tut_combined_SimpleNumberCounting_model.root"));
	RooWorkspace* w = (RooWorkspace*)f->Get("combined");
	if (!w)
	{
	cout << "ERROR::Workspace doesn't exist! Check file name" << endl;
	exit(1);
	}
	//Grab the ModelConfig and the data
	ModelConfig* mc = (ModelConfig*)w->obj("ModelConfig");
	RooDataSet* data = (RooDataSet*)w->data("obsData");
	
	
//-------------------------------------------------------------------------------------------------------
	
	ModelConfig*  sbModel = (ModelConfig*) w->obj(modelConfigName); // Alt Hypothesis
	sbModel->SetName("Alt Hypothesis");
	RooRealVar* poi = (RooRealVar*) sbModel->GetParametersOfInterest()->first();
	cout<< "HEHEHEHEHEHEHEHEHEHEEREREREREREJRJERE"<<poi<<endl;
	poi->setVal(1);
	sbModel->SetSnapshot(*poi);
	
	ModelConfig * bModel = (ModelConfig*) sbModel->Clone(); // NUll Hypothesis
	bModel->SetName("Null Hypothesis");
	poi->setVal(0);
	bModel->SetSnapshot(*poi);

//-----------------------------------------------------------------------------------------------------
	// create the AsymptoticCalculator: data, alt model, null model
	AsymptoticCalculator  ac(*data, *sbModel, *bModel);
	//ac.SetOneSidedDiscovery(true);  // for one-side discovery test
	ac.SetPrintLevel(-1);  // to suppress print level -- I have no clue if this actualy makes a difference

	// run the calculator
	HypoTestResult * asResult = ac.GetHypoTest();
	asResult->Print();
	
	
	return;
//------------------------------------------------------------------------------------------------
	std::cout << "\n\nRuning FrequentistCalculator.....\n";   

	FrequentistCalculator   fc(*data, *sbModel, *bModel);   
	fc.SetToys(10000,10000);    // 2000 for null (B) and 500 for alt (S+B)    
	
	// create the test statistics   
	ProfileLikelihoodTestStat profll(*sbModel->GetPdf());
	
	profll.SetOneSidedDiscovery(true);
	
	
	// configure  ToyMCSampler and set the test statistics
	ToyMCSampler *toymcs = (ToyMCSampler*)fc.GetTestStatSampler();
	toymcs->SetTestStatistic(&profll);

	if (!sbModel->GetPdf()->canBeExtended()) toymcs->SetNEventsPerToy(1);

	// run the test
	HypoTestResult * fqResult = fc.GetHypoTest();
	fqResult->Print();

	// plot test statistic distributions
	HypoTestPlot * plot = new HypoTestPlot(*fqResult);
	plot->SetLogYaxis(true);
	plot->Draw();

	
	
	
}
