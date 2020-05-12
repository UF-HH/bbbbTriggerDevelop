//c++ -o RecoJEff RecoJetsEfficiency.cpp ../src/Event.cc ../src/TriggerMaker.cc `root-config --cflags --glibs`

#include <iostream>
#include <string.h>
#include <iomanip>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "../interface/TriggerMaker.h"
#include <iostream>
#include <fstream>
#include "time.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TLine.h"
#include "TLegend.h"
#include "TEfficiency.h"
#include "TLatex.h"

using namespace hltObj;

double dr(hltObj::Jet* reco, hltObj::Jet* jet){
    return sqrt(pow(reco->eta-jet->eta,2) + pow(reco->phi-jet->phi,2));
}

int main(){

    std::string kl_map    = "../data_weight/gg_HH_4B_SM_2018.root"; // sample map fname
    std::string kl_histo  = "hhGenLevelDistr";           // sample map histo
    std::string kl_coeffs = "../data_weight/coefficientsByBin_extended_3M_costHHSim_19-4.txt"; // coefficient file

    TFile* f = new TFile("../../test/filter_tests_2018_nob.root");
    std::string branch = "checkTrg/trgObjTree";

    Event ev(f, branch);
    ev.WeightFactory(kl_map, kl_histo, kl_coeffs);

    std::vector<float> klambdas = {1, 2, 2.5, 3};

    //time checking for algorithms
    time_t my_time = time(NULL); 
    printf("%s", ctime(&my_time));

    int entries = ev.GetEntries();
    //int entries = 1000; //control

    //Cutting on Reco Objects
    hltObj::KinCuts RecoCut;
    RecoCut.Type = "RecoJets";
    RecoCut.MinPt = 20.0;
    RecoCut.MinEta = -5;
    RecoCut.MaxEta = 5;
    ev.AddCut(RecoCut);

    //Cutting on PF Objects
    hltObj::KinCuts PFCut;
    PFCut.Type = "PFJets";
    PFCut.MinPt = 20.0;
    PFCut.MinEta = -5;
    PFCut.MaxEta = 5;

    ev.AddCut(PFCut);

    hltObj::KinCuts CaloCut;
    RecoCut.Type = "CaloJets";
    RecoCut.MinPt = 20.0;
    RecoCut.MinEta = -5;
    RecoCut.MaxEta = 5;

    ev.AddCut(CaloCut);

    hltObj::KinCuts L1Cut;
    RecoCut.Type = "L1Jets";
    RecoCut.MinPt = 20.0;
    RecoCut.MinEta = -5;
    RecoCut.MaxEta = 5;

    ev.AddCut(L1Cut);


    //Plotting initializers
    TEfficiency* eff = new TEfficiency("pf_eff", "pf_eff", 50, 0, 200);
    eff->SetLineColor(kBlack);
    eff->SetMarkerStyle(8);
    eff->SetMarkerColor(kBlack);
    eff->SetFillColor(kWhite);
    TEfficiency* eff_calo = new TEfficiency("calo_eff", "calo_eff", 50, 0, 200);
    eff_calo->SetLineColor(kMagenta+1);
    eff_calo->SetMarkerStyle(24);
    eff_calo->SetMarkerColor(kMagenta+1);
    eff_calo->SetFillColor(kWhite);
    TEfficiency* eff_L1 = new TEfficiency("L1_eff", "L1_eff", 50, 0, 200);
    eff_L1->SetLineColor(kBlue);
    eff_L1->SetMarkerStyle(25);
    eff_L1->SetMarkerColor(kBlue);
    eff_L1->SetFillColor(kWhite);

    TLegend* leg = new TLegend(0.89, 0.35, 0.68, 0.18);
    leg->SetBorderSize(0.);
    leg->AddEntry(eff, "PFJets", "P");
    leg->AddEntry(eff_calo, "CaloJets", "P");
    leg->AddEntry(eff_L1, "L1Jets", "P");

    //Central
    TEfficiency* eff_1 = new TEfficiency("eff_1", "eff_1", 50, 0, 200);
    eff_1->SetLineColor(kBlack);
    eff_1->SetMarkerStyle(8);
    eff_1->SetMarkerColor(kBlack);
    eff_1->SetFillColor(kWhite);
    TEfficiency* eff_calo_1 = new TEfficiency("eff_calo_1", "eff_calo_1", 50, 0, 200);
    eff_calo_1->SetLineColor(kMagenta+1);
    eff_calo_1->SetMarkerStyle(24);
    eff_calo_1->SetMarkerColor(kMagenta+1);
    eff_calo_1->SetFillColor(kWhite);
    TEfficiency* eff_L1_1 = new TEfficiency("eff_L1_1", "eff_L1_1", 50, 0, 200);
    eff_L1_1->SetLineColor(kBlue);
    eff_L1_1->SetMarkerStyle(25);
    eff_L1_1->SetMarkerColor(kBlue);
    eff_L1_1->SetFillColor(kWhite);

    TLegend* leg_1 = new TLegend(0.89, 0.35, 0.68, 0.18);
    leg_1->SetBorderSize(0.);
    leg_1->AddEntry(eff_1, "PFJets", "P");
    leg_1->AddEntry(eff_calo_1, "CaloJets", "P");
    leg_1->AddEntry(eff_L1_1, "L1Jets", "P");

    //Forward
    TEfficiency* eff_2 = new TEfficiency("eff_2", "eff_2", 50, 0, 200);
    eff_2->SetLineColor(kBlack);
    eff_2->SetMarkerStyle(8);
    eff_2->SetMarkerColor(kBlack);
    eff_2->SetFillColor(kWhite);
    TEfficiency* eff_calo_2 = new TEfficiency("eff_calo_2", "eff_calo_2", 50, 0, 200);
    eff_calo_2->SetLineColor(kMagenta+1);
    eff_calo_2->SetMarkerStyle(24);
    eff_calo_2->SetMarkerColor(kMagenta+1);
    eff_calo_2->SetFillColor(kWhite);
    TEfficiency* eff_L1_2 = new TEfficiency("eff_L1_2", "eff_L1_2", 50, 0, 200);
    eff_L1_2->SetLineColor(kBlue);
    eff_L1_2->SetMarkerStyle(25);
    eff_L1_2->SetMarkerColor(kBlue);
    eff_L1_2->SetFillColor(kWhite);

    TLegend* leg_2 = new TLegend(0.89, 0.35, 0.68, 0.18);
    leg_2->SetBorderSize(0.);
    leg_2->AddEntry(eff_1, "PFJets", "P");
    leg_2->AddEntry(eff_calo_1, "CaloJets", "P");
    leg_2->AddEntry(eff_L1_1, "L1Jets", "P");

    //---------------------

    TH1F* h_calo = new TH1F("h_calo", "h_calo", 30, 0, 0.4);
    h_calo->SetMarkerStyle(24);
    h_calo->SetLineColor(kMagenta+1);
    h_calo->SetMarkerColor(kMagenta+1);

    TH1F* h_pf = new TH1F("h_pf", "h_pf", 30, 0, 0.4);
    h_pf->SetMarkerStyle(8);
    h_pf->SetLineColor(kBlack);
    h_pf->SetMarkerColor(kBlack);
    h_pf->GetXaxis()->SetTitle("#DeltaR (TriggerObject-Offline)");
    h_pf->GetXaxis()->SetTitleOffset(1.2);
    h_pf->GetYaxis()->SetTitle("(1/N_{jet})dN_{jet}/dR");
    h_pf->SetTitle("");

    TH1F* h_L1 = new TH1F("h_L1", "h_L1", 30, 0, 0.4);
    h_L1->SetMarkerStyle(25);
    h_L1->SetLineColor(kBlue);
    h_L1->SetMarkerColor(kBlue);

    TLegend* leg_c = new TLegend(0.85, 0.83, 0.65, 0.65);
    leg_c->SetBorderSize(0.);
    leg_c->AddEntry(h_pf, "PFJets", "P");
    leg_c->AddEntry(h_calo, "CaloJets", "P");
    leg_c->AddEntry(h_L1, "L1Jets", "P");


    TH1F* h_calo_f = new TH1F("h_calo_f", "h_calo_f", 30, 0, 0.4);
    h_calo_f->SetMarkerStyle(24);
    h_calo_f->SetLineColor(kMagenta+1);
    h_calo_f->SetMarkerColor(kMagenta+1);

    TH1F* h_pf_f = new TH1F("h_pf_f", "h_pf_f", 30, 0, 0.4);
    h_pf_f->SetMarkerStyle(8);
    h_pf_f->SetLineColor(kBlack);
    h_pf_f->SetMarkerColor(kBlack);
    h_pf_f->GetXaxis()->SetTitle("#DeltaR (TriggerObject-Offline)");
    h_pf_f->GetXaxis()->SetTitleOffset(1.2);
    h_pf_f->GetYaxis()->SetTitle("(1/N_{jet})dN_{jet}/dR");
    h_pf_f->SetTitle("");

    TH1F* h_L1_f = new TH1F("h_L1_f", "h_L1_f", 30, 0, 0.4);
    h_L1_f->SetMarkerStyle(25);
    h_L1_f->SetLineColor(kBlue);
    h_L1_f->SetMarkerColor(kBlue);

    TLegend* leg_f = new TLegend(0.85, 0.83, 0.65, 0.65);
    leg_f->SetBorderSize(0.);
    leg_f->AddEntry(h_pf_f, "PFJets", "P");
    leg_f->AddEntry(h_calo_f, "CaloJets", "P");
    leg_f->AddEntry(h_L1_f, "L1Jets", "P");


    for(int i = 0; i < entries; i++){

        ev.Generate();
        ev.UnpackCollections();
        ev.bMatch(0.4, "RecoJets");
        ev.jetMatch(0.4, "PFJets", "RecoJetsMatched");
        //matching efficiency both for reco jets
        //and for trgobj
        std::vector<hltObj::Jet*> pf = ev.GetPFJets();
        std::vector<hltObj::Jet*> reco = ev.GetRecoJetsMatch();

        if(reco.size() > 4){
            std::cerr << "non è possibile qvesto" << std::endl;
            throw std::runtime_error("nope");
        }
        if(std::count_if(pf.begin(), pf.end(), [](const hltObj::Jet* j){return j->matched == true;}) > 4){
            std::cerr << "non è possibile qvesto" << std::endl;
            throw std::runtime_error("nope");
        }

        //plotting jet recostruction efficiency as a function of the ET of the offline jet
        if(reco.size() == 4){
            
            for(auto c : reco){
                if(c->matched){
                    if(c->eta < 3.0 && c->eta > -3.0){
                        h_pf->Fill(dr(c, c->MatchedObj));
                    }
                    else if( (c->eta < 5.0 && c->eta > 3.0) || (c->eta > -5.0 && c->eta < -3.0) ){
                        h_pf_f->Fill(dr(c, c->MatchedObj));
                    }
                    eff->Fill(true, c->pt);
                }
                else{
                    eff->Fill(false, c->pt);
                }
            }

            for(auto c: reco){
                if(c->eta < 3.0 && c->eta > -3.0){
                    if(c->matched){
                        eff_1->Fill(true, c->pt);
                    }
                    else{
                        eff_1->Fill(false, c->pt);
                    }
                }

                if((c->eta < 5.0 && c->eta > 3.0) || (c->eta > -5.0 && c->eta < -3.0)){
                    if(c->matched){
                        eff_2->Fill(true, c->pt);
                    }
                    else{
                        eff_2->Fill(false, c->pt);
                    }
                }

            }
        }

        ev.jetMatch(0.4, "CaloJets", "RecoJetsMatched");
        //matching efficiency both for reco jets
        //and for trgobj
        reco = ev.GetRecoJetsMatch();

        if(reco.size() > 4){
            std::cerr << "non è possibile qvesto" << std::endl;
            throw std::runtime_error("nope");
        }

        //plotting jet recostruction efficiency as a function of the ET of the offline jet
        if(reco.size() == 4){
            
            for(auto c : reco){
                if(c->matched){
                    if(c->eta < 3.0 && c->eta > -3.0){
                        h_calo->Fill(dr(c, c->MatchedObj));
                    }
                    else if((c->eta < 5.0 && c->eta > 3.0) || (c->eta > -5.0 && c->eta < -3.0) ){
                        h_calo_f->Fill(dr(c, c->MatchedObj));
                        eff_calo_1->Fill(true, c->pt);
                    }
                    eff_calo->Fill(true, c->pt);
                }
                else{
                    eff_calo->Fill(false, c->pt);
                }
            }

            for(auto c: reco){
                if(c->eta < 3.0 && c->eta > -3.0){
                    if(c->matched){
                        eff_calo_1->Fill(true, c->pt);
                    }
                    else{
                        eff_calo_1->Fill(false, c->pt);
                    }
                }

                if((c->eta < 5.0 && c->eta > 3.0) || (c->eta > -5.0 && c->eta < -3.0)){
                    if(c->matched){
                        eff_calo_2->Fill(true, c->pt);
                    }
                    else{
                        eff_calo_2->Fill(false, c->pt);
                    }
                }

            }
        }

        ev.jetMatch(0.4, "L1Jets", "RecoJetsMatched");
        //matching efficiency both for reco jets
        //and for trgobj
        reco = ev.GetRecoJetsMatch();

        if(reco.size() > 4){
            std::cerr << "non è possibile qvesto" << std::endl;
            throw std::runtime_error("nope");
        }

        //plotting jet recostruction efficiency as a function of the ET of the offline jet
        if(reco.size() == 4){
            
            for(auto c : reco){
                if(c->matched){
                    if(c->eta < 3.0 && c->eta > -3.0){
                        h_L1->Fill(dr(c, c->MatchedObj));
                    }
                    else if((c->eta < 5.0 && c->eta > 3.0) || (c->eta > -5.0 && c->eta < -3.0) ){
                        h_L1_f->Fill(dr(c, c->MatchedObj));
                    }
                    eff_L1->Fill(true, c->pt);
                }
                else{
                    eff_L1->Fill(false, c->pt);
                }
            }

            for(auto c: reco){
                if(c->eta < 3.0 && c->eta > -3.0){
                    if(c->matched){
                        eff_L1_1->Fill(true, c->pt);
                    }
                    else{
                        eff_L1_1->Fill(false, c->pt);
                    }
                }

                if((c->eta < 5.0 && c->eta > 3.0) || (c->eta > -5.0 && c->eta < -3.0)){
                    if(c->matched){
                        eff_L1_2->Fill(true, c->pt);
                    }
                    else{
                        eff_L1_2->Fill(false, c->pt);
                    }
                }

            }
        }
    }

    gStyle->SetOptStat(0);

    //Grid

    //TLine* l1 = new TLine(20., 0., 20., 1.04 );
    //l1->SetLineStyle(kDashed);
    //l1->SetLineColorAlpha(kGray, 0.7);
    
    //TLine* l2 = new TLine(0., 0.5, 200., 0.5 );
    //l2->SetLineStyle(kDashed);
    //l2->SetLineColorAlpha(kGray, 0.8);
    
    TLine* l3 = new TLine(0., 1, 200., 1 );
    l3->SetLineStyle(kDashed);
    l3->SetLineColorAlpha(kGray, 0.7);
    

    TCanvas* c = new TCanvas("c", "c", 1000, 1000, 1000, 700);
    eff->SetTitle("; Offline Central PF Jet P_{T} (GeV); Efficiency");
    eff->Draw();
    eff_calo->Draw("same");
    eff_L1->Draw("same");
    //l1->Draw("same");
    //l2->Draw("same");
    TLatex T7 = TLatex();
    T7.DrawLatexNDC(.67, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}");
    TLatex T8 = TLatex();
    T8.DrawLatexNDC(.1, .92, "#scale[1.]{ #font[62]{CMS} #font[52]{Preliminary} }");
    //TLatex T9 = TLatex();
    //T9.DrawLatexNDC(.69, .38, "#scale[0.7]{#font[42]{#lbar #eta #lbar < 3.0}}");
    l3->Draw("same");
    leg->Draw();
    c->Draw();
    c->SaveAs("TotalJetsMatchingEfficiency.pdf");

    TCanvas* c5 = new TCanvas("c5", "c5", 1000, 1000, 1000, 700);
    eff_1->SetTitle("; Offline Central PF Jet P_{T} (GeV); Efficiency");
    eff_1->Draw();
    eff_calo_1->Draw("same");
    eff_L1_1->Draw("same");
    //l1->Draw("same");
    //l2->Draw("same");
    TLatex T10 = TLatex();
    T10.DrawLatexNDC(.67, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}");
    TLatex T11 = TLatex();
    T11.DrawLatexNDC(.1, .92, "#scale[1.]{ #font[62]{CMS} #font[52]{Preliminary} }");
    TLatex T12 = TLatex();
    T12.DrawLatexNDC(.69, .38, "#scale[0.7]{#font[42]{#lbar #eta #lbar < 3.0}}");
    l3->Draw("same");
    leg_1->Draw();
    c5->Draw();
    c5->SaveAs("CentralJetsMatchingEfficiency.pdf");

    TCanvas* c6 = new TCanvas("c6", "c6", 1000, 1000, 1000, 700);
    eff_2->SetTitle("; Offline Central PF Jet P_{T} (GeV); Efficiency");
    eff_2->Draw();
    eff_calo_2->Draw("same");
    eff_L1_2->Draw("same");
    //l1->Draw("same");
    //l2->Draw("same");
    TLatex T13 = TLatex();
    T13.DrawLatexNDC(.67, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}");
    TLatex T14 = TLatex();
    T14.DrawLatexNDC(.1, .92, "#scale[1.]{ #font[62]{CMS} #font[52]{Preliminary} }");
    TLatex T15 = TLatex();
    T15.DrawLatexNDC(.69, .38, "#scale[0.7]{#font[42]{3 < #lbar #eta #lbar < 5.0}}");
    l3->Draw("same");
    leg_2->Draw();
    c6->Draw();
    c6->SaveAs("ForwardJetsMatchingEfficiency.pdf");

    TCanvas* c2 = new TCanvas("c2", "c2", 1000, 1000, 1000, 700);
    h_pf->Scale(1./h_pf->Integral());
    h_calo->Scale(1./h_calo->Integral());
    h_L1->Scale(1./h_L1->Integral());
    h_pf->Draw("pe same");
    h_calo->Draw("pe same");
    h_L1->Draw("pe same");
    leg_c->Draw();
    TLatex T1 = TLatex();
    T1.DrawLatexNDC(.67, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}");
    TLatex T2 = TLatex();
    T2.DrawLatexNDC(.1, .92, "#scale[1.]{ #font[62]{CMS} #font[52]{Preliminary} }");
    TLatex T6 = TLatex();
    T6.DrawLatexNDC(.68, .85, "#scale[0.7]{#font[42]{#lbar #eta #lbar < 3.0}}");
    c2->Draw();
    c2->SetLogy();
    c2->SaveAs("./DrDistributionCentral.pdf");

    TCanvas* c3 = new TCanvas("c3", "c3", 1000, 1000, 1000, 700);
    h_pf_f->Scale(1./h_pf_f->Integral());
    h_calo_f->Scale(1./h_calo_f->Integral());
    h_L1_f->Scale(1./h_L1_f->Integral());
    h_pf_f->Draw("pe same");
    h_calo_f->Draw("pe same");
    h_L1_f->Draw("pe same");
    leg_f->Draw();
    TLatex T3 = TLatex();
    T3.DrawLatexNDC(.67, .92, "#scale[0.8]{#font[52]{ggF#rightarrow HH #rightarrow bbbb }}");
    TLatex T4 = TLatex();
    T4.DrawLatexNDC(.1, .92, "#scale[1.]{ #font[62]{CMS} #font[52]{Preliminary} }");
    TLatex T5 = TLatex();
    T5.DrawLatexNDC(.68, .85, "#scale[0.7]{#font[42]{3 < #lbar #eta #lbar < 5.0}}");
    c3->Draw();
    c3->SetLogy();
    c3->SaveAs("./DrDistributionForward.pdf");


    my_time = time(NULL); 
    printf("%s", ctime(&my_time));
   
    return 0;
}
