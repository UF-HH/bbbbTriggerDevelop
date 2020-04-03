# define basic process
import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import os

process = cms.Process("checkTrg")

# import standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')

process.load('Configuration.StandardSequences.EndOfProcess_cff')

options = VarParsing.VarParsing ('analysis')
options.inputFiles = []

options.register ('year',
                  2018, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,            # string, int, or float
                  "Year of the data taking")
options.register ('isData',
                  1, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,            # string, int, or float
                  "Is this real data (0 or 1)")
options.register ('onCRAB',
                  0, # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.int,            # string, int, or float
                  "Set to 1 if running on CRAB")
options.register ('out',
                  'dataset_general.root', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,            # string, int, or float
                  "path for output service file")

options.parseArguments()

### switches 
is_data = False if options.isData == 0 else True
year    = options.year

print '... running for the year : ', year
print '... is this data ?       : ', is_data
print '... out path        : ', options.out

# input
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

if options.inputFiles:
    Source_Files = cms.untracked.vstring(options.inputFiles)
elif year == 2016:
    Source_Files = cms.untracked.vstring(
        #'/store/data/Run2018A/SingleMuon/MINIAOD/17Sep2018-v2/00000/11697BCC-C4AB-204B-91A9-87F952F9F2C6.root',
    '/store/user/lcadamur/HH_2016_ggF_SM_miniAOD/8C5FF1BF-3C72-E911-BA91-0CC47A7C34B0.root',
    '/store/user/lcadamur/HH_2016_ggF_SM_miniAOD/A2391D24-2F73-E911-AE61-0CC47A4D7694.root',
    '/store/user/lcadamur/HH_2016_ggF_SM_miniAOD/FA8D404F-CB72-E911-BC8F-0CC47A4C8EE2.root'
)

elif year == 2018:
    Source_Files = cms.untracked.vstring(
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/04D8C95E-F2E1-C349-BADD-96CBF5E0BBF6.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/073225D4-5228-3948-ACBC-32CAEC3EF349.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/0C6717C2-FA2F-1B45-9E8E-439F904262C4.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/0D785969-E5CB-BC40-A1ED-C50645D20CFA.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/18C6BA7E-E6EE-7245-AAE1-5F5CB4307B11.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/1D2F6695-69B1-0D4A-8AC2-CEF73F5ABB90.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/20C33054-C7F1-5849-A9C9-3E5E7BCA8998.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/3DFA8FE6-01A1-9040-943D-45E7B278E493.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/54218005-FCF8-4941-A3E1-F8BC4FEAB78A.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/7664005B-1FF1-8040-8A6E-DD34266B2387.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/7C7D6A71-284D-834E-924C-A0A1A089D4FF.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/87C4DBB3-7FE6-BF4A-B5D1-41BD43ACB249.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/8C8C9A8A-A607-DC42-B9FA-5E1A0C9223D6.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/ADD9D6DA-20E7-AF42-B77B-599096AE9DC7.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/AE575998-6278-8345-AFD2-1F0BA47F0A54.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/D1FC0432-4C57-0345-815F-471A5F6D0637.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/D297112E-6382-C841-81F8-64179C1610B7.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/D2CC884A-8703-7D46-8AE8-33E1DB08D8AC.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/E543F337-45F2-4546-8321-75EAAEF0AD30.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/F518D1A3-87C3-F14A-A812-3DB78A3CC658.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/F74986B5-7568-EB4C-B20D-B3C23B48338F.root',
        '/store/user/lcadamur/HH_2018_ggF_SM_miniAOD/FB1FBA6E-9492-9F49-95AA-431B5EE13301.root'
    )

process.source = cms.Source("PoolSource",
    fileNames = Source_Files
)

from Configuration.AlCa.autoCond import autoCond
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")


## no need to use this on CRAB (onCRAB == 1), it will take care of handling the JSON filter
if is_data and options.onCRAB == 0:
    print '.... configuring manually the JSON filter for the year', year
    if year == 2016:
        jsonfile = 'jsons/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
    elif year == 2017:
        jsonfile = 'jsons/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt'
    elif year == 2018:
        jsonfile = 'jsons/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt'

    ## restrict the lumi sections
    import FWCore.PythonUtilities.LumiList as LumiList
    myLumis = LumiList.LumiList(filename = jsonfile).getCMSSWString().split(',')
    process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
    process.source.lumisToProcess.extend(myLumis)

process.TFileService = cms.Service('TFileService',
    fileName = cms.string(options.out)
)


process.checkTrg = cms.EDAnalyzer("TO_Tree_nob",
    puLabel = cms.InputTag("slimmedAddPileupInfo"),  # v1 miniaod addPileupInfo 
    triggerObjects = cms.InputTag("slimmedPatTrigger"),
    triggerResults = cms.InputTag("TriggerResults", "", "HLT"),
    reco_jets = cms.InputTag("slimmedJets"),
    genP = cms.InputTag("prunedGenParticles"),
    triggerList = cms.vstring(),
    filterList  = cms.vstring(),
    verbose     = cms.bool(False)
)


## will update the trigger and filter list
from bbbbTrg_nob.bbbbTrg.customize_trg_config import *
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
from PhysicsTools.PatAlgos.tools.helpers import getPatAlgosToolsTask


if year == 2016:
    #configuring global tags
    process.GlobalTag.globaltag = '94X_mcRun2_asymptotic_v3'
    #trigger paths
    customize_trg_config_2016(process)
    #b-tagging infos
    updateJetCollection(
    process,
    jetSource = cms.InputTag('slimmedJets'),
    pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
    svSource = cms.InputTag('slimmedSecondaryVertices'),
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
    btagDiscriminators = [
        'pfDeepFlavourJetTags:probb',
        'pfDeepFlavourJetTags:probbb',
        'pfDeepFlavourJetTags:problepb',
        'pfDeepFlavourJetTags:probc',
        'pfDeepFlavourJetTags:probuds',
        'pfDeepFlavourJetTags:probg'
        ],
    postfix='NewDFTraining',
    btagPrefix = 'new'
    )
    process.checkTrg.reco_jets = cms.InputTag("updatedPatJetsTransientCorrectedNewDFTraining")
    patAlgosToolsTask = getPatAlgosToolsTask(process)
    
elif year == 2017:
    customize_trg_config_2017(process)
elif year == 2018:
    #configuring global tags
    process.GlobalTag.globaltag = '102X_upgrade2018_realistic_v20'
    #configuring triggers
    customize_trg_config_2018(process)
else:
    raise RuntimeError("year not valid")


process.torun = cms.Path(
    process.checkTrg
)

process.schedule = cms.Schedule(process.torun)


##### screen output stuff
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
)

# Silence output
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
