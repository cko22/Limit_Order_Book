/* This project is put on hold on 12th Aug 2021
Orderbook implementation: https://github.com/aspone/OrderBook/blob/master/OrderBook.h 
DLL implementations: https://newbedev.com/initializer-list-constructor-for-doubly-linked-list-c-code-example 
*/


#include <iostream>     // std::cin, std::cout
#include <iomanip>      // std::get_time, std::fixed, std::setprecision
#include <set>          // std::set
#include <map>          // std::map
#include <cmath>
#include <unordered_map>
#include <list>         // std::list

#include <string>       // std::string
#include <queue>        // std::queue
#include <vector>
#include <chrono>       // std::chrono
#include "Orderbook.hpp"
#include "miscellaneous.hpp"

using std::cout;
using std::endl;


int main(){
    // Read order message from csv file, store in a vector
    std::string filename = "order_message.csv"; 
    std::vector <std::string> ordermessage_vect = read_csv(filename);
    
    // Create an orderbook instance for AAPL stock
    OrderBook APPL;  

    // Timer starts
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Pass order message into matching engine
    for (int i{0} ; i <  ordermessage_vect.size() ; ++i){
        APPL.matchingEngine(ordermessage_vect.at(i));
    }
    // Display the final full LOB
    APPL.print(); 

    // Timer ends    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    cout << "Elapsed Time = " << std::chrono::duration<double>(end - begin).count() << "[s]" << endl;
    cout << "No. of executions per second = " << round(ordermessage_vect.size()/std::chrono::duration<double>(end - begin).count()) << endl;

    
    return 0; 
}