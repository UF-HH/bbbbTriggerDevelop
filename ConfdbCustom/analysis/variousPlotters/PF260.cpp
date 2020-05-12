//c++ -o pf260 PF260.cpp ../src/Event.cc ../src/TriggerMaker.cc `root-config --cflags --glibs`

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
#include "TLatex.h"
#include "TLine.h"

using namespace hltObj;

template<class T>
int comp_stop(std::vector<T> v){
    for(int i = 0; i < v.size(); i++){
        if(v.at(i) == 0){
            return i;
        }
    }

    return -1;
}

int main(){

    std::string kl_map    = "../data_weight/gg_HH_4B_SM_2018.root"; // sample map fname
    std::string kl_histo  = "hhGenLevelDistr";           // sample map histo
    std::string kl_coeffs = "../data_weight/coefficientsByBin_extended_3M_costHHSim_19-4.txt"; // coefficient file

    TFile* f = new TFile("../../test/filter_tests_2018_nob.root");
    std::string branch = "checkTrg/trgObjTree";

    Event ev(f, branch);
    ev.WeightFactory(kl_map, kl_histo, kl_coeffs);

    TriggerMaker Trig2018NoB; //deafult constructor = 2018 configuration

    std::vector<float> klambdas = {1, 2, 2.5, 3};
    std::vector<int> colors = {1, 600, 616,  800};

    std::vector<TH1F*> histos;
    TLegend* leg = new TLegend(0.85, 0.817, 0.65, 0.62);
    leg->SetTextSize(0.04);
    //gStyle->SetPalette(kCool+1);

    time_t my_time = time(NULL); 
    printf("%s", ctime(&my_time)); 

    //Cutting on PF Objects
    hltObj::KinCuts PFCut;
    PFCut.Type = "CaloJets";
    PFCut.MinPt = 20.0;
    PFCut.MinEta = -2.5;
    PFCut.MaxEta = 2.5;
    ev.AddCut(PFCut);

    hltObj::KinCuts RecoCut;
    RecoCut.Type = "RecoJets";
    RecoCut.MinPt = 20.0;
    RecoCut.MinEta = -2.5;
    RecoCut.MaxEta = 2.5;
    ev.AddCut(RecoCut);

    hltObj::KinCuts CaloCut;
    RecoCut.Type = "CaloJets";
    RecoCut.MinPt = 20.0;
    RecoCut.MinEta = -2.5;
    RecoCut.MaxEta = 2.5;
    ev.AddCut(CaloCut);

    for(int j = 0; j < klambdas.size(); j++){

        std::cout << "kl: " << klambdas.at(j) << std::endl;
        double weight_sum =0;
        ev.SetStart(0);
        int entries = ev.GetEntries();
        //int entries = 30000;

        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << klambdas.at(j);
        std::string legState = "k_{#lambda} = " + stream.str();
        histos.push_back(new TH1F(legState.c_str(), legState.c_str(), 60, 10., 610));
        histos.at(j)->SetLineColor(colors.at(j));
        histos.at(j)->SetMarkerColor(colors.at(j));
        histos.at(j)->SetMarkerStyle(8);
        histos.at(j)->SetFillColor(colors.at(j));
        histos.at(j)->SetTitle("");
        histos.at(j)->GetXaxis()->SetTitle("Online Calo Jets P_{T}^{2} (Gev)");
        if(j == 0){
            //SM configurations
            leg->AddEntry(histos.at(j), legState.c_str(), "p");
            histos.at(j)->SetFillColor(0);
        }
        else{
            histos.at(j)->SetFillStyle(0);
            leg->AddEntry(histos.at(j), legState.c_str(), "l");
            histos.at(j)->SetLineWidth(2);
        }
        leg->SetBorderSize(0.);

        std::vector<int> results;
        results.resize(9);

        for(int i = 0; i < entries; i++){
            ev.Generate();
            ev.UnpackCollections();
            ev.bMatch(0.4, "RecoJets");
            ev.jetMatch(0.2, "RecoJetsMatched", "PFJets"); 
            auto PFjets = ev.GetPFBJets();
            auto Matched = ev.GetMatches();
            ev.compute_weight(klambdas.at(j));
            weight_sum += ev.weight;
            auto r_1 = Trig2018NoB.Sequence(ev);
            int stop = Trig2018NoB.FindFirstZero(r_1);

            std::vector<double> calo_pts;
            for(auto c : PFjets){
                calo_pts.push_back(c->pt);
            }

            std::sort(calo_pts.begin(), calo_pts.end());
            if(stop > 4 || stop < 0){//second leading jet
                if(PFjets.size() > 1){
                    histos.at(j)->Fill(calo_pts.at(calo_pts.size()-2), ev.weight);
                }
            }
    
            int m = 0;
            auto it = results.begin();
            while(m != stop && it != results.end()){
                results.at(m)++;
                m++;
                it++;
            }
        }

        for(auto c : results){
            std::cout << (double)c/entries << std::endl;
        }

        std::cout << "--" << histos.at(j)->GetEntries() << "--" << std::endl;

        std::string y_title = "(1/N)#DeltaN / 10 GeV";
        histos.at(j)->GetYaxis()->SetTitle(y_title.c_str());
        histos.at(j)->Scale(1./histos.at(j)->Integral());

    }

    double max = 0;
    for(auto c : histos){
        double m = c->GetMaximum();
        if(m > max)
            max = m;
    }

    for(auto c : histos){
        c->GetYaxis()->SetRangeUser(0, max+0.01);

    }

    gStyle->SetOptStat(0);
    TCanvas*c = new TCanvas("c", "c", 1000, 1000, 1000, 700);
    for(int i = 0; i <histos.size(); i++){
        if(i == 0){
            histos.at(i)->Draw("pe");
        }
        else{
            histos.at(i)->Draw("hist same");
        }
    }
    leg->Draw();
    TLatex T = TLatex();
    T.DrawLatexNDC(.67, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}");
    TLatex T1 = TLatex();
    T1.DrawLatexNDC(.1, .92, "#scale[1.]{ #font[62]{CMS} #font[52]{Preliminary} }");
    TLatex T2 = TLatex();
    T2.DrawLatexNDC(.66, .84, "#scale[0.7]{#font[42]{#lbar #eta #lbar < 2.5}}");
    c->Draw();
    c->SaveAs("PF260Jets.pdf");



    //------------------------------
    //Plotting CDF 
    //------------------------------


    std::vector<TH1F*> histos_cdf;
    TLegend* leg_1 = new TLegend(0.85, 0.46, 0.65, 0.24);
    leg_1->SetBorderSize(0.);

    for(int j = 0; j < klambdas.size(); j++){

        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << klambdas.at(j);
        std::string legState = "k_{#lambda} = " + stream.str();
        histos_cdf.push_back(new TH1F((legState+std::to_string(j)).c_str(), (legState+std::to_string(j)).c_str(), histos.at(j)->GetNbinsX(), 10., 610));
        histos_cdf.at(j)->SetLineColor(colors.at(j));
        histos_cdf.at(j)->SetMarkerColor(colors.at(j));
        histos_cdf.at(j)->SetMarkerStyle(8);
        histos_cdf.at(j)->SetFillColor(colors.at(j));
        histos_cdf.at(j)->SetTitle("");
        histos_cdf.at(j)->GetXaxis()->SetTitle("Online Calo Jets P_{T}^{2} (Gev)");
        histos_cdf.at(j)->GetYaxis()->SetTitle("(1/N)Cumulative(#DeltaN / 10 GeV)");
        leg_1->AddEntry(histos.at(j), legState.c_str(), "p");
    }

    for(int i = 0; i < histos.size(); i++){
        for(int j = 1; j <= histos.at(i)->GetNbinsX(); j++){
            histos_cdf.at(i)->SetBinContent(j, histos.at(i)->Integral() - histos.at(i)->Integral(1., j)/histos.at(i)->Integral());
            histos_cdf.at(i)->GetYaxis()->SetRangeUser(0, 1.2);
        }
    }

    TLine* l = new TLine(10., 1, 610., 1 );
    l->SetLineStyle(kDashed);
    l->SetLineColorAlpha(kGray, 0.7);

    TLine* l1 = new TLine(60., 0.001, 60., 1.2 );
    l1->SetLineStyle(kDashed);
    l1->SetLineColorAlpha(kGray, 0.7);

    TLine* l2 = new TLine(10., 0.989, 610., 0.989 );
    l2->SetLineStyle(kDashed);
    l2->SetLineColorAlpha(kGray, 0.7);

    TLine* l3 = new TLine(10., 0.989, 610., 0.989 );
    l3->SetLineStyle(kDashed);
    l3->SetLineColorAlpha(kGray, 0.7);

    TLine* l4 = new TLine(10., 0.989, 610., 0.989 );
    l4->SetLineStyle(kDashed);
    l4->SetLineColorAlpha(kGray, 0.7);

    TLine* l5 = new TLine(10., 0.989, 610., 0.989 );
    l5->SetLineStyle(kDashed);
    l5->SetLineColorAlpha(kGray, 0.7);


    gStyle->SetOptStat(0);
    TCanvas*c1 = new TCanvas("c1", "c1", 1000, 1000, 1000, 700);
    for(int i = 0; i <histos_cdf.size(); i++){
        histos_cdf.at(i)->Draw("p same");
    }
    leg_1->SetTextSize(0.04);
    leg_1->Draw();
    T.DrawLatexNDC(.67, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}");
    T1.DrawLatexNDC(.1, .92, "#scale[1.]{ #font[62]{CMS} #font[52]{Preliminary} }");
    T2.DrawLatexNDC(.66, .48, "#scale[0.7]{#font[42]{#lbar #eta #lbar < 2.5}}");
    l->Draw("same");
    l1->Draw("same");
    l2->Draw("same");
    l3->Draw("same");
    l4->Draw("same");
    l5->Draw("same");
    c1->Draw();
    c1->SaveAs("./CDFPfPt260.pdf");


    my_time = time(NULL); 
    printf("%s", ctime(&my_time)); 

    return 0;
}