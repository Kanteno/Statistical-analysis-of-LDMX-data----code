# Statistical-analysis-of-LDMX-data----code
This code assumes the use of the ROOT framework for C++ and was written on ROOT version 6-28-02 on Ubuntu 11.3.0.
To run the code put all the codes in the same directory as well as a folder called "Data_File" in which you put the .root files that you want to run.
Note that the WorkSpace.C file must be run before any of the other codes as it creates the ROOT worksapce that the other codes require.
It is also in this code that you select dataset, background, and signal hypothesis files as well as the up/dn variation files for systematic uncertainties.
The other codes can be run in any order. Parameters can be changed as required, but do note that it might take very long to run for some changes.

HistPLCalculatorF.C dose a profiling and returns the most likley value for the signal strength, \mu, (the signal provided in WorkSpace.C correspond to \mu of 1) as well as a likelihood plot.
The HistMCMC.C uses Baysian posteriors to calculate a prbability posterior for the signal strength again the signal provided correspond to \mu =1.
The code HistHypoF.C contains two methods for calculating CLs but they do not work properly and its CLb, CLs+b and CLs outputs can not be trusted to be accurate.

NOTE: The codes HistPLCalculatorF.C, HistMCMC.C, and HistHypoF.C where mostly taken from with only minor changes:
https://twiki.cern.ch/twiki/bin/view/RooStats/RooStatsTutorialsJune2013#Exercise_6_Compute_the_significa.
Hence creadit for them goes to LorenzoMoneta.
