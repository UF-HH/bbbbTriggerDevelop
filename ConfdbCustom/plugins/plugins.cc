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

#include "../interface/HLTHTDouble.h"
#include "../interface/HLTHTQuad.h"
#include "HLTHTDouble.cc"
#include "HLTHTQuad.cc"


using namespace reco;
using namespace trigger;

typedef JetSaver<CaloJet> CaloSaver;
typedef JetSaver<PFJet> PFSaver;
typedef JetBSaver<CaloJet> CaloBSaver;
typedef JetBSaver<PFJet> PFBSaver;

//trigger filters
typedef HLTHTDouble<CaloJet> HLTHTCaloDouble;
typedef HLTHTDouble<PFJet> HLTHTPFDouble;
typedef HLTHTQuad<CaloJet> HLTHTCaloQuad;
typedef HLTHTQuad<PFJet> HLTHTPFQuad;

DEFINE_FWK_MODULE(CaloSaver);
DEFINE_FWK_MODULE(PFSaver);
DEFINE_FWK_MODULE(CaloBSaver);
DEFINE_FWK_MODULE(PFBSaver);

DEFINE_FWK_MODULE(HLTHTCaloDouble);
DEFINE_FWK_MODULE(HLTHTPFDouble);
DEFINE_FWK_MODULE(HLTHTCaloQuad);
DEFINE_FWK_MODULE(HLTHTPFQuad);



