//c++ -o tmp TurnOnSumBmHH.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
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
    std::vector<std::string> path_names = {"HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v1", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v2", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v3", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v4", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v5","HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v6", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v7"};
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

    std::string trig_name = "HLT_Quad30_Double60_Sum2LeadingBTag_1p3";
    
    TH1F* h_tot = new TH1F("mHHTurnOn", "mHHTurnOn", 50, 0, 1200);
    TH1F* h_cut = new TH1F("mHHTurnOn_cut", "mHHTurnOn_cut", 50, 0, 1200);

    h_tot->SetMarkerStyle(2);
    h_tot->SetMarkerSize(1);
    h_tot->SetLineColor(0);
    h_cut->SetFillStyle(3003);
    h_cut->SetFillColor(kAzure+2);

    TLegend* leg = new TLegend(.89, .89, .45, .7);
    leg->SetBorderSize(0);
    leg->AddEntry(h_tot, "Tot Evts After L1 2018 Seed");
    leg->AddEntry(h_cut, "Passed HLT_Quad30_Double60_Sum2LeadingBTag_1p3");
    
    for(int ev_idx = 0; ev_idx < event_bits_.size(); ev_idx++){

        std::vector<int> evb = event_bits_.at(ev_idx);
        //if did not pass L1 then skip

        if(evb[0] != 0){
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
            h_tot->Fill(mHH);

            int count = 0;
            for(int i = 1; i < evb.size(); i++){
                if(evb.at(i) == 0) break;
                count++;
            }

            if (count == evb.size()-1) {
                h_cut->Fill(mHH);
            }
            
        }

    }


    std::cout << (double)h_cut->GetEntries()/h_tot->GetEntries() << std::endl;

    gStyle->SetOptStat(0);
    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 700);
    TEfficiency* eff = new TEfficiency(*h_cut, *h_tot);
    eff->SetFillStyle(3004);
    eff->SetLineColor(kRed);

    leg->AddEntry(eff, "Efficiency");

    double max = h_tot->GetMaximum();
    h_tot->Scale(1./max);
    h_cut->Scale(1./max);
    h_tot->SetMaximum(1.6);
    h_tot->GetXaxis()->SetTitle("mHH (GeV)");
    h_tot->GetXaxis()->SetTitleSize(.02);
    h_tot->GetXaxis()->SetTitleOffset(2);
    h_tot->GetYaxis()->SetTitle("a.u.");

    h_tot->Draw("P");
    h_cut->Draw("hist same");
    leg->Draw();
    eff->Draw("P same");
    c->Draw();
    std::string canv_save_title = "plots/" + trig_name + "_mHH.pdf";
    c->SaveAs((canv_save_title).c_str());


    return 0;

}




