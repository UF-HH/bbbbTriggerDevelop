# Custom Trigger Menu using ConfDB

Reproducing the Trigger HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9 for 2018 using ConfDB. Important step toward computing rate and timing performance 
ps: in the following the RAW file is totally random.

The following is a manual approach:

```
cmsrel CMSSW_10_Y_Z #for us 2018: CMSSW_10_2_18 
cd CMSSW_10_Y_Z/src
cmsenv

git cms-addpkg HLTrigger/Configuration
git cms-checkdeps -A -a
scram b -j 6

hltGetConfiguration /frozen/2018/2e34/v3.2/HLT/V1 --globaltag 101X_mc2017_realistic_TSG_2018_04_09_20_43_53 \
--input root:// #insert RAW file here
--mc --process HLTest --full --offline --l1-emulator uGT --type GRun \ #--mc: if MC and --data: if data, --process: cms.Process name --type GRun for pp collisions.
--path HLTriggerFirstPath,HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9,HLTriggerFinalPath,HLTAnalyzerEndpath \ #FirstPath and EndPath mandatory, in the middle whatever module you want (starts with HLT).
--l1 L1Menu_Collisions2018_v1_0_0-d1_xml --prescale none --max-events 10 --output none > hlt.py # --l1: .xml for L1 configuration,

cmsRun hlt.py
```

Already run all commands from ````./config```` you may need too install HLTrigger package (and scram it). Output is ````hlt.py````, ````hlt_menu_2018.py```` is the full menu of 2018 (/frozen/2018/2e34/v3.2/HLT/V1)  
