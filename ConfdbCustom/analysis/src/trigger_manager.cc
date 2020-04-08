#include "../interface/trigger_manager.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream> 

man_filter::man_filter(std::vector<std::string> f_n){
    filter_names_ = f_n;
};

man_filter::man_filter(std::vector<std::string> f_n, std::vector<int> f_val){
    filter_names_ = f_n;
    filter_th = f_val;

    save_cum.resize(filter_th.size(), 0);

};

man_filter::man_filter(std::vector<std::pair<std::string,int>> filter){
    filter_map = filter;
    for(auto c: filter){
        filter_names_.push_back(c.first);
        filter_th.push_back(c.second);
    }
    save_cum.resize(filter_th.size(), 0);
};

man_filter::man_filter(std::vector<std::pair<std::string,int>> filter, std::string trigger_name_){
    filter_map = filter;
    trig_n = trigger_name_;
        for(auto c: filter){
        filter_names_.push_back(c.first);
        filter_th.push_back(c.second);
    }
    save_cum.resize(filter_th.size(), 0);
};

void man_filter::initialize_f_threshold(std::vector<int> f_val){
    filter_th = f_val;
    save_cum.resize(filter_th.size(), 0);
};

void man_filter::initialize_f_names(std::vector<std::string> f_names){
    filter_names_ = f_names;
};

void man_filter::set_OR(std::vector<int>OR_conn){
    OR_connections = OR_conn;
    save_cum.resize(filter_th.size()-OR_connections.size()+1, 0);
}

void man_filter::set_trigger_name(std::string trigger_name_){
    trig_n = trigger_name_;
};

void man_filter::initialize_filters_id(){

    filters_index.resize(filter_th.size());
    for(int i = 0; i < filter_th.size(); i++){
        filters_index.at(i) = i;
    }

    return;

}

void man_filter::add_to_map(std::string to_add, int th){

    filter_map.push_back(std::pair<std::string,int>(to_add,th));
    filter_names_.push_back(to_add);
    filter_th.push_back(th);
    initialize_filters_id();

    return;
}

std::vector<int> man_filter::compute_or(std::vector<int> results){
    
    auto min = std::min(OR_connections.at(0), OR_connections.at(1));
    if(event_count.at(OR_connections.at(0)) >= filter_th.at(OR_connections.at(0)) || event_count.at(OR_connections.at(1)) >= filter_th.at(OR_connections.at(1))){
        results.at(min) = 1;
    }

    
    int scaler = 0;
    for(int i : OR_connections){
        filters_index.erase(filters_index.begin() + i - scaler);
        scaler ++;

    }
    
    return results;
}

std::vector<int> man_filter::clear_or(std::vector<int> results){


    auto max = std::max(OR_connections.at(0), OR_connections.at(1));
    results.erase(results.begin() + max);
    return results;
}

void man_filter::clear_save(){
    std::fill(save_cum.begin(), save_cum.end(), 0);
    return;
}



std::vector<int> man_filter::filter_test(std::vector<int> counts_){

    initialize_filters_id();
    event_count = counts_;
    std::vector<int> results(filter_th.size(), 0);

    if(filter_map.size() == 0 && filter_th.size() == 0){
        std::cout << "Initialize filter map with names and threshold values" << std::endl;
        return results;
    }

    //computing or and update filters and counts 
    if (OR_connections.size() != 0){
        results = compute_or(results);
    }

    for( int id_fil : filters_index){
        if(counts_.at(id_fil) >= filter_th.at(id_fil)){
            results.at(id_fil) = 1;
        }
    }

    if (OR_connections.size() != 0){
        results = clear_or(results);
    }
    
    event_pass_vector = results;

    if (std::find(results.begin(), results.end(), 0) != results.end())
        passed_trigger = false;
    else
        passed_trigger = true;


    return results;

}

void man_filter::print(){
    for(int c : filter_th ){
        std::cout << c << std::endl;
    }

    return;
}

