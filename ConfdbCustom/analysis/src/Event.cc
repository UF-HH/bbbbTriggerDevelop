#include "../interface/Event.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include <iostream>

Event::Event(TFile* f, std::string b){

    event_index = 0;
    infile = f;
    branch = b;
    Init();
};

void Event::Init(){

    std::cout << "[INFO]: Reading File" << std::endl;
    tree = (TTree*)infile->Get(branch.c_str());
    
    tree->SetBranchAddress("l1_pt", &l1_pt);
    tree->SetBranchAddress("l1_eta", &l1_eta);
    tree->SetBranchAddress("l1_phi", &l1_phi);
    tree->SetBranchAddress("l1_e", &l1_e);
    tree->SetBranchAddress("l1_mass", &l1_mass);
    
    tree->SetBranchAddress("calo_pt", &calo_pt);
    tree->SetBranchAddress("calo_eta", &calo_eta);
    tree->SetBranchAddress("calo_phi", &calo_phi);
    tree->SetBranchAddress("calo_e", &calo_e);
    tree->SetBranchAddress("calo_mass", &calo_mass);
    tree->SetBranchAddress("calo_btag", &calo_btag);

    tree->SetBranchAddress("pf_pt", &pf_pt);
    tree->SetBranchAddress("pf_eta", &pf_eta);
    tree->SetBranchAddress("pf_phi", &pf_phi);
    tree->SetBranchAddress("pf_e", &pf_e);
    tree->SetBranchAddress("pf_mass", &pf_mass);
    tree->SetBranchAddress("pf_btag", &pf_btag);

    //tree->SetBranchAddress("event", &event_);
    //tree->SetBranchAddress("run", &run_);
    //tree->SetBranchAddress("lumisection", &lumi_);

}

void Event::clear(){

    RecoJets.clear();
    L1Jets.clear();
    CaloJets.clear();
    PFJets.clear();
    CaloBJets.clear();
    PFBJets.clear();
    L1J.clear();
    CaloJ.clear();
    PFJ.clear();
    CaloBJ.clear();
    PFBJ.clear();
    Matches.clear();
    RecoJ.clear();

    return; 
}
void Event::Generate(){

    clear();

    if(event_index%10000 == 0){
        std::cout << event_index << std::endl;
    }

    tree->GetEntry(event_index);

    L1Jets.pt = *l1_pt;
    L1Jets.eta = *l1_eta;
    L1Jets.phi = *l1_phi;
    L1Jets.e = *l1_e;

    CaloJets.pt = *calo_pt;
    CaloJets.eta = *calo_eta;
    CaloJets.phi = *calo_phi;
    CaloJets.e = *calo_e;
    CaloJets.btag = *calo_btag;

    PFJets.pt = *pf_pt;
    PFJets.eta = *pf_eta;
    PFJets.phi = *pf_phi;
    PFJets.e = *pf_e;
    PFJets.btag = *pf_btag;
    
    /*
    RecoJets.pt = *reco_jet_pt;
    RecoJets.mass = *reco_jet_mass;
    RecoJets.eta = *reco_jet_eta;
    RecoJets.phi = *reco_jet_phi;
    RecoJets.e = *reco_jet_energy;
    RecoJets.et = *reco_jet_et;
    RecoJets.btag = *reco_jet_b_tag;
    */
    
    //building the bjets
    for(uint i = 0; i < CaloJets.pt.size(); i++){
        if(CaloJets.btag.at(i) != 0){
            CaloBJets.pt.push_back(CaloJets.pt.at(i));
            CaloBJets.eta.push_back(CaloJets.eta.at(i));
            CaloBJets.phi.push_back(CaloJets.phi.at(i));
            CaloBJets.e.push_back(CaloJets.e.at(i));
            CaloBJets.btag.push_back(CaloJets.btag.at(i));
        }
    }

    for(uint i = 0; i < PFJets.pt.size(); i++){
        if(PFJets.btag.at(i) != 0){
            PFBJets.pt.push_back(PFJets.pt.at(i));
            PFBJets.eta.push_back(PFJets.eta.at(i));
            PFBJets.phi.push_back(PFJets.phi.at(i));
            PFBJets.e.push_back(PFJets.e.at(i));
            PFBJets.btag.push_back(PFJets.btag.at(i));
        }
    }


    RecoJets.Type = "RecoJets"; //empty atm
    L1Jets.Type = "L1Jets"; 
    CaloJets.Type = "CaloJets";
    PFJets.Type = "PFJets";
    CaloBJets.Type = "CaloBJets";
    PFBJets.Type = "PFBJets";

    //creating local map to access reference by string contained in the kinnCut object
    std::map<std::string, hltObj::Jets* > StringToObj{

        std::make_pair("RecoJets", &RecoJets),
        std::make_pair("L1Jets", &L1Jets),
        std::make_pair("CaloJets", &CaloJets),  
        std::make_pair("PFJets", &PFJets),
        std::make_pair("CaloBJets", &CaloBJets),
        std::make_pair("PFBJets", &PFBJets),

    };

    //cuts on objects
    for(int i = 0; i < Cut_.size(); i++){
        if(StringToObj.count(Cut_.at(i).Type)){
            std::vector<int> index_to_delete;
            for(int j = 0; j < StringToObj[Cut_.at(i).Type]->size(); j++){
                if(StringToObj[Cut_.at(i).Type]->pt.at(j) < Cut_.at(i).MinPt || StringToObj[Cut_.at(i).Type]->pt.at(j) > Cut_.at(i).MaxPt || StringToObj[Cut_.at(i).Type]->et.at(j) < Cut_.at(i).MinEt ||
                    StringToObj[Cut_.at(i).Type]->eta.at(j) < Cut_.at(i).MinEta || StringToObj[Cut_.at(i).Type]->eta.at(j) > Cut_.at(i).MaxEta){
                    
                    StringToObj[Cut_.at(i).Type]->clear(j);
                    j--;
                }
            }
        }
    }

    event_index++;

    return; 

}


