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


    TFile* f = new TFile("../../../../../../CMSSW_11_0_0_patch1/src/ggHHRaw/ggHH/test/roots/ggHLT_RAWAOD_GenRECO.root");
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

    TH1F* h_q = new TH1F("h_quark","h_quark",  50, 50, 200);
    h_q->SetLineColor(kBlue);
    h_q->SetLineStyle(2);
    
    TH1F* h_gen = new TH1F("h_gen","h_gen",  50, 50, 200);
    h_gen->SetLineColor(kBlack);
    h_gen->SetLineStyle(2);
    h_gen->SetMarkerStyle(8);
    h_gen->SetMarkerColor(kBlack);


    TH1F* h_pf = new TH1F("h_pf","h_pf",  50, 50, 200);
    h_pf->SetLineColor(kBlue);
    h_pf->SetLineStyle(1);

    TH1F* h_calo = new TH1F("h_calo","h_calo",  50, 50, 200);
    h_calo->SetLineColor(kOrange);
    h_calo->SetLineStyle(1);

    TH1F* h_reco = new TH1F("h_reco","h_reco",  50, 50, 200);
    h_reco->SetLineColor(kMagenta);
    h_reco->SetLineStyle(1);

    TH1F* h_gen_nn = new TH1F("h_gen_nn","h_gen_nn",  50, 50, 200);
    h_gen_nn->SetLineColor(kBlack);
    h_gen_nn->SetLineStyle(2);
    h_gen_nn->SetMarkerStyle(8);
    h_gen_nn->SetMarkerColor(kBlack);


    TH1F* h_pf_nn = new TH1F("h_pf_nn","h_pf_nn",  50, 50, 200);
    h_pf_nn->SetLineColor(kBlue);
    h_pf_nn->SetLineStyle(1);

    TH1F* h_calo_nn = new TH1F("h_calo_nn","h_calo_nn",  50, 50, 200);
    h_calo_nn->SetLineColor(kOrange);
    h_calo_nn->SetLineStyle(1);

    TH1F* h_reco_nn = new TH1F("h_reco_nn","h_reco_nn",  50, 50, 200);
    h_reco_nn->SetLineColor(kMagenta);
    h_reco_nn->SetLineStyle(1);


    //Matching GenJets
    for(int i = 0; i < entries; i++){
    //for(int i = 0; i < 100; i++){

        //PFJets

        ev.Generate();
        ev.UnpackCollections();
        ev.bMatch(0.4, "GenJets");
        ev.jetMatch(0.1, "PFJets", "BMatchedJets");

        std::vector<hltObj::bQuark> bs = ev.GetBMatches(); //same for everyone

        std::vector<hltObj::Jet*> gen_bmatch = ev.GetMatchJetsToB(); //specific for pairing
        std::vector<hltObj::Jet*> pf = ev.GetPFJets(); //specific for pairing gen/PF

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
                    h_gen->Fill(invMass(gen_bmatch.at(H_cand.at(m).j_index.first), gen_bmatch.at(H_cand.at(m).j_index.second)));
                    h_pf->Fill(invMass(gen_bmatch.at(H_cand.at(m).j_index.first)->MatchedObj, gen_bmatch.at(H_cand.at(m).j_index.second)->MatchedObj));

                }

            }
        }

        ev.jetMatch(0.1, "CaloJets", "BMatchedJets");

        gen_bmatch = ev.GetMatchJetsToB(); //specific for pairing
        std::vector<hltObj::Jet*> calo = ev.GetCaloJets(); //specific for pairing gen/calo

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
                    h_calo->Fill(invMass(gen_bmatch.at(H_cand.at(m).j_index.first)->MatchedObj, gen_bmatch.at(H_cand.at(m).j_index.second)->MatchedObj));

                }

            }
        }

        ev.jetMatch(0.1, "RecoJets", "BMatchedJets");

        gen_bmatch = ev.GetMatchJetsToB(); //specific for pairing
        std::vector<hltObj::Jet*> reco = ev.GetRecoJets(); //specific for pairing gen/calo

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
                    h_reco->Fill(invMass(gen_bmatch.at(H_cand.at(m).j_index.first)->MatchedObj, gen_bmatch.at(H_cand.at(m).j_index.second)->MatchedObj));

                }

            }
        }
        
    }

    ev.SetStart(0); //Reading again the whole tree

    //Matching GenJets
    for(int i = 0; i < entries; i++){
    //for(int i = 0; i < 100; i++){

        //PFJets

        ev.Generate();
        ev.UnpackCollections();
        ev.bMatch(0.4, "GenNNJets");
        ev.jetMatch(0.1, "PFJets", "BMatchedJets");

        std::vector<hltObj::bQuark> bs = ev.GetBMatches(); //same for everyone

        std::vector<hltObj::Jet*> gen_bmatch = ev.GetMatchJetsToB(); //specific for pairing
        std::vector<hltObj::Jet*> pf = ev.GetPFJets(); //specific for pairing gen/PF

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
                    //h_q->Fill(H_cand.at(m).invMass);
                    h_gen_nn->Fill(invMass(gen_bmatch.at(H_cand.at(m).j_index.first), gen_bmatch.at(H_cand.at(m).j_index.second)));
                    h_pf_nn->Fill(invMass(gen_bmatch.at(H_cand.at(m).j_index.first)->MatchedObj, gen_bmatch.at(H_cand.at(m).j_index.second)->MatchedObj));

                }

            }
        }

        ev.jetMatch(0.1, "CaloJets", "BMatchedJets");

        gen_bmatch = ev.GetMatchJetsToB(); //specific for pairing
        std::vector<hltObj::Jet*> calo = ev.GetCaloJets(); //specific for pairing gen/calo

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
                    h_calo_nn->Fill(invMass(gen_bmatch.at(H_cand.at(m).j_index.first)->MatchedObj, gen_bmatch.at(H_cand.at(m).j_index.second)->MatchedObj));

                }

            }
        }

        ev.jetMatch(0.1, "RecoJets", "BMatchedJets");

        gen_bmatch = ev.GetMatchJetsToB(); //specific for pairing
        std::vector<hltObj::Jet*> reco = ev.GetRecoJets(); //specific for pairing gen/calo

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
                    h_reco_nn->Fill(invMass(gen_bmatch.at(H_cand.at(m).j_index.first)->MatchedObj, gen_bmatch.at(H_cand.at(m).j_index.second)->MatchedObj));

                }

            }
        }
        
    }


    gStyle->SetOptStat(0);

    h_gen->GetXaxis()->SetTitle("#font[52]{m_{jj}}");
    h_gen->GetYaxis()->SetTitle("#font[52]{Events}");
    h_gen->SetTitle("");

    h_gen_nn->GetXaxis()->SetTitle("#font[52]{m_{jj}}");
    //h_gen_nn->GetYaxis()->SetTitle("#font[52]{Events}");
    h_gen_nn->SetTitle("");

    h_pf->GetXaxis()->SetTitle("#font[52]{m_{jj}}");
    h_pf->GetYaxis()->SetTitle("#font[52]{Events}");
    h_pf->SetTitle("");

    h_calo->GetXaxis()->SetTitle("#font[52]{m_{jj}}");
    h_calo->GetYaxis()->SetTitle("#font[52]{Events}");
    h_calo->SetTitle("");

    h_reco->GetXaxis()->SetTitle("#font[52]{m_{jj}}");
    h_reco->GetYaxis()->SetTitle("#font[52]{Events}");
    h_reco->SetTitle("");

    h_q->GetXaxis()->SetTitle("#font[52]{m_{jj}}");
    h_q->GetYaxis()->SetTitle("#font[52]{Events}");
    h_q->SetTitle("");

    TLegend* leg = new TLegend(.89, .89, .6, .7);
    leg->AddEntry(h_gen, "#font[42]{GenJets}", "P");
    leg->AddEntry(h_pf, "#font[42]{PFJets}");
    leg->AddEntry(h_calo, "#font[42]{CaloJets}");
    leg->AddEntry(h_reco, "#font[42]{RecoJets}");
    leg->SetBorderSize(0);

    TLegend* leg1 = new TLegend(.89, .89, .6, .7);
    leg1->AddEntry(h_gen_nn, "#font[42]{GenNNJets}", "P");
    leg1->AddEntry(h_pf_nn, "#font[42]{PFJets}");
    leg1->AddEntry(h_calo_nn, "#font[42]{CaloJets}");
    leg1->AddEntry(h_reco_nn, "#font[42]{RecoJets}");
    leg1->SetBorderSize(0);

    double max = std::max(h_gen->GetMaximum(), h_gen_nn->GetMaximum());
    h_gen->SetAxisRange(0., max+200,"Y");
    h_gen_nn->SetAxisRange(0., max+200,"Y");

    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 2400, 1400);
    c->Divide(2,1);
    c->cd(1);
    h_gen->Draw("hist");
    h_pf->Draw("hist same");
    h_calo->Draw("hist same");
    h_reco->Draw("hist same");
    TLatex T1 = TLatex();
    T1.DrawLatexNDC(.57, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}");
    leg->Draw();

    c->cd(2);
    h_gen_nn->Draw("hist");
    h_pf_nn->Draw("hist same");
    h_calo_nn->Draw("hist same");
    h_reco_nn->Draw("hist same");
    TLatex T = TLatex();
    T.DrawLatexNDC(.57, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}");
    leg1->Draw();

    c->cd();
    c->Draw();
    c->SaveAs("CheckMatch.png");


    return 0;
}