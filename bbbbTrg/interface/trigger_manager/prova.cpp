//c++ -o p prova.cpp `root-config --glibs --cflags`

#include <iostream>
#include <string>
#include <sstream>
#include <TTree.h>
#include <TCanvas.h>
#include <TEfficiency.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TStyle.h>

int main(){

    TFile* f = new TFile("../data/trigger_obj_tree.root");
    TTree* tree_ = (TTree*)f->Get("checkTrg/trgObjTree");

    int count_1;
    int count_2;
    int count_3;
    int count_4;
    int count_5;
    //int count_6;
    bool cut_;
    /*
    tree_->SetBranchAddress("hltL1sTripleJetVBFIorHTTIorDoubleJetCIorSingleJet", &count_1);
    tree_->SetBranchAddress("hltDoubleCentralJet90", &count_2);
    tree_->SetBranchAddress("hltQuadCentralJet30", &count_3);
    tree_->SetBranchAddress("hltDoublePFCentralJetLooseID90", &count_4);
    tree_->SetBranchAddress("hltQuadPFCentralJetLooseID30", &count_5);
    tree_->SetBranchAddress("hltBTagCaloCSVp087Triple", &count_6);
    tree_->SetBranchAddress("Cut", &cut_);
    
    int count_true_6 = 0;
    int count_true_5 = 0;
    int count_true_4 = 0;
    int count_true_3 = 0;
    int count_true_2 = 0;
    int count_true_1 = 0;
    int count_cut = 0;
*/
    tree_->SetBranchAddress("hltL1sQuadJetC50IorQuadJetC60IorHTT280IorHTT300IorHTT320IorTripleJet846848VBFIorTripleJet887256VBFIorTripleJet927664VBF", &count_1);
    tree_->SetBranchAddress("hltL1sQuadJetCIorTripleJetVBFIorHTT", &count_2);
    tree_->SetBranchAddress("hltQuadCentralJet45", &count_3);
    tree_->SetBranchAddress("hltBTagCaloCSVp087Triple", &count_4);
    tree_->SetBranchAddress("hltQuadPFCentralJetLooseID45", &count_5);
    
    tree_->SetBranchAddress("Cut", &cut_);
    
    //int count_true_6 = 0;
    //int count_true_5 = 0;
    int count_true_4 = 0;
    int count_true_3 = 0;
    int count_true_2 = 0;
    int count_true_1 = 0;
    int count_cut = 0;
/*
    for(int i = 0; i < tree_->GetEntries(); i++){

        tree_->GetEntry(i);

        if(cut_){
            if(count_1 >= 1){
                count_true_1++;
                if(count_2 >= 2){
                    count_true_2++;
                    if(count_3 >= 4){
                        count_true_3++;
                        if(count_4 >= 2){
                            count_true_4++;
                            if(count_5 >= 4){
                                count_true_5++;
                                if(count_6 >= 3){
                                    count_true_6++;
                                }
                            }
                        }
                    }                
                }
            }
        }
        else
            count_cut++;

        //std::cout << count_1 << std::endl;
        //std::cout << count_2 << std::endl;
    }
*/
    for(int i = 0; i < tree_->GetEntries(); i++){

        tree_->GetEntry(i);

        if(cut_){
            if(count_1 >= 1 || count_2 >= 1){
                count_true_1++;
                if(count_3 >= 4){
                    count_true_2++;
                    if(count_4 >= 3){
                        count_true_3++;
                        if(count_5 >= 4){
                            count_true_4++;
                        }
                    }
                }
            }                
        }
        else
            count_cut++;

        //std::cout << count_1 << std::endl;
        //std::cout << count_2 << std::endl;
    }
    std::cout << "Tot events: " << tree_->GetEntries() << std::endl;
    std::cout << "Events after cut: " << tree_->GetEntries() - count_cut << std::endl;
    std::cout << "Ratio filter1 to tot evt (cumulative): " << (double)count_true_1/(tree_->GetEntries()-count_cut) << std::endl;
    std::cout << "Ratio filter1 to cut (relative): " << (double)count_true_1/(tree_->GetEntries()-count_cut) << std::endl;
    std::cout << "Ratio filter2 to tot evt (cumulative): " << (double)count_true_2/(tree_->GetEntries()-count_cut) << std::endl;
    //std::cout << "Ratio filter2 to filter1 (relative): " << (double)count_true_2/count_true_1 << std::endl;
    std::cout << "Ratio filter3 to tot evt (cumulative): " << (double)count_true_3/(tree_->GetEntries()-count_cut) << std::endl;
    //std::cout << "Ratio filter3 to filter2 (relative): " << (double)count_true_3/count_true_2 << std::endl;
    std::cout << "Ratio filter4 to tot evt (cumulative): " << (double)count_true_4/(tree_->GetEntries()-count_cut) << std::endl;
    //std::cout << "Ratio filter4 to filter3 (relative): " << (double)count_true_4/count_true_3 << std::endl;
    //std::cout << "Ratio filter5 to tot evt (cumulative): " << (double)count_true_5/tree_->GetEntries() << std::endl;
    //std::cout << "Ratio filter5 to filter4 (relative): " << (double)count_true_5/count_true_4 << std::endl;
    //std::cout << "Ratio filter6 to tot evt (cumulative): " << (double)count_true_6/tree_->GetEntries() << std::endl;
    //std::cout << "Ratio filter6 to filter5 (relative): " << (double)count_true_6/count_true_5 << std::endl;

    return 0;
}
