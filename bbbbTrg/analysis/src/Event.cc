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

    tree->SetBranchAddress("reco_jet_pt", &reco_jet_pt);
    tree->SetBranchAddress("reco_jet_mass", &reco_jet_mass);
    tree->SetBranchAddress("reco_jet_eta", &reco_jet_eta);
    tree->SetBranchAddress("reco_jet_phi", &reco_jet_phi);
    tree->SetBranchAddress("reco_jet_energy", &reco_jet_energy);
    tree->SetBranchAddress("reco_jet_et", &reco_jet_et);
    tree->SetBranchAddress("reco_jet_b_tag", &reco_jet_b_tag);

    tree->SetBranchAddress("trig_obj_pt", &trig_obj_pt);
    tree->SetBranchAddress("trig_obj_et", &trig_obj_et);
    tree->SetBranchAddress("trig_obj_mass", &trig_obj_mass);
    tree->SetBranchAddress("trig_obj_eta", &trig_obj_eta);
    tree->SetBranchAddress("trig_obj_phi", &trig_obj_phi);

    tree->SetBranchAddress("is_jet", &is_jet);
    tree->SetBranchAddress("is_bjet", &is_bjet);
    tree->SetBranchAddress("is_pf_ht", &is_pf_ht);
    tree->SetBranchAddress("is_calo_ht", &is_calo_ht);
    tree->SetBranchAddress("is_l1_ht", &is_l1_ht);
    tree->SetBranchAddress("is_l1_jet", &is_l1_jet);

    tree->SetBranchAddress("is_calo_jet", &is_calo_jet);
    tree->SetBranchAddress("is_calo_bjet", &is_calo_bjet);
    tree->SetBranchAddress("is_pf_jet", &is_pf_jet);
    tree->SetBranchAddress("is_pf_bjet", &is_pf_bjet);

    tree->SetBranchAddress("gen_H1_m",   &H1.gen_H_m);
    tree->SetBranchAddress("gen_H1_pt",  &H1.gen_H_pt);
    tree->SetBranchAddress("gen_H1_eta", &H1.gen_H_eta);
    tree->SetBranchAddress("gen_H1_phi", &H1.gen_H_phi);
    tree->SetBranchAddress("gen_H1_p4",  &H1.gen_H_p4);

    tree->SetBranchAddress("gen_H2_m",   &H2.gen_H_m);
    tree->SetBranchAddress("gen_H2_pt",  &H2.gen_H_pt);
    tree->SetBranchAddress("gen_H2_eta", &H2.gen_H_eta);
    tree->SetBranchAddress("gen_H2_phi", &H2.gen_H_phi);
    tree->SetBranchAddress("gen_H2_p4",  &H2.gen_H_p4);

    tree->SetBranchAddress("b_pt", &BS.pt);
    tree->SetBranchAddress("b_eta", &BS.eta);
    tree->SetBranchAddress("b_phi", &BS.phi);
    tree->SetBranchAddress("b_et", &BS.et);
    tree->SetBranchAddress("b_e", &BS.e);

}

