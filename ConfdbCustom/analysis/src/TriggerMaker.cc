#include "../interface/TriggerMaker.h"
#include "../interface/FilterSelector.h"
#include <iostream>

//using namespace cstmfltr;

TriggerMaker::TriggerMaker(){

    FilterMap = {

            std::make_pair("CustomFixedPtEtaSelector", cstmfltr::CustomFixedPtEtaSelector),
            std::make_pair("CustomVariablePtEtaSelector", cstmfltr::CustomVariablePtEtaSelector),
            std::make_pair("CustomFixedPtEtaHTSelector", cstmfltr::CustomFixedPtEtaHTSelector),
            std::make_pair("CustomFixedHTSelector", cstmfltr::CustomFixedHTSelector),
            std::make_pair("CustomVariablePtEtaHTSelector", cstmfltr::CustomVariablePtEtaHTSelector),
            std::make_pair("CustomFixedBtagSelector", cstmfltr::CustomFixedBtagSelector),
       
        };

    //Default constructor is the 2018 HLT configuration for HH->bbbb without b tag.
    // You can always change this.
    TrigName = "HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9";
    Fil_Names = {"hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet", "hltQuadCentralJet30", "hltCaloQuadJet30HT320", 
                "hltPFCentralJetLooseIDQuad30", "hlt1PFCentralJetLooseID75", "hlt2PFCentralJetLooseID60", "hlt3PFCentralJetLooseID45",
                "hlt4PFCentralJetLooseID40", "hltPFCentralJetsLooseIDQuad30HT330"};
        
    Fil_Types = {"CustomFixedPtEtaSelector", "CustomFixedPtEtaHTSelector", "CustomFixedPtEtaSelector", "CustomFixedPtEtaSelector", "CustomFixedPtEtaSelector", 
                "CustomFixedPtEtaSelector", "CustomFixedPtEtaSelector", "CustomFixedPtEtaHTSelector"};
    
    hltObj::HLTCuts hltQuadCentralJet30(30.0, 1.0E99, -2.5, 2.5, 4);
    hltObj::HLTCuts hltCaloQuadJet30HT320(30.0, 1.0E99, -2.5, 2.5, 320.0, 0.0, 1);
    hltObj::HLTCuts hltPFCentralJetLooseIDQuad30(30.0, 1.0E99, -2.5, 2.5, 4);
    hltObj::HLTCuts hlt1PFCentralJetLooseID75(75.0, 1.0E99, -2.5, 2.5, 1);
    hltObj::HLTCuts hlt2PFCentralJetLooseID60(60.0, 1.0E99, -2.5, 2.5, 2);
    hltObj::HLTCuts hlt3PFCentralJetLooseID45(45.0, 1.0E99, -2.5, 2.5, 3);
    hltObj::HLTCuts hlt4PFCentralJetLooseID40(40.0, 1.0E99, -2.5, 2.5, 4);
    hltObj::HLTCuts hltPFCentralJetsLooseIDQuad30HT330(30.0, 1.0E99, -2.5, 2.5, 330.0, 0.0, 1);

    Cuts = {hltQuadCentralJet30, hltCaloQuadJet30HT320, hltPFCentralJetLooseIDQuad30, hlt1PFCentralJetLooseID75,
            hlt2PFCentralJetLooseID60, hlt3PFCentralJetLooseID45, hlt4PFCentralJetLooseID40, hltPFCentralJetsLooseIDQuad30HT330};
    
    //We could Improve this part, it's quite shit
    hltObj::HLTCuts one(280.0);
    hltObj::HLTCuts two(320.0);
    hltObj::HLTCuts three(360.0);
    hltObj::HLTCuts four(400.0);
    hltObj::HLTCuts five(450.0);
    hltObj::HLTCuts six(60, 1.0E99, -2.5, 2.5);
    six.MinN=4;

    std::vector<double> pt_min = {70.0, 55.0, 40.0, 35.0};
    std::vector<double> pt_max = {1.0E99, 1.0E99, 1.0E99, 1.0E99};
    std::vector<double> eta_min = {-2.4, -2.4, -2.4, -2.4};
    std::vector<double> eta_max = {2.4, 2.4, 2.4, 2.4};
    hltObj::HLTCuts seven(pt_min, pt_max, eta_min, eta_max);
    seven.HTMin = 280.0;

    pt_min = {70,55,40,40};
    hltObj::HLTCuts eight(pt_min, pt_max, eta_min, eta_max);
    eight.HTMin = 320.0;

    pt_min = {80.0, 60.0, 45.0, 40.0};
    eta_min = {-2.1, -2.1, -2.3, -2.3};
    eta_max = {2.1, 2.1, 2.3, 2.3};
    hltObj::HLTCuts nine(pt_min, pt_max, eta_min, eta_max);
    nine.HTMin = 320.0;

    pt_min = {80.0, 60.0, 50.0, 45.0};
    hltObj::HLTCuts ten(pt_min, pt_max, eta_min, eta_max);
    ten.HTMin = 320.0;

    L1Cuts = {one, two, three, four, five, six, seven , eight, nine, ten};

    L1_Fil_Types = {"CustomFixedHTSelector", "CustomFixedHTSelector", "CustomFixedHTSelector", "CustomFixedHTSelector",  
                "CustomFixedHTSelector", "CustomFixedPtEtaSelector", "CustomVariablePtEtaHTSelector", "CustomVariablePtEtaHTSelector", 
                "CustomVariablePtEtaHTSelector", "CustomVariablePtEtaHTSelector" };
    L1_Required = {"L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets"};

    HLT_Required = { "CaloJets", "CaloJets", "PFJets", "PFJets", "PFJets", "PFJets", "PFJets", "PFJets" };

};

