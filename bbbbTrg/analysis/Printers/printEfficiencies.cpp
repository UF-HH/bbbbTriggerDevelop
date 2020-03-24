//c++ -o print printEfficiencies.cpp trigger_manager/trigger_manager.cc `root-config --glibs --cflags`
// ./print 2018 data/data_general_jets_2018.root 

#include "../bbbb_lambda/make_trigger_paths.h"
#include "../trigger_manager/trigger_manager.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main(int argc, char* argv[]){

    if(argc != 3){
        std::cerr<< "Only two arguments: year, filein" << std::endl;
        throw std::runtime_error(("Found " + std::to_string(argc-1) + " Input arguments while two is required").c_str());
    }

    int year = std::atoi(argv[1]);
    std::string in_file = argv[2];

    bool triggers_or = false;
    std::vector<std::vector<bool> > trig_or_count;
    if(year == 2016){
        triggers_or = true;
    }

    make_trigger_paths paths(year);

    std::vector<std::string> trig_names = paths.get_trig_name();
    trig_or_count.resize(trig_names.size());

    for(int trig = 0; trig < trig_names.size(); trig++){

        std::cout << "[INFO]: Trigger: " << trig_names.at(trig) << std::endl;
        std::vector<std::pair<std::string,int>> fil_map = paths.get_map().at(trig);

        //removing b-tag filters, only if u want this!!!!!
        //fil_map.erase(fil_map.begin()+11);
        //fil_map.erase(fil_map.begin()+10);
        //fil_map.erase(fil_map.begin()+3);
        //-------------------------------------------------

        man_filter* filter = new man_filter(fil_map , trig_names.at(trig));

        

        filter->set_OR(paths.get_OR().at(trig));

        TFile* f = new TFile(in_file.c_str());
        TTree *t1 = (TTree*)f->Get("checkTrg/trgObjTree");

        std::vector<int> counts_(fil_map.size(), 0);
        std::vector<int> trig_counts_(trig_names.size(), 0);

        for(int  i = 0; i < fil_map.size(); i++){
            //std::cout << "READING FILTER BRANCH: " << fil_map.at(i).first << std::endl;
            t1->SetBranchAddress((fil_map.at(i).first).c_str(), &counts_.at(i));
        }

        for(int i = 0; i < trig_names.size(); i++){
            //std::cout << "READING TRIGGER BRANCH: " << trig_names.at(i) << std::endl;
            t1->SetBranchAddress(trig_names.at(i).c_str(), &trig_counts_.at(i));
        }  

        double var;
        int pass;
        bool cut;

        t1->SetBranchAddress(trig_names.at(trig).c_str(), &pass);

        std::vector<int> pass_fil_counts;
        pass_fil_counts.resize(filter->get_fil_names().size());


        for(Int_t i =0; i < t1->GetEntries(); i++){

                t1->GetEntry(i);
                auto r_1 = filter->f_test_suppose_sequential(counts_, false, 1);
                int stop = filter->verify_filter_stop();


                if(stop != -1){
                    if(triggers_or) trig_or_count.at(trig).push_back(false);
                    for(int j = 0; j < stop; j++){
                        pass_fil_counts.at(j)++;
                    }
                }
                else{
                    if(triggers_or) trig_or_count.at(trig).push_back(true);
                    for(int j = 0; j < pass_fil_counts.size(); j++){
                        pass_fil_counts.at(j)++;
                    }
                }
        }


        for(int i = 0; i < filter->get_fil_names().size(); i++){
            std::cout << "[INFO]: Filter  " << filter->get_fil_names().at(i) << " tot_count: " << t1->GetEntries() << " pass_count: " << pass_fil_counts.at(i) << " EFF: " << (double)pass_fil_counts.at(i)/t1->GetEntries() << std::endl;
        }


    }

    if(triggers_or){
        TFile* f = new TFile(in_file.c_str());
        TTree *t1 = (TTree*)f->Get("checkTrg/trgObjTree");

        int or_counter = 0;
        for(int i = 0; i < trig_or_count.at(0).size(); i++){
            if(trig_or_count.at(0).at(i) || trig_or_count.at(1).at(i)){
                or_counter++;
            }
        }
        std::cout << "[INFO]: Trigger OR count: " << or_counter << " EFF: " << (double) or_counter/t1->GetEntries() << std::endl;
    }



    return 0;
}
