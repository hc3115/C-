#include "Riostream.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void Eff_Pur_Plot() {
   ifstream inFile;

   string Eff_Pur_Cut;
   inFile.open("Eff_Pur_Cut2.txt");
   if (inFile.is_open()) {
      cout << "File has been opened" << endl;
   }
   else {
      cout << "NO FILE HAS BEEN OPENED" << endl;
   }
   TCanvas *c1 = new TCanvas("c1","Eff_Pur_Cut");
   TH1D *Hist_Eff_Pur_Cut = new TH1D("Eff_Pur_Cut","Efficieny(Blue) Purity(Red) Cut",28,0,28);
   TH1D *Hist_Pur_Cut = new TH1D("Pur_Cut","Pur_Cut",28,0,28);
   Int_t Counter = 0;
   while (getline(inFile, Eff_Pur_Cut)) {
      Counter++;
      string Eff;
      string Pur;
      string Cut;
      stringstream lineStream(Eff_Pur_Cut);
      while(getline(lineStream, Eff, ' ')){
	 getline(lineStream, Pur, ' ');
	 getline(lineStream, Cut, ' ');
	 Hist_Eff_Pur_Cut->GetXaxis()->SetBinLabel(Counter,Cut.c_str());
         Hist_Pur_Cut->GetXaxis()->SetBinLabel(Counter,Cut.c_str());
	 Hist_Eff_Pur_Cut->SetBinContent(Counter,atof(Eff.c_str()));
	 Hist_Pur_Cut->SetBinContent(Counter,atof(Pur.c_str()));
      }
   }
   Hist_Eff_Pur_Cut->GetXaxis()->SetTitle("Cutting Point (Mean)_(N_{Pixels} above 200ADU)");
   Hist_Eff_Pur_Cut->GetYaxis()->SetTitle("Efficiency(Blue) OR Purity(Red)");
   Hist_Eff_Pur_Cut->GetXaxis()->SetTitleOffset(1.4);
   Hist_Eff_Pur_Cut->GetYaxis()->SetTitleOffset(1.4);
   Hist_Eff_Pur_Cut->SetMaximum(1.0000);
   Hist_Eff_Pur_Cut->SetMinimum(0.9840);
   Hist_Eff_Pur_Cut->Draw();
   Hist_Pur_Cut->SetLineColor(kRed);
   Hist_Pur_Cut->Draw("SAMES");
   Hist_Pur_Cut->SetStats(kFALSE);
   Hist_Eff_Pur_Cut->SetStats(kFALSE);
   inFile.close();
   return;
}
