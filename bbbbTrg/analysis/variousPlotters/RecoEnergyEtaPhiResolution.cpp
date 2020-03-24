//c++ -o res RecoEnergyEtaPhiResolution.cpp ../src/Event.cc ../src/TriggerMaker.cc `root-config --cflags --glibs`

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

    //Cutting on Reco Objects
    hltObj::KinCuts RecoCut;
    RecoCut.Type = "RecoJets";
    RecoCut.MinPt = 20.0;
    RecoCut.MinEta = -5;
    RecoCut.MaxEta = 5;
    ev.AddCut(RecoCut);

    //Cutting on Reco Objects
    hltObj::KinCuts PFCut;
    RecoCut.Type = "PFJets";
    RecoCut.MinPt = 20.0;
    RecoCut.MinEta = -2.4;
    RecoCut.MaxEta = 2.4;
    ev.AddCut(PFCut);
    

    //---------- pt ---------

    TH1F* h_calo = new TH1F("h_calo", "h_calo", 50, -1, 1);
    h_calo->SetMarkerStyle(24);
    h_calo->SetLineColor(kMagenta+1);
    h_calo->SetMarkerColor(kMagenta+1);

    TH1F* h_pf = new TH1F("h_pf", "h_pf", 50, -1, 1);
    h_pf->SetMarkerStyle(8);
    h_pf->SetLineColor(kBlack);
    h_pf->SetMarkerColor(kBlack);
    h_pf->GetXaxis()->SetTitle("(P_{T,offline}-P_{T,online})/P_{T,offline}");
    h_pf->GetXaxis()->SetTitleOffset(1.2);
    h_pf->GetYaxis()->SetTitle("(1/N_{jet})dN_{jet}/ 0.02");
    h_pf->SetTitle("");

    TH1F* h_L1 = new TH1F("h_L1", "h_L1", 50, -1, 1);
    h_L1->SetMarkerStyle(25);
    h_L1->SetLineColor(kBlue);
    h_L1->SetMarkerColor(kBlue);

    TLegend* leg_c = new TLegend(0.85, 0.83, 0.65, 0.65);
    leg_c->SetBorderSize(0.);
    leg_c->AddEntry(h_pf, "PFJets", "P");
    leg_c->AddEntry(h_calo, "CaloJets", "P");
    leg_c->AddEntry(h_L1, "L1Jets", "P");

    //---------- energy ---------

    TH1F* h_calo_en = new TH1F("h_calo_en", "h_calo_en", 50, -1, 1);
    h_calo_en->SetMarkerStyle(24);
    h_calo_en->SetLineColor(kMagenta+1);
    h_calo_en->SetMarkerColor(kMagenta+1);

    TH1F* h_pf_en = new TH1F("h_pf_en", "h_pf_en", 50, -1, 1);
    h_pf_en->SetMarkerStyle(8);
    h_pf_en->SetLineColor(kBlack);
    h_pf_en->SetMarkerColor(kBlack);
    h_pf_en->GetXaxis()->SetTitle("(E_{T,offline}-E_{T,online})/E_{T,offline}");
    h_pf_en->GetXaxis()->SetTitleOffset(1.2);
    h_pf_en->GetYaxis()->SetTitle("(1/N_{jet})dN_{jet}/ 0.02");
    h_pf_en->SetTitle("");

    TH1F* h_L1_en = new TH1F("h_L1_en", "h_L1_en", 50, -1, 1);
    h_L1_en->SetMarkerStyle(25);
    h_L1_en->SetLineColor(kBlue);
    h_L1_en->SetMarkerColor(kBlue);

    TLegend* leg_en = new TLegend(0.85, 0.83, 0.65, 0.65);
    leg_en->SetBorderSize(0.);
    leg_en->AddEntry(h_pf_en, "PFJets", "P");
    leg_en->AddEntry(h_calo_en, "CaloJets", "P");
    leg_en->AddEntry(h_L1_en, "L1Jets", "P");

    //---------- phi ---------

    TH1F* h_calo_p = new TH1F("h_calo_p", "h_calo_p", 50, -0.4, 0.4);
    h_calo_p->SetMarkerStyle(24);
    h_calo_p->SetLineColor(kMagenta+1);
    h_calo_p->SetMarkerColor(kMagenta+1);

    TH1F* h_pf_p = new TH1F("h_pf_p", "h_pf_p", 50, -0.4, 0.4);
    h_pf_p->SetMarkerStyle(8);
    h_pf_p->SetLineColor(kBlack);
    h_pf_p->SetMarkerColor(kBlack);
    h_pf_p->GetXaxis()->SetTitle("(#phi_{offline}-#phi_{online})/#phi_{offline}");
    h_pf_p->GetXaxis()->SetTitleOffset(1.2);
    h_pf_p->GetYaxis()->SetTitle("(1/N_{jet})dN_{jet}/ 0.016");
    h_pf_p->SetTitle("");

    TH1F* h_L1_p = new TH1F("h_L1_p", "h_L1_p", 50, -0.4, 0.4);
    h_L1_p->SetMarkerStyle(25);
    h_L1_p->SetLineColor(kBlue);
    h_L1_p->SetMarkerColor(kBlue);

    TLegend* leg_c_p = new TLegend(0.85, 0.83, 0.65, 0.65);
    leg_c_p->SetBorderSize(0.);
    leg_c_p->AddEntry(h_pf_p, "PFJets", "P");
    leg_c_p->AddEntry(h_calo_p, "CaloJets", "P");
    leg_c_p->AddEntry(h_L1_p, "L1Jets", "P");

    //---------- eta ---------

    TH1F* h_calo_e = new TH1F("h_calo_e", "h_calo_e", 50, -0.4, 0.4);
    h_calo_e->SetMarkerStyle(24);
    h_calo_e->SetLineColor(kMagenta+1);
    h_calo_e->SetMarkerColor(kMagenta+1);

    TH1F* h_pf_e = new TH1F("h_pf_e", "h_pf_e", 50, -0.4, 0.4);
    h_pf_e->SetMarkerStyle(8);
    h_pf_e->SetLineColor(kBlack);
    h_pf_e->SetMarkerColor(kBlack);
    h_pf_e->GetXaxis()->SetTitle("(#eta_{online}-#eta_{offline})/#eta_{offline}");
    h_pf_e->GetXaxis()->SetTitleOffset(1.2);
    h_pf_e->GetYaxis()->SetTitle("(1/N_{jet})dN_{jet}/ 0.016");
    h_pf_e->SetTitle("");

    TH1F* h_L1_e = new TH1F("h_L1_e", "h_L1_e", 50, -0.4, 0.4);
    h_L1_e->SetMarkerStyle(25);
    h_L1_e->SetLineColor(kBlue);
    h_L1_e->SetMarkerColor(kBlue);

    TLegend* leg_c_e = new TLegend(0.85, 0.83, 0.65, 0.65);
    leg_c_e->SetBorderSize(0.);
    leg_c_e->AddEntry(h_pf_e, "PFJets", "P");
    leg_c_e->AddEntry(h_calo_e, "CaloJets", "P");
    leg_c_e->AddEntry(h_L1_e, "L1Jets", "P");

    //int entries = ev.GetEntries();
    int entries = 50000;

    for(int i = 0; i < entries; i++){

        ev.Generate();
        ev.UnpackCollections();
        //matching efficiency both for reco jets
        //and for trgobj
        ev.bMatch(0.4, "RecoJets");
        ev.jetMatch(0.2, "RecoJetsMatched", "PFBJets");
        std::vector<hltObj::Jet*> pf = ev.GetPFBJets();

        for(auto c : pf){
            if(c->matched){
                h_pf->Fill((c->MatchedObj->pt - c->pt)/c->MatchedObj->pt);
                h_pf_p->Fill((c->MatchedObj->phi - c->phi)/c->MatchedObj->phi);
                h_pf_e->Fill((c->MatchedObj->eta - c->eta)/c->MatchedObj->eta);
                h_pf_en->Fill((c->MatchedObj->et - c->et)/c->MatchedObj->et);
            }
        }
        ev.jetMatch(0.2, "RecoJetsMatched", "CaloJets");
        std::vector<hltObj::Jet*> calo = ev.GetCaloJets();

        for(auto c : calo){
            if(c->matched){
                h_calo->Fill((c->MatchedObj->pt - c->pt)/c->MatchedObj->pt);
                h_calo_p->Fill((c->MatchedObj->phi - c->phi)/c->MatchedObj->phi);
                h_calo_e->Fill((c->MatchedObj->eta - c->eta)/c->MatchedObj->eta);
                h_calo_en->Fill((c->MatchedObj->et - c->et)/c->MatchedObj->et);
            }
        }

        ev.jetMatch(0.2, "RecoJetsMatched", "L1Jets");
        std::vector<hltObj::Jet*> l1 = ev.GetL1Jets();

        for(auto c : l1){
            if(c->matched){
                h_L1->Fill((c->MatchedObj->pt - c->pt)/c->MatchedObj->pt);
                h_L1_p->Fill((c->MatchedObj->phi - c->phi)/c->MatchedObj->phi);
                h_L1_e->Fill((c->MatchedObj->eta - c->eta)/c->MatchedObj->eta);
                h_L1_en->Fill((c->MatchedObj->et - c->et)/c->MatchedObj->et);
            }
        }
    }

    h_pf->Scale(1./h_pf->Integral());
    h_calo->Scale(1./h_calo->Integral());
    h_L1->Scale(1./h_L1->Integral());
    h_pf_p->Scale(1./h_pf_p->Integral());
    h_calo_p->Scale(1./h_calo_p->Integral());
    h_L1_p->Scale(1./h_L1_p->Integral());
    h_pf_e->Scale(1./h_pf_e->Integral());
    h_calo_e->Scale(1./h_calo_e->Integral());
    h_L1_e->Scale(1./h_L1_e->Integral());
    h_pf_en->Scale(1./h_pf_en->Integral());
    h_calo_en->Scale(1./h_calo_en->Integral());
    h_L1_en->Scale(1./h_L1_en->Integral());

    TF1*fpf = new TF1("fpf", "crystalball+expo", -1, 1);
    fpf->SetLineColor(kBlack);
    fpf->SetParameter(0, 0.18);
    fpf->SetParameter(1, 0.);
    fpf->SetParameter(2, 0.05);
    fpf->SetParameter(3, 1);
    fpf->SetParameter(4, 3);
    TF1*fcalo = new TF1("fcalo", "crystalball", -1, 1);
    fcalo->SetLineColor(kMagenta+1);
    fcalo->SetParameter(0, 0.18);
    fcalo->SetParameter(1, 0.05);
    fcalo->SetParameter(2, 0.);
    TF1*fl1 = new TF1("fl1", "crystalball", -1, 1);
    fl1->SetLineColor(kBlue);
    fl1->SetParameter(0, 0.18);
    fl1->SetParameter(1, 0.05);
    fl1->SetParameter(2, 0.);

    h_pf->Fit(fpf);
    h_calo->Fit(fcalo);
    h_L1->Fit(fl1);

    gStyle->SetOptStat(0);
    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 700);
    h_pf->Draw("pe");
    fpf->Draw("same");
    h_calo->Draw("pe same");
    fcalo->Draw("same");
    h_L1->Draw("pe same");
    fl1->Draw("same");
    leg_c->Draw();
    c->Draw();
    c->SaveAs("./PtJetsRes.pdf");

    h_pf_p->Fit(fpf);
    h_calo_p->Fit(fcalo);
    h_L1_p->Fit(fl1);

    TCanvas* c1 = new TCanvas("c1", "c1", 1000, 1000, 1000, 700);
    h_pf_p->Draw("pe");
    h_L1_p->Draw("pe same");
    h_calo_p->Draw("pe same");
    leg_c_p->Draw();
    c1->Draw();
    c1->SaveAs("./PhiJetsRes.pdf");

    h_pf_e->Fit(fpf);
    h_calo_e->Fit(fcalo);
    h_L1_e->Fit(fl1);

    TCanvas* c2 = new TCanvas("c2", "c2", 1000, 1000, 1000, 700);
    h_pf_e->Draw("pe");
    h_L1_e->Draw("pe same");
    h_calo_e->Draw("pe same");
    leg_c_e->Draw();
    c2->Draw();
    c2->SaveAs("./EtaJetsRes.pdf");

    //h_pf_en->Fit(fpf);
    //h_calo_en->Fit(fcalo);
    //h_L1_en->Fit(fl1);

    TCanvas* c3 = new TCanvas("c3", "c3", 1000, 1000, 1000, 700);
    h_pf_en->Draw("pe");
    h_L1_en->Draw("pe same");
    h_calo_en->Draw("pe same");
    leg_en->Draw();
    c3->Draw();
    c3->SaveAs("./EnergyJetsRes.pdf");

    return 0;
}
