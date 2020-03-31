//c++ -o prova ComputeEfficiencyFromJSON.cpp ../src/Event.cc ../src/TriggerMaker.cc `root-config --cflags --glibs`

#include <iostream>
#include <string.h>
#include "../interface/TriggerMaker.h"
#include <iostream>
#include <fstream>
#include "time.h"


using namespace hltObj;

int main(int argc, char* argv[]){

    int entries = 1000;

    if(argc == 1){
        std::cerr<< "invalid sets of input" << std::endl;
        throw std::runtime_error("few arguments passed");
    }

    if(argc == 3) entries = std::stoi(argv[2]);
    
    std::cout << "[INFO]: Setting up event " << std::endl;
    std::string kl_map    = "../data_weight/gg_HH_4B_SM_2018.root"; // sample map fname
    std::string kl_histo  = "hhGenLevelDistr";           // sample map histo
    std::string kl_coeffs = "../data_weight/coefficientsByBin_extended_3M_costHHSim_19-4.txt"; // coefficient file

    //std::ofstream ofile;
    std::ofstream ofile("Scan_HT_Eta.txt" , std::ios::app);
    //ofile.open ("Scan_HT_Eta.txt");

    TFile* f = new TFile("../../test/filter_tests_2018_nob.root");
    std::string branch = "checkTrg/trgObjTree";
    TriggerMaker Trig2018NoB;
    
    Event ev(f, branch);
    ev.WeightFactory(kl_map, kl_histo, kl_coeffs);

    std::cout << "[INFO]: Reading Config " << std::endl;

    std::string line;
    std::string json_file (argv[1]);
    Trig2018NoB.CutFromJson(json_file);

    std::vector<float> klambdas = {-1, 0, 1, 2, 3, 4, 5};

    //from here we read the config file and we modify the trigger configuration

    for(auto kl : klambdas){

        std::cout << "kl: " << kl << std::endl;
        double weight_sum =0;
        int count = 0;  
        ev.SetStart(0);
        double counter = 0;

        for(int i = 0; i < entries; i++){
            ev.Generate();
            ev.compute_weight(kl);
            weight_sum += ev.weight;
            auto r_1 = Trig2018NoB.Sequence(ev);
            int stop = Trig2018NoB.FindFirstZero(r_1);
            if(stop == -1) counter+= ev.weight;
        }

        ofile << "kl: " << kl << ", Eff: " << (double)counter/weight_sum << "\n" ;
    }



    return 0;
}
     


    
    
