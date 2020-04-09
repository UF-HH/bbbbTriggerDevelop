#ifndef JET_L1_SAVER_H
#define JET_L1_SAVER_H


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


class SaveL1Jets : public edm::EDAnalyzer {
    public:
        explicit SaveL1Jets(const edm::ParameterSet& iConfig);
        virtual ~SaveL1Jets(){};

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

};

using namespace reco;

SaveL1Jets::SaveL1Jets(const edm::ParameterSet& iConfig): 
    L1Tag_(iConfig.getParameter<edm::InputTag>("L1JetTag")),
    L1Token_(consumes<l1t::JetBxCollection>(L1Tag_)),
    verbose_(iConfig.getParameter<bool>("verbose")),
    t(iConfig.getParameter<std::string>("tree"))
{   
    if(verbose_){
        std::cout << "Initializing with: " << L1Tag_.encode() << std::endl;
    }

    //LOADING GOOD RUNS FROM GOLDEN JSON
    edm::ParameterSet const& inputs = iConfig.getParameter<edm::ParameterSet>("inputs");
    if ( inputs.exists("lumisToProcess") ) 
    {
        lumisTemp = inputs.getUntrackedParameter<std::vector<edm::LuminosityBlockRange> > ("lumisToProcess");
        if(verbose_) std::cout << ".... Loaded Json File" << std::endl; 
    }
};

void SaveL1Jets::beginJob()
{   
    edm::Service<TFileService> fs;
    tree_ = fs -> make<TTree>(t.c_str(), t.c_str());

    tree_->Branch("l1_pt", &l1_pt_);
    tree_->Branch("l1_et", &l1_et_);
    tree_->Branch("l1_eta", &l1_eta_);
    tree_->Branch("l1_phi", &l1_phi_);
    tree_->Branch("l1_e", &l1_e_);
    tree_->Branch("l1_mass", &l1_mass_);

    return;
}


void SaveL1Jets::clear(){

    l1_pt_->clear();
    l1_et_->clear();
    l1_eta_->clear();
    l1_phi_->clear();
    l1_e_->clear();
    l1_mass_->clear();

    return;
}

bool SaveL1Jets::jsonContainsEvent (const edm::Event& iEvent)
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

void SaveL1Jets::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{   
    using namespace edm;
    using namespace reco;

    event_ = iEvent.id().event();
    run_   = iEvent.id().run();
    lumi_  = iEvent.luminosityBlock();

    clear();

    //If this configuration is not present in the json then the event is skipped
    //Note than if no json is provided this function always returns true
    if(!jsonContainsEvent(iEvent)) return;

    edm::Handle<l1t::JetBxCollection> L1jets;
    iEvent.getByToken(L1Token_, L1jets);

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
 

    tree_->Fill();

};

//------------------Starting Event-----------------------------------------

void SaveL1Jets::beginRun(edm::Run const& iRun, edm::EventSetup const& iEvent)
{

};

void SaveL1Jets::endJob(){

};

void SaveL1Jets::endRun(edm::Run const& iRun, edm::EventSetup const& iEvent){

};

#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(SaveL1Jets);

#endif