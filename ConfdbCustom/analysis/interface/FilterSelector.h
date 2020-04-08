#ifndef FILTER_SELECTOR_H
#define FILTER_SELECTOR_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <functional>
#include <numeric>
#include <map>
#include "Obj.h"

namespace cstmfltr{

    
    int hltQuadCentralJet45(std::vector<double> pt, std::vector<double> eta){

        std::vector<size_t> results;

        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i > 45.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.6)
                results.emplace_back(dis);
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i > 45.0;});
        }

        return results.size();
    };

    int hltQuadPFCentralJetLooseID45(std::vector<double> pt, std::vector<double> eta){

        std::vector<size_t> results;        


        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i > 45.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.6)
                results.emplace_back(dis);
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i > 45.0;});
        }

        return results.size();
    };


    int hltBTagCaloCSVp087Triple(std::vector<int> btag){
        auto count = std::count_if(btag.begin(), btag.end(),[&](int const& val){ return val > 0.; }); 
        return count;
    };

    int hltQuadCentralJet30(std::vector<double> pt, std::vector<double> eta){

        std::vector<size_t> results;

        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 30.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                results.emplace_back(dis);
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >= 30;});
        }   

        return results.size();
    };

    int hltCaloQuadJet30HT320(std::vector<double> pt, std::vector<double> et, std::vector<double> eta){

        
        std::vector<double> et_final;
        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 30.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                et_final.emplace_back(et.at(dis));
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >= 30.0;});
        }

        if(et_final.size() >= 4){
            if(std::accumulate(et_final.begin(), et_final.end(), 0.) >= 320.0)
                return 1;
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
        

    };


    int hltBTagCaloDeepCSVp17Double(std::vector<int> btag, std::vector<double> pt, std::vector<double> eta){

        //std::cout << "calo pt: " << pt.size() << " btag: " << btag.size() << std::endl;
        for(int j = 0; j < btag.size(); j++){
            if(pt.at(j) < 30.0 || abs(eta.at(j)) > 2.4){
                btag.erase(btag.begin()+j);
                pt.erase(pt.begin()+j);
                eta.erase(eta.begin()+j);
            }
        }

        std::vector<std::pair<double, int> > pt_btag;
        pt_btag.reserve(pt.size());
        std::transform(pt.begin(), pt.end(), btag.begin(), std::back_inserter(pt_btag), [](double a, int b) { return std::make_pair(a, b); });
        std::sort(pt_btag.begin(), pt_btag.end(), [](std::pair<double, int> a, std::pair<double,int> b ){return a.first > b.first;});

        std::vector<std::pair<double, int>> to_count(pt_btag.begin(), pt_btag.begin()+8);
        auto count = std::count_if(to_count.begin(), to_count.end(),[](std::pair<double, int> const& val){ return val.second > 0; }); 
        //auto count = std::count_if(pt_btag.begin(), pt_btag.end(),[](std::pair<double, int> const& val){ return val.second > 0; }); 
        return count;
    };

    int hltPFCentralJetLooseIDQuad30(std::vector<double> pt, std::vector<double> eta){

        std::vector<size_t> results;

        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 30.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                results.emplace_back(dis);
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >= 30.0;});
        }

        return results.size();
    };

    int hlt1PFCentralJetLooseID75(std::vector<double> pt, std::vector<double> eta){
        
        std::vector<size_t> results;

        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 75.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                results.emplace_back(dis);
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >= 75.0;});
        }

        return results.size();
    };

    int hlt2PFCentralJetLooseID60(std::vector<double> pt, std::vector<double> eta){

        std::vector<size_t> results;

        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 60.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                results.emplace_back(dis);
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >= 60.0;});
        }

        return results.size();
    };

    int hlt3PFCentralJetLooseID45(std::vector<double> pt, std::vector<double> eta){

        std::vector<size_t> results;

        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 45.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                results.emplace_back(dis);
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >= 45.0;});
        }

        return results.size();

    };

    int hlt4PFCentralJetLooseID40(std::vector<double> pt, std::vector<double> eta){

        std::vector<size_t> results;

        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 40.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                results.emplace_back(dis);
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >=40.0;});
        }

        return results.size();

    };

    int hltPFCentralJetsLooseIDQuad30HT330(std::vector<double> pt, std::vector<double> et, std::vector<double> eta){

        
        std::vector<double> et_final;
        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 30.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                et_final.emplace_back(et.at(dis));
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >= 30.0;});
        }

        if(et_final.size() >= 4){
            if(std::accumulate(et_final.begin(), et_final.end(), 0.) >= 330.0)
                return 1;
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
        

    };


    int hltBTagPFDeepCSV4p5Triple(std::vector<int> btag, std::vector<double> pt, std::vector<double> eta){
        //std::cout << "calo pt: " << pt.size() << " btag: " << btag.size() << std::endl;
        for(int j = 0; j < btag.size(); j++){
            if(pt.at(j) < 30.0 || abs(eta.at(j)) > 2.6){
                btag.erase(btag.begin()+j);
                pt.erase(pt.begin()+j);
                eta.erase(eta.begin()+j);
            }
        }

        std::vector<std::pair<double, int> > pt_btag;
        pt_btag.reserve(pt.size());
        std::transform(pt.begin(), pt.end(), btag.begin(), std::back_inserter(pt_btag), [](double a, int b) { return std::make_pair(a, b); });
        std::sort(pt_btag.begin(), pt_btag.end(), [](std::pair<double, int> a, std::pair<double,int> b ){return a.first > b.first;});

        std::vector<std::pair<double, int>> to_count(pt_btag.begin(), pt_btag.end());
        auto count = std::count_if(to_count.begin(), to_count.end(),[](std::pair<double, int> const& val){ return val.second > 0.0; }); 
        return count;
    };

    int hltL1sQuadJetC50to60IorHTT280to500IorHTT250to340QuadJet(std::vector<double> pt, std::vector<double> eta, double HT){
        std::vector<bool> results;
        bool L1_HTT280er = HT >= 280.0;
        bool L1_HTT320er = HT >= 320.0;
        bool L1_HTT360er = HT >= 360.0;
        bool L1_HTT400er = HT >= 400.0;
        bool L1_HTT450er = HT >= 450.0;
        bool L1_QuadJet60er2p5;
        bool L1_HTT280er_QuadJet_70_55_40_35_er2p4;
        bool L1_HTT320er_QuadJet_70_55_40_40_er2p4;
        bool L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3 ;
        bool L1_HTT320er_QuadJet_80_60_er2p1_50_45_er2p3;

    
        std::vector<size_t> L1_QuadJet60er2p5_vec;

        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 60.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                L1_QuadJet60er2p5_vec.emplace_back(dis);
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >=60.0;});
        }

        L1_QuadJet60er2p5 = L1_QuadJet60er2p5_vec.size() >= 4;


        if(!L1_HTT280er){
            L1_HTT280er_QuadJet_70_55_40_35_er2p4 = false;
            L1_HTT320er_QuadJet_70_55_40_40_er2p4 = false;
            L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3 = false;
            L1_HTT320er_QuadJet_80_60_er2p1_50_45_er2p3 = false;
        }
        else if(L1_HTT280er && !L1_HTT320er){
            L1_HTT320er_QuadJet_70_55_40_40_er2p4 = false;
            L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3 = false;
            L1_HTT320er_QuadJet_80_60_er2p1_50_45_er2p3 = false;

            if(pt.size() >= 4 && eta.size() >= 4 ){

                std::vector<std::pair<double, double> > pt_eta;
                std::vector<double> pt_cut = {70.0,55.0,40.0,35.0};
                double eta_cut = 2.4;

                pt_eta.reserve(pt.size());
                std::transform(pt.begin(), pt.end(), eta.begin(), std::back_inserter(pt_eta), [](int a, int b) { return std::make_pair(a, b); });
                std::sort(pt_eta.begin(), pt_eta.end(), [](std::pair<double, double> a, std::pair<double,double> b ){return a.first > b.first;});

                std::vector<bool> HT280vec;
                for(int j = 0; j < 4; j++){
                    if(pt_eta.at(j).first >=pt_cut.at(j) && abs(pt_eta.at(j).second) < eta_cut )
                        HT280vec.push_back(true);
                    else
                        HT280vec.push_back(false);
                }

                if(std::all_of(HT280vec.begin(), HT280vec.end(), [](bool i ){return i == true; }))
                    L1_HTT280er_QuadJet_70_55_40_35_er2p4 = true;
                else
                    L1_HTT280er_QuadJet_70_55_40_35_er2p4 = false;
            }
            else{
                L1_HTT280er_QuadJet_70_55_40_35_er2p4 = false;
            }

        }
        else if(L1_HTT320er){
            if(pt.size() >= 4 && eta.size() >= 4 ){
                std::vector<std::pair<double, double> > pt_eta;
                std::vector<double> pt_1_cut = {70,55,40,35};
                std::vector<double> pt_2_cut = {70,55,40,40};
                std::vector<double> pt_3_cut = {80,60,45,40};
                std::vector<double> pt_4_cut = {80,60,50,45};
                double eta_1_cut = 2.4;
                double eta_2_cut = 2.1;
                double eta_3_cut = 2.3;

                pt_eta.reserve(pt.size());
                std::transform(pt.begin(), pt.end(), eta.begin(), std::back_inserter(pt_eta), [](int a, int b) { return std::make_pair(a, b); });
                std::sort(pt_eta.begin(), pt_eta.end(), [](std::pair<double, double> a, std::pair<double,double> b ){return a.first > b.first;});

                std::vector<bool> HT280vec;
                for(int j = 0; j < 4; j++){
                    if(pt_eta.at(j).first >=pt_1_cut.at(j) && abs(pt_eta.at(j).second) < eta_1_cut )
                        HT280vec.push_back(true);
                    else
                        HT280vec.push_back(false);
                }

                std::vector<bool> HT320vec1;
                for(int j = 0; j < 2; j++){
                    if(pt_eta.at(j).first >=pt_2_cut.at(j) && abs(pt_eta.at(j).second) < eta_1_cut )
                        HT320vec1.push_back(true);
                    else
                        HT320vec1.push_back(false);
                }

                std::vector<bool> HT320vec2;
                for(int j = 0; j < 2; j++){
                    if(pt_eta.at(j).first >=pt_3_cut.at(j) && abs(pt_eta.at(j).second) < eta_2_cut )
                        HT320vec2.push_back(true);
                    else
                        HT320vec2.push_back(false);
                }
                for(int j = 2; j < 4; j++){
                    if(pt_eta.at(j).first >=pt_3_cut.at(j) && abs(pt_eta.at(j).second) < eta_3_cut )
                        HT320vec2.push_back(true);
                    else
                        HT320vec2.push_back(false);
                }

                std::vector<bool> HT320vec3;
                for(int j = 0; j < 2; j++){
                    if(pt_eta.at(j).first >=pt_4_cut.at(j) && abs(pt_eta.at(j).second) < eta_2_cut )
                        HT320vec3.push_back(true);
                    else
                        HT320vec3.push_back(false);
                }
                for(int j = 2; j < 4; j++){
                    if(pt_eta.at(j).first >=pt_4_cut.at(j) && abs(pt_eta.at(j).second) < eta_3_cut )
                        HT320vec3.push_back(true);
                    else
                        HT320vec3.push_back(false);
                }



                if(std::all_of(HT280vec.begin(), HT280vec.end(), [](bool i ){return i == true; }))
                    L1_HTT280er_QuadJet_70_55_40_35_er2p4 = true;
                else
                    L1_HTT280er_QuadJet_70_55_40_35_er2p4 = false;

                if(std::all_of(HT320vec1.begin(), HT320vec1.end(), [](bool i ){return i == true; }))
                    L1_HTT320er_QuadJet_70_55_40_40_er2p4 = true;
                else
                    L1_HTT320er_QuadJet_70_55_40_40_er2p4 = false;

                if(std::all_of(HT320vec2.begin(), HT320vec2.end(), [](bool i ){return i == true; }))
                    L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3 = true;
                else
                    L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3 = false;

                if(std::all_of(HT320vec3.begin(), HT320vec3.end(), [](bool i ){return i == true; }))
                    L1_HTT320er_QuadJet_80_60_er2p1_50_45_er2p3 = true;
                else
                    L1_HTT320er_QuadJet_80_60_er2p1_50_45_er2p3 = false;
                }
            else{
                L1_HTT280er_QuadJet_70_55_40_35_er2p4 = false;
                L1_HTT320er_QuadJet_70_55_40_40_er2p4 = false;
                L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3 = false;
                L1_HTT320er_QuadJet_80_60_er2p1_50_45_er2p3 = false;

            }
        }

        results.push_back(L1_HTT280er);
        results.push_back(L1_HTT320er);
        results.push_back(L1_HTT360er);
        results.push_back(L1_HTT400er);
        results.push_back(L1_HTT450er);
        results.push_back(L1_QuadJet60er2p5);
        results.push_back(L1_HTT280er_QuadJet_70_55_40_35_er2p4);
        results.push_back(L1_HTT320er_QuadJet_70_55_40_40_er2p4);
        results.push_back(L1_HTT320er_QuadJet_80_60_er2p1_45_40_er2p3);
        results.push_back(L1_HTT320er_QuadJet_80_60_er2p1_50_45_er2p3);

        if ( std::any_of(results.begin(), results.end(), [](bool i){return i == true;}) )
            return 1;
        else 
            return 0;
    };




