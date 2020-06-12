import sys
import os

def HLTHTCaloDouble(saveTags = True , MaxMass = -1.0, MinHT=180, MaxEta=2.6, MinEta=-1.0, MinMass=-1.0, inputTag="'hltAK4CaloJetsCorrectedIDPassed'", MinE=-1.0, triggerType=86, MinPt=-1):
    module =  'cms.EDFilter( "HLTHTCaloDouble", \nsaveTags = cms.bool( {} ), \nMaxMass = cms.double( {} ), \nMinHT = cms.double( {} ),\nMaxEta = cms.double( {} ),\nMinEta = cms.double( {} ),\nMinMass = cms.double( {} ),\ninputTag = cms.InputTag( {} ),\nMinE = cms.double( {} ),\ntriggerType = cms.int32( {} ),\nMinPt = cms.double( {} )\n)\n'.format(saveTags , MaxMass, MinHT, MaxEta, MinEta, MinMass, inputTag, MinE, triggerType, MinPt)
    return module

def HLTHTCaloQuad(saveTags=True, MaxMass = -1.0, MinHT=90., MaxEta=2.6, MinEta=-1.0, MinMass = -1.0, inputTag = "'hltAK4CaloJetsCorrectedIDPassed'", MinE = -1.0, triggerType = 86 ,MinPt = -1 ):
    module = 'cms.EDFilter( "HLTHTCaloQuad",\nsaveTags = cms.bool( {} ),\nMaxMass = cms.double( {} ),\n MinHT = cms.double( {} ),\nMaxEta = cms.double( {} ),\n MinEta = cms.double( {} ),\nMinMass = cms.double( {} ),\ninputTag = cms.InputTag( {} ),\n MinE = cms.double( {} ),\n triggerType = cms.int32( {} ),\n MinPt = cms.double( {} )\n)\n'.format(saveTags , MaxMass, MinHT, MaxEta, MinEta, MinMass, inputTag, MinE, triggerType, MinPt)
    return module

def HLTHTPFDouble(saveTags = True , MaxMass = -1.0, MinHT=180, MaxEta=2.6, MinEta=-1.0, MinMass=-1.0, inputTag="'hltAK4PFJetsLooseIDCorrected'", MinE=-1.0, triggerType=86, MinPt=-1):
    module =  'cms.EDFilter( "HLTHTPFDouble", \nsaveTags = cms.bool( {} ), \nMaxMass = cms.double( {} ), \nMinHT = cms.double( {} ),\nMaxEta = cms.double( {} ),\nMinEta = cms.double( {} ),\nMinMass = cms.double( {} ),\ninputTag = cms.InputTag( {} ),\nMinE = cms.double( {} ),\ntriggerType = cms.int32( {} ),\nMinPt = cms.double( {} )\n)\n'.format(saveTags , MaxMass, MinHT, MaxEta, MinEta, MinMass, inputTag, MinE, triggerType, MinPt)
    return module

def HLTHTPFQuad(saveTags=True, MaxMass = -1.0, MinHT=90., MaxEta=2.6, MinEta=-10, MinMass = -1.0, inputTag = "'hltAK4PFJetsLooseIDCorrected'", MinE = -1.0, triggerType = 86 ,MinPt = -1 ):
    module = 'cms.EDFilter( "HLTHTPFQuad",\nsaveTags = cms.bool( {} ),\nMaxMass = cms.double( {} ),\n MinHT = cms.double( {} ),\nMaxEta = cms.double( {} ),\n MinEta = cms.double( {} ),\nMinMass = cms.double( {} ),\ninputTag = cms.InputTag( {} ),\n MinE = cms.double( {} ),\n triggerType = cms.int32( {} ),\n MinPt = cms.double( {} )\n)\n'.format(saveTags , MaxMass, MinHT, MaxEta, MinEta, MinMass, inputTag, MinE, triggerType, MinPt)
    return module

def HLT2DJetTagPF(saveTags = True , MinBTagBis = 0.7 , JetTags = "'hltDeepCombinedSecondaryVertexBJetTagsPF'", BTag = "'probb'", TriggerType = 86 , Jets =  "'hltPFJetForBtag'" , MinTag = 0.0 , MaxTag = 999999.0 ):
    module = 'cms.EDFilter( "HLT2DJetTagPF",\nsaveTags = cms.bool( {} ),\nMinBTagBis = cms.double( {} ),\nJetTags = cms.InputTag( {},{} ),\nTriggerType = cms.int32( {} ),\nJets = cms.InputTag( {} ),\nMinTag = cms.double( {} ),\nMaxTag = cms.double( {} )\n)\n'.format(saveTags, MinBTagBis, JetTags, BTag, TriggerType, Jets , MinTag, MaxTag)
    return module

