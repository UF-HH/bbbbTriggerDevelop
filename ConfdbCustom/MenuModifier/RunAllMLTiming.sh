python CNN1DConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=myHLT --data --tr 319941 --th 0p87
echo "[Info] Running th 0.87 menu"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4CNN1D_5_4_wp0p87.root 


python CNN1DConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=myHLT --data --tr 319941 --th 0p97
echo "[Info] Running th 0.97 menu"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000319941__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU48-49BX25_Run319941_CMSSW_10_1_11_Menu-HLTv36V4CNN1D_5_4_wp0p97.root 

#--------------------- HIGH PU -----------------------

python CNN1DConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 0p79
echo "[Info] Running th 0.87 menu"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4CNN1D_5_4_wp0p87.root 


python CNN1DConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 0p87
echo "[Info] Running th 0.87 menu"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4CNN1D_5_4_wp0p87.root 


python CNN1DConfigGenerator_Timing.py --menu=/online/collisions/2018/2e34/v3.6/HLT -gt=101X_dataRun2_HLT_v7 -out=myHLT_TimingData.py -pr=TIMING --data --tr 315188 --th 0p97
echo "[Info] Running th 0.97 menu"
nohup taskset -c 0-3 cmsRun myHLT_TimingData.py >& full.log&
wait
cmsRun harvestTiming_cff.py
wait
mv DQM_V0001_R000315188__HLT__FastTimerService__All.root  DQM_skimEphemeral_PU55-58BX25_Run315188_CMSSW_10_1_11_Menu-HLTv36V4CNN1D_5_4_wp0p97.root 