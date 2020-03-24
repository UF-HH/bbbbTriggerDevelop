#include <iostream>
#include <string.h>
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

    std::ofstream ofile;
    ofile.open ("Scan_HT_Eta.txt");

    TFile* f = new TFile("../../test/filter_tests_2018_nob.root");
    std::string branch = "checkTrg/trgObjTree";
    TriggerMaker Trig2018NoB;
    
    Event ev(f, branch);
    ev.WeightFactory(kl_map, kl_histo, kl_coeffs);
    
    //GRID SEARCH
    std::vector<double> MinPt75 = {70, 80};
    std::vector<double> MinPt60 = {55, 65};
    std::vector<double> MinPt45 = {40, 50};
    std::vector<double> MinPt40 = {35, 40};
    std::vector<double> hts = {320.0, 330.0, 340.0};
    std::vector<double> minetas = {-2.5, -2.4};
    std::vector<double> maxetas = {2.5, 2.4};
    std::vector<float> klambdas = {-1, 0, 1, 2, 3, 4, 5};

    //Saving in multi array
    std::vector<std::vector<float>> kresults_total;

    for(auto h : hts){
        for(auto p45 : MinPt45){
            for(auto p40 : MinPt40){
                for(auto p60 : MinPt60){
                    for(auto p75 : MinPt75){
                        for(int j = 0; j < minetas.size(); j++){
                        
                            std::vector<float> kresults;
                            for(auto kl : klambdas){

                                std::cout << "kl: " << kl << std::endl;
                                double weight_sum =0;
                                int count = 0;  
                                ev.SetStart(0);
                                ofile << "lambda: " << kl << " Filter: hltPFCentralJetsLooseIDQuad30HT330 MinHT: " << h << " MinEta: " << minetas.at(j) << " MaxEta: " << maxetas.at(j) << " Filter : hlt1PFCentralJetLooseID75 MinPt: " << p75 << " Filter 2_60 MinPt: " << p60 << " Filter 3_45 MinPt: " << p45; //<< " Filter 4_40 MinPt: " << p40;
                                Trig2018NoB.ModifyCut("hlt1PFCentralJetLooseID75", "setPtMin", p75);
                                Trig2018NoB.ModifyCut("hlt1PFCentralJetLooseID75", "setEtaMin", minetas.at(j));
                                Trig2018NoB.ModifyCut("hlt1PFCentralJetLooseID75", "setEtaMax", maxetas.at(j));

                                Trig2018NoB.ModifyCut("hlt2PFCentralJetLooseID60", "setPtMin", p60);
                                Trig2018NoB.ModifyCut("hlt2PFCentralJetLooseID60", "setEtaMin", minetas.at(j));
                                Trig2018NoB.ModifyCut("hlt2PFCentralJetLooseID60", "setEtaMax", maxetas.at(j));

                                Trig2018NoB.ModifyCut("hlt3PFCentralJetLooseID45", "setPtMin", p45);
                                Trig2018NoB.ModifyCut("hlt3PFCentralJetLooseID45", "setEtaMin", minetas.at(j));
                                Trig2018NoB.ModifyCut("hlt3PFCentralJetLooseID45", "setEtaMax", maxetas.at(j));

                                Trig2018NoB.ModifyCut("hlt4PFCentralJetLooseID40", "setPtMin", p40);
                                Trig2018NoB.ModifyCut("hlt4PFCentralJetLooseID40", "setEtaMin", minetas.at(j));
                                Trig2018NoB.ModifyCut("hlt4PFCentralJetLooseID40", "setEtaMax", maxetas.at(j));

                                Trig2018NoB.ModifyCut("hltPFCentralJetsLooseIDQuad30HT330", "setHTMin", h);
                                Trig2018NoB.ModifyCut("hltPFCentralJetsLooseIDQuad30HT330", "setEtaMin", minetas.at(j));
                                Trig2018NoB.ModifyCut("hltPFCentralJetsLooseIDQuad30HT330", "setEtaMax", maxetas.at(j));

                                double counter = 0;
                                //int entries = ev.GetEntries();
                                int entries = 1000;
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
                }
            }
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
        int entries = 1000;
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

    TGraph* base = new TGraph(res_base.size(), &(klambdas[0]), &(res_base[0]));
    base->SetLineColor(kRed);
    base->SetLineStyle(kDashed);
    base->SetLineWidth(2);
    TGraph *g[klambdas.size()*hts.size()];
    TMultiGraph *mg = new TMultiGraph();
    for(int i = 0; i < kresults_total.size(); i++){
        g[i] = new TGraph(kresults_total.at(i).size(), &(klambdas[0]), &(kresults_total.at(i)[0]));
        g[i]->SetLineColorAlpha(kAzure+7, 0.20);
        mg->Add(g[i]);
    }
    mg->Add(base);
    mg->SetTitle("2018 Trigger HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9 Efficiencies");
    mg->GetXaxis()->SetTitle("k_{#lambda}");
    mg->GetYaxis()->SetTitle("Efficiency");
    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 800);
    mg->Draw("AC");
    //base->Draw("C same");
    c->Draw();
    c->Update();
    c->SaveAs("./prova.pdf");



    ofile.close();

    return 0;
}