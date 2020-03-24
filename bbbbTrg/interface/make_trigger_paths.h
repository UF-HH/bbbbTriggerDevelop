#ifndef MAKE_TRIGGER_PATHS_H
#define MAKE_TRIGGER_PATHS_H

#include <vector>
#include <string>
#include <iostream>

class make_trigger_paths{
    private:
        std::vector<std::pair<std::string,int>> map_1;
        std::vector<std::pair<std::string, int>> map_2;
        std::vector<std::string> trig_names;
        std::vector<std::vector<int> > OR;
        std::vector<std::vector<std::pair<std::string,int>>> final_map;
        int year;
    public:
        make_trigger_paths(int y){
            
            year = y;
            if(y == 2016){
                trig_names = {"HLT_QuadJet45_TripleBTagCSV_p087_v6", "HLT_DoubleJet90_Double30_TripleBTagCSV_p087_v5"};

                //HLT_QuadJet45_TripleBTagCSV_p087_v6
                map_1.push_back(std::pair<std::string, int>("hltL1sQuadJetC50IorQuadJetC60IorHTT280IorHTT300IorHTT320IorTripleJet846848VBFIorTripleJet887256VBFIorTripleJet927664VBF", 1));
                map_1.push_back(std::pair<std::string, int>("hltL1sQuadJetCIorTripleJetVBFIorHTT", 1));
                map_1.push_back(std::pair<std::string, int>("hltQuadCentralJet45", 4));
                map_1.push_back(std::pair<std::string, int>("hltBTagCaloCSVp087Triple", 3));
                map_1.push_back(std::pair<std::string, int>("hltQuadPFCentralJetLooseID45", 4));
                
                //HLT_DoubleJet90_Double30_TripleBTagCSV_p087_v5
                map_2.push_back(std::pair<std::string, int>("hltL1sTripleJetVBFIorHTTIorDoubleJetCIorSingleJet", 1));
                map_2.push_back(std::pair<std::string, int>("hltQuadCentralJet30", 4));
                map_2.push_back(std::pair<std::string, int>("hltDoubleCentralJet90", 2));
                map_2.push_back(std::pair<std::string, int>("hltBTagCaloCSVp087Triple", 3));
                map_2.push_back(std::pair<std::string, int>("hltQuadPFCentralJetLooseID30", 4));
                map_2.push_back(std::pair<std::string, int>("hltDoublePFCentralJetLooseID90", 2));

                OR = {{0,1}, {}};

                final_map.push_back(map_1);
                final_map.push_back(map_2);

            }

            if(y == 2017){
                trig_names = {"HLT_PFHT300PT30_QuadPFJet_75_60_45_40_TriplePFBTagCSV"};

                //HLT PFHT300PT30 QuadPFJet 75 60 45 40 TriplePFBTagCSV
                map_1.push_back(std::pair<std::string, int>("hltL1sQuadJetC60IorHTT380IorHTT280QuadJetIorHTT300QuadJet", 1));
                map_1.push_back(std::pair<std::string, int>("hltQuadCentralJet30", 4));
                map_1.push_back(std::pair<std::string, int>("hltCaloQuadJet30HT300", 1));
                map_1.push_back(std::pair<std::string, int>("hltBTagCaloCSVp05Double", 2));
                map_1.push_back(std::pair<std::string, int>("hltPFCentralJetLooseIDQuad30", 4));
                map_1.push_back(std::pair<std::string, int>("hlt1PFCentralJetLooseID75", 1));
                map_1.push_back(std::pair<std::string, int>("hlt2PFCentralJetLooseID60", 2));
                map_1.push_back(std::pair<std::string, int>("hlt3PFCentralJetLooseID45", 3));
                map_1.push_back(std::pair<std::string, int>("hlt4PFCentralJetLooseID40", 4));
                map_1.push_back(std::pair<std::string, int>("hltPFCentralJetsLooseIDQuad30HT300", 1));
                map_1.push_back(std::pair<std::string, int>("hltBTagPFCSVp070Triple", 3));

                final_map.push_back(map_1);

                OR = {{}};

            }

            if(y == 2018){
                
                trig_names = {"HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v"};

                //HLT PFHT330PT30 QuadPFJet 75 60 45 40 TriplePFBTagDeepCSV
                map_1.push_back(std::pair<std::string, int>("hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet", 1));
                map_1.push_back(std::pair<std::string, int>("hltQuadCentralJet30", 4));
                map_1.push_back(std::pair<std::string, int>("hltCaloQuadJet30HT320", 1));
                map_1.push_back(std::pair<std::string, int>("hltBTagCaloDeepCSVp17Double", 2));
                map_1.push_back(std::pair<std::string, int>("hltPFCentralJetLooseIDQuad30", 4));
                map_1.push_back(std::pair<std::string, int>("hlt1PFCentralJetLooseID75", 1));
                map_1.push_back(std::pair<std::string, int>("hlt2PFCentralJetLooseID60", 2));
                map_1.push_back(std::pair<std::string, int>("hlt3PFCentralJetLooseID45", 3));
                map_1.push_back(std::pair<std::string, int>("hlt4PFCentralJetLooseID40", 4));
                map_1.push_back(std::pair<std::string, int>("hltPFCentralJetsLooseIDQuad30HT330", 1));
                map_1.push_back(std::pair<std::string, int>("hltBTagPFDeepCSVp24Triple", 3));
                map_1.push_back(std::pair<std::string, int>("hltBTagPFDeepCSV4p5Triple", 3));

                OR = {{10,11}};

                final_map.push_back(map_1);
            }
        };
        ~make_trigger_paths(){};
        std::vector<std::vector<std::pair<std::string,int>>> inline get_map(){return final_map;};
        std::vector<std::string> inline get_trig_name(){return trig_names; };
        std::vector<std::vector<int> > get_OR(){return OR;};

};

#endif