void Event::clear(){
    bs.clear();
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
    RecoJM.clear();
    BMatches.clear();

    return; 
}
void Event::Generate(){

    clear();

    if(event_index%10000 == 0){
        std::cout << event_index << std::endl;
    }

    tree->GetEntry(event_index);

    //Setting up Higgs pair cos and mHH:
    H1.SetCos(H2);
    //retrieving pf jets
    for(uint j = 0; j < trig_obj_pt->size(); j++){

        //kinematics
        if(is_pf_jet->at(j) == 1){
            PFJets.pt.push_back(trig_obj_pt->at(j));
            PFJets.eta.push_back(trig_obj_eta->at(j));
            PFJets.phi.push_back(trig_obj_phi->at(j));
            PFJets.et.push_back(trig_obj_et->at(j));
        }
        else{
            PFJets.pt.push_back(0);
            PFJets.eta.push_back(0);
            PFJets.phi.push_back(0);
            PFJets.et.push_back(0);
        }
        if(is_calo_jet->at(j) == 1){
            CaloJets.pt.push_back(trig_obj_pt->at(j));
            CaloJets.et.push_back(trig_obj_et->at(j));
            CaloJets.eta.push_back(trig_obj_eta->at(j));
            CaloJets.phi.push_back(trig_obj_phi->at(j));
        }
        else{
            CaloJets.pt.push_back(0);
            CaloJets.et.push_back(0);
            CaloJets.eta.push_back(0);
            CaloJets.phi.push_back(0);
        }   

        if(is_calo_ht->at(j) == 1){
            CaloJets.HT = trig_obj_et->at(j);
            CaloBJets.HT = trig_obj_et->at(j);
        }
        if(is_pf_ht->at(j) == 1){
            PFJets.HT = trig_obj_pt->at(j);
            PFBJets.HT = trig_obj_pt->at(j);
        }
        if(is_l1_ht->at(j) == 1){
            L1Jets.HT = trig_obj_pt->at(j);
        }

        if(is_l1_jet->at(j) == 1){
            L1Jets.pt.push_back(trig_obj_pt->at(j));
            L1Jets.eta.push_back(trig_obj_eta->at(j));
            L1Jets.phi.push_back(trig_obj_phi->at(j));
        }
        else{
            L1Jets.pt.push_back(0);
            L1Jets.eta.push_back(0);
            L1Jets.phi.push_back(0);
        }

        //btagging

        if(is_bjet->at(j) == 1){
            if(is_pf_bjet->at(j) == 1){
                PFBJets.pt.push_back(trig_obj_pt->at(j));
                PFBJets.et.push_back(trig_obj_et->at(j));
                PFBJets.eta.push_back(trig_obj_eta->at(j));
                PFBJets.phi.push_back(trig_obj_phi->at(j));
            }
            else{
                PFBJets.pt.push_back(0);
                PFBJets.et.push_back(0);
                PFBJets.eta.push_back(0);
                PFBJets.phi.push_back(0);
            }
            if (is_calo_bjet->at(j) == 1){
                CaloBJets.pt.push_back(trig_obj_pt->at(j));
                CaloBJets.et.push_back(trig_obj_et->at(j));
                CaloBJets.eta.push_back(trig_obj_eta->at(j));
                CaloBJets.phi.push_back(trig_obj_phi->at(j));
            }
            else{
                CaloBJets.pt.push_back(0);
                CaloBJets.et.push_back(0);
                CaloBJets.eta.push_back(0);
                CaloBJets.phi.push_back(0);
            }

        }
        else{
            CaloBJets.pt.push_back(0);
            CaloBJets.et.push_back(0);
            CaloBJets.eta.push_back(0);
            CaloBJets.phi.push_back(0);
            PFBJets.pt.push_back(0);
            PFBJets.et.push_back(0);
            PFBJets.eta.push_back(0);
            PFBJets.phi.push_back(0);
        }

    } 

    RecoJets.pt = *reco_jet_pt;
    RecoJets.mass = *reco_jet_mass;
    RecoJets.eta = *reco_jet_eta;
    RecoJets.phi = *reco_jet_phi;
    RecoJets.e = *reco_jet_energy;
    RecoJets.et = *reco_jet_et;
    RecoJets.btag = *reco_jet_b_tag;

    RecoJets.Type = "RecoJets";
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


void Event::WeightFactory(std::string kl_map, std::string kl_histo, std::string kl_coeffs){

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

    for(int i = 0; i < BS.size(); i++){
        bs.push_back(new hltObj::bQuark(BS.pt->at(i), BS.eta->at(i), BS.phi->at(i)));
    }

    return;

}


void Event::jetMatch(double R, std::string Reference, std::string SelectedJets){

    Matches.clear(); //we have to clear vector in order to make new matches

    std::map<std::string, std::vector<hltObj::Jet*> > StringToObj{

        std::make_pair("RecoJetsMatched", RecoJM),
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

void Event::bMatch(double R, std::string SelectedJets){

    BMatches.clear(); //we have to clear vector in order to make new matches

    std::map<std::string, std::vector<hltObj::Jet*> > StringToObj{

            std::make_pair("RecoJets", RecoJ),
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

double Event::GetCaloHT(){
    auto it = std::find_if(is_calo_ht->begin(), is_calo_ht->end(), [](auto i){return i != 0; });
    if(it != is_calo_ht->end())
        return trig_obj_pt->at(std::distance(is_calo_ht->begin(), it));
    else 
        return 0;
}

double Event::GetPFHT(){
    auto it = std::find_if(is_pf_ht->begin(), is_pf_ht->end(), [](auto i){return i != 0; });
    if(it != is_pf_ht->end())
        return trig_obj_pt->at(std::distance(is_pf_ht->begin(), it));
    else 
        return 0;
}

double Event::GetL1HT(){
    auto it = std::find_if(is_l1_ht->begin(), is_l1_ht->end(), [](auto i){return i != 0; });
    if(it != is_l1_ht->end())
        return trig_obj_pt->at(std::distance(is_l1_ht->begin(), it));
    else 
        return 0;
}





