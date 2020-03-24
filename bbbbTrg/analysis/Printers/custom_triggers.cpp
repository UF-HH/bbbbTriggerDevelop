// c++ -o cstmtrg custom_triggers.cpp ../src/trigger_manager.cc `root-config --cflags --glibs`

#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TApplication.h"
#include "TLorentzVector.h"
#include "TEfficiency.h"
#include "TStyle.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <valarray>
#include "../interface/FilterSelector.h"
#include "../interface/trigger_manager.h"
#include "../interface/make_trigger_paths.h"


using namespace cstmfltr;

bool check_dR(std::vector<double> b_eta, std::vector<double> b_phi, double R){

    std::vector<double> deltaR_list;
    for(int i = 0; i < b_eta.size(); i++){
        for(int j = 0; j < b_eta.size(); j++){
            if (j != i){
                deltaR_list.push_back( sqrt( pow(b_eta.at(i)-b_eta.at(j), 2) + pow(b_phi.at(i)-b_phi.at(j), 2) ) );
            }
        }
    }

    std::sort(deltaR_list.begin(), deltaR_list.end());
    if(deltaR_list[0]< R){
        return false;
    }
    else{
        return true;
    }
    
}

int main(int argc, char* argv[]) {
    
    if(argc != 3){
        std::cerr<< "Only two arguments: year, filein, isb?" << std::endl;
        throw std::runtime_error(("Found " + std::to_string(argc-1) + " Input arguments while two is required").c_str());
    }

    int year = std::atoi(argv[1]);
    std::string in_file = argv[2];

    make_trigger_paths paths(year, true);


    std::vector<std::string> trig_names = paths.get_trig_name();
    
    std::vector<std::pair<std::string,int>> fil_map = paths.get_map().at(0);    

    man_filter* filter = new man_filter(fil_map , trig_names.at(0));

    filter->set_OR(paths.get_OR().at(0));

    std::cout << "...year...  : " << year << std::endl;
    std::cout << "TRIGGER:  " << trig_names.at(0) << std::endl;
    for(auto c: filter->get_fil_names()){
        std::cout << "FILTER: " <<  c << std::endl;
    }

    TFile* f = new TFile(in_file.c_str());
    TTree* tree = (TTree*)f->Get("checkTrg/trgObjTree");
    
    //Saving relevant variables to compute by hand the efficiency
    std::vector<double>* trig_obj_pt = 0 ;
    std::vector<double>* trig_obj_et = 0 ;
    std::vector<double>* trig_obj_mass = 0;
    std::vector<double>* trig_obj_eta = 0;
    std::vector<double>* trig_obj_phi = 0;

    std::vector<int>* is_calo_jet = 0;
    std::vector<int>* is_calo_bjet = 0;

    std::vector<int>* is_pf_jet = 0;
    std::vector<int>* is_pf_bjet = 0;

    std::vector<int>* is_jet = 0;
    std::vector<int>* is_bjet = 0;

    std::vector<int>* is_calo_ht = 0;
    std::vector<int>* is_pf_ht = 0;
    std::vector<int>* is_l1_ht = 0;
    std::vector<int>* is_l1_jet = 0;
    std::vector<int>* is_hltPFCentralJetLooseIDQuad30 = 0;



    //Match jets
    std::vector<double>* matched_jet_pt = 0;
    std::vector<double>* matched_jet_mass = 0;
    std::vector<double>* matched_jet_eta = 0;
    std::vector<double>* matched_jet_phi = 0;
    std::vector<double>* matched_jet_energy = 0;
    std::vector<double>* matched_jet_b_tag = 0;


    //b quarks
    std::vector<double>* b_pt = 0;
    std::vector<double>* b_eta = 0;
    std::vector<double>* b_phi = 0;


    tree->SetBranchAddress("trig_obj_pt", &trig_obj_pt);
    tree->SetBranchAddress("trig_obj_et", &trig_obj_et);
    tree->SetBranchAddress("trig_obj_mass", &trig_obj_mass);
    tree->SetBranchAddress("trig_obj_eta", &trig_obj_eta);
    tree->SetBranchAddress("trig_obj_phi", &trig_obj_phi);
    
    tree->SetBranchAddress("matched_jet_pt", &matched_jet_pt);
    tree->SetBranchAddress("matched_jet_mass", &matched_jet_mass);
    tree->SetBranchAddress("matched_jet_eta", &matched_jet_eta);
    tree->SetBranchAddress("matched_jet_phi", &matched_jet_phi);
    tree->SetBranchAddress("matched_jet_energy", &matched_jet_energy);

    tree->SetBranchAddress("is_jet", &is_jet);
    tree->SetBranchAddress("is_bjet", &is_bjet);
    tree->SetBranchAddress("is_pf_ht", &is_pf_ht);
    tree->SetBranchAddress("is_calo_ht", &is_calo_ht);
    tree->SetBranchAddress("is_l1_ht", &is_l1_ht);
    tree->SetBranchAddress("is_l1_jet", &is_l1_jet);
    tree->SetBranchAddress("is_hltPFCentralJetLooseIDQuad30", &is_hltPFCentralJetLooseIDQuad30);

    tree->SetBranchAddress("is_calo_jet", &is_calo_jet);
    tree->SetBranchAddress("is_calo_bjet", &is_calo_bjet);
    tree->SetBranchAddress("is_pf_jet", &is_pf_jet);
    tree->SetBranchAddress("is_pf_bjet", &is_pf_bjet);

    tree->SetBranchAddress("b_pt", &b_pt);
    tree->SetBranchAddress("b_eta", &b_eta);
    tree->SetBranchAddress("b_phi", &b_phi);

    //Retrieve Info on counting labels
    std::vector<int> counts_(fil_map.size(), 0);
    std::vector<int> trig_counts_(trig_names.size(), 0);

    for(int  i = 0; i < fil_map.size(); i++){
        //std::cout << "READING FILTER BRANCH: " << fil_map.at(i).first << std::endl;
        tree->SetBranchAddress((fil_map.at(i).first).c_str(), &counts_.at(i));
    }

    for(int i = 0; i < trig_names.size(); i++){
        //std::cout << "READING TRIGGER BRANCH: " << trig_names.at(i) << std::endl;
        tree->SetBranchAddress(trig_names.at(i).c_str(), &trig_counts_.at(i));
    }  

    TH1F* pf_h_tot = new TH1F("tot_pf", "tot_pf", 100, 0, 700);
    TH1F* pf_h_cut = new TH1F("pf_h", "pf_h", 100, 0, 700);
    pf_h_cut->SetFillStyle(3003);
    pf_h_cut->SetFillColor(kAzure+3);
    pf_h_cut->SetLineColor(kAzure+2);

    TH1F* calo_h_tot = new TH1F("tot_calo", "tot_calo", 100, 0, 700);
    TH1F* calo_h_cut = new TH1F("calo_h", "calo_h", 100, 0, 700);
    calo_h_cut->SetFillStyle(3003);
    calo_h_cut->SetFillColor(kAzure+3);
    calo_h_cut->SetLineColor(kAzure+2);

    std::vector<TH1F*> h;
    for(int i = 0; i < 11; i++){
        std::string name = std::to_string(i);
        h.push_back(new TH1F(name.c_str(), name.c_str(), 100, 0, 700));
    }

    std::vector<int> eff;
    eff.resize(9);

    int count = 0;

    for(int i = 0; i < tree->GetEntries(); i++){

        tree->GetEntry(i);  
        //retrieving pf jets
        std::vector<double> pf_pt;
        std::vector<double> pf_eta;
        std::vector<double> pf_phi;
        std::vector<int> pf_btag;

        std::vector<double> calo_pt;
        std::vector<double> calo_eta;
        std::vector<double> calo_et;
        std::vector<double> calo_phi;
        std::vector<int> calo_btag;
        
        std::vector<double> calo_bjet_pt;
        std::vector<double> calo_bjet_et;
        std::vector<double> calo_bjet_eta;

        std::vector<double> pf_bjet_pt;
        std::vector<double> pf_bjet_et;
        std::vector<double> pf_bjet_eta;

        std::vector<double> l1_pt;
        std::vector<double> l1_eta;
        std::vector<double> l1_phi;

        std::vector<double> LI30_pt;
        std::vector<double> LI30_eta;


        

        double calo_HT = 0;
        double pf_HT = 0;
        double l1_HT = 0;


        for(uint j = 0; j < trig_obj_pt->size(); j++){

            if(is_hltPFCentralJetLooseIDQuad30->at(j) == 1){
                LI30_pt.push_back(trig_obj_pt->at(j));
                LI30_eta.push_back(trig_obj_eta->at(j));
            }
            else{
                LI30_pt.push_back(0);
                LI30_eta.push_back(0);
            }

            //kinematics
            if(is_pf_jet->at(j) == 1){
                pf_pt.push_back(trig_obj_pt->at(j));
                pf_eta.push_back(trig_obj_eta->at(j));
                pf_phi.push_back(trig_obj_phi->at(j));
            }
            else{
                pf_pt.push_back(0);
                pf_eta.push_back(0);
                pf_phi.push_back(0);
            }
            if(is_calo_jet->at(j) == 1){
                calo_pt.push_back(trig_obj_pt->at(j));
                calo_et.push_back(trig_obj_et->at(j));
                calo_eta.push_back(trig_obj_eta->at(j));
                calo_phi.push_back(trig_obj_phi->at(j));
            }
            else{
                calo_pt.push_back(0);
                calo_et.push_back(0);
                calo_eta.push_back(0);
                calo_phi.push_back(0);
            }   

            if(is_calo_ht->at(j) == 1){
                calo_HT = trig_obj_et->at(j);
            }

            if(is_pf_ht->at(j) == 1)
            {   
                pf_HT = trig_obj_pt->at(j);
                
            }
            if(is_l1_ht->at(j) == 1){
                l1_HT = trig_obj_pt->at(j);
            }

            if(is_l1_jet->at(j) == 1){
                l1_pt.push_back(trig_obj_pt->at(j));
                l1_eta.push_back(trig_obj_eta->at(j));
                l1_phi.push_back(trig_obj_phi->at(j));
            }
            else{
                l1_pt.push_back(0);
                l1_eta.push_back(0);
                l1_phi.push_back(0);
            }
            //else{
             //   continue;
            //}

            //btagging

            if(is_bjet->at(j) == 1){
                if(is_pf_bjet->at(j) == 1){
                    pf_bjet_pt.push_back(trig_obj_pt->at(j));
                    pf_bjet_et.push_back(trig_obj_et->at(j));
                    pf_bjet_eta.push_back(trig_obj_eta->at(j));
                }
                else{
                    pf_bjet_pt.push_back(0);
                    pf_bjet_et.push_back(0);
                    pf_bjet_eta.push_back(0);
                }
                if (is_calo_bjet->at(j) == 1){
                    calo_bjet_pt.push_back(trig_obj_pt->at(j));
                    calo_bjet_et.push_back(trig_obj_et->at(j));
                    calo_bjet_eta.push_back(trig_obj_eta->at(j));
                }
                else{
                    calo_bjet_pt.push_back(0);
                    calo_bjet_et.push_back(0);
                    calo_bjet_eta.push_back(0);
                }

            }
            else{
                calo_bjet_pt.push_back(0);
                calo_bjet_et.push_back(0);
                calo_bjet_eta.push_back(0);
                pf_bjet_pt.push_back(0);
                pf_bjet_et.push_back(0);
                pf_bjet_eta.push_back(0);
            }

        } 

        if(calo_HT < 320){
            counts_.at(2) = 0;
        }
        else if(pf_HT < 330){
            counts_.at(counts_.size()-1) = 0;
        }
        auto r_1 = filter->f_test_suppose_sequential(counts_, true, 1);


        //int count_bcalo = std::count_if(calo_btag.begin(), calo_btag.end(), [](double j){return j > 0;});
        //int count_calo = std::count_if(calo_pt.begin(), calo_pt.end(), [](double j){return j > 0;});

        //missing prescale
        std::vector<int> counts;
        
        //L1 filter
        counts.push_back(hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet(l1_pt, l1_eta, l1_HT));
        counts.push_back(hltQuadCentralJet30(calo_bjet_pt, calo_bjet_eta));
        counts.push_back(hltCaloQuadJet30HT320(calo_bjet_pt, calo_bjet_et, calo_bjet_eta));
        counts.push_back(hltPFCentralJetLooseIDQuad30(pf_bjet_pt, pf_bjet_eta));
        //counts.push_back(hltPFCentralJetLooseIDQuad30(LI30_pt, LI30_eta));
        counts.push_back(hlt1PFCentralJetLooseID75(pf_pt, pf_eta));
        counts.push_back(hlt2PFCentralJetLooseID60(pf_pt, pf_eta));
        counts.push_back(hlt3PFCentralJetLooseID45(pf_pt, pf_eta));
        counts.push_back(hlt4PFCentralJetLooseID40(pf_pt, pf_eta));
        counts.push_back(hltPFCentralJetsLooseIDQuad30HT330(pf_bjet_pt, pf_bjet_et, pf_bjet_eta));

        auto count_pf_b = std::count_if(pf_bjet_pt.begin(), pf_bjet_pt.end(), [](double p){ return p > 0; });
        auto count_LI = std::count_if(LI30_pt.begin(), LI30_pt.end(), [](double p){ return p > 0; });

        //std:: cout << hltPFCentralJetLooseIDQuad30(pf_pt, pf_eta) << " " <<  hltPFCentralJetLooseIDQuad30(pf_bjet_pt, pf_bjet_eta) << " " <<  count_LI << std::endl;
        
        std::vector<int> thresholds = {1,4,1,4,1,2,3,4,1};

        std::sort(pf_pt.begin(), pf_pt.end());
        std::sort(calo_pt.begin(), calo_pt.end());

        pf_h_tot->Fill(pf_pt.at(pf_pt.size()-1));

        bool ok = true;
        for(uint ind = 0; ind < counts.size(); ind++){
            if(counts.at(ind) < thresholds.at(ind)){
                ok = false;
            }
            if(ok){
                eff.at(ind)++;
                h.at(ind)->Fill(pf_pt.at(pf_pt.size()-1));
            }
                
        }

        if(!ok){
            count++;
        }

    }

    std::vector<double> tot_counts = filter->get_sum_vector();

    std::cout << "Efficiency tot evt:  " << pf_h_tot->GetEntries() << " cut evt: " << pf_h_cut->GetEntries() << " Eff: " << (double)pf_h_cut->GetEntries()/pf_h_tot->GetEntries()  << std::endl;
    std::cout << "Tot evts: " << tree->GetEntries() << std::endl;
    std::cout << "Not passed: " << count << std::endl;
    std::cout << "Rateo: " << (double)(tree->GetEntries()-count)/tree->GetEntries() << std::endl;

    for(int i = 0; i < filter->get_fil_names().size(); i++){
        std::cout << "Filter: " << filter->get_fil_names().at(i) << " Hand cut passed: " << eff.at(i) << " Rateo: " << (double)eff.at(i)/tree->GetEntries() << " Label count passed: " << tot_counts.at(i) << " Rateo: " << (double)tot_counts.at(i)/tree->GetEntries() << std::endl;
        
    }

    std::cout << "Relative Eff: " << std::endl;
    for(int i = 1; i < filter->get_fil_names().size(); i++){
        std::cout << "Filter: " << filter->get_fil_names().at(i) << " Hand cut passed: " << (double)eff.at(i)/eff.at(i-1) << " Label count passed: " << (double)tot_counts.at(i)/tot_counts.at(i-1)  << std::endl;
        
    }

    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 800);
    pf_h_tot->Draw("hist");
    for(auto hist : h){
        hist->Draw("hist same");
    }
    //pf_h_cut->Draw("hist same");
    c->Draw();
    c->Update();
     
    
    return 0;
}