//---------------JUST FOR CHECK ON HT----------------------
double hltCaloQuadJet30HT320_ret_HT(std::vector<double> pt, std::vector<double> et, std::vector<double> eta){

        
        std::vector<double> et_final;
        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 30.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                et_final.emplace_back(et.at(dis));
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >= 30.0;});
        }

        if(et_final.size() >= 4){
            return std::accumulate(et_final.begin(), et_final.end(), 0.);
        }
        else{
            return 0;
        }
    };

    double hltCaloQuadJet30HT320_ret_HT_copy(std::vector<double> pt, std::vector<double> et, std::vector<double> eta){

        
        std::vector<double> et_final;
        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 30.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                et_final.emplace_back(et.at(dis));
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >= 30.0;});
        }

        if(et_final.size() >= 4){
            return 1;
        }
        else{
            return 0;
        }
    };

    int hltCaloQuadJet30HT320_copy(double HT){
        if(HT > 320.0) return 1;
        else return 0;
    };



    int hltPFCentralJetsLooseIDQuad30HT330_copy(double HT){
        if(HT > 330.0) return 1;
        else return 0;
    };

    double hltPFCentralJetsLooseIDQuad30HT330_ret_HT(std::vector<double> pt, std::vector<double> et, std::vector<double> eta){

        
        std::vector<double> et_final;
        auto it = std::find_if(std::begin(pt), std::end(pt), [](double i){return i >= 30.0;});
        while (it != std::end(pt)) {
            auto dis = std::distance(std::begin(pt), it);
            if(abs(eta.at(dis)) <= 2.5)
                et_final.emplace_back(et.at(dis));
            it = std::find_if(std::next(it), std::end(pt), [](double i){return i >= 30.0;});
        }
        if(et_final.size() >= 4)
            return std::accumulate(et_final.begin(), et_final.end(), 0.);
        else
            return 0;
        
    };

    int hltBTagPFDeepCSV4p5Triple_copy(std::vector<int> btags){
        auto count = std::count_if(btags.begin(), btags.end(), [](int const& i){return i > 0.0;});
        return count;
    };

    int hltBTagCaloDeepCSVp17Double_copy(std::vector<int> btags){
        auto count = std::count_if(btags.begin(), btags.end(), [](int const& i){return i > 0.0;});
        return count;
    };



    //-----------------------------------------------------------------------------------
    
    template<typename T, typename U>
    bool check_length(std::vector<T> vec, std::vector<U> other_vec){
        bool pass = true;
        if(vec.size() != other_vec.size())
            pass = false;
        return pass;
    };

    bool CustomFixedPtEtaSelector(hltObj::Jets jets, hltObj::HLTCuts cuts){

        bool pass = true;
        if(!(check_length<double, double>(jets.pt, jets.eta) || check_length<double, double>(jets.eta, jets.btag))){
            throw std::runtime_error("--->Size does not Coincide<---");
        }

        int count = 0;
        //local copy for lambda, can we actually avoid this?
        double PtMin = cuts.PtMin;
        double PtMax = cuts.PtMax;

        auto it = std::find_if(std::begin(jets.pt), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
        while (it != std::end(jets.pt)) {
            auto dis = std::distance(std::begin(jets.pt), it);
            if(jets.eta.at(dis) <= cuts.EtaMax && jets.eta.at(dis) >= cuts.EtaMin)
                count++;            
            it = std::find_if(std::next(it), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
        }   

        return pass && count >= cuts.MinN;

    };

    bool CustomVariablePtEtaSelector(hltObj::Jets jets, hltObj::HLTCuts cuts){

        
        bool pass = true;
        if(!(check_length<double, double>(jets.pt, jets.eta) || check_length<double, double>(jets.eta, jets.btag) || check_length<double, int>(jets.pt, cuts.MinNs))){
            throw std::runtime_error("--->Size does not Coincide<---");
        }

        for(int i = 0; i < cuts.PtMinEachJet.size(); i++){
            //local copy for lambda, can we actually avoid this?
            double PtMin = cuts.PtMinEachJet.at(i);
            double PtMax = cuts.PtMaxEachJet.at(i);
            int count = 0;
            auto it = std::find_if(std::begin(jets.pt), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
            while (it != std::end(jets.pt)) {
                auto dis = std::distance(std::begin(jets.pt), it);
                if(jets.eta.at(dis) <= cuts.EtaMax && jets.eta.at(dis) >= cuts.EtaMin)
                    count++;
                it = std::find_if(std::next(it), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
            } 
            pass = pass && count >= cuts.MinNs.at(i);
        }  

        return pass;

   };

    bool CustomFixedPtEtaHTSelector(hltObj::Jets jets, hltObj::HLTCuts cuts){
        
        if(!(check_length<double, double>(jets.pt, jets.eta) || check_length<double, double>(jets.eta, jets.btag))){
            throw std::runtime_error("--->Size does not Coincide<---");
        }

        bool pass = true;
        std::vector<double> results;
        //local copy for lambda, can we actually avoid this?
        double PtMin = cuts.PtMin;
        double PtMax = cuts.PtMax;

        auto it = std::find_if(std::begin(jets.pt), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
        while (it != std::end(jets.pt)) {
            auto dis = std::distance(std::begin(jets.pt), it);
            if(jets.eta.at(dis) <= cuts.EtaMax && jets.eta.at(dis) >= cuts.EtaMin)
                results.emplace_back(jets.pt.at(dis));
            it = std::find_if(std::next(it), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
        }   
        if(results.size() < cuts.MinN){
            return false;
        }
        else{
            return pass && std::accumulate(results.begin(), results.end(), 0.) >= cuts.HTMin;
        }

    };

    bool CustomVariablePtEtaHTSelector(hltObj::Jets jets, hltObj::HLTCuts cuts){

        std::vector<double> results;
        bool pass = true;
        if(!(check_length<double, double>(jets.pt, jets.eta) || check_length<double, double>(jets.eta, jets.btag) || check_length<double, int>(jets.pt, cuts.MinNs))){
            throw std::runtime_error("--->Size does not Coincide<---");
        }

        for(int i = 0; i < cuts.PtMinEachJet.size(); i++){
            //local copy for lambda, can we actually avoid this?
            double PtMin = cuts.PtMinEachJet.at(i);
            double PtMax = cuts.PtMaxEachJet.at(i);
            int count = 0;
            auto it = std::find_if(std::begin(jets.pt), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
            while (it != std::end(jets.pt)) {
                auto dis = std::distance(std::begin(jets.pt), it);
                if(jets.eta.at(dis) <= cuts.EtaMaxEachJet.at(i) && jets.eta.at(dis) >= cuts.EtaMinEachJet.at(i)){
                    results.push_back(jets.pt.at(dis));
                    count++;
                }
                it = std::find_if(std::next(it), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
            } 
            pass = pass && count >= cuts.MinNs.at(i);
        }  
        
        if(pass && jets.HT >= cuts.HTMin){
            return true;
        }
        else{
            return false;
        }

   };

   bool CustomFixedBtagSelector(hltObj::Jets jets, hltObj::HLTCuts cuts){
        
        if(!(check_length<double, double>(jets.pt, jets.eta) || check_length<double, double>(jets.eta, jets.btag))){
            throw std::runtime_error("--->Size does not Coincide<---");
        }

        bool pass = true;
        std::vector<double> results;
        //local copy for lambda, can we actually avoid this?
        double PtMin = cuts.PtMin;
        double PtMax = cuts.PtMax;

        auto it = std::find_if(std::begin(jets.pt), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
        while (it != std::end(jets.pt)) {
            auto dis = std::distance(std::begin(jets.pt), it);
            if(jets.eta.at(dis) <= cuts.EtaMax && jets.eta.at(dis) >= cuts.EtaMin && jets.btag.at(dis) >= cuts.BtagMin)
                results.emplace_back(jets.pt.at(dis));
            it = std::find_if(std::next(it), std::end(jets.pt), [PtMin, PtMax](double i){return (i >= PtMin && i <= PtMax);});
        }   
        if(results.size() < cuts.MinN){
            return false;
        }
        else{
            return pass;
        }

    };

    bool CustomFixedHTSelector(hltObj::Jets jets, hltObj::HLTCuts cuts){

        if(jets.HT >= cuts.HTMin){
            return true;
        }
        else{
            return false;
        }
    };
    
    /*
    std::map<std::string, std::function<bool(hltObj::Jets, hltObj::HLTCuts)>> FilterMap = {

       std::make_pair("CustomFixedPtEtaSelector", CustomFixedPtEtaSelector),
       std::make_pair("CustomVariablePtEtaSelector", CustomVariablePtEtaSelector),
       std::make_pair("CustomFixedPtEtaHTSelector", CustomFixedPtEtaHTSelector),
       std::make_pair("CustomFixedHTSelector", CustomFixedHTSelector),
       std::make_pair("CustomVariablePtEtaHTSelector", CustomVariablePtEtaHTSelector),
       
    };
    
    
    bool CustomL1Trigger(std::vector<std::string> FunNames, std::vector<hltObj::HLTCuts> cuts, hltObj::Jets jets){
        
        bool pass = false;
        for(int i = 0; i < FunNames.size(); i++){
            pass = pass || FilterMap[FunNames.at(i)](jets, cuts.at(i));
            if(pass == true)
                return true;
        }

        return pass;
    };
    

    */
};


#endif
