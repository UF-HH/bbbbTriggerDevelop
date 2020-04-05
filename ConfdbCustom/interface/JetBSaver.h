#ifndef JET_B_SAVER_H
#define JET_B_SAVER_H


#include <vector>
#include <string>
#include <iostream>
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TTree.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/BTauReco/interface/JetTag.h"


template <typename T>

class JetBSaver : public edm::EDAnalyzer {
    public:
        explicit JetBSaver(const edm::ParameterSet& iConfig);
        virtual ~JetBSaver(){};

    private:
        //----edm control---
        virtual void beginJob();
        virtual void beginRun(edm::Run const&, edm::EventSetup const&);
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob();
        virtual void endRun(edm::Run const&, edm::EventSetup const&);
        virtual void clear();

        //Important objects:
    
        const edm::InputTag JetsBTag_; 
        const edm::EDGetTokenT<reco::JetTagCollection> JetsBToken_;


        //booleans
        bool verbose_;

        //Tree
        std::string t;
        std::string branchName;
        TTree* tree_;

        //Kinematics
        std::vector<double>* btag_ = new std::vector<double>;
        std::vector<double>* pt_ = new std::vector<double>;
        std::vector<double>* et_ = new std::vector<double>;
        std::vector<double>* eta_ = new std::vector<double>;
        std::vector<double>* phi_ = new std::vector<double>;
        std::vector<double>* e_ = new std::vector<double>;
        std::vector<double>* mass_ = new std::vector<double>;

};

#endif //JET_SAVER_H