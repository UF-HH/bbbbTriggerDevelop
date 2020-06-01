#include "../interface/Event.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include <iostream>

Event::Event(TFile* f, std::string b){

    event_index = 0;
    infile = f;
    branch = b;
    eventType = "data";
    Init();
};

Event::Event(TFile* f, std::string b, std::string genb){

    event_index = 0;
    infile = f;
    branch = b;
    genBranch = genb;
    eventType = "MC";
    //passed two trees, one for MC other for jets
    Init();
};

void Event::Init(){

    std::cout << "[INFO]: Reading File" << std::endl;
    tree = (TTree*)infile->Get(branch.c_str());
    
    //This will be red by default for data and MC
    tree->SetBranchAddress("l1_pt", &l1_pt);
    tree->SetBranchAddress("l1_eta", &l1_eta);
    tree->SetBranchAddress("l1_phi", &l1_phi);
    tree->SetBranchAddress("l1_e", &l1_e);
    tree->SetBranchAddress("l1_mass", &l1_mass);
    
    tree->SetBranchAddress("calo_pt", &calo_pt);
    tree->SetBranchAddress("calo_eta", &calo_eta);
    tree->SetBranchAddress("calo_phi", &calo_phi);
    tree->SetBranchAddress("calo_e", &calo_e);
    tree->SetBranchAddress("calo_et", &calo_et);
    tree->SetBranchAddress("calo_mass", &calo_mass);
    tree->SetBranchAddress("calo_btag", &calo_btag);

    tree->SetBranchAddress("pf_pt", &pf_pt);
    tree->SetBranchAddress("pf_eta", &pf_eta);
    tree->SetBranchAddress("pf_phi", &pf_phi);
    tree->SetBranchAddress("pf_e", &pf_e);
    tree->SetBranchAddress("pf_et", &pf_et);
    tree->SetBranchAddress("pf_mass", &pf_mass);
    tree->SetBranchAddress("pf_btag", &pf_btag);

    tree->SetBranchAddress("event", &event_);
    tree->SetBranchAddress("run", &run_);
    tree->SetBranchAddress("lumi", &lumi_);

    if ( eventType == "MC"){
        
        //Reading GenJets from SaveAllJets plugin
        tree->SetBranchAddress("gen_pt", &gen_pt);
        tree->SetBranchAddress("gen_eta", &gen_eta);
        tree->SetBranchAddress("gen_phi", &gen_phi);
        tree->SetBranchAddress("gen_e", &gen_e);
        tree->SetBranchAddress("gen_et", &gen_et);
        tree->SetBranchAddress("gen_mass", &gen_mass);

        tree->SetBranchAddress("gen_nonu_pt", &gen_nonu_pt);
        tree->SetBranchAddress("gen_nonu_eta", &gen_nonu_eta);
        tree->SetBranchAddress("gen_nonu_phi", &gen_nonu_phi);
        tree->SetBranchAddress("gen_nonu_e", &gen_nonu_e);
        tree->SetBranchAddress("gen_nonu_et", &gen_nonu_et);
        tree->SetBranchAddress("gen_nonu_mass", &gen_nonu_mass);

        tree->SetBranchAddress("reco_pt", &reco_jet_pt);
        tree->SetBranchAddress("reco_eta", &reco_jet_eta);
        tree->SetBranchAddress("reco_phi", &reco_jet_phi);
        tree->SetBranchAddress("reco_e", &reco_jet_e);
        tree->SetBranchAddress("reco_et", &reco_jet_et);
        tree->SetBranchAddress("reco_mass", &reco_jet_mass);
        tree->SetBranchAddress("reco_btag", &reco_jet_btag);


        //Reading infos from SaveGenHH plugin
        treeGen = (TTree*)infile->Get(genBranch.c_str());

        treeGen->SetBranchAddress("gen_H1_m",   &H1.gen_H_m);
        treeGen->SetBranchAddress("gen_H1_pt",  &H1.gen_H_pt);
        treeGen->SetBranchAddress("gen_H1_eta", &H1.gen_H_eta);
        treeGen->SetBranchAddress("gen_H1_phi", &H1.gen_H_phi);
        treeGen->SetBranchAddress("gen_H1_p4",  &H1.gen_H_p4);

        treeGen->SetBranchAddress("gen_H2_m",   &H2.gen_H_m);
        treeGen->SetBranchAddress("gen_H2_pt",  &H2.gen_H_pt);
        treeGen->SetBranchAddress("gen_H2_eta", &H2.gen_H_eta);
        treeGen->SetBranchAddress("gen_H2_phi", &H2.gen_H_phi);
        treeGen->SetBranchAddress("gen_H2_p4",  &H2.gen_H_p4);

        treeGen->SetBranchAddress("b_pt", &BS.pt);
        treeGen->SetBranchAddress("b_eta", &BS.eta);
        treeGen->SetBranchAddress("b_phi", &BS.phi);

        
    }

}

