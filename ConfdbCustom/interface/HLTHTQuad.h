#ifndef HLTHTQuad_h
#define HLTHTQuad_h

#include <vector>
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

template <typename T>
class HLTHTQuad : public HLTFilter {
public:
  explicit HLTHTQuad(const edm::ParameterSet&);
  ~HLTHTQuad() override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  bool hltFilter(edm::Event&,
                 const edm::EventSetup&,
                 trigger::TriggerFilterObjectWithRefs& filterproduct) const override;

private:
  const edm::InputTag inputTag_;                       // input tag identifying product
  const edm::EDGetTokenT<std::vector<T>> inputToken_;  // token identifying product
  const int triggerType_;                              // triggerType configured
  const double min_HT;                                    // number of objects passing cuts required
  const double min_E_;                                 // energy threshold in GeV
  const double min_Pt_;                                // pt threshold in GeV
  const double min_Mass_;                              // min mass threshold in GeV
  const double max_Mass_;                              // max mass threshold in GeV
  const double min_Eta_;                               // lower eta cut to define eta-range (symmetric)
  const double max_Eta_;                               // upper eta cut to define eta-range (symmetric)
};

#endif //HLTHTQuad
