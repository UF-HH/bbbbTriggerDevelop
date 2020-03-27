# Custom Trigger Menu using ConfDB

Reproducing the Trigger HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9 for 2018 using ConfDB. Important step toward computing rate and timing performance 
ps: in the following the RAW file is totally random.

The following is a manual approach:

```
export SCRAM_ARCH=slc7_amd64_gcc700 #bash

cmsrel CMSSW_10_Y_Z #for us 2018: CMSSW_10_2_18 
cd CMSSW_10_Y_Z/src
cmsenv
hash

git cms-addpkg HLTrigger/Configuration
git cms-addpkg HLTrigger/HLTanalyzers
git cms-addpkg GeneratorInterface/GenFilters 
git cms-addpkg L1Trigger/L1TGlobal

#L1 Prescales
mkdir -p L1Trigger/L1TGlobal/data/Luminosity/startup

cp /afs/cern.ch/user/n/ndaci/public/STEAM/L1Menus/2018/L1Menu_Collisions2018_v1_0_0.xml L1Trigger/L1TGlobal/data/Luminosity/startup
cp /afs/cern.ch/user/n/ndaci/public/STEAM/Prescales/2018/L1PS_2018_06_13_Run317696.xml L1Trigger/L1TGlobal/data/Luminosity/startup
cp /afs/cern.ch/user/n/ndaci/public/STEAM/Prescales/2018/mask-trivial.xml L1Trigger/L1TGlobal/data/Luminosity/startup
git cms-checkdeps -A -a
scram b -j 6

hltConfigFromDB --cff --configName /frozen/2018/2e34/v3.2/HLT/V1 --nopaths --services \
-PrescaleService,-EvFDaqDirector,-FastMonitoringService > setup_cff.py

hltGetConfiguration /frozen/2018/2e34/v3.2/HLT/V1 --globaltag 101X_mc2017_realistic_TSG_2018_04_09_20_43_53 \
--input root:// #insert RAW file here
--mc --process HLTest --full --offline --l1-emulator uGT --type GRun \ #--mc: if MC and --data: if data, --process: cms.Process name --type GRun for pp collisions.
--path HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9,HLTriggerFinalPath,HLTAnalyzerEndpath \ #FirstPath and EndPath mandatory, in the middle whatever module you want (starts with HLT).
--l1 L1Menu_Collisions2018_v1_0_0-d1_xml --prescale none --max-events 10 --output none > hlt.py # --l1: .xml for L1 configuration,

cmsRun hlt.py
```

Some lines have been added to ```hlt.py``` like the ntuplizer or the PURemoval  

# USAGE

````
git clone https://github.com/UF-HH/bbbbTriggerDevelop.git
cd ConfdbCustom
./setup #this generates setup_cff.py
./config #this generates hlt.py
#insert the following line in hlt.py just after process = cms.Process(...) : process.load("setup_cff")
cmsRun hlt.py
    
```
