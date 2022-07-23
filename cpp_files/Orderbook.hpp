#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

class Order {
    public: 
        double price;       
        std::string time;    
        int orderSize;          // 5 = 500 shares
        std::string orderID;    // O00001 
        std::string orderType;  // bid or ask

        // Parameterised ctor
        Order (double price1, std::string time1, int orderSize1, std::string orderID1, std::string orderType1);
        
        // Default ctor
        Order () = default; 

        // ">" operator overloading
        bool operator > (const Order& rhs) const;

        // "<" operator overloading
        bool operator < (const Order& rhs) const;

};



class OrderBook {
    public:
        // Create bid and ask sets (trees) when an OrderBook instance is constructed
        std::set<Order, std::greater<Order>> bid;
        std::set<Order, std::less<Order>> ask;

        // Create a queue to store pending market orders 
        std::queue<Order> pendingBuyOrder;      // buy market order => ask limit order book
        std::queue<Order> pendingSellOrder;     // sell market order => bid limit order book 

        // Print the entire orderbook, BID # ASK
        void print();

        // Add limit order, Cancel order, Market order
        void matchingEngine(std::string orderMessage);
    
};
#endif