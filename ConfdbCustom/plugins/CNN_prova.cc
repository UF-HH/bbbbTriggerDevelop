#include "../interface/CNN_prova.h"

#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "HLTrigger/HLTcore/interface/defaultModuleLabel.h"

//lwtnn
#include "lwtnn/LightweightGraph.hh"
#include "lwtnn/LightweightNeuralNetwork.hh"
#include "lwtnn/parse_json.hh"

#include <boost/algorithm/string.hpp>



//
// constructors and destructor
//

template <typename T>
CNN_prova<T>::CNN_prova(const edm::ParameterSet& iConfig)
    : HLTFilter(iConfig),
      m_Jets(iConfig.getParameter<edm::InputTag>("Jets")),
      m_JetsBase(iConfig.getParameter<edm::InputTag>("BaseJets")),
      m_JetTags(iConfig.getParameter<edm::InputTag>("JetTags")),
      m_MinTag(iConfig.getParameter<double>("MinTag")),
      m_MaxTag(iConfig.getParameter<double>("MaxTag")),
      m_MinEta(iConfig.getParameter<double>("MinEta")),
      m_MaxEta(iConfig.getParameter<double>("MaxEta")),
      m_MinPt(iConfig.getParameter<double>("MinPt")),
      m_MaxPt(iConfig.getParameter<double>("MaxPt")),
      m_TriggerType(iConfig.getParameter<int>("TriggerType")),
      nnconfig(iConfig.getParameter<std::string>("NNConfig")),
      m_WP(iConfig.getParameter<double>("WorkingPoint")){
          m_JetsToken = consumes<std::vector<T>>(m_Jets), m_JetTagsToken = consumes<reco::JetTagCollection>(m_JetTags), m_JetsBaseToken = consumes<std::vector<T>>(m_JetsBase);

      tensorflow::setLogging("0");
      std::cout << "[Info] CNN_prova: Loading graph def from " << nnconfig << std::endl;
      graphDef_ = tensorflow::loadGraphDef(nnconfig);
      session_ = tensorflow::createSession(graphDef_);

}

template <typename T>
CNN_prova<T>::~CNN_prova() = default;


template <typename T>
void CNN_prova<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  makeHLTFilterDescription(desc);
  desc.add<edm::InputTag>("Jets", edm::InputTag("hltJetCollection"));
  desc.add<edm::InputTag>("BaseJets", edm::InputTag("hltJetCollection"));
  desc.add<edm::InputTag>("JetTags", edm::InputTag("hltJetTagCollection"));
  desc.add<double>("MinTag", 0.0);
  desc.add<double>("MaxTag", 999999.0);
  desc.add<double>("MinEta", -1);
  desc.add<double>("MaxEta", 999999.0);
  desc.add<double>("MinPt", -1);
  desc.add<double>("MaxPt", 999999.0);
  desc.add<int>("TriggerType", 0);
  desc.add<std::string>("NNConfig", std::string("models_json/Calo_T4_HPU_CNN1D.pb"));
  desc.add<double>("WorkingPoint", 0.0);
  descriptions.add(defaultModuleLabel<CNN_prova<T>>(), desc);
}

//
// member functions
//

/*
template <typename T>
input_t CNN_prova<T>::DummyInputGeneration() const{
  
  return { {"inputs",{{"1LeadingPt", 1}, {"1LeadingEta", 1}, {"1LeadingPhi", 1}, {"1LeadingBTag", 1}, {"2LeadingPt", 1}, {"2LeadingEta", 1}, {"2LeadingPhi", 1}, {"2LeadingBTag", 1}, {"3LeadingPt", 1}, {"3LeadingEta", 1}, {"3LeadingPhi", 1}, {"3LeadingBTag", 1}, {"4LeadingPt", 1}, {"4LeadingEta", 1}, {"4LeadingPhi", 1}, {"4LeadingBTag", 1}, {"BTag1", 1}, {"BTag2", 1}, {"BTag3", 1}, {"BTag4", 1} }} };

}
*/

