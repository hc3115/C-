#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF2.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TBrowser.h>
#include <iostream>
#include <TSystem.h>
#include "Riostream.h"
#include "TString.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TLegend.h>

using namespace std;

TH1I *AvgPix_Event_cam0= new TH1I("Pixel0","Mean Pixels cam0",300, -100, 200); //1
TH1I *AvgPix_Event_cam1= new TH1I("Pixel1","Mean Pixels cam1",300, -100, 200);
TH1I *AvgPix_Event_cam2= new TH1I("Pixel2","Mean Pixels cam2",300, -100, 200);
TH1I *AvgPix_Event_cam3= new TH1I("Pixel3","Mean Pixels cam3",300, -100, 200);

TH1I *AvgPix_Event_cam0v2= new TH1I("Pixel0v2","Pixel_Event0v2",300, -100, 200); //1
TH1I *AvgPix_Event_cam1v2= new TH1I("Pixel1v2","Pixel_Event1v2",300, -100, 200);
TH1I *AvgPix_Event_cam2v2= new TH1I("Pixel2v2","Pixel_Event2v2",300, -100, 200);
TH1I *AvgPix_Event_cam3v2= new TH1I("Pixel3v2","Pixel_Event3v2",300, -100, 200);

TH1I *count_cam0= new TH1I("count0","Count above 200ADU cam0",120000, 0, 120000); //2
TH1I *count_cam1= new TH1I("count1","Count above 200ADU cam1",120000, 0, 120000);
TH1I *count_cam2= new TH1I("count2","Count above 200ADU cam2",120000, 0, 120000);
TH1I *count_cam3= new TH1I("count3","Count above 200ADU cam3",120000, 0, 120000);

TH1I *count_cam0v2= new TH1I("count0v2","count_cam0v2",120000, 0, 120000); //2
TH1I *count_cam1v2= new TH1I("count1v2","count_cam1v2",120000, 0, 120000);
TH1I *count_cam2v2= new TH1I("count2v2","count_cam2v2",120000, 0, 120000);
TH1I *count_cam3v2= new TH1I("count3v2","count_cam3v2",120000, 0, 120000);

TH2I *AvgPixvs200ADU_0= new TH2I("AvgPixvs200ADU_0","AvgPix vs 200 ADU camera 0",120000, 0, 120000, 150, -50, 100); //3
TH2I *AvgPixvs200ADU_1= new TH2I("AvgPixvs200ADU_1","AvgPix vs 200 ADU camera 1",120000, 0, 120000, 150, -50, 100);
TH2I *AvgPixvs200ADU_2= new TH2I("AvgPixvs200ADU_2","AvgPix vs 200 ADU camera 2",120000, 0, 120000, 150, -50, 100);
TH2I *AvgPixvs200ADU_3= new TH2I("AvgPixvs200ADU_3","AvgPix vs 200 ADU camera 3",120000, 0, 120000, 150, -50, 100);

TH2I *AvgPixvs200ADU_0v2= new TH2I("AvgPixvs200ADU_0_Sparks","AvgPix vs 200 ADU camera 0 Sparks",120000, 0, 120000, 150, -50, 100); //3
TH2I *AvgPixvs200ADU_1v2= new TH2I("AvgPixvs200ADU_1_Sparks","AvgPix vs 200 ADU camera 1 Sparks",120000, 0, 120000, 150, -50, 100);
TH2I *AvgPixvs200ADU_2v2= new TH2I("AvgPixvs200ADU_2_Sparks","AvgPix vs 200 ADU camera 2 Sparks",120000, 0, 120000, 150, -50, 100);
TH2I *AvgPixvs200ADU_3v2= new TH2I("AvgPixvs200ADU_3_Sparks","AvgPix vs 200 ADU camera 3 Sparks",120000, 0, 120000, 150, -50, 100);

Int_t Total_Entries = 0;
Int_t Spark_Entries = 0;
Int_t Spark_Count = 0;
vector<string> Run_Number;
vector<string> Event_Number;

void CSVFile() { //4
   ifstream inFile;

   string Run;
   inFile.open("Hand_Scan.csv");
   if (inFile.is_open()) {
      cout << "File has been opened" << endl;
   }
   else {
      cout << "NO FILE HAS BEEN OPENED" << endl;
   }

   while (getline(inFile, Run)) {
      string var;
      stringstream lineStream(Run);
      while(getline(lineStream, var, ' ')){
         if (var.length()> 3) {
	    Run_Number.push_back(var);
	 }
	 else {
	    Event_Number.push_back(var);
	 }
      }
   }
   inFile.close();
   return;
}

