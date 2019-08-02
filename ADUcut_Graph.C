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

void ADUcut_Graph() {

   TFile *f = TFile::Open("CCD_images_R1323047.root");
   if ((!f) || f->IsZombie()) { delete f; return;}
   TTree *TreeB; f->GetObject("Biases", TreeB);
   if (!TreeB) { delete f; return;}
   TH2I *Bias_cam0=0, *Bias_cam1=0, *Bias_cam2=0, *Bias_cam3=0;

   TH2I *Bias_cam0_Avg= new TH2I("Bias0","Bias_cam0_Avg",385, 0, 3080, 385, 0, 3080); //See lab book to see how I got x and y axis range.
   TH2I *Bias_cam1_Avg= new TH2I("Bias1","Bias_cam1_Avg",385, 0, 3080, 385, 0, 3080); //See lab book to see how I got x and y axis range.
   TH2I *Bias_cam2_Avg= new TH2I("Bias2","Bias_cam2_Avg",385, 0, 3080, 385, 0, 3080); //See lab book to see how I got x and y axis range.
   TH2I *Bias_cam3_Avg= new TH2I("Bias3","Bias_cam3_Avg",385, 0, 3080, 385, 0, 3080); //See lab book to see how I got x and y axis range.

   TreeB -> SetBranchAddress("Bias_cam0", &Bias_cam0);
   TreeB -> SetBranchAddress("Bias_cam1", &Bias_cam1);
   TreeB -> SetBranchAddress("Bias_cam2", &Bias_cam2);
   TreeB -> SetBranchAddress("Bias_cam3", &Bias_cam3);

   Double_t AvgPix_B_cam0;
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

   }

   Bias_cam0_Avg->Scale(1.0/nentriesB);
   Bias_cam1_Avg->Scale(1.0/nentriesB);
   Bias_cam2_Avg->Scale(1.0/nentriesB);
   Bias_cam3_Avg->Scale(1.0/nentriesB);

   TTree *TreeE; f->GetObject("Events", TreeE);
   if (!TreeE) { delete f; return; }
   TH2I *Event_cam0 = 0, *Event_cam1 = 0, *Event_cam2 = 0, *Event_cam3 = 0;

   TreeE -> SetBranchAddress("Event_cam0", &Event_cam0);
   TreeE -> SetBranchAddress("Event_cam1", &Event_cam1);
   TreeE -> SetBranchAddress("Event_cam2", &Event_cam2);
   TreeE -> SetBranchAddress("Event_cam3", &Event_cam3);

   Int_t nentriesE = (Double_t)TreeE->GetEntries();
   Double_t xcam0[100],xcam1[100],xcam2[100],xcam3[100];
   Double_t ycam0[100],ycam1[100],ycam2[100],ycam3[100];
   Double_t Count = 0;

   for (Int_t i=0;i<nentriesE;i++) {
      TreeE->GetEntry(i); //Gets each entry in current tree branch(es)

      TH2I *CEvent_cam0 = (TH2I*) Event_cam0->Clone();
      CEvent_cam0->Add(Bias_cam0_Avg,-1);

      TH2I *CEvent_cam1 = (TH2I*) Event_cam1->Clone();
      CEvent_cam1->Add(Bias_cam1_Avg,-1);

      TH2I *CEvent_cam2 = (TH2I*) Event_cam2->Clone();
      CEvent_cam2->Add(Bias_cam2_Avg,-1);

      TH2I *CEvent_cam3 = (TH2I*) Event_cam3->Clone();
      CEvent_cam3->Add(Bias_cam3_Avg,-1);

      Double_t AvgPix_BE_cam0; //these variables are will give the mean pixel value for each event individually to see if they are above the cut off pixel value which would mean they are a spark.
      Double_t AvgPix_BE_cam1;
      Double_t AvgPix_BE_cam2;
      Double_t AvgPix_BE_cam3;

      TH1I *Pixel_Event_cam0= new TH1I("Pixel0","Pixel_Event0",10100, -100, 10000);
      TH1I *Pixel_Event_cam1= new TH1I("Pixel1","Pixel_Event1",10100, -100, 10000);
      TH1I *Pixel_Event_cam2= new TH1I("Pixel2","Pixel_Event2",10100, -100, 10000);
      TH1I *Pixel_Event_cam3= new TH1I("Pixel3","Pixel_Event3",10100, -100, 10000);

      Int_t countcam0 = 0;
      Int_t countcam1 = 0;
      Int_t countcam2 = 0;
      Int_t countcam3 = 0;

      for (Int_t j=0; j<CEvent_cam0->GetXaxis()->GetNbins(); j++) {

         for (Int_t z=0; z<CEvent_cam0->GetYaxis()->GetNbins(); z++) {

	    Pixel_Event_cam0->Fill(CEvent_cam0->GetBinContent(j+1,z+1)); // gets pixel value in specified bin in event for camera 0,
            Pixel_Event_cam1->Fill(CEvent_cam1->GetBinContent(j+1,z+1)); // camera 1,
            Pixel_Event_cam2->Fill(CEvent_cam2->GetBinContent(j+1,z+1)); // camera 2,
            Pixel_Event_cam3->Fill(CEvent_cam3->GetBinContent(j+1,z+1)); // camera 3.

	 }
      }

      Double_t StdDevcam0 = Pixel_Event_cam0->GetStdDev(), Meancam0 = Pixel_Event_cam0->GetMean();
      Double_t StdDevcam1 = Pixel_Event_cam1->GetStdDev(), Meancam1 = Pixel_Event_cam1->GetMean();
      Double_t StdDevcam2 = Pixel_Event_cam2->GetStdDev(), Meancam2 = Pixel_Event_cam2->GetMean();
      Double_t StdDevcam3 = Pixel_Event_cam3->GetStdDev(), Meancam3 = Pixel_Event_cam3->GetMean();
      if(i==0){cout<<Meancam0<<" "<<StdDevcam0<<endl;}
      for (Int_t j=0; j<CEvent_cam0->GetXaxis()->GetNbins(); j++) {
	 
	 for (Int_t z=0; z<CEvent_cam0->GetYaxis()->GetNbins(); z++) {
	    
	    AvgPix_BE_cam0 += CEvent_cam0->GetBinContent(j+1,z+1); // contributes to finding mean pixel value of event (bias-subtracted)
	    AvgPix_BE_cam1 += CEvent_cam1->GetBinContent(j+1,z+1);
	    AvgPix_BE_cam2 += CEvent_cam2->GetBinContent(j+1,z+1);
	    AvgPix_BE_cam3 += CEvent_cam3->GetBinContent(j+1,z+1);
	    
	    if (CEvent_cam0->GetBinContent(j+1,z+1)>200){//Meancam0+(4*StdDevcam0)){ // contributes to finding pixels above Average  Mean + 4SD
	       countcam0++;
	    }
	    if (CEvent_cam1->GetBinContent(j+1,z+1)>200){//Meancam1+(4*StdDevcam1)){
	       countcam1++;
	    }
	    if (CEvent_cam2->GetBinContent(j+1,z+1)>200){//Meancam2+(4*StdDevcam2)){
	       countcam2++;
	    }
	    if (CEvent_cam3->GetBinContent(j+1,z+1)>200){//Meancam3+(4*StdDevcam3)){
	       countcam3++;
	    }

	 }

      }

      AvgPix_BE_cam0 = AvgPix_BE_cam0 / 148225; //contributes to finding mean pixel value for event (bias-subtracted)
      AvgPix_BE_cam1 = AvgPix_BE_cam1 / 148225;
      AvgPix_BE_cam2 = AvgPix_BE_cam2 / 148225;
      AvgPix_BE_cam3 = AvgPix_BE_cam3 / 148225;
      
      xcam0[(int)Count] = Count, xcam1[(int)Count] = Count, xcam2[(int)Count] = Count, xcam3[(int)Count] = Count;
      ycam0[(int)Count] = countcam0, ycam1[(int)Count] = countcam1, ycam2[(int)Count] = countcam2, ycam3[(int)Count] = countcam3;

      Count++;
      
      delete Pixel_Event_cam0;
      delete Pixel_Event_cam1;
      delete Pixel_Event_cam2;
      delete Pixel_Event_cam3;
   }

   TCanvas *c1 = new TCanvas("c1","Consecutive_Mean");
   /*
   c1->Divide(2,2);
   */
   c1->cd(1);
   TGraph *grcam0 = new TGraph(nentriesE,xcam0,ycam0);
   grcam0->GetXaxis()->SetTitle("Event Number");
   grcam0->GetYaxis()->SetTitle("Number of Pixels");
   grcam0->SetMaximum(50);
   //gPad->SetLogy();
   grcam0->SetTitle("Variation of N_{pixels} above 200 ADU with event (cam0)");
   grcam0->SetLineColor(5);
   grcam0->Draw("AL*");
   /*
   c1->cd(2);
   TGraph *grcam1 = new TGraph(nentriesE,xcam1,ycam1);
   grcam1->GetXaxis()->SetTitle("Event Number");
   grcam1->GetYaxis()->SetTitle("Number of Pixels");
   grcam1->SetMaximum(50);
   //gPad->SetLogy();
   grcam1->SetTitle("Variation of N_{pixels} above 200 ADU  with event (cam1)");
   grcam1->SetLineColor(5);
   grcam1->Draw("AL*");
   
   c1->cd(3);
   TGraph *grcam2 = new TGraph(nentriesE,xcam2,ycam2);
   grcam2->GetXaxis()->SetTitle("Event Number");
   grcam2->GetYaxis()->SetTitle("Number of Pixels");
   grcam2->SetMaximum(50);
   //gPad->SetLogy();
   grcam2->SetTitle("Variation of N_{pixels} above 200 ADU  with event (cam2)");
   grcam2->SetLineColor(5);
   grcam2->Draw("AL*");
   
   c1->cd(4);
   TGraph *grcam3 = new TGraph(nentriesE,xcam3,ycam3);
   grcam3->GetXaxis()->SetTitle("Event Number");
   grcam3->GetYaxis()->SetTitle("Number of Pixels");
   grcam3->SetMaximum(50);
   //gPad->SetLogy();
   grcam3->SetTitle("Variation of N_{pixels} above 200 ADU  with event (cam3)");
   grcam3->SetLineColor(5);
   grcam3->Draw("AL*");
   */
   return;
}
