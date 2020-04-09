#ifndef JET_ALL_SAVER_H
#define JET_ALL_SAVER_H


#include <vector>
#include <string>
#include <iostream>
#include <boost/bind.hpp>

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


class SaveAllJets : public edm::EDAnalyzer {
    public:
        explicit SaveAllJets(const edm::ParameterSet& iConfig);
        virtual ~SaveAllJets(){};

    private:
        //----edm control---
        virtual void beginJob();
        virtual void beginRun(edm::Run const&, edm::EventSetup const&);
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob();
        virtual void endRun(edm::Run const&, edm::EventSetup const&);
        virtual void clear();
        virtual bool jsonContainsEvent (const edm::Event& iEvent);

        //check on lumi
        std::vector<edm::LuminosityBlockRange> lumisTemp ;

        //basic infos on the event
        unsigned long long int event_;
        int       run_;
        int       lumi_;

        //Important objects:

        const edm::InputTag L1Tag_;
        const edm::EDGetTokenT<l1t::JetBxCollection> L1Token_;
        const edm::InputTag PFTag_; 
        const edm::EDGetTokenT<std::vector<reco::PFJet>> PFToken_;
        const edm::InputTag CaloTag_; 
        const edm::EDGetTokenT<std::vector<reco::CaloJet>> CaloToken_;
        const edm::InputTag PFBTag_; 
        const edm::EDGetTokenT<reco::JetTagCollection> PFBToken_;
        const edm::InputTag CaloBTag_; 
        const edm::EDGetTokenT<reco::JetTagCollection> CaloBToken_;


        //booleans
        bool verbose_;

        //Tree
        std::string t;
        TTree* tree_;

        //Kinematics
        std::vector<double>* l1_pt_ = new std::vector<double>;
        std::vector<double>* l1_et_ = new std::vector<double>;
        std::vector<double>* l1_eta_ = new std::vector<double>;
        std::vector<double>* l1_phi_ = new std::vector<double>;
        std::vector<double>* l1_e_ = new std::vector<double>;
        std::vector<double>* l1_mass_ = new std::vector<double>;

        std::vector<double>* calo_btag_ = new std::vector<double>;
        std::vector<double>* calo_pt_ = new std::vector<double>;
        std::vector<double>* calo_et_ = new std::vector<double>;
        std::vector<double>* calo_eta_ = new std::vector<double>;
        std::vector<double>* calo_phi_ = new std::vector<double>;
        std::vector<double>* calo_e_ = new std::vector<double>;
        std::vector<double>* calo_mass_ = new std::vector<double>;

        std::vector<double>* pf_btag_ = new std::vector<double>;
        std::vector<double>* pf_pt_ = new std::vector<double>;
        std::vector<double>* pf_et_ = new std::vector<double>;
        std::vector<double>* pf_eta_ = new std::vector<double>;
        std::vector<double>* pf_phi_ = new std::vector<double>;
        std::vector<double>* pf_e_ = new std::vector<double>;
        std::vector<double>* pf_mass_ = new std::vector<double>;

};

using namespace reco;

SaveAllJets::SaveAllJets(const edm::ParameterSet& iConfig): 
    L1Tag_(iConfig.getParameter<edm::InputTag>("L1JetTag")),
    L1Token_(consumes<l1t::JetBxCollection>(L1Tag_)),
    PFTag_(iConfig.getParameter<edm::InputTag>("PFJetTag")),
    PFToken_(consumes<std::vector<PFJet>>(PFTag_)),
    CaloTag_(iConfig.getParameter<edm::InputTag>("CaloJetTag")),
    CaloToken_(consumes<std::vector<CaloJet>>(CaloTag_)),
    PFBTag_(iConfig.getParameter<edm::InputTag>("PFBJetTag")),
    PFBToken_(consumes<reco::JetTagCollection>(PFBTag_)),
    CaloBTag_(iConfig.getParameter<edm::InputTag>("CaloBJetTag")),
    CaloBToken_(consumes<reco::JetTagCollection>(CaloBTag_)),
    verbose_(iConfig.getParameter<bool>("verbose")),
    t(iConfig.getParameter<std::string>("tree"))
{   
    if(verbose_){
        std::cout << "Initializing with: " << L1Tag_.encode() << std::endl;
        std::cout << "Initializing with: " << PFTag_.encode() << std::endl;
        std::cout << "Initializing with: " << CaloTag_.encode() << std::endl;
        std::cout << "Initializing with: " << CaloBTag_.encode() << std::endl;
        std::cout << "Initializing with: " << PFBTag_.encode() << std::endl;
    }

    //LOADING GOOD RUNS FROM GOLDEN JSON
    edm::ParameterSet const& inputs = iConfig.getParameter<edm::ParameterSet>("inputs");
    if ( inputs.exists("lumisToProcess") ) 
    {
        lumisTemp = inputs.getUntrackedParameter<std::vector<edm::LuminosityBlockRange> > ("lumisToProcess");
        if(verbose_) std::cout << ".... Loaded Json File" << std::endl; 
    }
};

