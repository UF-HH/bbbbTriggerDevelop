//c++ -o tmp TMprinter.cpp ../src/TriggerMaker.cc `root-config --cflags --glibs`

//------------------------------------
// ** Author: Giacomo Boldrini 2020
// ** TMprinter.cpp
//
// ** Programs that prints the efficiencies 
//    for each filter using the TriggerMaker 
//    Class with hand cut on kin variables
//
// ** Reference values for 2018 trigger 
//    without b tagging modules:
//    0.697499
//    0.528253
//    0.448039
//    0.42243
//    0.421162
//    0.417912
//    0.406156
//    0.355946
//    0.340435    
//------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <valarray>
#include "../interface/Obj.h"
#include "../interface/TriggerMaker.h"

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

    TFile* f = new TFile("../../test/filter_tests_2018_nob.root");
    std::string branch = "checkTrg/trgObjTree";

    Event ev(f, branch);
    ev.WeightFactory(kl_map, kl_histo, kl_coeffs);

    std::vector<int> results;
    results.resize(9);

    TriggerMaker Trig2018NoB; //deafult constructor = 2018 configuration
    int entries = ev.GetEntries();
    //int entries = 1000;

    int check = 0;

    for(int i = 0; i < entries; i++){
        ev.Generate();
        auto r_1 = Trig2018NoB.Sequence(ev);
        int stop = Trig2018NoB.FindFirstZero(r_1);
    
        int j = 0;
        auto it = results.begin();
        while(j != stop && it != results.end()){
            results.at(j)++;
            j++;
            it++;
        }
        if(ev.GetL1HT() >= 280){
            check++;
        }
        
    }

    for(auto c : results){
        std::cout << (double)c/entries << std::endl;
    }

    std::cout << "check: " << (double)check/entries << std::endl;

    return 0;
            
}