python PersonalConfig_Timing.py --menu=/dev/CMSSW_10_1_0/GRun/V7 -gt=101X_dataRun2_HLT_v7 -out=myHLT.py -pr=myHLT -out=myHLT.py -input=root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/Timing/SKIMs/skim_Ephemeral_319941/EphemeralHLTPhysics_PU48-49_319941000.root,root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/Timing/SKIMs/skim_Ephemeral_319941/EphemeralHLTPhysics_PU48-49_319941001.root,root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/Timing/SKIMs/skim_Ephemeral_319941/EphemeralHLTPhysics_PU48-49_319941002.root,root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/Timing/SKIMs/skim_Ephemeral_319941/EphemeralHLTPhysics_PU48-49_319941003.root,root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/Timing/SKIMs/skim_Ephemeral_319941/EphemeralHLTPhysics_PU48-49_319941004.root,root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/Timing/SKIMs/skim_Ephemeral_319941/EphemeralHLTPhysics_PU48-49_319941005.root,root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/Timing/SKIMs/skim_Ephemeral_319941/EphemeralHLTPhysics_PU48-49_319941006.root,root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/Timing/SKIMs/skim_Ephemeral_319941/EphemeralHLTPhysics_PU48-49_319941007.root,root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/Timing/SKIMs/skim_Ephemeral_319941/EphemeralHLTPhysics_PU48-49_319941008.root,root://eoscms.cern.ch//eos/cms/store/group/dpg_trigger/comm_trigger/TriggerStudiesGroup/Timing/SKIMs/skim_Ephemeral_319941/EphemeralHLTPhysics_PU48-49_319941009.root -nev=50000 -data --l1 L1Menu_Collisions2018_v2_1_0-d1_xml --prescale "2.0e34+ZB+HLTPhysics"

nohup taskset -c 0-3 cmsRun myHLT.py >& full.log&
