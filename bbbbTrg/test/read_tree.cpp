//c++ -o read read_tree.cpp `root-config --cflags --glibs`

#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TCanvas.h"

int main(){
    
    TFile* fIn = new TFile("./For_HHReweight_2016.root");
    TTree* tree = (TTree*) fIn->Get("checkTrg/trgObjTree");
    std::vector<std::vector<int>>* point_c = 0;
    tree->SetBranchAddress("trig_obj_fil_map", &point_c);
    tree->GetEntry(0);
    for(int i = 0; i < point_c->size(); i++){
        std::cout << "size: " << point_c->at(i).size() << std::endl;
    }
    return 0;
}
