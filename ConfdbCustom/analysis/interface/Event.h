#ifndef EVENT_OBJECTS_H
#define EVENT_OBJECTS_H

#include "Obj.h"
#include "TFile.h"
#include "TTree.h"
#include <memory>


class Event{

    private:
        int event_index;
        TFile* infile;
        TTree* tree;
        std::string branch;
        
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

        //Jets Collections 
        hltObj::Jets RecoJets;
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
        //Storing offline match results
        std::vector<hltObj::Jet> Matches;


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
        std::vector<double>* calo_mass = 0;
        std::vector<double>* calo_btag = 0;

        std::vector<double>* pf_pt = 0;
        std::vector<double>* pf_eta = 0;
        std::vector<double>* pf_phi = 0;
        std::vector<double>* pf_e = 0;
        std::vector<double>* pf_mass = 0;
        std::vector<double>* pf_btag = 0;

        std::vector<double> *reco_jet_pt = 0;
        std::vector<double> *reco_jet_mass = 0;
        std::vector<double> *reco_jet_eta = 0;
        std::vector<double> *reco_jet_phi = 0;
        std::vector<double> *reco_jet_energy = 0;
        std::vector<double> *reco_jet_et = 0;
        std::vector<double> *reco_jet_b_tag = 0;

        
    public:

        double weight = 1.;
        Event() : event_index(0) {};
        Event(TFile* f) : event_index(0), infile(f) {};
        Event(TFile* f, std::string b);
        ~Event(){};
        void Init();
        void SetInFile(TFile* f) {infile = f;};
        void SetInBranch(std::string b){branch = b, Init(); };
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
        hltObj::Jets GetCaloBJetsCollection(){return CaloBJets; };
        hltObj::Jets GetPFBJetsCollection(){ return PFBJets; };
        std::vector<hltObj::Jet*> GetL1Jets(){ return L1J; };
        std::vector<hltObj::Jet*> GetCaloJets(){ return CaloJ; };
        std::vector<hltObj::Jet*> GetPFJets(){ return PFJ; };
        std::vector<hltObj::Jet*> GetCaloBJets(){ return CaloBJ; };
        std::vector<hltObj::Jet*> GetPFBJets(){ return PFBJ; };
        std::vector<hltObj::Jet*> GetRecoJets(){ return RecoJ; };
        std::vector<hltObj::Jet> GetMatches(){ return Matches; };
        int GetEntries(){ return tree->GetEntries(); }; 
        void UnpackCollections();
        void jetMatch(double R, std::string Reference, std::string SelectedJets);


};

#endif
