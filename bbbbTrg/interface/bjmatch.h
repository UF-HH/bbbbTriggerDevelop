#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenStatusFlags.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace edm;
using namespace reco;
using namespace std;


namespace Jet_Match{

    inline std::vector<std::pair<pat::Jet,int>> b_jet_match(std::vector<reco::GenParticle> b, std::vector<pat::Jet> jets){

        //b matching with jets. Match only if deltaR < 0.4 between jets and b quark.
        std::vector<std::pair<pat::Jet, int> >  match_jets;
        match_jets.resize(b.size());
        std::vector<reco::GenParticle> b_copy = b;
        
        //creating a map for b and index
        std::vector<std::pair<reco::GenParticle, int>> b_pair;
        std::vector<std::pair<pat::Jet, int>> jet_pair;

        for(uint i = 0; i < b.size(); i++){
            b_pair.push_back(std::pair<reco::GenParticle, int>(b.at(i), i));
        }

        for(uint i = 0; i < jets.size(); i++){
            jet_pair.push_back(std::pair<pat::Jet, int>(jets.at(i), i));
        }

        while(b_pair.size() != 0){

            std::vector<int> b_ind;
            std::vector<int> jet_ind;

            std::vector<double> dR_b_j;

            for(uint b_id = 0; b_id < b_pair.size(); b_id++){
                for(uint j_id = 0; j_id < jet_pair.size(); j_id++){
                    
                    double dR = sqrt( pow(b_pair.at(b_id).first.eta()-jet_pair.at(j_id).first.eta(),2) + pow(b_pair.at(b_id).first.phi()-jet_pair.at(j_id).first.phi(),2));
                    dR_b_j.push_back(dR);
                    b_ind.push_back(b_id);
                    jet_ind.push_back(j_id);
                }    

            }

            int min_index = std::min_element(dR_b_j.begin(),dR_b_j.end()) - dR_b_j.begin();

            if(dR_b_j.at(min_index) <= 0.4){
                int b_index = b_pair.at(b_ind.at(min_index)).second; //true index of b quark 
                int j_index = jet_pair.at(jet_ind.at(min_index)).second;

                match_jets.at(b_index) = std::pair<pat::Jet, int>(jets.at(j_index), b_index);

                b_pair.erase(b_pair.begin() + b_ind.at(min_index));
                jet_pair.erase(jet_pair.begin() + jet_ind.at(min_index));
            }

            else{
                pat::Jet fake;
                int b_index = b_pair.at(b_ind.at(min_index)).second; //true index of b quark 
                match_jets.at(b_index) = std::pair<pat::Jet, int>(fake, -999);
                b_pair.erase(b_pair.begin() + b_ind.at(min_index));
            }
        }

        return match_jets;
    };

}