TriggerMaker::TriggerMaker(std::string TN, std::vector<std::string> FN, std::vector<std::string> FT){
    
    TrigName = TN; Fil_Names = FN;  Fil_Types = FT;
    FilterMap = {

            std::make_pair("CustomFixedPtEtaSelector", cstmfltr::CustomFixedPtEtaSelector),
            std::make_pair("CustomVariablePtEtaSelector", cstmfltr::CustomVariablePtEtaSelector),
            std::make_pair("CustomFixedPtEtaHTSelector", cstmfltr::CustomFixedPtEtaHTSelector),
            std::make_pair("CustomFixedHTSelector", cstmfltr::CustomFixedHTSelector),
            std::make_pair("CustomVariablePtEtaHTSelector", cstmfltr::CustomVariablePtEtaHTSelector),
            std::make_pair("CustomFixedBtagSelector", cstmfltr::CustomFixedBtagSelector),
       
        };

};

TriggerMaker::TriggerMaker(std::string opt = "l1"){

    if(opt == "l1"){

        FilterMap = {

                std::make_pair("CustomFixedPtEtaSelector", cstmfltr::CustomFixedPtEtaSelector),
                std::make_pair("CustomVariablePtEtaSelector", cstmfltr::CustomVariablePtEtaSelector),
                std::make_pair("CustomFixedPtEtaHTSelector", cstmfltr::CustomFixedPtEtaHTSelector),
                std::make_pair("CustomFixedHTSelector", cstmfltr::CustomFixedHTSelector),
                std::make_pair("CustomVariablePtEtaHTSelector", cstmfltr::CustomVariablePtEtaHTSelector),
                std::make_pair("CustomFixedBtagSelector", cstmfltr::CustomFixedBtagSelector),
        
            };

        //Default constructor is the 2018 HLT configuration for HH->bbbb without b tag.
        // You can always change this.
        TrigName = "HLT_PFHT330PT30_QuadPFJet_75_60_45_40_v9";
        
        //We could Improve this part, it's quite shit
        hltObj::HLTCuts one(280.0);
        hltObj::HLTCuts two(320.0);
        hltObj::HLTCuts three(360.0);
        hltObj::HLTCuts four(400.0);
        hltObj::HLTCuts five(450.0);
        hltObj::HLTCuts six(60, 1.0E99, -2.5, 2.5);
        six.MinN=4;

        std::vector<double> pt_min = {70.0, 55.0, 40.0, 35.0};
        std::vector<double> pt_max = {1.0E99, 1.0E99, 1.0E99, 1.0E99};
        std::vector<double> eta_min = {-2.4, -2.4, -2.4, -2.4};
        std::vector<double> eta_max = {2.4, 2.4, 2.4, 2.4};
        hltObj::HLTCuts seven(pt_min, pt_max, eta_min, eta_max);
        seven.HTMin = 280.0;

        pt_min = {70,55,40,40};
        hltObj::HLTCuts eight(pt_min, pt_max, eta_min, eta_max);
        eight.HTMin = 320.0;

        pt_min = {80.0, 60.0, 45.0, 40.0};
        eta_min = {-2.1, -2.1, -2.3, -2.3};
        eta_max = {2.1, 2.1, 2.3, 2.3};
        hltObj::HLTCuts nine(pt_min, pt_max, eta_min, eta_max);
        nine.HTMin = 320.0;

        pt_min = {80.0, 60.0, 50.0, 45.0};
        hltObj::HLTCuts ten(pt_min, pt_max, eta_min, eta_max);
        ten.HTMin = 320.0;

        L1Cuts = {one, two, three, four, five, six, seven , eight, nine, ten};

        L1_Fil_Types = {"CustomFixedHTSelector", "CustomFixedHTSelector", "CustomFixedHTSelector", "CustomFixedHTSelector",  
                    "CustomFixedHTSelector", "CustomFixedPtEtaSelector", "CustomVariablePtEtaHTSelector", "CustomVariablePtEtaHTSelector", 
                    "CustomVariablePtEtaHTSelector", "CustomVariablePtEtaHTSelector" };
        L1_Required = {"L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets", "L1Jets"};
    }
}

