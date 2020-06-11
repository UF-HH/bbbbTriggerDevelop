#ifndef FFNNHH4b_h
#define FFNNHH4b_h


#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

//lwtnn
#include "lwtnn/LightweightNeuralNetwork.hh"
#include "lwtnn/parse_json.hh"

namespace edm {
  class ConfigurationDescriptions;
}

typedef std::map<std::string, std::map<std::string, double> > input_t;

//
// class declaration
//

template <typename T>
class FFNNHH4b : public HLTFilter {
public:
  explicit FFNNHH4b(const edm::ParameterSet& config);
  ~FFNNHH4b() override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  //input_t DummyInputGeneration() const; //creates map with predefined names var. Can be made smarter
  bool hltFilter(edm::Event& event,
                 const edm::EventSetup& setup,
                 trigger::TriggerFilterObjectWithRefs& filterproduct) const override;
  
private:
  edm::InputTag m_Jets;  // module label of input JetCollection
  edm::EDGetTokenT<std::vector<T> > m_JetsToken;
  edm::InputTag m_JetTags;  // module label of input JetTagCollection
  edm::EDGetTokenT<reco::JetTagCollection> m_JetTagsToken;
  double m_MinTag, m_MaxTag;  // tag descriminator cuts applied to each jet
  double m_MinEta, m_MaxEta;
  double m_MinPt, m_MaxPt;
  int m_TriggerType;
  edm::FileInPath nnconfig;
  double m_WP;
  lwt::LightweightNeuralNetwork* neural_network_;
};

#endif  // FFNNHH4b_h