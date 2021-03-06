python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --empty
echo "[Info] Running Basic menu"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4_Base.root 


python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 0p9
echo "[Info] Running 0.9"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4SumB0p9.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 1p0
echo "[Info] Running 1.0"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4SumB1p0.root

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 1p1
echo "[Info] Running 1.1"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4SumB1p1.root

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 1p2
echo "[Info] Running 1.2"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4SumB1p2.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 1p3
echo "[Info] Running 1.3"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4SumB1p3.root

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 1p4
echo "[Info] Running 1.4"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4SumB1p4.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 1p5
echo "[Info] Running 1.5"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4SumB1p5.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 1p6
echo "[Info] Running 1.6"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4SumB1p6.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 1p7
echo "[Info] Running 1.7"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4SumB1p7.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 1p8
echo "[Info] Running 1.8"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4SumB1p8.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 1p9
echo "[Info] Running 1.9"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4SumB1p9.root 
