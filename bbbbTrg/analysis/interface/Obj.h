#ifndef OBJ_CLASS_H
#define OBJ_CLASS_H

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <functional>
#include <string>
#include "TLorentzVector.h"

namespace hltObj{

    class KinCuts{
        public:
            std::string Type;
            double MinPt = -1;
            double MinEt = -1;
            double MinEta = -99E99;
            double MaxEta = 99E99;
    };

    //It is expansive to build an object for each jet, it is far more economic
    //To work with collections. Jets->Collection, Jet->Single Object.
    class Jets{
        public:
            std::string Type = "Default";
            std::vector<double> mass;
            std::vector<double> e;
            std::vector<double> pt;
            std::vector<double> et;
            std::vector<double> eta;
            std::vector<double> phi;
            std::vector<double> btag;
            double HT = 0;
            Jets(){};
            Jets(std::vector<double> p, std::vector<double> e, std::vector<double> b) : pt(p), eta(e), btag(b) {};
            Jets(std::vector<double> p, std::vector<double> e, std::vector<double> b, double H) : pt(p), eta(e), btag(b), HT(H) {};
            Jets(double H) : HT(H) {};
            ~Jets(){};
            void print(){std::cout << "Pt: ";  for(auto c : pt ){ std:: cout << c << " "; }; std::cout << std::endl;
                        std::cout << "Eta: ";  for(auto c : eta ){ std:: cout << c << " "; }; std::cout << std::endl;
                        std::cout << "btag: ";  for(auto c : btag ){ std:: cout << c << " "; }; std::cout << std::endl;
                        std::cout << "et: ";  for(auto c : et ){ std:: cout << c << " "; }; std::cout << std::endl;
                        };
            int size(){return pt.size();};
            void clear(){ mass.clear(); e.clear(); pt.clear(); et.clear(); eta.clear(); phi.clear(); btag.clear(); HT = 0; };
            void clear(int index){ 
                if(pt.size() > 0) pt.erase(pt.begin()+index); 
                if(eta.size() > 0) eta.erase(eta.begin()+index); 
                if(phi.size() > 0) phi.erase(phi.begin()+index); 
                if(et.size() > 0) et.erase(et.begin()+index);
                if(mass.size() > 0) mass.erase(mass.begin()+index);
                if(e.size() > 0) e.erase(e.begin()+index);
                if(btag.size() > 0) btag.erase(btag.begin()+index);};

            void resize(int s){mass.resize(s); e.resize(s); pt.resize(s); et.resize(s); eta.resize(s); phi.resize(s); btag.resize(s);};
    };


    class BQuarks{
        public:
            std::string Type = "bquarkColl";
            std::vector<double>* e = 0;
            std::vector<double>* pt = 0;
            std::vector<double>* et = 0;
            std::vector<double>* eta = 0;
            std::vector<double>* phi = 0;
            BQuarks(){};
            BQuarks(std::vector<double>* p, std::vector<double>* e, std::vector<double>* ph) : pt(p), eta(e), phi(ph) {};
            ~BQuarks(){};
            int size(){return pt->size();};
            void clear(){ e->clear(); pt->clear(); et->clear(); eta->clear(); phi->clear(); };
    };

    class bQuark{
        public:
            std::string Type = "bquark";
            double e;
            double pt;
            double et;
            double eta;
            double phi;
            bQuark(){};
            bQuark(double p, double e, double ph) : pt(p), eta(e), phi(ph) {}; //minimal infos
            bQuark(double energy, double p, double tenergy,  double e, double ph) : e(energy), pt(p), et(tenergy), eta(e), phi(ph) {}; //complete infos
            ~bQuark(){};
            void clear(){ e = 0; pt = 0; et = 0; eta = 0; phi = 0; };
    };

    
    class Jet : public bQuark{
        public: 
            std::string Type = "Jet";
            double mass;
            double btag;
            bool bmatched = false;
            bool matched = false;
            hltObj::Jet* MatchedObj;
            hltObj::bQuark* MatchedB;
            Jet(double p, double e, double ph) : bQuark(p, e, ph) {}; //minimal infos
            Jet(double energy, double p, double tenergy,  double e, double ph) : bQuark(energy, p, tenergy, e, ph) {}; //complete infos
            ~Jet(){};
            void clear(){ mass = 0; e = 0; pt = 0; et = 0; eta = 0; phi = 0; btag = 0;};
    };
    

