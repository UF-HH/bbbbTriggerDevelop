import numpy as np
import itertools
import json
import sys
import os
import subprocess
from tqdm import tqdm
import argparse
import matplotlib
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument('-f', '--jsonfile', type=str, required=True, help="File")
parser.add_argument('-o', '--output', type=str, required=True, help="Output file")
parser.add_argument('-cpp', '--cppscript', type=str, required=False, help="Script")
parser.add_argument('-n', '--nevents', type=int, required=False, help="Number of events to process")


args = parser.parse_args()

with open(args.jsonfile) as json_file:
    data= json.load(json_file)

print("[INFO:] Creating Map...")
builder = open(args.output, "w+")
for i in data: #trigger names
    list_cuts = []
    list_names = []
    list_literals = []
    for j in data[i]: #filter names
        cuts = []
        names = []
        cuts.append([j])
        names.append("Filter")
        for el in data[i][j]: #cut names
            if(type(data[i][j][el]) == list):
                cuts.append([float(i) for i in data[i][j][el]])#else they are string such as names
            else:
                try:
                    cuts.append([float(data[i][j][el])])
                except:
                    cuts.append([data[i][j][el]])#else they are string such as names
            names.append(el)
        list_cuts.append(cuts)
        list_names.append(names)
        
    all_combo = []
    for c in range(len(list_cuts)):
        all_combo.append(list(itertools.product(*list_cuts[c])))
    parsing = list(itertools.product(*all_combo))
    
    for trigger_config in parsing:
        builder.write("Trigger Name: {} \n".format(i))
        for line, names in zip(trigger_config, list_names):
            for value, name in zip(line, names):
                builder.write("{}:{}".format(name,value))
                builder.write(",")
            builder.write("\n")
        
builder.close()

if(args.cppscript):
    print("...[INFO] Compiling...")
    os.system("c++ -o exec {} ../src/Event.cc ../src/TriggerMaker.cc `root-config --cflags --glibs`".format(args.cppscript))
    print("...[INFO] Calling the executable...")
    for trigger_config in tqdm(unPacking_lists):
        with open('Build_.txt', 'w') as file_:
            for name, mod in zip(list_names, trigger_config):
                for n, v in zip(name, mod):
                    file_.write("{}:{}".format(n,v))
                    file_.write(",")
                file_.write("\n")
        
        file_.close()
        if(args.nevents):
            process = subprocess.Popen("./exec Build_.txt {}".format(args.nevents), shell=True, stdout=subprocess.PIPE)
            process.wait()
            continue
        else:
            process = subprocess.Popen("./exec Build_.txt", shell=True, stdout=subprocess.PIPE)
            process.wait()
            continue

    os.system("rm Build_.txt")
    os.system("rm exec")

