#ifndef HLTKinModel_h
#define HLTKinModel_h


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
class HLTKinModel : public HLTFilter {
public:
  explicit HLTKinModel(const edm::ParameterSet& config);
  ~HLTKinModel() override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  //input_t DummyInputGeneration() const; //creates map with predefined names var. Can be made smarter
  bool hltFilter(edm::Event& event,
                 const edm::EventSetup& setup,
                 trigger::TriggerFilterObjectWithRefs& filterproduct) const override;
  
private:
  edm::InputTag m_JetsBase;  // module label of base input jets (not btagged)
  edm::EDGetTokenT<std::vector<T> > m_JetsBaseToken;
  double m_MinEta, m_MaxEta;
  double m_MinPt, m_MaxPt;
  int m_TriggerType;
  edm::FileInPath nnconfig;
  double m_WP;
  lwt::LightweightNeuralNetwork* neural_network_;
};

#endif  // HLTKinModel_h