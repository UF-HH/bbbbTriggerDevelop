//c++ -o weight weight.cpp HHReweight5D.cc `root-config --cflags --glibs`

#include "HHReweight5D.h"
#include "make_trigger_paths.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char*argv[]){

    if(argc != 2){
        std::cerr << "Only integer year must be insert" << std::endl;
        throw std::runtime_error("Too much input arguments or year is missing");
    }

    int year = std::atoi(argv[1]);
    make_trigger_paths* trigger_paths_year = new make_trigger_paths(year);

    bool save_p4 = true;
    std::string weight_file = "data/coefficientsByBin_extended_3M_costHHSim_19-4.txt";

    std::unique_ptr<HHReweight5D> hhreweighter;
    //std::vector<float> klambdas = {-2, -1, 0, 1, 2, 5, 10};
        
    std::vector<double> klambdas(20);
    double start = -10.;
    double finish = 10.;
    double step = (double)(finish-start)/klambdas.size();
    for(int i = 0; i < klambdas.size(); i++){
        klambdas.at(i) = start +((double)i*step);
    }  

    std::string kl_map    = "data/gg_HH_4B_SM_2018.root"; // sample map fname
    std::string kl_histo  = "hhGenLevelDistr";           // sample map histo
    std::string kl_coeffs = weight_file; // coefficient file

    TFile* fIn = TFile::Open(kl_map.c_str());
    TH2D*  hIn = (TH2D*) fIn->Get(kl_histo.c_str());
    hhreweighter = std::unique_ptr<HHReweight5D> (new HHReweight5D(kl_coeffs.c_str(), hIn));
    fIn->Close();

    std::string out_file = "data/weight_out_2018.root";
    std::cout << out_file << std::endl;
    TFile outputFile(out_file.c_str(), "recreate");
    
    // TTree with a custom format
    std::string out_title = "HH_gen_info";
    TTree* tOut = new TTree(out_title.c_str(), out_title.c_str());
    
    //reading original tree 
    TFile* f = new TFile("../data/For_HHReweight_2018.root");
    TTree* tIn = (TTree*) f->Get("checkTrg/trgObjTree;3");
    
    // vars
    double gen_H1_m;
    double gen_H1_pt;
    double gen_H1_eta;
    double gen_H1_phi;
    TLorentzVector* gen_H1_p4;

    double gen_H2_m;
    double gen_H2_pt;
    double gen_H2_eta;
    double gen_H2_phi;
    TLorentzVector* gen_H2_p4;

    float gen_mHH;
    float gen_costh_H1_cm;
    float gen_costh_H2_cm;

    bool cut_;
    

    // branches
    tIn->SetBranchAddress("gen_H1_m",   &gen_H1_m);
    tIn->SetBranchAddress("gen_H1_pt",  &gen_H1_pt);
    tIn->SetBranchAddress("gen_H1_eta", &gen_H1_eta);
    tIn->SetBranchAddress("gen_H1_phi", &gen_H1_phi);
    tIn->SetBranchAddress("gen_H1_p4",  &gen_H1_p4);

    tIn->SetBranchAddress("gen_H2_m",   &gen_H2_m);
    tIn->SetBranchAddress("gen_H2_pt",  &gen_H2_pt);
    tIn->SetBranchAddress("gen_H2_eta", &gen_H2_eta);
    tIn->SetBranchAddress("gen_H2_phi", &gen_H2_phi);
    tIn->SetBranchAddress("gen_H2_p4",  &gen_H2_p4);

    tIn->SetBranchAddress("gen_mHH",         &gen_mHH);
    tIn->SetBranchAddress("gen_costh_H1_cm", &gen_costh_H1_cm);
    tIn->SetBranchAddress("gen_costh_H2_cm", &gen_costh_H2_cm);

    tIn->SetBranchAddress("Cut", &cut_);


    //copying filters in new weighted tree
    std::vector<std::pair<std::string,int>> fil_map = (trigger_paths_year->get_map()).at(0);

    /*
    std::vector<std::pair<std::string,int>> fil_map;

    fil_map.push_back(std::pair<std::string, int>("hltL1sQuadJetC50IorQuadJetC60IorHTT280IorHTT300IorHTT320IorTripleJet846848VBFIorTripleJet887256VBFIorTripleJet927664VBF", 1));
    fil_map.push_back(std::pair<std::string, int>("hltL1sQuadJetCIorTripleJetVBFIorHTT", 1));
    fil_map.push_back(std::pair<std::string, int>("hltQuadCentralJet45", 4));
    fil_map.push_back(std::pair<std::string, int>("hltQuadPFCentralJetLooseID45", 4));
    fil_map.push_back(std::pair<std::string, int>("hltBTagCaloCSVp087Triple", 3));
    fil_map.push_back(std::pair<std::string, int>("hltL1sTripleJetVBFIorHTTIorDoubleJetCIorSingleJet", 1));
    fil_map.push_back(std::pair<std::string, int>("hltDoubleCentralJet90", 2));
    fil_map.push_back(std::pair<std::string, int>("hltQuadCentralJet30", 4));
    fil_map.push_back(std::pair<std::string, int>("hltDoublePFCentralJetLooseID90", 2));
    fil_map.push_back(std::pair<std::string, int>("hltQuadPFCentralJetLooseID30", 4));
    
    */
    std::vector<std::string> trigger_names_ = trigger_paths_year->get_trig_name();
    //std::vector<std::string> trigger_names_;
    //trigger_names_.push_back("HLT_QuadJet45_TripleBTagCSV_p087_v6");
    //trigger_names_.push_back("HLT_DoubleJet90_Double30_TripleBTagCSV_p087_v5");

    std::vector<int> counts_(fil_map.size(), 0);
    std::vector<int> trig_counts_(trigger_names_.size(), 0);

    for(int  i = 0; i < fil_map.size(); i++){
        std::cout << "SETTING FILTER BRANCH: " << fil_map.at(i).first << std::endl;
        tIn->SetBranchAddress((fil_map.at(i).first).c_str(), &counts_.at(i));
        tOut->Branch((fil_map.at(i).first).c_str(), &counts_.at(i));
    }

    for(int i = 0; i < trigger_names_.size(); i++){
        std::cout << "SETTING TRIGGER BRANCH: " << trigger_names_.at(i) << std::endl;
        tIn->SetBranchAddress(trigger_names_.at(i).c_str(), &trig_counts_.at(i));
        tOut->Branch(trigger_names_.at(i).c_str(), &trig_counts_.at(i));
    }  

    tOut->Branch("Cut", &cut_);

    std::vector<float>  klambdas_branch(klambdas.size());
    std::vector<std::string> klambdas_names(klambdas.size());
    if (hhreweighter)
    {
        for (uint ikl = 0; ikl < klambdas.size(); ++ikl)
        {
            float kl = klambdas.at(ikl);
            //string brname = std::to_string(kl);
            std::stringstream stream;
            stream << std::fixed << std::setprecision(4) << kl;
            std::string brname = stream.str();
            std::replace( brname.begin(), brname.end(), '-', 'm');
            std::replace( brname.begin(), brname.end(), '.', 'd');
            brname = std::string("HH_rew_") + brname;
            std::cout << ".... making reweight for " << kl << " in branch " << brname << std::endl;
            tOut->Branch(brname.c_str(), &klambdas_branch.at(ikl));
        }
    }

    std::cout << "Tree Entries: " << tIn->GetEntries() << std::endl;
    for(int ev_id = 0; ev_id < tIn->GetEntries(); ev_id++){
    //for(int ev_id = 0; ev_id < 1; ev_id++){
        if(ev_id%1000 == 0)
            std::cout << "Event: " << ev_id << std::endl;

        tIn->GetEntry(ev_id);
        

        if (hhreweighter)
            {
                for (uint ikl = 0; ikl < klambdas.size(); ++ikl)
                {
                    float kl = klambdas.at(ikl);
                    //std::cout << "ev gen_mHH: " << gen_mHH << " ev gen_coth_H1" << gen_costh_H1_cm <<  "ev gen_coth_H2" <<  gen_costh_H2_cm<<std::endl;
                    float w = hhreweighter->getWeight(kl, 1.0, gen_mHH, gen_costh_H1_cm);
                    //std::cout << "w: " << w << std::endl;
                    klambdas_branch.at(ikl) = w;
                }
            }

        tOut->Fill();
    }  

    std::cout << "Saving output at: " << out_title << std::endl;

    outputFile.Write();
    
    return 0;
}
