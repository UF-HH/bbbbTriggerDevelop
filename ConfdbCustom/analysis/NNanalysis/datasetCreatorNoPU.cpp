//c++ -o tmp datasetCreatorNoPU.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
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

    TFile* fr = new TFile("../../test/datasets/323725_37_57_PU50.root");
    TFile* fgen = new TFile("../../../../../../CMSSW_11_0_0_patch1/src/ggHHRaw/ggHH/test/roots/ggHLT_RAWAOD_GenRECO.root");
    std::string branch = "SaveAllJets/Jets";
    std::string genbranch = "SaveGenHH/Gen";

    TFile fout("NNdata_noPU_323725_37_57_PU50.root", "RECREATE");
    TTree* tout = new TTree("data", "data");

    double lead_pt;
    double lead_mass;
    double lead_eta;
    double lead_phi;
    double lead_btag;
    double lead2_pt;
    double lead2_mass;
    double lead2_eta;
    double lead2_phi;
    double lead2_btag;
    double lead3_pt;
    double lead3_mass;
    double lead3_eta;
    double lead3_phi;
    double lead3_btag;
    double lead4_pt;
    double lead4_mass;
    double lead4_eta;
    double lead4_phi;
    double lead4_btag;
    double leadb;
    double secb;
    double thirdb;
    double fourthb;
    int type;
    std::string JetsType;

    tout->Branch("leading_pt", &lead_pt);
    tout->Branch("leading_mass", &lead_mass);
    tout->Branch("leading_eta", &lead_eta);
    tout->Branch("leading_phi", &lead_phi);
    tout->Branch("leading_btag", &lead_btag);
    tout->Branch("2leading_pt", &lead2_pt);
    tout->Branch("2leading_mass", &lead2_mass);
    tout->Branch("2leading_eta", &lead2_eta);
    tout->Branch("2leading_phi", &lead2_phi);
    tout->Branch("2leading_btag", &lead2_btag);
    tout->Branch("3leading_pt", &lead3_pt);
    tout->Branch("3leading_mass", &lead3_mass);
    tout->Branch("3leading_eta", &lead3_eta);
    tout->Branch("3leading_phi", &lead3_phi);
    tout->Branch("3leading_btag", &lead3_btag);
    tout->Branch("4leading_pt", &lead4_pt);
    tout->Branch("4leading_mass", &lead4_mass);
    tout->Branch("4leading_eta", &lead4_eta);
    tout->Branch("4leading_phi", &lead4_phi);
    tout->Branch("4leading_btag", &lead4_btag);
    tout->Branch("1_btag", &leadb);
    tout->Branch("2_btag", &secb);
    tout->Branch("3_btag", &thirdb);
    tout->Branch("4_btag", &fourthb);
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
    
    genev.SetStart(0);
    std::cout << "[INFO] Building signal..." << std::endl;
    for(int i = 0; i < bal_ev; i++){

        std::vector<double> btags;
        
        genev.Generate();

        //PfJets
        hltObj::Jets pfj = genev.GetPFJetsCollection();
        JetsType = "PF";

        std::vector<double> sorted_pts = pfj.pt;
        std::sort(sorted_pts.begin(), sorted_pts.end());

        //leading pt
        lead_pt = sorted_pts.at(sorted_pts.size()-1);
        auto it = std::find(pfj.pt.begin(), pfj.pt.end(), lead_pt);
        int lead_idx = std::distance(pfj.pt.begin(), it);
        lead_mass = pfj.mass.at(lead_idx);
        lead_eta = pfj.eta.at(lead_idx);
        lead_phi = pfj.phi.at(lead_idx);
        lead_btag = pfj.btag.at(lead_idx);

        //2ndleading pt
        lead2_pt = sorted_pts.at(sorted_pts.size()-2);
        it = std::find(pfj.pt.begin(), pfj.pt.end(), lead2_pt);
        int lead2_idx = std::distance(pfj.pt.begin(), it);
        lead2_mass = pfj.mass.at(lead2_idx);
        lead2_eta = pfj.eta.at(lead2_idx);
        lead2_phi = pfj.phi.at(lead2_idx);
        lead2_btag = pfj.btag.at(lead2_idx);

        //3rdleading pt
        lead3_pt = sorted_pts.at(sorted_pts.size()-3);
        it = std::find(pfj.pt.begin(), pfj.pt.end(), lead3_pt);
        int lead3_idx = std::distance(pfj.pt.begin(), it);
        lead3_mass = pfj.mass.at(lead3_idx);
        lead3_eta = pfj.eta.at(lead3_idx);
        lead3_phi = pfj.phi.at(lead3_idx);
        lead3_btag = pfj.btag.at(lead3_idx);

        //4thleading pt
        lead4_pt = sorted_pts.at(sorted_pts.size()-4);
        it = std::find(pfj.pt.begin(), pfj.pt.end(), lead4_pt);
        int lead4_idx = std::distance(pfj.pt.begin(), it);
        lead4_mass = pfj.mass.at(lead4_idx);
        lead4_eta = pfj.eta.at(lead4_idx);
        lead4_phi = pfj.phi.at(lead4_idx);
        lead4_btag = pfj.btag.at(lead4_idx);

        

        btags = highest4btag(pfj, btags_cut);
        leadb = btags.at(0);
        secb = btags.at(1);
        thirdb = btags.at(2);
        fourthb = btags.at(3);


        tout->Fill();

        //CaloJets
        hltObj::Jets caloj = genev.GetCaloJetsCollection();
        JetsType = "CALO";

        sorted_pts = caloj.pt;
        std::sort(sorted_pts.begin(), sorted_pts.end());

        //leading pt
        lead_pt = sorted_pts.at(sorted_pts.size()-1);
        auto it2 = std::find(caloj.pt.begin(), caloj.pt.end(), lead_pt);
        lead_idx = std::distance(caloj.pt.begin(), it2);
        lead_mass = caloj.mass.at(lead_idx);
        lead_eta = caloj.eta.at(lead_idx);
        lead_phi = caloj.phi.at(lead_idx);
        lead_btag = caloj.btag.at(lead_idx);

        //2ndleading pt
        lead2_pt = sorted_pts.at(sorted_pts.size()-2);
        it2 = std::find(caloj.pt.begin(), caloj.pt.end(), lead2_pt);
        lead2_idx = std::distance(caloj.pt.begin(), it2);
        lead2_mass = caloj.mass.at(lead2_idx);
        lead2_eta = caloj.eta.at(lead2_idx);
        lead2_phi = caloj.phi.at(lead2_idx);
        lead2_btag = caloj.btag.at(lead2_idx);

        //3rdleading pt
        lead3_pt = sorted_pts.at(sorted_pts.size()-3);
        it2 = std::find(caloj.pt.begin(), caloj.pt.end(), lead3_pt);
        lead3_idx = std::distance(caloj.pt.begin(), it2);
        lead3_mass = caloj.mass.at(lead3_idx);
        lead3_eta = caloj.eta.at(lead3_idx);
        lead3_phi = caloj.phi.at(lead3_idx);
        lead3_btag = caloj.btag.at(lead3_idx);

        //4thleading pt
        lead4_pt = sorted_pts.at(sorted_pts.size()-4);
        it2 = std::find(caloj.pt.begin(), caloj.pt.end(), lead4_pt);
        lead4_idx = std::distance(caloj.pt.begin(), it2);
        lead4_mass = caloj.mass.at(lead4_idx);
        lead4_eta = caloj.eta.at(lead4_idx);
        lead4_phi = caloj.phi.at(lead4_idx);
        lead4_btag = caloj.btag.at(lead4_idx);
        
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

        std::vector<double> sorted_pts = pfj.pt;
        std::sort(sorted_pts.begin(), sorted_pts.end());

        //leading pt
        lead_pt = sorted_pts.at(sorted_pts.size()-1);
        auto it = std::find(pfj.pt.begin(), pfj.pt.end(), lead_pt);
        int lead_idx = std::distance(pfj.pt.begin(), it);
        lead_mass = pfj.mass.at(lead_idx);
        lead_eta = pfj.eta.at(lead_idx);
        lead_phi = pfj.phi.at(lead_idx);
        lead_btag = pfj.btag.at(lead_idx);

        //2ndleading pt
        lead2_pt = sorted_pts.at(sorted_pts.size()-2);
        it = std::find(pfj.pt.begin(), pfj.pt.end(), lead2_pt);
        int lead2_idx = std::distance(pfj.pt.begin(), it);
        lead2_mass = pfj.mass.at(lead2_idx);
        lead2_eta = pfj.eta.at(lead2_idx);
        lead2_phi = pfj.phi.at(lead2_idx);
        lead2_btag = pfj.btag.at(lead2_idx);

        //3rdleading pt
        lead3_pt = sorted_pts.at(sorted_pts.size()-3);
        it = std::find(pfj.pt.begin(), pfj.pt.end(), lead3_pt);
        int lead3_idx = std::distance(pfj.pt.begin(), it);
        lead3_mass = pfj.mass.at(lead3_idx);
        lead3_eta = pfj.eta.at(lead3_idx);
        lead3_phi = pfj.phi.at(lead3_idx);
        lead3_btag = pfj.btag.at(lead3_idx);

        //4thleading pt
        lead4_pt = sorted_pts.at(sorted_pts.size()-4);
        it = std::find(pfj.pt.begin(), pfj.pt.end(), lead4_pt);
        int lead4_idx = std::distance(pfj.pt.begin(), it);
        lead4_mass = pfj.mass.at(lead4_idx);
        lead4_eta = pfj.eta.at(lead4_idx);
        lead4_phi = pfj.phi.at(lead4_idx);
        lead4_btag = pfj.btag.at(lead4_idx);
        
        btags = highest4btag(pfj, btags_cut);
        leadb = btags.at(0);
        secb = btags.at(1);
        thirdb = btags.at(2);
        fourthb = btags.at(3);


        tout->Fill();

        //CaloJets
        hltObj::Jets caloj = bkgev.GetCaloJetsCollection();
        JetsType = "CALO";

        sorted_pts = caloj.pt;
        std::sort(sorted_pts.begin(), sorted_pts.end());

        //leading pt
        lead_pt = sorted_pts.at(sorted_pts.size()-1);
        auto it2 = std::find(caloj.pt.begin(), caloj.pt.end(), lead_pt);
        lead_idx = std::distance(caloj.pt.begin(), it2);
        lead_mass = caloj.mass.at(lead_idx);
        lead_eta = caloj.eta.at(lead_idx);
        lead_phi = caloj.phi.at(lead_idx);
        lead_btag = caloj.btag.at(lead_idx);

        //2ndleading pt
        lead2_pt = sorted_pts.at(sorted_pts.size()-2);
        it2 = std::find(caloj.pt.begin(), caloj.pt.end(), lead2_pt);
        lead2_idx = std::distance(caloj.pt.begin(), it2);
        lead2_mass = caloj.mass.at(lead2_idx);
        lead2_eta = caloj.eta.at(lead2_idx);
        lead2_phi = caloj.phi.at(lead2_idx);
        lead2_btag = caloj.btag.at(lead2_idx);

        //3rdleading pt
        lead3_pt = sorted_pts.at(sorted_pts.size()-3);
        it2 = std::find(caloj.pt.begin(), caloj.pt.end(), lead3_pt);
        lead3_idx = std::distance(caloj.pt.begin(), it2);
        lead3_mass = caloj.mass.at(lead3_idx);
        lead3_eta = caloj.eta.at(lead3_idx);
        lead3_phi = caloj.phi.at(lead3_idx);
        lead3_btag = caloj.btag.at(lead3_idx);

        //4thleading pt
        lead4_pt = sorted_pts.at(sorted_pts.size()-4);
        it2 = std::find(caloj.pt.begin(), caloj.pt.end(), lead4_pt);
        lead4_idx = std::distance(caloj.pt.begin(), it2);
        lead4_mass = caloj.mass.at(lead4_idx);
        lead4_eta = caloj.eta.at(lead4_idx);
        lead4_phi = caloj.phi.at(lead4_idx);
        lead4_btag = caloj.btag.at(lead4_idx);
        
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
