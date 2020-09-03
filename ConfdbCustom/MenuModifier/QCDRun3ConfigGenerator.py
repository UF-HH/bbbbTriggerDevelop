#--------------------------------------------------------------------------------------------------------------------------------
# author: Giacomo Boldrini
# This script reproduces the
# HLT menu with modules and
# triggers from the analysis
#
# example usage MC: python QCDRun3ConfigGenerator.py --menu=/dev/CMSSW_11_0_0/GRun/V7 -out=myHLT.py -gt=110X_mcRun3_2021_realistic_v6 -pr=myHLT \
# -paths=HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3,HLTriggerFinalPath,HLTAnalyzerEndpath --timing --nev 200000 --outfile QCD_15_7000_Run3_Flat_0_80.root --dataset /QCD_Pt-15to7000_TuneCP5_Flat_14TeV_pythia8/Run3Winter20DRMiniAOD-FlatPU0to80_110X_mcRun3_2021_realistic_v6-v1/GEN-SIM-RAW
#
# example usage Data: python QCDRun3ConfigGenerator.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT.py -pr=myHLT -paths=HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3,HLTriggerFinalPath,HLTAnalyzerEndpath --timing --tr 319941,316457,316058,315188,315106 --data
#
#--------------------------------------------------------------------------------------------------------------------------------

import os
import sys
import argparse
import shutil
from ModMenu import *
from QueryForFiles import *
from ModuleManager import ModMan
import argparse
import numpy as np

parser = argparse.ArgumentParser()
parser.add_argument('-gt', '--globaltag', type=str, required=True, help="globaltag")
parser.add_argument('-menu', '--menu', type=str, required=True, help="Menu config to be dumped")
parser.add_argument('-pr', '--process', type=str, required=False, default="MyHLT", help="Process Name")
parser.add_argument('-out', '--out', type=str, required=True, help="Output name of the menu created")
parser.add_argument('-data', '--data', default=False, action='store_true', required=False, help="data or mc")
parser.add_argument('-offline', '--offline', default=True, action='store_false', required=False, help="offline?")
parser.add_argument('-type', '--type', type=str, required=False, default="GRun", help="GRun?")
parser.add_argument('-paths', '--paths', type=str, required=False, help="Paths comma separated")
parser.add_argument('-timing', '--timing', default=True, action='store_false', required=False, help="do you want timing modules?")
parser.add_argument('-nev', '--nev', type=int, default=-1, required=False, help="Number of events to compute")
parser.add_argument('-prescale', '--prescale', type=float, required=False, help="Prescale")
parser.add_argument('-output', '--output', type=str, required=False, help="Outputmodules inside the menu")
parser.add_argument('-input', '--input', type=str, required=False, help="Input files")
parser.add_argument('-setup', '--setup', default="setup_cff", required=False, help="setup name")
parser.add_argument('-TimingRun', '--tr', type=str, required=False, help="select a run to query files")
parser.add_argument('-outfile', '--outfile', type=str, required=True, help="Name of output file")
parser.add_argument('-dataset', '--dataset', type=str, required=False, help="MC dataset to query")

args = parser.parse_args()

#if a timing run is required we query for files
if args.tr and args.data:
    args.tr = [int(item) for item in args.tr.split(',')]
    file_list = []
    for tr in args.tr:
        print("@[BeginQuery]: Query for timing files from run {}".format(tr))
        run , pu, ps, files = QueryForTimingFiles(tr)
        file_list += files
        args.prescale = ps
        print("@[EndQuery]: Queried for {} files".format(len(files)))

print("@[BeginJob]: Initiating... ")

to_os = "hltGetConfiguration {} --globaltag {} --process {} --full".format(args.menu, args.globaltag, args.process)

if args.data: to_os += " --data"
else: to_os += " --mc"
if args.offline: to_os += " --offline"
if args.type: to_os += " --type {}".format(args.type)
if args.paths: to_os += " --paths {}".format(args.paths)
if args.timing: to_os += " --timing"
if args.nev: to_os += " --max-events {}".format(args.nev)
if args.prescale: to_os += " --prescale {}".format(args.prescale)
else: to_os += " --prescale none"
if args.output: to_os += " --output {}".format(args.output)
else: to_os += " --output none"
if args.input: to_os += " --input {}".format(args.input)

to_os += " > {}".format(args.out)

print("@[Info]: Dumping menu with proposed inputs: ")
print(to_os)
os.system(to_os)

if os.path.isfile(args.out):
    menu_path = args.out 


print("@[Info]: Dumping Setup: ")
to_os = "hltConfigFromDB --cff --configName {} --nopaths --services -PrescaleService,-EvFDaqDirector,-FastMonitoringService > {}.py".format(args.menu, args.setup)
print(to_os)
os.system(to_os)

man = ModMan(menu_path)

#ADD MY MODULES FOR CUSTOMIZATION OF HLT MENU
print("@[Info]: Adding Imports and setup... ")

man.Insert("import FWCore.PythonUtilities.LumiList as LumiList\n", ind=3)
man.Insert("import FWCore.ParameterSet.Types as CfgTypes\n", ind=3)
man.SetCurrentLine(option_str="after:process = cms.Process(")
man.Insert('process.load("{}")\n'.format(args.setup))

print("@[Info]: Adding Analyzers... ")

