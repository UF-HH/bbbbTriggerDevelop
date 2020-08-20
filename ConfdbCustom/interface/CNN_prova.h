#ifndef CALO_PROVA_H
#define CALO_PROVA_H


#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"


//
// class declaration
//

template <typename T>
class CNN_prova : public HLTFilter {
public:
  explicit CNN_prova(const edm::ParameterSet& config);
  ~CNN_prova() override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  //input_t DummyInputGeneration() const; //creates map with predefined names var. Can be made smarter
  bool hltFilter(edm::Event& event,
                 const edm::EventSetup& setup,
                 trigger::TriggerFilterObjectWithRefs& filterproduct) const override;
  void endJob();
  
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
  tensorflow::GraphDef* graphDef_;
  tensorflow::Session* session_;
};

#endif  // CNN_prova_h