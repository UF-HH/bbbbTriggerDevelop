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

#include "../interface/HLTKinModel.h"
#include "HLTFFKinModel.cc"

#include "../interface/HLTBTagModel.h"
#include "HLTBTagModel.cc"

#include "../interface/CNN1D_20_1.h"
#include "CNN1D_20_1.cc"

#include "../interface/CNN1D_5_4.h"
#include "CNN1D_5_4.cc"

#include "../interface/DNN_20_Adytia.h"
#include "DNN_20_Adytia.cc"

#include "../interface/LSTM_20_Adytia.h"
#include "LSTM_20_Adytia.cc"

#include "../interface/LSTM_5_4_Adytia.h"
#include "LSTM_5_4_Adytia.cc"

#include "../interface/DNN_prova.h"
#include "DNN_prova.cc"

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

typedef DNN_20_Adytia<CaloJet> DNN_20_Adytia_Calo;
typedef DNN_20_Adytia<PFJet> DNN_20_Adytia_PF;

typedef HLTBTagModel<CaloJet> HLTBTagFFCalo;
typedef HLTBTagModel<PFJet> HLTBTagFFPF;

typedef HLTKinModel<CaloJet> HLTKinFFCalo;
typedef HLTKinModel<PFJet> HLTKinFFPF;

typedef LSTM_20_Adytia<CaloJet> LSTM_20_1_Calo;
typedef LSTM_20_Adytia<PFJet> LSTM_20_1_PF;

typedef LSTM_5_4_Adytia<CaloJet> LSTM_5_4_Calo;
typedef LSTM_5_4_Adytia<PFJet> LSTM_5_4_PF;

typedef CNN1D_20_1<CaloJet> CNN1D_20_1_Calo;
typedef CNN1D_20_1<PFJet> CNN1D_20_1_PF;

typedef CNN1D_5_4<CaloJet> CNN1D_5_4_Calo;
typedef CNN1D_5_4<PFJet> CNN1D_5_4_PF;

typedef DNN_prova<CaloJet> CaloDNNProva;

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

DEFINE_FWK_MODULE(DNN_20_Adytia_Calo);
DEFINE_FWK_MODULE(DNN_20_Adytia_PF);

DEFINE_FWK_MODULE(HLTBTagFFCalo);
DEFINE_FWK_MODULE(HLTBTagFFPF);

DEFINE_FWK_MODULE(HLTKinFFCalo);
DEFINE_FWK_MODULE(HLTKinFFPF);

//Prova
DEFINE_FWK_MODULE(CNN1D_20_1_Calo);
DEFINE_FWK_MODULE(CNN1D_20_1_PF);

DEFINE_FWK_MODULE(LSTM_20_1_Calo);
DEFINE_FWK_MODULE(LSTM_20_1_PF);

DEFINE_FWK_MODULE(LSTM_5_4_Calo);
DEFINE_FWK_MODULE(LSTM_5_4_PF);

DEFINE_FWK_MODULE(CNN1D_5_4_Calo);
DEFINE_FWK_MODULE(CNN1D_5_4_PF);

DEFINE_FWK_MODULE(CaloDNNProva);
