//c++ -o rate Rate.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <valarray>
#include <fstream>
#include "../interface/Obj.h"
#include "../interface/TriggerMaker.h"
#include "TTree.h"

int main(int argc, char* argv[]){

    //Setting Ephemeral
    TFile* f = new TFile("../../test/datasets/323725_37_57_PU50.root");
    std::string branch = "SaveAllJets/Jets";

    //Retrieving total events
    TTree* tree = (TTree*)f->Get("MyHLTAnalyzer/trgObjTree");
    int Totevts_ = tree->GetEntries();

    Event ev(f, branch);

    //Setting Signal
    TFile* f_sig = new TFile("../../../../../../CMSSW_11_0_0_patch1/src/ggHHRaw/ggHH/test/ggMCRAW.root");
    TTree* treeSig_ = (TTree*)f_sig->Get("MyHLTAnalyzer/trgObjTree");
    int TotEvtsSig_ = treeSig_->GetEntries();

    Event evsig(f_sig, branch);

    //HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3
    //Building it as default and adding modules in defined positions
    TriggerMaker Trig2018; //deafult constructor = 2018 configuration
    Trig2018.SkipL1Seed = true; //skipping L1 seed 

    hltObj::HLTCuts hltBTagCaloDeepCSVp17Double;
    hltBTagCaloDeepCSVp17Double.MinN = 2;
    hltBTagCaloDeepCSVp17Double.EtaMin = -2.4;
    hltBTagCaloDeepCSVp17Double.EtaMax = 2.4;
    hltBTagCaloDeepCSVp17Double.PtMin = 30; 
    hltBTagCaloDeepCSVp17Double.PtMax = 1E99; 
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

    std::string json_file (argv[1]);
    Trig2018.CutFromJson(json_file);
    
    //Computing Acceptance for Bkg and save it in RateBkg
    int entries = ev.GetEntries();

    std::ofstream ofile("RateBkg.txt" , std::ios::app);

    int Accepted = 0; 
    std::vector<int> Lumi;
    int Run;

    for(int i = 0; i < entries; i++){
        ev.Generate();
        int LS = ev.GetLS();
        Run = ev.GetRun();
        if(std::find(Lumi.begin(), Lumi.end(), LS) == Lumi.end()) Lumi.push_back(LS);

        //computing with b modules result
        auto r_1 = Trig2018.Sequence(ev);
        auto stop = Trig2018.FindFirstZero(r_1);
        
        if(stop == -1) Accepted++;
        
    }

    ofile << "Tot: " << Totevts_ << ", Accepted: " << Accepted << ", Run: " << Run << ", Lumis: ";
    for(auto l : Lumi){
        ofile << l << " ";
    }
    ofile << "\n";

    //Computing acceptance for signal
    std::ofstream ofilesig("RateSig.txt" , std::ios::app);

    Accepted = 0; 
    entries = evsig.GetEntries();

    for(int i = 0; i < entries; i++){
        evsig.Generate();

        //computing with b modules result
        auto r_1 = Trig2018.Sequence(evsig);
        auto stop = Trig2018.FindFirstZero(r_1);
        
        if(stop == -1) Accepted++;
        
    }

    std::cout << TotEvtsSig_ << " " << Accepted << std::endl;
    ofilesig << "Tot: " << TotEvtsSig_ << ", Accepted: " << Accepted << "\n";

    return 0;
            
}