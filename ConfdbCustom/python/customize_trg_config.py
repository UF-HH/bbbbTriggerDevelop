import FWCore.ParameterSet.Config as cms

def customize_trg_config_2016(process):
    process.MyHLTAnalyzer.triggerList = cms.vstring(
        #'HLT_QuadJet45_TripleBTagCSV_v',
        #'HLT_DoubleJet90_Double30_TripleBTagCSV_v',
        'HLT_QuadJet45_TripleBTagCSV_p087_v6',
        'HLT_DoubleJet90_Double30_TripleBTagCSV_p087_v5'
    )

    process.MyHLTAnalyzer.filterList = cms.vstring(
        #### HLT_QuadJet45_TripleBTagCSV_v
        ## the two below in OR
        'hltL1sQuadJetC50IorQuadJetC60IorHTT280IorHTT300IorHTT320IorTripleJet846848VBFIorTripleJet887256VBFIorTripleJet927664VBF',
        'hltL1sQuadJetCIorTripleJetVBFIorHTT',  # 1
        #
        'hltQuadCentralJet45',          # 4
        'hltQuadPFCentralJetLooseID45', # 4
        'hltBTagCaloCSVp087Triple',     # 3
        ###
        ### HLT_DoubleJet90_Double30_TripleBTagCSV_v
        'hltL1sTripleJetVBFIorHTTIorDoubleJetCIorSingleJet', #  1
        'hltDoubleCentralJet90',                             #  2
        'hltQuadCentralJet30',                               #  4
        'hltDoublePFCentralJetLooseID90',                    #  2
        'hltQuadPFCentralJetLooseID30',                      #  4
        'hltBTagCaloCSVp087Triple',                          #  2           
    )

def customize_trg_config_2017(process):
    process.MyHLTAnalyzer.triggerList = cms.vstring(
        'HLT_HT300PT30_QuadJet_75_60_45_40_TripeCSV_p07_v',
        'HLT_PFHT300PT30_QuadPFJet_75_60_45_40_TriplePFBTagCSV_3p0_v',
    )

    process.MyHLTAnalyzer.filterList = cms.vstring(
        'hltL1sQuadJetC60IorHTT380IorHTT280QuadJetIorHTT300QuadJet',  #  1
        'hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet',    #  1, replaces the above sometimes
        'hltQuadCentralJet30',                                        #  4
        'hltCaloJetsQuad30ForHt',                                     #  not saved
        'hltHtMhtCaloJetsQuadC30',                                    #  not saved
        'hltCaloQuadJet30HT300',                                      #  1
        'hltBTagCaloCSVp05Double',                                    #  2
        'hltPFCentralJetLooseIDQuad30',                               #  4
        'hlt1PFCentralJetLooseID75',                                  #  1
        'hlt2PFCentralJetLooseID60',                                  #  2
        'hlt3PFCentralJetLooseID45',                                  #  3
        'hlt4PFCentralJetLooseID40',                                  #  4
        'hltPFCentralJetLooseIDQuad30forHt',                          #  not saved
        'hltHtMhtPFCentralJetsLooseIDQuadC30',                        #  not saved
        'hltPFCentralJetsLooseIDQuad30HT300',                         #   1
        'hltBTagPFCSVp070Triple',                                     #  3
    )


def customize_trg_config_2018(process):
    print "... trigger configuration customised for the year 2018"

    process.MyHLTAnalyzer.triggerList = cms.vstring(
        'HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9', #nobtag
        'HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v3', #btag
        'HLT_PFHT270_180_Double180_Double90_BisectorBTag07'
        #'HLT_PFHT250_v17' #for data, do we have these objects in nanoAOD?
    )
 

    process.MyHLTAnalyzer.filterList = cms.vstring(

        'hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet',  #  1
        'hltPrePFHT330PT30QuadPFJet75604540',
        'HLTAK4CaloJetsSequence',
        'hltQuadCentralJet30',                  #  4
        'hltCaloJetsQuad30ForHt',               #  not saved
        'hltHtMhtCaloJetsQuadC30',              #  not saved
        'hltCaloQuadJet30HT320',                #  1
        'HLTAK4PFJetsSequence',
        'hltPFCentralJetLooseIDQuad30',         #  4
        'hlt1PFCentralJetLooseID75',            #  1
        'hlt2PFCentralJetLooseID60',            #  2
        'hlt3PFCentralJetLooseID45',            #  3
        'hlt4PFCentralJetLooseID40',            #  4
        'hltPFCentralJetLooseIDQuad30forHt',    #  not saved
        'hltHtMhtPFCentralJetsLooseIDQuadC30',  #  not saved
        'hltPFCentralJetsLooseIDQuad30HT330',   #   1

    )
