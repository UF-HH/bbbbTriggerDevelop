#ifndef TO_TREE_NOB_CC
#define TO_TREE_NOB_CC

//#include "bbbbWeight/bbbbWeight/interface/HHReweight5D.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenStatusFlags.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

//
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
//

#include "TTree.h"
#include "TLorentzVector.h"

#include "../interface/bjmatch.h"

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <algorithm>

using namespace edm; //edm is CMSSW stores events in ROOT trees
// using namespace l1t;
using namespace reco;
using namespace std;

class TO_Tree_nob : public edm::EDAnalyzer {
    public:
        explicit TO_Tree_nob(const edm::ParameterSet&);
        virtual ~TO_Tree_nob(){};

    private:
        //----edm control---
        virtual void beginJob();
        virtual void beginRun(edm::Run const&, edm::EventSetup const&);
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob();
        virtual void endRun(edm::Run const&, edm::EventSetup const&){};

        void reset();
        bool getOR();
        void check();

        static std::vector<string> remove_duplicates(std::vector<string> vIn);

        bool verbose_;

        const edm::EDGetTokenT< pat::TriggerObjectStandAloneCollection > triggerObjectsToken_;
        const edm::EDGetTokenT< edm::TriggerResults >                    triggerBitsToken_;
        const edm::EDGetTokenT< reco::GenParticleCollection >            gensToken_; //GenParticles
        const edm::EDGetTokenT< std::vector<pat::Jet> >                  jetsToken_; //jets

        std::string calojet_fil_2018 = "hltCaloJetCollection20Filter";
        std::string pfjet_fil_2018 = "hltAK4PFJetCollection20Filter";
        std::string calojet_HT_2018 = "hltCaloHTOpenFilter";
        std::string pfjet_HT_2018 = "hltPFHTOpenFilter";
        std::string l1_objects_2018 = "hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet";
        std::string calo_ht_label = "hltCaloQuadJet30HT320";
        std::string pf_ht_label = "hltPFCentralJetsLooseIDQuad30HT330";
        std::string string_hltPFCentralJetLooseIDQuad30 = "hltPFCentralJetLooseIDQuad30";
        std::string string_hltAK4PFJetsLooseID = "hltAK4PFJetsLooseID";


        HLTConfigProvider hltConfig_;
        edm::InputTag trgprocessName_;

        std::unordered_map <string, int> trgs_of_interest_; // maps the trg name to the idx of the HLT vector
        std::vector<string>    trgs_of_interest_names_; // the keys of the above
        std::vector<long long> trgs_of_interest_counts_; // the counts of the above
        long long tot_evts_;

        std::vector<string> filters_of_interest_names_;

        std::vector<int> trg_pass_; // one int (0/1) for every trigger of interest
        std::vector<int> filter_count_; // one int to count num objects that pass a certain filer

        //Trigger objects
        std::vector<double>* trig_obj_pt = new std::vector<double>;
        std::vector<double>* trig_obj_et = new std::vector<double>;
        std::vector<double>* trig_obj_mass = new std::vector<double>;
        std::vector<double>* trig_obj_eta = new std::vector<double>;
        std::vector<double>* trig_obj_phi = new std::vector<double>;
        //std::vector<double>* trig_calo_obj_btag = new std::vector<double>;
        //std::vector<double>* trig_pf_obj_btag = new std::vector<double>;

        //PF Jets Loose ID 
        //std::vector<double>* chargedHadronEnergyFraction = new std::vector<double>;
        //std::vector<double>* neutralHadronEnergyFraction = new std::vector<double>;
        //std::vector<double>* chargedEmEnergyFraction = new std::vector<double>;
        //std::vector<double>* neutralEmEnergyFraction = new std::vector<double>;
        //std::vector<double>* chargedMultiplicity = new std::vector<double>;
        //std::vector<double>* numberOfDaughters = new std::vector<double>;

        //IS
        std::vector<int>* is_calo_jet = new std::vector<int>;
        std::vector<int>* is_calo_bjet = new std::vector<int>;
        std::vector<int>* is_l1_ht = new std::vector<int>;
        std::vector<int>* is_l1_jet = new std::vector<int>;

        std::vector<int>* is_pf_jet = new std::vector<int>;
        std::vector<int>* is_pf_bjet = new std::vector<int>;

