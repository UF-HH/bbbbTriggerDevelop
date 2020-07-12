//c++ -o tmp TurnOnSumB_KinBtag.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include "../interface/Obj.h"
#include "../interface/TriggerMaker.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TEfficiency.h"

double invMass(hltObj::Jet* j1, hltObj::Jet* j2, hltObj::Jet* j3, hltObj::Jet* j4 ){

    double theta1 = 2*atan(exp(-j1->eta));
    double theta2 = 2*atan(exp(-j2->eta));
    double theta3 = 2*atan(exp(-j3->eta));
    double theta4 = 2*atan(exp(-j4->eta));

    
    double p1 = j1->pt / sin(theta1);
    double p2 = j2->pt / sin(theta2);
    double p3 = j3->pt / sin(theta3);
    double p4 = j4->pt / sin(theta4);

    double e1  = j1->e;
    double e2  = j2->e; 
    double e3  = j3->e; 
    double e4  = j4->e; 
    
    double dpx_2 = pow(p1*sin(theta1)*sin(j1->phi) + p2*sin(theta2)*sin(j2->phi) + p3*sin(theta3)*sin(j3->phi) + p4*sin(theta4)*sin(j4->phi), 2);
    double dpy_2 = pow(p1*sin(theta1)*cos(j1->phi) + p2*sin(theta2)*cos(j2->phi) + p3*sin(theta3)*cos(j3->phi) + p4*sin(theta4)*cos(j4->phi), 2);
    double dpz_2 = pow(p1*cos(theta1) + p2*cos(theta2) + p3*cos(theta3) + p4*cos(theta4), 2);
    
    return sqrt(pow(e1 + e2 + e3 + e4, 2) -(dpx_2 + dpy_2 + dpz_2));

};