man.Insert("#-------------My Analyzers-------------\n", ind=FindFirstSequence(menu_path)-1)
man.SetCurrentLine(option_str="after:#-------------My Analyzers-------------")
man.MakeSpace(n=20) #caveat, does not work without this, problem with indexing inside man...Need to work on this
man.SetCurrentLine(option_str="after:#-------------My Analyzers-------------")

if not args.data:
    man.CreateFromLocal(in_class="SaveRecoJ",mod_name="SaveRecoJ")
    man.InsertInMenu(in_class="SaveRecoJ",process_name = 'in_class')
    man.AddLuminosityToModule("SaveRecoJ") #MC no need to specify json but analyzer wants an input

    man.Insert('process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")')
    man.AddGenParticleProd()

    man.CreateFromLocal(in_class="SaveGenHH", mod_name="SaveGenHH")
    man.InsertInMenu(in_class="SaveGenHH",process_name = 'in_class')

    man.Insert("\n")

    man.CreateFromLocal(in_class="SaveAllJetsMC",mod_name="SaveAllJetsMC")
    man.InsertInMenu(in_class="SaveAllJetsMC",process_name = 'in_class')
    man.AddLuminosityToModule("SaveAllJetsMC") #MC no need to specify json but analyzer wants an input

    man.Insert("\n")

else:
    man.CreateFromLocal(in_class="SaveAllJets",mod_name="SaveAllJetsSignal")
    man.InsertInMenu(in_class="SaveAllJets",process_name = 'in_class')
    man.AddLuminosityToModule("SaveAllJets") #MC no need to specify json but analyzer wants an input

    man.Insert("\n")

man.Insert("#------------- Services ------------ \n")

print("@[Info]: Adding Services... ")

man.AddTFileService(file_name="{}".format(args.outfile))

man.SetCurrentLine(FindLastPath(menu_path))
man.Insert("\n")

#inserting my paths

print("@[Info]: Adding Paths... ")

man.Insert("#-----------------My Paths-----------------\n")

man.InsertPath("process.HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPrePFHT330PT30QuadPFJet75604540TriplePFBTagDeepCSV4p5 + process.HLTAK4CaloJetsSequence + process.hltQuadCentralJet30 + process.hltCaloJetsQuad30ForHt + process.hltHtMhtCaloJetsQuadC30 + process.hltCaloQuadJet30HT320 + process.HLTBtagDeepCSVSequenceL3 + process.hltBTagCaloDeepCSVp17Double + process.HLTAK4PFJetsSequence + process.hltPFCentralJetLooseIDQuad30 + process.hlt1PFCentralJetLooseID75 + process.hlt2PFCentralJetLooseID60 + process.hlt3PFCentralJetLooseID45 + process.hlt4PFCentralJetLooseID40 + process.hltPFCentralJetLooseIDQuad30forHt + process.hltHtMhtPFCentralJetsLooseIDQuadC30 + process.hltPFCentralJetsLooseIDQuad30HT330 + process.HLTBtagDeepCSVSequencePF + process.hltBTagPFDeepCSV4p5Triple + process.HLTEndSequence )\n")
#analyzers path
if not args.data:
    man.Insert("process.SaveGen = cms.Sequence( process.prunedGenParticles + process.SaveGenHH)\n")
    man.Insert("process.SaveRecoJets = cms.Path( process.HLTBeginSequence + process.SaveGen + process.SaveRecoJ + process.HLTEndSequence)\n")
    man.Insert("process.SaveJets = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPrePFHT330PT30QuadPFJet75604540TriplePFBTagDeepCSV4p5 +process.HLTAK4CaloJetsSequence + process.HLTBtagDeepCSVSequenceL3 + process.HLTAK4PFJetsSequence + process.HLTBtagDeepCSVSequencePF + process.SaveAllJetsMC + process.HLTEndSequence)\n") 
else:
    man.Insert("process.SaveJets = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPrePFHT330PT30QuadPFJet75604540TriplePFBTagDeepCSV4p5 +process.HLTAK4CaloJetsSequence + process.HLTBtagDeepCSVSequenceL3 + process.HLTAK4PFJetsSequence + process.HLTBtagDeepCSVSequencePF + process.SaveAllJets + process.HLTEndSequence)\n") 


if args.tr and args.data:
    print("@[Info]: Adding timing files as inputs... ")
    man.AddDASQuery(file_list)
elif args.dataset: 
    man.AddDASQueryMCDataset(args.dataset)

else:
    print("@[Info] Did not query any files...")


#adding last analyzer with trigger of interest:
man.SetCurrentLine(option_str="after:#-------------My Analyzers-------------")
trigger_list = "" 
trigger_names = man.GetAllPaths()
for name in trigger_names:
    trigger_list = trigger_list + "'{}',".format(name)
trigger_list = trigger_list[:-1]

man.CreateFromLocal(in_class="MyHLTAnalyzer",mod_name="MyHLTAnalyzer", triggerList = trigger_list)
man.InsertInMenu(in_class="MyHLTAnalyzer",process_name = 'in_class')
man.SetCurrentLine(option_str="before:process.SaveRecoJ.inputs")
man.AddLuminosityToModule("MyHLTAnalyzer", line=False) #MC no need to specify json but analyzer wants an input

man.AddModuleToPath("process.HLTAnalyzerEndpath", "process.MyHLTAnalyzer")

print("@[EndJob]: Done")