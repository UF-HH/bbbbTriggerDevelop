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

print(FindFirstPath(menu_path))

#Insert(menu_path, "\n#-------------My Modules Insert-------------\n", FindFirstPath(menu_path)-1)
#AddModule(menu_path, "ciao", "HLTHTCaloDouble", 13063)
#AddToPath(menu_path,"process.HLTriggerFinalPath",  "process.ciao", pos = "first")
#man = ModMan(menu_path)
"""
man.SetCurrentLine("before:process.hltDoubleCentralCaloHT180")
#man.Insert("#-----Added Modules-----\n")
man.CloneModule("process.hltBTagBisector23PF", in_class="Module")
str_ = man.ConvertModuleToString("Module")
print(str_)

"""
#man.ModifyPar("Module", "saveTags", False)
#man.InsertInMenu(in_class="Module",process_name = 'in_class')
#man.CreateFromLocal(in_class="hltProvaInsert",mod_name="HLTHTCaloQuad")
#str_ = man.ConvertModuleToString("hltProvaInsert")
#man.ModifyPar(in_class = "hltProvaInsert", attr_name="saveTags", attr_value=False)
#strs = man.ConvertModuleToString(in_class="HLTHTCaloQuad", process_name = "HLTHTCaloQuad")
#man.InsertInMenu(in_class="hltProvaInsert",process_name = 'in_class', line=13061)

