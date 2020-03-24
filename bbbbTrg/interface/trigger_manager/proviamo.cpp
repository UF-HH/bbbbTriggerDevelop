#include "trigger_manager.h"
#include <iostream>
#include <vector>
#include <string>

int main() {

    std::vector<int> fil_th = {1,2,4,5};
    std::vector<std::string> fil_n = {"paperino", "topolino", "pippo", "minnie"};
    std::vector<std::vector<int>> OR_con = {{2,1}};
    man_filter fil(fil_n, fil_th);
    fil.set_OR(OR_con);

    std::vector<std::string> fil_names = fil.get_fil_names();

    std::cout << fil_names.size()<< std::endl;
    for(auto c : fil_names){
        std::cout << c << std::endl;
    }
    return 0;
}