int TriggerMaker::computeL1(Event ev){

    bool pass = false;

    //checking consistency of length
    if(!(L1_Fil_Types.size() == L1Cuts.size()) || !(L1_Required.size() == L1Cuts.size())){
        std::cerr<< "L1 Configuration does not make sense. Check for input Cuts, names and required objects" << std::endl;
        //throw std::runtime_error(("Vector length does not coincide").c_str());
    }
    
    hltObj::Jets ReqObj;
    for(int i = 0; i < L1_Fil_Types.size(); i++){

        //Check for object required...No need for check, at L1 we should always call for
        //L1 objects but still we can play around.

        //this ifs are shit...they slow down
        if(L1_Required.at(i) == "L1Jets")
            ReqObj = ev.GetL1JetsCollection();
        else if(L1_Required.at(i) == "CaloJets")
            ReqObj = ev.GetCaloJetsCollection();
        else if(L1_Required.at(i) == "PFJets")
            ReqObj = ev.GetCaloJetsCollection();
        else if(L1_Required.at(i) == "CaloBJets")
            ReqObj = ev.GetCaloBJetsCollection();
        else if(L1_Required.at(i) == "PFBJets")
            ReqObj = ev.GetPFBJetsCollection();
        else if(L1_Required.at(i) != "L1Jets" && L1_Required.at(i) != "CaloJets" && L1_Required.at(i) != "PFJets" && L1_Required.at(i) != "CaloBJets" && L1_Required.at(i) != "PFBJets"){
            std::cerr<< "Required L1 Object does not exist." << std::endl;
            throw std::runtime_error((L1_Required.at(i) + " Is not a valid type: L1Jets, CaloJets, PFJets.").c_str());
        }

        pass = pass || FilterMap[L1_Fil_Types.at(i)](ReqObj, L1Cuts.at(i));
        if(pass == true)
            return true;
    }
    return pass;
};


int TriggerMaker::FindFirstZero(std::vector<double> v){
    //actually the for is quicker than std::find by order of magnitude
    for(int i = 0; i < v.size(); i++){
        if(v.at(i)==0){
            return i;
        }
    }
    return -1;
}

std::vector<double> TriggerMaker::Sequence(Event ev){

    std::vector<double> results; 

    hltObj::Jets ReqObj;

    bool pass = true;

    if(!SkipL1Seed){
        if(L1Cuts.size() == 0){
            std::cerr << "Warning, no L1 seed in this trigger" << std::endl;
        }
        results.push_back(computeL1(ev));
    }

    for(int i = 0; i < Fil_Types.size(); i++){

        if(pass){
            //this ifs are shit...they slow down
            if(HLT_Required.at(i) == "L1Jets")
                ReqObj = ev.GetL1JetsCollection();
            else if(HLT_Required.at(i) == "CaloJets")
                ReqObj = ev.GetCaloJetsCollection();
            else if(HLT_Required.at(i) == "PFJets")
                ReqObj = ev.GetPFJetsCollection();
            else if(HLT_Required.at(i) == "CaloBJets")
                ReqObj = ev.GetCaloBJetsCollection();
            else if(HLT_Required.at(i) == "PFBJets")
                ReqObj = ev.GetPFBJetsCollection();
            else if(HLT_Required.at(i) != "L1Jets" && HLT_Required.at(i) != "CaloJets" && HLT_Required.at(i) != "PFJets" && HLT_Required.at(i) != "CaloBJets" && HLT_Required.at(i) != "PFBJets"){
                std::cerr<< "Required HLT Object does not exist." << std::endl;
                throw std::runtime_error((L1_Required.at(i) + " Is not a valid type: L1Jets, CaloJets, PFJets.").c_str());
            }
            pass = pass && FilterMap[Fil_Types.at(i)](ReqObj, Cuts.at(i));
            results.push_back(FilterMap[Fil_Types.at(i)](ReqObj, Cuts.at(i)));
        }
        else{
            results.push_back(0);
        }
    }

    //sequentiality assured here 
    int stop = FindFirstZero(results);

    if(stop != -1){
        for(int i = stop; i < results.size(); i++){
            results.at(i) = 0;
        }
    }

    //mult by weight. That's why the vector is a double
    if(ev.weight != 0){
        for(int i = 0; i < stop; i++){
            results.at(i)*=ev.weight;
        }
    }

    return results;
};