        std::vector<int>* is_jet = new std::vector<int>;
        std::vector<int>* is_bjet = new std::vector<int>;

        std::vector<int>* is_pf_ht = new std::vector<int>;
        std::vector<int>* is_calo_ht = new std::vector<int>;
        std::vector<int>* is_hltPFCentralJetLooseIDQuad30 = new std::vector<int>;
        std::vector<int>* is_hltAK4PFJetsLooseID = new std::vector<int>;

        //Match jets
        std::vector<double>* matched_jet_pt = new std::vector<double>;
        std::vector<double>* matched_jet_mass = new std::vector<double>;
        std::vector<double>* matched_jet_eta = new std::vector<double>;
        std::vector<double>* matched_jet_phi = new std::vector<double>;
        std::vector<double>* matched_jet_energy = new std::vector<double>;
        std::vector<double>* matched_jet_b_tag = new std::vector<double>;

        //Reco jets
        std::vector<double>* reco_jet_pt = new std::vector<double>;
        std::vector<double>* reco_jet_mass = new std::vector<double>;
        std::vector<double>* reco_jet_eta = new std::vector<double>;
        std::vector<double>* reco_jet_phi = new std::vector<double>;
        std::vector<double>* reco_jet_energy = new std::vector<double>;
        std::vector<double>* reco_jet_et = new std::vector<double>;
        std::vector<double>* reco_jet_b_tag = new std::vector<double>;


        //b quarks
        std::vector<double>* b_pt = new std::vector<double>;
        std::vector<double>* b_eta = new std::vector<double>;
        std::vector<double>* b_phi = new std::vector<double>;
        std::vector<double>* b_et = new std::vector<double>;
        std::vector<double>* b_e = new std::vector<double>;

        //Cuts
        bool      cut_;

        // vars from HHReweight5D
        float gen_H1_m;
        float gen_H1_pt;
        float gen_H1_eta;
        float gen_H1_phi;
        TLorentzVector gen_H1_p4;

        float gen_H2_m;
        float gen_H2_pt;
        float gen_H2_eta;
        float gen_H2_phi;
        TLorentzVector gen_H2_p4;

        float gen_mHH;
        float gen_costh_H1_cm;
        float gen_costh_H2_cm;

        TLorentzVector vH1;

        int       count;

        std::vector<double> Higgs_part; 
        TLorentzVector Higgs1_p4;
        TLorentzVector Higgs2_p4;
 


        TTree *tree_;
};

    TO_Tree_nob::TO_Tree_nob(const edm::ParameterSet& iConfig):
    triggerObjectsToken_ (consumes< pat::TriggerObjectStandAloneCollection >  (iConfig.getParameter<edm::InputTag>("triggerObjects"))),
    triggerBitsToken_    (consumes< edm::TriggerResults >                     (iConfig.getParameter<edm::InputTag>("triggerResults"))),
    gensToken_           (consumes< reco::GenParticleCollection >             (iConfig.getParameter<edm::InputTag>("genP"))),
    jetsToken_           (consumes< std::vector<pat::Jet>   >                 (iConfig.getParameter<edm::InputTag>("reco_jets")))
{
    trgprocessName_            = iConfig.getParameter<edm::InputTag>("triggerResults");
    trgs_of_interest_names_    = iConfig.getParameter<std::vector<string>> ("triggerList");
    filters_of_interest_names_ = iConfig.getParameter<std::vector<string>> ("filterList");

    // remove possible duplicates
    trgs_of_interest_names_    = remove_duplicates(trgs_of_interest_names_);
    filters_of_interest_names_ = remove_duplicates(filters_of_interest_names_);

    verbose_ = iConfig.getParameter<bool>("verbose");

    // loop through all the names
    for (auto s : trgs_of_interest_names_)
    {
        trgs_of_interest_[s] = -1; // invalid ref
        trgs_of_interest_counts_.push_back(0); // 0 counts at startup
    }

    tot_evts_ = 0;
}

std::vector<string> TO_Tree_nob::remove_duplicates(std::vector<string> vIn)
{
    std::vector<string> vOut;
    for (string s : vIn)
    {
        if (std::find(vOut.begin(), vOut.end(), s) == vOut.end())
            vOut.push_back(s);
        else
            cout << "[INFO] : " << s << " found as duplicated and removed" << endl;
    }
    return vOut;
}


