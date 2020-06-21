//c++ -o tmp NN_Weight_Check.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
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

double invMass(hltObj::Jet* j1, hltObj::Jet* j2, hltObj::Jet* j3, hltObj::Jet* j4 ){

    double theta1 = 2*atan(exp(-j1->eta));
    double theta2 = 2*atan(exp(-j2->eta));
    double theta3 = 2*atan(exp(-j3->eta));
    double theta4 = 2*atan(exp(-j4->eta));

    
    double p1 = j1->pt / sin(theta1);
    double p2 = j2->pt / sin(theta2);
    double p3 = j3->pt / sin(theta3);
    double p4 = j4->pt / sin(theta4);

    double e1  = j1->e;
    double e2  = j2->e; 
    double e3  = j3->e; 
    double e4  = j4->e; 
    
    double dpx_2 = pow(p1*sin(theta1)*sin(j1->phi) + p2*sin(theta2)*sin(j2->phi) + p3*sin(theta3)*sin(j3->phi) + p4*sin(theta4)*sin(j4->phi), 2);
    double dpy_2 = pow(p1*sin(theta1)*cos(j1->phi) + p2*sin(theta2)*cos(j2->phi) + p3*sin(theta3)*cos(j3->phi) + p4*sin(theta4)*cos(j4->phi), 2);
    double dpz_2 = pow(p1*cos(theta1) + p2*cos(theta2) + p3*cos(theta3) + p4*cos(theta4), 2);
    
    return sqrt(pow(e1 + e2 + e3 + e4, 2) -(dpx_2 + dpy_2 + dpz_2));

};

