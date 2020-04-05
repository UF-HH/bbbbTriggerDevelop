#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"

#include "../interface/JetSaver.h"
#include "JetSaver.cc"

using namespace reco;
using namespace trigger;

typedef JetSaver<CaloJet> CaloSaver;
typedef JetSaver<PFJet> PFSaver;

DEFINE_FWK_MODULE(CaloSaver);
DEFINE_FWK_MODULE(PFSaver);