void TO_Tree_nob::beginJob()
{
    edm::Service<TFileService> fs;
    tree_ = fs -> make<TTree>("trgObjTree", "trgObjTree");

    //---------------------------------------
    //Filters and triggeer info for the event
    //---------------------------------------

    trg_pass_.resize(trgs_of_interest_names_.size()); // one counter per trigger
    filter_count_.resize(filters_of_interest_names_.size()); // one counter per filters
    Higgs_part.resize(8);

    // set the branches in the tree
    for (size_t it = 0; it < trgs_of_interest_names_.size(); ++it)
    {
        string bname = trgs_of_interest_names_.at(it);
        cout << ".... new branch (TRG) : " << bname << endl;
        tree_->Branch(bname.c_str(), &(trg_pass_.at(it)));
    }

    for (size_t iflt = 0; iflt < filters_of_interest_names_.size(); ++iflt)
    {
        string bname = filters_of_interest_names_.at(iflt);
        cout << ".... new branch (FILTER): " << bname << endl;
        tree_->Branch(bname.c_str(), &(filter_count_.at(iflt)));
    }


    //-----------------------
    //Trigger Objects vectors
    //-----------------------

    tree_->Branch("trig_obj_pt", &trig_obj_pt);
    tree_->Branch("trig_obj_et", &trig_obj_et);
    tree_->Branch("trig_obj_mass", &trig_obj_mass);
    tree_->Branch("trig_obj_eta", &trig_obj_eta);
    tree_->Branch("trig_obj_phi", &trig_obj_phi);

    //-----------------------
    //Parton Matched Jets vectors
    //-----------------------

    tree_->Branch("matched_jet_pt", &matched_jet_pt);
    tree_->Branch("matched_jet_mass", &matched_jet_mass);
    tree_->Branch("matched_jet_eta", &matched_jet_eta);
    tree_->Branch("matched_jet_phi", &matched_jet_phi);
    tree_->Branch("matched_jet_energy", &matched_jet_energy);
    tree_->Branch("matched_jet_b_tag", &matched_jet_b_tag);

    //-----------------------
    //Reco jets collection
    //-----------------------

    tree_->Branch("reco_jet_pt", &reco_jet_pt);
    tree_->Branch("reco_jet_mass", &reco_jet_mass);
    tree_->Branch("reco_jet_eta", &reco_jet_eta);
    tree_->Branch("reco_jet_phi", &reco_jet_phi);
    tree_->Branch("reco_jet_energy", &reco_jet_energy);
    tree_->Branch("reco_jet_et", &reco_jet_et);
    tree_->Branch("reco_jet_b_tag", &reco_jet_b_tag);

    //-----------------------
    //PF Loose ID
    //-----------------------
    //tree_->Branch("chargedHadronEnergyFraction", &chargedHadronEnergyFraction);
    //tree_->Branch("neutralHadronEnergyFraction", &neutralHadronEnergyFraction);
    //tree_->Branch("chargedEmEnergyFraction", &chargedEmEnergyFraction);
    //tree_->Branch("neutralEmEnergyFraction", &neutralEmEnergyFraction);
    //tree_->Branch("chargedMultiplicity", &chargedMultiplicity);
    //tree_->Branch("numberOfDaughters", &numberOfDaughters);

    //-----------------------
    //IS
    //-----------------------

    tree_->Branch("is_jet", &is_jet);
    tree_->Branch("is_bjet", &is_bjet);
    tree_->Branch("is_calo_jet", &is_calo_jet);
    tree_->Branch("is_calo_bjet", &is_calo_bjet);
    tree_->Branch("is_pf_jet", &is_pf_jet);
    tree_->Branch("is_pf_bjet", &is_pf_bjet);
    tree_->Branch("is_pf_ht", &is_pf_ht);
    tree_->Branch("is_calo_ht", &is_calo_ht);
    tree_->Branch("is_l1_ht", &is_l1_ht);
    tree_->Branch("is_l1_jet", &is_l1_jet);
    tree_->Branch("is_hltAK4PFJetsLooseID", &is_hltAK4PFJetsLooseID);
    tree_->Branch("is_hltPFCentralJetLooseIDQuad30", &is_hltPFCentralJetLooseIDQuad30);

    //-----------------------
    //b quarks vectors
    //-----------------------

    tree_->Branch("b_pt", &b_pt);
    tree_->Branch("b_eta", &b_eta);
    tree_->Branch("b_phi", &b_phi);
    tree_->Branch("b_et", &b_et);
    tree_->Branch("b_e", &b_e);


    //------------------------
    //Higgs GenLevel
    //------------------------

    //added from HHReweight5D
    tree_->Branch("gen_H1_m",   &Higgs_part.at(0));
    tree_->Branch("gen_H1_pt",  &Higgs_part.at(1));
    tree_->Branch("gen_H1_eta", &Higgs_part.at(2));
    tree_->Branch("gen_H1_phi", &Higgs_part.at(3));
    tree_->Branch("gen_H1_p4",  &Higgs1_p4);

    tree_->Branch("gen_H2_m",   &Higgs_part.at(4));
    tree_->Branch("gen_H2_pt",  &Higgs_part.at(5));
    tree_->Branch("gen_H2_eta", &Higgs_part.at(6));
    tree_->Branch("gen_H2_phi", &Higgs_part.at(7));
    tree_->Branch("gen_H2_p4",  &Higgs2_p4);

    tree_->Branch("gen_mHH",         &gen_mHH);
    tree_->Branch("gen_costh_H1_cm", &gen_costh_H1_cm);
    tree_->Branch("gen_costh_H2_cm", &gen_costh_H2_cm);


}

