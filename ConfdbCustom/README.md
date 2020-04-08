# Custom Trigger Menu using ConfDB

Reproducing the Triggers HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3 and HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9 for 2018 using ConfDB. Important step toward computing rate and timing performance 

Structure of the repo:
test: Containing json and HLT menus meant to be ran on RAW data.
python: Customize analyzer with path informations and Query to DAS to obtain RAW files of interest namely ggF and Ephemeral{1-8}.
plugins: Set of Analyzers to save Jets and trigger bits.
interface: ...
analysis: The software used to reproduce the triggers with a modular interface to vary cuts on each filter.

```
#data
export SCRAM_ARCH=slc7_amd64_gcc700 #bash

cmsrel CMSSW_10_Y_Z #for us 2018: CMSSW_10_2_18 
cd CMSSW_10_Y_Z/src
cmsenv
hash

git cms-addpkg HLTrigger/Configuration
git cms-addpkg HLTrigger/Timer
git cms-addpkg L1Trigger/L1TGlobal

hltConfigFromDB --cff --configName /frozen/2018/2e34/v3.2/HLT/V1 --nopaths --services \
-PrescaleService,-EvFDaqDirector,-FastMonitoringService > setup_cff.py

hltGetConfiguration /frozen/2018/2e34/v3.2/HLT/V1 --globaltag 101X_mc2017_realistic_TSG_2018_04_09_20_43_53 \
--input root:// #insert RAW file here
--mc --process HLTest --full --offline --l1-emulator uGT --type GRun \ #--mc: if MC and --data: if data, --process: cms.Process name --type GRun for pp collisions.
--path HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9,HLTriggerFinalPath,HLTAnalyzerEndpath \ #FirstPath and EndPath mandatory, in the middle whatever module you want (starts with HLT).
--l1 L1Menu_Collisions2018_v1_0_0-d1_xml --prescale none --timing  --max-events 10 --output none > hlt.py # --l1: .xml for L1 configuration,

#add Analyzers and schedule them,load the setup_cff just after the cms.Process definition
#Input filer are queried by QueryForFiles.py in ConfdbCustom (returning a list of file stirngs)

cmsRun hlt.py
```

# Usage of analysis Tools

Three classes: Obj.h, Event.h and TriggerMaker.h. Obj has definitions for the following objects: BQuarks, BQuarksCollections,Jets, JetsCollection, HLTCuts, KinCuts.\
Event reads the .root output from ```cmsRun hlt.py``` and reads the branches (std::vector) and builds the objects (Jets if data, Jets and bquarks if MC). It has inner modules for matching jets with jets and jets with partons.\
TriggerMaker builds the trigger, predefined modules can be found in FilterSelector.h, many can be added. Default configuratgion is 2018 trigger wo btag but one can add modules (ex: simplePrinter.cpp).
TriggerMaker objects can be modified. One can creare a json script with arbitrarely different thresholds (inherts from HLTCuts). 
Scripts can be made to iteratively change thresholds from given json (like adding lists of threshold to json ex: FromJson.py)
 
