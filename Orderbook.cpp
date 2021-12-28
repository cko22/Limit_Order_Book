#include <iostream>     // std::cin, std::cout
#include <iomanip>      // std::get_time, std::fixed, std::setprecision
#include <set>          // std::set
#include <string>       // std::string
#include <queue>        // std::queue
#include <vector>
#include "Orderbook.hpp"

using std::cout;
using std::endl;


// Parameterised ctor
Order::Order (double price1, std::string time1, int orderSize1, std::string orderID1, std::string orderType1){
    price = price1;
    time = time1;
    orderSize = orderSize1;
    orderID = orderID1;
    orderType = orderType1; 
}

// ">" operator overloading
bool Order::operator > (const Order& rhs) const{
    if (price == rhs.price) {
        int hour = std::stoi(time.substr(0,2))*3600; 
        int minute = std::stoi(time.substr(3,2))*60;
        int seconds = std::stoi(time.substr(6,2));
        int total_seconds = hour + minute + seconds; 

        int rhs_hour = std::stoi(rhs.time.substr(0,2))*3600; 
        int rhs_minute = std::stoi(rhs.time.substr(3,2))*60;
        int rhs_seconds = std::stoi(rhs.time.substr(6,2));
        int rhs_total_seconds = rhs_hour + rhs_minute + rhs_seconds; 

        return total_seconds < rhs_total_seconds;
    } else{
        return price > rhs.price;
    };
    
}

// "<" operator overloading
bool Order::operator < (const Order& rhs) const{
    if (price == rhs.price) {
        int hour = std::stoi(time.substr(0,2))*3600; 
        int minute = std::stoi(time.substr(3,2))*60;
        int seconds = std::stoi(time.substr(6,2));
        int total_seconds = hour + minute + seconds; 

        int rhs_hour = std::stoi(rhs.time.substr(0,2))*3600; 
        int rhs_minute = std::stoi(rhs.time.substr(3,2))*60;
        int rhs_seconds = std::stoi(rhs.time.substr(6,2));
        int rhs_total_seconds = rhs_hour + rhs_minute + rhs_seconds; 

        return total_seconds < rhs_total_seconds;
    } else{
        return price < rhs.price;
    };
    
}



