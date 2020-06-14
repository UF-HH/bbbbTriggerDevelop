// c++ -o filEff filter_efficiency.cpp ../src/trigger_manager.cc `root-config --glibs --cflags`

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
#include "../interface/make_trigger_paths.h"
#include "../interface/trigger_manager.h"

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

    for(int trig = 0; trig < trig_names.size(); trig++){
    //for(int trig = 1; trig < 1; trig++){

        TH1F* h_tot;
        std::vector<TH1F*> h_cut;
        std::vector<TCanvas*> c;
        std::vector<TEfficiency*> eff;
        std::vector<TLegend*> leg;

        std::vector<std::pair<std::string,int>> fil_map = paths.get_map().at(trig);

        man_filter* filter = new man_filter(fil_map , trig_names.at(trig));

        filter->set_OR(paths.get_OR().at(trig));

        std::cout << "...year...  : " << year << std::endl;
        std::cout << "TRIGGER:  " << trig_names.at(trig) << std::endl;
        for(auto c: filter->get_fil_names()){
            std::cout << "FILTER: " <<  c << std::endl;
        }
        
        for(int fil_id = 0; fil_id < filter->get_fil_names().size(); fil_id++){
            

            std::string fil_name = filter->get_fil_names().at(fil_id);

            std::string b_name = argv[1];

            std::string h_tot_title = b_name + " " + std::to_string(fil_id) + " " + argv[2] + " " + fil_name;
            std::string h_cut_title = b_name  + std::to_string(fil_id) + " cut " + fil_name;
            h_tot = new TH1F(h_tot_title.c_str(), h_tot_title.c_str(), 200, min, max);

            h_tot->SetMarkerStyle(2);
            h_tot->SetMarkerSize(1);
            h_cut.push_back(new TH1F(h_cut_title.c_str(), h_cut_title.c_str(), 200, min, max));
            h_cut.at(fil_id)->SetFillStyle(3003);
            h_cut.at(fil_id)->SetFillColor(kAzure+2);

            eff.push_back(new TEfficiency((b_name + " " + std::to_string(fil_id) + argv[2] + " " + fil_name).c_str(),b_name.c_str(), 200, min, max));
            eff[fil_id]->SetFillStyle(3004);
            eff[fil_id]->SetLineColor(kRed);

            leg.push_back(new TLegend(.89, .89, .45, .7));
            leg[fil_id]->SetBorderSize(0);
            leg[fil_id]->AddEntry(h_tot, (b_name + " tot " + trig_names.at(trig)).c_str());
            leg[fil_id]->AddEntry(h_cut[fil_id], (b_name  + " cut " + fil_name).c_str());
            leg[fil_id]->AddEntry(eff[fil_id], "Efficiency");

        }

        TTree *t1 = (TTree*)f->Get("checkTrg/trgObjTree");

        std::vector<int> counts_(fil_map.size(), 0);
        std::vector<int> trig_counts_(trig_names.size(), 0);

        for(int  i = 0; i < fil_map.size(); i++){
            //std::cout << "READING FILTER BRANCH: " << fil_map.at(i).first << std::endl;
            t1->SetBranchAddress((fil_map.at(i).first).c_str(), &counts_.at(i));
        }

        for(int i = 0; i < trig_names.size(); i++){
            //std::cout << "READING TRIGGER BRANCH: " << trig_names.at(i) << std::endl;
            t1->SetBranchAddress(trig_names.at(i).c_str(), &trig_counts_.at(i));
        }  

        double var;
        int pass;
        bool cut;

        std::string b_name = argv[1];

        t1->SetBranchAddress(b_name.c_str(), &var);
        t1->SetBranchAddress(trig_names.at(trig).c_str(), &pass);
        t1->SetBranchAddress("Cut", &cut);

        std::vector<int> counter(h_cut.size(), 0);

        for(Int_t i =0; i < t1->GetEntries(); i++){
                t1->GetEntry(i);
                if(cut){

                    auto r_1 = filter->f_test_suppose_sequential(counts_, false, 1);
                    int stop = filter->verify_filter_stop();

                    h_tot->Fill(var);

                    if(stop == -1)
                        stop = h_cut.size()-1;

                    counter.at(stop) ++;
                    
                    for(int j = 0; j < stop; j++){
                            h_cut[j]->Fill(var);
                            eff[j]->Fill(true, var);
                    }

                    for(int j = stop; j<filter->get_fil_names().size(); j++)
                            eff[j]->Fill(false, var);


                }
        }


        for(int i = 0; i < filter->get_fil_names().size(); i++){
            std::cout << "[INFO]: Filter  " << filter->get_fil_names().at(i) << "tot_count: " << (int)h_tot->Integral() << " pass_count: " << h_cut[i]->Integral() << " EFF: " << (double)h_cut[i]->Integral()/(h_tot->Integral()) << std::endl;
        }

        
        gStyle->SetOptStat(0);
        double max = h_tot->GetMaximum();
        h_tot->Scale(1./max);

        for(int i = 0; i < filter->get_fil_names().size(); i++){  
            
            std::string var = argv[1];
            
            h_tot->GetXaxis()->SetTitle((var+ " [Gev]").c_str());
            h_tot->GetYaxis()->SetTitle("Events");

            std::string fil_name = filter->get_fil_names().at(i);
            std::string b_name = argv[1];
            std::string h_tot_title = b_name + " " + argv[2] + " " + fil_name;

            h_tot->SetTitle(h_tot_title.c_str());
            
            c.push_back(new TCanvas((filter->get_fil_names().at(i)+std::to_string(i)).c_str(), (filter->get_fil_names().at(i)+std::to_string(i)).c_str(), 1000, 1000, 1000, 700));
            h_cut[i]->Scale(1./max);
            h_tot->SetMaximum(1.6);
            
            h_tot->Draw("P");
            h_cut[i]->Draw("hist same");
            leg[i]->Draw();
            eff[i]->Draw("P same");
            //c[i]->SetLogy();
            c[i]->Draw();
            std::string y = argv[2];
            std::string canv_save_title = "plots/filter_" + y + "/" + argv[1] + "/" + filter->get_fil_names().at(i) + "_" + argv[1] + std::to_string(trig) +  ".pdf";
            c[i]->SaveAs((canv_save_title).c_str());
        }

    }
    

    return 0;
}


