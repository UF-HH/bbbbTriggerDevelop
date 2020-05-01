//c++ -o tmp datasetCreator.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <valarray>
#include "../interface/Obj.h"
#include "../interface/TriggerMaker.h"
#include "TTree.h"

double HTfun(hltObj::Jets jets, hltObj::HLTCuts cuts){

    bool pass = true;
    std::vector<double> results;
    //local copy for lambda, can we actually avoid this?
    double PtMin = cuts.PtMin;
    double PtMax = cuts.PtMax;

    auto it = std::find_if(std::begin(jets.pt), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
    while (it != std::end(jets.pt)) {
        auto dis = std::distance(std::begin(jets.pt), it);
        if(jets.eta.at(dis) <= cuts.EtaMax && jets.eta.at(dis) >= cuts.EtaMin)
            results.emplace_back(jets.pt.at(dis));
        it = std::find_if(std::next(it), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
    }

    return std::accumulate(results.begin(), results.end(), 0.);

};

int Njetsfun(hltObj::Jets jets, hltObj::HLTCuts cuts){

    std::vector<double> results;
    //local copy for lambda, can we actually avoid this?
    double PtMin = cuts.PtMin;
    double PtMax = cuts.PtMax;

    auto it = std::find_if(std::begin(jets.pt), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
    while (it != std::end(jets.pt)) {
        auto dis = std::distance(std::begin(jets.pt), it);
        if(jets.eta.at(dis) <= cuts.EtaMax && jets.eta.at(dis) >= cuts.EtaMin && jets.btag.at(dis) >= cuts.BtagMin)
            results.emplace_back(jets.pt.at(dis));
        it = std::find_if(std::next(it), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
    }   
    
    return results.size();

};

std::vector<double> highest4btag(hltObj::Jets jets, hltObj::HLTCuts cuts){

    std::vector<double> results;
    //local copy for lambda, can we actually avoid this?
    double PtMin = cuts.PtMin;
    double PtMax = cuts.PtMax;

    auto it = std::find_if(std::begin(jets.pt), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
    while (it != std::end(jets.pt)) {
        auto dis = std::distance(std::begin(jets.pt), it);
        if(jets.eta.at(dis) <= cuts.EtaMax && jets.eta.at(dis) >= cuts.EtaMin){
            results.emplace_back(jets.btag.at(dis));
        }
        it = std::find_if(std::next(it), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
    }   

    std::sort(results.begin(), results.end());
    
    std::vector<double> res;
    res.push_back(results.at(results.size()-1));
    res.push_back(results.at(results.size()-2));
    res.push_back(results.at(results.size()-3));
    res.push_back(results.at(results.size()-4));

    return res;
};



int main(){

    TFile* fr = new TFile("../../test/datasets/323725_37_57_PU50.root");
    TFile* fgen = new TFile("../../../../../../CMSSW_11_0_0_patch1/src/ggHHRaw/ggHH/test/ggMCRAW_GenJets.root");
    std::string branch = "SaveAllJets/Jets";
    std::string genbranch = "SaveGenHH/Gen";

    TFile fout("NNdata.root", "RECREATE");
    TTree* tout = new TTree("data", "data");

    int NJets;
    int NJets_er2p5;
    int NJets_er2p4;
    int NJets_er2p1;
    double HT_er2p5;
    double HT_er2p4;
    double HT_er2p1;
    int NJets_PTgeq40;
    int NJets_PTgeq45;
    int NJets_PTgeq60;
    int NJets_PTgeq75;
    int NJets_PTgeq100;
    int NJets_PTgeq125;
    double leadb;
    double secb;
    double thirdb;
    double fourthb;
    int type;
    std::string JetsType;

    tout->Branch("NJets", &NJets);
    tout->Branch("NJets_er2p5", &NJets_er2p5);
    tout->Branch("NJets_er2p4", &NJets_er2p4);
    tout->Branch("NJets_er2p1", &NJets_er2p1);
    tout->Branch("HT_er2p5", &HT_er2p5);
    tout->Branch("HT_er2p4", &HT_er2p4);
    tout->Branch("HT_er2p1", &HT_er2p1);
    tout->Branch("NJets_PTgeq40", &NJets_PTgeq40);
    tout->Branch("NJets_PTgeq45", &NJets_PTgeq45);
    tout->Branch("NJets_PTgeq60", &NJets_PTgeq60);
    tout->Branch("NJets_PTgeq75", &NJets_PTgeq75);
    tout->Branch("NJets_PTgeq100", &NJets_PTgeq100);
    tout->Branch("NJets_PTgeq125", &NJets_PTgeq125);
    tout->Branch("leadBtag", &leadb);
    tout->Branch("secBtag", &secb);
    tout->Branch("thirdBtag", &thirdb);
    tout->Branch("fourthBtag", &fourthb);
    tout->Branch("JetsType", &JetsType); //calo or PF
    tout->Branch("class", &type);

    hltObj::HLTCuts NJets_cut; //default = all
    NJets_cut.PtMin = 30.;
 
    hltObj::HLTCuts NJets_er2p5_cut;
    NJets_er2p5_cut.EtaMin = -2.5;
    NJets_er2p5_cut.EtaMax = 2.5;
    NJets_er2p5_cut.PtMin = 30.;

    hltObj::HLTCuts NJets_er2p4_cut;
    NJets_er2p4_cut.EtaMin = -2.4;
    NJets_er2p4_cut.EtaMax = 2.4;
    NJets_er2p4_cut.PtMin = 30.;

    hltObj::HLTCuts NJets_er2p1_cut;
    NJets_er2p1_cut.EtaMin = -2.1;
    NJets_er2p1_cut.EtaMax = 2.1;
    NJets_er2p1_cut.PtMin = 30.;

    hltObj::HLTCuts HT_er2p5_cut;
    HT_er2p5_cut.EtaMin = -2.5;
    HT_er2p5_cut.EtaMax = 2.5;
    HT_er2p5_cut.PtMin = 30.;

    hltObj::HLTCuts HT_er2p4_cut;
    HT_er2p4_cut.EtaMin = -2.4;
    HT_er2p4_cut.EtaMax = 2.4;
    HT_er2p4_cut.PtMin = 30.;

    hltObj::HLTCuts HT_er2p1_cut;
    HT_er2p1_cut.EtaMin = -2.1;
    HT_er2p1_cut.EtaMax = 2.1;
    HT_er2p1_cut.PtMin = 30.;

    hltObj::HLTCuts NJets_PTgeq40_cut;
    NJets_PTgeq40_cut.EtaMin = -2.4;
    NJets_PTgeq40_cut.EtaMax = 2.4;
    NJets_PTgeq40_cut.PtMin = 40.; 

    hltObj::HLTCuts NJets_PTgeq45_cut;
    NJets_PTgeq45_cut.EtaMin = -2.4;
    NJets_PTgeq45_cut.EtaMax = 2.4;
    NJets_PTgeq45_cut.PtMin = 45.; 

    hltObj::HLTCuts NJets_PTgeq60_cut;
    NJets_PTgeq60_cut.EtaMin = -2.4;
    NJets_PTgeq60_cut.EtaMax = 2.4;
    NJets_PTgeq60_cut.PtMin = 60.; 

    hltObj::HLTCuts NJets_PTgeq75_cut;
    NJets_PTgeq75_cut.EtaMin = -2.4;
    NJets_PTgeq75_cut.EtaMax = 2.4;
    NJets_PTgeq75_cut.PtMin = 75.; 

    hltObj::HLTCuts NJets_PTgeq100_cut;
    NJets_PTgeq100_cut.EtaMin = -2.4;
    NJets_PTgeq100_cut.EtaMax = 2.4;
    NJets_PTgeq100_cut.PtMin = 100.; 

    hltObj::HLTCuts NJets_PTgeq125_cut;
    NJets_PTgeq125_cut.EtaMin = -2.4;
    NJets_PTgeq125_cut.EtaMax = 2.4;
    NJets_PTgeq125_cut.PtMin = 125.; 

    hltObj::HLTCuts btags_cut; //default = all
    

    //setting up the events
    Event genev(fgen, branch, genbranch);
    Event bkgev(fr, branch);

    int bal_ev = std::min(genev.GetEntries(), bkgev.GetEntries());

    std::cout << "Events for bkg and signal: " << bal_ev << std::endl;

    type = 1;
    //building signal
    std::cout << "[INFO] Building signal..." << std::endl;
    for(int i = 0; i < bal_ev; i++){

        std::vector<double> btags;
        
        genev.Generate();

        //PfJets
        hltObj::Jets pfj = genev.GetPFJetsCollection();
        JetsType = "PF";
        NJets = Njetsfun(pfj, NJets_cut);
        NJets_er2p5 = Njetsfun(pfj, NJets_er2p5_cut);
        NJets_er2p4 = Njetsfun(pfj, NJets_er2p4_cut);
        NJets_er2p1 = Njetsfun(pfj, NJets_er2p1_cut);
        HT_er2p5 = HTfun(pfj, HT_er2p5_cut);
        HT_er2p4 = HTfun(pfj, HT_er2p4_cut);
        HT_er2p1 = HTfun(pfj, HT_er2p1_cut);
        NJets_PTgeq40 = Njetsfun(pfj, NJets_PTgeq40_cut);
        NJets_PTgeq45 = Njetsfun(pfj, NJets_PTgeq45_cut);
        NJets_PTgeq60 = Njetsfun(pfj, NJets_PTgeq60_cut);
        NJets_PTgeq75 = Njetsfun(pfj, NJets_PTgeq75_cut);
        NJets_PTgeq100 = Njetsfun(pfj, NJets_PTgeq100_cut);
        btags = highest4btag(pfj, btags_cut);
        leadb = btags.at(0);
        secb = btags.at(1);
        thirdb = btags.at(2);
        fourthb = btags.at(3);


        tout->Fill();

        //CaloJets
        hltObj::Jets caloj = genev.GetCaloJetsCollection();
        JetsType = "CALO";
        NJets = Njetsfun(caloj, NJets_cut);
        NJets_er2p5 = Njetsfun(caloj, NJets_er2p5_cut);
        NJets_er2p4 = Njetsfun(caloj, NJets_er2p4_cut);
        NJets_er2p1 = Njetsfun(caloj, NJets_er2p1_cut);
        HT_er2p5 = HTfun(caloj, HT_er2p5_cut);
        HT_er2p4 = HTfun(caloj, HT_er2p4_cut);
        HT_er2p1 = HTfun(caloj, HT_er2p1_cut);
        NJets_PTgeq40 = Njetsfun(caloj, NJets_PTgeq40_cut);
        NJets_PTgeq45 = Njetsfun(caloj, NJets_PTgeq45_cut);
        NJets_PTgeq60 = Njetsfun(caloj, NJets_PTgeq60_cut);
        NJets_PTgeq75 = Njetsfun(caloj, NJets_PTgeq75_cut);
        NJets_PTgeq100 = Njetsfun(caloj, NJets_PTgeq100_cut);
        NJets_PTgeq125 = Njetsfun(caloj, NJets_PTgeq125_cut);
        btags = highest4btag(caloj, btags_cut);
        leadb = btags.at(0);
        secb = btags.at(1);
        thirdb = btags.at(2);
        fourthb = btags.at(3);

        tout->Fill();

    }

    std::cout << "[INFO] Building background..." << std::endl;
    type = 0;
    //building background
    for(int i = 0; i < bal_ev; i++){

        std::vector<double> btags;

        bkgev.Generate();

        //PfJets
        hltObj::Jets pfj = bkgev.GetPFJetsCollection();
        JetsType = "PF";
        NJets = Njetsfun(pfj, NJets_cut);
        NJets_er2p5 = Njetsfun(pfj, NJets_er2p5_cut);
        NJets_er2p4 = Njetsfun(pfj, NJets_er2p4_cut);
        NJets_er2p1 = Njetsfun(pfj, NJets_er2p1_cut);
        HT_er2p5 = HTfun(pfj, HT_er2p5_cut);
        HT_er2p4 = HTfun(pfj, HT_er2p4_cut);
        HT_er2p1 = HTfun(pfj, HT_er2p1_cut);
        NJets_PTgeq40 = Njetsfun(pfj, NJets_PTgeq40_cut);
        NJets_PTgeq45 = Njetsfun(pfj, NJets_PTgeq45_cut);
        NJets_PTgeq60 = Njetsfun(pfj, NJets_PTgeq60_cut);
        NJets_PTgeq75 = Njetsfun(pfj, NJets_PTgeq75_cut);
        NJets_PTgeq100 = Njetsfun(pfj, NJets_PTgeq100_cut);
        NJets_PTgeq125 = Njetsfun(pfj, NJets_PTgeq125_cut);
        btags = highest4btag(pfj, btags_cut);
        leadb = btags.at(0);
        secb = btags.at(1);
        thirdb = btags.at(2);
        fourthb = btags.at(3);


        tout->Fill();

        //CaloJets
        hltObj::Jets caloj = bkgev.GetCaloJetsCollection();
        JetsType = "CALO";
        NJets = Njetsfun(caloj, NJets_cut);
        NJets_er2p5 = Njetsfun(caloj, NJets_er2p5_cut);
        NJets_er2p4 = Njetsfun(caloj, NJets_er2p4_cut);
        NJets_er2p1 = Njetsfun(caloj, NJets_er2p1_cut);
        HT_er2p5 = HTfun(caloj, HT_er2p5_cut);
        HT_er2p4 = HTfun(caloj, HT_er2p4_cut);
        HT_er2p1 = HTfun(caloj, HT_er2p1_cut);
        NJets_PTgeq40 = Njetsfun(caloj, NJets_PTgeq40_cut);
        NJets_PTgeq45 = Njetsfun(caloj, NJets_PTgeq45_cut);
        NJets_PTgeq60 = Njetsfun(caloj, NJets_PTgeq60_cut);
        NJets_PTgeq75 = Njetsfun(caloj, NJets_PTgeq75_cut);
        NJets_PTgeq100 = Njetsfun(caloj, NJets_PTgeq100_cut);
        NJets_PTgeq125 = Njetsfun(caloj, NJets_PTgeq125_cut);
        btags = highest4btag(caloj, btags_cut);
        leadb = btags.at(0);
        secb = btags.at(1);
        thirdb = btags.at(2);
        fourthb = btags.at(3);

        tout->Fill();

    }


    fout.Write();

    return 0;


}