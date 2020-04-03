#ifndef HLT_AN_CC
#define HLT_AN_CC

#include "FWCore/Framework/interface/EDAnalyzer.h"
//#include "FWCore/Framework/interface/one/EDAnalyzer.h"
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

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "TTree.h"

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <boost/bind.hpp>

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"


using namespace std;

class HLTAn : public edm::EDAnalyzer {
    public:
        explicit HLTAn(const edm::ParameterSet&);
        virtual ~HLTAn(){};

    private:
        //----edm control---
        virtual void beginJob();
        virtual void beginRun(edm::Run const&, edm::EventSetup const&);
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob();
        virtual void endRun(edm::Run const&, edm::EventSetup const&){};

        void reset();

        static std::vector<string> remove_duplicates(std::vector<string> vIn);
        bool jsonContainsEvent (const edm::Event& iEvent);

        bool verbose_;
                
        const edm::InputTag hlTriggerResults_;  // Input tag for TriggerResults
        const edm::EDGetTokenT<edm::TriggerResults> triggerBitsToken_;
        std::vector<edm::LuminosityBlockRange> lumisTemp ;

        HLTConfigProvider hltConfig_;
        edm::InputTag trgprocessName_;

        std::unordered_map <string, int> trgs_of_interest_; // maps the trg name to the idx of the HLT vector
        std::vector<string>    trgs_of_interest_names_; // the keys of the above
        std::vector<long long> trgs_of_interest_counts_; // the counts of the above
        long long tot_evts_;

        std::vector<string> filters_of_interest_names_;

        // and the associated filters - counts stored in a TTree
        TTree *tree_;

        // branches to save in the output
        std::vector<int> trg_pass_; // one int (0/1) for every trigger of interest
        std::vector<int> filter_count_; // one int to count num objects that pass a certain filer

        unsigned long long int event_;
        int       run_;
        int       lumi_;

};

HLTAn::HLTAn(const edm::ParameterSet& iConfig):
    hlTriggerResults_(iConfig.getParameter<edm::InputTag>("triggerResults")),
    triggerBitsToken_ (consumes<edm::TriggerResults>(hlTriggerResults_))
{
    trgprocessName_            = iConfig.getParameter<edm::InputTag>("triggerResults");
    trgs_of_interest_names_    = iConfig.getParameter<std::vector<string>> ("triggerList");
    filters_of_interest_names_ = iConfig.getParameter<std::vector<string>> ("filterList");

    //LOADING GOOD RUNS FROM GOLDEN JSON
    edm::ParameterSet const& inputs = iConfig.getParameter<edm::ParameterSet>("inputs");
    if ( inputs.exists("lumisToProcess") ) 
    {
        lumisTemp = inputs.getUntrackedParameter<std::vector<edm::LuminosityBlockRange> > ("lumisToProcess");
        if(verbose_) std::cout << ".... Loaded Json File" << std::endl; 
    }

    // remove possible duplicates
    trgs_of_interest_names_    = remove_duplicates(trgs_of_interest_names_);

    verbose_ = iConfig.getParameter<bool>("verbose");

    // loop through all the names
    for (auto s : trgs_of_interest_names_)
    {
        trgs_of_interest_[s] = -1; // invalid ref
        trgs_of_interest_counts_.push_back(0); // 0 counts at startup
    }

    tot_evts_ = 0;
}

bool HLTAn::jsonContainsEvent (const edm::Event& iEvent)
{
   // if the jsonVec is empty, then no JSON file was provided so all
   // events should pass
   if (lumisTemp.empty())
   {
      return true;
   }
   bool (* funcPtr) (edm::LuminosityBlockRange const &,
                     edm::LuminosityBlockID const &) = &edm::contains;
   edm::LuminosityBlockID lumiID (iEvent.id().run(), 
                                  iEvent.id().luminosityBlock());
   std::vector< edm::LuminosityBlockRange >::const_iterator iter = 
      std::find_if (lumisTemp.begin(), lumisTemp.end(),
                    boost::bind(funcPtr, _1, lumiID) );
   return lumisTemp.end() != iter;

}

std::vector<string> HLTAn::remove_duplicates(std::vector<string> vIn)
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

void HLTAn::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){

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
                        edm::LogError("trgFilterPrinter") << "Double match for desired trg name found " << old_idx; 
                    }
                    trgs_of_interest_.at(tname) = j;
                    if (verbose_) cout << "  ^^^ flagged as interesting " << endl;
                }
            }
        } 
    }
}

void HLTAn::beginJob()
{
    edm::Service<TFileService> fs;
    tree_ = fs -> make<TTree>("trgObjTree", "trgObjTree");

    tree_->Branch("event", &event_);
    tree_->Branch("run",   &run_);
    tree_->Branch("lumi",  &lumi_);


    trg_pass_.resize(trgs_of_interest_names_.size()); // one counter per trigger

    // set the branches in the tree
    for (size_t it = 0; it < trgs_of_interest_names_.size(); ++it)
    {
        string bname = trgs_of_interest_names_.at(it);
        cout << ".... new branch (TRG) : " << bname << endl;
        tree_->Branch(bname.c_str(), &(trg_pass_.at(it)));
    }


}

void HLTAn::reset()
{
    for (size_t i = 0; i < trg_pass_.size(); ++i)
        trg_pass_.at(i) = 0;
    

}

void HLTAn::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    reset(); // all counters of the output tree set to 0

    event_ = iEvent.id().event();
    run_   = iEvent.id().run();
    lumi_  = iEvent.luminosityBlock();

    //If this configuration is not present in the json then the event is skipped
    //Note than if no json is provided this function always returns true
    if(!jsonContainsEvent(iEvent)){
        if(verbose_) std::cout << "Skipping Event: " << event_ << " Run: " << run_ << " Lumi: " << lumi_ << std::endl;
        return;
    }


    if(verbose_) std::cout << "Run: " << run_ << " Lumisection: " << lumi_  << " Event: " << event_ << std::endl;

    tot_evts_ += 1;

    edm::Handle<edm::TriggerResults> triggerBitsH;
    iEvent.getByToken(triggerBitsToken_, triggerBitsH);

    // std::string my_HLT = "HLT_PFHT330PT30_QuadPFJet_75_60_45_40_TriplePFBTagDeepCSV_4p5_v";
    // cout << " --- size of triggerBitsH is " << triggerBitsH->size() << endl; // same size as the full list printed below

    // ------------------------------
    // check the trigger pass/fail

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

        if(verbose_){
            std::cout << "Is Valis?: " << triggerBitsH.isValid() << " WasRun?: " 
            << triggerBitsH->wasrun(hlt_idx) <<  " Trigger: " << name << " idx: " 
            << hlt_idx << " Decision: " << pass << std::endl;
        }
    }

    
    tree_->Fill(); // only save info for events that passed a trigger

}

void HLTAn::endJob()
{
    cout << "-------------- here is the count summary ------------- " << endl; 
    cout << "... TOTAL processed : " << tot_evts_ << endl;
    for (size_t itrg = 0; itrg < trgs_of_interest_names_.size(); ++itrg)
    {
        auto name   = trgs_of_interest_names_.at(itrg);
        auto counts = trgs_of_interest_counts_.at(itrg);
        cout << "..... " << name << "   : " << counts << endl;
    }
}


#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(HLTAn);

#endif

