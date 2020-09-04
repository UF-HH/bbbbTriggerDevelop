#ifndef LSTM_5_4_Adytia_h
#define LSTM_5_4_Adytia_h


#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

//lwtnn
#include "lwtnn/LightweightNeuralNetwork.hh"
#include "lwtnn/LightweightGraph.hh"
#include "lwtnn/parse_json.hh"

namespace edm {
  class ConfigurationDescriptions;
}

//Defining inputs to the LSTM net
typedef std::map<std::string, std::map<std::string, double> > input_t;
typedef std::map<std::string, std::vector<double> > map_vec_t;
typedef std::map<std::string, map_vec_t> inputv_t;

//
// class declaration
//

template <typename T>
class LSTM_5_4_Adytia : public HLTFilter {
public:
  explicit LSTM_5_4_Adytia(const edm::ParameterSet& config);
  ~LSTM_5_4_Adytia() override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  //input_t DummyInputGeneration() const; //creates map with predefined names var. Can be made smarter
  bool hltFilter(edm::Event& event,
                 const edm::EventSetup& setup,
                 trigger::TriggerFilterObjectWithRefs& filterproduct) const override;
  inputv_t get_empty_input();
  
private:
  edm::InputTag m_Jets;  // module label of input JetCollection
  edm::EDGetTokenT<std::vector<T> > m_JetsToken;
  edm::InputTag m_JetsBase;  // module label of base input jets (not btagged)
  edm::EDGetTokenT<std::vector<T> > m_JetsBaseToken;
  edm::InputTag m_JetTags;  // module label of input JetTagCollection
  edm::EDGetTokenT<reco::JetTagCollection> m_JetTagsToken;
  double m_MinTag, m_MaxTag;  // tag descriminator cuts applied to each jet
  double m_MinEta, m_MaxEta;
  double m_MinPt, m_MaxPt;
  int m_TriggerType;
  edm::FileInPath nnconfig;
  double m_WP;
  lwt::LightweightNeuralNetwork* graph;
  //lwt::LightweightGraph* graph;
  //lwt::GraphConfig config;};
};

#endif  // LSTM_5_4_Adytia_h