// ------------ method called to produce the data  ------------
template <typename T>
bool CNN_prova<T>::hltFilter(edm::Event& event,
                             const edm::EventSetup& setup,
                             trigger::TriggerFilterObjectWithRefs& filterproduct) const {
  using namespace std;
  using namespace edm;
  using namespace reco;

  typedef vector<T> TCollection;
  typedef Ref<TCollection> TRef;

  edm::Handle<TCollection> h_Jets;
  event.getByToken(m_JetsToken, h_Jets);
  if (saveTags())
    filterproduct.addCollectionTag(m_Jets);

  edm::Handle<TCollection> h_JetsBase;
  event.getByToken(m_JetsBaseToken, h_JetsBase);

  edm::Handle<JetTagCollection> h_JetTags;
  event.getByToken(m_JetTagsToken, h_JetTags);

  // check if the product this one depends on is available
  auto const& handle = h_JetTags;
  auto const& dependent = handle->keyProduct();
  if (not dependent.isNull() and not dependent.hasCache()) {
    // only an empty AssociationVector can have a invalid dependent collection
    edm::Provenance const& dependent_provenance = event.getProvenance(dependent.id());
    if (dependent_provenance.branchDescription().dropped())
      // FIXME the error message should be made prettier
      throw edm::Exception(edm::errors::ProductNotFound)
          << "Product " << handle.provenance()->branchName() << " requires product "
          << dependent_provenance.branchName() << ", which has been dropped";
  }

  TRef jetRef;

  tensorflow::Tensor input(tensorflow::DT_FLOAT, tensorflow::TensorShape({ 20,1,1 }));
  auto input_tensor_mapped = input.tensor<float, 3>();
  float* d = input.flat<float>().data();

  // Look at all jets in decreasing order of Pt (corrected jets).
  int nJet = 0;
  std::vector<double> btags_val_;
  
  for (auto const& jet : *h_JetsBase) {
    double pt = jet.pt(), mass = jet.mass(), e = jet.energy(), eta = jet.eta();
    double btag = 0;
    //searching for the btagged jets
    for(auto const& btag_jet : *h_JetTags){
      if(btag_jet.first->pt() == pt && btag_jet.first->eta() == eta && btag_jet.first->energy() == e){
        btag = btag_jet.second;
      }
    }
    //jetRef = TRef(h_Jets, jet.first.key());
    //LogTrace("") << "Jet " << nJet << " : Pt = " << jet.first->pt() << " , tag value = " << jet.second;
    if(nJet < 20){

        //input.matrix<float>()(0, nJet) = float(pt);
        //input.matrix<float>()(0, nJet+1) = float(mass);
        //input.matrix<float>()(0, nJet+2) = float(e);
        //input.matrix<float>()(0, nJet+3) = float(eta);
        //input.matrix<float>()(0, nJet+4) = float(btag);

        // *d = float(pt);
        // std::cout << *d << std::endl;
        // d++;
        // *d = float(mass);
        // std::cout << *d << std::endl;
        // d++;
        // *d = float(e);
        // std::cout << *d << std::endl;
        // d++;
        // *d = float(eta);
        // std::cout << *d << std::endl;
        // d++;
        // *d = float(btag);
        // std::cout << *d << std::endl;
        // d++;

        input_tensor_mapped(nJet, 0, 0) = float(pt);
        std::cout << input_tensor_mapped(nJet, 0, 0) << std::endl;
        nJet++;
        input_tensor_mapped(nJet, 0, 0) = float(mass);
        std::cout << input_tensor_mapped(nJet, 0, 0) << std::endl;
        nJet++;
        input_tensor_mapped(nJet, 0, 0) = float(e);
        std::cout << input_tensor_mapped(nJet, 0, 0) << std::endl;
        nJet++;
        input_tensor_mapped(nJet, 0, 0) = float(eta);
        std::cout << input_tensor_mapped(nJet, 0, 0) << std::endl;
        nJet++;
        input_tensor_mapped(nJet, 0, 0) = float(btag);
        std::cout << input_tensor_mapped(nJet, 0, 0) << std::endl;
        nJet++;

    
    }

    //nJet++;

    //Save all BTag Scores. Only for central jets eta < 2.5 and pt > 30, that's why the selection on pt and eta
    if ((m_MinTag <= btag) && (btag <= m_MaxTag) && (abs(eta) <= m_MaxEta) && (abs(eta) >= m_MinEta) &&
        (pt <= m_MaxPt) && (pt >= m_MinPt) )  {
        btags_val_.push_back(btag);
        // Store a reference to the jets which passed tagging cuts
        filterproduct.addObject(m_TriggerType, jetRef);
    }
  }

  std::cout << input.matrix<float>()(0, 0) << std::endl;


  std::vector<tensorflow::Tensor> outputs;
  tensorflow::run(session_, { { "Input", input } }, { "Output/Sigmoid" }, &outputs);
  
  std::cout << " -> " << outputs[0].matrix<float>()(0, 0) << std::endl << std::endl;

  //decision
  bool accept(true);

  //edm::LogInfo("") << " trigger accept ? = " << accept << " nTag/nJet = " << nTag << "/" << nJet << std::endl;

  return accept;
}

template <typename T>
void CNN_prova<T>::endJob(){

  // close the session
    tensorflow::closeSession(session_);
    session_ = nullptr;

    // delete the graph
    delete graphDef_;
    graphDef_ = nullptr;

}
