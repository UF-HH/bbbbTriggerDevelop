#ifndef EVENT_OBJECTS_H
#define EVENT_OBJECTS_H

#include "Obj.h"
#include "TFile.h"
#include "TTree.h"
#include "../interface/HHReweight5D.h"
#include <memory>


class Event{

    private:
        int event_index;
        TFile* infile;
        TTree* tree;
        std::string branch;

        std::string eventType = "data";
        TTree* treeGen;
        std::string genBranch;
        
        //info on event
        int run_;
        ULong64_t event_;
        int lumi_;

        //Kinematical cuts
        std::vector<hltObj::KinCuts> Cut_;

        //Scalar variables
        double Pf_HT;
        double Calo_HT;
        double L1_HT;

        //genLevel Objects
        hltObj::Higgs H1;
        hltObj::Higgs H2;
        hltObj::BQuarks BS;

        //Jets Collections 
        hltObj::Jets RecoJets;
        hltObj::Jets GenJets;
        hltObj::Jets GenNNJets;
        hltObj::Jets L1Jets;
        hltObj::Jets CaloJets;
        hltObj::Jets PFJets;
        hltObj::Jets CaloBJets;
        hltObj::Jets PFBJets;
    
        //Jets Object
        std::vector<hltObj::Jet*> L1J;
        std::vector<hltObj::Jet*> CaloJ;
        std::vector<hltObj::Jet*> PFJ;
        std::vector<hltObj::Jet*> CaloBJ;
        std::vector<hltObj::Jet*> PFBJ;
        std::vector<hltObj::Jet*> RecoJ;
        std::vector<hltObj::Jet*> GenJ;
        std::vector<hltObj::Jet*> GenNNJ;
        std::vector<hltObj::Jet*> RecoJM;
        
        //Storing offline match results
        std::vector<hltObj::Jet> Matches;
        std::vector<hltObj::bQuark> BMatches;
        //Quark objects
        std::vector<hltObj::bQuark*> bs; 

        //reweight obj
        std::shared_ptr<HHReweight5D> hhreweighter;


        //Uggly!!!!! double making of vectors...In Obj.h change default
        //vector attributes to pointer vector so we can assign this directly 
        //to hltObjects...

        std::vector<double>* l1_pt = 0;
        std::vector<double>* l1_eta = 0;
        std::vector<double>* l1_phi = 0;
        std::vector<double>* l1_e = 0;
        std::vector<double>* l1_mass = 0;

        std::vector<double>* calo_pt = 0;
        std::vector<double>* calo_eta = 0;
        std::vector<double>* calo_phi = 0;
        std::vector<double>* calo_e = 0;
        std::vector<double>* calo_et = 0;
        std::vector<double>* calo_mass = 0;
        std::vector<double>* calo_btag = 0;

        std::vector<double>* pf_pt = 0;
        std::vector<double>* pf_eta = 0;
        std::vector<double>* pf_phi = 0;
        std::vector<double>* pf_e = 0;
        std::vector<double>* pf_et = 0;
        std::vector<double>* pf_mass = 0;
        std::vector<double>* pf_btag = 0;

        std::vector<double>* gen_pt = 0;
        std::vector<double>* gen_eta = 0;
        std::vector<double>* gen_phi = 0;
        std::vector<double>* gen_e = 0;
        std::vector<double>* gen_et = 0;
        std::vector<double>* gen_mass = 0;

        std::vector<double>* gen_nonu_pt = 0;
        std::vector<double>* gen_nonu_eta = 0;
        std::vector<double>* gen_nonu_phi = 0;
        std::vector<double>* gen_nonu_e = 0;
        std::vector<double>* gen_nonu_et = 0;
        std::vector<double>* gen_nonu_mass = 0;

        std::vector<double> *reco_jet_pt = 0;
        std::vector<double> *reco_jet_mass = 0;
        std::vector<double> *reco_jet_eta = 0;
        std::vector<double> *reco_jet_phi = 0;
        std::vector<double> *reco_jet_e = 0;
        std::vector<double> *reco_jet_et = 0;
        std::vector<double> *reco_jet_btag = 0;

        
    public:
        float weight = 0; //event weight associated to anomalour coupling, default zero (no multiplication)
        float kl_ev = 1; //anomalous coupling
        Event() : event_index(0) {};
        Event(TFile* f) : event_index(0), infile(f) {};
        Event(TFile* f, std::string b);
        Event(TFile* f, std::string b, std::string genb);
        ~Event(){};
        void SetInFile(TFile* f) {infile = f;};
        void SetBranch(std::string b){branch = b;};
        void SetGenBranch(std::string b){genBranch = b; eventType = "MC";};
        void SetEventType(std::string evt){eventType = evt;};
        void Init();
        int GetRun(){return run_;};
        int GetLS(){return lumi_;};
        ULong64_t GetEvent(){return event_;};
        void Generate();
        void clear();
        void SetStart(int i){ event_index = i; };
        void AddCut(hltObj::KinCuts c){ Cut_.push_back(c);};
        double GetPFHT();
        double GetCaloHT();
        double GetL1HT();
        double GetHT(hltObj::KinCuts cut);
        hltObj::Jets GetL1JetsCollection(){return L1Jets; };
        hltObj::Jets GetCaloJetsCollection(){return CaloJets; };
        hltObj::Jets GetPFJetsCollection(){ return PFJets; };
        hltObj::Jets GetGenJetsCollection(){ return GenJets; };
        hltObj::Jets GetGenNNJetsCollection(){ return GenNNJets; };
        hltObj::Jets GetCaloBJetsCollection(){return CaloBJets; };
        hltObj::Jets GetPFBJetsCollection(){ return PFBJets; };
        hltObj::Jets GetRecoJetsCollection(){ return RecoJets; };
        std::vector<hltObj::Jet*> GetL1Jets(){ return L1J; };
        std::vector<hltObj::Jet*> GetCaloJets(){ return CaloJ; };
        std::vector<hltObj::Jet*> GetPFJets(){ return PFJ; };
        std::vector<hltObj::Jet*> GetGenJets(){ return GenJ; };
        std::vector<hltObj::Jet*> GetGenNNJets(){ return GenNNJ; };
        std::vector<hltObj::Jet*> GetCaloBJets(){ return CaloBJ; };
        std::vector<hltObj::Jet*> GetPFBJets(){ return PFBJ; };
        std::vector<hltObj::Jet*> GetRecoJets(){ return RecoJ; };
        std::vector<hltObj::Jet*> GetMatchJetsToB(){ return RecoJM; }; //Get jets matched to b quarks
        std::vector<hltObj::bQuark*> GetBQuarks(){ return bs; }; // get bs in random order
        std::vector<hltObj::Jet> GetMatches(){ return Matches; }; //reference to orded objects of b quarks
        std::vector<hltObj::bQuark> GetBMatches(){ return BMatches; };
        std::pair<hltObj::Higgs, hltObj::Higgs> GetHiggsPair(){ return std::pair<hltObj::Higgs, hltObj::Higgs>(H1, H2); };
        int GetEntries(){ return tree->GetEntries(); }; 
        void WeightFactory(std::string kl_map, std::string kl_histo, std::string kl_coeffs);
        void compute_weight(float kl);
        void UnpackCollections();
        void jetMatch(double R, std::string Reference, std::string SelectedJets);
        void jetMatch(double R, std::vector<hltObj::Jet*> Reference, std::string SelectedJets);
        void bMatch(double R, std::string SelectedJets);
        bool isResolved(double R);


};

#endif
