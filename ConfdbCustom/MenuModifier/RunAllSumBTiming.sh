python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --empty
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4_Base.root 


python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --th 0p9
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4SumB0p9.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --th 1p0
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4SumB1p0.root

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --th 1p1
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4SumB1p1.root

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --th 1p2
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4SumB1p2.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --th 1p3
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4SumB1p3.root

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --th 1p4
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4SumB1p4.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --th 1p5
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4SumB1p5.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --th 1p6
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4SumB1p6.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --th 1p7
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4SumB1p7.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --th 1p8
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4SumB1p8.root 

python SumBTagConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 319941 --th 1p9
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4SumB1p9.root 