// Print the entire orderbook, BID # ASK
void OrderBook::print(){
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    cout << "--------------- BID ------------- # -------------- ASK --------------" << endl;
    if (bid.size() == 0 && ask.size() == 0){
        cout << "|       "  << " | " << " " << " | "  << 
            "        " <<  " | "  <<  "       "  << " # " << "       "  << " | " << "        " << " | "  << 
            " " <<  " | "  <<  "       |" << endl; 
    }
    std::set<Order>::iterator it1 = bid.begin();
    std::set<Order>::iterator it2 = ask.begin(); 
    while  (it1 != bid.end() || it2 != ask.end()){
        if (it1 == bid.end()) {
            cout << "|       "  << " | " << " " << " | "  << 
            "        " <<  " | "  <<  "       "  ; 
        }else if (it1 != bid.end()) {
            Order bid_row = *it1; 
            cout  << "| " << bid_row.orderID << " | "  << 
            bid_row.orderSize <<  " | "  <<  bid_row.time << " | " <<  "$ " << bid_row.price ; 
            ++it1; 
        }; 
        cout << " # " ; 
        if (it2 == ask.end()) {
            cout << "       "  << " | " << "        " << " | "  << 
            " " <<  " | "  <<  "       |" << endl; 
        }else if (it2 != ask.end()) {
            Order ask_row = *it2;
            cout << "$ " << ask_row.price  << " | " << ask_row.time << " | "  << 
            ask_row.orderSize <<  " | "  <<  ask_row.orderID  << " |" << endl;  
            ++it2; 
        }
    }  
    return;
};

        
// Add limit order, Cancel order, Market order
void OrderBook::matchingEngine(std::string orderMessage){
    
    // Check first character
    char firstChar  = orderMessage.at(0);

    // Add limit order
    if (firstChar == 'A'){
        Order newOrder;
        // Get the content between commas
        std::vector<std::string> entries;
        std::string content = "";
        for (int i{17} ; i < orderMessage.length() ; ++i){
            // Ignore the dollar sign
            if (orderMessage.at(i)  == '$'){
                continue;
            }

            // Store the characters as content, except comma and space
            if (orderMessage.at(i) != ',' && orderMessage.at(i) != ' '){
                content += orderMessage.at(i);
            }
            
            // Store the content in a vector if it reaches a comma or end of string
            if (orderMessage.at(i) == ',' || i == orderMessage.length() - 1 ){
                if (content == "SELL"){
                    content = "ask";
                }
                if (content == "BUY"){
                    content = "bid";
                }
                entries.emplace_back(content);
                content = "";
            }
        }
        // Populate orders 
        newOrder.orderID = entries.at(0); 
        newOrder.time = entries.at(1);
        newOrder.orderType = entries.at(2);
        newOrder.orderSize = std::stoi(entries.at(3))/100;
        newOrder.price = std::stod(entries.at(4)); 

        // Add to limit order book 
        if (newOrder.orderType == "bid"){
            bid.insert(newOrder);
        }else if ((newOrder.orderType == "ask")){
            ask.insert(newOrder);
        }

        // After a new limit order is added, we should check if pending market order can be filled if there is any
        // if (pendingSellOrder.size() != 0 && newOrder.orderType == "bid"){
        while (pendingSellOrder.size() != 0 && newOrder.orderType == "bid" &&  bid.size() != 0){
            Order pendingOrder = pendingSellOrder.front();  
            int bookSize = bid.size();
            std::set<Order>::iterator it = bid.begin(); 
            int requiredOrderSize = pendingOrder.orderSize;
            cout << "************" << endl;
            cout << "Market Order " << pendingOrder.orderID << ":" << endl; 
            while (it!= bid.end()){
                Order newOrder; 
                if (requiredOrderSize == it->orderSize){
                    // If market order is filled exactly by current limit order
                    cout << "- " << it->orderSize*100 << " shares sold at " << "$ " << it->price << endl;
                    requiredOrderSize = 0; // reset to 0 
                    bid.erase(it); // remove order from the book
                    pendingSellOrder.pop(); // remove pending order 
                    break;
                }else if (requiredOrderSize < it->orderSize){
                    // If market order is completely filled by current limit order, but there are some shares remaining at this price level
                    // Modify the limit order
                    newOrder.price = it->price;
                    newOrder.time = it->time;
                    newOrder.orderSize = it->orderSize - requiredOrderSize;
                    newOrder.orderID = it->orderID;
                    newOrder.orderType = it->orderType;
                    cout << "- " << requiredOrderSize*100 << " shares sold at " << "$ " << it->price << endl;
                    requiredOrderSize = 0; // reset to 0 
                    bid.erase(it); // remove old order from the book
                    bid.insert(newOrder); // insert modified order into the book
                    pendingSellOrder.pop(); // remove pending order 
                    break;
                }else if (requiredOrderSize > it->orderSize){
                    // If market order is partially filled by current limit order
                    cout << "- " << it->orderSize*100 << " shares sold at " << "$ " << it->price << endl;
                    requiredOrderSize = requiredOrderSize - it->orderSize; // update required order size 
                    bid.erase(it); // remove order 
                    pendingSellOrder.front().orderSize = requiredOrderSize; // edit required ordersize
                    it = bid.begin(); // reset iterator to start from the beginning, if the book is empty, this will cause the while loop to stop
                }
            }
            cout << "************" << endl;
        }
        // }else if (pendingBuyOrder.size() != 0 && newOrder.orderType == "ask" && ask.size() != 0 ){
        while (pendingBuyOrder.size() != 0 && newOrder.orderType == "ask" && ask.size() != 0 ){
            Order pendingOrder = pendingBuyOrder.front();  
            int bookSize = ask.size();
            std::set<Order>::iterator it = ask.begin(); 
            int requiredOrderSize = pendingOrder.orderSize;
            cout << "************" << endl;
            cout << "Market Order " << pendingOrder.orderID << ":" << endl; 
            while (it!= ask.end()){
                Order newOrder; 
                if (requiredOrderSize == it->orderSize){
                    // If market order is filled exactly by current limit order
                    cout << "- " << it->orderSize*100 << " shares bought at " << "$ " << it->price << endl;
                    requiredOrderSize = 0; // reset to 0 
                    ask.erase(it); // remove order from the book
                    pendingBuyOrder.pop(); // remove pending order 
                    break;
                }else if (requiredOrderSize < it->orderSize){
                    // If market order is completely filled by current limit order, but there are some shares remaining at this price level
                    // Modify the limit order
                    newOrder.price = it->price;
                    newOrder.time = it->time;
                    newOrder.orderSize = it->orderSize - requiredOrderSize;
                    newOrder.orderID = it->orderID;
                    newOrder.orderType = it->orderType;
                    cout << "- " << requiredOrderSize*100 << " shares bought at " << "$ " << it->price << endl;
                    requiredOrderSize = 0; // reset to 0 
                    ask.erase(it); // remove old order from the book
                    ask.insert(newOrder); // insert modified order into the book
                    pendingBuyOrder.pop(); // remove pending order 
                    break;
                }else if (requiredOrderSize > it->orderSize){
                    // If market order is partially filled by current limit order
                    cout << "- " << it->orderSize*100 << " shares bought at " << "$ " << it->price << endl;
                    requiredOrderSize = requiredOrderSize - it->orderSize; // update required order size 
                    ask.erase(it); // remove order 
                    pendingBuyOrder.front().orderSize = requiredOrderSize; // edit required ordersize
                    it = ask.begin(); // reset iterator to start from the beginning, if the book is empty, this will cause the while loop to stop
                }
            }
            cout << "************" << endl;
        }

    }else if (firstChar == 'C'){
        // Cancel order
        Order cancelOrder;
        // Get the content between commas
        std::vector<std::string> entries;
        std::string content = "";
        for (int i{8} ; i < orderMessage.length() ; ++i){
            // Ignore the dollar sign
            if (orderMessage.at(i)  == '$'){
                continue;
            }
            
            // Store the characters as content, except comma and space
            if (orderMessage.at(i) != ',' && orderMessage.at(i) != ' '){
                content += orderMessage.at(i);
            }
            
            // Store the content in a vector if it reaches a comma or end of string
            if (orderMessage.at(i) == ',' || i == orderMessage.length() - 1 ){
                if (content == "SELL"){
                    content = "ask";
                }
                if (content == "BUY"){
                    content = "bid";
                }
                entries.emplace_back(content);
                content = "";
            }
        }
        // Create cancel order from order message string
        cancelOrder.orderID = entries.at(0); 
        cancelOrder.time = entries.at(1);
        cancelOrder.orderType = entries.at(2);
        cancelOrder.orderSize = std::stoi(entries.at(3))/100;
        cancelOrder.price = std::stod(entries.at(4));

        cout << "*****************" << endl;
        // Remove order to be cancelled
        if (cancelOrder.orderType == "bid"){
            auto search = bid.find(cancelOrder);
            if (search != bid.end()) {
                cout << "Order " << bid.find(cancelOrder)->orderID << " cancelled." << endl;
                bid.erase(bid.find(cancelOrder));
            } else {
                std::cerr << "Error: Could not cancel order. Order not found!" << endl;
            }
            
        }else if ((cancelOrder.orderType == "ask")){
            std::set<Order>::iterator search = ask.find(cancelOrder);
            if (search != ask.end()) {
                cout << "Order " << ask.find(cancelOrder)->orderID << " cancelled." << endl;
                ask.erase(ask.find(cancelOrder));
            } else {
                std::cerr << "Error: Could not cancel order. Order not found!" << endl;
            }
        }
        cout << "*****************" << endl;

    }else if (firstChar == 'M'){
        // Market order
        Order marketOrder;
        // Get the content between commas
        std::vector<std::string> entries;
        std::string content = "";
        for (int i{14} ; i < orderMessage.length() ; ++i){
            // Ignore the dollar sign
            if (orderMessage.at(i)  == '$'){
                continue;
            }
            
            // Store the characters as content, except comma and space
            if (orderMessage.at(i) != ',' && orderMessage.at(i) != ' '){
                content += orderMessage.at(i);
            }
            
            // Store the content in a vector if it reaches a comma or end of string
            if (orderMessage.at(i) == ',' || i == orderMessage.length() - 1 ){
                if (content == "SELL"){
                    // Market sell order
                    content = "bid";
                }
                if (content == "BUY"){
                    // Market buy order
                    content = "ask";
                }
                entries.emplace_back(content);
                content = "";
            }
        }
        // Populate create market order from order message string 
        marketOrder.orderID = entries.at(0); 
        marketOrder.time = entries.at(1);
        marketOrder.orderType = entries.at(2);
        marketOrder.orderSize = std::stoi(entries.at(3))/100;

        int bookSize;
        std::set<Order>::iterator it;
        int requiredOrderSize = marketOrder.orderSize;
        if (marketOrder.orderType == "bid"){
            if (bid.size() == 0){
                // If no limit order available, store in a pending queue
                pendingSellOrder.push(marketOrder); 
                return;
            }

            bookSize = bid.size();
            it = bid.begin(); 
            cout << "************" << endl;
            cout << "Market Order " << marketOrder.orderID << ":" << endl; 
            while (it!= bid.end()){
                Order newOrder; 
                if (requiredOrderSize == it->orderSize){
                    // If market order is filled exactly by current limit order
                    cout << "- " << it->orderSize*100 << " shares sold at " << "$ " << it->price << endl;
                    requiredOrderSize = 0; // reset to 0 
                    bid.erase(it); // remove order
                    break;
                }else if (requiredOrderSize < it->orderSize){
                    // If market order is completely filled by current limit order, but there are some shares remaining at this price level
                    // Modify the limit order
                    newOrder.price = it->price;
                    newOrder.time = it->time;
                    newOrder.orderSize = it->orderSize - requiredOrderSize;
                    newOrder.orderID = it->orderID;
                    newOrder.orderType = it->orderType;
                    cout << "- " << requiredOrderSize*100 << " shares sold at " << "$ " << it->price << endl;
                    requiredOrderSize = 0; // reset to 0 
                    bid.erase(it); // remove old order
                    bid.insert(newOrder); // insert modified order
                    break;
                }else if (requiredOrderSize > it->orderSize){
                    // If market order is partially filled by current limit order
                    cout << "- " << it->orderSize*100 << " shares sold at " << "$ " << it->price << endl;
                    requiredOrderSize = requiredOrderSize - it->orderSize; // update required order size 
                    bid.erase(it); // remove order 
                    it = bid.begin(); // reset iterator to start from the beginning, if the book is empty, this will cause the while loop to stop
                }
            }
            cout << "************" << endl;
            // If market order is partially filled and the book is empty, modify it and store in pending order queue
            if (requiredOrderSize != 0 && bid.size() == 0){
                Order newMarketOrder;
                // Modify orders
                newMarketOrder.orderID = marketOrder.orderID; 
                newMarketOrder.time = marketOrder.time; 
                newMarketOrder.orderType = marketOrder.orderType; 
                newMarketOrder.orderSize = requiredOrderSize; 
                pendingSellOrder.push(newMarketOrder); 
            }
        }else if (marketOrder.orderType == "ask"){
            if (ask.size() == 0){
                // If no limit order available, store in a pending queue
                pendingBuyOrder.push(marketOrder); 
                return;
            }

            bookSize = ask.size();
            it = ask.begin(); 
            cout << "************" << endl;
            cout << "Market Order " << marketOrder.orderID << ":" << endl; 
            while (it!= ask.end()){
                Order newOrder; 
                if (requiredOrderSize == it->orderSize){
                    // If market order is filled exactly by current limit order
                    cout << "- " << it->orderSize*100 << " shares bought at " << "$ " << it->price << endl;
                    requiredOrderSize = 0; // reset to 0 
                    ask.erase(it); // remove order
                    break;
                }else if (requiredOrderSize < it->orderSize){
                    // If market order is completely filled by current limit order, but there are some shares remaining at this price level
                    // Modify the limit order
                    newOrder.price = it->price;
                    newOrder.time = it->time;
                    newOrder.orderSize = it->orderSize - requiredOrderSize;
                    newOrder.orderID = it->orderID;
                    newOrder.orderType = it->orderType;
                    cout << "- " << requiredOrderSize*100 << " shares bought at " << "$ " << it->price << endl;
                    requiredOrderSize = 0; // reset to 0 
                    ask.erase(it); // remove old order
                    ask.insert(newOrder); // insert modified order
                    break;
                }else if (requiredOrderSize > it->orderSize){
                    // If market order is partially filled by current limit order
                    cout << "- " << it->orderSize*100 << " shares bought at " << "$ " << it->price << endl;
                    requiredOrderSize = requiredOrderSize - it->orderSize; // update required order size 
                    ask.erase(it); // remove order 
                    it = ask.begin(); // reset iterator to start from the beginning, if the book is empty, this will cause the while loop to stop
                }
            }
            cout << "************" << endl;
            // If market order is partially filled and the book is empty, modify it and store in pending order queue
            if (requiredOrderSize != 0 && ask.size() == 0){
                Order newMarketOrder;
                // Modify orders
                newMarketOrder.orderID = marketOrder.orderID; 
                newMarketOrder.time = marketOrder.time; 
                newMarketOrder.orderType = marketOrder.orderType; 
                newMarketOrder.orderSize = requiredOrderSize; 
                pendingBuyOrder.push(newMarketOrder);
            }
        }
        
    }else {
        cout << "**********" << endl;
        std::cerr << "Error: Unknown order message" << endl;
        cout << "**********" << endl;
    }
    return; 
}