void Event::clear(){

    RecoJets.clear();
    L1Jets.clear();
    CaloJets.clear();
    PFJets.clear();
    GenJets.clear();
    GenNNJets.clear();
    CaloBJets.clear();
    PFBJets.clear();
    GenJ.clear();
    GenNNJ.clear();
    L1J.clear();
    CaloJ.clear();
    PFJ.clear();
    CaloBJ.clear();
    PFBJ.clear();
    Matches.clear();
    RecoJ.clear();
    RecoJM.clear();
    bs.clear();
    BMatches.clear();

    return; 
}
void Event::Generate(){

    clear();

    if(event_index%1000 == 0){
        std::cout << event_index << std::endl;
    }

    tree->GetEntry(event_index);

    if(eventType == "MC"){
        treeGen->GetEntry(event_index);

        GenJets.pt = *gen_pt;
        GenJets.eta = *gen_eta;
        GenJets.phi = *gen_phi;
        GenJets.e = *gen_e;
        GenJets.et = *gen_et;

        GenNNJets.pt = *gen_nonu_pt;
        GenNNJets.eta = *gen_nonu_eta;
        GenNNJets.phi = *gen_nonu_phi;
        GenNNJets.e = *gen_nonu_e;
        GenNNJets.et = *gen_nonu_et;

        RecoJets.pt = *reco_jet_pt;
        RecoJets.mass = *reco_jet_mass;
        RecoJets.eta = *reco_jet_eta;
        RecoJets.phi = *reco_jet_phi;
        RecoJets.e = *reco_jet_e;
        RecoJets.et = *reco_jet_et;
        RecoJets.btag = *reco_jet_btag;
    }

    L1Jets.pt = *l1_pt;
    L1Jets.eta = *l1_eta;
    L1Jets.phi = *l1_phi;
    L1Jets.e = *l1_e;

    CaloJets.pt = *calo_pt;
    CaloJets.eta = *calo_eta;
    CaloJets.phi = *calo_phi;
    CaloJets.e = *calo_e;
    CaloJets.et = *calo_et;
    CaloJets.btag = *calo_btag;

    PFJets.pt = *pf_pt;
    PFJets.eta = *pf_eta;
    PFJets.phi = *pf_phi;
    PFJets.e = *pf_e;
    PFJets.et = *pf_et;
    PFJets.btag = *pf_btag;
    
    
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
            PFBJets.et.push_back(PFJets.et.at(i));
            PFBJets.btag.push_back(PFJets.btag.at(i));
        }
    }


    RecoJets.Type = "RecoJets"; 
    GenJets.Type = "GenJets"; 
    GenNNJets.Type = "GenNNJets"; 
    L1Jets.Type = "L1Jets"; 
    CaloJets.Type = "CaloJets";
    PFJets.Type = "PFJets";
    CaloBJets.Type = "CaloBJets";
    PFBJets.Type = "PFBJets";

    //creating local map to access reference by string contained in the kinnCut object
    std::map<std::string, hltObj::Jets* > StringToObj{

        std::make_pair("RecoJets", &RecoJets),
        std::make_pair("GenJets", &GenJets),
        std::make_pair("GenNNJets", &GenNNJets),
        std::make_pair("L1Jets", &L1Jets),
        std::make_pair("CaloJets", &CaloJets),  
        std::make_pair("PFJets", &PFJets),
        std::make_pair("CaloBJets", &CaloBJets),
        std::make_pair("PFBJets", &PFBJets),

    };

    //cuts on objects
    for(int i = 0; i < Cut_.size(); i++){
        if(StringToObj.count(Cut_.at(i).Type)){
            for(int j = 0; j < StringToObj[Cut_.at(i).Type]->size(); j++){
                if(StringToObj[Cut_.at(i).Type]->pt.at(j) < Cut_.at(i).MinPt || StringToObj[Cut_.at(i).Type]->e.at(j) < Cut_.at(i).MinE || StringToObj[Cut_.at(i).Type]->pt.at(j) > Cut_.at(i).MaxPt || StringToObj[Cut_.at(i).Type]->et.at(j) < Cut_.at(i).MinEt ||
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

void Event::WeightFactory(std::string kl_map, std::string kl_histo, std::string kl_coeffs){

    if(eventType == "data"){
        std::cerr<< "Cannot compute weight if data" << std::endl;
        throw std::runtime_error(("Event type is " + eventType + " while expected MC").c_str());
    }
    //initializing the Reweight shared point. This can be done only once so 
    //doing an external function which creates the pointer once and for all really 
    //boosts the computation.
    TFile* fIn = TFile::Open(kl_map.c_str());
    TH2D*  hIn = (TH2D*) fIn->Get(kl_histo.c_str());
    hhreweighter = std::shared_ptr<HHReweight5D> (new HHReweight5D(kl_coeffs.c_str(), hIn));
    fIn->Close();

    return;
}

void Event::compute_weight(float kl){

    if(eventType == "data"){
        std::cerr<< "Cannot compute weight if data" << std::endl;
        throw std::runtime_error(("Event type is " + eventType + " while expected MC").c_str());
    }

    //loading coeff for the 5D space and building the hhreweighter
    if(hhreweighter){
        kl_ev = kl; //saving info in event attribute
        double gen_mHH = H1.GetMHH(H2); //retireving MHH from the event.
        weight = hhreweighter->getWeight(kl, 1.0, gen_mHH, H1.gen_costh_H_cm); //computing weight for kl
    }

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
            CaloJ.push_back(new hltObj::Jet(CaloJets.pt.at(i), CaloJets.eta.at(i), CaloJets.phi.at(i), CaloJets.btag.at(i)));
            CaloJ.at(CaloJ.size()-1)->e = CaloJets.e.at(i); //last jet added, add info about energy
        }
    }


    for(int i = 0; i < PFJets.size(); i++){
        if(PFJets.pt.at(i) != 0){
            PFJ.push_back(new hltObj::Jet(PFJets.pt.at(i), PFJets.eta.at(i), PFJets.phi.at(i), PFJets.btag.at(i)));
            PFJ.at(PFJ.size()-1)->e = PFJets.e.at(i); //last jet added, add info about energy
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
        RecoJ.at(RecoJ.size()-1)->e = RecoJets.e.at(i); //last jet added, add info about energy
    }

    if(eventType == "MC"){

        for(int i = 0; i < GenJets.size(); i++){
            if(GenJets.pt.at(i) != 0){
                GenJ.push_back(new hltObj::Jet(GenJets.pt.at(i), GenJets.eta.at(i), GenJets.phi.at(i)));
                GenJ.at(GenJ.size()-1)->e = GenJets.e.at(i); //last jet added, add info about energy
            }
        }

        for(int i = 0; i < GenNNJets.size(); i++){
            if(GenNNJets.pt.at(i) != 0){
                GenNNJ.push_back(new hltObj::Jet(GenNNJets.pt.at(i), GenNNJets.eta.at(i), GenNNJets.phi.at(i)));
                GenNNJ.at(GenNNJ.size()-1)->e = GenNNJets.e.at(i); //last jet added, add info about energy
            }
        }

        for(int i = 0; i < BS.size(); i++){
            bs.push_back(new hltObj::bQuark(BS.pt->at(i), BS.eta->at(i), BS.phi->at(i)));
        }
    }

    return;

}


void Event::jetMatch(double R, std::string Reference, std::string SelectedJets){

    Matches.clear(); //we have to clear vector in order to make new matches

    std::map<std::string, std::vector<hltObj::Jet*> > StringToObj{

        std::make_pair("RecoJets", RecoJ),
        std::make_pair("GenJets", GenJ),
        std::make_pair("GenNNJets", GenNNJ),
        std::make_pair("L1Jets", L1J),
        std::make_pair("CaloJets", CaloJ),  
        std::make_pair("PFJets", PFJ),
        std::make_pair("CaloBJets", CaloBJ),
        std::make_pair("PFBJets", PFBJ),
        std::make_pair("BMatchedJets", RecoJM),

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
                mj_match_ref.e = MatchedJets_copy.at(index_mj.at(min_index))->e; //filling energy
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

//passing vectors by value to do the match
void Event::jetMatch(double R, std::vector<hltObj::Jet*> Reference, std::string SelectedJets){

    Matches.clear(); //we have to clear vector in order to make new matches

    std::map<std::string, std::vector<hltObj::Jet*> > StringToObj{

        std::make_pair("RecoJets", RecoJ),
        std::make_pair("GenJets", GenJ),
        std::make_pair("GenNNJets", GenNNJ),
        std::make_pair("L1Jets", L1J),
        std::make_pair("CaloJets", CaloJ),  
        std::make_pair("PFJets", PFJ),
        std::make_pair("CaloBJets", CaloBJ),
        std::make_pair("PFBJets", PFBJ),
        std::make_pair("BMatchedJets", RecoJM)
    };

    //clearing matches before continue
    for(auto j : StringToObj[SelectedJets]){
        j->matched = false;
    }
    

    //creating copy. If a jet is matched we delete it from the 
    //local list.
    std::vector<hltObj::Jet*> MatchedJets_copy = Reference;

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
                mj_match_ref.e = MatchedJets_copy.at(index_mj.at(min_index))->e; //filling energy
            
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

void Event::bMatch(double R, std::string SelectedJets){

    BMatches.clear(); //we have to clear vector in order to make new matches

    std::map<std::string, std::vector<hltObj::Jet*> > StringToObj{

            std::make_pair("RecoJets", RecoJ),
            std::make_pair("GenJets", GenJ),
            std::make_pair("GenNNJets", GenNNJ),
            std::make_pair("L1Jets", L1J),
            std::make_pair("CaloJets", CaloJ),  
            std::make_pair("PFJets", PFJ),
            std::make_pair("CaloBJets", CaloBJ),
            std::make_pair("PFBJets", PFBJ),

    };

    //clearing matches before continue, not wasting much time
    for(auto j : StringToObj[SelectedJets]){
        j->matched = false;
    }

    //creating copy. If a jet is matched we delete it from the 
    //local list.
    std::vector<hltObj::bQuark*> b_copy = bs;

    while(b_copy.size()!=0 && StringToObj[SelectedJets].size() != 0){
        std::vector<double> dr;
        std::vector<int> index_trg;
        std::vector<int> index_mj;
        for(int i = 0; i < b_copy.size(); i++){
            for(int j = 0; j < StringToObj[SelectedJets].size(); j++){
                if(!StringToObj[SelectedJets].at(j)->matched){
                    dr.push_back(sqrt(pow(b_copy.at(i)->eta-StringToObj[SelectedJets].at(j)->eta, 2) + 
                                        pow(b_copy.at(i)->phi-StringToObj[SelectedJets].at(j)->phi, 2)));
                    index_trg.push_back(j);
                    index_mj.push_back(i);
                }

            }
        }

        int min_index = std::min_element(dr.begin(),dr.end()) - dr.begin();

        //due to the fact that we have if(!StringToObj->matched)  we have to check dr.size()
        if(dr.size() > 0){
            if(dr.at(min_index) <= R){
                
                hltObj::bQuark mj_match_ref(b_copy.at(index_mj.at(min_index))->pt, 
                                            b_copy.at(index_mj.at(min_index))->eta,
                                                b_copy.at(index_mj.at(min_index))->phi);
            
                StringToObj[SelectedJets].at(index_trg.at(min_index))->bmatched = true;
                BMatches.push_back(mj_match_ref);
                StringToObj[SelectedJets].at(index_trg.at(min_index))->MatchedB = &BMatches.at(BMatches.size()-1);
                RecoJM.push_back(StringToObj[SelectedJets].at(index_trg.at(min_index)));
                
            }
            b_copy.erase(b_copy.begin()+index_mj.at(min_index));

        }
        else{
            b_copy.erase(b_copy.begin()+0);
        }

    }    

    return;
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

bool Event::isResolved(double R){

    bool resolved = true;
    for(int i = 0; i < bs.size()-1; i++){
        for(int j = i+1; j < bs.size(); j++){
            resolved = resolved && sqrt(pow(bs.at(i)->eta - bs.at(j)->eta, 2) + 
                                            pow(bs.at(i)->phi - bs.at(j)->phi, 2)) >= R;
        }
    }

    return resolved;
}





