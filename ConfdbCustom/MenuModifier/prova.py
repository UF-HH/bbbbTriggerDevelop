import os
import sys
import argparse
import shutil
from ModMenu import *
from ModuleManager import ModMan

menu_path = "../test/hlt_wMyModule_copy.py"
#Insert(menu_path, "\n#-------------My Modules Insert-------------\n", 13061)
#AddModule(menu_path, "ciao", "HLTHTCaloDouble", 13063)

man = ModMan(menu_path)
"""
man.SetCurrentLine("before:process.hltDoubleCentralCaloHT180")
#man.Insert("#-----Added Modules-----\n")
man.CloneModule("process.hltBTagBisector23PF", in_class="Module")
str_ = man.ConvertModuleToString("Module")
print(str_)

"""
#man.ModifyPar("Module", "saveTags", False)
#man.InsertInMenu(in_class="Module",process_name = 'in_class')
man.CreateFromLocal(in_class="hltProvaInsert",mod_name="HLTHTCaloQuad")
str_ = man.ConvertModuleToString("hltProvaInsert")
print(str_)
#man.ModifyPar(in_class = "hltProvaInsert", attr_name="saveTags", attr_value=False)
#strs = man.ConvertModuleToString(in_class="HLTHTCaloQuad", process_name = "HLTHTCaloQuad")
#man.InsertInMenu(in_class="hltProvaInsert",process_name = 'in_class', line=13061)

