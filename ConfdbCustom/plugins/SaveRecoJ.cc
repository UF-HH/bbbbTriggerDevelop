#ifndef JET_ALL_RECO_MC_SAVER_H
#define JET_ALL_RECO_MC_SAVER_H


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
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "DataFormats/L1TGlobal/interface/GlobalObjectMapFwd.h"
#include "DataFormats/L1TGlobal/interface/GlobalObjectMap.h"
#include "DataFormats/L1TGlobal/interface/GlobalObjectMapRecord.h"
#include "DataFormats/L1TGlobal/interface/GlobalObject.h"


class SaveRecoJ : public edm::EDAnalyzer {
    public:
        explicit SaveRecoJ(const edm::ParameterSet& iConfig);
        virtual ~SaveRecoJ(){};

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
        const edm::InputTag RecoTag_; 
        const edm::EDGetTokenT<std::vector<pat::Jet>> RecoToken_;


        //booleans
        bool verbose_;

        //Tree
        std::string t;
        TTree* tree_;

        //Kinematics
        std::vector<double>* reco_btag_ = new std::vector<double>;
        std::vector<double>* reco_pt_ = new std::vector<double>;
        std::vector<double>* reco_et_ = new std::vector<double>;
        std::vector<double>* reco_eta_ = new std::vector<double>;
        std::vector<double>* reco_phi_ = new std::vector<double>;
        std::vector<double>* reco_e_ = new std::vector<double>;
        std::vector<double>* reco_mass_ = new std::vector<double>;

};

using namespace reco;

SaveRecoJ::SaveRecoJ(const edm::ParameterSet& iConfig): 
    RecoTag_(iConfig.getParameter<edm::InputTag>("RecoTag")),
    RecoToken_(consumes<std::vector<pat::Jet> >(RecoTag_)),
    verbose_(iConfig.getParameter<bool>("verbose")),
    t(iConfig.getParameter<std::string>("tree"))
{   
    if(verbose_){
        std::cout << "Initializing with: " << RecoTag_.encode() << std::endl;
    }

    //LOADING GOOD RUNS FROM GOLDEN JSON
    edm::ParameterSet const& inputs = iConfig.getParameter<edm::ParameterSet>("inputs");
    if ( inputs.exists("lumisToProcess") ) 
    {
        lumisTemp = inputs.getUntrackedParameter<std::vector<edm::LuminosityBlockRange> > ("lumisToProcess");
        if(verbose_) std::cout << ".... Loaded Json File" << std::endl; 
    }
};

void SaveRecoJ::beginJob()
{   
    edm::Service<TFileService> fs;
    tree_ = fs -> make<TTree>(t.c_str(), t.c_str());

    tree_->Branch("reco_btag", &reco_btag_);
    tree_->Branch("reco_pt", &reco_pt_);
    tree_->Branch("reco_et", &reco_et_);
    tree_->Branch("reco_eta", &reco_eta_);
    tree_->Branch("reco_phi", &reco_phi_);
    tree_->Branch("reco_e", &reco_e_);
    tree_->Branch("reco_mass", &reco_mass_);

    tree_->Branch("event", &event_);
    tree_->Branch("run",   &run_);
    tree_->Branch("lumi",  &lumi_);

    return;
}


void SaveRecoJ::clear(){

    reco_btag_->clear();
    reco_pt_->clear();
    reco_et_->clear();
    reco_eta_->clear();
    reco_phi_->clear();
    reco_e_->clear();
    reco_mass_->clear();

    return;
}

bool SaveRecoJ::jsonContainsEvent (const edm::Event& iEvent)
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

void SaveRecoJ::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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

    edm::Handle<std::vector<pat::Jet>> Recojets;
    iEvent.getByToken(RecoToken_, Recojets);

    typename std::vector<pat::Jet>::const_iterator m(Recojets->begin());

    for (; m != Recojets->end(); m++) {
        reco_pt_->push_back(m->pt());
        reco_et_->push_back(m->et());
        reco_eta_->push_back(m->eta());
        reco_phi_->push_back(m->phi());
        reco_e_->push_back(m->energy());
        reco_mass_->push_back(m->mass());
        reco_btag_->push_back(m->bDiscriminator("pfDeepFlavourJetTags:probb") + m->bDiscriminator("pfDeepFlavourJetTags:probbb") + m->bDiscriminator("pfDeepFlavourJetTags:problepb"));

    }

    tree_->Fill();

};

//------------------Starting Event-----------------------------------------

void SaveRecoJ::beginRun(edm::Run const& iRun, edm::EventSetup const& iEvent)
{

};

void SaveRecoJ::endJob(){

};

void SaveRecoJ::endRun(edm::Run const& iRun, edm::EventSetup const& iEvent){

};

#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(SaveRecoJ);

#endif