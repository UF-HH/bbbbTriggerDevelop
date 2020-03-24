#ifndef TRIGGER_MANAGER_H
#define TRIGGER_MANAGER_H

#include <string>
#include <vector>

class man_filter{

    private:
        std::vector<std::string> filter_names_;
        std::vector<int> filter_th;
        std::vector<std::pair<std::string,int>> filter_map;
        std::vector<int> pass_filter_;
        std::string trig_n;
        bool passed_trigger;
        std::vector<int> event_pass_vector;
        std::vector<int> OR_connections;
        std::vector<int> event_count;
        std::vector<int> filters_index;
        double weight;
        std::vector<double> save_cum;

    public:
        man_filter(){};
        man_filter(std::vector<std::string> f_n);
        man_filter(std::vector<std::string> f_n, std::vector<int> f_val);
        man_filter(std::vector<std::pair<std::string,int>> filter);
        man_filter(std::vector<std::pair<std::string,int>> filter, std::string trigger_name_);
        ~man_filter(){};

        //methods
        void initialize_filters_id();
        void set_trigger_name(std::string trigger_name_);
        void initialize_f_threshold(std::vector<int> f_val);
        void initialize_f_names(std::vector<std::string> f_names);
        void set_OR(std::vector<int> OR_conn);
        void add_to_map(std::string filter_name, int th);
        std::vector<int> filter_test(std::vector<int> counts_);
        void print();
        void print_map();
        int verify_filter_stop();
        std::vector<int> compute_or(std::vector<int> results);
        std::vector<int> clear_or(std::vector<int> results);
        void clear_save();
        bool is_trigger_passed(){return passed_trigger;};
        std::vector<int> get_relative_eff();
        std::vector<int> get_cumulative_eff();
        int get_trigger_length();
        std::vector<std::string> get_fil_names();
        std::vector<int> f_test_suppose_sequential(std::vector<int> counts_, bool save, double weight);
        std::string get_trigger_name(){return trig_n;};
        std::vector<double> get_sum_vector(){return save_cum; };


};

#endif // TRIGGER_MANAGER_H