void TriggerMaker::ModifyCut(std::string FilName, std::string Action, double value){
    
    int index = std::distance(Fil_Names.begin(), std::find(Fil_Names.begin(), Fil_Names.end(), FilName));
    //index-1 because first name is always the l1 name...implementing a L1 search?

    for(auto p : Cuts.at(index-1).MappedSetCuts){
        if(p.first == Action){
            std::cout << "Before" << std::endl;
            std::cout << Cuts.at(index-1).PtMin << std::endl;
            std::cout << Cuts.at(index-1).EtaMin << std::endl;
            std::cout << Cuts.at(index-1).EtaMax << std::endl;
            std::cout << Cuts.at(index-1).BtagMin << std::endl;
            (Cuts.at(index-1).*p.second)(value);
            std::cout << "After" << std::endl;
            std::cout << Cuts.at(index-1).PtMin << std::endl;
            std::cout << Cuts.at(index-1).EtaMin << std::endl;
            std::cout << Cuts.at(index-1).EtaMax << std::endl;
            std::cout << Cuts.at(index-1).BtagMin << std::endl;
            //std::cout << Cuts.at(index-1).PtMin << std::endl;
        }
        
    }
};

void TriggerMaker::CutFromJson(std::string config){

    std::ifstream cut_file (config);

    bool Continue = true;

    std::string line;
    std::string mod_name;
    std::string mod_type;
    std::string algo_name;
    std::string algo_type;
    
    if (cut_file.is_open()){
            
        while(std::getline(cut_file, line)){
                        
            std::string delimeter = ",";
            std::string value_delimeter = ":";
            
            mod_type = line.substr(0, line.find(value_delimeter));
            line.erase(0, line.find(value_delimeter) + value_delimeter.length());
            mod_name = line.substr(0, line.find(delimeter));
            line.erase(0, line.find(delimeter) + value_delimeter.length());
            
            algo_type = line.substr(0, line.find(value_delimeter));
            line.erase(0, line.find(value_delimeter) + value_delimeter.length());
            algo_name = line.substr(0, line.find(delimeter));
            line.erase(0, line.find(delimeter) + value_delimeter.length());
            
            
            while(line.find(delimeter) != std::string::npos){
                
                std::string action = line.substr(0, line.find(value_delimeter));
                line.erase(0, line.find(value_delimeter) + value_delimeter.length());
                double set = std::stod(line.substr(0, line.find(delimeter)));
                line.erase(0, line.find(delimeter) + delimeter.length());
            
                ModifyCut(mod_name, action , set);                    
            
            }
        
        }
        
        cut_file.close();
    } 

    else std::cout << "Unable to open file";

    return;
};

void TriggerMaker::InsertCut(std::string name, std::string Fil_type, std::string req, hltObj::HLTCuts C, int position){
    
    //FilNames has also the L1 name!!
    Fil_Names.insert(Fil_Names.begin() + position+1, name); 
    Fil_Types.insert(Fil_Types.begin() + position, Fil_type); 
    Cuts.insert(Cuts.begin() + position, C); 
    HLT_Required.insert(HLT_Required.begin() + position, req); 
};

void TriggerMaker::PushCut(std::string name, std::string Fil_type, std::string req, hltObj::HLTCuts C ){
    Fil_Names.push_back(name);
    Fil_Types.push_back(Fil_type);
    Cuts.push_back(C); 
    HLT_Required.push_back(req); 
};




