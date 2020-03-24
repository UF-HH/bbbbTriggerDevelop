//c++ -o trig_eff trig_eff.cpp trigger_manager.cc `root-config --cflags --glibs`

#include "trigger_manager.h"
#include <TTree.h>
#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TStyle.h>
#include <iostream>
#include <string>
#include <sstream>

struct trigger_counter{
    int trig1_ = 0;
    int trig2_ = 0;
    int trig_or_ = 0;
};

std::vector<int> config_counts(std::vector<int> ev_count, std::vector<int> fil_id){

    std::vector<int> result;
    for(auto c : fil_id){
        result.push_back(ev_count.at(c));
    }

    return result;
}

std::vector<double> compute_total_relative_eff(std::vector<int> fil_res, int tot_ev, int after_cut){
        std::vector<double> final;
        final.push_back((double)after_cut/after_cut);

        for( int i = 0; i < fil_res.size(); i++){
            if( i == 0) {
                final.push_back((double)fil_res.at(0)/after_cut);
            }
            else{
                final.push_back((double)fil_res.at(i)/fil_res.at(i-1));
            }
        }

        return final;
}

std::vector<double> compute_total_cumulative_eff(std::vector<int> fil_res, int tot_ev, int after_cut){
        std::vector<double> final;
        final.push_back((double)after_cut/after_cut);

        for( int i = 0; i < fil_res.size(); i++){
            final.push_back((double)fil_res.at(i)/after_cut);
        }

        return final;
}


