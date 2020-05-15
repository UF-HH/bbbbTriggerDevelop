//c++ -o tmp CheckOnMatch.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <numeric>
#include <valarray>
#include "../interface/Obj.h"
#include "../interface/TriggerMaker.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TEfficiency.h"

struct JetHReco{
    double invMass;
    std::pair<int, int>  index;
    std::pair<int, int> j_index;
};

struct B2Jet{
    hltObj::bQuark* b;
    int index;
};

double invMass(hltObj::bQuark* b1, hltObj::bQuark* b2){
    double b_mass = 4.7;
    double theta1 = 2*atan(exp(-b1->eta));
    double theta2 = 2*atan(exp(-b2->eta));
    
    double p1 = b1->pt / sin(theta1);
    double p2 = b2->pt / sin(theta2);

    double e1  = sqrt(p1*p1 + b_mass*b_mass);
    double e2  = sqrt(p2*p2 + b_mass*b_mass); 
    
    double dpx_2 = pow(p1*sin(theta1)*sin(b1->phi) + p2*sin(theta2)*sin(b2->phi), 2);
    double dpy_2 = pow(p1*sin(theta1)*cos(b1->phi) + p2*sin(theta2)*cos(b2->phi), 2);
    double dpz_2 = pow(p1*cos(theta1) + p2*cos(theta2), 2);
    
    return sqrt(pow(e1 + e2, 2) -(dpx_2 + dpy_2 + dpz_2));

};

double invMass(hltObj::Jet* j1, hltObj::Jet* j2){

    double theta1 = 2*atan(exp(-j1->eta));
    double theta2 = 2*atan(exp(-j2->eta));

    
    double p1 = j1->pt / sin(theta1);
    double p2 = j2->pt / sin(theta2);

    double e1  = j1->e;
    double e2  = j2->e; 
    
    double dpx_2 = pow(p1*sin(theta1)*sin(j1->phi) + p2*sin(theta2)*sin(j2->phi), 2);
    double dpy_2 = pow(p1*sin(theta1)*cos(j1->phi) + p2*sin(theta2)*cos(j2->phi), 2);
    double dpz_2 = pow(p1*cos(theta1) + p2*cos(theta2), 2);
    
    return sqrt(pow(e1 + e2, 2) -(dpx_2 + dpy_2 + dpz_2));

};


