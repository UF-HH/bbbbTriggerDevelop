import os
import sys
import argparse
import shutil
from ModMenu import *
from python.QueryForFiles import *

parser = argparse.ArgumentParser()
parser.add_argument('-d_P', '--primaryDataset', type=str, required=True, help="Add primary dataset to query")
parser.add_argument('-d_S', '--secondaryDataset', type=str, required=True, help="Add secondary dataset to query")
parser.add_argument('-menu', '--menu', type=str, required=True, help="Path of the menu to modified")
parser.add_argument('-keep', '--keep', default=False, action='store_true', required=False, help="Keep The menu and create copy")
parser.add_argument('-out', '--out', type=str, required=False, help="output for the new menu if keep is true")
parser.add_argument('-line', '--line', type=int, required=False, help="Line of the menu to insert the code")

args = parser.parse_args()

menu_path = args.menu

#check consistency of arguments
if args.keep and not args.out:
    sys.exit("Need out path to save the new menu. Otherwise specify --keep False to override menu")

#Copy the original menu if needed
if args.keep:
    shutil.copy(args.menu, args.out)
    menu_path = args.out

raw_data, aod_data = QueryForRAWandAOD(args.primaryDataset, args.secondaryDataset)

if not args.line:
    print("...Finding process.source line")
    line = FindLine(menu_path, "cms.Source")
    print("...Found in line {} ".format(line))
else:
    line = args.line

AddPrimarySecondaryPoolSource(menu_path, raw_data, aod_data, line)









