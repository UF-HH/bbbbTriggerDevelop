for module in \
../TriggerCnfg/hltQuadCentralJet30.txt \
../TriggerCnfg/hltCaloQuadJet30HT320.txt  \
../TriggerCnfg/hltBTagCaloDeepCSVp17Double.txt \
../TriggerCnfg/hltPFCentralJetLooseIDQuad30.txt \
../TriggerCnfg/hlt1PFCentralJetLooseID75.txt \
../TriggerCnfg/hlt2PFCentralJetLooseID60.txt \
../TriggerCnfg/hlt3PFCentralJetLooseID45.txt \
../TriggerCnfg/hlt4PFCentralJetLooseID40.txt \
../TriggerCnfg/hltPFCentralJetsLooseIDQuad30HT330.txt \
../TriggerCnfg/hltBTagPFDeepCSV4p5Triple.txt ; do \

python3 RateFromJson.py -f $module -o TriggerConfig.txt -cpp Rate.cpp

done