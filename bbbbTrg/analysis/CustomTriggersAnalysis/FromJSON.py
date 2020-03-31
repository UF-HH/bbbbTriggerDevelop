# THIS ONLY WORKS IN PYTHON 3, JSON IN PYTHON2 ARE NON-ORDERED MAPS
#python3 FromJSON.py -f ../TriggerCnfg/ThVar2018NoBtag.txt -o TriggerConfig.txt -cpp ComputeEfficiencyFromJSON.cpp -n 1000

import json
import sys
import os
import subprocess
from tqdm import tqdm
import argparse


parser = argparse.ArgumentParser()
parser.add_argument('-f', '--jsonfile', type=str, required=True, help="File") 
parser.add_argument('-o', '--output', type=str, required=True, help="Output file")
parser.add_argument('-cpp', '--cppscript', type=str, required=False, help="Script") 
parser.add_argument('-n', '--nevents', type=int, required=False, help="Number of events to process") 


args = parser.parse_args()

print("[INFO] Creating map")

with open(args.jsonfile) as json_file:
    data= json.load(json_file)

builder = open(args.output, "w+")
for i in data: #trigger names
    list_cuts = []
    list_names = []
    for j in data[i]: #filter names
        cuts = []
        names = []
        cuts.append(j)
        names.append("Filter")
        for el in data[i][j]: #cut names
            cuts.append(data[i][j][el])#else they are string such as names
            names.append(el)
        list_cuts.append(cuts)
        list_names.append(names)
        
    unPacking_lists = []
    for index,line in enumerate(list_cuts):
        if any(isinstance(x, list) for x in line):
            for line_index in range(len(line)):
                if isinstance(line[line_index], list):
                    if(len(unPacking_lists) != 0):
                        support_list = []
                        to_remove_list = []
                        for j in line[line_index]:
                            l_copy = list(line)
                            l_copy[line_index] = j
                            for l in unPacking_lists:
                                to_remove_list.append(l)
                                list_cuts_copy = list(l)
                                list_cuts_copy.insert(index,l_copy)
                                try:
                                    list_cuts_copy.remove(line)
                                except:
                                    continue
                                support_list.append(list_cuts_copy)
                                
                        for to_app in support_list:
                            unPacking_lists.append(to_app)
                        for to_rem in to_remove_list:
                            try:
                                unPacking_lists.remove(to_rem)
                            except:
                                pass
                                
                    else:        
                        for j in line[line_index]:
                            l_copy = list(line)
                            l_copy[line_index] = j
                            list_cuts_copy = list(list_cuts)
                            list_cuts_copy.insert(index,l_copy)
                            list_cuts_copy.remove(line)
                            unPacking_lists.append(list_cuts_copy)
            
            
    if len(unPacking_lists) == 0:
        unPacking_lists.append(list_cuts)
    
    for trigger_config in unPacking_lists:
        builder.write("Trigger Name: {} \n".format(i))
        for line, names in zip(trigger_config, list_names):
            for value, name in zip(line, names):
                builder.write("{}:{}".format(name,value))
                builder.write(",")
            builder.write("\n")
        
builder.close()

if(args.cppscript):
    print("[INFO] Compiling")
    os.system("c++ -o exec {} ../src/Event.cc ../src/TriggerMaker.cc `root-config --cflags --glibs`".format(args.cppscript))
    print("[INFO] Calling the executable")
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

