#--------------------------------------------------------------------------------------------------------------------------------
# author: Giacomo Boldrini
# This script reproduces the
# HLT menu with modules and
# triggers from the analysis
#
# example usage MC: python NNConfigGenerator.py --menu=/dev/CMSSW_11_0_0/GRun/V7 -out=myHLT.py -gt=110X_mcRun3_2021_realistic_v6 -pr=myHLT -paths=HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3,HLTriggerFinalPath,HLTAnalyzerEndpath --timing --outfile 
# example usage Data: python NNConfigGenerator.py --menu=/dev/CMSSW_10_1_0/GRun -gt=101X_dataRun2_HLT_v7 -out=myHLT.py -pr=myHLT -paths=HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3,HLTriggerFinalPath,HLTAnalyzerEndpath --timing  --data --run 323725,323725 --ls [18,22],[37,57] --outfile NNRateEstimate.root --json ../test/Json/json_2018D_Ephemeral_20181022_PU50.txt
# example usage Data online menu (no HLTAnalyzerEndPath): python NNConfigGenerator.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT.py -pr=myHLT -paths=HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3,HLTriggerFinalPath,HLTAnalyzerEndpath --timing  --data --run 323725,323725 --ls [18,22],[37,57] --outfile NNOnlineRateEstimate.root --json ../test/Json/json_2018D_Ephemeral_20181022_PU50.txt
# example usage Data for Timing: python NNConfigGenerator.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=myHLT -paths=HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3,HLTriggerFinalPath,HLTAnalyzerEndpath --data --tr 319941 --outfile NNTimingRateEstimate.root 
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

def lumisectionGenerator(lumi_str):
    lumi_str = lumi_str.split(',')
    final_list = []
    for entry in lumi_str:
        if '[' in entry :
            final_list.append(int(entry.replace('[', '')))
        elif ']' in entry :
            final_list.append(int(entry.replace(']', '')))
        else:
            continue
    #grouping by two
    final_list = [final_list[i:i+2] for i in range(0, len(final_list), 2)]
    tot_list = []
    for item in final_list:
        tot_list.append([i for i in range(item[0], item[1]+1)])
    return tot_list

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
parser.add_argument('-run', '--run', type=str, required=False, help="select a run to query files")
parser.add_argument('-ls', '--ls', type=str, required=False, help="select a LS to query files")
parser.add_argument('-outfile', '--outfile', type=str, required=True, help="Name of output file")
parser.add_argument('-json', '--json', type=str, required=False, help="Name of golden json if data")
parser.add_argument('-tr', '--tr', type=str, required=False, help="Timing Run selection")

args = parser.parse_args()

#if a run is required we query for files
if args.run and args.data:
    args.run = [int(item) for item in args.run.split(',')]
    if args.ls:
        args.ls = lumisectionGenerator(args.ls)
    else:
        args.ls = [None]*len(args.tr)
    
    assert len(args.ls) == len(args.run)
    file_list = []
    for tr,ls_ in zip(args.run, args.ls):
        print("@[BeginQuery]: Query for DATA files from run {}, LS: {}".format(tr,ls_))
        files = QueryFilesFromRuns([tr], ls_)
        file_list += files
        print("@[EndQuery]: Queried for {} files".format(len(files)))

    print("@[EndQuery]: Queried for a total of: {} DATA files".format(len(file_list)))

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

if args.data: to_os += " --data --l1-emulator uGT --l1 L1Menu_Collisions2018_v1_0_0-d1_xml"
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
man.MakeSpace(n=120) #caveat, does not work without this, problem with indexing inside man...Need to work on this
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

else:
    man.CreateFromLocal(in_class="SaveAllJets",mod_name="SaveAllJetsSignal")
    man.InsertInMenu(in_class="SaveAllJets",process_name = 'in_class')
    #man.AddLuminosityToModule("SaveAllJets") #MC no need to specify json but analyzer wants an input

    man.Insert("\n")

man.Insert("#------------- My Filters -------------------\n")

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
#man.InsertPrescaleModule('hltFFNNHH4bCalowp0p87', offset=0, name='strip')
#man.InsertPrescaleModule('hltCaloCNNT4HighPU0p9993', offset=0, name='strip')
#man.InsertPrescaleModule('hltCaloDNNprova', offset=0, name='strip')
#man.InsertPrescaleModule('hltKinFFCaloHH4bwp0p5BTagFFCaloHH4bwp0p85', offset=0, name='strip')

#This module is present in /dev/CMSSW_11_0_0/GRun/V7, does not work if not present in the menu
man.CloneModule("process.hltQuadCentralJet30", in_class="hltDoubleCentralJet60")
man.ModifyPar("hltDoubleCentralJet60", "MaxEta", 2.5)
man.ModifyPar("hltDoubleCentralJet60", "MinPt", 60)
man.ModifyPar("hltDoubleCentralJet60", "MinN", 2)
man.ModifyPar("hltDoubleCentralJet60", "inputTag", "'hltAK4CaloJetsCorrectedIDPassed'")
man.ModifyPar("hltDoubleCentralJet60", "triggerType", 86)
man.InsertInMenu(in_class="hltDoubleCentralJet60",process_name = 'in_class')



