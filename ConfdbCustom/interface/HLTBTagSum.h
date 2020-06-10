#ifndef HLTBTagSum_h
#define HLTBTagSum_h


#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

namespace edm {
  class ConfigurationDescriptions;
}

//
// class declaration
//

template <typename T>
class HLTBTagSum : public HLTFilter {
public:
  explicit HLTBTagSum(const edm::ParameterSet& config);
  ~HLTBTagSum() override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
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
  uint m_MinSum;
  double m_MinBTagSum; 
  int m_TriggerType;
};

#endif  // HLTBTagSum_h