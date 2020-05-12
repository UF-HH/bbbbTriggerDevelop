//c++ -o match MatchPlots.cpp ../src/Event.cc ../src/TriggerMaker.cc `root-config --cflags --glibs`

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

using namespace hltObj;

int main(){

    std::string kl_map    = "../data_weight/gg_HH_4B_SM_2018.root"; // sample map fname
    std::string kl_histo  = "hhGenLevelDistr";           // sample map histo
    std::string kl_coeffs = "../data_weight/coefficientsByBin_extended_3M_costHHSim_19-4.txt"; // coefficient file

    TFile* f = new TFile("../../test/filter_tests_2018_nob.root");
    std::string branch = "checkTrg/trgObjTree";

    Event ev(f, branch);
    ev.WeightFactory(kl_map, kl_histo, kl_coeffs);

    std::vector<float> klambdas = {1, 2, 2.5, 3};


    time_t my_time = time(NULL); 
    printf("%s", ctime(&my_time));

    int entries = ev.GetEntries();
    //int entries = 100000;
    int count_recoj = 0;
    int count_trg = 0;

    //Cutting on Reco Objects
    hltObj::KinCuts RecoCut;
    RecoCut.Type = "RecoJets";
    RecoCut.MinPt = 20.0;
    RecoCut.MinEta = -2.5;
    RecoCut.MaxEta = 2.5;

    ev.AddCut(RecoCut);

    TH1F* h_reco = new TH1F("h_reco", "h_reco", 40, 20, 700);
    h_reco->SetMarkerSize(1);
    h_reco->SetMarkerStyle(8);
    h_reco->SetLineColor(kBlack);
    TH1F* h_pf = new TH1F("h_pf", "h_pf", 40, 20, 700);
    h_pf->SetLineColor(kBlue);
    h_pf->SetFillStyle(3003);
    h_pf->SetFillColor(kAzure+2);
    TH1F* h_dev = new TH1F("h_dev", "h_dev", 40, 20, 700);
    h_dev->SetLineColor(kBlack);
    h_dev->SetMarkerStyle(8);
    h_dev->SetMarkerSize(1);

    TLegend* leg = new TLegend(.89, .89, .7, .7);
    leg->SetBorderSize(0);
    leg->AddEntry(h_reco, "#font[42]{RecoJets}");
    leg->AddEntry(h_pf, "#font[42]{PFJets HLT}");

    TEfficiency* eff = new TEfficiency("eff", "eff", 80, 20, 700);
    eff->SetLineColor(kRed);
    eff->SetMarkerStyle(8);

    TEfficiency* eff2 = new TEfficiency("eff2", "eff2", 80, 20, 700);
    eff2->SetLineColor(kRed);
    eff2->SetMarkerStyle(8);


    for(int i = 0; i < entries; i++){
        ev.Generate();
        ev.UnpackCollections();
        //ev.jetMatch(0.4, "RecoJets", "PFJets");
        ev.bMatch(0.4, "RecoJets");
        ev.jetMatch(0.15, "RecoJetsMatched", "PFJets");
        //matching efficiency both for reco jets
        //and for trgobj
        //std::vector<hltObj::Jet*> recoj = ev.GetRecoJets();
        std::vector<hltObj::Jet*> pf = ev.GetPFJets();
        std::vector<hltObj::Jet*> reco = ev.GetRecoJetsMatch();
        std::vector<hltObj::bQuark*> bquarks = ev.GetBQuarks();
        std::vector<hltObj::Jet*> reco_unmatch = ev.GetRecoJets();

        if(reco.size() > 4){
            std::cerr << "non è possibile qvesto" << std::endl;
            throw std::runtime_error("nope");
        }
        if(std::count_if(pf.begin(), pf.end(), [](const hltObj::Jet* j){return j->matched == true;}) > 4){
            std::cerr << "non è possibile qvesto" << std::endl;
            throw std::runtime_error("nope");
        }

        if(reco.size() == 4){

            count_recoj++;
            
            for(auto c : reco){
                h_reco->Fill(c->pt);
            }

            for(auto c : pf){
                if(c->matched){
                    h_pf->Fill(c->pt);
                    eff->Fill(true, c->pt);
                    eff2->Fill(true, c->MatchedObj->pt);
                }
                else{
                    eff->Fill(false, c->pt);
                    eff2->Fill(false, c->pt);
                }
                
            }
            if(std::count_if(pf.begin(), pf.end(), [](const hltObj::Jet* j){return j->matched == true;}) == reco.size()){
                count_trg++;
            }
        }
    }

    for(int i = 1; i <= h_reco->GetNbinsX(); i++){
        double r = h_reco->GetBinContent(i);
        double g = h_pf->GetBinContent(i);
        h_dev->SetBinContent(i, (r-g)/r);
        std::cout << h_reco->GetBinContent(i) << std::endl;;
    }

    std::cout << entries << " " << count_recoj << " Rateo: " << (double)count_recoj/entries << std::endl;
    std::cout << "Count match pf-b, count match pf-reco, rateo: " << count_recoj << " " << count_trg << " " <<  (double)count_trg/count_recoj << std::endl;
    //std::cout << entries << " " << count_trg << " Rateo: " << (double)count_trg/entries << std::endl;
    std::cout << h_reco->GetEntries() << " " << h_pf->GetEntries() << " " << (double)h_pf->GetEntries()/h_reco->GetEntries() << std::endl;
    gStyle->SetOptStat(0);
    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 900);

    // Upper histogram plot is pad1
    TPad* pad1 = new TPad("pad1", "pad1", 0, 0.28, 1, 1);
    pad1->SetBottomMargin(0);  // joins upper and lower plot
    pad1->SetRightMargin(0.04);
    pad1->SetLeftMargin(0.13);
    pad1->Draw();
    // Lower ratio plot is pad2
    c->cd();  //returns to main canvas before defining pad2
    TPad* pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.28);
    pad2->SetTopMargin(0); // joins upper and lower plot
    pad2->SetRightMargin(0.04);
    pad2->SetLeftMargin(0.13);
    pad2->SetBottomMargin(0.22);
    pad2->Draw();

    c->Update();
    pad1->cd();

    gStyle->SetOptStat(0);

    h_reco->GetYaxis()->SetLabelSize(0.03);
    h_reco->GetYaxis()->SetTitle("#font[42]{Events}");
    //h_pf->GetYaxis()->SetTitleSize(10);
    //h_pf->GetYaxis()->SetTitleOffset(1);

    h_reco->Draw("PE");
    h_reco->SetTitle("#font[62]{P_{T} Matched RecoJ and PF TriggerObjects}");
    h_pf->Draw("hist same");
    leg->Draw("same");

    gPad->SetLogy();


    pad2->cd();

    h_dev->GetXaxis()->SetTitle("#font[42]{P_{T}^{PF} [GeV]}");
    h_dev->GetYaxis()->SetNdivisions(5, kTRUE);
    h_dev->GetXaxis()->SetLabelSize(0.08);
    h_dev->GetXaxis()->SetTitleOffset(1);
    h_dev->GetXaxis()->SetTitleSize(0.1);

    h_dev->GetYaxis()->SetTitleOffset(0.4);
    h_dev->GetYaxis()->SetTitle("#font[42]{(Reco-PF)/Reco}");
    h_dev->GetYaxis()->SetTitleSize(.08);
    h_dev->GetYaxis()->SetLabelSize(.07);
    h_dev->SetTitle("");
    h_dev->SetStats(0);
    h_dev->SetMaximum(1.2);
    h_dev->SetMinimum(-1.2);

    TLine* line = new TLine(20, 0, 700, 0);
    line->SetLineColor(kRed);
    line->SetLineStyle(kDotted);

    h_dev->Draw("pe");
    line->Draw("same");
    c->cd();
    c->Update();
    c->Draw();
    c->SaveAs("./prova.pdf");

    TCanvas* c2 = new TCanvas("c2", "c2", 1000, 1000, 1000, 700);
    eff->SetTitle("Match Efficiency of PFJets with RecoJets; P_{T}^{PF} [GeV]; Match Efficiency");
    eff->Draw();
    c2->Draw();
    c2->SaveAs("prova2.pdf");

    TCanvas* c3 = new TCanvas("c3", "c3", 1000, 1000, 1000, 700);
    eff->SetTitle("Match Efficiency of PFJets with RecoJets; P_{T}^{Offline} [GeV]; Match Efficiency");
    eff->Draw();
    c3->Draw();
    c3->SaveAs("prova3.pdf");

    my_time = time(NULL); 
    printf("%s", ctime(&my_time));
   
    return 0;
}
