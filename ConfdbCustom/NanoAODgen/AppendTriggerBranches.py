"""
This scripts append to some nanoAOD files the result of the relative triggers from a .root file obtained from the HLT
(usually MC ggF).

python AppendTriggerBranches.py --nanoaod nanoaod --trigresults SumBTagTriggersReco.root --triggers HLT_Quad30_Double60_Sum2LeadingBTag_1p3,HLT_PFHT270_180_Double180_Double90_BisectorBTag07,HLT_Quad30_Double60_Sum2LeadingBTag_OnlyKin,HLT_Quad30_Double60_Sum2LeadingBTag0p9,HLT_Quad30_Double60_Sum2LeadingBTag1p0,HLT_Quad30_Double60_Sum2LeadingBTag1p1,HLT_Quad30_Double60_Sum2LeadingBTag1p2,HLT_Quad30_Double60_Sum2LeadingBTag1p3,HLT_Quad30_Double60_Sum2LeadingBTag1p4,HLT_Quad30_Double60_Sum2LeadingBTag1p5,HLT_Quad30_Double60_Sum2LeadingBTag1p6,HLT_Quad30_Double60_Sum2LeadingBTag1p7,HLT_Quad30_Double60_Sum2LeadingBTag1p8,HLT_Quad30_Double60_Sum2LeadingBTag1p9,HLT_Quad30_Double60_Sum2LeadingBTag2p0,HLT_Quad30_Double60_Sum2LeadingBTag2p1

"""

import argparse
import ROOT
import collections
import fnmatch
import ROOT
import ast
from glob import glob
import os
import sys
from array import array
import numpy as np

ROOT.PyConfig.IgnoreCommandLineOptions = True

def mkdir(path):
	try:
		os.mkdir(path)
	except OSError:
		print ("Creation of the directory %s failed" % path)
	else:
		print ("Successfully created the directory %s " % path)


def checkTriggers(file_, trigs):

    print("[INFO] Checking if all triggers are accessible for each event of trig result root file...")
    f = ROOT.TFile(file_)
    t = f.Get("MyHLTAnalyzer/trgObjTree")
    pass_ = True
    for event in t:
        for tr in trigs:
            try:
                a = getattr(event, tr)
            except:
                print("[INFO] some events do not have trigger: {}".format(tr))
                pass_ = False

    f.Close()
    return pass_

def checkLen(nanoaod, trig_file):

    pass_ = True
    f_trig = ROOT.TFile(trig_file)
    t = f_trig.Get("MyHLTAnalyzer/trgObjTree")

    ch = ROOT.TChain("Events")
    for nano in nanoaods:
        ch.Add(nano)

    if t.GetEntries() < ch.GetEntries():
        pass_ = False

    return pass_



parser = argparse.ArgumentParser(description='Command line parser of plotting options')
#string opts
parser.add_argument('--nanoaod', dest='nanoaod', help='NanoAOD folder with files to be appended trigger results', required = True, default=None)
parser.add_argument('--trigresults', dest='trigresults', help='.root file with a MyHLTAnalyzer/trgObjTree tree containing triggers bit', default=None)
parser.add_argument('--triggers', dest='triggers', help='comma separated list of trigger branches for retrieving bits. Same name of appended branch on nanoaod', default=None)
parser.add_argument('--outfolder', dest='outfolder', help='Name of the folder to save new .roots', default="nanoAOD_Trig")

args = parser.parse_args()

triggers = args.triggers.split(",")
nanoaods = glob(args.nanoaod + "/*.root")
if args.triggers == 'all':
   f = ROOT.TFile(nanoaods[0])
   t = f.Get("MyHLTAnalyzer/trgObjTree")
   triggers = [i.GetName() for i in t.GetListOfBranches() if "HLT" in i]
#-------    Perform sanity checks    --------

if len(nanoaods) == 0: sys.exit("[ERROR] No NANOAOD files in folder {} ...".format(args.nanoaod))
if not checkTriggers(args.trigresults, triggers): sys.exit("[ERROR] Trigger errors, check input arguments...")
if not checkLen(nanoaods, args.trigresults): sys.exit("[ERROR] Trigger results length is less than nanoaod, aborting...")

print("[INFO] Basic checks succeed... Proceeding with appending trigger results...")

mkdir(args.outfolder)

for nano in nanoaods:
    name = nano.split("/")[-1]
    new_file_name = args.outfolder + "/" + name

    if not os.path.isfile(new_file_name): #if file does not exist then create it

        print("[INFO] Creating new file {} from {}...".format(new_file_name, nano))
        #reading eos skims thagt contains also trigger weights
        f = ROOT.TFile(nano)
        t = f.Get("Events")

        #cycle on branches required and create new branches on the new file

        f_w = ROOT.TFile(args.trigresults)
        t_w = f_w.Get("MyHLTAnalyzer/trgObjTree")
        
        #opening a new file in the output folder
        new_f = ROOT.TFile(new_file_name, "RECREATE")
        newtree = t.CloneTree(0)
        newtree.CopyEntries(t)

        """
        #this way we decouple pointers to fill separately each branch
        weight_ = []
        for wid in range(len(triggers)):
            a = array('i', [0])
            weight_.append(a)

        branches = []
        for id_,trig_name in enumerate(triggers):
            branches.append(newtree.Branch( trig_name , weight_[id_], trig_name + "/O"))

        """
        
        
        weight_ = []
        for wid in range(len(triggers)):
            a = np.array([0], dtype=bool)
            weight_.append(a)

        branches = []
        for id_,trig_name in enumerate(triggers):
            branches.append(newtree.Branch( trig_name , weight_[id_], trig_name + "/O"))

        
        j = 0
        for event in newtree:
            if j%1000 == 0: print("Processing event: {}".format(j))
            ev_id = event.event 
            check = False
            for event2 in t_w:
                if ev_id == event2.event:
                    check = True
                    for tr_id, tr in enumerate(triggers):
                        var_ = getattr(event2, tr)
                        weight_[tr_id][0] = bool(var_)

                    for id_br, br in enumerate(branches):
                        branches[id_br].Fill()

            if check == False:
                print("Event {} does not match ant event in the trigger file ...".format(ev_id))
            j+=1

        f_w.Close()
        print("[INFO] Saving output in {}".format(new_file_name))
        newtree.Write()
        new_f.Write()
        new_f.Close()
        f.Close()

    else:
        print("[INFO] File {} already exists, delete it before continue ...".format(new_file_name))
        continue