void SaveAllJets::beginJob()
{   
    edm::Service<TFileService> fs;
    tree_ = fs -> make<TTree>(t.c_str(), t.c_str());

    tree_->Branch("l1_pt", &l1_pt_);
    tree_->Branch("l1_et", &l1_et_);
    tree_->Branch("l1_eta", &l1_eta_);
    tree_->Branch("l1_phi", &l1_phi_);
    tree_->Branch("l1_e", &l1_e_);
    tree_->Branch("l1_mass", &l1_mass_);

    tree_->Branch("calo_btag", &calo_btag_);
    tree_->Branch("calo_pt", &calo_pt_);
    tree_->Branch("calo_et", &calo_et_);
    tree_->Branch("calo_eta", &calo_eta_);
    tree_->Branch("calo_phi", &calo_phi_);
    tree_->Branch("calo_e", &calo_e_);
    tree_->Branch("calo_mass", &calo_mass_);

    tree_->Branch("pf_btag", &pf_btag_);
    tree_->Branch("pf_pt", &pf_pt_);
    tree_->Branch("pf_et", &pf_et_);
    tree_->Branch("pf_eta", &pf_eta_);
    tree_->Branch("pf_phi", &pf_phi_);
    tree_->Branch("pf_e", &pf_e_);
    tree_->Branch("pf_mass", &pf_mass_);

    tree_->Branch("event", &event_);
    tree_->Branch("run",   &run_);
    tree_->Branch("lumi",  &lumi_);

    return;
}


void SaveAllJets::clear(){

    l1_pt_->clear();
    l1_et_->clear();
    l1_eta_->clear();
    l1_phi_->clear();
    l1_e_->clear();
    l1_mass_->clear();

    calo_btag_->clear();
    calo_pt_->clear();
    calo_et_->clear();
    calo_eta_->clear();
    calo_phi_->clear();
    calo_e_->clear();
    calo_mass_->clear();

    pf_btag_->clear();
    pf_pt_->clear();
    pf_et_->clear();
    pf_eta_->clear();
    pf_phi_->clear();
    pf_e_->clear();
    pf_mass_->clear();

    return;
}

bool SaveAllJets::jsonContainsEvent (const edm::Event& iEvent)
{
   // if the jsonVec is empty, then no JSON file was provided so all
   // events should pass
   if (lumisTemp.empty())
   {
      return true;
   }
   bool (* funcPtr) (edm::LuminosityBlockRange const &,
                     edm::LuminosityBlockID const &) = &edm::contains;
   edm::LuminosityBlockID lumiID (iEvent.id().run(), 
                                  iEvent.id().luminosityBlock());
   std::vector< edm::LuminosityBlockRange >::const_iterator iter = 
      std::find_if (lumisTemp.begin(), lumisTemp.end(),
                    boost::bind(funcPtr, _1, lumiID) );
   return lumisTemp.end() != iter;

}