#creating range for the NN, varying thresholds
calo_names_th = []
for th in np.arange(0.8, 0.98 , 0.01):
    th = round(th, 2)
    #calos
    inclass_name = "FFNNHH4bCalowp{}".format(str(th).replace(".", "p"))
    calo_names_th.append([inclass_name, th])
    man.CreateFromLocal(in_class=inclass_name,mod_name="FFNNHH4bCalo", WorkingPoint=th)
    man.InsertInMenu(in_class=inclass_name,process_name = 'in_class')


if "HLTAnalyzerEndpath" in args.paths and "online" in args.menu:
    print("[Info]: online menu does not contain HLTAnalyzerEndpath as requested... Dumping from offline...")
    man.AddHLTAnalyzerEndPathModules()

man.Insert("\n")


man.Insert("#------------- Services ------------ \n")

print("@[Info]: Adding Services... ")

man.AddTFileService(file_name="{}".format(args.outfile))

man.SetCurrentLine(FindLastPath(menu_path))
man.Insert("\n")

#inserting my paths

print("@[Info]: Adding Paths... ")

man.Insert("#-----------------My Paths-----------------\n")
#New filters based


for calo in calo_names_th:
    th = str(calo[1]).replace(".", "p")
    man.InsertPath("process.HLT_FFNNHHwp{} = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.HLTAK4CaloJetsSequence + process.HLTBtagDeepCSVSequenceL3 + process.{} + process.HLTEndSequence )\n".format(th, calo[0]))


#Benchmark +  analyzer
man.InsertPath("process.HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPrePFHT330PT30QuadPFJet75604540TriplePFBTagDeepCSV4p5 + process.HLTAK4CaloJetsSequence + process.hltQuadCentralJet30 + process.hltCaloJetsQuad30ForHt + process.hltHtMhtCaloJetsQuadC30 + process.hltCaloQuadJet30HT320 + process.HLTBtagDeepCSVSequenceL3 + process.hltBTagCaloDeepCSVp17Double + process.HLTAK4PFJetsSequence + process.hltPFCentralJetLooseIDQuad30 + process.hlt1PFCentralJetLooseID75 + process.hlt2PFCentralJetLooseID60 + process.hlt3PFCentralJetLooseID45 + process.hlt4PFCentralJetLooseID40 + process.hltPFCentralJetLooseIDQuad30forHt + process.hltHtMhtPFCentralJetsLooseIDQuadC30 + process.hltPFCentralJetsLooseIDQuad30HT330 + process.HLTBtagDeepCSVSequencePF + process.hltBTagPFDeepCSV4p5Triple + process.HLTEndSequence )\n")
#man.InsertPath("process.HLT_HLT_FFNNHHwp0p87 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.HLTAK4CaloJetsSequence + process.HLTBtagDeepCSVSequenceL3 + process.FFNNHH4bCalowp0p87 + process.HLTEndSequence )\n")

#analyzers path
if not args.data:
    man.Insert("process.SaveGen = cms.Sequence( process.prunedGenParticles + process.SaveGenHH)\n")
    man.Insert("process.SaveJets = cms.Path( process.HLTBeginSequence + process.SaveGen + process.SaveRecoJ + process.HLTEndSequence)\n")
else:
    man.Insert("process.SaveJets = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPrePFHT330PT30QuadPFJet75604540TriplePFBTagDeepCSV4p5 +process.HLTAK4CaloJetsSequence + process.HLTBtagDeepCSVSequenceL3 + process.HLTAK4PFJetsSequence + process.HLTBtagDeepCSVSequencePF + process.SaveAllJets + process.HLTEndSequence)\n") 

if "HLTAnalyzerEndpath" in args.paths and "online" in args.menu:
    man.AddHLTAnalyzerEndPath()

if args.run and args.data:
    print("@[Info]: Adding DATA files as inputs... ")
    man.AddDASQuery(file_list)
elif args.tr and args.data:
    print("@[Info]: Adding Timing DATA files as inputs... ")
    man.AddDASQuery(file_list)
else: man.AddDASQueryMC()

#adding last analyzer with trigger of interest:
man.SetCurrentLine(option_str="after:#-------------My Analyzers-------------")
trigger_list = "" 
trigger_names = man.GetAllPaths()
for name in trigger_names:
    trigger_list = trigger_list + "'{}',".format(name)
trigger_list = trigger_list[:-1]

man.CreateFromLocal(in_class="MyHLTAnalyzer",mod_name="MyHLTAnalyzer", triggerList = trigger_list)
man.InsertInMenu(in_class="MyHLTAnalyzer",process_name = 'in_class')
#man.SetCurrentLine(option_str="before:process.SaveRecoJ.inputs")
man.AddModuleToPath("process.HLTAnalyzerEndpath", "process.MyHLTAnalyzer")

man.SetCurrentLine(option_str="after:#------------- My Filters -------------------")

if args.json:
    man.AddLuminosityJson(args.json)

man.AddLuminosityToModule("MyHLTAnalyzer", line=False) #MC no need to specify json but analyzer wants an input
man.AddLuminosityToModule("SaveAllJets", line=False) #MC no need to specify json but analyzer wants an input

"""
if args.json:
    man.AddLuminosityJson(args.json, extend=True, process='MyHLTAnalyzer')
    man.AddLuminosityJson(args.json, extend=True, process='SaveJets')
"""
print("@[EndJob]: Done")

