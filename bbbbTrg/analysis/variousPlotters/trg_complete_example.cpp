//c++ -o trig_eff_w trg_complete_example.cpp ../trigger_manager/trigger_manager.cc `root-config --cflags --glibs`


#include "../trigger_manager/trigger_manager.h"
#include "make_trigger_paths.h"
#include <TTree.h>
#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TStyle.h>
#include "TROOT.h"
#include "TGraph.h"
#include "TString.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>


std::vector<double> Mult(std::vector<double> eff, double weight){
    std::vector<double> res;
    res.resize(eff.size());
    for(int i = 0; i < res.size(); i++){
        res.at(i) = weight*eff.at(i);
    }

    return res;
}

void print(man_filter filter, std::vector<std::vector<double>> all_eff, std::vector<double> klambdas, int year){

    std::vector<std::string> names_= filter.get_fil_names();
    std::vector<double> filter_eff_values(klambdas.size(), 0);
    TString gname;
    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 800);
    TLegend* leg = new TLegend(.9, .9, .2, .7);

    gStyle->SetPalette(kRainBow);

    for(int j = 0; j < filter.get_trigger_length(); j++){
        for(int i = 0; i < klambdas.size(); i++){               
            filter_eff_values.at(i) = all_eff.at(i).at(j);
        }
        gname.Form("graph%d", j);
        std::string name = Form("c%d",j);
        TGraph *gname = new TGraph(filter_eff_values.size(), &(klambdas[0]), &(filter_eff_values[0]));
        gname->SetTitle(names_.at(j).c_str());
        gname->SetMarkerStyle(8);
        gname->SetMarkerSize(0);
        gname->GetHistogram()->SetMaximum(1.3);         
        gname->GetHistogram()->SetMinimum(0.); 
        leg->AddEntry(gname, names_.at(j).c_str());
        if(j == 0){
            gname->GetXaxis()->SetTitle("#lambda");
            gname->GetYaxis()->SetTitle("Filter Efficiency");
            std::string title = "Filters efficiency, Trigger path " + std::to_string(year) + ":"  + filter.get_trigger_name();
            gname->SetTitle(title.c_str());
            gname->Draw("AC PLC PFC");
        }
        else
        {
            gname->Draw("C PLC PFC");
        }
            
    }

    leg->Draw();
    c->Draw();
    std::string save_name = "plots/" + filter.get_trigger_name() + "_"+ std::to_string(year) + ".pdf";
    c->SaveAs(save_name.c_str());

    return;
}


int main(int argc, char*argv[]){

    if(argc != 2){
        std::cerr << "Only integer year must be insert" << std::endl;
        throw std::runtime_error("Too much input arguments or year is missing");
    }

    int year = std::atoi(argv[1]);
    make_trigger_paths m(year);

    //initialize maps
    std::vector<std::vector<std::pair<std::string,int>>> trig_path = m.get_map();
    std::vector<std::string> trig_name = m.get_trig_name();
    std::vector<std::vector<int> > OR = m.get_OR();

    std::vector<double> klambdas(20);
    double start = -10.;
    double finish = 10.;
    double step = (double)(finish-start)/klambdas.size();
    for(int i = 0; i < klambdas.size(); i++){
        klambdas.at(i) = start+((double)i*step);
    }  


    for(int trig_n = 0; trig_n < trig_path.size(); trig_n ++){
        
        //READING TREE
        std::string file_name = "./data/weight_out_" + std::to_string(year) + ".root";
        //TFile* f = new TFile("./data/weight_out.root");
        TFile* f = new TFile(file_name.c_str());
        TTree* tree_ = (TTree*)f->Get("HH_gen_info");

        //reading weights
        std::vector<float>  klambdas_branch(klambdas.size());
        std::vector<std::string> klambdas_names(klambdas.size());
        
        for (uint ikl = 0; ikl < klambdas.size(); ++ikl)
        {
            float kl = klambdas.at(ikl);
            std::stringstream stream;
            stream << std::fixed << std::setprecision(4) << kl;
            std::string brname = stream.str();
            std::replace( brname.begin(), brname.end(), '-', 'm');
            std::replace( brname.begin(), brname.end(), '.', 'd');
            brname = std::string("HH_rew_") + brname;
            std::cout << "SETTING WEIGHT BRANCH " << brname << std::endl;
            tree_->SetBranchAddress(brname.c_str(), &klambdas_branch.at(ikl));
        }


        //initialize filter structure for the trigger selected
        auto or_conn = OR.at(trig_n);
        man_filter* filter = new man_filter(trig_path.at(trig_n), trig_name.at(trig_n));
        filter->initialize_filters_id();
        if(or_conn.size() != 0)
            filter->set_OR(or_conn);
        
        //reading only interesting branches
        std::vector<int> counts_(trig_path.at(trig_n).size(), 0);
        int trig_counts_;

        
        for(int  i = 0; i < trig_path.at(trig_n).size(); i++){
            std::cout << "READING FILTER BRANCH: " << trig_path.at(trig_n).at(i).first << std::endl;
            tree_->SetBranchAddress((trig_path.at(trig_n).at(i).first).c_str(), &counts_.at(i));
        }

        std::cout << "READING TRIGGER BRANCH: " << filter->get_trigger_name() << std::endl;
        tree_->SetBranchAddress(filter->get_trigger_name().c_str(), &trig_counts_);
        
        
        //adding cut branch
        bool cut_;
        tree_->SetBranchAddress("Cut", &cut_);

        double weight_sum = 0;
        bool is_weight = true;

        std::vector<std::vector<double>> all_eff;

        std::cout << "[INFO] STARTING ANALYSIS" << std::endl;
        
        for(int i = 0; i < klambdas.size(); i++){
            //storing trigger result 1,2,OR
            if(i%1 == 0)
                std::cout << "Lambda: " << i << std::endl;

            weight_sum = 0;
            filter->clear_save();

            for(int ev_id = 0; ev_id < tree_->GetEntries(); ev_id++){
            //for(int ev_id = 0; ev_id < 1000; ev_id++){
                tree_->GetEntry(ev_id);
                //if(cut_){
                
                double weight = klambdas_branch.at(i);
                if(!is_weight)
                    weight = 1;

                weight_sum+= weight;

                auto r_1 = filter->f_test_suppose_sequential(counts_, true, weight);

            }

            auto final = filter->get_sum_vector();
            final = Mult(final, 1./weight_sum);
            std::cout << "PRINTING RESULTS: "  <<  std::endl;
            for(int bl = 0; bl < final.size(); bl++){
                std::cout << " New: " << final.at(bl) << std::endl;
            }

            
            all_eff.push_back(final);
        }

        std::cout << "ALL eff size. " << all_eff.size() << std::endl;
        print(*filter, all_eff, klambdas, year);

    }
    return 0;

}