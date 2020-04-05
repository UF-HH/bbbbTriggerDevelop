#include "../interface/JetBSaver.h"
#include "TTree.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/BTauReco/interface/JetTag.h"




template <typename T>
JetBSaver<T>::JetBSaver(const edm::ParameterSet& iConfig): 
    JetsBTag_(iConfig.getParameter<edm::InputTag>("JetTags")),
    JetsBToken_(consumes<reco::JetTagCollection>(JetsBTag_)),
    verbose_(iConfig.getParameter<bool>("verbose")),
    t(iConfig.getParameter<std::string>("tree")),
    branchName(iConfig.getParameter<std::string>("branch"))
{   
    if(verbose_) std::cout << "Initializing with: " << JetsBTag_.encode() << std::endl;
};

template <typename T>
void JetBSaver<T>::beginJob()
{   
    edm::Service<TFileService> fs;
    tree_ = fs -> make<TTree>(t.c_str(), t.c_str());

    tree_->Branch((branchName+"_btag").c_str(), &btag_);
    tree_->Branch((branchName+"_pt").c_str(), &pt_);
    tree_->Branch((branchName+"_et").c_str(), &et_);
    tree_->Branch((branchName+"_eta").c_str(), &eta_);
    tree_->Branch((branchName+"_phi").c_str(), &phi_);
    tree_->Branch((branchName+"_e").c_str(), &e_);
    tree_->Branch((branchName+"_mass").c_str(), &mass_);

    return;
}

template <typename T>
void JetBSaver<T>::clear(){

    btag_->clear();
    pt_->clear();
    et_->clear();
    eta_->clear();
    phi_->clear();
    e_->clear();
    mass_->clear();

    return;
}

template <typename T>
void JetBSaver<T>::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{   
    using namespace edm;
    using namespace reco;

    clear();

    
    edm::Handle<JetTagCollection> h_JetTags;
    iEvent.getByToken(JetsBToken_, h_JetTags);

    for (auto const& jet : *h_JetTags) {
        btag_->push_back(jet.second);
        pt_->push_back(jet.first->pt());
        et_->push_back(jet.first->et());
        eta_->push_back(jet.first->eta());
        phi_->push_back(jet.first->phi());
        e_->push_back(jet.first->energy());
        mass_->push_back(jet.first->mass());

    }
    

    tree_->Fill();

};

//------------------Starting Event-----------------------------------------

template <typename T>
void JetBSaver<T>::beginRun(edm::Run const& iRun, edm::EventSetup const& iEvent)
{

};

template <typename T>
void JetBSaver<T>::endJob(){

};

template <typename T>
void JetBSaver<T>::endRun(edm::Run const& iRun, edm::EventSetup const& iEvent){

};