void SaveAllJets::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{   
    using namespace edm;
    using namespace reco;

    event_ = iEvent.id().event();
    run_   = iEvent.id().run();
    lumi_  = iEvent.luminosityBlock();

    clear();

    //If this configuration is not present in the json then the event is skipped
    //Note than if no json is provided this function always returns true
    if(!jsonContainsEvent(iEvent)){
        std::cout << "Removing Event, not found in Json" << std::endl;
        return;
    } 

    edm::Handle<l1t::JetBxCollection> L1jets;
    iEvent.getByToken(L1Token_, L1jets);

    edm::Handle<std::vector<reco::CaloJet>> Calojets;
    iEvent.getByToken(CaloToken_, Calojets);
    edm::Handle<std::vector<reco::PFJet>> PFjets;
    iEvent.getByToken(PFToken_, PFjets);

    edm::Handle<JetTagCollection> CaloBjets;
    iEvent.getByToken(CaloBToken_, CaloBjets);
    edm::Handle<JetTagCollection> PFBjets;
    iEvent.getByToken(PFBToken_, PFBjets);

    //filling in kinematical for l1 calo and pf
    typename l1t::JetBxCollection::const_iterator l1(L1jets->begin());

    for (; l1 != L1jets->end(); l1++) {
        l1_pt_->push_back(l1->pt());
        l1_et_->push_back(l1->et());
        l1_eta_->push_back(l1->eta());
        l1_phi_->push_back(l1->phi());
        l1_e_->push_back(l1->energy());
        l1_mass_->push_back(l1->mass());

    }

    typename std::vector<reco::CaloJet>::const_iterator i(Calojets->begin());

    for (; i != Calojets->end(); i++) {
        calo_pt_->push_back(i->pt());
        calo_et_->push_back(i->et());
        calo_eta_->push_back(i->eta());
        calo_phi_->push_back(i->phi());
        calo_e_->push_back(i->energy());
        calo_mass_->push_back(i->mass());

    }

    typename std::vector<reco::PFJet>::const_iterator j(PFjets->begin());

    for (; j != PFjets->end(); j++) {
        pf_pt_->push_back(j->pt());
        pf_et_->push_back(j->et());
        pf_eta_->push_back(j->eta());
        pf_phi_->push_back(j->phi());
        pf_e_->push_back(j->energy());
        pf_mass_->push_back(j->mass());

    }

    std::vector<double> pfb_pt;
    std::vector<double> pfb_btag;
    std::vector<double> pfb_eta;
    std::vector<double> pfb_phi;

    for (auto const& jet : *PFBjets) {
        pfb_btag.push_back(jet.second);
        pfb_pt.push_back(jet.first->pt());
        pfb_eta.push_back(jet.first->eta());
        pfb_phi.push_back(jet.first->phi());
    }

    std::vector<double> calob_pt;
    std::vector<double> calob_btag;
    std::vector<double> calob_eta;
    std::vector<double> calob_phi;

    for (auto const& jet : *CaloBjets) {
        calob_btag.push_back(jet.second);
        calob_pt.push_back(jet.first->pt());
        calob_eta.push_back(jet.first->eta());
        calob_phi.push_back(jet.first->phi());
    }


    uint nmatched = 0;
    for(uint i = 0; i < pf_pt_->size(); i++){
        double pt = pf_pt_->at(i);
        double eta = pf_eta_->at(i);
        double phi = pf_phi_->at(i);
        bool match = false;
        for(uint j = 0; j < pfb_pt.size(); j++){
            double ptb = pfb_pt.at(j);
            double etab = pfb_eta.at(j);
            double phib = pfb_phi.at(j);

            if(ptb == pt && etab == eta && phib == phi){
                pf_btag_->push_back(pfb_btag.at(j));
                match = true;
                nmatched++;
            }
        }

        if(!match) pf_btag_->push_back(0);

    }

    nmatched = 0;
    for(uint i = 0; i < calo_pt_->size(); i++){
        double pt = calo_pt_->at(i);
        double eta = calo_eta_->at(i);
        double phi = calo_phi_->at(i);
        bool match = false;
        for(uint j = 0; j < calob_pt.size(); j++){
            double ptb = calob_pt.at(j);
            double etab = calob_eta.at(j);
            double phib = calob_phi.at(j);

            if(ptb == pt && etab == eta && phib == phi){
                calo_btag_->push_back(calob_btag.at(j));
                match = true;
                nmatched++;
            }
        }

        if(!match) calo_btag_->push_back(0);

    }    

    tree_->Fill();

};

//------------------Starting Event-----------------------------------------

void SaveAllJets::beginRun(edm::Run const& iRun, edm::EventSetup const& iEvent)
{

};

void SaveAllJets::endJob(){

};

void SaveAllJets::endRun(edm::Run const& iRun, edm::EventSetup const& iEvent){

};

#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(SaveAllJets);

#endif