"""

Add lines in menu to query for primary/secondary dataset RAW/AOD. Only works in current directory
Due to sys.path.insert. Example:

python AddPrimarySecondaryDatasets.py -d_P=/GluGluToHHTo4B_node_SM_TuneCP5_14TeV-madgraph-pythia8/Run3Winter20DRPremixMiniAOD-110X_mcRun3_2021_realistic_v6-v2/GEN-SIM-RAW \
-d_S=/GluGluToHHTo4B_node_SM_TuneCP5_14TeV-madgraph-pythia8/Run3Winter20DRPremixMiniAOD-110X_mcRun3_2021_realistic_v6-v2/MINIAODSIM -menu=../MC/hlt_gg_MyTrigger.py -keep \
 -out=hlt_gg_myTrigger_PS.py -line "last"

"""

import os
import sys
import argparse
import shutil
from ModMenu import *
from QueryForFiles import *

parser = argparse.ArgumentParser()
parser.add_argument('-d_P', '--primaryDataset', type=str, required=True, help="Add primary dataset to query")
parser.add_argument('-d_S', '--secondaryDataset', type=str, required=True, help="Add secondary dataset to query")
parser.add_argument('-menu', '--menu', type=str, required=True, help="Path of the menu to modified")
parser.add_argument('-keep', '--keep', default=False, action='store_true', required=False, help="Keep The menu and create copy")
parser.add_argument('-out', '--out', type=str, required=False, help="output for the new menu if keep is true")
parser.add_argument('-line', '--line', required=False, help="Line of the menu to insert the code")

args = parser.parse_args()

menu_path = args.menu

#check consistency of arguments
if args.keep and not args.out:
    sys.exit("Need out path to save the new menu. Otherwise specify --keep False to override menu")

#Copy the original menu if needed
if args.keep:
    shutil.copy(args.menu, args.out)
    menu_path = args.out

#querying for raw and aod from parsed inputs
raw_data, aod_data = QueryForRAWandAOD(args.primaryDataset, args.secondaryDataset)

#if no line passed the we find line for previous poolsource
if not args.line:
    print("...Finding process.source line")
    line, mod_def = FindModule(menu_path, "PoolSource")
    line = line -len(mod_def) #after removing module
    print("...Found in line {} ".format(line))

    # we search for the PoolSource, present by default when you dump a menu (empty if not --inputs) annd delete it
    RemoveModule(menu_path, "PoolSource")
    
else:
    RemoveModule(menu_path, "PoolSource")
    
    if args.line == "first":
        line = 0
    elif args.line == "last":
        line = FindLastLine(menu_path)
    elif isinstance(args.line, int):
        line = args.line



#obscuring inputs if present:
line_inputs = FindLine(menu_path, "_customInfo['inputFile' ]")
if line_inputs != None:
    to_add = "_customInfo['inputFile' ]= ['@']\n" #this line has to be present otherwise HLTrigger.Configuration.customizeHLTforALL will crash
    ReplaceLine(menu_path, line_inputs, to_add)


#finally adding the source 
AddPrimarySecondaryPoolSource(menu_path, raw_data, aod_data, line)









