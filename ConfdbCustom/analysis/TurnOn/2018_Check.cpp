//c++ -o tmp 2018_Check.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
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

bool PassedSkims(hltObj::Jets jets){

    bool pass = true;
    std::vector<double> results;
    double PtMin = 40;
    double PtMax = 1e99;
    double EtaMax = 2.4;
    double EtaMin = -2.4;
    double BtagMin = 0.27;
    int MinJets = 4;
    int MinBtaggedJets = 3;

    auto it = std::find_if(std::begin(jets.pt), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
    while (it != std::end(jets.pt)) {
        auto dis = std::distance(std::begin(jets.pt), it);
        if(jets.eta.at(dis) <= EtaMax && jets.eta.at(dis) >= EtaMin)
            results.emplace_back(jets.btag.at(dis));
        it = std::find_if(std::next(it), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
    }   
    if(results.size() < MinJets || std::count_if(results.begin(), results.end(), [BtagMin](double i){return i > BtagMin;}) < MinBtaggedJets){
        return false;
    }
    else{
        return pass;
    }

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
    std::vector<std::string> path_names_Kin = {"HLT_2018_OnlyKin"};
    std::vector<std::string> path_names_Btag = {"HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3_v11"};
    int trigbit_Kin;
    int trigbit_Btag;
    std::vector<int> event_bits_Kin;
    std::vector<int> event_bits_Btag;

    TTree* tree = (TTree*)f->Get("MyHLTAnalyzer/trgObjTree");
    std::cout << "[INFO]: Reading branches..." << std::endl;
    std::cout << "[INFO]: New branch ... " << path_names_Kin.at(0) << std::endl;
    tree->SetBranchAddress(path_names_Kin.at(0).c_str(), &trigbit_Kin);
    std::cout << "[INFO]: New branch ... " << path_names_Btag.at(0) << std::endl;
    tree->SetBranchAddress(path_names_Btag.at(0).c_str(), &trigbit_Btag);


    for (int ev=0; ev < tree->GetEntries(); ev++){
        tree->GetEntry(ev);
        event_bits_Kin.push_back(trigbit_Kin);
        event_bits_Btag.push_back(trigbit_Btag);
    }

    std::string trig_name = "HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3";

    TH1F* h = new TH1F("", "", 50, -5, 5);
    TH1F* h_ = new TH1F("", "", 50, -5, 5);

    h->GetXaxis()->SetTitle("Offline Jets #eta");
    h->GetYaxis()->SetTitle("Events");
    h->SetFillColor(kGray);
    h->SetLineColor(0);
    h_->SetMarkerStyle(8);
    h_->SetFillColor(0);
    h_->SetLineColor(kBlack);
    h_->SetMarkerColor(kBlack);

    TLegend* leg = new TLegend(.89, .89, .4, .7);
    leg->SetBorderSize(0);
    leg->AddEntry(h, "Events passing HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3", "F");
    leg->AddEntry(h_, "Reweighted by mHH and Btag3", "P");

    //opening TEff files
    TFile* f_mhh = new TFile("weights/2018Trig_mHHweights.root");
    TEfficiency* eff_mhh = (TEfficiency*)f_mhh->Get("mHH_weight");
    TFile* f_btag3 = new TFile("weights/2018Trig_BTag3weights.root");
    TEfficiency* eff_btag3 = (TEfficiency*)f_btag3->Get("BTag3_weight");

    //cycling on the events
    for(int ev_idx = 0; ev_idx < event_bits_Btag.size(); ev_idx++){
        
        //retrieving the trigger bits (this will be unidimensional vectors)
        int evb_k = event_bits_Kin.at(ev_idx);
        int evb_b = event_bits_Btag.at(ev_idx);

        //loading the offline objects for this events
        jets_tree->GetEntry(ev_idx);

        //constructing mHH
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

        //building histo 4 turn on curves
        if(PassedSkims(RecoJets)){
            
            //double lead_pt = reco_pt_->at(std::distance(reco_pt_->begin(), std::max_element(reco_pt_->begin(), reco_pt_->begin())));
            double mhh_w = eff_mhh->GetEfficiency(eff_mhh->FindFixBin(mHH));
            double btag3_w = eff_btag3->GetEfficiency(eff_btag3->FindFixBin(btag3));

            for (auto c : *reco_eta_){
                h_->Fill(c, mhh_w*btag3_w);
            }
            //h_->Fill(lead_pt, mhh_w*btag3_w);

            //if whole trigger passed then fill histo
            if(evb_b == 1){
                
                for (auto c : *reco_eta_){
                    h->Fill(c);
                }
                //h->Fill(lead_pt);

            }

        }
    }
    
    std::cout << "[INFO] Trigger passed integral... : " << h->Integral() << std::endl;
    std::cout << "[INFO] Weighted integral... : " << h_->Integral() << std::endl; 
    gStyle->SetOptStat(0);
    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 800);
    h->Draw("hist");
    h_->Draw("P same");
    leg->Draw();
    c->Draw();
    c->SaveAs("plots/2018_Weight_Eta.png");

    return 0;

}