void man_filter::print_map(){
    std::cout << "[INFO]: Trigger: " << trig_n << std::endl;
    for(auto c : filter_map){
        std::cout << "Filter: " << c.first << " " << "Threshold: " << c.second << std::endl;
    }

    if(OR_connections.size() != 0){
        std::cout<< "OR: " << filter_map.at(OR_connections.at(0)).first  << " " << filter_map.at(OR_connections.at(1)).first << std::endl;
    }
}

int man_filter::verify_filter_stop(){

    for(int i = 0; i < event_pass_vector.size(); i++){
        if(event_pass_vector.at(i) == 0){
            return i;
        }
    }

    return -1;
}

std::vector<int> man_filter::get_relative_eff(){

    std::vector<int> rel_eff(event_pass_vector.size(), 0);

    if(event_pass_vector.at(0) == 1)
        rel_eff.at(0) = 1;
    
    for(int i = 1; i < event_pass_vector.size(); i++){
        if(event_pass_vector.at(i-1) == 1 && event_pass_vector.at(i) == 1){
            rel_eff.at(i) = 1;

        }
    }

    return rel_eff;

}

std::vector<int> man_filter::get_cumulative_eff(){

    std::vector<int> cum_eff(event_pass_vector.size(), 0);
    int first_zero = verify_filter_stop();

    if (first_zero==-1)
        first_zero = cum_eff.size();

    for(int i = 0; i < first_zero; i++){
            cum_eff.at(i) = 1;
    }

    return cum_eff;

}

std::vector<int> man_filter::f_test_suppose_sequential(std::vector<int> counts_, bool save, double weight){

    initialize_filters_id();
    auto results = filter_test(counts_);
    int stop = verify_filter_stop();

    for(int i = stop; i < results.size(); i++){
        results.at(i) = 0;
    }

    event_pass_vector = results;    
    std::vector<double> weight_comp(event_pass_vector.size());

    if(save){
        for(int j = 0; j < event_pass_vector.size(); j++){
            weight_comp.at(j) = (double) event_pass_vector.at(j)*weight;
        }
        std::transform (save_cum.begin(), save_cum.end(), weight_comp.begin(), save_cum.begin(), std::plus<double>());
    }

    return results;

}

std::vector<std::string> man_filter::get_fil_names(){

    std::vector<std::string> names_;
    std::vector<int> or_positions;

    if(OR_connections.size() == 0){
        return filter_names_;
    }
    else{
         
        for(auto i : OR_connections){
            or_positions.push_back(i);
        }
         

        std::sort(or_positions.begin(), or_positions.end());
        std::vector<int> pos_already_taken;

        for(int i = 0; i < filter_names_.size(); i++){
            if(!(std::find(std::begin(or_positions), std::end(or_positions), i) != std::end(or_positions))){
                names_.push_back(filter_names_.at(i));
            }
            else{
                for(auto c  : OR_connections){
                    
                    if( i == c && !(std::find(std::begin(pos_already_taken), std::end(pos_already_taken), c) != std::end(pos_already_taken))){
                        
                        std::stringstream stream;
                        stream << "OR : " ;
                        for(auto j : OR_connections){
                            stream << filter_names_.at(j) << " ";
                            pos_already_taken.push_back(j);
                        }
                        names_.push_back(stream.str());
                        
                    }
                }
                
            }

        }
    }
    
    return names_;

}

int man_filter::get_trigger_length(){
    
    if(OR_connections.size() != 0)
        return filter_th.size()-OR_connections.size()+1;
    else
        return filter_th.size();
        
}

void man_filter::remove_by_index(int ind){
    
    filter_map.erase(filter_map.begin()+ind);
    filter_names_.erase(filter_names_.begin()+ind);
    filter_th.erase(filter_th.begin()+ind);
    save_cum.resize(filter_th.size(), 0);

    return;
}


