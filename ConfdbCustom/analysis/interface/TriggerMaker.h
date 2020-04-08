#ifndef TRIGGER_MAKER_H
#define TRIGGER_MAKER_H
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "Obj.h"
#include "Event.h"

class TriggerMaker{

    private:
        std::string TrigName;
        std::vector<std::string> Fil_Names;
        std::vector<std::string> Fil_Types;
        std::vector<hltObj::HLTCuts> Cuts;
        std::vector<hltObj::HLTCuts> L1Cuts;
        std::vector<std::string> L1_Fil_Types;
        std::vector<std::string> L1_Required;
        std::vector<std::string> HLT_Required;
        std::map<std::string, std::function<bool(hltObj::Jets, hltObj::HLTCuts)>> FilterMap;
        
    public:
        TriggerMaker(); //default 2018 config
        TriggerMaker(std::string opt); //optional partial only 2018 L1 configuration
        TriggerMaker(std::string TN, std::vector<std::string> FN, std::vector<std::string> FT); //inizializing only names
        ~TriggerMaker(){}; //default destructor
        void SetTrigName(std::string name){ TrigName = name; };
        void SetL1Cuts(std::vector<hltObj::HLTCuts> C){ L1Cuts = C; };
        void SetCuts(std::vector<hltObj::HLTCuts> C){ Cuts = C; };
        void InsertCut(std::string name, std::string Fil_type, std::string req, hltObj::HLTCuts C, int position);
        void PushCut(std::string name, std::string Fil_type, std::string req, hltObj::HLTCuts C);
        //void PushL1Cut(hltObj::HLTCuts C){L1Cuts.push_back(C);};
        void SetFilNames(std::vector<std::string> Fn){ Fil_Names = Fn; };
        void SetFilTypes(std::vector<std::string> Ft){ Fil_Types = Ft; };
        void SetL1FilTypes(std::vector<std::string> L1FT){ L1_Fil_Types = L1FT; };
        void SetL1Required(std::vector<std::string> L1R){ L1_Required = L1R; };
        void SetHLTRequired(std::vector<std::string> HLTR){ HLT_Required = HLTR; };
        int computeL1(Event ev);
        void ModifyCut(std::string FilName, std::string Action, double value);
        void CutFromJson(std::string config);
        int FindFirstZero(std::vector<double> v);
        std::vector<double> Sequence(Event ev);
        bool SkipL1Seed = false;

};
#endif