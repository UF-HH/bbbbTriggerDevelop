#include "../interface/JetSaver.h"
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




template <typename T>
JetSaver<T>::JetSaver(const edm::ParameterSet& iConfig): 
    JetsTag_(iConfig.getParameter<edm::InputTag>("Jets")),
    JetsToken_(consumes<std::vector<T>>(JetsTag_)),
    verbose_(iConfig.getParameter<bool>("verbose")),
    t(iConfig.getParameter<std::string>("tree")),
    branchName(iConfig.getParameter<std::string>("branch"))
{
    if(verbose_) std::cout << "Initializing with: " << JetsTag_.encode() << std::endl;
};

template <typename T>
void JetSaver<T>::beginJob()
{   
    edm::Service<TFileService> fs;
    tree_ = fs -> make<TTree>(t.c_str(), t.c_str());

    tree_->Branch((branchName.append("_pt")).c_str(), &pt_);
    tree_->Branch((branchName.append("_et")).c_str(), &et_);
    tree_->Branch((branchName.append("_eta")).c_str(), &eta_);
    tree_->Branch((branchName.append("_phi")).c_str(), &phi_);
    tree_->Branch((branchName.append("_e")).c_str(), &e_);
    tree_->Branch((branchName.append("_mass")).c_str(), &mass_);

    return;
}

template <typename T>
void JetSaver<T>::clear(){
    pt_->clear();
    et_->clear();
    eta_->clear();
    phi_->clear();
    e_->clear();
    mass_->clear();

    return;
}

template <typename T>
void JetSaver<T>::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{   
    using namespace edm;
    using namespace reco;

    typedef std::vector<T> TCollection;

    edm::Handle<TCollection> jets;
    iEvent.getByToken(JetsToken_, jets);

    clear();

    typename TCollection::const_iterator i(jets->begin());
    for (; i != jets->end(); i++) {
        pt_->push_back(i->pt());
        et_->push_back(i->et());
        eta_->push_back(i->eta());
        phi_->push_back(i->phi());
        e_->push_back(i->energy());
        mass_->push_back(i->mass());

    }

    tree_->Fill();

};

//------------------Starting Event-----------------------------------------

template <typename T>
void JetSaver<T>::beginRun(edm::Run const& iRun, edm::EventSetup const& iEvent)
{

};

template <typename T>
void JetSaver<T>::endJob(){

};

template <typename T>
void JetSaver<T>::endRun(edm::Run const& iRun, edm::EventSetup const& iEvent){

};