void TO_Tree_nob::reset()
{
    for (size_t i = 0; i < trg_pass_.size(); ++i)
        trg_pass_.at(i) = 0;

    for (size_t i = 0; i < filter_count_.size(); ++i)
        filter_count_.at(i) = 0;

    trig_obj_pt->clear();
    trig_obj_et->clear();
    trig_obj_mass->clear();
    trig_obj_eta->clear();
    trig_obj_phi->clear();
    matched_jet_pt->clear();
    matched_jet_mass->clear();
    matched_jet_eta->clear();
    matched_jet_phi->clear();
    matched_jet_energy->clear();
    matched_jet_b_tag->clear();
    b_pt->clear();
    b_eta->clear();
    b_phi->clear();
    b_et->clear();
    b_e->clear();
    is_jet->clear();
    is_bjet->clear();
    is_calo_jet->clear();
    is_calo_bjet->clear();
    is_pf_jet->clear();
    is_pf_bjet->clear();
    is_pf_ht->clear();
    is_calo_ht->clear();
    is_l1_ht->clear();
    is_l1_jet->clear();
    is_hltAK4PFJetsLooseID->clear();
    is_hltPFCentralJetLooseIDQuad30->clear();
    reco_jet_pt->clear();
    reco_jet_mass->clear();
    reco_jet_eta->clear();
    reco_jet_phi->clear();
    reco_jet_energy->clear();
    reco_jet_et->clear();
    reco_jet_b_tag->clear();
    //chargedHadronEnergyFraction->clear();
    //neutralHadronEnergyFraction->clear();
    //chargedEmEnergyFraction->clear();
    //neutralEmEnergyFraction->clear();
    //chargedMultiplicity->clear();
    //numberOfDaughters->clear();

    count = 0;//for Higgs particles
    
}


