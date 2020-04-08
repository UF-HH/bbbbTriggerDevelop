//c++ -o prova JsonEffRaw.cpp ../src/Event.cc ../src/TriggerMaker.cc `root-config --cflags --glibs`
//./prova ../TriggerCnfg/Trig2018.txt

#include <iostream>
#include <string.h>
#include "../interface/TriggerMaker.h"
#include <iostream>
#include <fstream>
#include "time.h"


using namespace hltObj;

int main(int argc, char* argv[]){

    TFile* f = new TFile("../../test/323727_3.root");
    std::string branch = "SaveAllJets/Jets";

    Event ev(f, branch);

    
    //HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3
    //Building it as default and adding modules in defined positions
    std::vector<int> results;
    results.resize(10);
    TriggerMaker Trig2018; //deafult constructor = 2018 configuration
    Trig2018.SkipL1Seed = true; //skipping L1 seed 

    hltObj::HLTCuts hltBTagCaloDeepCSVp17Double;
    hltBTagCaloDeepCSVp17Double.MinN = 2;
    hltBTagCaloDeepCSVp17Double.EtaMin = -2.4;
    hltBTagCaloDeepCSVp17Double.EtaMax = 2.4;
    hltBTagCaloDeepCSVp17Double.PtMin = 30; 
    hltBTagCaloDeepCSVp17Double.BtagMin = 0.17; //0.17

    hltObj::HLTCuts hltBTagPFDeepCSV4p5Triple;
    hltBTagPFDeepCSV4p5Triple.MinN = 3;
    hltBTagPFDeepCSV4p5Triple.EtaMin = -2.6;
    hltBTagPFDeepCSV4p5Triple.EtaMax = 2.6;
    hltBTagPFDeepCSV4p5Triple.PtMin = 30; 
    hltBTagPFDeepCSV4p5Triple.BtagMin = 0.24; //0.24

    //sintax: Name of new module, Type of selector, required objects to compute, HLTCuts object, position in the chain of filters
    Trig2018.InsertCut("hltBTagCaloDeepCSVp17Double", "CustomFixedBtagSelector", "CaloJets", hltBTagCaloDeepCSVp17Double, 2);
    //last btag is in last position so we just push it into the module
    Trig2018.PushCut("hltBTagPFDeepCSV4p5Triple", "CustomFixedBtagSelector", "PFJets", hltBTagPFDeepCSV4p5Triple);
    

    int entries = ev.GetEntries();

    //std::ofstream ofile;
    std::ofstream ofile("RawEff.txt" , std::ios::app);
    //ofile.open ("Scan_HT_Eta.txt");

    std::cout << "[INFO]: Reading Config " << std::endl;

    std::string line;
    std::string json_file (argv[1]);
    Trig2018.CutFromJson(json_file);

    //from here we read the config file and we modify the trigger configuration

    int count = 0;  
    ev.SetStart(0);
    double counter = 0;

    for(int i = 0; i < entries; i++){
        ev.Generate();
        auto r_1 = Trig2018.Sequence(ev);
        int stop = Trig2018.FindFirstZero(r_1);
        if(stop == -1) counter++;
    }

    ofile << "Total: " << entries << " Counts: " << counter << "\n" ;
    
    return 0;
}
     


    
    
