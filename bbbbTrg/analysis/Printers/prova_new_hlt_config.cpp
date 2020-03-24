// c++ -o new_cstmtrg prova_new_hlt_config.cpp ../src/trigger_manager.cc `root-config --cflags --glibs`

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
#include "../interface/Obj.h"

using namespace cstmfltr;


int main(int argc, char* argv[]) {
    
    if(argc != 3){
        std::cerr<< "Only two arguments: year, filein, isb?" << std::endl;
        throw std::runtime_error(("Found " + std::to_string(argc-1) + " Input arguments while two is required").c_str());
    }

    hltObj::HLTCuts cuts_quad_jet_40(30.0, 1.0E99, -2.5, 2.5);
    cuts_quad_jet_40.MinN = 4;

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



    int count = 0;
    int count2 = 0;

    for(int i = 0; i < tree->GetEntries(); i++){

        hltObj::Jets PFjets;
        hltObj::Jets L1jets;
        hltObj::Jets Calojets;

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

        if(calo_HT < 320.0){
            counts_.at(2) = 0;
        }
        else if(pf_HT < 330.0){
            counts_.at(counts_.size()-1) = 0;
        }
        auto r_1 = filter->f_test_suppose_sequential(counts_, true, 1);


        //int count_bcalo = std::count_if(calo_btag.begin(), calo_btag.end(), [](double j){return j > 0;});
        //int count_calo = std::count_if(calo_pt.begin(), calo_pt.end(), [](double j){return j > 0;});

        //missing prescale

        L1jets.pt = l1_pt;
        L1jets.eta = l1_eta;
        L1jets.HT = l1_HT;

        PFjets.pt = pf_bjet_pt;
        PFjets.eta = pf_bjet_eta;
        PFjets.HT = pf_HT;

        Calojets.pt = calo_bjet_pt;
        Calojets.eta = calo_bjet_eta;
        Calojets.HT = calo_HT;


        bool passed_new = true;
        bool passed_old = true;

        //-----------------L1--------------------------------------
        std::vector<hltObj::HLTCuts> L1Cuts;
        std::vector<std::string> Names = {"CustomFixedHTSelector", "CustomFixedHTSelector", "CustomFixedHTSelector", "CustomFixedHTSelector",  
                                            "CustomFixedHTSelector", "CustomFixedPtEtaSelector", "CustomVariablePtEtaHTSelector", "CustomVariablePtEtaHTSelector", 
                                                "CustomVariablePtEtaHTSelector", "CustomVariablePtEtaHTSelector" };
        
        hltObj::HLTCuts one(280.0);
        hltObj::HLTCuts two(320.0);
        hltObj::HLTCuts three(360.0);
        hltObj::HLTCuts four(400.0);
        hltObj::HLTCuts five(450.0);
        hltObj::HLTCuts six(60, 1.0E99, -2.5, 2.5);
        six.MinN=4;

        std::vector<double> pt_min = {70.0, 55.0, 40.0, 35.0};
        std::vector<double> pt_max = {1.0E99, 1.0E99, 1.0E99, 1.0E99};
        std::vector<double> eta_min = {-2.4, -2.4, -2.4, -2.4};
        std::vector<double> eta_max = {2.4, 2.4, 2.4, 2.4};
        hltObj::HLTCuts seven(pt_min, pt_max, eta_min, eta_max);
        seven.HTMin = 280.0;

        pt_min = {70.0,55.0,40.0,40.0};
        hltObj::HLTCuts eight(pt_min, pt_max, eta_min, eta_max);
        eight.HTMin = 320.0;

        pt_min = {80.0, 60.0, 45.0, 40.0};
        eta_min = {-2.1, -2.1, -2.3, -2.3};
        eta_max = {2.1, 2.1, 2.3, 2.3};
        hltObj::HLTCuts nine(pt_min, pt_max, eta_min, eta_max);
        nine.HTMin = 320.0;

        pt_min = {80.0, 60.0, 50.0, 45.0};
        hltObj::HLTCuts ten(pt_min, pt_max, eta_min, eta_max);
        ten.HTMin = 320.0;

        L1Cuts.push_back(one);
        L1Cuts.push_back(two);
        L1Cuts.push_back(three);
        L1Cuts.push_back(four);
        L1Cuts.push_back(five);
        L1Cuts.push_back(six);
        L1Cuts.push_back(seven);
        L1Cuts.push_back(eight);
        L1Cuts.push_back(nine);
        L1Cuts.push_back(ten);
        
        passed_old  = passed_old && hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet(l1_pt, l1_eta, l1_HT)>=1;
        passed_old  = passed_old &&  hltQuadCentralJet30(calo_bjet_pt, calo_bjet_eta)>=4;
        passed_old  = passed_old && hltCaloQuadJet30HT320(calo_bjet_pt, calo_bjet_et, calo_bjet_eta)>=1;
        passed_old  = passed_old && hltPFCentralJetLooseIDQuad30(pf_bjet_pt, pf_bjet_eta)>=4;
        passed_old  = passed_old && hlt1PFCentralJetLooseID75(pf_pt, pf_eta)>=1;
        passed_old  = passed_old &&  hlt2PFCentralJetLooseID60(pf_pt, pf_eta)>=2;
        passed_old  = passed_old &&  hlt3PFCentralJetLooseID45(pf_pt, pf_eta)>=3;
        passed_old  = passed_old &&  hlt4PFCentralJetLooseID40(pf_pt, pf_eta)>=4;
        passed_old  = passed_old &&  hltPFCentralJetsLooseIDQuad30HT330(pf_bjet_pt, pf_bjet_et, pf_bjet_eta)>=1;

        passed_new  =  passed_new && CustomL1Trigger(Names, L1Cuts, L1jets);

        hltObj::HLTCuts hltQuadCentralJet30(30.0, 1.0E99, -2.5, 2.5);
        hltQuadCentralJet30.MinN = 4;
        passed_new = passed_new && CustomFixedPtEtaSelector(Calojets, hltQuadCentralJet30);

        hltObj::HLTCuts hltCaloQuadJet30HT320(30.0, 1.0E99, -2.5, 2.5, 320.0, 0.0, 1);
        passed_new = passed_new && CustomFixedPtEtaHTSelector(Calojets, hltCaloQuadJet30HT320);

        hltObj::HLTCuts hltPFCentralJetLooseIDQuad30(30.0, 1.0E99, -2.5, 2.5);
        hltPFCentralJetLooseIDQuad30.MinN = 4;
        passed_new = passed_new && CustomFixedPtEtaSelector(PFjets, hltPFCentralJetLooseIDQuad30);

        hltObj::HLTCuts hlt1PFCentralJetLooseID75(75.0, 1.0E99, -2.5, 2.5);
        hlt1PFCentralJetLooseID75.MinN = 1;
        passed_new = passed_new && CustomFixedPtEtaSelector(PFjets, hlt1PFCentralJetLooseID75);

        hltObj::HLTCuts hlt1PFCentralJetLooseID60(60.0, 1.0E99, -2.5, 2.5);
        hlt1PFCentralJetLooseID60.MinN = 2;
        passed_new = passed_new && CustomFixedPtEtaSelector(PFjets, hlt1PFCentralJetLooseID60);

        hltObj::HLTCuts hlt1PFCentralJetLooseID45(45.0, 1.0E99, -2.5, 2.5);
        hlt1PFCentralJetLooseID45.MinN = 3;
        passed_new = passed_new && CustomFixedPtEtaSelector(PFjets, hlt1PFCentralJetLooseID45);

        hltObj::HLTCuts hlt1PFCentralJetLooseID40(40.0, 1.0E99, -2.5, 2.5);
        hlt1PFCentralJetLooseID40.MinN = 4;
        passed_new = passed_new && CustomFixedPtEtaSelector(PFjets, hlt1PFCentralJetLooseID40);

        hltObj::HLTCuts hltPFCentralJetsLooseIDQuad30HT330(30.0, 1.0E99, -2.5, 2.5, 330.0, 0.0, 1);
        passed_new = passed_new && CustomFixedPtEtaHTSelector(PFjets, hltPFCentralJetsLooseIDQuad30HT330);

        if(passed_new)
            count++;
        if(passed_old)
            count2++;
        
    }

    //std::cout << "Efficiency tot evt:  " << pf_h_tot->GetEntries() << " cut evt: " << pf_h_cut->GetEntries() << " Eff: " << (double)pf_h_cut->GetEntries()/pf_h_tot->GetEntries()  << std::endl;
    std::cout << "Tot evts: " << tree->GetEntries() << std::endl;
    std::cout << "Count: " << count << std::endl;
    std::cout << "Count2: " << count2 << std::endl;


    return 0;
}