int main(){


    TFile* f = new TFile("../../../../../../CMSSW_11_0_0_patch1/src/ggHHRaw/ggHH/test/roots/ggMCRAW_GenJets.root");
    std::string branch = "SaveAllJets/Jets";
    std::string genbranch = "SaveGenHH/Gen";

    Event ev(f, branch, genbranch);

    //Cutting on PF Objects, save only positive energy
    hltObj::KinCuts PFCut;
    PFCut.Type = "PFJets";
    PFCut.MinPt = 0.0;
    PFCut.MinE = 0.0;

    ev.AddCut(PFCut);
    
    int entries = ev.GetEntries();

    TH1F* h_q = new TH1F("h_quark","h_quark",  20, 50, 200);
    h_q->SetLineColor(kBlue);
    h_q->SetLineStyle(2);
    
    TH1F* h_j = new TH1F("h_jet","h_jet",  20, 50, 200);
    h_j->SetLineColor(kMagenta);
    h_j->SetLineStyle(1);

    TH1F* h_pf = new TH1F("h_pf","h_pf",  20, 50, 200);
    h_pf->SetLineColor(kBlack);
    h_pf->SetLineStyle(1);


    for(int i = 0; i < entries; i++){

        ev.Generate();
        ev.UnpackCollections();
        ev.bMatch(0.4, "GenJets");
        ev.jetMatch(0.2, "PFJets", "BMatchedJets");

        std::vector<hltObj::Jet*> pf = ev.GetPFJets();
        std::vector<hltObj::bQuark> bs = ev.GetBMatches();
        std::vector<hltObj::Jet*> gen_bmatch = ev.GetMatchJetsToB();

        if(gen_bmatch.size() > 4){
            std::cerr << "non è possibile qvesto" << std::endl;
            throw std::runtime_error("nope");
        }
        
        if(gen_bmatch.size() == 4){
            if(std::count_if(gen_bmatch.begin(), gen_bmatch.end(), [](const hltObj::Jet* j){return j->matched == true;}) == gen_bmatch.size()){

                std::vector<B2Jet> bs_m;
                for(int ind = 0; ind < gen_bmatch.size(); ind++){
                    if(gen_bmatch.at(ind)->bmatched){
                        B2Jet J_{gen_bmatch.at(ind)->MatchedB, ind};
                        bs_m.push_back(J_);
                    }
                }

                std::vector<JetHReco> H_cand;
                double H_m = 125.;
                
                for(int i = 0; i < bs_m.size()-1; i++){
                    for(int j = i+1; j < bs_m.size(); j++){
                        JetHReco H{invMass(bs_m.at(i).b, bs_m.at(j).b), std::pair<int,int>(i,j), std::pair<int,int>(bs_m.at(i).index, bs_m.at(j).index) };
                        H_cand.push_back(H);
                    }
                }

                //finding pairs closest to higgs mass
                std::sort(H_cand.begin(), H_cand.end(), [=] (JetHReco x, JetHReco y){ return abs(x.invMass - H_m) < abs(y.invMass - H_m); });
                
                for(int m = 0; m < 2; m++){
                    h_q->Fill(H_cand.at(m).invMass);
                    h_j->Fill(invMass(gen_bmatch.at(H_cand.at(m).j_index.first), gen_bmatch.at(H_cand.at(m).j_index.second)));
                    //std::cout << invMass(gen_bmatch.at(H_cand.at(m).j_index.first)->MatchedObj, gen_bmatch.at(H_cand.at(m).j_index.second)->MatchedObj)<< std::endl;
                    h_pf->Fill(invMass(gen_bmatch.at(H_cand.at(m).j_index.first)->MatchedObj, gen_bmatch.at(H_cand.at(m).j_index.second)->MatchedObj));

                }

                //std::cout << "First: " << H_cand.at(0).invMass << " Q index: " << H_cand.at(0).index.first << " " << H_cand.at(0).index.second << " J index: " << H_cand.at(0).j_index.first << " " << H_cand.at(0).j_index.second << std::endl;
                //std::cout << "Second: " << H_cand.at(1).invMass << " Q index: " << H_cand.at(1).index.first << " " << H_cand.at(1).index.second << " J index: " << H_cand.at(1).j_index.first << " " << H_cand.at(1).j_index.second << std::endl;
            }
        }
        
    }

    gStyle->SetOptStat(0);

    h_j->GetXaxis()->SetTitle("#font[52]{m_{jj}}");
    h_j->GetYaxis()->SetTitle("#font[52]{Events}");
    h_j->SetTitle("");

    h_q->GetXaxis()->SetTitle("#font[52]{m_{jj}}");
    h_q->GetYaxis()->SetTitle("#font[52]{Events}");
    h_q->SetTitle("");

    TLegend* leg = new TLegend(.89, .89, .6, .7);
    leg->AddEntry(h_j, "#font[42]{GenJets}");
    leg->AddEntry(h_q, "#font[42]{bQuarks}");
    leg->AddEntry(h_pf, "#font[42]{PFJets}");
    leg->SetBorderSize(0);

    TLegend* leg1 = new TLegend(.89, .89, .7, .7);
    leg1->AddEntry(h_j, "#font[42]{GenJets}");
    leg1->AddEntry(h_pf, "#font[42]{PFJets}");
    leg1->SetBorderSize(0);

    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 1500);
    c->Divide(1,2);
    c->cd(1);
    h_q->Draw("hist");
    h_j->Draw("hist same");
    h_pf->Draw("hist same");
    leg->Draw();

    c->cd(2);
    h_j->Draw("hist");
    h_pf->Draw("hist same");
    leg1->Draw();

    c->cd();
    c->Draw();
    c->SaveAs("CheckMatch.png");


    return 0;
}