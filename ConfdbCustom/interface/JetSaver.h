#ifndef JET_SAVER_H
#define JET_SAVER_H


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


template <typename T>

class JetSaver : public edm::EDAnalyzer {
    public:
        explicit JetSaver(const edm::ParameterSet& iConfig);
        virtual ~JetSaver(){};

    private:
        //----edm control---
        virtual void beginJob();
        virtual void beginRun(edm::Run const&, edm::EventSetup const&);
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob();
        virtual void endRun(edm::Run const&, edm::EventSetup const&);
        virtual void clear();

        //Important objects:
        const edm::InputTag JetsTag_; 
        const edm::EDGetTokenT<std::vector<T>> JetsToken_;


        //Verbose
        bool verbose_;

        //Tree
        std::string t;
        std::string branchName;
        TTree* tree_;

        //Kinematics
        std::vector<double>* pt_ = new std::vector<double>;
        std::vector<double>* et_ = new std::vector<double>;
        std::vector<double>* eta_ = new std::vector<double>;
        std::vector<double>* phi_ = new std::vector<double>;
        std::vector<double>* e_ = new std::vector<double>;
        std::vector<double>* mass_ = new std::vector<double>;

        //optional
        std::vector<double> btag_;
};

#endif //JET_SAVER_H