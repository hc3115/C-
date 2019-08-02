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

void Eff_Pur_Graph() {
   ifstream inFile;

   string Eff_Pur_Cut;
   inFile.open("Eff_Pur_Cut3.txt");
   if (inFile.is_open()) {
      cout << "File has been opened" << endl;
   }
   else {
      cout << "NO FILE HAS BEEN OPENED" << endl;
   }
   TCanvas *c1 = new TCanvas("c1","Eff_Pur_Cut");
   TMultiGraph *mg = new TMultiGraph();
   mg->SetTitle("Efficiency vs Purity");
   Int_t Counter = 0;
   Int_t Colour = 1;
   Double_t x[7], y[7];
   TLegend *legend = new TLegend(0.1, 0.1, 0.3, 0.3);
   legend->SetHeader("N_{pixels} above 200 ADU"); // option "C" allows to center the header
   while (getline(inFile, Eff_Pur_Cut)) {
      Counter++;
      string Eff;
      string Pur;
      string Cut;
      stringstream lineStream(Eff_Pur_Cut);
      while(getline(lineStream, Eff, ' ')){
	 getline(lineStream, Pur, ' ');
	 getline(lineStream, Cut, ' ');
	 x[Counter-1] = atof(Pur.c_str());
	 y[Counter-1] = atof(Eff.c_str());
      }
      if (Counter==7) {
	 TGraph* gr = new TGraph(7,x,y);
	 gr->SetLineColor(Colour);
	 Counter=0;
	 mg->Add(gr);
	 legend->AddEntry(gr,TString::Format("%i", Colour*50),"l");
	 Colour++;
      }
   }
   mg->Draw("AL*");
   legend->SetTextSize(0.03);
   legend->SetTextAlign(22);
   legend->SetMargin(0.4);
   legend->Draw();
   inFile.close();
   return;
}
