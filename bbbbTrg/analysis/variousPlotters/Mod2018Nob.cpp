//c++ -o mod2018nob Mod2018Nob.cpp ../src/TriggerMaker.cc ../src/Event.cc 
#include <iostream>
#include <string.h>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "../interface/Obj.h"
#include "../interface/TriggerMaker.h"
#include <iostream>
#include <fstream>
#include "time.h"
#include "TMultiGraph.h"
#include "TLatex.h"
#include "TLegend.h"

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

    /*
    typedef void(HLTCuts::*M)(double);
    std::map<std::string, M> MC;

    HLTCuts prova;
    std::string set = "setMinPt";
    
    for(auto p : prova.MappedSetCuts){
        if(p.first.compare(set))
            (prova.*p.second)(11.1);
    }
    */

    std::string kl_map    = "../data_weight/gg_HH_4B_SM_2018.root"; // sample map fname
    std::string kl_histo  = "hhGenLevelDistr";           // sample map histo
    std::string kl_coeffs = "../data_weight/coefficientsByBin_extended_3M_costHHSim_19-4.txt"; // coefficient file

    std::ofstream ofile;
    ofile.open ("Scan_HT_Eta.txt");

    TFile* f = new TFile("../../test/filter_tests_2018_nob.root");
    std::string branch = "checkTrg/trgObjTree";
    TriggerMaker Trig2018NoB;
    
    Event ev(f, branch);
    ev.WeightFactory(kl_map, kl_histo, kl_coeffs);
    
    //GRID SEARCH
    std::vector<double> MinPt40 = {36, 38, 40, 42, 44};
    std::vector<double> minetas = {-2.5, -2.4, -2.1};
    std::vector<double> maxetas = {2.5, 2.4, 2.1};
    std::vector<float> klambdas = {-1, 0, 1, 2, 3, 4, 5};

    //Saving in multi array
    std::vector<std::vector<float>> kresults_total;

    for(auto p40 : MinPt40){
        for(int j = 0; j < minetas.size(); j++){
        
            std::vector<float> kresults;
            for(auto kl : klambdas){

                std::cout << "kl: " << kl << std::endl;
                double weight_sum =0;
                int count = 0;  
                ev.SetStart(0);
                ofile << "lambda: " << kl << " Filter: hlt4PFCentralJetLooseID40 Pt4Min: " << p40 << " MinEta: " << minetas.at(j) << " MaxEta: " << maxetas.at(j);

                Trig2018NoB.ModifyCut("hlt4PFCentralJetLooseID40", "setPtMin", p40);
                Trig2018NoB.ModifyCut("hlt4PFCentralJetLooseID40", "setEtaMin", minetas.at(j));
                Trig2018NoB.ModifyCut("hlt4PFCentralJetLooseID40", "setEtaMax", maxetas.at(j));

                double counter = 0;
                //int entries = ev.GetEntries();
                int entries = 1000000;
                //std::vector<int> results(9,0);

                for(int i = 0; i < entries; i++){
                    ev.Generate();
                    ev.compute_weight(kl);
                    weight_sum += ev.weight;
                    auto r_1 = Trig2018NoB.Sequence(ev);
                    if(comp_stop(r_1)==-1){
                        counter+= ev.weight;
                        count++;
                    }
                    
                }

                kresults.push_back((double)counter/weight_sum);
                ofile << " Eff: " << (double)counter/weight_sum << "\n" ;
            }

            kresults_total.push_back(kresults);
        }
    }      

    //computing the baseline of 2018
    TriggerMaker BaseLine;
    Event ev_base(f, branch); //for comodity let's make the computations independent
    ev_base.WeightFactory(kl_map, kl_histo, kl_coeffs);
    std::vector<float> res_base;
    for(auto kl : klambdas){
        double counter = 0;  
        double weight_sum =0;
        ev_base.SetStart(0);
        int entries = 10000;
        for(int i = 0; i < entries; i++){
            ev_base.Generate();
            ev_base.compute_weight(kl);
            weight_sum += ev_base.weight;
            auto r_1 = BaseLine.Sequence(ev_base);
            if(comp_stop(r_1)==-1){
                counter+= ev_base.weight;
            }
            
        }

        res_base.push_back((double)counter/weight_sum);
    }

    TLegend* leg = new TLegend(0.85, 0.817, 0.65, 0.62);
    leg->SetTextSize(0.015);
    leg->SetBorderSize(0.);

    TGraph* base = new TGraph(res_base.size(), &(klambdas[0]), &(res_base[0]));
    base->SetLineColor(kRed);
    base->SetLineStyle(kDashed);
    base->SetLineWidth(2);
    TGraph *g[klambdas.size()*MinPt40.size()*minetas.size()];
    TMultiGraph *mg = new TMultiGraph();
    for(int i = 0; i < kresults_total.size(); i++){
        g[i] = new TGraph(kresults_total.at(i).size(), &(klambdas[0]), &(kresults_total.at(i)[0]));
        g[i]->GetYaxis()->SetRangeUser(0., 0.6);
        g[i]->SetLineColorAlpha(kAzure+7, 0.20);
        mg->Add(g[i]);
    }
    leg->AddEntry(g[0], "hlt4PFCentralJetLooseID40 modified", "l");
    leg->AddEntry(base, "HLTPFHT330PT30QuadPFJet75604540", "l");

    mg->Add(base);
    mg->SetTitle("");
    mg->GetYaxis()->SetRangeUser(0., 0.6);
    mg->GetXaxis()->SetTitle("k_{#lambda}");
    mg->GetYaxis()->SetTitle("Efficiency");
    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 700);
    mg->Draw("AC");
    TLatex T = TLatex();
    T.DrawLatexNDC(.67, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}");
    TLatex T1 = TLatex();
    T1.DrawLatexNDC(.1, .92, "#scale[1.]{ #font[62]{CMS} #font[52]{Preliminary} }");
    TLatex T2 = TLatex();
    T2.DrawLatexNDC(.66, .82, "#scale[0.35]{#font[42]{#lbar #eta #lbar < (2.5, 2.4, 2.1), P #in [30,44]}}");
    leg->Draw();
    c->Draw();
    c->Update();
    c->SaveAs("./prova_modcuts.pdf");



    ofile.close();

    return 0;
}