//c++ -o tmp simplePrinter.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <valarray>
#include "../interface/Obj.h"
#include "../interface/TriggerMaker.h"
#include "TTree.h"

int main(){


    //TFile* f = new TFile("../../test/datasets/323725_37_57_PU50.root");
    TFile* f = new TFile("../../../../../../CMSSW_11_0_0_patch1/src/ggHHRaw/ggHH/test/ggMCRAW.root");
    std::string branch = "SaveAllJets/Jets";

    //retrieving infos from HLTAnalyzer which stores the online HLT decision
    std::vector<int> trigcount_;
    trigcount_.resize(2);
    TTree* tree = (TTree*)f->Get("MyHLTAnalyzer/trgObjTree");
    tree->SetBranchAddress("HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9", &trigcount_.at(0));
    tree->SetBranchAddress("HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3", &trigcount_.at(1));

    std::vector<int> finalcounts_;
    finalcounts_.resize(2);

    for(int i = 0; i < tree->GetEntries(); i++){
        tree->GetEntry(i);
        for(int j = 0; j < trigcount_.size(); j++){
            if(trigcount_.at(j)) finalcounts_.at(j)++;
        }
    }


    Event ev(f, branch);

    //HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9
    std::vector<int> results_nob;
    results_nob.resize(8);
    TriggerMaker Trig2018NoB; //deafult constructor = 2018 configuration
    Trig2018NoB.SkipL1Seed = true; //skipping L1 seed 

    
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


    for(int i = 0; i < entries; i++){
        ev.Generate();
        int LS = ev.GetLS();

        //computing nob result
        auto r_1 = Trig2018NoB.Sequence(ev);
        int stop = Trig2018NoB.FindFirstZero(r_1);
    
        int j = 0;
        auto it = results_nob.begin();
        while(j != stop && it != results_nob.end()){
            results_nob.at(j)++;
            j++;
            it++;
        }

        
        //computing with b modules result
        r_1 = Trig2018.Sequence(ev);
        stop = Trig2018.FindFirstZero(r_1);
        
        j = 0;
        auto it_ = results.begin();
        while(j != stop && it_ != results.end()){
            results.at(j)++;
            j++;
            it_++;
        }
        
    }

    std::cout << "...ONLINE RESULTS:..." << std::endl;
    std::cout << "....Events Processed: " << tree->GetEntries() << std::endl;
    std::cout << "....HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9: " << finalcounts_.at(0) << std::endl;
    std::cout << "....HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3: " << finalcounts_.at(1) << std::endl;
    std::cout << "....OFFILINE RESULTS:...." << std::endl;
    std::cout << "....Events Processed: " << entries << std::endl;
    std::cout << "....HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9: " << results_nob.at(results_nob.size()-1) << std::endl;
    std::cout << "....HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3: " << results.at(results.size()-1) << std::endl;

    std::cout << "...Results per filter:...." << std::endl;
    for(auto c : results){
        std::cout << c << std::endl;
    }
    return 0;
            
}
