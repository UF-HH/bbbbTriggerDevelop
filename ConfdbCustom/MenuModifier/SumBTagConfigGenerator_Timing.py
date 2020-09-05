#--------------------------------------------------------------------------------------------------------------------------------
# author: Giacomo Boldrini
# This script reproduces the
# HLT menu with modules and
# triggers from the analysis
#
# example usage MC: python SumBTagConfigGenerator.py --menu=/dev/CMSSW_11_0_0/GRun/V7 -out=myHLT.py -gt=110X_mcRun3_2021_realistic_v6 -pr=myHLT -paths=HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3,HLTriggerFinalPath,HLTAnalyzerEndpath --timing --outfile 
# example usage Data: python SumBTagConfigGenerator.py --menu=/dev/CMSSW_10_1_0/GRun -gt=101X_dataRun2_HLT_v7 -out=myHLT.py -pr=myHLT -paths=HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3,HLTriggerFinalPath,HLTAnalyzerEndpath --timing  --data --run 323725,323725 --ls [18,22],[37,57] --outfile SumBTagRateEstimate.root --json ../test/Json/json_2018D_Ephemeral_20181022_PU50.txt
# example usage Data online menu (no HLTAnalyzerEndPath): python SumBTagConfigGenerator.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT.py -pr=myHLT -paths=HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3,HLTriggerFinalPath,HLTAnalyzerEndpath --timing  --data --run 323725,323725 --ls [18,22],[37,57] --outfile SumBTagOnlineRateEstimate.root --json ../test/Json/json_2018D_Ephemeral_20181022_PU50.txt
# example usage Data for Timing: python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=myHLT --data --tr 319941 --th 0p9
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
parser.add_argument('-json', '--json', type=str, required=False, help="Name of golden json if data")
parser.add_argument('-tr', '--tr', type=str, required=False, help="Timing Run selection")
parser.add_argument('-th', '--th', type=str, required=False, help="Threshold for btag sum trigger", default='0p9')
parser.add_argument('-empty', '--empty', default=False, help="No new paths", default='0p9', action='store_true')

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

#if args.data: to_os += " --data --l1-emulator uGT --l1 L1Menu_Collisions2018_v1_0_0-d1_xml"
if args.data: to_os += " --data --l1 L1Menu_Collisions2018_v2_1_0-d1_xml"
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


man.Insert("#------------- My Filters -------------------\n", ind=FindFirstSequence(menu_path)-1)
man.SetCurrentLine(option_str="after:#------------- My Filters -------------------")
man.MakeSpace(n=30) #caveat, does not work without this, problem with indexing inside man...Need to work on this
man.SetCurrentLine(option_str="after:#------------- My Filters -------------------\n")

print("@[Info]: Adding Filters definintions... ")

#This module is present in /dev/CMSSW_11_0_0/GRun/V7, does not work if not present in the menu
man.CloneModule("process.hltQuadCentralJet30", in_class="hltDoubleCentralJet60")
man.ModifyPar("hltDoubleCentralJet60", "MaxEta", 2.5)
man.ModifyPar("hltDoubleCentralJet60", "MinPt", 60)
man.ModifyPar("hltDoubleCentralJet60", "MinN", 2)
man.ModifyPar("hltDoubleCentralJet60", "inputTag", "'hltAK4CaloJetsCorrectedIDPassed'")
man.ModifyPar("hltDoubleCentralJet60", "triggerType", 86)
man.InsertInMenu(in_class="hltDoubleCentralJet60",process_name = 'in_class')

#creating range for the leading 2 btag sum, varying thresholds
calo_name = "hltDoubleLeadingBTagSumCentralJet30th{}".format(args.th)
pf_name = "hltDoublePFLeadingBTagSumCentralJet30th{}".format(args.th)

float_th = float(args.th.replace('p', '.'))

#calos

man.CreateFromLocal(in_class=calo_name,mod_name="HLTBTagSumCalo", MinBTagSum=float_th)
man.InsertInMenu(in_class=calo_name, process_name = 'in_class')
#pfs

man.CreateFromLocal(in_class=pf_name,mod_name="HLTBTagSumPF", MinBTagSum=float_th)
man.InsertInMenu(in_class=pf_name, process_name = 'in_class')

man.InsertPrescaleModule('hltSumBtag', offset=0, name='strip')

#Adding lumi module
man.addLumiHistos()


#inserting my paths
man.SetCurrentLine(FindLastPath(menu_path))
man.Insert("\n")
print("@[Info]: Adding Paths... ")

if not args.empty:
    man.Insert("#-----------------My Paths-----------------\n")
    #New filters based

    man.InsertPath("process.HLT_Quad30_Double60_Sum2LeadingBTag{} = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPreSumBtag + process.HLTAK4CaloJetsSequence + process.hltQuadCentralJet30 + process.hltDoubleCentralJet60 + process.HLTBtagDeepCSVSequenceL3 + process.{} + process.HLTAK4PFJetsSequence + process.hltPFCentralJetLooseIDQuad30 + process.hlt2PFCentralJetLooseID60 + process.HLTBtagDeepCSVSequencePF + process.{}  + process.HLTEndSequence )\n".format(args.th, calo_name, pf_name))

    #Benchmark +  analyzer
    man.InsertPath("process.HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3 = cms.Path( process.HLTBeginSequence + process.hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet + process.hltPrePFHT330PT30QuadPFJet75604540TriplePFBTagDeepCSV4p5 + process.HLTAK4CaloJetsSequence + process.hltQuadCentralJet30 + process.hltCaloJetsQuad30ForHt + process.hltHtMhtCaloJetsQuadC30 + process.hltCaloQuadJet30HT320 + process.HLTBtagDeepCSVSequenceL3 + process.hltBTagCaloDeepCSVp17Double + process.HLTAK4PFJetsSequence + process.hltPFCentralJetLooseIDQuad30 + process.hlt1PFCentralJetLooseID75 + process.hlt2PFCentralJetLooseID60 + process.hlt3PFCentralJetLooseID45 + process.hlt4PFCentralJetLooseID40 + process.hltPFCentralJetLooseIDQuad30forHt + process.hltHtMhtPFCentralJetsLooseIDQuadC30 + process.hltPFCentralJetsLooseIDQuad30HT330 + process.HLTBtagDeepCSVSequencePF + process.hltBTagPFDeepCSV4p5Triple + process.HLTEndSequence )\n")


if args.run and args.data:
    print("@[Info]: Adding DATA files as inputs... ")
    man.AddDASQuery(file_list)
elif args.tr and args.data:
    print("@[Info]: Adding Timing DATA files as inputs... ")
    man.AddDASQuery(file_list)
else: man.AddDASQueryMC()

print("@[EndJob]: Done")

