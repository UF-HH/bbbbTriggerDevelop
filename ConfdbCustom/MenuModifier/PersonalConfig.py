#--------------------------------------------------------------------------------------------------------------------------------
# author: Giacomo Boldrini
# This script reproduces the
# HLT menu with modules and
# triggers from the analysis
#
# example usage: python PersonalConfig.py --menu=/dev/CMSSW_11_0_0/GRun/V7 -out=myHLT.py -gt=110X_mcRun3_2021_realistic_v6-v2 -pr=myHLT -out=myHLT.py
#
#--------------------------------------------------------------------------------------------------------------------------------

import os
import sys
import argparse
import shutil
from ModMenu import *
from ModuleManager import ModMan
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
parser.add_argument('-prescale', '--prescale', type=float, required=False, help="Prescale")
parser.add_argument('-output', '--output', type=str, required=False, help="Outputmodules inside the menu")
parser.add_argument('-input', '--input', type=str, required=False, help="Input files")

args = parser.parse_args()

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

man = ModMan(menu_path)

#ADD MY MODULES FOR CUSTOMIZATION OF HLT MENU
print("@[Info]: Adding Analyzers... ")

man.Insert("import FWCore.PythonUtilities.LumiList as LumiList\n", ind=3)
man.Insert("import FWCore.ParameterSet.Types as CfgTypes\n", ind=3)
man.Insert("#-------------My Analyzers-------------\n", ind=FindFirstSequence(menu_path)-1)
man.SetCurrentLine(option_str="after:#-------------My Analyzers-------------")
man.MakeSpace(n=20) #caveat, does not work without this, problem with indexing inside man...Need to work on this
man.SetCurrentLine(option_str="after:#-------------My Analyzers-------------")

trigger_list = "'HLT_Quad30_Double60_Sum2LeadingBTag015','HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3','HLT_PFHT270_180_Double180_Double90_BisectorBTag07'"

man.CreateFromLocal(in_class="MyHLTAnalyzer",mod_name="MyHLTAnalyzer", triggerList = trigger_list)
man.InsertInMenu(in_class="MyHLTAnalyzer",process_name = 'in_class')
man.AddLuminosityToModule("MyHLTAnalyzer", line=False) #MC no need to specify json but analyzer wants an input

man.CreateFromLocal(in_class="SaveAllJetsMC",mod_name="SaveAllJetsMC")
man.InsertInMenu(in_class="SaveAllJetsMC",process_name = 'in_class')
man.AddLuminosityToModule("SaveAllJetsMC") #MC no need to specify json but analyzer wants an input

man.Insert('process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")')
man.AddGenParticleProd()

man.CreateFromLocal(in_class="SaveGenHH", mod_name="SaveGenHH")
man.InsertInMenu(in_class="SaveGenHH",process_name = 'in_class')

man.Insert("\n")

man.Insert("#------------- My Filters -------------------\n")

print("@[Info]: Adding Filters definintions... ")

man.SetCurrentLine("after:#------------- My Filters -------------------")
man.CreateFromLocal(in_class="hltDoubleCentralCaloHT180",mod_name="HLTHTCaloDouble")
man.InsertInMenu(in_class="hltDoubleCentralCaloHT180",process_name = 'in_class')

man.CreateFromLocal(in_class="hltDoubleCentralCaloHT90",mod_name="HLTHTCaloQuad")
man.InsertInMenu(in_class="hltDoubleCentralCaloHT90",process_name = 'in_class')

man.CreateFromLocal(in_class="hltDoubleCentralPFHT180",mod_name="HLTHTPFDouble")
man.InsertInMenu(in_class="hltDoubleCentralPFHT180",process_name = 'in_class')

man.CreateFromLocal(in_class="hltDoubleCentralPFHT90",mod_name="HLTHTPFQuad")
man.InsertInMenu(in_class="hltDoubleCentralPFHT90",process_name = 'in_class')

man.CreateFromLocal(in_class="hltBTagBisector23PF",mod_name="HLT2DJetTagPF")
man.InsertInMenu(in_class="hltBTagBisector23PF",process_name = 'in_class')

man.CreateFromLocal(in_class="hltBTagBisector23Calo",mod_name="HLT2DJetTagCalo")
man.InsertInMenu(in_class="hltBTagBisector23Calo",process_name = 'in_class')

#This module is present in /dev/CMSSW_11_0_0/GRun/V7, does not work if not present in the menu
man.CloneModule("process.hltDoubleCentralCaloJetpt60", in_class="hltDoubleCentralJet60")
man.ModifyPar("hltDoubleCentralJet60", "MaxEta", 2.5)
man.ModifyPar("hltDoubleCentralJet60", "inputTag", "'hltAK4CaloJetsCorrectedIDPassed'")
man.ModifyPar("hltDoubleCentralJet60", "triggerType", 86)
man.InsertInMenu(in_class="hltDoubleCentralJet60",process_name = 'in_class')

