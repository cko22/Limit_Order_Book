#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>    // std::runtime_error
#include <sstream>      // std::stringstream
#include "miscellaneous.hpp"

using std::cout; 
using std::endl; 

void PrintVector(std::vector<int>& a) {
      for(std::vector<int>::iterator it = a.begin() ; it != a.end() ; ++it ){
          cout << *it <<endl;
      }
}; 

void PrintVector(std::vector<std::string>& a) {
      for(std::vector<std::string>::iterator it = a.begin() ; it != a.end() ; ++it ){
          cout << *it <<endl;
      }
}; 


std::vector<std::string> read_csv (std::string& filename){
    // Reads a CSV file into a vector of <string, vector<int>> pairs where
    // each pair represents <column name, column values>

    // Create a vector of <string, int vector> pairs to store the result
    std::vector<std::string> ordermessage;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line;

    // Read the column names
    if(myFile.good()){

        // Extract each line
        while(std::getline(myFile, line)){

            // Do not read double quotes
            line.erase(remove(line.begin(), line.end(), '"'), line.end());
            
            ordermessage.push_back(line);
        }
    }

    // Close file
    myFile.close();

    return ordermessage;
}