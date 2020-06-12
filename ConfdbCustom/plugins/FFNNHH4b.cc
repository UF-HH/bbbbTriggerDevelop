#include "../interface/FFNNHH4b.h"

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
FFNNHH4b<T>::FFNNHH4b(const edm::ParameterSet& iConfig)
    : HLTFilter(iConfig),
      m_Jets(iConfig.getParameter<edm::InputTag>("Jets")),
      m_JetTags(iConfig.getParameter<edm::InputTag>("JetTags")),
      m_MinTag(iConfig.getParameter<double>("MinTag")),
      m_MaxTag(iConfig.getParameter<double>("MaxTag")),
      m_MinEta(iConfig.getParameter<double>("MinEta")),
      m_MaxEta(iConfig.getParameter<double>("MaxEta")),
      m_MinPt(iConfig.getParameter<double>("MinPt")),
      m_MaxPt(iConfig.getParameter<double>("MaxPt")),
      m_TriggerType(iConfig.getParameter<int>("TriggerType")),
      nnconfig(iConfig.getParameter<edm::FileInPath>("NNConfig")),
      m_WP(iConfig.getParameter<double>("WorkingPoint")){
          m_JetsToken = consumes<std::vector<T>>(m_Jets), m_JetTagsToken = consumes<reco::JetTagCollection>(m_JetTags);
          //parse json
          ifstream jsonfile(nnconfig.fullPath());
          auto config = lwt::parse_json(jsonfile);

          //create NN and store the output names for the future
          neural_network_ = new lwt::LightweightNeuralNetwork(config.inputs, config.layers, config.outputs); //This will be fixed (std::make_unique<const)
}

template <typename T>
FFNNHH4b<T>::~FFNNHH4b() = default;


template <typename T>
void FFNNHH4b<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  makeHLTFilterDescription(desc);
  desc.add<edm::InputTag>("Jets", edm::InputTag("hltJetCollection"));
  desc.add<edm::InputTag>("JetTags", edm::InputTag("hltJetTagCollection"));
  desc.add<double>("MinTag", 0.0);
  desc.add<double>("MaxTag", 999999.0);
  desc.add<double>("MinEta", -1);
  desc.add<double>("MaxEta", 999999.0);
  desc.add<double>("MinPt", -1);
  desc.add<double>("MaxPt", 999999.0);
  desc.add<int>("TriggerType", 0);
  desc.add<edm::FileInPath>("NNConfig", edm::FileInPath("models_json/FFNNHH_4b_lwtnn_2.json"));
  desc.add<double>("WorkingPoint", 0.0);
  descriptions.add(defaultModuleLabel<FFNNHH4b<T>>(), desc);
}

//
// member functions
//

/*
template <typename T>
input_t FFNNHH4b<T>::DummyInputGeneration() const{
  
  return { {"inputs",{{"1LeadingPt", 1}, {"1LeadingEta", 1}, {"1LeadingPhi", 1}, {"1LeadingBTag", 1}, {"2LeadingPt", 1}, {"2LeadingEta", 1}, {"2LeadingPhi", 1}, {"2LeadingBTag", 1}, {"3LeadingPt", 1}, {"3LeadingEta", 1}, {"3LeadingPhi", 1}, {"3LeadingBTag", 1}, {"4LeadingPt", 1}, {"4LeadingEta", 1}, {"4LeadingPhi", 1}, {"4LeadingBTag", 1}, {"BTag1", 1}, {"BTag2", 1}, {"BTag3", 1}, {"BTag4", 1} }} };

}
*/

// ------------ method called to produce the data  ------------
template <typename T>
bool FFNNHH4b<T>::hltFilter(edm::Event& event,
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

  lwt::ValueMap inputs_; //NN inputs

  //Dummy input to NN
  //auto inputs = DummyInputGeneration();

  // Look at all jets in decreasing order of Pt (corrected jets).
  int nJet = 0;
  std::vector<double> btags_val_;
  
  //HORRIBLEEEEEEEEEEEEEEE
  for (auto const& jet : *h_JetTags) {
    nJet++;
  }
  std::cout << nJet << std::endl;
  if(nJet < 4){
    return false;
  }

  nJet = 0;
  //NEEDS SOLUTION HELPPPPPPPPP
  
  for (auto const& jet : *h_JetTags) {
    jetRef = TRef(h_Jets, jet.first.key());
    LogTrace("") << "Jet " << nJet << " : Pt = " << jet.first->pt() << " , tag value = " << jet.second;
    ++nJet;
    if(nJet < 5){

        std::cout << std::to_string(nJet)+"LeadingPt" << " " << std::to_string(nJet)+"LeadingMass" << " " << std::to_string(nJet)+"LeadingE" << " " << std::to_string(nJet)+"LeadingEta" << " " << std::to_string(nJet)+"LeadingPhi" << " " << std::to_string(nJet)+"LeadingBTag" << std::endl;
        
        inputs_[std::to_string(nJet)+"LeadingPt"] = jet.first->pt();
        inputs_[std::to_string(nJet)+"LeadingMass"] = jet.first->mass();
        inputs_[std::to_string(nJet)+"LeadingE"] = jet.first->energy();
        inputs_[std::to_string(nJet)+"LeadingEta"] = jet.first->eta();
        inputs_[std::to_string(nJet)+"LeadingBTag"] = jet.second;
    
    }

    //Save all BTag Scores. Only for central jets eta < 2.5 and pt > 30, that's why the selection on pt and eta
    if ((m_MinTag <= jet.second) && (jet.second <= m_MaxTag) && (abs(jet.first->eta()) <= m_MaxEta) && (abs(jet.first->eta()) >= m_MinEta) &&
        (jet.first->pt() <= m_MaxPt) && (jet.first->pt() >= m_MinPt) )  {
        btags_val_.push_back(jet.second);
        // Store a reference to the jets which passed tagging cuts
        filterproduct.addObject(m_TriggerType, jetRef);
    }
  }

  std::sort(btags_val_.begin(), btags_val_.end(), std::greater<double>());
  if(btags_val_.size() >= 4){
      std::vector<double> FourleadingBTag(btags_val_.begin(), btags_val_.begin()+4);
      for(int i = 0; i < 4; i++){
          inputs_[std::to_string(i+1)+"BTag"] = FourleadingBTag.at(i);
          std::cout << std::endl;
      }
  }
  else{
      std::vector<double> FourleadingBTag(btags_val_.begin(), btags_val_.end());
      for(int j = 0; j < 4-(int)btags_val_.size(); j++){
          FourleadingBTag.push_back(0);
      }
      for(int i = 0; i < 4; i++){
          inputs_[std::to_string(i+1)+"BTag"] = FourleadingBTag.at(i);
          std::cout << std::endl;
      }
  }
  
  auto nnoutput = neural_network_->compute(inputs_);

  //horrible
  double output_value = 0; //initialize as empty as to avoid crashes
  for (const auto& out: nnoutput) {
     output_value = out.second;
    }

  //decision
  bool accept(output_value >= m_WP);

  //edm::LogInfo("") << " trigger accept ? = " << accept << " nTag/nJet = " << nTag << "/" << nJet << std::endl;

  return accept;
}
