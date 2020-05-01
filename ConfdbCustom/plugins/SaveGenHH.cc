#ifndef SAVE_GEN_H
#define SAVE_GEN_H


#include <vector>
#include <string>
#include <iostream>
#include <boost/bind.hpp>
#include "TLorentzVector.h"

#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TTree.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenStatusFlags.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "DataFormats/L1TGlobal/interface/GlobalObjectMapFwd.h"
#include "DataFormats/L1TGlobal/interface/GlobalObjectMap.h"
#include "DataFormats/L1TGlobal/interface/GlobalObjectMapRecord.h"
#include "DataFormats/L1TGlobal/interface/GlobalObject.h"
#include "DataFormats/Math/interface/LorentzVector.h"
//#include "SimGeneral/HepPDTESSource/data/pythiapdt.cfi"
//#include "PhysicsTools/HepMCCandAlgos/data/genParticles.cfi"


class SaveGenHH : public edm::EDAnalyzer {
    public:
        explicit SaveGenHH(const edm::ParameterSet& iConfig);
        virtual ~SaveGenHH(){};

    private:
        //----edm control---
        virtual void beginJob();
        virtual void beginRun(edm::Run const&, edm::EventSetup const&);
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob();
        virtual void endRun(edm::Run const&, edm::EventSetup const&);
        virtual void clear();

        //check on lumi
        std::vector<edm::LuminosityBlockRange> lumisTemp ;

        //basic infos on the event
        unsigned long long int event_;
        int       run_;
        int       lumi_;

        //Important objects:
        const edm::EDGetTokenT< reco::GenParticleCollection >            gensToken_; //GenParticles

        //booleans
        bool verbose_;

        //Tree
        std::string t;
        TTree* tree_;

        //Kinematics

        // vars from HHReweight5D
        float gen_H1_m;
        float gen_H1_pt;
        float gen_H1_eta;
        float gen_H1_phi;
        TLorentzVector gen_H1_p4;

        float gen_H2_m;
        float gen_H2_pt;
        float gen_H2_eta;
        float gen_H2_phi;
        TLorentzVector gen_H2_p4;

        float gen_mHH;
        float gen_costh_H1_cm;
        float gen_costh_H2_cm;

        TLorentzVector vH1;

        int       count;

        std::vector<double> Higgs_part; 
        TLorentzVector Higgs1_p4;
        TLorentzVector Higgs2_p4;

        //bquarks
        std::vector<double>* b_eta = new std::vector<double>;
        std::vector<double>* b_phi = new std::vector<double>;
        std::vector<double>* b_pt = new std::vector<double>;
        

};

using namespace reco;

SaveGenHH::SaveGenHH(const edm::ParameterSet& iConfig): 
    gensToken_(consumes< reco::GenParticleCollection >(iConfig.getParameter<edm::InputTag>("genP"))),
    verbose_(iConfig.getParameter<bool>("verbose")),
    t(iConfig.getParameter<std::string>("tree"))
{   
    if(verbose_){
        
    }

};

void SaveGenHH::beginJob()
{   
    edm::Service<TFileService> fs;
    tree_ = fs -> make<TTree>(t.c_str(), t.c_str());

    Higgs_part.resize(8);

    tree_->Branch("gen_H1_m",   &Higgs_part.at(0));
    tree_->Branch("gen_H1_pt",  &Higgs_part.at(1));
    tree_->Branch("gen_H1_eta", &Higgs_part.at(2));
    tree_->Branch("gen_H1_phi", &Higgs_part.at(3));
    tree_->Branch("gen_H1_p4",  &Higgs1_p4);

    tree_->Branch("gen_H2_m",   &Higgs_part.at(4));
    tree_->Branch("gen_H2_pt",  &Higgs_part.at(5));
    tree_->Branch("gen_H2_eta", &Higgs_part.at(6));
    tree_->Branch("gen_H2_phi", &Higgs_part.at(7));
    tree_->Branch("gen_H2_p4",  &Higgs2_p4);

    tree_->Branch("gen_mHH",         &gen_mHH);
    tree_->Branch("gen_costh_H1_cm", &gen_costh_H1_cm);
    tree_->Branch("gen_costh_H2_cm", &gen_costh_H2_cm);

    //-----------------------
    // infos
    //-----------------------
    tree_->Branch("event", &event_);
    tree_->Branch("run",   &run_);
    tree_->Branch("lumi",  &lumi_);

    //-----------------------
    // bquarks
    //-----------------------
    tree_->Branch("b_pt", &b_pt);
    tree_->Branch("b_eta", &b_eta);
    tree_->Branch("b_phi", &b_phi);

    return;
}


