//c++ -o trgmatchscan TrgMatchDistributions.cpp ../src/Event.cc ../src/TriggerMaker.cc `root-config --cflags --glibs`

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

using namespace hltObj;

std::vector<hltObj::Jet> Match(std::vector<hltObj::Jet*> trgobj, std::vector<hltObj::Jet*> bjets){
    //scanning to delete not matched to bs
    std::vector<hltObj::Jet> result;
    for(int i = 0; i < bjets.size(); i++){
        if(bjets.at(i)->pt < 0)
            bjets.erase(bjets.begin()+i);
    }
    while(bjets.size() != 0 && trgobj.size() != 0){
        std::vector<double> dr;
        std::vector<int> b_ind;
        std::vector<int> trg_ind;
        for(int i = 0; i < bjets.size(); i++){
            for(int j = 0; j < trgobj.size(); j++){
                dr.push_back(sqrt(pow(bjets.at(i)->eta-trgobj.at(j)->eta,2)+pow(bjets.at(i)->phi-trgobj.at(j)->phi,2)));
                b_ind.push_back(i);
                trg_ind.push_back(j);
            }
        }

        int min_dr = std::min_element(dr.begin(),dr.end()) - dr.begin();
        if(dr.at(min_dr) <= 0.4){
            result.push_back(*trgobj.at(trg_ind.at(min_dr)));
            trgobj.erase(trgobj.begin() + trg_ind.at(min_dr));
            bjets.erase(bjets.begin() + b_ind.at(min_dr));
        }
        else{
            bjets.erase(bjets.begin() + b_ind.at(min_dr));
        }
    }

    return result;

}


int main(){

    std::string kl_map    = "../data_weight/gg_HH_4B_SM_2018.root"; // sample map fname
    std::string kl_histo  = "hhGenLevelDistr";           // sample map histo
    std::string kl_coeffs = "../data_weight/coefficientsByBin_extended_3M_costHHSim_19-4.txt"; // coefficient file

    TFile* f = new TFile("../../test/filter_tests_2018_nob.root");
    std::string branch = "checkTrg/trgObjTree";

    Event ev(f, branch);
    ev.WeightFactory(kl_map, kl_histo, kl_coeffs);

    std::vector<float> klambdas = {1, 2, 2.5, 3};

    std::vector<TH1F*> histos;
    TLegend* leg = new TLegend(.9, .9, .6, .7);
    gStyle->SetPalette(kRainBow);

    time_t my_time = time(NULL); 
    printf("%s", ctime(&my_time));

    for(int j = 0; j < klambdas.size(); j++){

        std::cout << "kl: " << klambdas.at(j) << std::endl;
        double weight_sum =0;
        ev.SetStart(0);
        int entries = ev.GetEntries();

        histos.push_back(new TH1F("h", "h", 100, 0, 200));
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << klambdas.at(j);
        std::string legState = "k_{#lambda} = " + stream.str();
        leg->AddEntry(histos.at(j), legState.c_str());
        histos.at(j)->SetTitle("Lowest P_{T} for trigger jet matched distributions");
        histos.at(j)->GetXaxis()->SetTitle("Lowest p_{T} [Gev]");
        histos.at(j)->GetYaxis()->SetTitle("Events/integral");

        if(j == 0){
            histos.at(j)->SetFillStyle(3003);
        }
        else{
            histos.at(j)->SetFillStyle(0);
        }

        int count_trg_match = 0;
        int count_reco_match = 0;
        int count_trg_match_2 = 0;

        for(int i = 0; i < entries; i++){
            ev.Generate();
            ev.UnpackCollections();
            ev.jetMatch(0.4, "RecoJets", "PFJets");
            //ev.trg_jet_match(0.25, "PFJets");
            //ev.Match(0.3, "bs", "RecoJets");
            
            //matching efficiency both for reco jets
            //and for trgobj
            std::vector<hltObj::Jet*> pf = ev.GetPFJets();
            std::vector<hltObj::Jet*> reco = ev.GetRecoJetsMatch();

            std::vector<hltObj::Jet> res = Match(pf, reco);
            if(pf.size() >= 4){
                if(std::count_if(reco.begin(), reco.end(), [](const hltObj::Jet* j){return j->matched == true;}) == 4){
                    count_reco_match++;
                    if(std::count_if(pf.begin(), pf.end(), [](const hltObj::Jet* j){return j->matched == true;}) >= 4){
                        count_trg_match++;
                    }
                    if(res.size() == 4){
                        count_trg_match_2++;
                    }
                }
            }
            

            //for plotting leading pt or lowest pt out 
            //of the matched trigger objects
            std::vector<double> pts;
            for(auto c : pf){
                if(c->matched){
                    pts.push_back(c->pt);
                }
            }

            ev.compute_weight(klambdas.at(j));
            weight_sum += ev.weight;

            std::sort(pts.begin(), pts.end());
            if(pts.size() == 4)
                histos.at(j)->Fill(pts.at(0), ev.weight);

        }
        histos.at(j)->Scale(1./histos.at(j)->Integral());
        std::cout << "Total events: " << entries << " / 4 recojets matched: " << count_reco_match << " / 4 trgObj matched: " << count_trg_match  << "/ 4 trgObj matched 2: " << count_trg_match_2 << std::endl;
        std::cout << "Rateo reco/tot: " << (double)count_reco_match/entries << std::endl;
        std::cout << "Rateo trg/tot: " << (double)count_trg_match/entries << std::endl;
        std::cout << "Rateo trg/reco: " << (double)count_trg_match/count_reco_match << std::endl;

    }

    /*
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
    for(auto h : histos){
        h->Draw("hist same PLC PFC");
    }
    leg->Draw();
    c->Draw();
    c->SaveAs("Lowest_PT_lambda.pdf");
    */

    my_time = time(NULL); 
    printf("%s", ctime(&my_time)); 

    return 0;
}