int main(int argc, char* argv[]){

    std::vector<std::pair<std::string,int>> fil_map;
    fil_map.push_back(std::pair<std::string, int>("hltL1sQuadJetC50IorQuadJetC60IorHTT280IorHTT300IorHTT320IorTripleJet846848VBFIorTripleJet887256VBFIorTripleJet927664VBF", 1));
    fil_map.push_back(std::pair<std::string, int>("hltL1sQuadJetCIorTripleJetVBFIorHTT", 1));
    fil_map.push_back(std::pair<std::string, int>("hltQuadCentralJet45", 4));
    fil_map.push_back(std::pair<std::string, int>("hltQuadPFCentralJetLooseID45", 4));
    fil_map.push_back(std::pair<std::string, int>("hltBTagCaloCSVp087Triple", 3));
    fil_map.push_back(std::pair<std::string, int>("hltL1sTripleJetVBFIorHTTIorDoubleJetCIorSingleJet", 1));
    fil_map.push_back(std::pair<std::string, int>("hltDoubleCentralJet90", 2));
    fil_map.push_back(std::pair<std::string, int>("hltQuadCentralJet30", 4));
    fil_map.push_back(std::pair<std::string, int>("hltDoublePFCentralJetLooseID90", 2));
    fil_map.push_back(std::pair<std::string, int>("hltQuadPFCentralJetLooseID30", 4));

    std::vector<std::string> trigger_names_;

    trigger_names_.push_back("HLT_QuadJet45_TripleBTagCSV_p087_v6");
    trigger_names_.push_back("HLT_DoubleJet90_Double30_TripleBTagCSV_p087_v5");
    
    std::vector<std::vector<int> > OR = {{0,1}};

    std::vector<std::pair<std::string,int>> fil_1_map;
    std::vector<std::pair<std::string,int>> fil_2_map;

    std::vector<int> fil_ind_1 = {0, 1, 2, 4, 3};
    std::vector<int> fil_ind_2 = {5, 7, 6, 4, 9, 8};

    for(auto c: fil_ind_1){
        fil_1_map.push_back(fil_map.at(c));
    }

    for(auto c: fil_ind_2){
        fil_2_map.push_back(fil_map.at(c));
    }
    
    //std::vector<std::pair<std::string,int>> fil_1_map(fil_map.begin(), fil_map.begin()+5);
    //std::vector<std::pair<std::string,int>> fil_2_map(fil_map.begin()+5, fil_map.end());


    man_filter filter_1(fil_1_map, trigger_names_.at(0));
    man_filter filter_2(fil_2_map, trigger_names_.at(1));

    filter_1.initialize_filters_id();
    filter_1.set_OR(OR);

    filter_2.initialize_filters_id();
    //filter_2.add_to_map("hltBTagCaloCSVp087Triple", 3);
    

    //reading the tree

    TFile* f = new TFile("../data/trigger_obj_tree.root");
    TTree* tree_ = (TTree*)f->Get("checkTrg/trgObjTree");

    std::vector<int> counts_(fil_map.size(), 0);
    std::vector<int> trig_counts_(trigger_names_.size(), 0);

    for(int  i = 0; i < fil_map.size(); i++){
        std::cout << "READING FILTER BRANCH: " << fil_map.at(i).first << std::endl;
        tree_->SetBranchAddress((fil_map.at(i).first).c_str(), &counts_.at(i));
    }

    for(int i = 0; i < trigger_names_.size(); i++){
        std::cout << "READING TRIGGER BRANCH: " << trigger_names_.at(i) << std::endl;
        tree_->SetBranchAddress(trigger_names_.at(i).c_str(), &trig_counts_.at(i));
    }  

    //adding cut branch
    bool cut_;
    tree_->SetBranchAddress("Cut", &cut_);


    //storing trigger result 1,2,OR
    trigger_counter t_count;

    //cumulative and relative efficiencies
    std::vector<int> rel_eff1(filter_1.get_trigger_length(),0);
    std::vector<int> rel_eff2(filter_2.get_trigger_length(),0);
    std::vector<int> cum_eff1(filter_1.get_trigger_length(),0);
    std::vector<int> cum_eff2(filter_2.get_trigger_length(),0);

    int cut_count = 0;

    for(int ev_id = 0; ev_id < tree_->GetEntries(); ev_id++){
    //for(int ev_id = 0; ev_id < 10; ev_id++){

        tree_->GetEntry(ev_id);
        if(cut_){

            std::vector<int> c_1_trig_ = config_counts(counts_, fil_ind_1);
            std::vector<int> c_2_trig_ = config_counts(counts_, fil_ind_2);
            //std::vector<int> c_1_trig_(counts_.begin(), counts_.begin()+5);
            //std::vector<int> c_2_trig_(counts_.begin()+5, counts_.end());

            //adding filter
            c_2_trig_.push_back(counts_.at(4));

            filter_1.initialize_filters_id();
            filter_2.initialize_filters_id();

            //auto r_1 = filter_1.filter_test(c_1_trig_);
            //auto r_2 = filter_2.filter_test(c_2_trig_);

            auto r_1 = filter_1.f_test_suppose_sequential(c_1_trig_);
            auto r_2 = filter_2.f_test_suppose_sequential(c_2_trig_);

            //std::cout << "Trig one res: " << filter_1.is_trigger_passed() << " " << "Trig two res: " << filter_2.is_trigger_passed() << std::endl;
            //std::cout << "-------------------------------------" << std::endl;

            if(filter_1.is_trigger_passed())
                t_count.trig1_ ++;

            if(filter_2.is_trigger_passed())
                t_count.trig2_ ++;

            if(filter_1.is_trigger_passed() || filter_2.is_trigger_passed())
                t_count.trig_or_ ++;

            
            auto relative_efficiency = filter_1.get_relative_eff();
            std::transform (rel_eff1.begin(), rel_eff1.end(), relative_efficiency.begin(), rel_eff1.begin(), std::plus<int>());

            relative_efficiency = filter_2.get_relative_eff();
            std::transform (rel_eff2.begin(), rel_eff2.end(), relative_efficiency.begin(), rel_eff2.begin(), std::plus<int>());

            auto cumulative_efficiency = filter_1.get_cumulative_eff();
            std::transform (cum_eff1.begin(), cum_eff1.end(), cumulative_efficiency.begin(), cum_eff1.begin(), std::plus<int>());

            cumulative_efficiency = filter_2.get_cumulative_eff();
            std::transform (cum_eff2.begin(), cum_eff2.end(), cumulative_efficiency.begin(), cum_eff2.begin(), std::plus<int>());



        }
        else
            cut_count++;
    }

    std::vector<double> tot_relative_eff_1 = compute_total_relative_eff(rel_eff1, tree_->GetEntries(), tree_->GetEntries()-cut_count);
    std::vector<double> tot_relative_eff_2 = compute_total_relative_eff(rel_eff2, tree_->GetEntries(), tree_->GetEntries()-cut_count);
    std::vector<double> tot_cumulative_eff_1 = compute_total_cumulative_eff(cum_eff1, tree_->GetEntries(), tree_->GetEntries()-cut_count);
    std::vector<double> tot_cumulative_eff_2 = compute_total_cumulative_eff(cum_eff2, tree_->GetEntries(), tree_->GetEntries()-cut_count);

    //output results
    std::string * f1_name = filter_1.get_fil_names();
    std::string * f2_name = filter_2.get_fil_names();
    
    std::cout << "Total events: " << tree_->GetEntries() << " " << "After kin cuts: " << tree_->GetEntries() - cut_count << std::endl;
    std::cout << " " << std::endl;
    std::cout << "Trigger 1 ---- " << trigger_names_.at(0) <<  std::endl;
    for(int i = 0; i < tot_relative_eff_1.size(); i++){
        if(i == 0)
            std::cout << "Ratio from cuts: " << tot_relative_eff_1.at(i) << std::endl;
        
        else if(i == 1){
            
            std::cout << "Filter: " << *f1_name;//<< " | " << "Cumulative eff: " << cum_eff1.at(i) << std::endl;
            f1_name++;
            std::cout << " OR " << *f1_name << " | " << "Relative eff: " << tot_relative_eff_1.at(i) << " | Cumulative eff:" << tot_cumulative_eff_1.at(i) << std::endl;
            f1_name++;
        }
        
        else{
            std::cout << "Filter: " << *f1_name << " | " << "Relative eff: " << tot_relative_eff_1.at(i) << " | Cumulative eff: " << tot_cumulative_eff_1.at(i) <<  std::endl;
            f1_name++;
        }
    }

    std::cout << "Trigger 2 ---- " << trigger_names_.at(1) <<  std::endl;
    for(int i = 0; i < tot_relative_eff_2.size(); i++){
        if(i == 0)
            std::cout << "Ratio from cuts: " << tot_relative_eff_2.at(i) << std::endl;
        else{
        std::cout << "Filter: " << *f2_name << " | " << "Relative eff: " << tot_relative_eff_2.at(i) << " | Cumulative eff: " << tot_cumulative_eff_2.at(i) << std::endl;// " | " << "Cumulative eff: " << cum_eff2.at(i) << std::endl;
        f2_name++;
        }
    }

    std::cout << "---------------------" << std::endl;
    std::cout << "Total events: " << tree_->GetEntries() << std::endl;
    std::cout << "Total events post cuts: " << tree_->GetEntries() - cut_count << std::endl;
    std::cout << "Trigger 1 counts: " << t_count.trig1_ << " | Pass/Total_after_cut: " << (double) (100*t_count.trig1_)/(tree_->GetEntries() - cut_count) <<"%"<< std::endl;
    std::cout << "Trigger 2 counts: " << t_count.trig2_ <<  " | Pass/Total_after_cut: " << (double) (100*t_count.trig2_)/(tree_->GetEntries() - cut_count) << "%" << std::endl;
    std::cout << "Trigger OR counts: " << t_count.trig_or_ << " | Pass/Total_after_cut: " << (double) (100*t_count.trig_or_)/(tree_->GetEntries() - cut_count) << "%"<< std::endl;

    TH1F* h = new TH1F("h", "Relative Efficiency Trigger: HLT_QuadJet45_TripleBTagCSV_p087_v6", tot_relative_eff_1.size(), 0, tot_relative_eff_1.size()); 
    h->GetXaxis()->SetTitle("Filter index");
    h->GetYaxis()->SetTitle("Relative Efficiency (%)");
    h->SetFillStyle(3003);
    h->SetFillColor(kAzure+3);
    h->SetLineColor(kAzure+3);
    h->SetLineWidth(2);
    
    TH1F* h2 = new TH1F("h", "Relative Efficiency Trigger: HLT_DoubleJet90_Double30_TripleBTagCSV_p087_v5", tot_relative_eff_2.size(), 0, tot_relative_eff_2.size()); 
    h2->GetXaxis()->SetTitle("Filter index");
    h2->GetYaxis()->SetTitle("Relative Efficiency (%)");
    h2->SetFillStyle(3003);
    h2->SetFillColor(kAzure+2);
    h2->SetLineColor(kAzure+3);
    h2->SetLineWidth(2);

    gStyle->SetOptStat(0);
    //filling histos
    for(int i = 1; i < tot_relative_eff_1.size()+1; i++){
        h->SetBinContent(i, tot_relative_eff_1.at(i-1)*100);
    }

    for(int i = 1; i < tot_relative_eff_2.size()+1; i++){
        h2->SetBinContent(i, tot_relative_eff_2.at(i-1)*100);
    }

    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 800);
    h->Draw("hist");
    c->Draw();
    c->SaveAs("./trig2_rel_eff.pdf");

    TCanvas* c2 = new TCanvas("c2", "c2", 1000, 1000, 1000, 800);
    h2->Draw("hist");
    c2->Draw();
    c2->SaveAs("./trig_2_rel_eff.pdf");





  
    return 0;
}