    class Higgs{
        public:
            std::string Type = "Higgs"; 
            double gen_costh_H_cm;
            double gen_H_m;
            double gen_H_pt;
            double gen_H_eta;
            double gen_H_phi;
            TLorentzVector* gen_H_p4 = 0;
            Higgs(){};
            Higgs(double m, double pt, double eta, double phi, TLorentzVector* p4) : gen_H_m(m), gen_H_pt(pt), gen_H_eta(eta), gen_H_phi(phi), gen_H_p4(p4) {};
            ~Higgs(){};
            double GetMHH(Higgs Hbro){
                TLorentzVector vSum =   *gen_H_p4 + *Hbro.gen_H_p4;
                double gen_mHH          = vSum.M();
                return gen_mHH;
            }
            void SetCos(Higgs Hbro){
                TLorentzVector vSum =   *gen_H_p4 + *Hbro.gen_H_p4;
                // boost to CM 
                TLorentzVector vH1_cm = *gen_H_p4;
                TLorentzVector vH2_cm = *Hbro.gen_H_p4;
                vH1_cm.Boost(-vSum.BoostVector());
                vH2_cm.Boost(-vSum.BoostVector());

                gen_costh_H_cm  = vH1_cm.CosTheta();
                Hbro.gen_costh_H_cm  = vH2_cm.CosTheta();
                return;
            }

    };

    class HLTCuts{

        public:
            double PtMin;
            double PtMax;
            double EtaMin;
            double EtaMax;
            double HTMin;
            double MHTMin;
            bool ordered;
            int MinN;
            std::vector<int> MinNs;
            std::vector<double> PtMinEachJet;
            std::vector<double> PtMaxEachJet;
            std::vector<double> EtaMinEachJet;
            std::vector<double> EtaMaxEachJet;
            int size_for_each;
            typedef void(HLTCuts::*M)(double);
            std::map<std::string, M> MappedSetCuts;
            HLTCuts() : PtMin(-1), PtMax(1.0E99), EtaMin(-1.0E99), EtaMax(1.0E99), HTMin(0), MHTMin(0), MinN(1) { SetMap(); };
            HLTCuts(double PtMin_, double PtMax_, double EtaMin_, double EtaMax_) : PtMin(PtMin_), PtMax(PtMax_), EtaMin(EtaMin_), EtaMax(EtaMax_), MinN(1) { SetMap(); };
            HLTCuts(double PtMin_, double PtMax_, double EtaMin_, double EtaMax_, int MinN_) : PtMin(PtMin_), PtMax(PtMax_), EtaMin(EtaMin_), EtaMax(EtaMax_), MinN(MinN_){ SetMap(); };
            HLTCuts(double PtMin_, double PtMax_, double EtaMin_, double EtaMax_, double HTMin_, double MHTMin_, int MinN_) : PtMin(PtMin_), PtMax(PtMax_), EtaMin(EtaMin_), EtaMax(EtaMax_), HTMin(HTMin_), MHTMin(MHTMin_), MinN(MinN_) { SetMap(); };
            HLTCuts(double HTMin_) : HTMin(HTMin_) { SetMap(); };
            HLTCuts(std::vector<double> PtMinEachJet_, std::vector<double> PtMaxEachJet_, std::vector<double> EtaMinEachJet_, std::vector<double> EtaMaxEachJet_) : PtMinEachJet(PtMinEachJet_), PtMaxEachJet(PtMaxEachJet_), EtaMinEachJet(EtaMinEachJet_), EtaMaxEachJet(EtaMaxEachJet_) {std::vector<int> to_min(PtMinEachJet_.size(), 1); MinNs = to_min;  SetMap(); };
            HLTCuts(std::vector<double> PtMinEachJet_, std::vector<double> PtMaxEachJet_, std::vector<double> EtaMinEachJet_, std::vector<double> EtaMaxEachJet_, double HTMin_, double MHTMin_) : PtMinEachJet(PtMinEachJet_), PtMaxEachJet(PtMaxEachJet_), EtaMinEachJet(EtaMinEachJet_), EtaMaxEachJet(EtaMaxEachJet_), HTMin(HTMin_), MHTMin(MHTMin_) {std::vector<int> to_min(PtMinEachJet_.size(), 1); MinNs = to_min;  SetMap(); };
            ~HLTCuts(){};
            void SetMap(){
                MappedSetCuts.insert(std::make_pair("setPtMin", &HLTCuts::setPtMin));
                MappedSetCuts.insert(std::make_pair("setPtMax", &HLTCuts::setPtMax));
                MappedSetCuts.insert(std::make_pair("setEtaMin", &HLTCuts::setEtaMin));
                MappedSetCuts.insert(std::make_pair("setEtaMax", &HLTCuts::setEtaMax));
                MappedSetCuts.insert(std::make_pair("setHTMin", &HLTCuts::setHTMin));
                MappedSetCuts.insert(std::make_pair("setMHTMin", &HLTCuts::setMHTMin));
            }
            void setPtMin(double p){PtMin = p; };
            void setPtMax(double p){PtMax = p;};
            void setEtaMin(double e){EtaMin = e;};
            void setEtaMax(double e){EtaMax = e;};
            void setHTMin(double H){HTMin = H;};
            void setMHTMin(double H){MHTMin=H;};
            void setOrdered(bool o){ordered=o;};
            void setMinN(int N){MinN=N;};


    };


}
#endif