void Event::UnpackCollections(){

    
    for(int i = 0; i < L1Jets.size(); i++){
        if(L1Jets.pt.at(i) != 0){
            L1J.push_back(new hltObj::Jet(L1Jets.pt.at(i), L1Jets.eta.at(i), L1Jets.phi.at(i)));
        }
    }
    
    for(int i = 0; i < CaloJets.size(); i++){
        if(CaloJets.pt.at(i) != 0){
            CaloJ.push_back(new hltObj::Jet(CaloJets.pt.at(i), CaloJets.eta.at(i), CaloJets.phi.at(i)));
        }
    }


    for(int i = 0; i < PFJets.size(); i++){
        if(PFJets.pt.at(i) != 0){
            PFJ.push_back(new hltObj::Jet(PFJets.pt.at(i), PFJets.eta.at(i), PFJets.phi.at(i)));
        }
    }

    for(int i = 0; i < CaloBJets.size(); i++){
        if(CaloBJets.pt.at(i) != 0){
            CaloBJ.push_back(new hltObj::Jet(CaloBJets.pt.at(i), CaloBJets.eta.at(i), CaloBJets.phi.at(i)));
        }
    }

    for(int i = 0; i < PFBJets.size(); i++){
        if(PFBJets.pt.at(i) != 0){
            PFBJ.push_back(new hltObj::Jet(PFBJets.pt.at(i), PFBJets.eta.at(i), PFBJets.phi.at(i)));
        }
    }

    //this two objects should always exist
    for(int i = 0; i < RecoJets.size(); i++){
        RecoJ.push_back(new hltObj::Jet(RecoJets.pt.at(i), RecoJets.eta.at(i), RecoJets.phi.at(i)));
    }

    return;

}


