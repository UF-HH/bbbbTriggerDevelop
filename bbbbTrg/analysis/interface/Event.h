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
        //genLevel Objects
        hltObj::Higgs H1;
        hltObj::Higgs H2;
        hltObj::BQuarks BS;
        //Jets Object
        std::vector<hltObj::Jet*> L1J;
        std::vector<hltObj::Jet*> CaloJ;
        std::vector<hltObj::Jet*> PFJ;
        std::vector<hltObj::Jet*> CaloBJ;
        std::vector<hltObj::Jet*> PFBJ;
        std::vector<hltObj::Jet*> RecoJ;
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

        //Match jets
        std::vector<double>* matched_jet_pt = 0;
        std::vector<double>* matched_jet_mass = 0;
        std::vector<double>* matched_jet_eta = 0;
        std::vector<double>* matched_jet_phi = 0;
        std::vector<double>* matched_jet_energy = 0;
        std::vector<double>* matched_jet_b_tag = 0;

        //Reco jets
        std::vector<double>* reco_jet_pt = 0;
        std::vector<double>* reco_jet_et = 0;
        std::vector<double>* reco_jet_mass = 0;
        std::vector<double>* reco_jet_eta = 0;
        std::vector<double>* reco_jet_phi = 0;
        std::vector<double>* reco_jet_energy = 0;
        std::vector<double>* reco_jet_b_tag = 0;

        //Trigger Objects
        std::vector<double>* trig_obj_pt = 0 ;
        std::vector<double>* trig_obj_et = 0 ;
        std::vector<double>* trig_obj_mass = 0;
        std::vector<double>* trig_obj_eta = 0;
        std::vector<double>* trig_obj_phi = 0;

        std::vector<int>* is_calo_jet = 0;
        std::vector<int>* is_calo_bjet = 0;

        std::vector<int>* is_pf_jet = 0;
        std::vector<int>* is_pf_bjet = 0;

        std::vector<int>* is_jet = 0;
        std::vector<int>* is_bjet = 0;

        std::vector<int>* is_calo_ht = 0;
        std::vector<int>* is_pf_ht = 0;
        std::vector<int>* is_l1_ht = 0;
        std::vector<int>* is_l1_jet = 0;

        
    public:
        float weight = 0; //event weight associated to anomalour coupling, default zero (no multiplication)
        float kl_ev = 1; //anomalous coupling
        Event() : event_index(0) {};
        Event(TFile* f) : event_index(0), infile(f) {};
        Event(TFile* f, std::string b);
        ~Event(){};
        void Init();
        void SetInFile(TFile* f) {infile = f;};
        void SetInBranch(std::string b){branch = b, Init(); };
        void Generate();
        void clear();
        void SetStart(int i){ event_index = i; };
        void AddCut(hltObj::KinCuts c){ Cut_.push_back(c);};
        double GetPFHT();
        double GetCaloHT();
        double GetL1HT();
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
        std::vector<hltObj::Jet*> GetRecoJetsMatch(){ return RecoJM; };
	    std::vector<hltObj::bQuark*> GetBQuarks(){ return bs; };
        std::vector<hltObj::Jet> GetMatches(){ return Matches; };
        std::vector<hltObj::bQuark> GetBMatches(){ return BMatches; };
        std::pair<hltObj::Higgs, hltObj::Higgs> GetHiggsPair(){ return std::pair<hltObj::Higgs, hltObj::Higgs>(H1, H2); };
        int GetEntries(){ tree->GetEntries(); }; 
        void WeightFactory(std::string kl_map, std::string kl_histo, std::string kl_coeffs);
        void compute_weight(float kl);
        void UnpackCollections();
        //void trg_jet_match(double R, std::string SelectedJets);
        void jetMatch(double R, std::string Reference, std::string SelectedJets);
        void bMatch(double R, std::string SelectedJets);


};

#endif
