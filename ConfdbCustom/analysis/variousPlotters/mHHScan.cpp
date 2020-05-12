//c++ -o mhhscan mHHScan.cpp ../src/Event.cc ../src/TriggerMaker.cc `root-config --cflags --glibs`

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
    std::vector<int> colors = {1, 600, 616,  800};

    std::vector<TH1F*> histos;
    TLegend* leg = new TLegend(0.85, 0.817, 0.65, 0.62);
    leg->SetTextSize(0.04);

    time_t my_time = time(NULL); 
    printf("%s", ctime(&my_time)); 

    for(int j = 0; j < klambdas.size(); j++){

        std::cout << "kl: " << klambdas.at(j) << std::endl;
        double weight_sum =0;
        ev.SetStart(0);
        int entries = ev.GetEntries();

        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << klambdas.at(j);
        std::string legState = "k_{#lambda} = " + stream.str();
        histos.push_back(new TH1F(legState.c_str(), legState.c_str(), 50, 200., 1000));
        histos.at(j)->SetLineColor(colors.at(j));
        histos.at(j)->SetMarkerColor(colors.at(j));
        histos.at(j)->SetMarkerStyle(8);
        histos.at(j)->SetFillColor(colors.at(j));
        histos.at(j)->SetTitle("");
        histos.at(j)->GetXaxis()->SetTitle("mHH (Gev)");
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

        for(int i = 0; i < entries; i++){
            ev.Generate();
            ev.compute_weight(klambdas.at(j));
            weight_sum += ev.weight;
            auto Higgs =  ev.GetHiggsPair();
            histos.at(j)->Fill(Higgs.first.GetMHH(Higgs.second), ev.weight);

        }
        std::string y_title = "(1/N)#DeltaN / 16 GeV";
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
        c->GetYaxis()->SetRangeUser(0, max+0.02);
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
    c->Draw();
    c->SaveAs("mhh.pdf");

    my_time = time(NULL); 
    printf("%s", ctime(&my_time)); 

    return 0;
}