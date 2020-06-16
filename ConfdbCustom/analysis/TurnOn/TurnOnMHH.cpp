//c++ -o tmp TurnOnMHH.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
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

int main(){


    TFile* f = new TFile("SequentialForTurnOn.root");
    std::string branch = "SaveAllJetsMC/Jets";
    std::string genbranch = "SaveGenHH/Gen";

    Event ev(f, branch, genbranch);

    //retrieving infos from HLTAnalyzer which stores the online HLT decision
    std::vector<std::string> path_names = {"HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v1", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v2", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v3", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v4", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v5","HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v6", "HLT_Quad30_Double60_Sum2LeadingBTag_1p5_v7"};
    std::vector<std::string> variables = {"p_{T}^{4} Calo Offline (Gev)", "p_{T}^{2} Calo Offline (Gev)", "#sum_{i=1}^{2} L_{btag}^{i} Calo", "p_{T}^{4} PF Offline (Gev)", "p_{T}^{2} PF Offline (Gev)", "#sum_{i=1}^{2} L_{btag}^{i} PF" };
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
    std::vector<std::string> filters_names = {"hltQuadCentralJet30", "hltDoubleCentralJet60", "hltCaloSum2LeadingBTag", "hltPFCentralJetLooseIDQuad30", "hltPFCentralJetLooseIDQuad60", "hltPFSum2LeadingBTag"};
    std::vector<std::vector<double>> ranges = {{0,200}, {0,400}, {0,2}, {0,200}, {0,400}, {0,2}};
    int bins = 50;
    std::vector<TH1F*> h_tot;
    std::vector<TH1F*> h_cut;
    std::vector<TCanvas*> c;
    std::vector<TEfficiency*> eff;
    std::vector<TLegend*> leg;

    for(int fil_id = 0; fil_id < filters_names.size(); fil_id++){
            
        std::string fil_name = filters_names.at(fil_id);

        std::string h_tot_title = "tot_"+trig_name+"_"+fil_name;
        std::string h_cut_title = "cut_"+trig_name+"_"+fil_name;
        std::string eff_title = "cut_"+trig_name+"_"+fil_name;
        std::string leg_title_before = "Before "+fil_name;
        std::string leg_title_after = "After "+fil_name;
        h_tot.push_back(new TH1F(h_tot_title.c_str(), h_tot_title.c_str(), bins, ranges.at(fil_id).at(0), ranges.at(fil_id).at(1)));

        h_tot[fil_id]->SetMarkerStyle(2);
        h_tot[fil_id]->SetMarkerSize(1);
        h_cut.push_back(new TH1F(h_cut_title.c_str(), h_cut_title.c_str(),bins, ranges.at(fil_id).at(0), ranges.at(fil_id).at(1)));
        h_cut.at(fil_id)->SetFillStyle(3003);
        h_cut.at(fil_id)->SetFillColor(kAzure+2);

        eff.push_back(new TEfficiency(eff_title.c_str(),eff_title.c_str(), bins, ranges.at(fil_id).at(0), ranges.at(fil_id).at(1)));
        eff[fil_id]->SetFillStyle(3004);
        eff[fil_id]->SetLineColor(kRed);

        leg.push_back(new TLegend(.89, .89, .45, .7));
        leg[fil_id]->SetBorderSize(0);
        leg[fil_id]->AddEntry(h_tot[fil_id], leg_title_before.c_str());
        leg[fil_id]->AddEntry(h_cut[fil_id], leg_title_after.c_str());
        leg[fil_id]->AddEntry(eff[fil_id], "Efficiency");

        c.push_back(new TCanvas(("canv_"+fil_name).c_str(), ("canv_"+fil_name).c_str(), 1000, 1000, 1000, 700));

    }

    
    for(int ev_idx = 0; ev_idx < event_bits_.size(); ev_idx++){

        std::vector<int> evb = event_bits_.at(ev_idx);
        //if did not pass L1 then skip
        if(evb[0] != 0){
            ev.Generate();
            ev.UnpackCollections();
            hltObj::Jets reco = ev.GetRecoJetsCollection();

            std::vector<double> btags = reco.btag;
            std::vector<double> pt = reco.pt;
            std::sort(btags.begin(), btags.end(), std::greater<double>());
            std::sort(pt.begin(), pt.end(), std::greater<double>());
            std::vector<double> vars = {pt[3], pt[1], btags[0]+btags[1], pt[3], pt[1],  btags[0]+btags[1]};

            int count = 0;
            for(int i = 1; i < evb.size(); i++){
                if(evb.at(i) == 0) break;
                count++;
            }

            for(int i =0; i < count; i++){
                h_tot[i]->Fill(vars[i]);
                h_cut[i]->Fill(vars[i]);
                eff[i]->Fill(true, vars[i]);
            }

            if(count < evb.size()-1){
                h_tot[count]->Fill(vars[count])
            }

            for(int j = count; j < evb.size()-1; j++)
                eff[j]->Fill(false, vars[j]);
            
        }

    }


    gStyle->SetOptStat(0);

    for(int i = 0; i < filters_names.size(); i++){  
        
        std::string fil_name = filters_names.at(i);
        TCanvas* c = new TCanvas(("canv_"+fil_name).c_str(), ("canv_"+fil_name).c_str(), 1000, 1000, 1000, 700));
        double max = h_tot[i]->GetMaximum();
        h_tot[i]->Scale(1./max);
        std::string var = variables[i];
        
        h_tot[i]->GetXaxis()->SetTitle((var).c_str());
        h_tot[i]->GetYaxis()->SetTitle("Events");

        std::string fil_name = filters_names.at(i);
        std::string h_tot_title = fil_name;

        h_tot[i]->SetTitle(h_tot_title.c_str());
        
        h_cut[i]->Scale(1./max);
        h_tot[i]->SetMaximum(1.6);
        
        h_tot[i]->Draw("P");
        h_cut[i]->Draw("hist same");
        leg[i]->Draw();
        eff[i]->Draw("P same");
        //c[i]->SetLogy();
        c->Draw();
        std::string canv_save_title = "plots/" + fil_name + ".pdf";
        c->SaveAs((canv_save_title).c_str());
    }

    return 0;

}




