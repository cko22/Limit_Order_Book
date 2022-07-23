#ifndef MISCELLANEOUS_HPP
#define MISCELLANEOUS_HPP

#include <vector>
#include <string>


void PrintVector(std::vector<int>& a);

void PrintVector(std::vector<std::string>& a);  

std::vector<std::string> read_csv (std::string& filename); 

#endif