int main(){


    TFile* f = new TFile("SequentialForTurnOn.root");
    std::string branch = "SaveAllJetsMC/Jets";
    std::string genbranch = "SaveGenHH/Gen";

    Event ev(f, branch, genbranch);

    //retrieving infos from HLTAnalyzer which stores the online HLT decision
    std::vector<std::string> path_names_Kin = {"HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v1", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_OnlyKin"};
    std::vector<std::string> path_names_Btag = {"HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v1", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_OnlyBTag"};
    std::vector<std::string> variables = {"p_{T}^{4} Calo Offline (Gev)", "p_{T}^{2} Calo Offline (Gev)", "#sum^{i=2} btag^{i} Calo", "p_{T}^{4} PF Offline (Gev)", "p_{T}^{2} PF Offline (Gev)", "#sum^{i=2} btag^{i} PF" };
    std::vector<int> trigcount_Kin;
    std::vector<int> trigcount_Btag;
    std::vector<std::vector<int>> event_bits_Kin;
    std::vector<std::vector<int>> event_bits_Btag;
    trigcount_Kin.resize(path_names_Kin.size());
    trigcount_Btag.resize(path_names_Btag.size());

    TTree* tree = (TTree*)f->Get("MyHLTAnalyzer/trgObjTree");
    std::cout << "[INFO]: Reading branches..." << std::endl;
    for(int i = 0; i < path_names_Kin.size(); i++){
        std::cout << "[INFO]: New branch ... " << path_names_Kin.at(i) << std::endl;
        tree->SetBranchAddress(path_names_Kin.at(i).c_str(), &trigcount_Kin.at(i));
    }

    for(int i = 0; i < path_names_Btag.size(); i++){
        std::cout << "[INFO]: New branch ... " << path_names_Btag.at(i) << std::endl;
        tree->SetBranchAddress(path_names_Btag.at(i).c_str(), &trigcount_Btag.at(i));
    }

    for (int ev=0; ev < tree->GetEntries(); ev++){
        tree->GetEntry(ev);
        event_bits_Kin.push_back(trigcount_Kin);
        event_bits_Btag.push_back(trigcount_Btag);
    }

    std::string trig_name = "HLT_Quad30_Double60_Sum2LeadingBTag_1p3";
    
    TH1F* h_tot_kin = new TH1F("mHHTurnOn", "mHHTurnOn", 50, 0, 1200);
    TH1F* h_cut_kin = new TH1F("mHHTurnOn_cut", "mHHTurnOn_cut", 50, 0, 1200);

    TH1F* h_tot_btag = new TH1F("BtagTurnOn", "BtagTurnOn", 30, 0, 2);
    TH1F* h_cut_btag = new TH1F("BtagTurnOn_cut", "BtagTurnOn_cut", 30, 0, 2);

    h_tot_kin->SetMarkerStyle(8);
    h_tot_kin->SetMarkerSize(1);
    h_tot_kin->SetLineColor(kBlack);
    h_cut_kin->SetFillStyle(3003);
    h_cut_kin->SetFillColor(kAzure+2);

    h_tot_btag->SetMarkerStyle(8);
    h_tot_btag->SetMarkerSize(1);
    h_tot_btag->SetLineColor(kBlack);
    h_cut_btag->SetFillStyle(3003);
    h_cut_btag->SetFillColor(kAzure+2);

    TLegend* leg = new TLegend(.89, .89, .45, .7);
    leg->SetBorderSize(0);
    leg->AddEntry(h_tot_kin, "Tot Evts After L1 2018 Seed");
    leg->AddEntry(h_cut_kin, "Passed HLT_Quad30_Double60_Sum2LeadingBTag_1p3 Only Kin");

    TLegend* leg1 = new TLegend(.89, .89, .45, .7);
    leg1->SetBorderSize(0);
    leg1->AddEntry(h_tot_btag, "Tot Evts After L1 2018 Seed");
    leg1->AddEntry(h_cut_btag, "Passed HLT_Quad30_Double60_Sum2LeadingBTag_1p3 Only Btag");
    
    for(int ev_idx = 0; ev_idx < event_bits_Btag.size(); ev_idx++){

        std::vector<int> evb_k = event_bits_Kin.at(ev_idx);
        std::vector<int> evb_b = event_bits_Btag.at(ev_idx);
        //if did not pass L1 then skip

        if(evb_b[0] != 0){
            ev.Generate();
            ev.UnpackCollections();
            hltObj::Jets reco = ev.GetRecoJetsCollection();
            std::vector<hltObj::Jet*> recoj = ev.GetRecoJets();

            std::vector<hltObj::Jet*> candidate_mhh;

            std::vector<double> btags = reco.btag;

            for(int i = 0; i < 4; i++){
                int j = std::distance(btags.begin(), std::max_element(btags.begin(), btags.end()));
                candidate_mhh.push_back(recoj.at(j));
                btags.erase(btags.begin() + j);
                recoj.erase(recoj.begin() + j);
            }

            double mHH = invMass(candidate_mhh[0], candidate_mhh[1], candidate_mhh[2], candidate_mhh[3]);
            h_tot_kin->Fill(mHH);

            std::sort(reco.btag.begin(), reco.btag.end(), std::greater<double>());
            double BtagSum = reco.btag.at(0) + reco.btag.at(1); //saving thrid btag score offline objects
            h_tot_btag->Fill(BtagSum);

            int count = 0;
            for(int i = 1; i < evb_k.size(); i++){
                if(evb_k.at(i) == 0) break;
                count++;
            }

            if (count == evb_k.size()-1) {
                h_cut_kin->Fill(mHH);
            }

            count = 0;
            for(int i = 1; i < evb_b.size(); i++){
                if(evb_b.at(i) == 0) break;
                count++;
            }

            if (count == evb_b.size()-1) {
                h_cut_btag->Fill(BtagSum);
            }
            
        }

    }

    std::cout << "[INFO] Kinematic statistics..." << std::endl;
    std::cout << (double)h_tot_kin->GetEntries() << std::endl;
    std::cout << (double)h_cut_kin->GetEntries() << std::endl;
    std::cout << (double)h_cut_kin->GetEntries()/h_tot_kin->GetEntries() << std::endl;

    std::cout << "[INFO] Btag statistics..." << std::endl;
    std::cout << (double)h_tot_btag->GetEntries() << std::endl;
    std::cout << (double)h_cut_btag->GetEntries() << std::endl;
    std::cout << (double)h_cut_btag->GetEntries()/h_tot_btag->GetEntries() << std::endl;

    gStyle->SetOptStat(0);

    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 700);
    TEfficiency* eff_kin = new TEfficiency(*h_cut_kin, *h_tot_kin);
    eff_kin->SetFillStyle(3004);
    eff_kin->SetLineColor(kRed);

    leg->AddEntry(eff_kin, "Efficiency");

    h_tot_kin->SetTitle("");
    double max = h_tot_kin->GetMaximum();
    h_tot_kin->Scale(1./max);
    h_cut_kin->Scale(1./max);
    h_tot_kin->SetMaximum(1.6);
    h_tot_kin->GetXaxis()->SetTitle("Offline mHH (GeV)");
    h_tot_kin->GetXaxis()->SetTitleSize(.02);
    h_tot_kin->GetXaxis()->SetTitleOffset(2);
    h_tot_kin->GetYaxis()->SetTitle("a.u.");

    h_tot_kin->Draw("P");
    h_cut_kin->Draw("hist same");
    leg->Draw();
    eff_kin->Draw("P same");
    c->Draw();
    std::string canv_save_title = "plots/" + trig_name + "_mHH.pdf";
    c->SaveAs((canv_save_title).c_str());

    TFile* f_o = new TFile("weights/SumBTag1p3_mHHweights.root", "RECREATE");
    eff_kin->Write("mHH_weight");
    f_o->Write();
    f_o->Close();

    //Saving btag3 weights 

    TCanvas* c1 = new TCanvas("c", "c", 1000, 1000, 1000, 700);
    TEfficiency* eff_btag = new TEfficiency(*h_cut_btag, *h_tot_btag);
    eff_btag->SetFillStyle(3004);
    eff_btag->SetLineColor(kRed);

    leg1->AddEntry(eff_btag, "Efficiency");

    h_tot_btag->SetTitle("");
    max = h_tot_btag->GetMaximum();
    h_tot_btag->Scale(1./max);
    h_cut_btag->Scale(1./max);
    h_tot_btag->SetMaximum(1.6);
    h_tot_btag->GetXaxis()->SetTitle("Offline Leading 2 Btag Sum");
    h_tot_btag->GetXaxis()->SetTitleSize(.02);
    h_tot_btag->GetXaxis()->SetTitleOffset(2);
    h_tot_btag->GetYaxis()->SetTitle("a.u.");

    h_tot_btag->Draw("P");
    h_cut_btag->Draw("hist same");
    leg1->Draw();
    eff_btag->Draw("P same");
    c1->Draw();
    canv_save_title = "plots/" + trig_name + "_BtagSum.pdf";
    c1->SaveAs((canv_save_title).c_str());

    TFile* f_o_ = new TFile("weights/SumBTag1p3_BTagweights.root", "RECREATE");
    eff_btag->Write("BTagSum_weight");
    f_o_->Write();
    f_o_->Close();


    return 0;

}