void BE_cams_ADUcut(const char* file, const char* x) { //5

   TFile *f = TFile::Open(file);
   if ((!f) || f->IsZombie()) { delete f; return;}
   TTree *TreeB; f->GetObject("Biases", TreeB);
   if (!TreeB) { delete f; return;}
   TH2I *Bias_cam0=0, *Bias_cam1=0, *Bias_cam2=0, *Bias_cam3=0;
   //TH2I **addrcam0=&Bias_cam0, **addrcam1=&Bias_cam1, **addrcam2=&Bias_cam2, **addrcam3=&Bias_cam3;
   //cout<<addrcam0<<" "<<addrcam1<<" "<<addrcam2<<" "<<addrcam3<<endl;
   TH2I *Bias_cam0_Avg= new TH2I("Bias0","Bias_cam0_Avg",385, 0, 3080, 385, 0, 3080); //5.1
   TH2I *Bias_cam1_Avg= new TH2I("Bias1","Bias_cam1_Avg",385, 0, 3080, 385, 0, 3080);
   TH2I *Bias_cam2_Avg= new TH2I("Bias2","Bias_cam2_Avg",385, 0, 3080, 385, 0, 3080);
   TH2I *Bias_cam3_Avg= new TH2I("Bias3","Bias_cam3_Avg",385, 0, 3080, 385, 0, 3080);

   TreeB -> SetBranchAddress("Bias_cam0", &Bias_cam0);
   TreeB -> SetBranchAddress("Bias_cam1", &Bias_cam1);
   TreeB -> SetBranchAddress("Bias_cam2", &Bias_cam2);
   TreeB -> SetBranchAddress("Bias_cam3", &Bias_cam3);

   TCanvas *c4 = new TCanvas("c4","c4");
   c4->Divide(2,2);

   Double_t AvgPix_B_cam0; //5.2
   Double_t AvgPix_B_cam1;
   Double_t AvgPix_B_cam2;
   Double_t AvgPix_B_cam3;

   Int_t nentriesB = (Double_t)TreeB->GetEntries();
   for (Int_t i=0;i<nentriesB;i++) {
      TreeB->GetEntry(i); //Gets each entry in current tree branch(es)

      Bias_cam0_Avg->Add(Bias_cam0); //add current entry
      Bias_cam1_Avg->Add(Bias_cam1); //add current entry
      Bias_cam2_Avg->Add(Bias_cam2); //add current entry
      Bias_cam3_Avg->Add(Bias_cam3); //add current entry

      for (Int_t j=0; j<Bias_cam0->GetXaxis()->GetNbins(); j++) { //checks each bin in 2D histogram

         for (Int_t z=0; z<Bias_cam0->GetYaxis()->GetNbins(); z++) {

            AvgPix_B_cam0 += Bias_cam0->GetBinContent(j+1,z+1);
            AvgPix_B_cam1 += Bias_cam1->GetBinContent(j+1,z+1);
            AvgPix_B_cam2 += Bias_cam2->GetBinContent(j+1,z+1);
            AvgPix_B_cam3 += Bias_cam3->GetBinContent(j+1,z+1);

	 }
      }
      AvgPix_B_cam0 = AvgPix_B_cam0 / 148225;
      AvgPix_B_cam1 = AvgPix_B_cam1 / 148225;
      AvgPix_B_cam2 = AvgPix_B_cam2 / 148225;
      AvgPix_B_cam3 = AvgPix_B_cam3 / 148225;
      
      if (AvgPix_B_cam0>2900 || AvgPix_B_cam1>2900 || AvgPix_B_cam2>2900 || AvgPix_B_cam3>3600) { //5.3
	 delete c4;
	 return;
      }
   }

   c4->cd(1);
   Bias_cam0_Avg->Scale(1.0/nentriesB); //5.4
   Bias_cam0_Avg->Draw("colz");

   c4->cd(2);
   Bias_cam1_Avg->Scale(1.0/nentriesB);
   Bias_cam1_Avg->Draw("colz");

   c4->cd(3);
   Bias_cam2_Avg->Scale(1.0/nentriesB);
   Bias_cam2_Avg->Draw("colz");

   c4->cd(4);
   Bias_cam3_Avg->Scale(1.0/nentriesB);
   Bias_cam3_Avg->Draw("colz");

   TTree *TreeE; f->GetObject("Events", TreeE);
   if (!TreeE) { delete f; return; }
   TH2I *Event_cam0 = 0, *Event_cam1 = 0, *Event_cam2 = 0, *Event_cam3 = 0;

   TreeE -> SetBranchAddress("Event_cam0", &Event_cam0);
   TreeE -> SetBranchAddress("Event_cam1", &Event_cam1);
   TreeE -> SetBranchAddress("Event_cam2", &Event_cam2);
   TreeE -> SetBranchAddress("Event_cam3", &Event_cam3);

   TCanvas *c0 = new TCanvas("c0","c0");
   c0->Divide(2,2);

   TCanvas *c5 = new TCanvas("c5","c5");
   c5->Divide(2,2);

   TCanvas *c6 = new TCanvas("c6","c6");
   c6->Divide(2,2);

   TCanvas *c7 = new TCanvas("c7","c7"); //Spare Canvas
   c7->Divide(2,2);
   Double_t Prev_Mean_cam0; //5.5
   Double_t Prev_Mean_cam1;
   Double_t Prev_Mean_cam2;
   Double_t Prev_Mean_cam3;
   Int_t nentriesE = (Double_t)TreeE->GetEntries();

   for (Int_t i=0;i<nentriesE;i++) {
      TreeE->GetEntry(i); //Gets each entry in current tree branch(es)
      Total_Entries++;
      TH2I *CEvent_cam0 = (TH2I*) Event_cam0->Clone(); //5.6
      CEvent_cam0->Add(Bias_cam0_Avg,-1);

      TH2I *CEvent_cam1 = (TH2I*) Event_cam1->Clone();
      CEvent_cam1->Add(Bias_cam1_Avg,-1);

      TH2I *CEvent_cam2 = (TH2I*) Event_cam2->Clone();
      CEvent_cam2->Add(Bias_cam2_Avg,-1);

      TH2I *CEvent_cam3 = (TH2I*) Event_cam3->Clone();
      CEvent_cam3->Add(Bias_cam3_Avg,-1);

      Double_t AvgPix_BE_cam0; //5.7
      Double_t AvgPix_BE_cam1;
      Double_t AvgPix_BE_cam2;
      Double_t AvgPix_BE_cam3;

      TH1I *Pixel_Event_cam0= new TH1I("Pixel0","Pixel_Event0",600, -100, 500); //5.8
      TH1I *Pixel_Event_cam1= new TH1I("Pixel1","Pixel_Event1",600, -100, 500);
      TH1I *Pixel_Event_cam2= new TH1I("Pixel2","Pixel_Event2",600, -100, 500);
      TH1I *Pixel_Event_cam3= new TH1I("Pixel3","Pixel_Event3",600, -100, 500);

      Int_t countcam0 = 0; //5.9
      Int_t countcam1 = 0;
      Int_t countcam2 = 0;
      Int_t countcam3 = 0;

      for (Int_t j=0; j<CEvent_cam0->GetXaxis()->GetNbins(); j++) {

         for (Int_t z=0; z<CEvent_cam0->GetYaxis()->GetNbins(); z++) {

            AvgPix_BE_cam0 += CEvent_cam0->GetBinContent(j+1,z+1); // contributes to finding mean pixel value of event (bias-subtracted)
            AvgPix_BE_cam1 += CEvent_cam1->GetBinContent(j+1,z+1);
            AvgPix_BE_cam2 += CEvent_cam2->GetBinContent(j+1,z+1);
            AvgPix_BE_cam3 += CEvent_cam3->GetBinContent(j+1,z+1);

            Pixel_Event_cam0->Fill(CEvent_cam0->GetBinContent(j+1,z+1)); // gets bin value in specified bin in event for camera 0,
            Pixel_Event_cam1->Fill(CEvent_cam1->GetBinContent(j+1,z+1)); // camera 1,
            Pixel_Event_cam2->Fill(CEvent_cam2->GetBinContent(j+1,z+1)); // camera 2,
            Pixel_Event_cam3->Fill(CEvent_cam3->GetBinContent(j+1,z+1)); // camera 3.


            if (CEvent_cam0->GetBinContent(j+1,z+1)> 200){ //5.10
               countcam0++;
            }
            if (CEvent_cam1->GetBinContent(j+1,z+1)> 200){
               countcam1++;
            }
            if (CEvent_cam2->GetBinContent(j+1,z+1)> 200){
               countcam2++;
            }
            if (CEvent_cam3->GetBinContent(j+1,z+1)> 200){
               countcam3++;
            }

         }
      }

      AvgPix_BE_cam0 = AvgPix_BE_cam0 / 148225; //contributes to finding mean pixel value for event (bias-subtracted)
      AvgPix_BE_cam1 = AvgPix_BE_cam1 / 148225;
      AvgPix_BE_cam2 = AvgPix_BE_cam2 / 148225;
      AvgPix_BE_cam3 = AvgPix_BE_cam3 / 148225;
      if(i==0){
         Prev_Mean_cam0=AvgPix_BE_cam0;
         Prev_Mean_cam1=AvgPix_BE_cam1;
         Prev_Mean_cam2=AvgPix_BE_cam2;
         Prev_Mean_cam3=AvgPix_BE_cam3;
      }
      //string Run = "2347";
      //if(x==Run){cout<<AvgPix_BE_cam0<<" "<<Prev_Mean_cam0<<endl;}
      
      for (Int_t j=0; j<Run_Number.size(); j++) { //5.11
	 if (Run_Number[j]==x) { //no final else statement because we do not want to note the same event twice.
	    string Event = Event_Number[j];
	    if (atoi(Event.c_str())==i && ((countcam0>200||AvgPix_BE_cam0>20)||(countcam1>200||AvgPix_BE_cam1>20)||(countcam2>200||AvgPix_BE_cam2>20)||(countcam3>200||AvgPix_BE_cam3>20))) {
	       /*
	       AvgPix_Event_cam0v2->Fill(AvgPix_BE_cam0); 
	       AvgPix_Event_cam1v2->Fill(AvgPix_BE_cam1);
	       AvgPix_Event_cam2v2->Fill(AvgPix_BE_cam2);
	       AvgPix_Event_cam3v2->Fill(AvgPix_BE_cam3);
	       
	       count_cam0v2->Fill(countcam0);
	       count_cam1v2->Fill(countcam1);
	       count_cam2v2->Fill(countcam2);
	       count_cam3v2->Fill(countcam3);
	       */
	       if (countcam0>200||AvgPix_BE_cam0>20) { //these next 4 if statements means that sparks will only be added if there is spark activity in the specified camera.
		  AvgPixvs200ADU_0v2->Fill(countcam0,AvgPix_BE_cam0);
	       }
	       if (countcam1>200||AvgPix_BE_cam1>20) {
		  AvgPixvs200ADU_1v2->Fill(countcam1,AvgPix_BE_cam1);
	       }
	       if (countcam2>200||AvgPix_BE_cam2>20) {
		  AvgPixvs200ADU_2v2->Fill(countcam2,AvgPix_BE_cam2);
	       }
	       if (countcam3>200||AvgPix_BE_cam3>20) {
		  AvgPixvs200ADU_3v2->Fill(countcam3,AvgPix_BE_cam3);
	       }
	       
	       Spark_Count++;
	       break;
	    }
	    else if (atoi(Event.c_str())==i) {
	       c0->cd(1);
	       CEvent_cam0->Draw("colz");
	       CEvent_cam0->SetMaximum(500); //must set zaxis color range otherwise the noise colour will vary per event
	       CEvent_cam0->SetMinimum(-100);
	       CEvent_cam0->SetStats(kFALSE);

	       c0->cd(2);
	       CEvent_cam1->Draw("colz");
	       CEvent_cam1->SetMaximum(500);
	       CEvent_cam1->SetMinimum(-100);
	       CEvent_cam1->SetStats(kFALSE);

	       c0->cd(3);
	       CEvent_cam2->Draw("colz");
	       CEvent_cam2->SetMaximum(500);
	       CEvent_cam2->SetMinimum(-100);
	       CEvent_cam2->SetStats(kFALSE);

	       c0->cd(4);
	       CEvent_cam3->Draw("colz");
	       CEvent_cam3->SetMaximum(500);
	       CEvent_cam3->SetMinimum(-100);
	       CEvent_cam3->SetStats(kFALSE);

	       gStyle->SetNumberContours(64);
	       if((AvgPix_BE_cam0-Prev_Mean_cam0)>5||(AvgPix_BE_cam1-Prev_Mean_cam1)>5||(AvgPix_BE_cam2-Prev_Mean_cam2)>5||(AvgPix_BE_cam3-Prev_Mean_cam3)>5){
		  //gSystem->cd("Consecutive_Means");
                  //c0->Print(TString::Format("%iRun%s.png", i, x), "png");
                  //gSystem->cd("..");
	       }
	       else{
		  //gSystem->cd("LowSparks");
                  //c0->Print(TString::Format("%iRun%s.png", i, x), "png");
                  //gSystem->cd("..");
	       }
	       break;
	    }
       	    else if (j==Run_Number.size()-1) {
	       if ((countcam0>200||AvgPix_BE_cam0>20)||(countcam1>200||AvgPix_BE_cam1>20)||(countcam2>200||AvgPix_BE_cam2>20)||(countcam3>200||AvgPix_BE_cam3>20)) {
		  c0->cd(1);
		  CEvent_cam0->Draw("colz");
		  CEvent_cam0->SetMaximum(500); //must set zaxis color range otherwise the noise colour will varie per event
		  CEvent_cam0->SetMinimum(-100);
		  CEvent_cam0->SetStats(kFALSE);

		  c0->cd(2);
		  CEvent_cam1->Draw("colz");
		  CEvent_cam1->SetMaximum(500);
		  CEvent_cam1->SetMinimum(-100);
		  CEvent_cam1->SetStats(kFALSE);

		  c0->cd(3);
		  CEvent_cam2->Draw("colz");
		  CEvent_cam2->SetMaximum(500);
		  CEvent_cam2->SetMinimum(-100);
		  CEvent_cam2->SetStats(kFALSE);

		  c0->cd(4);
		  CEvent_cam3->Draw("colz");
		  CEvent_cam3->SetMaximum(500);
		  CEvent_cam3->SetMinimum(-100);
		  CEvent_cam3->SetStats(kFALSE);

		  gStyle->SetNumberContours(64);
		  //gSystem->cd("HighNonSparks");
		  //c0->Print(TString::Format("%iRun%s.png", i, x), "png");
		  //gSystem->cd("..");
	       }
	       else {
		  if((AvgPix_BE_cam0-Prev_Mean_cam0)>5||(AvgPix_BE_cam1-Prev_Mean_cam1)>5||(AvgPix_BE_cam2-Prev_Mean_cam2)>5||(AvgPix_BE_cam3-Prev_Mean_cam3)>5){
		     c0->cd(1);
		     CEvent_cam0->Draw("colz");
		     CEvent_cam0->SetMaximum(500); //must set zaxis color range otherwise the noise colour will varie per event
		     CEvent_cam0->SetMinimum(-100);
		     CEvent_cam0->SetStats(kFALSE);

		     c0->cd(2);
		     CEvent_cam1->Draw("colz");
		     CEvent_cam1->SetMaximum(500);
		     CEvent_cam1->SetMinimum(-100);
		     CEvent_cam1->SetStats(kFALSE);

		     c0->cd(3);
		     CEvent_cam2->Draw("colz");
		     CEvent_cam2->SetMaximum(500);
		     CEvent_cam2->SetMinimum(-100);
		     CEvent_cam2->SetStats(kFALSE);

		     c0->cd(4);
		     CEvent_cam3->Draw("colz");
		     CEvent_cam3->SetMaximum(500);
		     CEvent_cam3->SetMinimum(-100);
		     CEvent_cam3->SetStats(kFALSE);

		     gStyle->SetNumberContours(64);
		     //gSystem->cd("Consecutive_Means");
                     //c0->Print(TString::Format("%iRun%s.png", i, x), "png");
                     //gSystem->cd("..");
		  }
		  else{
		     AvgPixvs200ADU_0->Fill(countcam0,AvgPix_BE_cam0);
                     AvgPixvs200ADU_1->Fill(countcam1,AvgPix_BE_cam1);
                     AvgPixvs200ADU_2->Fill(countcam2,AvgPix_BE_cam2);
                     AvgPixvs200ADU_3->Fill(countcam3,AvgPix_BE_cam3);

		     count_cam0->Fill(countcam0);
		     count_cam1->Fill(countcam1);
		     count_cam2->Fill(countcam2);
		     count_cam3->Fill(countcam3);

		     AvgPix_Event_cam0->Fill(AvgPix_BE_cam0);
		     AvgPix_Event_cam1->Fill(AvgPix_BE_cam1);
		     AvgPix_Event_cam2->Fill(AvgPix_BE_cam2);
		     AvgPix_Event_cam3->Fill(AvgPix_BE_cam3);
		  }
	       }
	    }
	 }
	 else if (j==Run_Number.size()-1) {
	    if ((countcam0>200||AvgPix_BE_cam0>20)||(countcam1>200||AvgPix_BE_cam1>20)||(countcam2>200||AvgPix_BE_cam2>20)||(countcam3>200||AvgPix_BE_cam3>20)) {
	       c0->cd(1);
               CEvent_cam0->Draw("colz");
               CEvent_cam0->SetMaximum(500); //must set zaxis color range otherwise the noise colour will varie per event
               CEvent_cam0->SetMinimum(-100);
               CEvent_cam0->SetStats(kFALSE);

               c0->cd(2);
               CEvent_cam1->Draw("colz");
               CEvent_cam1->SetMaximum(500);
               CEvent_cam1->SetMinimum(-100);
               CEvent_cam1->SetStats(kFALSE);

               c0->cd(3);
               CEvent_cam2->Draw("colz");
               CEvent_cam2->SetMaximum(500);
               CEvent_cam2->SetMinimum(-100);
               CEvent_cam2->SetStats(kFALSE);

               c0->cd(4);
               CEvent_cam3->Draw("colz");
               CEvent_cam3->SetMaximum(500);
               CEvent_cam3->SetMinimum(-100);
               CEvent_cam3->SetStats(kFALSE);

               gStyle->SetNumberContours(64);
               //gSystem->cd("HighNonSparks");
               //c0->Print(TString::Format("%iRun%s.png", i, x), "png");
               //gSystem->cd("..");
	    }
	    else {
	       if((AvgPix_BE_cam0-Prev_Mean_cam0)>5||(AvgPix_BE_cam1-Prev_Mean_cam1)>5||(AvgPix_BE_cam2-Prev_Mean_cam2)>5||(AvgPix_BE_cam3-Prev_Mean_cam3)>5){
		  c0->cd(1);
		  CEvent_cam0->Draw("colz");
		  CEvent_cam0->SetMaximum(500); //must set zaxis color range otherwise the noise colour will varie per event
		  CEvent_cam0->SetMinimum(-100);
		  CEvent_cam0->SetStats(kFALSE);

		  c0->cd(2);
		  CEvent_cam1->Draw("colz");
		  CEvent_cam1->SetMaximum(500);
		  CEvent_cam1->SetMinimum(-100);
		  CEvent_cam1->SetStats(kFALSE);

		  c0->cd(3);
		  CEvent_cam2->Draw("colz");
		  CEvent_cam2->SetMaximum(500);
		  CEvent_cam2->SetMinimum(-100);
		  CEvent_cam2->SetStats(kFALSE);

		  c0->cd(4);
		  CEvent_cam3->Draw("colz");
		  CEvent_cam3->SetMaximum(500);
		  CEvent_cam3->SetMinimum(-100);
		  CEvent_cam3->SetStats(kFALSE);

		  gStyle->SetNumberContours(64);
		  //gSystem->cd("Consecutive_Means");
                  //c0->Print(TString::Format("%iRun%s.png", i, x), "png");
                  //gSystem->cd("..");
	       }
	       else{
		  AvgPixvs200ADU_0->Fill(countcam0,AvgPix_BE_cam0);
                  AvgPixvs200ADU_1->Fill(countcam1,AvgPix_BE_cam1);
                  AvgPixvs200ADU_2->Fill(countcam2,AvgPix_BE_cam2);
                  AvgPixvs200ADU_3->Fill(countcam3,AvgPix_BE_cam3);

		  count_cam0->Fill(countcam0);
		  count_cam1->Fill(countcam1);
		  count_cam2->Fill(countcam2);
		  count_cam3->Fill(countcam3);

		  AvgPix_Event_cam0->Fill(AvgPix_BE_cam0);
		  AvgPix_Event_cam1->Fill(AvgPix_BE_cam1);
		  AvgPix_Event_cam2->Fill(AvgPix_BE_cam2);
		  AvgPix_Event_cam3->Fill(AvgPix_BE_cam3);
	       }
	    }
	 }
      }
      if((AvgPix_BE_cam0-Prev_Mean_cam0)>5||(AvgPix_BE_cam1-Prev_Mean_cam1)>5||(AvgPix_BE_cam2-Prev_Mean_cam2)>5||(AvgPix_BE_cam3-Prev_Mean_cam3)>5){
	 delete Pixel_Event_cam0;
	 delete Pixel_Event_cam1;
	 delete Pixel_Event_cam2;
	 delete Pixel_Event_cam3;
	 continue;
      }
      else if(i>0){
	 Prev_Mean_cam0=AvgPix_BE_cam0;
	 Prev_Mean_cam1=AvgPix_BE_cam1;
	 Prev_Mean_cam2=AvgPix_BE_cam2;
	 Prev_Mean_cam3=AvgPix_BE_cam3;
      }
      /*
      if ( AvgPix_BE_cam0 > 7 || AvgPix_BE_cam1 > 7 || AvgPix_BE_cam2 > 7 || AvgPix_BE_cam3 > 7 ) { //5.12
	 Spark_Entries++;
         c0->cd(1);
         CEvent_cam0->Draw("colz");
         CEvent_cam0->SetMaximum(500); //must set zaxis color range otherwise the noise colour will varie per event
         CEvent_cam0->SetMinimum(-100);
         CEvent_cam0->SetStats(kFALSE);

         c0->cd(2);
         CEvent_cam1->Draw("colz");
         CEvent_cam1->SetMaximum(500);
         CEvent_cam1->SetMinimum(-100);
         CEvent_cam1->SetStats(kFALSE);

         c0->cd(3);
         CEvent_cam2->Draw("colz");
         CEvent_cam2->SetMaximum(500);
         CEvent_cam2->SetMinimum(-100);
         CEvent_cam2->SetStats(kFALSE);

         c0->cd(4);
         CEvent_cam3->Draw("colz");
         CEvent_cam3->SetMaximum(500);
         CEvent_cam3->SetMinimum(-100);
         CEvent_cam3->SetStats(kFALSE);

         gStyle->SetNumberContours(64);

         c6->cd(1);
         gPad->SetLogy();
         Pixel_Event_cam0->Draw();
         c6->cd(2);
         gPad->SetLogy();
         Pixel_Event_cam1->Draw();
         c6->cd(3);
         gPad->SetLogy();
         c6->cd(4);
         gPad->SetLogy();
         Pixel_Event_cam3->Draw();

         AvgPix_Event_cam0->Fill(AvgPix_BE_cam0); // creates 1D Average pixel histogram of spark events for camera 0, (bias-subtracted)
         AvgPix_Event_cam1->Fill(AvgPix_BE_cam1); // camera 1,(bias-subtracted)
         AvgPix_Event_cam2->Fill(AvgPix_BE_cam2); // camera 2,(bias-subtracted)
         AvgPix_Event_cam3->Fill(AvgPix_BE_cam3); // camera 3.(bias-subtracted)

         count_cam0v2->Fill(countcam0);
         count_cam1v2->Fill(countcam1);
         count_cam2v2->Fill(countcam2);
         count_cam3v2->Fill(countcam3);

	 if ( AvgPix_BE_cam0 > 7 ) {

	    AvgPixvs200ADU_0v2->Fill(countcam0,AvgPix_BE_cam0);

	 }
	 if ( AvgPix_BE_cam1 > 7 ) {

	    AvgPixvs200ADU_1v2->Fill(countcam1,AvgPix_BE_cam1);

	 }
	 if ( AvgPix_BE_cam2 > 7 ) {

	    AvgPixvs200ADU_2v2->Fill(countcam2,AvgPix_BE_cam2);

	 }
	 if ( AvgPix_BE_cam3 > 7 ) {

	    AvgPixvs200ADU_3v2->Fill(countcam3,AvgPix_BE_cam3);

	 }
      }
      
      AvgPix_Event_cam0->Fill(AvgPix_BE_cam0); // creates 1D Average pixel histogram of non spark events for camera 0, (bias-subtracted)
      AvgPix_Event_cam1->Fill(AvgPix_BE_cam1); // camera 1,(bias-subtracted)
      AvgPix_Event_cam2->Fill(AvgPix_BE_cam2); // camera 2,(bias-subtracted)
      AvgPix_Event_cam3->Fill(AvgPix_BE_cam3); // camera 3.(bias-subtracted)

      AvgPixvs200ADU_0->Fill(countcam0,AvgPix_BE_cam0);
      AvgPixvs200ADU_1->Fill(countcam1,AvgPix_BE_cam1);
      AvgPixvs200ADU_2->Fill(countcam2,AvgPix_BE_cam2);
      AvgPixvs200ADU_3->Fill(countcam3,AvgPix_BE_cam3);
      
      count_cam0->Fill(countcam0);
      count_cam1->Fill(countcam1);
      count_cam2->Fill(countcam2);
      count_cam3->Fill(countcam3);
      */
      delete Pixel_Event_cam0;
      delete Pixel_Event_cam1;
      delete Pixel_Event_cam2;
      delete Pixel_Event_cam3;
   }
   delete c6;
   delete c4;
   delete c0;
   delete c5;
   delete c7;
}

