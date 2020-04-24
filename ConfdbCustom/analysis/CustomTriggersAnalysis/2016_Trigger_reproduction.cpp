//c++ -o 2016 2016_Trigger_reproduction.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
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

    //Building the 2016 trigger_1
    std::vector<std::string> F_types = {"CustomFixedPtEtaSelector", "CustomFixedBtagSelector", "CustomFixedPtEtaSelector"};
    std::vector<std::string> F_names = {"hltQuadCentralJet45", "hltBTagCaloCSVp087Triple", "hltQuadPFCentralJetLooseID45"};
    std::string TrigName2016 = "HLT_QuadJet45_TripleBTagCSV_p087_v6";

    TriggerMaker Trigger2016(TrigName2016, F_names, F_types);
    Trigger2016.SkipL1Seed = true;

    std::vector<std::string> HLT_Required = {"CaloJets", "CaloJets", "PFJets"};
    hltObj::HLTCuts hltQuadCentralJet45(45.0, 1.0E99, -2.6, 2.6, 4);
    hltObj::HLTCuts hltBTagCaloCSVp087Triple;
    hltBTagCaloCSVp087Triple.MinN = 3;
    hltBTagCaloCSVp087Triple.EtaMin = -2.6;
    hltBTagCaloCSVp087Triple.EtaMax = 2.6;
    hltBTagCaloCSVp087Triple.PtMin = 30; 
    hltBTagCaloCSVp087Triple.BtagMin = 0.1; //0.24
    hltObj::HLTCuts hltQuadPFCentralJetLooseID45(45.0, 1.0E99, -2.6, 2.6, 4);

    std::vector<hltObj::HLTCuts> HLT_Cuts = {hltQuadCentralJet45, hltBTagCaloCSVp087Triple, hltQuadPFCentralJetLooseID45};

    Trigger2016.SetHLTRequired(HLT_Required);
    Trigger2016.SetCuts(HLT_Cuts);

    //Building the 2016 trigger_2
    std::vector<std::string> F_types_2 = {"CustomFixedPtEtaSelector", "CustomFixedPtEtaSelector", "CustomFixedBtagSelector", "CustomFixedPtEtaSelector", "CustomFixedPtEtaSelector"};
    std::vector<std::string> F_names_2 = {"hltQuadCentralJet30", "hltDoubleCentralJet90", "hltBTagCaloCSVp087Triple", "hltQuadPFCentralJetLooseID30", "hltDoublePFCentralJetLooseID90"};
    std::string TrigName2016_2 = "HLT_DoubleJet90_Double30_TripleBTag";

    TriggerMaker Trigger2016_2(TrigName2016_2, F_names_2, F_types_2);
    Trigger2016_2.SkipL1Seed = true;

    std::vector<std::string> HLT_Required_2 = {"CaloJets", "CaloJets", "CaloJets", "PFJets", "PFJets"};
    hltObj::HLTCuts hltQuadCentralJet30(30.0, 1.0E99, -2.6, 2.6, 4);
    hltObj::HLTCuts hltDoubleCentralJet90(90.0, 1.0E99, -2.6, 2.6, 2);
    hltObj::HLTCuts hltQuadPFCentralJetLooseID30(30.0, 1.0E99, -2.6, 2.6, 4);
    hltObj::HLTCuts hltDoublePFCentralJetLooseID90(90.0, 1.0E99, -2.6, 2.6, 2);

    std::vector<hltObj::HLTCuts> HLT_Cuts_2 = {hltQuadCentralJet30, hltDoubleCentralJet90, hltBTagCaloCSVp087Triple, hltQuadPFCentralJetLooseID30, hltDoublePFCentralJetLooseID90};

    Trigger2016_2.SetHLTRequired(HLT_Required_2);
    Trigger2016_2.SetCuts(HLT_Cuts_2);


    //Computing Acceptance for Bkg and save it in RateBkg
    int entries = evsig.GetEntries();

    int Accepted1 = 0; 
    int Accepted2 = 0; 
    int AcceptedOR = 0;

    std::vector<int> results;
    results.resize(3);

    for(int i = 0; i < entries; i++){
        evsig.Generate();
        //computing with b modules result
        auto r_1 = Trigger2016.Sequence(evsig);
        auto stop = Trigger2016.FindFirstZero(r_1);
        if(stop == -1) Accepted1++;

        int j = 0;
        auto it = results.begin();
        while(j != stop && it != results.end()){
            results.at(j)++;
            j++;
            it++;
        }

        auto r_2 = Trigger2016_2.Sequence(evsig);
        auto stop_2 = Trigger2016_2.FindFirstZero(r_2);
        if(stop_2 == -1) Accepted2++;

        //compute or
        if(stop == -1 || stop_2 == -1) AcceptedOR++;
        
    }

    std::cout << "Results for signal" << std::endl;
    std::cout << Accepted1 << " " << Accepted2 << " " << AcceptedOR << std::endl;

    std::cout << "HLT_QuadJet45_TripleBTagCSV_p087_v6" << std::endl;
    for(int i = 0; i < results.size(); i++){
        std::cout << F_names.at(i) << " " << results.at(i) << std::endl;
    }


    //Computing Acceptance for Bkg and save it in RateBkg
    entries = ev.GetEntries();

    Accepted1 = 0; 
    Accepted2 = 0; 
    AcceptedOR = 0;


    for(int i = 0; i < entries; i++){
        ev.Generate();
        //computing with b modules result
        auto r_1 = Trigger2016.Sequence(ev);
        auto stop = Trigger2016.FindFirstZero(r_1);
        if(stop == -1) Accepted1++;

        auto r_2 = Trigger2016_2.Sequence(ev);
        auto stop_2 = Trigger2016_2.FindFirstZero(r_2);
        if(stop_2 == -1) Accepted2++;

        //compute or
        if(stop == -1 || stop_2 == -1) AcceptedOR++;
        
    }

    std::cout << "Results for Background" << std::endl;
    std::cout << Accepted1 << " " << Accepted2 << " " << AcceptedOR << std::endl;


    return 0;

}