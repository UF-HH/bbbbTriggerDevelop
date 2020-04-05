#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"

#include "../interface/JetSaver.h"
#include "../interface/JetBSaver.h"
#include "JetSaver.cc"
#include "JetBSaver.cc"

using namespace reco;
using namespace trigger;

typedef JetSaver<CaloJet> CaloSaver;
typedef JetSaver<PFJet> PFSaver;
typedef JetBSaver<CaloJet> CaloBSaver;
typedef JetBSaver<PFJet> PFBSaver;

DEFINE_FWK_MODULE(CaloSaver);
DEFINE_FWK_MODULE(PFSaver);
DEFINE_FWK_MODULE(CaloBSaver);
DEFINE_FWK_MODULE(PFBSaver);
