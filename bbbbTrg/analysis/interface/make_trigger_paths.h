#ifndef MAKE_TRIGGER_PATHS_H
#define MAKE_TRIGGER_PATHS_H

#include <vector>
#include <string>
#include <iostream>
#include <numeric>

class make_trigger_paths{
    private:
        std::vector<std::pair<std::string,int>> map_1;
        std::vector<std::pair<std::string, int>> map_2;
        std::vector<std::string> trig_names;
        std::vector<std::vector<std::string>> var_names;
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

                var_names.push_back({"j_HT", "pt_4", "DeepFlavB", "pt_4"});
                var_names.push_back({"j_HT", "pt_4", "pt_2", "DeepFlavB", "pt_4", "pt_2"});

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

                var_names.push_back({""});

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

                var_names.push_back({"j_HT", "pt_4", "j_HT", "DeepFlavB", "pt_4", "pt_1", "pt_2", "pt_3", "pt_4", "j_HT", "DeepFlavB"});
            }
        }

        make_trigger_paths(int y, bool nob = true){

            if(y == 2018){
                trig_names = {"HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9"};


                //HLT PFHT330PT30 QuadPFJet 75 60 45 40 No B
                map_1.push_back(std::pair<std::string, int>("hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet", 1));
                map_1.push_back(std::pair<std::string, int>("hltQuadCentralJet30", 4));
                map_1.push_back(std::pair<std::string, int>("hltCaloQuadJet30HT320", 1));
                map_1.push_back(std::pair<std::string, int>("hltPFCentralJetLooseIDQuad30", 4));
                map_1.push_back(std::pair<std::string, int>("hlt1PFCentralJetLooseID75", 1));
                map_1.push_back(std::pair<std::string, int>("hlt2PFCentralJetLooseID60", 2));
                map_1.push_back(std::pair<std::string, int>("hlt3PFCentralJetLooseID45", 3));
                map_1.push_back(std::pair<std::string, int>("hlt4PFCentralJetLooseID40", 4));
                map_1.push_back(std::pair<std::string, int>("hltPFCentralJetsLooseIDQuad30HT330", 1));

                OR = {{}};

                final_map.push_back(map_1);

                var_names.push_back({"j_HT", "pt_4", "j_HT", "pt_4", "pt_1", "pt_2", "pt_3", "pt_4", "j_HT"});
            }
        };
            
        ~make_trigger_paths(){};
        std::vector<std::vector<std::pair<std::string,int>>> inline get_map(){return final_map;};
        std::vector<std::string> inline get_trig_name(){return trig_names; };
        std::vector<std::vector<int> > get_OR(){return OR;};
        std::vector<std::vector<std::string>> get_vars(){return var_names; };
        std::vector<double> get_var_values(std::vector<double> jets_pt, std::vector<double> jets_b_tag, int trig){

            std::vector<double> results;
            if(year== 2016){
                
                std::sort(jets_pt.begin(), jets_pt.end());
                double HT = std::accumulate(jets_pt.begin(), jets_pt.end(), 0);
                std::sort(jets_b_tag.begin(), jets_b_tag.end());

                if(trig == 0)
                    results = {HT, jets_pt[0], jets_b_tag.at(jets_b_tag.size()-1), jets_pt[0]};
                else if(trig == 1)
                    results = {HT, jets_pt[0], jets_pt[2], jets_b_tag.at(jets_b_tag.size()-1), jets_pt[0], jets_pt[2]};

            }

            if(year==2018){

                std::sort(jets_pt.begin(), jets_pt.end());
                double HT = std::accumulate(jets_pt.begin(), jets_pt.end(), 0);
                std::sort(jets_b_tag.begin(), jets_b_tag.end());
                if(trig == 0){
                    results = {HT, jets_pt[0], HT, jets_b_tag.at(jets_b_tag.size()-1), jets_pt[0], jets_pt[3], jets_pt[2], jets_pt[1], jets_pt[0], HT, jets_b_tag.at(jets_b_tag.size()-1)};
                }
            }

            return results;
        };

        std::vector<std::vector<std::pair<double, double>>> get_fil_limits(){

            std::vector<std::vector<std::pair<double, double>>> limits;

            if(year == 2016){
                limits.push_back({std::pair<double, double>(100, 1000), std::pair<double, double>(25, 140), std::pair<double, double>(0, 1), std::pair<double, double>(25, 140)});
                limits.push_back({std::pair<double, double>(100, 1000), std::pair<double, double>(25, 140), std::pair<double, double>(25, 300), std::pair<double, double>(0, 1), std::pair<double, double>(25, 140), std::pair<double, double>(25, 300)});
            }
            if(year==2018){
                limits.push_back({std::pair<double, double>(100, 1000), std::pair<double, double>(25, 140), std::pair<double, double>(100, 1000), std::pair<double, double>(0.7, 1), std::pair<double, double>(25, 140), std::pair<double, double>(25, 500), std::pair<double, double>(25, 400), std::pair<double, double>(25, 300), std::pair<double, double>(25, 140), std::pair<double, double>(100, 1000), std::pair<double, double>(0, 1)});
            }

            return limits;
        };

        std::vector<std::vector<double>> get_bins(){

            std::vector<std::vector<double>> bins;

            if(year == 2016){
                bins.push_back({100, 100, 30, 100});
                bins.push_back({100, 100, 120, 30, 100, 120});
            }
            if(year==2018){
                bins.push_back({100, 100, 100, 30, 100, 100, 120, 150, 170, 100, 30});
            }

            return bins;
        };

};

#endif