int main(){


    TFile* f = new TFile("SequentialForTurnOn.root");
    std::string branch = "SaveAllJetsMC/Jets";
    std::string genbranch = "SaveGenHH/Gen";

    Event ev(f, branch, genbranch);

    //retrieving infos from HLTAnalyzer which stores the online HLT decision
    std::vector<std::string> path_names = {"HLT_Quad30Double60_FFNNHH4bwp0p87Calo_PFQuad30_v1", "HLT_Quad30Double60_FFNNHH4bwp0p87Calo_PFQuad30_v2"};    
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

    std::string trig_name = "HLT_FFNNHH4bwp0p87Calo";
    
    TH1F* h_tot_mhh = new TH1F("h_tot_mhh", "h_tot_mhh", 50, 0, 1200);
    TH1F* h_cut_mhh = new TH1F("h_cut_mhh", "h_cut_mhh", 50, 0, 1200);
    std::vector<TH1F*> h_tot;
    std::vector<TH1F*> h_cut;
    std::vector<TLegend*> leg;
    std::vector<TCanvas*> c;

    std::vector<std::vector<double>> ranges = {{0, 200}, {-5,5}, {-3.14, 3.14}, {0, 800}};
    

    for(int i = 0; i < 4; i++){
        h_cut.push_back(new TH1F(("h_cut_"+std::to_string(i)).c_str(), ("h_cut_"+std::to_string(i)).c_str(), 100, ranges.at(i).at(0), ranges.at(i).at(1)));
        h_cut[i]->SetFillStyle(3003);
        h_cut[i]->SetFillColor(kAzure+2);

        h_tot.push_back(new TH1F(("h_tot_"+std::to_string(i)).c_str(), ("h_tot_"+std::to_string(i)).c_str(), 100, ranges.at(i).at(0), ranges.at(i).at(1)));
        h_tot[i]->SetMarkerStyle(8);
        h_tot[i]->SetMarkerSize(0.8);
        h_tot[i]->SetLineColor(kBlack);

        leg.push_back(new TLegend(.89, .89, .45, .7));
        leg[i]->SetBorderSize(0);
        leg[i]->AddEntry(h_tot[i], "Weighted by TEfficiency");
        leg[i]->AddEntry(h_cut[i], "Passed HLT_FFNNHH4bwp0p87Calo");

        c.push_back(new TCanvas(("c_"+std::to_string(i)).c_str(), ("c_"+std::to_string(i)).c_str(), 1000, 1000, 1000, 700));
    }

    
    for(int ev_idx = 0; ev_idx < event_bits_.size(); ev_idx++){

        std::vector<int> evb = event_bits_.at(ev_idx);
        //if did not pass L1 then skip

        if(evb[0] != 0){
            ev.Generate();
            ev.UnpackCollections();
            hltObj::Jets reco = ev.GetRecoJetsCollection();
            std::vector<hltObj::Jet*> recoj = ev.GetRecoJets();

            std::vector<std::vector<double>> vars = {reco.pt, reco.eta, reco.phi, reco.e};

            std::vector<hltObj::Jet*> candidate_mhh;

            std::vector<double> btags = reco.btag;

            for(int i = 0; i < 4; i++){
                int j = std::distance(btags.begin(), std::max_element(btags.begin(), btags.end()));
                candidate_mhh.push_back(recoj.at(j));
                btags.erase(btags.begin() + j);
                recoj.erase(recoj.begin() + j);
            }

            double mHH = invMass(candidate_mhh[0], candidate_mhh[1], candidate_mhh[2], candidate_mhh[3]);
            h_tot_mhh->Fill(mHH);

            int count = 0;
            for(int i = 1; i < evb.size(); i++){
                if(evb.at(i) == 0) break;
                count++;
            }

            if (count == evb.size()-1) {
                h_cut_mhh->Fill(mHH);
                for(int i = 0; i < vars.size(); i++){
                    for(int j = 0; j < vars.at(i).size(); j++){
                        h_cut.at(i)->Fill(vars.at(i).at(j));
                    }
                }
            }
            
        }

    }

    TEfficiency* eff = new TEfficiency(*h_cut_mhh, *h_tot_mhh);

    std::cout << "Checking agreement" << std::endl;

    ev.SetStart(0);

     for(int ev_idx = 0; ev_idx < event_bits_.size(); ev_idx++){

        std::vector<int> evb = event_bits_.at(ev_idx);
        //if did not pass L1 then skip

        if(evb[0] != 0){
            ev.Generate();
            ev.UnpackCollections();
            hltObj::Jets reco = ev.GetRecoJetsCollection();
            std::vector<hltObj::Jet*> recoj = ev.GetRecoJets();

            std::vector<std::vector<double>> vars = {reco.pt, reco.eta, reco.phi, reco.e};

            std::vector<hltObj::Jet*> candidate_mhh;

            std::vector<double> btags = reco.btag;

            for(int i = 0; i < 4; i++){
                int j = std::distance(btags.begin(), std::max_element(btags.begin(), btags.end()));
                candidate_mhh.push_back(recoj.at(j));
                btags.erase(btags.begin() + j);
                recoj.erase(recoj.begin() + j);
            }

            double mHH = invMass(candidate_mhh[0], candidate_mhh[1], candidate_mhh[2], candidate_mhh[3]);

            double weight = eff->GetEfficiency(eff->FindFixBin(mHH));

            for(int i = 0; i < vars.size(); i++){
                for(int j = 0; j < vars.at(i).size(); j++){
                    h_tot.at(i)->Fill(vars.at(i).at(j), weight);
                }
            }

        }
    }

    std::cout << "Plotting..." << std::endl;

    gStyle->SetOptStat(0);
    std::vector<std::string> var_names = {"Offline Jet p_{T} (GeV)", "Offline Jet #eta", "Offline Jet #phi", "Offline Jet Energy (GeV)"};

    //TCanvas* canv = new TCanvas("cc", "cc", 1000, 1000, 3000, 2000);
    //canv->Divide(2,2);

    for(int i = 0; i < h_cut.size(); i++){
        std::string canv_save_title = ("plots/Weight_"+trig_name+"_"+std::to_string(i)+".pdf").c_str();
        c[i]->cd();
        //canv->cd(i+1);

        TPad *pad1 = new TPad(("pad1_"+std::to_string(i)).c_str(), ("pad1_"+std::to_string(i)).c_str(), 0, 0.3, 1, 1.0);
        pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
        pad1->Draw();             // Draw the upper pad: pad1
        pad1->cd();               // pad1 becomes the current pad
        double max = std::max(h_tot[i]->GetMaximum(), h_cut[i]->GetMaximum()) + std::max(h_tot[i]->GetMaximum(), h_cut[i]->GetMaximum())*0.2;
        h_tot[i]->SetMaximum(max);
        h_cut[i]->SetMaximum(max);
        h_tot[i]->GetYaxis()->SetTitle("Events");
        h_tot[i]->SetStats(0);          // No statistics on upper plot
        h_tot[i]->SetTitle(""); 
        h_tot[i]->Draw("pe2");               // Draw h1
        h_cut[i]->Draw("same");
        leg[i]->Draw("same");

        c[i]->cd();          // Go back to the main canvas before defining pad2
        //canv->cd(i+1);
        TPad *pad2 = new TPad(("pad2_"+std::to_string(i)).c_str(), ("pad2_"+std::to_string(i)).c_str(), 0, 0.05, 1, 0.3);
        pad2->SetTopMargin(0);
        pad2->SetBottomMargin(0.27);
        pad2->SetGridx(); // vertical grid
        pad2->Draw();
        pad2->cd();       // pad2 becomes the current pad

        // Define the ratio plot
        TH1F *h3 = (TH1F*)h_tot[i]->Clone(("h3_"+std::to_string(i)).c_str());
        h3->SetLineColor(kBlack);
        h3->SetMinimum(0.8);  // Define Y ..
        h3->SetMaximum(1.35); // .. range
        h3->Sumw2();
        h3->SetStats(0);      // No statistics on lower plot
        h3->Divide(h_cut[i]);
        h3->SetMarkerStyle(8);
        h3->SetMarkerSize(0.5);
        h3->SetFillStyle(3002);
        h3->SetFillColor(kAzure+2);
        h3->Draw("e2");
        h3->Draw("ep same");   

        h3->SetTitle("");

        // Y axis ratio plot settings
        h3->GetYaxis()->SetTitle("passed/weight ");
        h3->GetYaxis()->SetNdivisions(505);
        h3->GetYaxis()->SetTitleSize(20);
        h3->GetYaxis()->SetTitleFont(43);
        h3->GetYaxis()->SetTitleOffset(1.55);
        h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
        h3->GetYaxis()->SetLabelSize(15);

        // X axis ratio plot settings
        h3->GetXaxis()->SetTitle(var_names[i].c_str());
        h3->GetXaxis()->SetTitleSize(20);
        h3->GetXaxis()->SetTitleFont(43);
        h3->GetXaxis()->SetTitleOffset(4.);
        h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
        h3->GetXaxis()->SetLabelSize(15);

        c[i]->cd();
        c[i]->Draw();
        c[i]->SaveAs((canv_save_title).c_str());


    }

    //canv->Draw();
    //canv->SaveAs("plots/prova.pdf");

    return 0;

}