def HLT2DJetTagCalo(saveTags = True , MinBTagBis = 0.7 , JetTags = "'hltDeepCombinedSecondaryVertexBJetTagsCalo'", BTag = "'probb'", TriggerType = 86 , Jets =  "'hltSelector8CentralJetsL1FastJet'" , MinTag = 0.0 , MaxTag = 999999.0 ):
    module = 'cms.EDFilter( "HLT2DJetTagCalo",\nsaveTags = cms.bool( {} ),\nMinBTagBis = cms.double( {} ),\nJetTags = cms.InputTag( {},{} ),\nTriggerType = cms.int32( {} ),\nJets = cms.InputTag( {} ),\nMinTag = cms.double( {} ),\nMaxTag = cms.double( {} )\n)\n'.format(saveTags, MinBTagBis, JetTags, BTag, TriggerType, Jets , MinTag, MaxTag)
    return module

def HLTBTagSumCalo(saveTags = True, Jets = "'hltSelector8CentralJetsL1FastJet'", JetTags = "'hltDeepCombinedSecondaryVertexBJetTagsCalo'", BTag = "'probb'", MinTag = 0.0,  MaxTag = 999999.0, TriggerType = 86 , MinEta = -1.0 , MaxEta = 2.5 , MinPt = 30.0, MaxPt = 999999.0 , SumN = 2, MinBTagSum = 1.3):
    module = 'cms.EDFilter("HLTBTagSumCalo",\nsaveTags = cms.bool( {} ),\nJets = cms.InputTag( {} ),\nJetTags = cms.InputTag( {},{} ),\nMinTag = cms.double( {} ),\nMaxTag = cms.double( {} ),\nTriggerType = cms.int32( {} ),\nMinEta = cms.double( {} ),\nMaxEta = cms.double( {} ),\nMinPt = cms.double( {} ),\nMaxPt = cms.double( {} ),\nSumN = cms.uint32({}),\nMinBTagSum = cms.double({})\n\n)\n'.format(saveTags, Jets, JetTags, BTag, MinTag,  MaxTag, TriggerType, MinEta, MaxEta, MinPt, MaxPt, SumN, MinBTagSum)
    return module

def HLTBTagSumPF(saveTags = True, Jets = "'hltPFJetForBtag'", JetTags = "'hltDeepCombinedSecondaryVertexBJetTagsPF'", BTag = "'probb'", MinTag = 0.0,  MaxTag = 999999.0, TriggerType = 86 , MinEta = -1.0 , MaxEta = 2.5 , MinPt = 30.0, MaxPt = 999999.0 , SumN = 2, MinBTagSum = 1.3):
    module = 'cms.EDFilter("HLTBTagSumPF",\nsaveTags = cms.bool( {} ),\nJets = cms.InputTag( {} ),\nJetTags = cms.InputTag( {},{} ),\nMinTag = cms.double( {} ),\nMaxTag = cms.double( {} ),\nTriggerType = cms.int32( {} ),\nMinEta = cms.double( {} ),\nMaxEta = cms.double( {} ),\nMinPt = cms.double( {} ),\nMaxPt = cms.double( {} ),\nSumN = cms.uint32({}),\nMinBTagSum = cms.double({})\n\n)\n'.format(saveTags, Jets, JetTags, BTag, MinTag,  MaxTag, TriggerType, MinEta, MaxEta, MinPt, MaxPt, SumN, MinBTagSum)
    return module

