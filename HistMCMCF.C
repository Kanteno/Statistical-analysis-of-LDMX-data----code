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
#include "RooStats/MCMCCalculator.h"
#include "RooStats/SequentialProposal.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"

using namespace RooFit; 
using namespace RooStats;

void HistMCMCF(  
	const char* infile =  "CountingModel.root", 
	const char* workspaceName = "w",
	const char* modelConfigName = "ModelConfig",
	const char* dataName = "data" )
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
//--------------------------------------------------------------------------------------------
	
	RooRealVar * nsig = w->var("mu"); 
	if (nsig) nsig->setRange(0,100);

	MCMCCalculator mcmc(*data,*mc);
	mcmc.SetConfidenceLevel(0.95); // 68.3% interval or 1 sigma
	
	// Define proposal function
	SequentialProposal sp(0.1); // need to look into this... I think it is required for the Metropolis-Hastings algorithm
	mcmc.SetProposalFunction(sp);

	// set number of iterations and initial burning steps 
	mcmc.SetNumIters(1000000);         // Metropolis-Hastings algorithm iterations
	mcmc.SetNumBurnInSteps(2000);     // first N steps to be ignored as burn-in <-- what does this imply? i.e. what is its effect


	// default is the shortest interval, here the central interval is used... I need to look into this, what does central vs one-sided Baysian interval imply??? is it one-sided vs two sided?
	mcmc.SetLeftSideTailFraction(0); // for central Bayesian interval
	//mcmc.SetLeftSideTailFraction(0); // for one-sided Bayesian interval

	// run the calculator
	MCMCInterval* interval = mcmc.GetInterval();



	// print out the iterval on the first Parameter of Interest
	RooRealVar* firstPOI = (RooRealVar*) mc->GetParametersOfInterest()->first();


//-----------------------------------------------------------------------------------------------------------
	
	cout << "\n95% interval on " <<firstPOI->GetName()<<" is : ["<< interval->LowerLimit(*firstPOI) << ", "<< interval->UpperLimit(*firstPOI) <<"] "<<endl;

	// Ploting the result
	new TCanvas("IntervalPlot");
	MCMCIntervalPlot plot(*interval);
	plot.Draw();

}
