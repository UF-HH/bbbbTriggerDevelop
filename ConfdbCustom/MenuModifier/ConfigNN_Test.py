#--------------------------------------------------------------------------------------------------------------------------------
# author: Giacomo Boldrini
# This script reproduces the full
# HLT menu 11_0_0 with NNmodules and
# Specific implementation for timing 
# studies. Runs on vocms003
# Example usage for files under vocms003 with pile up 48-49 (run 319941)
# python ConfigNN_Test.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT.py -pr=TIMING -out=myHLT.py -nev=50000 -data --l1 L1Menu_Collisions2018_v2_1_0-d1_xml --tr=319941
#
#--------------------------------------------------------------------------------------------------------------------------------

import os
import sys
import argparse
import shutil
from ModMenu import *
from ModuleManager import ModMan
from QueryForFiles import *
import argparse

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
parser.add_argument('-prescale', '--prescale', type=str, required=False, help="Prescale")
parser.add_argument('-output', '--output', type=str, required=False, help="Outputmodules inside the menu")
parser.add_argument('-input', '--input', type=str, required=False, help="Input files")
parser.add_argument('-setup', '--setup', default="setup_cff", required=False, help="setup name")
parser.add_argument('-l1', '--l1', type=str, required=False, help="l1 xml config")
parser.add_argument('-tr', '--tr', required=False, help="select a run to query files")

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
if args.l1: to_os += " --l1 {}".format(args.l1)

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

man.Insert("#------------- My Filters -------------------\n", ind=FindFirstSequence(menu_path)-1)
man.SetCurrentLine(option_str="after:#------------- My Filters -------------------")
man.MakeSpace(n=20) #caveat, does not work without this, problem with indexing inside man...Need to work on this

print("@[Info]: Adding Filters definintions... ")

man.SetCurrentLine("after:#------------- My Filters -------------------")

man.CreateFromLocal(in_class="FFNNHH4bCalowp0p87",mod_name="FFNNHH4bCalo")
man.InsertInMenu(in_class="FFNNHH4bCalowp0p87",process_name = 'in_class')

man.CreateFromLocal(in_class="CaloCNNT4HighPU0p9993",mod_name="CaloCNNT4HighPU")
man.InsertInMenu(in_class="CaloCNNT4HighPU0p9993",process_name = 'in_class')

man.CreateFromLocal(in_class="DNNProva0p87",mod_name="CaloDNNProva")
man.InsertInMenu(in_class="DNNProva0p87",process_name = 'in_class')

man.CreateFromLocal(in_class="KinFFCaloHH4bwp0p5",mod_name="KinFFCaloHH4b")
man.InsertInMenu(in_class="KinFFCaloHH4bwp0p5",process_name = 'in_class')

man.CreateFromLocal(in_class="BTagFFCaloHH4bwp0p85",mod_name="BTagFFCaloHH4b")
man.InsertInMenu(in_class="BTagFFCaloHH4bwp0p85",process_name = 'in_class')

#This module is present in /dev/CMSSW_11_0_0/GRun/V7, does not work if not present in the menu
man.CloneModule("process.hltQuadCentralJet30", in_class="hltDoubleCentralJet60")
man.ModifyPar("hltDoubleCentralJet60", "MaxEta", 2.5)
man.ModifyPar("hltDoubleCentralJet60", "MinN", 2)
man.ModifyPar("hltDoubleCentralJet60", "MinPt", 60)
man.ModifyPar("hltDoubleCentralJet60", "inputTag", "'hltAK4CaloJetsCorrectedIDPassed'")
man.ModifyPar("hltDoubleCentralJet60", "triggerType", 86)
man.InsertInMenu(in_class="hltDoubleCentralJet60",process_name = 'in_class')

#Prescale for my paths
man.InsertPrescaleModule('hltFFNNHH4bCalowp0p87', offset=0, name='strip')
man.InsertPrescaleModule('hltCaloCNNT4HighPU0p9993', offset=0, name='strip')
man.InsertPrescaleModule('hltCaloDNNprova', offset=0, name='strip')
man.InsertPrescaleModule('hltKinFFCaloHH4bwp0p5BTagFFCaloHH4bwp0p85', offset=0, name='strip')

#inserting my paths

print("@[Info]: Adding Paths... ")

man.SetCurrentLine(FindLastPath(menu_path)) #Need to add after HLTBeginSequence
man.Insert("\n")

man.Insert("#-----------------My Paths-----------------\n")
#Benchmark 2018
man.Insert("process.HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPrePFHT330PT30QuadPFJet75604540TriplePFBTagDeepCSV4p5 + process.HLTAK4CaloJetsSequence + process.hltQuadCentralJet30 + process.hltCaloJetsQuad30ForHt + process.hltHtMhtCaloJetsQuadC30 + process.hltCaloQuadJet30HT320 + process.HLTBtagDeepCSVSequenceL3 + process.hltBTagCaloDeepCSVp17Double + process.HLTAK4PFJetsSequence + process.hltPFCentralJetLooseIDQuad30 + process.hlt1PFCentralJetLooseID75 + process.hlt2PFCentralJetLooseID60 + process.hlt3PFCentralJetLooseID45 + process.hlt4PFCentralJetLooseID40 + process.hltPFCentralJetLooseIDQuad30forHt + process.hltHtMhtPFCentralJetsLooseIDQuadC30 + process.hltPFCentralJetsLooseIDQuad30HT330 + process.HLTBtagDeepCSVSequencePF + process.hltBTagPFDeepCSV4p5Triple + process.HLTEndSequence )\n")
#Inserting truly new paths
man.Insert("process.HLT_Quad30Double60_FFNNHH4bwp0p87Calo_PFQuad30 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPreFFNNHH4bCalowp0p87 + process.HLTAK4CaloJetsSequence + process.HLTBtagDeepCSVSequenceL3 + process.FFNNHH4bCalowp0p87 + process.HLTEndSequence )\n")
man.Insert("process.HLT_CaloDNNT4_HighPU0p87 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPreCaloDNNprova + process.HLTAK4CaloJetsSequence + process.HLTBtagDeepCSVSequenceL3 + process.DNNProva0p87 + process.HLTEndSequence )\n")
man.Insert("process.HLT_CaloCNNT4_HighPU0p9993 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPreCaloCNNT4HighPU0p9993 + process.HLTAK4CaloJetsSequence + process.HLTBtagDeepCSVSequenceL3 + process.CaloCNNT4HighPU0p9993 + process.HLTEndSequence )\n")
man.Insert("process.HLT_KinFFCaloHH4bwp0p5_BTagFFCaloHH4bwp0p85 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPreKinFFCaloHH4bwp0p5BTagFFCaloHH4bwp0p85 + process.HLTAK4CaloJetsSequence + process.HLTBtagDeepCSVSequenceL3 + process.KinFFCaloHH4bwp0p5 + process.BTagFFCaloHH4bwp0p85 + process.HLTEndSequence )\n")

if args.tr:
    print("@[Info]: Adding timing files as inputs... ")
    man.AddDASQuery(files)


print("@[EndJob]: Done")
