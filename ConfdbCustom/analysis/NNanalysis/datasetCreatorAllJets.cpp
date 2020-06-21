//c++ -o tmp datasetCreatorAllJets.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <valarray>
#include "../interface/Obj.h"
#include "../interface/TriggerMaker.h"
#include "TTree.h"

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

    TFile* fr = new TFile("../../test/datasets/324997.root");
    TFile* fgen = new TFile("../../../../../../CMSSW_11_0_0_patch1/src/ggHHRaw/ggHH/test/roots/ggHLT_RAWAOD_GenRECO.root");
    std::string branch = "SaveAllJets/Jets";
    std::string genbranch = "SaveGenHH/Gen";

    TFile fout("AllJets_324997.root", "RECREATE");
    TTree* tout = new TTree("data", "data");

    std::vector<double>* calo_pt = new std::vector<double>; 
    std::vector<double>* calo_eta = new std::vector<double>; 
    std::vector<double>* calo_mass = new std::vector<double>; 
    std::vector<double>* calo_phi = new std::vector<double>; 
    std::vector<double>* calo_energy = new std::vector<double>;
    std::vector<double>* calo_et = new std::vector<double>;
    std::vector<double>* calo_btag = new std::vector<double>;

    std::vector<double>* pf_pt = new std::vector<double>; 
    std::vector<double>* pf_eta = new std::vector<double>; 
    std::vector<double>* pf_mass = new std::vector<double>; 
    std::vector<double>* pf_phi = new std::vector<double>; 
    std::vector<double>* pf_energy = new std::vector<double>;
    std::vector<double>* pf_et = new std::vector<double>;
    std::vector<double>* pf_btag = new std::vector<double>;

    int type;
    std::string JetsType;

    tout->Branch("calo_pt", &calo_pt);
    tout->Branch("calo_eta", &calo_eta);
    tout->Branch("calo_phi", &calo_phi);
    tout->Branch("calo_energy", &calo_energy);
    tout->Branch("calo_mass", &calo_mass);
    tout->Branch("calo_et", &calo_et);
    tout->Branch("calo_btag", &calo_btag);
    tout->Branch("pf_pt", &pf_pt);
    tout->Branch("pf_eta", &pf_eta);
    tout->Branch("pf_phi", &pf_phi);
    tout->Branch("pf_energy", &pf_energy);
    tout->Branch("pf_mass", &pf_mass);
    tout->Branch("pf_et", &pf_et);
    tout->Branch("pf_btag", &pf_btag);
    tout->Branch("JetsType", &JetsType); //calo or PF
    tout->Branch("class", &type);

    hltObj::HLTCuts btags_cut; //default = all
    

    //setting up the events
    Event genev(fgen, branch, genbranch);
    Event bkgev(fr, branch);

    int bal_ev = std::min(genev.GetEntries(), bkgev.GetEntries());

    std::cout << "Events for bkg and signal: " << bal_ev << std::endl;
    
    type = 1;
    //building signal
    
    genev.SetStart(11439);
    std::cout << "[INFO] Building signal..." << std::endl;
    for(int i = 0; i < bal_ev; i++){

        std::vector<double> btags;
        
        genev.Generate();

        //PfJets
        hltObj::Jets pfj = genev.GetPFJetsCollection();
        JetsType = "PF";

        pf_pt = &pfj.pt;
        pf_eta = &pfj.eta;
        pf_phi = &pfj.phi;
        pf_mass = &pfj.mass;
        pf_energy = &pfj.e;
        pf_et = &pfj.et;
        pf_btag = &pfj.btag;



        tout->Fill();

        //CaloJets
        hltObj::Jets caloj = genev.GetCaloJetsCollection();
        JetsType = "CALO";

        calo_pt = &caloj.pt;
        calo_eta = &caloj.eta;
        calo_phi = &caloj.phi;
        calo_mass = &caloj.mass;
        calo_energy = &caloj.e;
        calo_et = &caloj.et;
        calo_btag = &caloj.btag;

        

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

        pf_pt = &pfj.pt;
        pf_eta = &pfj.eta;
        pf_phi = &pfj.phi;
        pf_mass = &pfj.mass;
        pf_energy = &pfj.e;
        pf_et = &pfj.et;
        pf_btag = &pfj.btag;

        tout->Fill();

        //CaloJets
        hltObj::Jets caloj = bkgev.GetCaloJetsCollection();
        JetsType = "CALO";

        calo_pt = &caloj.pt;
        calo_eta = &caloj.eta;
        calo_phi = &caloj.phi;
        calo_mass = &caloj.mass;
        calo_energy = &caloj.e;
        calo_et = &caloj.et;
        calo_btag = &caloj.btag;

        tout->Fill();

    }


    fout.Write();

    return 0;


}
