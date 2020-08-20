//c++ -o tmp TurnOnAllSumB_KinBtag_OnlyReco.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
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

bool PassedSkims(hltObj::Jets jets){

    bool pass = true;
    std::vector<double> results;
    double PtMin = 40;
    double PtMax = 1e99;
    double EtaMax = 2.4;
    double EtaMin = -2.4;
    double BtagMin = 0.27;
    int MinJets = 4;
    int MinBtaggedJets = 3;

    auto it = std::find_if(std::begin(jets.pt), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
    while (it != std::end(jets.pt)) {
        auto dis = std::distance(std::begin(jets.pt), it);
        if(jets.eta.at(dis) <= EtaMax && jets.eta.at(dis) >= EtaMin)
            results.emplace_back(jets.btag.at(dis));
        it = std::find_if(std::next(it), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
    }   
    if(results.size() < MinJets || std::count_if(results.begin(), results.end(), [BtagMin](double i){return i > BtagMin;}) < MinBtaggedJets){
        return false;
    }
    else{
        return pass;
    }

};

int main(){


    TFile* f = new TFile("SumBTagTriggersReco.root");
    std::string branch = "SaveRecoJ/Jets";
    std::string genbranch = "SaveGenHH/Gen";

    TTree* jets_tree = (TTree*)f->Get(branch.c_str());
    std::vector<double>* reco_btag_ = 0;
    std::vector<double>* reco_pt_ = 0;
    std::vector<double>* reco_et_ = 0;
    std::vector<double>* reco_eta_ = 0;
    std::vector<double>* reco_phi_ = 0;
    std::vector<double>* reco_e_ = 0;
    std::vector<double>* reco_mass_ = 0;

    jets_tree->SetBranchAddress("reco_btag", &reco_btag_);
    jets_tree->SetBranchAddress("reco_pt", &reco_pt_);
    jets_tree->SetBranchAddress("reco_et", &reco_et_);
    jets_tree->SetBranchAddress("reco_eta", &reco_eta_);
    jets_tree->SetBranchAddress("reco_phi", &reco_phi_);
    jets_tree->SetBranchAddress("reco_e", &reco_e_);
    jets_tree->SetBranchAddress("reco_mass", &reco_mass_);

    std::vector<std::string> all_paths_names = {"HLT_Quad30_Double60_Sum2LeadingBTag0p9","HLT_Quad30_Double60_Sum2LeadingBTag1p0","HLT_Quad30_Double60_Sum2LeadingBTag1p1","HLT_Quad30_Double60_Sum2LeadingBTag1p2","HLT_Quad30_Double60_Sum2LeadingBTag1p3","HLT_Quad30_Double60_Sum2LeadingBTag1p4","HLT_Quad30_Double60_Sum2LeadingBTag1p5","HLT_Quad30_Double60_Sum2LeadingBTag1p6","HLT_Quad30_Double60_Sum2LeadingBTag1p7","HLT_Quad30_Double60_Sum2LeadingBTag1p8","HLT_Quad30_Double60_Sum2LeadingBTag1p9","HLT_Quad30_Double60_Sum2LeadingBTag2p0","HLT_Quad30_Double60_Sum2LeadingBTag2p1"};
    std::vector<std::string> path_names_Kin = {"HLT_Quad30_Double60_Sum2LeadingBTag_OnlyKin"};

    for(int p_name = 0; p_name < all_paths_names.size(); p_name++){

        //retrieving infos from HLTAnalyzer which stores the online HLT decision
        std::string path_names_Btag = all_paths_names.at(p_name);
        int trigbit_Kin;
        int trigbit_Btag;
        std::vector<int> event_bits_Kin;
        std::vector<int> event_bits_Btag;

        TTree* tree = (TTree*)f->Get("MyHLTAnalyzer/trgObjTree");
        std::cout << "[INFO]: Reading branches..." << std::endl;
        std::cout << "[INFO]: New branch ... " << path_names_Kin.at(0) << std::endl;
        tree->SetBranchAddress(path_names_Kin.at(0).c_str(), &trigbit_Kin);
        std::cout << "[INFO]: New branch ... " << path_names_Btag << std::endl;
        tree->SetBranchAddress(path_names_Btag.c_str(), &trigbit_Btag);


        for (int ev=0; ev < tree->GetEntries(); ev++){
            tree->GetEntry(ev);
            event_bits_Kin.push_back(trigbit_Kin);
            event_bits_Btag.push_back(trigbit_Btag);
        }

        std::string trig_name = path_names_Btag;
        
        TH1F* h_tot_kin = new TH1F("mHHTurnOn", "mHHTurnOn", 100, 0, 1500);
        TH1F* h_cut_kin = new TH1F("mHHTurnOn_cut", "mHHTurnOn_cut", 100, 0, 1500);

        TH1F* h_tot_btag = new TH1F("BtagTurnOn", "BtagTurnOn", 30, 0, 2);
        TH1F* h_cut_btag = new TH1F("BtagTurnOn_cut", "BtagTurnOn_cut", 30, 0, 2);

        h_tot_kin->SetMarkerStyle(8);
        h_tot_kin->SetMarkerSize(1);
        h_tot_kin->SetLineColor(kBlack);
        h_cut_kin->SetFillStyle(3003);
        h_cut_kin->SetFillColor(kAzure+2);

        h_tot_btag->SetMarkerStyle(8);
        h_tot_btag->SetMarkerSize(1);
        h_tot_btag->SetLineColor(kBlack);
        h_cut_btag->SetFillStyle(3003);
        h_cut_btag->SetFillColor(kAzure+2);

        TLegend* leg = new TLegend(.89, .89, .45, .7);
        leg->SetBorderSize(0);
        leg->AddEntry(h_tot_kin, "Tot Events");
        std::string leg_name = "Passed ";
        leg_name = leg_name + trig_name + " Only Kin";
        leg->AddEntry(h_cut_kin, leg_name.c_str());

        TLegend* leg1 = new TLegend(.89, .89, .45, .7);
        leg1->SetBorderSize(0);
        leg1->AddEntry(h_tot_btag, "Tot Events");
        leg_name = "Passed ";
        leg_name = leg_name + trig_name + " Only Btag";
        leg1->AddEntry(h_cut_btag, leg_name.c_str());

        int count = 0;
        
        for(int ev_idx = 0; ev_idx < event_bits_Btag.size(); ev_idx++){

            //retrieving the trigger bits (this will be unidimensional vectors)
            int evb_k = event_bits_Kin.at(ev_idx);
            int evb_b = event_bits_Btag.at(ev_idx);


            jets_tree->GetEntry(ev_idx);

            hltObj::Jets RecoJets;

            RecoJets.pt = *reco_pt_;
            RecoJets.mass = *reco_mass_;
            RecoJets.eta = *reco_eta_;
            RecoJets.phi = *reco_phi_;
            RecoJets.e = *reco_e_;
            RecoJets.et = *reco_et_;
            RecoJets.btag = *reco_btag_;

            std::vector<hltObj::Jet*> RecoJ;

            for(int i = 0; i < RecoJets.size(); i++){
                RecoJ.push_back(new hltObj::Jet(RecoJets.pt.at(i), RecoJets.eta.at(i), RecoJets.phi.at(i)));
                RecoJ.at(RecoJ.size()-1)->e = RecoJets.e.at(i); //last jet added, add info about energy
            }

            if (evb_b && PassedSkims(RecoJets)){
                count++;
            }

            std::vector<hltObj::Jet*> candidate_mhh;

            std::vector<double> btags = *reco_btag_;

            for(int i = 0; i < 4; i++){
                int j = std::distance(btags.begin(), std::max_element(btags.begin(), btags.end()));
                candidate_mhh.push_back(RecoJ.at(j));
                btags.erase(btags.begin() + j);
                RecoJ.erase(RecoJ.begin() + j);
            }

            double mHH = invMass(candidate_mhh[0], candidate_mhh[1], candidate_mhh[2], candidate_mhh[3]);
            std::sort(reco_btag_->begin(), reco_btag_->end(), std::greater<double>());
            double BtagSum = reco_btag_->at(0) + reco_btag_->at(1); //saving thrid btag score offline objectsdouble BtagSum = reco.btag.at(0) + reco.btag.at(1); //saving thrid btag score offline objects

            //building histo 4 turn on curves
            if(PassedSkims(RecoJets)){

                h_tot_kin->Fill(mHH);

                if (evb_k) {
                    h_cut_kin->Fill(mHH);
                    h_tot_btag->Fill(BtagSum);

                    if (evb_b) {
                        h_cut_btag->Fill(BtagSum);
                    }

                }

            }

        }

        std::cout << "[INFO]: Trigger & preselections intersection: " << count << "/" << event_bits_Btag.size() << std::endl;

        std::cout << "[INFO] Kinematic statistics..." << std::endl;
        std::cout << (double)h_tot_kin->GetEntries() << std::endl;
        std::cout << (double)h_cut_kin->GetEntries() << std::endl;
        std::cout << (double)h_cut_kin->GetEntries()/h_tot_kin->GetEntries() << std::endl;

        std::cout << "[INFO] Btag statistics..." << std::endl;
        std::cout << (double)h_tot_btag->GetEntries() << std::endl;
        std::cout << (double)h_cut_btag->GetEntries() << std::endl;
        std::cout << (double)h_cut_btag->GetEntries()/h_tot_btag->GetEntries() << std::endl;

        gStyle->SetOptStat(0);

        TLatex T = TLatex();
        TLatex T1 = TLatex();

        TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 700);
        TEfficiency* eff_kin = new TEfficiency(*h_cut_kin, *h_tot_kin);
        eff_kin->SetFillStyle(3004);
        eff_kin->SetLineColor(kRed);

        leg->AddEntry(eff_kin, "Efficiency");

        h_tot_kin->SetTitle("");
        double max = h_tot_kin->GetMaximum();
        h_tot_kin->Scale(1./max);
        h_cut_kin->Scale(1./max);
        h_tot_kin->SetMaximum(1.6);
        h_tot_kin->GetXaxis()->SetTitle("Offline mHH (GeV)");
        h_tot_kin->GetXaxis()->SetTitleSize(.02);
        h_tot_kin->GetXaxis()->SetTitleOffset(2);
        h_tot_kin->GetYaxis()->SetTitle("a.u.");

        h_tot_kin->Draw("P");
        h_cut_kin->Draw("hist same");
        leg->Draw();
        eff_kin->Draw("P same");
        T.DrawLatexNDC(.45, .92, "#scale[0.6]{#font[52]{ggF#rightarrow HH #rightarrow bbbb, #sqrt{s} = 14 TeV, [k_{t},k_{#lambda}] = [1.0,1.0]}}");
        T1.DrawLatexNDC(.1, .92, "#scale[1.]{ #font[62]{CMS} #font[52]{Preliminary} }");
        c->Draw();
        std::string canv_save_title = "plots/" + trig_name + "_mHH.pdf";
        c->SaveAs((canv_save_title).c_str());

        std::string mHH_file_name = "weights/" + trig_name + "_mHHweights.root";
        TFile* f_o = new TFile(mHH_file_name.c_str(), "RECREATE");
        eff_kin->Write("mHH_weight");
        f_o->Write();
        f_o->Close();

        //Saving btag3 weights 

        TCanvas* c1 = new TCanvas("c", "c", 1000, 1000, 1000, 700);
        TEfficiency* eff_btag = new TEfficiency(*h_cut_btag, *h_tot_btag);
        eff_btag->SetFillStyle(3004);
        eff_btag->SetLineColor(kRed);

        leg1->AddEntry(eff_btag, "Efficiency");

        h_tot_btag->SetTitle("");
        max = h_tot_btag->GetMaximum();
        h_tot_btag->Scale(1./max);
        h_cut_btag->Scale(1./max);
        h_tot_btag->SetMaximum(1.6);
        h_tot_btag->GetXaxis()->SetTitle("Offline Leading 2 Btag Sum");
        h_tot_btag->GetXaxis()->SetTitleSize(.02);
        h_tot_btag->GetXaxis()->SetTitleOffset(2);
        h_tot_btag->GetYaxis()->SetTitle("a.u.");

        h_tot_btag->Draw("P");
        h_cut_btag->Draw("hist same");
        leg1->Draw();
        eff_btag->Draw("P same");
        T.DrawLatexNDC(.45, .92, "#scale[0.6]{#font[52]{ggF#rightarrow HH #rightarrow bbbb, #sqrt{s} = 14 TeV, [k_{t},k_{#lambda}] = [1.0,1.0]}}");
        T1.DrawLatexNDC(.1, .92, "#scale[1.]{ #font[62]{CMS} #font[52]{Preliminary} }");

        c1->Draw();
        canv_save_title = "plots/" + trig_name + "_BtagSum.pdf";
        c1->SaveAs((canv_save_title).c_str());

        std::string Btag_file_name = "weights/" + trig_name + "_BTagweights.root";
        TFile* f_o_ = new TFile(Btag_file_name.c_str(), "RECREATE");
        eff_btag->Write("BTagSum_weight");
        f_o_->Write();
        f_o_->Close();

    }


    return 0;

}




