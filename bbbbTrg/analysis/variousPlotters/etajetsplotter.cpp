//c++ -o etaj etajetsplotter.cpp ../src/Event.cc ../src/TriggerMaker.cc `root-config --cflags --glibs`

#include <iostream>
#include <string.h>
#include <iomanip>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "../interface/TriggerMaker.h"
#include <iostream>
#include <fstream>
#include "time.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TLine.h"
#include "TLegend.h"
#include "TEfficiency.h"
#include "TLatex.h"

using namespace hltObj;

int main(){


    TFile* f = new TFile("../../test/filter_tests_2018_nob.root");
    std::string branch = "checkTrg/trgObjTree";

    Event ev(f, branch);
    //time checking for algorithms
    time_t my_time = time(NULL); 
    printf("%s", ctime(&my_time));

    //---------- eta ---------

    TH1F* h_calo = new TH1F("h_calo", "h_calo", 50, -5, 5);
    h_calo->SetMarkerStyle(24);
    h_calo->SetLineColor(kMagenta+1);
    h_calo->SetMarkerColor(kMagenta+1);

    TH1F* h_off = new TH1F("h_off", "h_off", 50, -5, 5);
    h_off->SetMarkerStyle(22);
    h_off->SetLineColor(kSpring);
    h_off->SetMarkerColor(kSpring);

    TH1F* h_pf = new TH1F("h_pf", "h_pf", 50, -5, 5);
    h_pf->SetMarkerStyle(8);
    h_pf->SetLineColor(kBlack);
    h_pf->SetMarkerColor(kBlack);
    h_pf->GetXaxis()->SetTitle("#eta (TriggerObject)");
    h_pf->GetXaxis()->SetTitleOffset(1.2);
    h_pf->GetYaxis()->SetTitle("(1/N_{jet})dN_{jet}/ 0.2");
    h_pf->SetTitle("");

    TH1F* h_L1 = new TH1F("h_L1", "h_L1", 50, -5, 5);
    h_L1->SetMarkerStyle(25);
    h_L1->SetLineColor(kBlue);
    h_L1->SetMarkerColor(kBlue);

    TLegend* leg_c = new TLegend(0.85, 0.83, 0.65, 0.65);
    leg_c->SetBorderSize(0.);
    leg_c->AddEntry(h_pf, "PFJets", "P");
    leg_c->AddEntry(h_calo, "CaloJets", "P");
    leg_c->AddEntry(h_L1, "L1Jets", "P");
    leg_c->AddEntry(h_off, "RecoJets", "P");

    //---------- phi ---------

    TH1F* h_calo_p = new TH1F("h_calo_p", "h_calo_p", 50, -3.14, 3.14);
    h_calo_p->SetMarkerStyle(24);
    h_calo->SetLineColor(kMagenta+1);
    h_calo->SetMarkerColor(kMagenta+1);

    TH1F* h_off_p = new TH1F("h_off_p", "h_off_p", 50, -3.14, 3.14);
    h_off_p->SetMarkerStyle(22);
    h_off_p->SetLineColor(kSpring);
    h_off_p->SetMarkerColor(kSpring);

    TH1F* h_pf_p = new TH1F("h_pf_p", "h_pf_p", 50, -3.14, 3.14);
    h_pf_p->SetMarkerStyle(8);
    h_pf_p->SetLineColor(kBlack);
    h_pf_p->SetMarkerColor(kBlack);
    h_pf_p->GetXaxis()->SetTitle("#phi");
    h_pf_p->GetXaxis()->SetTitleOffset(1.2);
    h_pf_p->GetYaxis()->SetTitle("(1/N_{jet})dN_{jet}/ 0.1256");
    h_pf_p->SetTitle("");

    TH1F* h_L1_p = new TH1F("h_L1_p", "h_L1_p", 50, -3.14, 3.14);
    h_L1_p->SetMarkerStyle(25);
    h_L1_p->SetLineColor(kBlue);
    h_L1_p->SetMarkerColor(kBlue);

    TLegend* leg_c_p = new TLegend(0.85, 0.83, 0.65, 0.65);
    leg_c_p->SetBorderSize(0.);
    leg_c_p->AddEntry(h_pf_p, "PFJets", "P");
    leg_c_p->AddEntry(h_calo_p, "CaloJets", "P");
    leg_c_p->AddEntry(h_L1_p, "L1Jets", "P");
    leg_c_p->AddEntry(h_off_p, "RecoJets", "P");

    int entries = ev.GetEntries();

    for(int i = 0; i < entries; i++){

        ev.Generate();
        ev.UnpackCollections();
        //matching efficiency both for reco jets
        //and for trgobj
        std::vector<hltObj::Jet*> pf = ev.GetPFJets();
        std::vector<hltObj::Jet*> reco = ev.GetRecoJets();
        std::vector<hltObj::Jet*> calo = ev.GetCaloJets();
        std::vector<hltObj::Jet*> l1 = ev.GetL1Jets();

        for(auto c : pf){
            h_pf->Fill(c->eta);
            h_pf_p->Fill(c->phi);
        }

        for(auto c : reco){
            h_off->Fill(c->eta);
            h_off_p->Fill(c->phi);
        }

        for(auto c : calo){
            h_calo->Fill(c->eta);
            h_calo_p->Fill(c->phi);
        }

        for(auto c : l1){
            h_L1->Fill(c->eta);
            h_L1->Fill(c->phi);
        }
    }

    h_pf->Scale(1./h_pf->Integral());
    h_off->Scale(1./h_off->Integral());
    h_calo->Scale(1./h_calo->Integral());
    h_L1->Scale(1./h_L1->Integral());

    gStyle->SetOptStat(0);
    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 700);
    h_pf->Draw("pe");
    h_off->Draw("pe same");
    h_calo->Draw("pe same");
    h_L1->Draw("pe same");
    leg_c->Draw();
    c->Draw();
    c->SaveAs("./EtaJetsDist.pdf");

    TCanvas* c1 = new TCanvas("c1", "c1", 1000, 1000, 1000, 700);
    h_L1_p->Draw("pe");
    h_pf_p->Draw("pe same");
    h_off_p->Draw("pe same");
    h_calo_p->Draw("pe same");
    leg_c_p->Draw();
    c1->Draw();
    c1->SaveAs("./PhiJetsDist.pdf");

    return 0;
}