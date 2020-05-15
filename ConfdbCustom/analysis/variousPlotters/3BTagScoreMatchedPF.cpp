//c++ -o tmp 3BTagScoreMatchedPF.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
#include <iostream>
#include <vector>
#include <string>
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

bool Double90(hltObj::Jets jets, double MinPt){

    std::vector<double> pts;
    for(int i = 0; i < jets.pt.size(); i++){
        if(abs(jets.eta.at(i)) <= 2.5){
            pts.push_back(jets.pt.at(i));
        }
    }

    std::sort(pts.begin(), pts.end());

    double sum = pts.at(pts.size()-1) + pts.at(pts.size()-2);

    return sum >= MinPt;
}

bool Quad40(hltObj::Jets jets, double MinPt){

    std::vector<double> pts;
    for(int i = 0; i < jets.pt.size(); i++){
        if(abs(jets.eta.at(i)) <= 2.5){
            pts.push_back(jets.pt.at(i));
        }
    }

    std::sort(pts.begin(), pts.end());

    double sum = pts.at(pts.size()-3) + pts.at(pts.size()-4);

    return sum >= MinPt;
}

bool Bisector(hltObj::Jets jets, double MinBTag){

    std::vector<double> btags;
    for(int i = 0; i < jets.pt.size(); i++){
        if(jets.pt.at(i) >= 30 && abs(jets.eta.at(i)) <= 2.5 ){
            btags.push_back(jets.btag.at(i));
        }
    }

    std::sort(btags.begin(), btags.end());


    return ( btags.at(btags.size()-2) + btags.at(btags.size()-3) ) >= MinBTag;
}

bool myTrigSeq(Event ev){
    hltObj::Jets pfj = ev.GetPFJetsCollection();
    hltObj::Jets caloj = ev.GetCaloJetsCollection();

    bool accept(Double90(caloj, 180) && Quad40(caloj, 90) && Bisector(caloj, 0.7) && Double90(pfj, 180) && Quad40(pfj, 90) && Bisector(pfj, 0.7));
    return accept;
}

