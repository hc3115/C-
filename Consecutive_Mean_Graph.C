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

void Consecutive_Mean_Graph() {

   TFile *f = TFile::Open("CCD_images_R1324005.root");
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
   cout<<nentriesB<<endl;
   Double_t xcam0[5],xcam1[5],xcam2[5],xcam3[5];
   Double_t ycam0[5],ycam1[5],ycam2[5],ycam3[5];
   Double_t Count = 0;

   for (Int_t i=0;i<nentriesB;i++) {
      TreeB->GetEntry(i); //Gets each entry in current tree branch(es)

      Double_t AvgPix_B_cam0;
      Double_t AvgPix_B_cam1;
      Double_t AvgPix_B_cam2;
      Double_t AvgPix_B_cam3;
      
      for (Int_t j=0; j<Bias_cam0->GetXaxis()->GetNbins(); j++) {
	 
	 for (Int_t z=0; z<Bias_cam0->GetYaxis()->GetNbins(); z++) {

	    AvgPix_B_cam0 += Bias_cam0->GetBinContent(j+1,z+1); // contributes to finding mean pixel value of event (bias-subtracted)
	    AvgPix_B_cam1 += Bias_cam1->GetBinContent(j+1,z+1);
	    AvgPix_B_cam2 += Bias_cam2->GetBinContent(j+1,z+1);
	    AvgPix_B_cam3 += Bias_cam3->GetBinContent(j+1,z+1);

	 }
	 
      }

      AvgPix_B_cam0 = AvgPix_B_cam0 / 148225; //contributes to finding mean pixel value for event (bias-subtracted)
      AvgPix_B_cam1 = AvgPix_B_cam1 / 148225;
      AvgPix_B_cam2 = AvgPix_B_cam2 / 148225;
      AvgPix_B_cam3 = AvgPix_B_cam3 / 148225;

      Bias_cam0_Avg->Add(Bias_cam0); //add current entry
      Bias_cam1_Avg->Add(Bias_cam1); //add current entry
      Bias_cam2_Avg->Add(Bias_cam2); //add current entry
      Bias_cam3_Avg->Add(Bias_cam3); //add current entry

      xcam0[(int)Count] = Count-nentriesB, xcam1[(int)Count] = Count-nentriesB, xcam2[(int)Count] = Count-nentriesB, xcam3[(int)Count] = Count-nentriesB;
      ycam0[(int)Count] = AvgPix_B_cam0, ycam1[(int)Count] = AvgPix_B_cam1, ycam2[(int)Count] = AvgPix_B_cam2, ycam3[(int)Count] = AvgPix_B_cam3;

      Count++;

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
   Double_t Excam0[100],Excam1[100],Excam2[100],Excam3[100];
   Double_t Eycam0[100],Eycam1[100],Eycam2[100],Eycam3[100];
   Double_t ECount = 0;

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

      Double_t AvgPix_E_cam0; //these variables are will give the mean pixel value for each event individually to see if they are above the cut off pixel value which would mean they are a spark.
      Double_t AvgPix_E_cam1;
      Double_t AvgPix_E_cam2;
      Double_t AvgPix_E_cam3;

      for (Int_t j=0; j<Event_cam0->GetXaxis()->GetNbins(); j++) {

         for (Int_t z=0; z<Event_cam0->GetYaxis()->GetNbins(); z++) {

            AvgPix_E_cam0 += Event_cam0->GetBinContent(j+1,z+1); // contributes to finding mean pixel value of event (bias-subtracted)
            AvgPix_E_cam1 += Event_cam1->GetBinContent(j+1,z+1);
            AvgPix_E_cam2 += Event_cam2->GetBinContent(j+1,z+1);
            AvgPix_E_cam3 += Event_cam3->GetBinContent(j+1,z+1);

	 }

      }

      AvgPix_E_cam0 = AvgPix_E_cam0 / 148225; //contributes to finding mean pixel value for event (bias-subtracted)
      AvgPix_E_cam1 = AvgPix_E_cam1 / 148225;
      AvgPix_E_cam2 = AvgPix_E_cam2 / 148225;
      AvgPix_E_cam3 = AvgPix_E_cam3 / 148225;
      
      Excam0[(int)ECount] = ECount, Excam1[(int)ECount] = ECount, Excam2[(int)ECount] = ECount, Excam3[(int)ECount] = ECount;
      Eycam0[(int)ECount] = AvgPix_E_cam0, Eycam1[(int)ECount] = AvgPix_E_cam1, Eycam2[(int)ECount] = AvgPix_E_cam2, Eycam3[(int)ECount] = AvgPix_E_cam3;

      ECount++;
      
   }

   TCanvas *c1 = new TCanvas("c1","Consecutive_Mean");
   /*
   c1->Divide(2,2);

   c1->cd(1);
   */
   TMultiGraph *mgcam0 = new TMultiGraph();
   mgcam0->SetTitle("Variation of Mean ADU with event");

   TGraph *Bgrcam0 = new TGraph(nentriesB,xcam0,ycam0);
   Bgrcam0->SetLineColor(5);
   mgcam0->Add(Bgrcam0);
   TGraph *Egrcam0 = new TGraph(nentriesE,Excam0,Eycam0);
   Egrcam0->SetLineColor(6);
   mgcam0->Add(Egrcam0);

   mgcam0->SetMaximum(2825);
   mgcam0->SetMinimum(2805);
   mgcam0->Draw("ALP");
   mgcam0->GetXaxis()->SetTitle("Event Number");
   mgcam0->GetYaxis()->SetTitle("ADU");
   mgcam0->GetYaxis()->SetTitleOffset(1.4);

   TLegend *lcam0 = new TLegend(0.7, 0.1, 0.9, 0.3);
   lcam0->SetHeader("Type of event"); // option "C" allows to center the header
   lcam0->AddEntry(Bgrcam0,"Bias","l");
   lcam0->AddEntry(Egrcam0,"Exposure","l");
   lcam0->SetTextSize(0.03);
   lcam0->SetTextAlign(22);
   lcam0->SetMargin(0.4);
   lcam0->Draw();
   /*
   c1->cd(2);

   TMultiGraph *mgcam1 = new TMultiGraph();
   mgcam1->SetTitle("Variation of Mean ADU with event");

   TGraph *Bgrcam1 = new TGraph(nentriesB,xcam1,ycam1);
   Bgrcam1->SetLineColor(5);
   mgcam1->Add(Bgrcam1);
   TGraph *Egrcam1 = new TGraph(nentriesE,Excam1,Eycam1);
   Egrcam1->SetLineColor(6);
   mgcam1->Add(Egrcam1);

   mgcam1->SetMaximum(2765);
   mgcam1->SetMinimum(2745);
   mgcam1->Draw("ALP");
   mgcam1->GetXaxis()->SetTitle("Event Number");
   mgcam1->GetYaxis()->SetTitle("ADU");
   mgcam1->GetYaxis()->SetTitleOffset(1.4);

   TLegend *lcam1 = new TLegend(0.7, 0.1, 0.9, 0.3);
   lcam1->SetHeader("Type of event"); // option "C" allows to center the header
   lcam1->AddEntry(Bgrcam1,"Bias","l");
   lcam1->AddEntry(Egrcam1,"Exposure","l");
   lcam1->SetTextSize(0.03);
   lcam1->SetTextAlign(22);
   lcam1->SetMargin(0.4);
   lcam1->Draw();

   c1->cd(3);
   TMultiGraph *mgcam2 = new TMultiGraph();
   mgcam2->SetTitle("Variation of Mean ADU with event");

   TGraph *Bgrcam2 = new TGraph(nentriesB,xcam2,ycam2);
   Bgrcam2->SetLineColor(5);
   mgcam2->Add(Bgrcam2);
   TGraph *Egrcam2 = new TGraph(nentriesE,Excam2,Eycam2);
   Egrcam2->SetLineColor(6);
   mgcam2->Add(Egrcam2);

   mgcam2->SetMaximum(2885);
   mgcam2->SetMinimum(2865);
   mgcam2->Draw("ALP");
   mgcam2->GetXaxis()->SetTitle("Event Number");
   mgcam2->GetYaxis()->SetTitle("ADU");
   mgcam2->GetYaxis()->SetTitleOffset(1.4);

   TLegend *lcam2 = new TLegend(0.7, 0.1, 0.9, 0.3);
   lcam2->SetHeader("Type of event"); // option "C" allows to center the header
   lcam2->AddEntry(Bgrcam2,"Bias","l");
   lcam2->AddEntry(Egrcam2,"Exposure","l");
   lcam2->SetTextSize(0.03);
   lcam2->SetTextAlign(22);
   lcam2->SetMargin(0.4);
   lcam2->Draw();

   c1->cd(4);
   TMultiGraph *mgcam3 = new TMultiGraph();
   mgcam3->SetTitle("Variation of Mean ADU with event");

   TGraph *Bgrcam3 = new TGraph(nentriesB,xcam3,ycam3);
   Bgrcam3->SetLineColor(5);
   mgcam3->Add(Bgrcam3);
   TGraph *Egrcam3 = new TGraph(nentriesE,Excam3,Eycam3);
   Egrcam3->SetLineColor(6);
   mgcam3->Add(Egrcam3);

   mgcam3->SetMaximum(3560);
   mgcam3->SetMinimum(3540);
   mgcam3->Draw("ALP");
   mgcam3->GetXaxis()->SetTitle("Event Number");
   mgcam3->GetYaxis()->SetTitle("ADU");
   mgcam3->GetYaxis()->SetTitleOffset(1.4);

   TLegend *lcam3 = new TLegend(0.7, 0.1, 0.9, 0.3);
   lcam3->SetHeader("Type of event"); // option "C" allows to center the header
   lcam3->AddEntry(Bgrcam3,"Bias","l");
   lcam3->AddEntry(Egrcam3,"Exposure","l");
   lcam3->SetTextSize(0.03);
   lcam3->SetTextAlign(22);
   lcam3->SetMargin(0.4);
   lcam3->Draw();
   */
   return;
}