man.CloneModule("process.hltDoubleCentralJet60", in_class="hlt2PFCentralJetLooseID60")
man.ModifyPar("hlt2PFCentralJetLooseID60", "inputTag", "'hltAK4PFJetsLooseIDCorrected'")
man.InsertInMenu(in_class="hlt2PFCentralJetLooseID60",process_name = 'in_class')

man.CreateFromLocal(in_class="hltDoubleLeadingBTagSumCentralJet30",mod_name="HLTBTagSumCalo")
man.InsertInMenu(in_class="hltDoubleLeadingBTagSumCentralJet30",process_name = 'in_class')

man.CreateFromLocal(in_class="hltDoublePFLeadingBTagSumCentralJet30",mod_name="HLTBTagSumPF")
man.InsertInMenu(in_class="hltDoublePFLeadingBTagSumCentralJet30",process_name = 'in_class')

man.CreateFromLocal(in_class="hltDoublePFLeadingBTagSumCentralJet30",mod_name="HLTBTagSumPF")
man.InsertInMenu(in_class="hltDoublePFLeadingBTagSumCentralJet30",process_name = 'in_class')

man.Insert("\n")
man.Insert("#------------- Services ------------ \n")

print("@[Info]: Adding Services... ")

man.AddTFileService(file_name="prova.root")

man.SetCurrentLine(FindLastPath(menu_path))
man.Insert("\n")

#inserting my paths

print("@[Info]: Adding Paths... ")

man.Insert("#-----------------My Paths-----------------\n")
man.Insert("process.HLT_Quad30_Double60_Sum2LeadingBTag015 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPrePFHT330PT30QuadPFJet75604540TriplePFBTagDeepCSV4p5 + process.HLTAK4CaloJetsSequence + process.hltQuadCentralJet30 + process.hltDoubleCentralJet60 + process.HLTBtagDeepCSVSequenceL3 + process.hltDoubleLeadingBTagSumCentralJet30 + process.HLTAK4PFJetsSequence + process.hltPFCentralJetLooseIDQuad30 + process.hlt2PFCentralJetLooseID60 + process.HLTBtagDeepCSVSequencePF + process.hltDoublePFLeadingBTagSumCentralJet30  + process.HLTEndSequence )\n")
#man.Insert("process.HLT_PFHT270_180_Double180_Double90_BisectorBTag07 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPrePFHT330PT30QuadPFJet75604540TriplePFBTagDeepCSV4p5 + process.HLTAK4CaloJetsSequence + process.hltDoubleCentralCaloHT180 + process.hltDoubleCentralCaloHT90 + process.HLTBtagDeepCSVSequenceL3 + process.hltBTagBisector23Calo + process.HLTAK4PFJetsSequence + process.hltDoubleCentralPFHT180 + process.hltDoubleCentralPFHT90  + process.HLTBtagDeepCSVSequencePF + process.hltBTagBisector23PF + process.HLTEndSequence )\n")
man.Insert("process.HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPrePFHT330PT30QuadPFJet75604540TriplePFBTagDeepCSV4p5 + process.HLTAK4CaloJetsSequence + process.hltQuadCentralJet30 + process.hltCaloJetsQuad30ForHt + process.hltHtMhtCaloJetsQuadC30 + process.hltCaloQuadJet30HT320 + process.HLTBtagDeepCSVSequenceL3 + process.hltBTagCaloDeepCSVp17Double + process.HLTAK4PFJetsSequence + process.hltPFCentralJetLooseIDQuad30 + process.hlt1PFCentralJetLooseID75 + process.hlt2PFCentralJetLooseID60 + process.hlt3PFCentralJetLooseID45 + process.hlt4PFCentralJetLooseID40 + process.hltPFCentralJetLooseIDQuad30forHt + process.hltHtMhtPFCentralJetsLooseIDQuadC30 + process.hltPFCentralJetsLooseIDQuad30HT330 + process.HLTBtagDeepCSVSequencePF + process.hltBTagPFDeepCSV4p5Triple + process.HLTEndSequence )\n")
man.Insert("process.SaveGen = cms.Sequence( process.prunedGenParticles + process.SaveGenHH)\n")
man.Insert("process.SaveJets = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPrePFHT330PT30QuadPFJet75604540TriplePFBTagDeepCSV4p5 +process.HLTAK4CaloJetsSequence + process.HLTBtagDeepCSVSequenceL3 + process.HLTAK4PFJetsSequence + process.HLTBtagDeepCSVSequencePF + process.SaveGen + process.SaveAllJetsMC + process.HLTEndSequence)\n")

print("@[Info]: Adding Query for MC samples... ")
man.AddDASQuery()

print("@[EndJob]: Done")