int main(){


    TFile* f = new TFile("../../../../../../CMSSW_11_0_0_patch1/src/ggHHRaw/ggHH/test/GenJets_myTER.root");
    std::string branch = "SaveAllJets/Jets";
    std::string genbranch = "SaveGenHH/Gen";

    //retrieving infos from HLTAnalyzer which stores the online HLT decision
    std::vector<int> trigcount_;
    trigcount_.resize(2);
    TTree* tree = (TTree*)f->Get("MyHLTAnalyzer/trgObjTree");
    tree->SetBranchAddress("HLT_PFHT270_180_Double180_Double90_BisectorBTag07", &trigcount_.at(0));
    tree->SetBranchAddress("HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3", &trigcount_.at(1));

    std::vector<int> finalcounts_;
    finalcounts_.resize(2);

    for(int i = 0; i < tree->GetEntries(); i++){
        tree->GetEntry(i);
        for(int j = 0; j < trigcount_.size(); j++){
            if(trigcount_.at(j)) finalcounts_.at(j)++;
        }
    }

    //HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3
    //Building it as default and adding modules in defined positions
    std::vector<int> results;
    results.resize(10);
    TriggerMaker Trig2018; //deafult constructor = 2018 configuration
    Trig2018.SkipL1Seed = true; //skipping L1 seed 

    hltObj::HLTCuts hltBTagCaloDeepCSVp17Double;
    hltBTagCaloDeepCSVp17Double.MinN = 2;
    hltBTagCaloDeepCSVp17Double.EtaMin = -2.4;
    hltBTagCaloDeepCSVp17Double.EtaMax = 2.4;
    hltBTagCaloDeepCSVp17Double.PtMin = 30; 
    hltBTagCaloDeepCSVp17Double.BtagMin = 0.17; //0.17

    hltObj::HLTCuts hltBTagPFDeepCSV4p5Triple;
    hltBTagPFDeepCSV4p5Triple.MinN = 3;
    hltBTagPFDeepCSV4p5Triple.EtaMin = -2.6;
    hltBTagPFDeepCSV4p5Triple.EtaMax = 2.6;
    hltBTagPFDeepCSV4p5Triple.PtMin = 30; 
    hltBTagPFDeepCSV4p5Triple.BtagMin = 0.24; //0.24

    //sintax: Name of new module, Type of selector, required objects to compute, HLTCuts object, position in the chain of filters
    Trig2018.InsertCut("hltBTagCaloDeepCSVp17Double", "CustomFixedBtagSelector", "CaloJets", hltBTagCaloDeepCSVp17Double, 2);
    //last btag is in last position so we just push it into the module
    Trig2018.PushCut("hltBTagPFDeepCSV4p5Triple", "CustomFixedBtagSelector", "PFJets", hltBTagPFDeepCSV4p5Triple);

    Event ev(f, branch, genbranch);
    

    int entries = ev.GetEntries();

    int counter = 0;
    int counter1 = 0;
    int b_match_2018 = 0;
    int match_2018 = 0;
    int b_match_my = 0;
    int match_my = 0;

    TEfficiency* eff_my = new TEfficiency("eff_my", "eff_my", 120, 0, 320);
    eff_my->SetMarkerStyle(8);
    eff_my->SetMarkerColor(kMagenta);
    eff_my->SetLineColor(kMagenta);
    eff_my->SetFillColor(0);

    TEfficiency* eff_2018 = new TEfficiency("eff_2018", "eff_2018", 120, 0, 320);
    eff_2018->SetMarkerStyle(8);
    eff_2018->SetMarkerColor(kAzure);
    eff_2018->SetLineColor(kAzure);
    eff_2018->SetFillColor(0);

    TH1F* h_3b = new TH1F("h_3b", "h_3b", 40, -1, 1);
    h_3b->GetXaxis()->SetTitle("3rd BTag Score");
    h_3b->GetYaxis()->SetTitle("Events / 0.05");
    h_3b->SetTitle("");
    h_3b->SetLineColor(kMagenta);
    TH1F* h_3b_ = new TH1F("h_3b_", "h_3b_", 40, -1, 1);
    h_3b_->GetXaxis()->SetTitle("3rd BTag Score");
    h_3b_->GetYaxis()->SetTitle("Events / 0.05");
    h_3b_->SetLineColor(kBlue);
    h_3b_->SetTitle("");



    for(int i = 0; i < entries; i++){
    //for(int i = 0; i < 1000; i++){

        ev.Generate();
        ev.UnpackCollections();
        ev.bMatch(0.4, "GenJets");
        ev.jetMatch(0.15, "BMatchedJets", "PFJets");

        bool four_b_match = false;
        bool four_jets_matched = false;

        std::vector<hltObj::Jet*> pf = ev.GetPFJets();
        std::vector<hltObj::Jet*> gen_bmatch = ev.GetMatchJetsToB();
        std::vector<hltObj::bQuark*> bs = ev.GetBQuarks();
        hltObj::Jets genj = ev.GetGenJetsCollection();
        hltObj::Jets pfj = ev.GetPFJetsCollection();

        for (auto c: gen_bmatch){
            std::cout << c->pt << " ";
        }
        std::cout << std::endl;


        if(gen_bmatch.size() > 4){
            std::cerr << "non è possibile qvesto" << std::endl;
            throw std::runtime_error("nope");
        }


        if(gen_bmatch.size() == 4){
            four_b_match = true;

            if(std::count_if(pf.begin(), pf.end(), [](const hltObj::Jet* j){return j->matched == true;}) == gen_bmatch.size()){
                four_jets_matched = true;
            }
        }

        if(myTrigSeq(ev)){
            counter++;
            if(ev.isResolved(0.4)){
                if(four_b_match) b_match_my++;
                if(four_jets_matched) match_my++;

                if(four_b_match && four_jets_matched){
                    std::vector<double> btags;
                    for(auto c : pf){
                        if(c->matched){
                            eff_my->Fill(true, c->pt);
                            btags.push_back(c->btag);
                        }
                        else{
                            eff_my->Fill(false, c->pt);
                        }
                    }
                    std::sort(btags.begin(), btags.end());
                    h_3b->Fill(btags.at(btags.size()-3));
                } 
            }
        }

        auto r_1 = Trig2018.Sequence(ev);
        int stop = Trig2018.FindFirstZero(r_1);

        if(stop == -1){
            counter1++;
            if(ev.isResolved(0.4)){
                if(four_b_match) b_match_2018++;
                if(four_jets_matched) match_2018++;

                if(four_b_match && four_jets_matched){
                    std::vector<double> btags;
                    for(auto c : pf){
                        if(c->matched){
                            eff_2018->Fill(true, c->pt);
                            btags.push_back(c->btag);
                        }
                        else{
                            eff_2018->Fill(false, c->pt);
                        }
                    }
                    std::sort(btags.begin(), btags.end());
                    h_3b_->Fill(btags.at(btags.size()-3));
                } 
            }
        }

        
    }

    std::cout << "...ONLINE RESULTS:..." << std::endl;
    std::cout << "....Events Processed: " << tree->GetEntries() << std::endl;
    std::cout << "....HLT_PFHT270_180_Double180_Double90_BisectorBTag07: " << finalcounts_.at(0) << std::endl;
    std::cout << "....HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3: " << finalcounts_.at(1) << std::endl;

    std::cout << "...OFFLINE RESULTS... "<< std::endl;
    std::cout << "....HLT_PFHT270_180_Double180_Double90_BisectorBTag07: "<< counter << " 4b matched to Gen: " << b_match_my << " " << " 4 Jets matched to bMatchedJets: " << match_my << std::endl;
    std::cout << "....HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3: "<< counter1 << " " << " 4b matched to Gen: " << b_match_2018 <<  " " << " 4 Jets matched to bMatchedJets: " << match_2018 << std::endl;

    TLegend* leg = new TLegend(.89, .11, .6, .4);
    leg->SetBorderSize(0);
    leg->AddEntry(eff_2018, "#font[42]{2018 Trigger}");
    leg->AddEntry(eff_my, "#font[42]{New Trigger}");

    TLegend* leg1 = new TLegend(.11, .89, .4, .6);
    leg1->SetBorderSize(0);
    leg1->AddEntry(h_3b_, "#font[42]{2018 Trigger}");
    leg1->AddEntry(h_3b, "#font[42]{New Trigger}");

    gStyle->SetOptStat(0);

    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 700);
    h_3b->Draw("hist");
    h_3b_->Draw("hist same");
    leg1->Draw();
    c->Draw();
    c->SaveAs("3BatgMatchedPF.pdf");



    TCanvas* c2 = new TCanvas("c2", "c2", 1000, 1000, 1000, 700);
    eff_my->SetTitle(";GenJet p_{T} (Gev);Events / 3 GeV");
    eff_my->Draw("AP");
    eff_2018->Draw("P same");
    leg->Draw();
    TLatex T = TLatex();
    T.DrawLatexNDC(.67, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}");
    c2->Draw();
    c2->SaveAs("MatchEffMytrig.pdf");

    return 0;
            
}
