#include "RooWorkspace.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooRandom.h"
#include "TFile.h"

#include "RooStats/ModelConfig.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodIntervalPlot.h"

#include "RooStats/HypoTestResult.h"


using namespace RooFit; 
using namespace RooStats;

void HistPLCalculatorF(  
	const char* infile =  "Data_File/analysisHistograms.root", 
	const char* workspaceName = "w",
	const char* modelConfigName = "ModelConfig",
	const char* dataName = "obsdata" )


{
	/////////////////////////////////////////////////////////////
	// First part is just to access the workspace file 
	////////////////////////////////////////////////////////////

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
	
//----------------------------------------------------------------------------------------------

	ProfileLikelihoodCalculator pl(*data,*mc);
	//ProfileLikelihoodCalculator plc(*data,*mc);
	
	pl.SetConfidenceLevel(0.68); // 68% interval -- also known as 1 sigma
	
//------------------------------------------------------------------------------------------------

////////////Interval liklihood calculator/////////////////

	LikelihoodInterval* interval = pl.GetInterval();// running the calculator


	
	// find the iterval on the first Parameter of Interest
	RooRealVar* firstPOI = (RooRealVar*) mc->GetParametersOfInterest()->first();
	

	
	double lowerLimit = interval->LowerLimit(*firstPOI);
	double upperLimit = interval->UpperLimit(*firstPOI);

	cout << endl <<"95 interval on " <<firstPOI->GetName()<<" is : ["<<     lowerLimit << ", "<<     upperLimit <<"] "<<endl;   
	
	LikelihoodIntervalPlot * plot = new LikelihoodIntervalPlot(interval);   
	
	
	plot->SetRange(lowerLimit*.7,upperLimit*1.3);  // possible eventually to change ranges
	//plot->SetNPoints(50);  // do not use too many points, it could become very slow for some models
	plot->Draw("tf1");  // use option TF1 if too slow (plot.Draw("tf1")

/*
//------------------------------------------------------------------------------------------------------


////////////hypotest calculator///////////////// 

	HypoTestResult *htr = plc.GetHypoTest();
	
	htr->Print();
*/
	
	//double pval0=htr->NullPValue(); 

	//std::cout<<"nullpval="<<pval0<<" sig="<<PValueToSignificance(pval0)<<std::endl;



}





















