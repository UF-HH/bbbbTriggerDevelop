import FWCore.ParameterSet.Config as cms
process = cms.Process('HARVESTING')
# read all the DQMIO files produced by the previous jobs
process.source = cms.Source("DQMRootSource",
fileNames = cms.untracked.vstring( "file:DQMIO.root" ) )
# DQMStore service
process.load('DQMServices.Core.DQMStore_cfi')
# FastTimerService client
process.load('HLTrigger.Timer.fastTimerServiceClient_cfi')
process.fastTimerServiceClient.dqmPath = "HLT/TimerService"
# DQM file saver
process.load('DQMServices.Components.DQMFileSaver_cfi')
process.dqmSaver.workflow = "/HLT/FastTimerService/All"
process.DQMFileSaverOutput = cms.EndPath( process.fastTimerServiceClient + process.dqmSaver )