def FFNNHH4bCalo(saveTags = True, Jets = "'hltSelector8CentralJetsL1FastJet'", JetTags = "'hltDeepCombinedSecondaryVertexBJetTagsCalo'", BTag = "'probb'", MinTag = 0.0,  MaxTag = 999999.0, TriggerType = 86 , MinEta = -1.0 , MaxEta = 999999.0 , MinPt = 0.0, MaxPt = 999999.0 , WorkingPoint = 0.8, NNConfig="'models_json/FFNNHH_4b_lwtnn_2.json'"):
    module = 'cms.EDFilter("CaloNNHH4b",\nsaveTags = cms.bool( {} ),\nJets = cms.InputTag( {} ),\nJetTags = cms.InputTag( {},{} ),\nMinTag = cms.double( {} ),\nMaxTag = cms.double( {} ),\nTriggerType = cms.int32( {} ),\nMinEta = cms.double( {} ),\nMaxEta = cms.double( {} ),\nMinPt = cms.double( {} ),\nMaxPt = cms.double( {} ),\nWorkingPoint = cms.double({}),\nNNConfig = cms.FileInPath({})\n\n)\n'.format(saveTags, Jets, JetTags, BTag, MinTag,  MaxTag, TriggerType, MinEta, MaxEta, MinPt, MaxPt, WorkingPoint, NNConfig)
    return module

def MyHLTAnalyzer(triggerResults = "'TriggerResults'", TRsecond = "''", process = "'@currentProcess'", triggerList = "", filterList = "", verbose = False):
    module = 'cms.EDAnalyzer("HLTAn",\ntriggerResults = cms.InputTag({}, {}, {}),\ntriggerList = cms.vstring({}),\nfilterList = cms.vstring({}),\nverbose = cms.bool({})\n)\n'.format(triggerResults, TRsecond, process, triggerList, filterList, verbose)
    return module

def SaveAllJetsSignal(L1JetTag = "'hltGtStage2Digis'", L1Jet = "'Jet'", PFJetTag = "'hltAK4PFJetsLooseIDCorrected'", CaloJetTag = "'hltAK4CaloJetsCorrectedIDPassed'", PFBJetTag = "'hltDeepCombinedSecondaryVertexBJetTagsPF'", PFBTag = "'probb'", CaloBJetTag = "'hltDeepCombinedSecondaryVertexBJetTagsCalo'", CaloBTag = "'probb'", verbose = True, tree = "'Jets'"):
    module = 'cms.EDAnalyzer("SaveAllJets",\nL1JetTag = cms.InputTag( {},{} ),\nPFJetTag = cms.InputTag({}),\nCaloJetTag = cms.InputTag({}),\nPFBJetTag = cms.InputTag({},{}),\nCaloBJetTag = cms.InputTag({},{}),\nverbose = cms.bool({}),\ntree = cms.string({})\n'.format(L1JetTag, L1Jet, PFJetTag, CaloJetTag, PFBJetTag, PFBTag, CaloBJetTag, CaloBTag, verbose, tree)
    return module

def SaveAllJetsMC(L1JetTag = "'hltGtStage2Digis'", L1Jet = "'Jet'", PFJetTag = "'hltAK4PFJetsLooseIDCorrected'", GenJetTag = "'ak4GenJets'", GenNoNuJetTag = "'ak4GenJetsNoNu'", CaloJetTag = "'hltAK4CaloJetsCorrectedIDPassed'", PFBJetTag = "'hltDeepCombinedSecondaryVertexBJetTagsPF'", PFBTag = "'probb'", CaloBJetTag = "'hltDeepCombinedSecondaryVertexBJetTagsCalo'", CaloBTag = "'probb'", RecoTag = "'slimmedJets'", verbose = True, tree = "'Jets'"):
    module = 'cms.EDAnalyzer("SaveAllJetsRecoMC",\nL1JetTag = cms.InputTag( {},{} ),\nPFJetTag = cms.InputTag({}),\nGenJetTag = cms.InputTag({}),\nGenNoNuJetTag = cms.InputTag({}),\nCaloJetTag = cms.InputTag({}),\nPFBJetTag = cms.InputTag({},{}),\nCaloBJetTag = cms.InputTag({},{}),\nRecoTag = cms.InputTag({}),\nverbose = cms.bool({}),\ntree = cms.string({})\n)\n'.format(L1JetTag, L1Jet, PFJetTag, GenJetTag, GenNoNuJetTag, CaloJetTag, PFBJetTag, PFBTag, CaloBJetTag, CaloBTag, RecoTag, verbose, tree)
    return module

def SaveGenHH(genP = "'prunedGenParticles'", verbose = False, tree = "'Gen'"):
    module = 'cms.EDAnalyzer("SaveGenHH",\ngenP = cms.InputTag({}),\nverbose = cms.bool({}),\ntree = cms.string({})\n)\n'.format(genP, verbose, tree)
    return module