void SaveGenHH::clear(){

    b_eta->clear();
    b_phi->clear();
    b_pt->clear();

    return;
}


void SaveGenHH::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{   
    using namespace edm;
    using namespace reco;

    event_ = iEvent.id().event();
    run_   = iEvent.id().run();
    lumi_  = iEvent.luminosityBlock();

    clear();

    edm::Handle<GenParticleCollection> genParticleH;
    iEvent.getByToken(gensToken_, genParticleH);  
    const reco::GenParticleCollection& genParticle = (*genParticleH.product());

    
    int count = 0;
    for(size_t idpart = 0; idpart < genParticle.size(); idpart++){

        reco::GenParticle particle = genParticle.at(idpart);
        

        if(particle.pdgId() == 25){
            if(particle.status() == 22){
                count++;
                if(count == 1){
                    Higgs_part.at(0) = particle.mass();
                    Higgs_part.at(1) = particle.pt();
                    Higgs_part.at(2) = particle.eta();
                    Higgs_part.at(3) = particle.phi();

                    vH1.SetPx(particle.px());
                    vH1.SetPy(particle.py());
                    vH1.SetPz(particle.pz());
                    vH1.SetE(particle.energy());
                    Higgs1_p4 = vH1;
                }
                else if (count == 2){
                    Higgs_part.at(4) = particle.mass();
                    Higgs_part.at(5) = particle.pt();
                    Higgs_part.at(6) = particle.eta();
                    Higgs_part.at(7) = particle.phi();

                    vH1.SetPx(particle.px());
                    vH1.SetPy(particle.py());
                    vH1.SetPz(particle.pz());
                    vH1.SetE(particle.energy());
                    Higgs2_p4 = vH1;
                }
                
            }
        }

        //check for b particles
        else if(particle.pdgId() == 5 || particle.pdgId() == -5){
            //check if b is first in chain
            if(particle.statusFlags().isFirstCopy()){
                //check if b has a higgs mother
                if(particle.mother()->pdgId() == 25){
                    //then it is the b we want 
                    b_pt->push_back(particle.pt());
                    b_eta->push_back(particle.eta());
                    b_phi->push_back(particle.phi());
                    
                }
            }
        }
    }
        

    //Computing Higgs quan tities

    TLorentzVector vSum =   Higgs1_p4 + Higgs2_p4;
    
    // boost to CM copy vectors otherwise we won't load them in tree 

    TLorentzVector vH1_cm = Higgs1_p4;
    TLorentzVector vH2_cm = Higgs2_p4;
    vH1_cm.Boost(-vSum.BoostVector());
    vH2_cm.Boost(-vSum.BoostVector());

    gen_mHH          = vSum.M();
    gen_costh_H1_cm  = vH1_cm.CosTheta();
    gen_costh_H2_cm  = vH2_cm.CosTheta();


    tree_->Fill();

};

//------------------Starting Event-----------------------------------------

void SaveGenHH::beginRun(edm::Run const& iRun, edm::EventSetup const& iEvent)
{

};

void SaveGenHH::endJob(){

};

void SaveGenHH::endRun(edm::Run const& iRun, edm::EventSetup const& iEvent){

};

#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(SaveGenHH);

#endif