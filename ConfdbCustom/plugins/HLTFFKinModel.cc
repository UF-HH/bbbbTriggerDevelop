#include "../interface/HLTKinModel.h"

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
HLTKinModel<T>::HLTKinModel(const edm::ParameterSet& iConfig)
    : HLTFilter(iConfig),
      m_JetsBase(iConfig.getParameter<edm::InputTag>("BaseJets")),
      m_MinEta(iConfig.getParameter<double>("MinEta")),
      m_MaxEta(iConfig.getParameter<double>("MaxEta")),
      m_MinPt(iConfig.getParameter<double>("MinPt")),
      m_MaxPt(iConfig.getParameter<double>("MaxPt")),
      m_TriggerType(iConfig.getParameter<int>("TriggerType")),
      nnconfig(iConfig.getParameter<edm::FileInPath>("NNConfig")),
      m_WP(iConfig.getParameter<double>("WorkingPoint")){
          m_JetsBaseToken = consumes<std::vector<T>>(m_JetsBase);
  
          //parse json
          ifstream jsonfile(nnconfig.fullPath());
          auto config = lwt::parse_json(jsonfile);

          //create NN and store the output names for the future
          neural_network_ = new lwt::LightweightNeuralNetwork(config.inputs, config.layers, config.outputs); //This will be fixed (std::make_unique<const)
}

template <typename T>
HLTKinModel<T>::~HLTKinModel() = default;


template <typename T>
void HLTKinModel<T>::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  makeHLTFilterDescription(desc);
  desc.add<edm::InputTag>("BaseJets", edm::InputTag("hltJetCollection"));
  desc.add<double>("MinEta", -1);
  desc.add<double>("MaxEta", 999999.0);
  desc.add<double>("MinPt", -1);
  desc.add<double>("MaxPt", 999999.0);
  desc.add<int>("TriggerType", 0);
  desc.add<edm::FileInPath>("NNConfig", edm::FileInPath("models_json/FFNNHH_4b_lwtnn_2.json"));
  desc.add<double>("WorkingPoint", 0.0);
  descriptions.add(defaultModuleLabel<HLTKinModel<T>>(), desc);
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
bool HLTKinModel<T>::hltFilter(edm::Event& event,
                             const edm::EventSetup& setup,
                             trigger::TriggerFilterObjectWithRefs& filterproduct) const {
  using namespace std;
  using namespace edm;
  using namespace reco;

  typedef vector<T> TCollection;
  typedef Ref<TCollection> TRef;

  edm::Handle<TCollection> h_JetsBase;
  event.getByToken(m_JetsBaseToken, h_JetsBase);

  TRef jetRef;

  lwt::ValueMap inputs_; //NN inputs

  //Dummy input to NN
  //auto inputs = DummyInputGeneration();

  // Look at all jets in decreasing order of Pt (corrected jets).
  int nJet = 0;
  
  for (auto const& jet : *h_JetsBase) {
    double pt = jet.pt(), mass = jet.mass(), e = jet.energy(), eta = jet.eta();
    //jetRef = TRef(h_Jets, jet.first.key());
    //LogTrace("") << "Jet " << nJet << " : Pt = " << jet.first->pt() << " , tag value = " << jet.second;
    ++nJet;
    if(nJet < 5){        
        inputs_[std::to_string(nJet)+"LeadingPt"] = pt;
        inputs_[std::to_string(nJet)+"LeadingMass"] = mass;
        inputs_[std::to_string(nJet)+"LeadingE"] = e;
        inputs_[std::to_string(nJet)+"LeadingEta"] = eta;
    }
  }

  if(nJet < 4){ //Dummy padding with 0 jets
    for(int i = nJet+1; i <= 4; i++){
      inputs_[std::to_string(nJet)+"LeadingPt"] = 0;
      inputs_[std::to_string(nJet)+"LeadingMass"] = 0;
      inputs_[std::to_string(nJet)+"LeadingE"] = 0;
      inputs_[std::to_string(nJet)+"LeadingEta"] = 0;
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