void Event::jetMatch(double R, std::string Reference, std::string SelectedJets){

    Matches.clear(); //we have to clear vector in order to make new matches

    std::map<std::string, std::vector<hltObj::Jet*> > StringToObj{

        std::make_pair("RecoJets", RecoJ),
        std::make_pair("L1Jets", L1J),
        std::make_pair("CaloJets", CaloJ),  
        std::make_pair("PFJets", PFJ),
        std::make_pair("CaloBJets", CaloBJ),
        std::make_pair("PFBJets", PFBJ),

    };

    //clearing matches before continue
    for(auto j : StringToObj[SelectedJets]){
        j->matched = false;
    }
    

    //creating copy. If a jet is matched we delete it from the 
    //local list.
    std::vector<hltObj::Jet*> MatchedJets_copy = StringToObj[Reference];

    while(MatchedJets_copy.size()!=0 && StringToObj[SelectedJets].size() != 0){
        std::vector<double> dr;
        std::vector<int> index_trg;
        std::vector<int> index_mj;
        for(int i = 0; i < MatchedJets_copy.size(); i++){
            for(int j = 0; j < StringToObj[SelectedJets].size(); j++){
                if(!StringToObj[SelectedJets].at(j)->matched){
                    dr.push_back(sqrt(pow(MatchedJets_copy.at(i)->eta-StringToObj[SelectedJets].at(j)->eta, 2) + 
                                        pow(MatchedJets_copy.at(i)->phi-StringToObj[SelectedJets].at(j)->phi, 2)));
                    index_trg.push_back(j);
                    index_mj.push_back(i);
                }

            }
        }

        int min_index = std::min_element(dr.begin(),dr.end()) - dr.begin();

        //due to the fact that we have if(!StringToObj->matched)  we have to check dr.size()
        if(dr.size() > 0){
            if(dr.at(min_index) <= R){
                hltObj::Jet mj_match_ref(MatchedJets_copy.at(index_mj.at(min_index))->pt, 
                                            MatchedJets_copy.at(index_mj.at(min_index))->eta,
                                                MatchedJets_copy.at(index_mj.at(min_index))->phi);
            
                StringToObj[SelectedJets].at(index_trg.at(min_index))->matched = true;
                Matches.push_back(mj_match_ref);
                StringToObj[SelectedJets].at(index_trg.at(min_index))->MatchedObj = &Matches.at(Matches.size()-1);
                
            }
            MatchedJets_copy.erase(MatchedJets_copy.begin()+index_mj.at(min_index));

        }
        else{
            MatchedJets_copy.erase(MatchedJets_copy.begin()+0);
        }

    }    

    return;
}


double Event::GetCaloHT(){
    double HT = 0;
    for(auto jet : CaloJ){
        HT += jet->pt;
    }

    return HT;
}

double Event::GetPFHT(){
    double HT = 0;
    for(auto jet : PFJ){
        HT += jet->pt;
    }

    return HT;
}

double Event::GetL1HT(){
    double HT = 0;
    for(auto jet : L1J){
        HT += jet->pt;
    }

    return HT;
}

double Event::GetHT(hltObj::KinCuts cut){
    double HT = 0;

    //creating local map to access reference by string contained in the kinnCut object
    std::map<std::string, hltObj::Jets* > StringToObj{

        std::make_pair("RecoJets", &RecoJets),
        std::make_pair("L1Jets", &L1Jets),
        std::make_pair("CaloJets", &CaloJets),  
        std::make_pair("PFJets", &PFJets),
        std::make_pair("CaloBJets", &CaloBJets),
        std::make_pair("PFBJets", &PFBJets),

    };

    for(int j = 0; j < StringToObj[cut.Type]->size(); j++){
        if(StringToObj[cut.Type]->pt.at(j) > cut.MinPt || StringToObj[cut.Type]->pt.at(j) < cut.MaxPt || StringToObj[cut.Type]->et.at(j) > cut.MinEt ||
            StringToObj[cut.Type]->eta.at(j) > cut.MinEta || StringToObj[cut.Type]->eta.at(j) < cut.MaxEta){
            
            HT += StringToObj[cut.Type]->pt.at(j);
        }
    }


    return HT;
}