void TO_Tree_nob::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

    tot_evts_++;


    reset();

    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjectsH;
    iEvent.getByToken(triggerObjectsToken_, triggerObjectsH);  
    const pat::TriggerObjectStandAloneCollection& triggerObjects = (*triggerObjectsH.product());

    edm::Handle<GenParticleCollection> genParticleH;
    iEvent.getByToken(gensToken_, genParticleH);  
    const reco::GenParticleCollection& genParticle = (*genParticleH.product());

    edm::Handle<std::vector<pat::Jet>> jetH;
    iEvent.getByToken(jetsToken_ , jetH);
    const std::vector<pat::Jet>  jet = (*jetH.product());  


    edm::Handle<edm::TriggerResults> triggerBitsH;
    iEvent.getByToken(triggerBitsToken_, triggerBitsH);
    const edm::TriggerNames &trg_names = iEvent.triggerNames(*triggerBitsH);


    //check for triggers for given year
    for (size_t itrg = 0; itrg < trgs_of_interest_names_.size(); ++itrg)
    {
        string name = trgs_of_interest_names_.at(itrg);
        int hlt_idx = trgs_of_interest_[name];
        bool pass = false;
        if (hlt_idx >= 0)
            pass = triggerBitsH->accept(hlt_idx);

        // update counts
        if (pass){
            trgs_of_interest_counts_.at(itrg) += 1;
            trg_pass_.at(itrg) = 1; // flag this event as passed
        }
    }


    for (size_t idxto = 0; idxto < triggerObjects.size(); ++idxto)
    {
        pat::TriggerObjectStandAlone obj = triggerObjects.at(idxto);
        obj.unpackPathNames(trg_names);
        obj.unpackFilterLabels(iEvent, *triggerBitsH);

        trig_obj_pt->push_back(obj.pt());
        trig_obj_et->push_back(obj.pt());
        trig_obj_mass->push_back(obj.mass());
        trig_obj_eta->push_back(obj.eta());
        trig_obj_phi->push_back(obj.phi());
        std::vector<int> types = obj.triggerObjectTypes();

        if(obj.hasFilterLabel(string_hltAK4PFJetsLooseID)){
            is_hltAK4PFJetsLooseID->push_back(1);
        }
        else{
            is_hltAK4PFJetsLooseID->push_back(0);
        }
         
        
        if(obj.hasFilterLabel(string_hltPFCentralJetLooseIDQuad30)){
            is_hltPFCentralJetLooseIDQuad30->push_back(1);
        }
        else{
            is_hltPFCentralJetLooseIDQuad30->push_back(0);
        }

        if(std::find(types.begin(), types.end(), 85) != types.end()){
            is_jet->push_back(1);
        }
        else{
            is_jet->push_back(0);
        }

        if(obj.hasFilterLabel(calojet_fil_2018))
            is_calo_jet->push_back(1);
        else
            is_calo_jet->push_back(0);


        if(obj.hasFilterLabel(pfjet_fil_2018)){
            is_pf_jet->push_back(1);
            //chargedHadronEnergyFraction->push_back(obj.chargedHadronEnergyFraction());
            //neutralHadronEnergyFraction->push_back(obj.neutralHadronEnergyFraction());
            //chargedEmEnergyFraction->push_back(obj.chargedEmEnergyFraction());
            //neutralEmEnergyFraction->push_back(obj.neutralEmEnergyFraction());
            //chargedMultiplicity->push_back(obj.chargedMultiplicity());
            //numberOfDaughters->push_back(obj.numberOfDaughters());

        }
        else{
            is_pf_jet->push_back(0);
            //chargedHadronEnergyFraction->push_back(0);
            //neutralHadronEnergyFraction->push_back(0);
            //chargedEmEnergyFraction->push_back(0);
            //neutralEmEnergyFraction->push_back(0);
            //chargedMultiplicity->push_back(0);
            //numberOfDaughters->push_back(0);
        }
        


        if(std::find(types.begin(), types.end(), 86) != types.end()){
            is_bjet->push_back(1);
        }
        else{
            is_bjet->push_back(0);
        }


        if(obj.hasFilterLabel(calojet_fil_2018)){
            is_calo_bjet->push_back(1);
        }
        else{
            is_calo_bjet->push_back(0);
        }

            
        if(obj.hasFilterLabel(pfjet_fil_2018)){
            is_pf_bjet->push_back(1);
        }
        else{
            is_pf_bjet->push_back(0);
        }


        if(obj.hasFilterLabel(pf_ht_label)){
            if(std::find(types.begin(), types.end(), +89) != types.end()){
                is_pf_ht->push_back(1);
            }
            else{
                is_pf_ht->push_back(0);
            }
        }
        else{
            is_pf_ht->push_back(0);
        }

        if(obj.hasFilterLabel(calo_ht_label)){
            if(std::find(types.begin(), types.end(), +89) != types.end()){
                is_calo_ht->push_back(1);
            }
            else{
                is_calo_ht->push_back(0);
            }
        }
        else{
            is_calo_ht->push_back(0);
        }

        //if(obj.hasFilterLabel(l1_objects_2018)){
            if(std::find(types.begin(), types.end(), -89) != types.end()){
                    is_l1_ht->push_back(1);
            }
            else{
                    is_l1_ht->push_back(0);
            }

            if(std::find(types.begin(), types.end(), -99) != types.end()){
                    is_l1_jet->push_back(1);
            }
            else{
                    is_l1_jet->push_back(0);
            }
        //}
        //else{
            //is_l1_ht->push_back(0);
            //is_l1_jet->push_back(0);
        //}

        for (size_t ifilter = 0; ifilter < filters_of_interest_names_.size(); ++ifilter)
        {   

            string fname = filters_of_interest_names_.at(ifilter);

            if (obj.hasFilterLabel(fname.c_str())){
                filter_count_.at(ifilter) += 1;

            }
        }
        
    }

    int count = 0;
    for(size_t idpart = 0; idpart < genParticle.size(); idpart++){

        reco::GenParticle particle = genParticle.at(idpart);
        

        if(particle.pdgId() == 25){
            if(particle.status() == 22){
                count++;
                if(count == 1){
                    Higgs_part.at(0) = particle.mass();
                    Higgs_part.at(1) = particle.pt();
                    Higgs_part.at(2) = particle.eta();
                    Higgs_part.at(3) = particle.phi();

                    vH1.SetPx(particle.px());
                    vH1.SetPy(particle.py());
                    vH1.SetPz(particle.pz());
                    vH1.SetE(particle.energy());
                    Higgs1_p4 = vH1;
                }
                else if (count == 2){
                    Higgs_part.at(4) = particle.mass();
                    Higgs_part.at(5) = particle.pt();
                    Higgs_part.at(6) = particle.eta();
                    Higgs_part.at(7) = particle.phi();

                    vH1.SetPx(particle.px());
                    vH1.SetPy(particle.py());
                    vH1.SetPz(particle.pz());
                    vH1.SetE(particle.energy());
                    Higgs2_p4 = vH1;
                }
                
            }
        }
        
    }

    //Computing Higgs quan tities

    TLorentzVector vSum =   Higgs1_p4 + Higgs2_p4;
    
    // boost to CM copy vectors otherwise we won't load them in tree 

    TLorentzVector vH1_cm = Higgs1_p4;
    TLorentzVector vH2_cm = Higgs2_p4;
    vH1_cm.Boost(-vSum.BoostVector());
    vH2_cm.Boost(-vSum.BoostVector());

    gen_mHH          = vSum.M();
    gen_costh_H1_cm  = vH1_cm.CosTheta();
    gen_costh_H2_cm  = vH2_cm.CosTheta();


    //std::cout << trig_pf_obj_btag->size() << " " << trig_calo_obj_btag->size() << " " << trig_obj_pt->size() << " " << is_jet->size() << " " <<  is_bjet->size() << " " << is_pf_jet->size() << " " << is_pf_bjet->size() << " "<< is_calo_jet->size() << " " << is_calo_bjet->size() << " " << is_ht->size() << std::endl;
    
    std::vector<reco::GenParticle> b_quarks;
    
    for(size_t idpart = 0; idpart < genParticle.size(); idpart++){

        reco::GenParticle particle = genParticle.at(idpart);

        //check for b particles
        if(particle.pdgId() == 5 || particle.pdgId() == -5){
            //check if b is first in chain
            if(particle.statusFlags().isFirstCopy()){
                //check if b has a higgs mother
                if(particle.mother()->pdgId() == 25){
                    //then it is the b we want 
                    b_pt->push_back(particle.pt());
                    b_eta->push_back(particle.eta());
                    b_phi->push_back(particle.phi());
                    b_et->push_back(particle.et());
                    b_e->push_back(particle.energy());
                    b_quarks.push_back(particle);
                    
                }
            }
        }
    }

    
    if(jet.size() < 4){
       return;
    }

    for(size_t recoid = 0; recoid < jet.size() ; recoid++){
        reco_jet_pt->push_back(jet.at(recoid).pt());
        reco_jet_mass->push_back(jet.at(recoid).mass());
        reco_jet_eta->push_back(jet.at(recoid).eta());
        reco_jet_phi->push_back(jet.at(recoid).phi());
        reco_jet_energy->push_back(jet.at(recoid).energy());
        reco_jet_et->push_back(jet.at(recoid).et());
        reco_jet_b_tag->push_back(jet.at(recoid).bDiscriminator("pfDeepFlavourJetTags:probb") + jet.at(recoid).bDiscriminator("pfDeepFlavourJetTags:probbb") + jet.at(recoid).bDiscriminator("pfDeepFlavourJetTags:problepb"));
    }

    std::vector<std::pair<pat::Jet, int> >  match_jets = Jet_Match::b_jet_match(b_quarks, jet);
    for(uint id = 0; id < match_jets.size(); id++){

        if(match_jets.at(id).second != -999){
            matched_jet_pt->push_back(match_jets.at(id).first.pt());
            matched_jet_mass->push_back(match_jets.at(id).first.mass());
            matched_jet_eta->push_back(match_jets.at(id).first.eta());
            matched_jet_phi->push_back(match_jets.at(id).first.phi());
            matched_jet_energy->push_back(match_jets.at(id).first.energy());
            //b_tag->push_back(match_jets.at(id).first.bDiscriminator("newpfDeepFlavourJetTags:probb") + match_jets.at(id).first.bDiscriminator("newpfDeepFlavourJetTags:probbb") + match_jets.at(id).first.bDiscriminator("newpfDeepFlavourJetTags:problepb")); //2016 corr
            matched_jet_b_tag->push_back(match_jets.at(id).first.bDiscriminator("pfDeepFlavourJetTags:probb") + match_jets.at(id).first.bDiscriminator("pfDeepFlavourJetTags:probbb") + match_jets.at(id).first.bDiscriminator("pfDeepFlavourJetTags:problepb")); //2018 no need for corr

        }

        else{
            matched_jet_pt->push_back(-999);
            matched_jet_mass->push_back(-999);
            matched_jet_eta->push_back(-999);
            matched_jet_phi->push_back(-999);
            matched_jet_energy->push_back(-999);
            matched_jet_b_tag->push_back(-999);
        }

    }


    if(*std::min_element(matched_jet_pt->begin(), matched_jet_pt->end()) >= 25 && *std::min_element(matched_jet_eta->begin(), matched_jet_eta->end()) >= -2.4 && *std::max_element(matched_jet_eta->begin(), matched_jet_eta->end()) <= 2.4){
        cut_ = true;
    }
    else{
        cut_ = false;
    }
    
    tree_->Fill();
}

