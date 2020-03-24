// c++ -o trgEff trigger_efficiencies.cpp `root-config --glibs --cflags`

//---------------------
// Reads the tree output from the plug-in trgFilter 
// and plots the total events contained, the events that passed only filter one
// only filter two, and the or between filter in terms of a kinematical variable
// saved in the TTree and passed by command line (as name of branch in the .root)
//---------------------

#include <TTree.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TPaveLabel.h>
#include <TString.h>
#include <TObjArray.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <TEfficiency.h>
#include "bbbb_lambda/make_trigger_paths.h"

int main(int argc, char* argv[]){

    if(argc != 6){
        std::cerr<< "Only three kin variable at a time: branch_name, year, limit_inf, limit_sup, filein" << std::endl;
        throw std::runtime_error(("Found " + std::to_string(argc-1) + " Input arguments while two is required").c_str());
    }

    int year = std::atoi(argv[2]);
    double min = std::atof(argv[3]);
    double max = std::atof(argv[4]);
    std::string in_file = argv[5];


    make_trigger_paths paths(year);
    TFile* f = new TFile(in_file.c_str());

    std::vector<std::string> trig_names = paths.get_trig_name();
    for(auto c: trig_names){
        std::cout << c << std::endl;
    }

    std::vector<TH1F*> h_tot;
    std::vector<TH1F*> h_cut;
    std::vector<TCanvas*> c;
    std::vector<TEfficiency*> eff;
    std::vector<TLegend*> leg;


    for(int trig = 0; trig < trig_names.size(); trig++){

        TTree *t1 = (TTree*)f->Get("checkTrg/trgObjTree");

        double var;
        int pass;
        bool cut;

        std::string b_name = argv[1];

        t1->SetBranchAddress(b_name.c_str(), &var);
        t1->SetBranchAddress(trig_names.at(trig).c_str(), &pass);
        t1->SetBranchAddress("Cut", &cut);


        std::string h_tot_title = b_name + " " + argv[2] + " " + trig_names.at(trig);
        std::string h_cut_title = b_name + " cut " + trig_names.at(trig);
        h_tot.push_back(new TH1F(h_tot_title.c_str(), h_tot_title.c_str(), 200, min, max));
        h_tot.at(trig)->SetMarkerStyle(2);
        h_tot.at(trig)->SetMarkerSize(1);
        h_cut.push_back(new TH1F(h_cut_title.c_str(), h_cut_title.c_str(), 200, min, max));
        h_cut.at(trig)->SetFillStyle(3003);
        h_cut.at(trig)->SetFillColor(kAzure+2);

        eff.push_back(new TEfficiency(b_name.c_str(),b_name.c_str(), 200, min, max));
        eff[trig]->SetFillStyle(3004);
        eff[trig]->SetLineColor(kRed);

        int not_cut_count = 0;
        int count_pass_trig = 0;
        for(Int_t i =0; i < t1->GetEntries(); i++){
                bool passed = false;
                t1->GetEntry(i);
                if(cut){
                    h_tot[trig]->Fill(var);

                    if(pass){
                        h_cut[trig]->Fill(var);
                        passed = true;
                        count_pass_trig ++;
                    }

                    eff[trig]->Fill(passed, var);
                    not_cut_count ++;
                }

        }

        leg.push_back(new TLegend(.89, .89, .45, .6));
        leg[trig]->SetBorderSize(0);
        leg[trig]->AddEntry(h_tot[trig], (b_name + " tot " + trig_names.at(trig)).c_str());
        leg[trig]->AddEntry(h_cut[trig], h_cut_title.c_str());
        leg[trig]->AddEntry(eff[trig], "Efficiency");

        std::cout << "[INFO] TRIGGER: " << trig_names.at(trig) << " TOT Ev: " << t1->GetEntries() << " After CUT: " << not_cut_count << " After Trigger: " << count_pass_trig <<  std::endl;
    }

    //plotting
    gStyle->SetOptStat(0);
    for(int i = 0; i < trig_names.size(); i++){
        
        std::cout << "[INFO]: TRIGGER  " << trig_names.at(i) << " EFF: " << (double)h_cut[i]->Integral()/h_tot[i]->Integral() << std::endl;
        std::string var = argv[1];
        double max = h_tot[i]->GetMaximum();
        h_tot[i]->GetXaxis()->SetTitle((var+ " [Gev]").c_str());
        h_tot[i]->GetYaxis()->SetTitle("Events");
        c.push_back(new TCanvas(trig_names.at(i).c_str(), trig_names.at(i).c_str(), 1000, 1000, 1000, 700));
        h_tot[i]->Scale(1./max);
        h_cut[i]->Scale(1./max);
        h_tot[i]->SetMaximum(1.5);
        h_tot[i]->Draw("P");
        h_cut[i]->Draw("hist same");
        leg[i]->Draw();
        eff[i]->Draw("P same");
        //c[i]->SetLogy();
        c[i]->Draw();
        c[i]->SaveAs(("plots/" + trig_names.at(i) + "_" + argv[1] + ".pdf").c_str());
    }


    

    return 0;
}


