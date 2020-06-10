//c++ -o eff TriggerEff.cpp ../src/TriggerMaker.cc ../src/Event.cc `root-config --cflags --glibs`
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

double invMass(hltObj::Jet* j1, hltObj::Jet* j2){

    double theta1 = 2*atan(exp(-j1->eta));
    double theta2 = 2*atan(exp(-j2->eta));

    
    double p1 = j1->pt / sin(theta1);
    double p2 = j2->pt / sin(theta2);

    double e1  = j1->e;
    double e2  = j2->e; 
    
    double dpx_2 = pow(p1*sin(theta1)*sin(j1->phi) + p2*sin(theta2)*sin(j2->phi), 2);
    double dpy_2 = pow(p1*sin(theta1)*cos(j1->phi) + p2*sin(theta2)*cos(j2->phi), 2);
    double dpz_2 = pow(p1*cos(theta1) + p2*cos(theta2), 2);
    
    return sqrt(pow(e1 + e2, 2) -(dpx_2 + dpy_2 + dpz_2));

};

int main(int argc, char* argv[]){

    //Cutting on PF Objects, save only positive energy
    hltObj::KinCuts PFCut;
    PFCut.Type = "PFJets";
    PFCut.MinPt = 0.0;
    PFCut.MinE = 0.0;

    //Setting Signal
    TFile* f_sig = new TFile("../../../../../../CMSSW_11_0_0_patch1/src/ggHHRaw/ggHH/test/roots/prova_PS_btagreco.root");
    TTree* treeSig_ = (TTree*)f_sig->Get("MyHLTAnalyzer/trgObjTree"); //Trigger bits
    int TotEvtsSig_ = treeSig_->GetEntries(); //real total events processed

    std::string jet_branch = "SaveAllJets/Jets"; //save jets branches 
    std::string gen_branch = "SaveGenHH/Gen";

    Event evsig(f_sig, jet_branch, gen_branch);

    evsig.AddCut(PFCut);

    int entries = evsig.GetEntries();

    //Building trigger

    //std::string option = "l1";
    TriggerMaker Trig2018(option); //deafult constructor = 2018 configuration
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

    for(int i = 0; i < entries; i++){

        ev.Generate();
        ev.UnpackCollections();
        ev.bMatch(0.4, "GenJets");
        ev.jetMatch(0.2, "BMatchedJets", "RecoJets");

        std::vector<hltObj::Jet*> gen_bmatch = ev.GetMatchJetsToB(); //specific for pairing
        std::vector<hltObj::Jet*> reco = ev.GetRecoJets(); //specific for pairing gen/PF

        if(gen_bmatch.size() > 4){
            std::cerr << "non è possibile qvesto" << std::endl;
            throw std::runtime_error("nope");
        }

        //we need 4 gen jets matched to b quarks
        if(gen_bmatch.size() == 4){
            //then we need 4 pf jets matched to this jets
            if(std::count_if(reco.begin(), reco.end(), [](const hltObj::Jet* j){return j->matched == true;}) == gen_bmatch.size()){



    return 0;

}


