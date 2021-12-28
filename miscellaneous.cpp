#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept> 
#include <sstream>
#include "miscellaneous.hpp"

using std::cout;
using std::endl;

void PrintVector(std::vector<int> & a){
    for (std::vector<int>::iterator it = a.begin() ; it != a.end() ; ++it){
        cout << *it << endl;
    }
}

void PrintVector(std::vector<std::string> & a){
    for (std::vector<std::string>::iterator it = a.begin() ; it != a.end() ; ++it){
        cout << *it << endl;
    }
}