void TO_Tree_nob::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){

    // L1 Menù printing
    edm::ESHandle<L1GtTriggerMenu> menuRcd;
    iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
    const L1GtTriggerMenu* menu = menuRcd.product();

    for (CItAlgo algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) {
        edm::LogVerbatim("L1Menu") << "Name: " << (algo->second).algoName() << " Alias: " << (algo->second).algoAlias() << std::endl;
    }
    //

    bool changedConfig = false;
 
    if(!hltConfig_.init(iRun, iSetup, trgprocessName_.process(), changedConfig)){
        edm::LogError("HLTMatchingFilter") << "Initialization of HLTConfigProvider failed!!"; 
        return;
    }
    else
    {
        // clear the trgs_of_interest_ map
        for (auto it = trgs_of_interest_.begin(); it != trgs_of_interest_.end(); it++ ){
            it->second = -1;
        }

        if (verbose_){
            cout << "=== Marked a change in the HLT menu . Run " << iRun.run() << endl;
            cout << "== Here are all the triggers in this run" << endl;
        }
        for(size_t j = 0; j < hltConfig_.triggerNames().size(); j++)
        {
            string pathName = hltConfig_.triggerNames()[j];
            if (verbose_) cout << j << " .... : " << pathName << endl;

            // does this trigger match the desired name?
            for (auto tname : trgs_of_interest_names_){
                if (pathName.find(tname) != std::string::npos)
                {
                    int old_idx = trgs_of_interest_.at(tname);
                    if (old_idx >= 0) // shouldn't be a valid one
                    {
                        edm::LogError("HH_Reweight") << "Double match for desired trg name found " << old_idx; 
                    }
                    trgs_of_interest_.at(tname) = j;
                    if (verbose_) cout << "  ^^^ flagged as interesting " << endl;
                }
            }
        } 
    }
}

void TO_Tree_nob::endJob()
{
    cout << "... TOTAL processed : " << tot_evts_ << endl;
    for (size_t itrg = 0; itrg < trgs_of_interest_names_.size(); ++itrg)
    {
        auto name   = trgs_of_interest_names_.at(itrg);
        auto counts = trgs_of_interest_counts_.at(itrg);
        cout << "..... " << name << "   : " << counts << endl;
    }
}


#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(TO_Tree_nob);

#endif
