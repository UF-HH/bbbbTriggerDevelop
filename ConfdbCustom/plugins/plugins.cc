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

#include "../interface/HLT2DJetTag.h"
#include "HLT2DJetTag.cc"

#include "../interface/HLTBTagSum.h"
#include "HLTBTagSum.cc"

#include "../interface/FFNNHH4b.h"
#include "FFNNHH4b.cc"

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

typedef HLT2DJetTag<CaloJet> HLT2DJetTagCalo;
typedef HLT2DJetTag<PFJet> HLT2DJetTagPF;

typedef HLTBTagSum<CaloJet> HLTBTagSumCalo;
typedef HLTBTagSum<PFJet> HLTBTagSumPF;

typedef FFNNHH4b<CaloJet> CaloNNHH4b;
typedef FFNNHH4b<PFJet> PFNNHH4b;

DEFINE_FWK_MODULE(CaloSaver);
DEFINE_FWK_MODULE(PFSaver);
DEFINE_FWK_MODULE(CaloBSaver);
DEFINE_FWK_MODULE(PFBSaver);

DEFINE_FWK_MODULE(HLTHTCaloDouble);
DEFINE_FWK_MODULE(HLTHTPFDouble);
DEFINE_FWK_MODULE(HLTHTCaloQuad);
DEFINE_FWK_MODULE(HLTHTPFQuad);

DEFINE_FWK_MODULE(HLT2DJetTagCalo);
DEFINE_FWK_MODULE(HLT2DJetTagPF);

DEFINE_FWK_MODULE(HLTBTagSumCalo);
DEFINE_FWK_MODULE(HLTBTagSumPF);

DEFINE_FWK_MODULE(CaloNNHH4b);
DEFINE_FWK_MODULE(PFNNHH4b);
