//c++ -o tmp TurnOn2018_Inclusive.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
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


    TFile* f = new TFile("SequentialOnlyReco.root");
    std::string branch = "SaveRecoJ/Jets";
    std::string genbranch = "SaveGenHH/Gen";

    TTree* jets_tree = (TTree*)f->Get(branch.c_str());
    std::vector<double>* reco_btag_ = 0;
    std::vector<double>* reco_pt_ = 0;
    std::vector<double>* reco_et_ = 0;
    std::vector<double>* reco_eta_ = 0;
    std::vector<double>* reco_phi_ = 0;
    std::vector<double>* reco_e_ = 0;
    std::vector<double>* reco_mass_ = 0;

    jets_tree->SetBranchAddress("reco_btag", &reco_btag_);
    jets_tree->SetBranchAddress("reco_pt", &reco_pt_);
    jets_tree->SetBranchAddress("reco_et", &reco_et_);
    jets_tree->SetBranchAddress("reco_eta", &reco_eta_);
    jets_tree->SetBranchAddress("reco_phi", &reco_phi_);
    jets_tree->SetBranchAddress("reco_e", &reco_e_);
    jets_tree->SetBranchAddress("reco_mass", &reco_mass_);

    //retrieving infos from HLTAnalyzer which stores the online HLT decision
    std::vector<std::string> path_names = {"HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3_v11"};
    //std::vector<std::string> path_names_Btag = {"HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3_v11"};
    std::vector<std::string> variables = {"p_{T}^{4} Calo Offline (Gev)", "p_{T}^{2} Calo Offline (Gev)", "#sum^{i=2} btag^{i} Calo", "p_{T}^{4} PF Offline (Gev)", "p_{T}^{2} PF Offline (Gev)", "#sum^{i=2} btag^{i} PF" };
    std::vector<int> trigcount_;
    std::vector<std::vector<int>> event_bits_;
    trigcount_.resize(path_names.size());

    TTree* tree = (TTree*)f->Get("MyHLTAnalyzer/trgObjTree");
    std::cout << "[INFO]: Reading branches..." << std::endl;
    for(int i = 0; i < path_names.size(); i++){
        std::cout << "[INFO]: New branch ... " << path_names.at(i) << std::endl;
        tree->SetBranchAddress(path_names.at(i).c_str(), &trigcount_.at(i));
    }

    for (int ev=0; ev < tree->GetEntries(); ev++){
        tree->GetEntry(ev);
        event_bits_.push_back(trigcount_);
    }

    std::string trig_name = "HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3";
    
    TH1F* h_tot_kin = new TH1F("mHHTurnOn", "mHHTurnOn", 150, 0, 1500);
    TH1F* h_cut_kin = new TH1F("mHHTurnOn_cut", "mHHTurnOn_cut", 150, 0, 1500);

    h_tot_kin->SetMarkerStyle(8);
    h_tot_kin->SetMarkerSize(1);
    h_tot_kin->SetLineColor(kBlack);
    h_cut_kin->SetFillStyle(3003);
    h_cut_kin->SetFillColor(kAzure+2);

    TLegend* leg = new TLegend(.89, .89, .45, .7);
    leg->SetBorderSize(0);
    leg->AddEntry(h_tot_kin, "Tot Events");
    leg->AddEntry(h_cut_kin, "Passed HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3");

    for(int ev_idx = 0; ev_idx < event_bits_.size(); ev_idx++){

        std::vector<int> evb_k = event_bits_.at(ev_idx);

        jets_tree->GetEntry(ev_idx);

        hltObj::Jets RecoJets;

        RecoJets.pt = *reco_pt_;
        RecoJets.mass = *reco_mass_;
        RecoJets.eta = *reco_eta_;
        RecoJets.phi = *reco_phi_;
        RecoJets.e = *reco_e_;
        RecoJets.et = *reco_et_;
        RecoJets.btag = *reco_btag_;

        std::vector<hltObj::Jet*> RecoJ;

        for(int i = 0; i < RecoJets.size(); i++){
            RecoJ.push_back(new hltObj::Jet(RecoJets.pt.at(i), RecoJets.eta.at(i), RecoJets.phi.at(i)));
            RecoJ.at(RecoJ.size()-1)->e = RecoJets.e.at(i); //last jet added, add info about energy
        }

        std::vector<hltObj::Jet*> candidate_mhh;

        std::vector<double> btags = *reco_btag_;

        for(int i = 0; i < 4; i++){
            int j = std::distance(btags.begin(), std::max_element(btags.begin(), btags.end()));
            candidate_mhh.push_back(RecoJ.at(j));
            btags.erase(btags.begin() + j);
            RecoJ.erase(RecoJ.begin() + j);
        }

        std::sort(reco_btag_->begin(), reco_btag_->end(), std::greater<double>());
        std::sort(reco_pt_->begin(), reco_pt_->end(), std::greater<double>());

        double mHH = invMass(candidate_mhh[0], candidate_mhh[1], candidate_mhh[2], candidate_mhh[3]);
        double btag3 = reco_btag_->at(2); //saving thrid btag score offline objects

        if(btag3 >= 0.27 && RecoJ.size() >= 4 && reco_pt_->at(3) > 40){
            h_tot_kin->Fill(mHH);

            int count = 0;
            for(int i = 0; i < evb_k.size(); i++){
                //std::cout << evb_k.at(i) <<std::endl;
                if(evb_k.at(i) == 0) break;
                count++;
            }
            //std::cout << "------------" << std::endl;

            if (count == evb_k.size()) {
                h_cut_kin->Fill(mHH);

            }
        }

    }

    std::cout << "[INFO] Kinematic statistics..." << std::endl;
    std::cout << (double)h_tot_kin->GetEntries() << std::endl;
    std::cout << (double)h_cut_kin->GetEntries() << std::endl;
    std::cout << (double)h_cut_kin->GetEntries()/h_tot_kin->GetEntries() << std::endl;

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
    std::string canv_save_title = "plots/InclusiveBtag3_" +  trig_name + "_mHH.pdf";
    c->SaveAs((canv_save_title).c_str());

    TFile* f_o = new TFile("weights/2018Trig_mHH_InclusiveBtag3_weights.root", "RECREATE");
    eff_kin->Write("mHH_weight");
    f_o->Write();
    f_o->Close();

    return 0;

}




