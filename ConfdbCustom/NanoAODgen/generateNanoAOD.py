import os 
import sys
import glob
import subprocess
import shlex

parse = 'dasgoclient --query="file dataset=/GluGluToHHTo4B_node_SM_TuneCP5_14TeV-madgraph-pythia8/Run3Winter20DRPremixMiniAOD-110X_mcRun3_2021_realistic_v6-v2/MINIAODSIM"'
files = subprocess.Popen(shlex.split(parse), stdout=subprocess.PIPE)
file_list = files.communicate()[0].split("\n") #dividing single tuple by \n
file_list = file_list[:len(file_list)-1] #last element is ''

print(len(file_list))

for file in file_list:
   name = file.split('/')[-1]
   print(name)
   parse = "cmsDriver.py myNANOAOD --filein {} --fileout nanoaod/{} --step NANO --eventcontent NANOAODSIM --datatier NANOAODSIM --conditions 110X_mcRun3_2021_realistic_v6 --era Run2_2018 --customise_commands='process.add_(cms.Service('InitRootHandlers', EnableIMT = cms.untracked.bool(False)))' --no_exec --mc --fast --number -1".format(file, name) 
   os.system(parse)
   os.system("cmsRun NANOAOD_NANO.py")
   