int main() { //6.1
   CSVFile();
   for (Int_t i = 1; i < 73; i++) {

      if ( i < 10 ) {

         try { //6.2

	    BE_cams_ADUcut(TString::Format("CCD_images_R132300%i.root", i),TString::Format("230%i", i));
	    BE_cams_ADUcut(TString::Format("CCD_images_R132400%i.root", i),TString::Format("240%i", i));
	    //BE_cams_ADUcut(TString::Format("CCD_images_R125700%i.root", i),TString::Format("570%i", i));

         }
         catch(...) {

            continue;

         }

      }

      else {

         try {

	    BE_cams_ADUcut(TString::Format("CCD_images_R13230%i.root", i),TString::Format("23%i", i));
	    //BE_cams_ADUcut(TString::Format("CCD_images_R12570%i.root", i),TString::Format("57%i", i));
	    
         }
         catch(...) {

            continue;

         }

      }
   }
   cout<<Spark_Count<<endl;

   //6.3
   TCanvas *c2 = new TCanvas("c2","c2");
   /*
   c2->Divide(2,2);
   c2->cd(1);
   */
   gPad->SetLogy();
   AvgPix_Event_cam0 -> GetXaxis() -> SetTitle("ADU (Analogue to Digital Unit)");
   AvgPix_Event_cam0 -> GetYaxis() -> SetTitle("Number of Events");
   AvgPix_Event_cam0->Draw();
   AvgPix_Event_cam0v2->SetLineColor(kRed);
   AvgPix_Event_cam0v2->Draw("SAMES");
   AvgPix_Event_cam0->SetStats(kFALSE);
   AvgPix_Event_cam0v2->SetStats(kFALSE);
   AvgPix_Event_cam0->GetXaxis()-> SetTitleOffset(1.4);
   /*
   c2->cd(2);
   gPad->SetLogy();
   AvgPix_Event_cam1 -> GetXaxis() -> SetTitle("ADU (Analogue to Digital Unit)");
   AvgPix_Event_cam1 -> GetYaxis() -> SetTitle("Number of Events");
   AvgPix_Event_cam1->Draw();
   AvgPix_Event_cam1v2->SetLineColor(kRed);
   AvgPix_Event_cam1v2->Draw("SAMES");
   AvgPix_Event_cam1->SetStats(kFALSE);
   AvgPix_Event_cam1v2->SetStats(kFALSE);
   c2->cd(3);
   gPad->SetLogy();
   AvgPix_Event_cam2 -> GetXaxis() -> SetTitle("ADU (Analogue to Digital Unit)");
   AvgPix_Event_cam2 -> GetYaxis() -> SetTitle("Number of Events");
   AvgPix_Event_cam2->Draw();
   AvgPix_Event_cam2v2->SetLineColor(kRed);
   AvgPix_Event_cam2v2->Draw("SAMES");
   AvgPix_Event_cam2->SetStats(kFALSE);
   AvgPix_Event_cam2v2->SetStats(kFALSE);
   c2->cd(4);
   gPad->SetLogy();
   AvgPix_Event_cam3 -> GetXaxis() -> SetTitle("ADU (Analogue to Digital Unit)");
   AvgPix_Event_cam3 -> GetYaxis() -> SetTitle("Number of Events");
   AvgPix_Event_cam3->Draw();
   AvgPix_Event_cam3v2->SetLineColor(kRed);
   AvgPix_Event_cam3v2->Draw("SAMES");
   AvgPix_Event_cam3->SetStats(kFALSE);
   AvgPix_Event_cam3v2->SetStats(kFALSE);
   */
   //c2->Print("Hand_Scan_MeanPixels.png", "png");
   Double_t Integral0 = count_cam0->Integral(-1,500);
   Double_t Integral1 = count_cam1->Integral(-1,500);
   Double_t Integral2 = count_cam2->Integral(-1,500);
   Double_t Integral3 = count_cam3->Integral(-1,500);

   cout<<Integral0<<endl;
   cout<<Integral1<<endl;
   cout<<Integral2<<endl;
   cout<<Integral3<<endl;

   TCanvas *c9 = new TCanvas("c9v2","c9v2");
   TLegend *legend0 = new TLegend(0.7, 0.7, 0.9, 0.9);
   TLegend *legend1 = new TLegend(0.7, 0.7, 0.9, 0.9);
   TLegend *legend2 = new TLegend(0.7, 0.7, 0.9, 0.9);
   TLegend *legend3 = new TLegend(0.7, 0.7, 0.9, 0.9);
   /*
   c9->Divide(2,2);
   c9->cd(1);
   */
   gPad->SetLogy();
   gPad->SetLogx();
   legend0->AddEntry(count_cam0,"Non-Spark","l");
   legend0->AddEntry(count_cam0v2,"Spark","l");
   count_cam0->GetXaxis()->SetTitle("N_{Pixels} above 200 ADU");
   count_cam0->GetYaxis()->SetTitle("Number of Events");
   count_cam0->GetXaxis()-> SetTitleOffset(1.4);
   count_cam0->SetStats(kFALSE);
   count_cam0v2->SetStats(kFALSE);
   count_cam0->Draw();
   count_cam0v2->SetLineColor(kRed);
   count_cam0v2->Draw("SAMES");
   legend0->Draw();
   /*
   c9->cd(2);
   gPad->SetLogy();
   gPad->SetLogx();
   legend1->AddEntry(count_cam1,"Non-Spark","l");
   legend1->AddEntry(count_cam1v2,"Spark","l");
   count_cam1->GetXaxis()->SetTitle("N_{Pixels} above 200 ADU");
   count_cam1->GetYaxis()->SetTitle("Number of Events");
   count_cam1->SetStats(kFALSE);
   count_cam1v2->SetStats(kFALSE);
   count_cam1->Draw();
   count_cam1v2->SetLineColor(kRed);
   count_cam1v2->Draw("SAMES");
   legend1->Draw();

   c9->cd(3);
   gPad->SetLogy();
   gPad->SetLogx();
   legend2->AddEntry(count_cam2,"Non-Spark","l");
   legend2->AddEntry(count_cam2v2,"Spark","l");
   count_cam2->GetXaxis()->SetTitle("N_{Pixels} above 200 ADU");
   count_cam2->GetYaxis()->SetTitle("Number of Events");
   count_cam2->SetStats(kFALSE);
   count_cam2v2->SetStats(kFALSE);
   count_cam2->Draw();
   count_cam2v2->SetLineColor(kRed);
   count_cam2v2->Draw("SAMES");
   legend2->Draw();

   c9->cd(4);
   gPad->SetLogy();
   gPad->SetLogx();
   legend3->AddEntry(count_cam3,"Non-Spark","l");
   legend3->AddEntry(count_cam3v2,"Spark","l");
   count_cam3->GetXaxis()->SetTitle("N_{Pixels} above 200 ADU");
   count_cam3->GetYaxis()->SetTitle("Number of Events");
   count_cam3->SetStats(kFALSE);
   count_cam3v2->SetStats(kFALSE);
   count_cam3->Draw();
   count_cam3v2->SetLineColor(kRed);
   count_cam3v2->Draw("SAMES");
   legend3->Draw();
   */
   //c9->Print("CountCamFinal.png","png");   

   TCanvas *c10 = new TCanvas("c10","c10");
   c10->Divide(2,1);
   /*
   c10->cd(1);
   gPad->SetLogx();
   AvgPixvs200ADU_0->GetXaxis()->SetTitle("Mean ADU");
   AvgPixvs200ADU_0->GetYaxis()->SetTitle("N_{Pixels} above 200 ADU");
   AvgPixvs200ADU_0->Draw("colz");
   c10->cd(2);
   gPad->SetLogx();
   AvgPixvs200ADU_0v2->GetXaxis()->SetTitle("Mean ADU");
   AvgPixvs200ADU_0v2->GetYaxis()->SetTitle("N_{Pixels} above 200 ADU");
   AvgPixvs200ADU_0v2->Draw("colz");
   c10->Print("BoxCut.pdf(","pdf");
   c10->cd(1);
   gPad->SetLogx();
   AvgPixvs200ADU_1->GetXaxis()->SetTitle("Mean ADU");
   AvgPixvs200ADU_1->GetYaxis()->SetTitle("N_{Pixels} above 200 ADU");
   AvgPixvs200ADU_1->Draw("colz");
   c10->cd(2);
   gPad->SetLogx();
   AvgPixvs200ADU_1v2->GetXaxis()->SetTitle("Mean ADU");
   AvgPixvs200ADU_1v2->GetYaxis()->SetTitle("N_{Pixels} above 200 ADU");
   AvgPixvs200ADU_1v2->Draw("colz");
   c10->Print("BoxCut.pdf","pdf");
   c10->cd(1);
   gPad->SetLogx();
   AvgPixvs200ADU_2->GetXaxis()->SetTitle("Mean ADU");
   AvgPixvs200ADU_2->GetYaxis()->SetTitle("N_{Pixels} above 200 ADU");
   AvgPixvs200ADU_2->Draw("colz");
   c10->cd(2);
   gPad->SetLogx();
   AvgPixvs200ADU_2v2->GetXaxis()->SetTitle("Mean ADU");
   AvgPixvs200ADU_2v2->GetYaxis()->SetTitle("N_{Pixels} above 200 ADU");
   AvgPixvs200ADU_2v2->Draw("colz");
   c10->Print("BoxCut.pdf","pdf");
   */
   c10->cd(1);
   gPad->SetLogx();
   AvgPixvs200ADU_3->GetXaxis()->SetTitle("Mean ADU");
   AvgPixvs200ADU_3->GetYaxis()->SetTitle("N_{Pixels} above 200 ADU");
   AvgPixvs200ADU_3->GetXaxis()->SetTitleOffset(1.4);
   AvgPixvs200ADU_3->GetYaxis()->SetTitleOffset(1.4);
   AvgPixvs200ADU_3->Draw("colz");
   c10->cd(2);
   gPad->SetLogx();
   AvgPixvs200ADU_3v2->GetXaxis()->SetTitle("Mean ADU");
   AvgPixvs200ADU_3v2->GetYaxis()->SetTitle("N_{Pixels} above 200 ADU");
   AvgPixvs200ADU_3v2->GetXaxis()->SetTitleOffset(1.4);
   AvgPixvs200ADU_3v2->GetYaxis()->SetTitleOffset(1.4);
   AvgPixvs200ADU_3v2->Draw("colz");
   c10->Print("BoxCut.png","png");
   gStyle->SetNumberContours(64);

   cout<<Total_Entries<<endl;
   cout<<Spark_Entries<<endl;

   return 0;
}

