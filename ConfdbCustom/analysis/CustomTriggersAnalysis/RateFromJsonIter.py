import numpy as np
import itertools
import json
import sys
import os
import subprocess
from tqdm import tqdm
import argparse
from operator import itemgetter
import pandas as pd


parser = argparse.ArgumentParser()
parser.add_argument('-f', '--jsonfile', type=str, required=True, help="File")
parser.add_argument('-o', '--output', type=str, required=True, help="Output file")
parser.add_argument('-cpp', '--cppscript', type=str, required=False, help="Script")
parser.add_argument('-n', '--nevents', type=int, required=False, help="Number of events to process")
parser.add_argument('-csv', '--csv', default=False,  action='store_true', required=False, help="Conversion of configuration and rates into csv")
parser.add_argument('-keep', '--keep', default=False, action='store_true', required=False, help="Keep The txt after the conversion to csv?")


args = parser.parse_args()

with open(args.jsonfile) as json_file:
    data= json.load(json_file)

print("[INFO:] Creating Map...")
builder = open(args.output, "w+")

for i in data: #trigger names
    list_cuts = []
    list_names = []
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
    for trigger_config in tqdm(parsing):
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

if(args.csv):
    try:
        cfile = open("RateBkg.txt", "r")
        nlines = len(cfile.readlines())
        cfile.close()

        with open(args.output, "r") as cfgfile:
            num = len(cfgfile.readlines())
        cfgfile.close()

        block = int(num/nlines)

        #columns
        cfgfile_ = open(args.output, "r")
        hypercol = []
        col = cfgfile_.readlines()[:block]

        innercol = []
        outercol = []
        for c in col:
            if "Trigger" in c:
                continue
            c = c.split(",")
            for i in c:
                if "Filter" in i:
                    hypcol = i.split(":")[1]
                
                else:
                    if(i.split(":")[0] == "\n"):
                        continue
                    innercol.append(i.split(":")[0])
                    outercol.append(hypcol)
        arrays = [np.array(outercol), np.array(innercol)]
        cfgfile_.close()

        #Reading config values 
        cfgfile_ = open(args.output, "r")
        values = []
        lines = cfgfile_.readlines()
        for i in range(nlines):
            vals = []
            start = i*block
            end = (i+1)*block
            for c in lines[start:end]:
                if "Trigger" in c:
                    continue
                c = c.split(",")
                for i in c:
                    if "Filter" in i or i.split(":")[0] == "\n":
                        continue
                    vals.append(i.split(":")[1])
            values.append(vals)

        #creating nested dataframe   
        df = pd.DataFrame(np.array(values), columns = arrays)    
                

        totsig = []
        countsig = []
        totbkg = []
        countbkg = []
        with open("RateSig.txt", "r") as cfile_:
            line = cfile_.readline()
            while(line):
                line = line.split(",")
                for sec in line:
                    if "Tot" in sec:
                        totsig.append(sec.split(":")[1])

                    if "Accepted" in sec:
                        countsig.append(sec.split(":")[1])
                line = cfile_.readline()
                

        with open("RateBkg.txt", "r") as cfile_:
            line = cfile_.readline()
            while(line):
                line = line.split(",")
                for sec in line:
                    if "Tot" in sec:
                        totbkg.append(sec.split(":")[1])

                    if "Accepted" in sec:
                        countbkg.append(sec.split(":")[1])
                line = cfile_.readline()
        df['totsig', 'count'] = totsig
        df['countsig', 'count'] = countsig
        df['totbkg', 'count'] = totbkg
        df['countbkg', 'count'] = countbkg

        if not args.keep:
            os.system("rm RateBkg.txt RateSig.txt {}".format(args.output))
        df.to_csv("Results.csv")
        
    except IOError as e:
        print("I/O error({0}): {1}").format(e.errno, e.strerror)
    


    


    

