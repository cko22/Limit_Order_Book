/* This project is put on hold on 12th Aug 2021
Orderbook idea: https://github.com/aspone/OrderBook/blob/master/OrderBook.h 
DLL implementations: https://newbedev.com/initializer-list-constructor-for-doubly-linked-list-c-code-example 
*/


#include <iostream>     // std::cin, std::cout
#include <iomanip>      // std::get_time, std::fixed, std::setprecision
#include <set>          // std::set
#include <map>          // std::map
#include <cmath>
#include <unordered_map>
#include <list>         // std::list
// #include <cstdlib>
#include <string>       // std::string
#include <queue>        // std::queue
#include <vector>
#include <chrono>       // std::chrono
#include "Orderbook.hpp"
#include "miscellaneous.hpp"

using std::cout;
using std::endl;

class Order1{
    public:
        double price;       
        std::string time;    
        int orderSize;          // 5 = 500 shares
        std::string orderID;    // O00001 
        std::string orderType;  // bid or ask

        // Parameterised ctor
        Order1 (double price1, std::string time1, int orderSize1, std::string orderID1, std::string orderType1){
            price = price1;
            time = time1;
            orderSize = orderSize1;
            orderID = orderID1;
            orderType = orderType1; 
        }
        
        // Default ctor
        Order1 () = default; 

};

/* We need to write a doubly-linked list class that does:
1. Insertion at back of the list 
2. Removal at front, middle, back of the list
3. Print DLL 
*/
class Node{
    public:
        Order1 data;
        Node *next;
        Node *prev;
};

class DLinkedList{
    private:
        Node* head_;
        Node* tail_; 
    public:
        DLinkedList(){
                head_ = NULL;
                tail_ = NULL;
        }

        Node* getHead(){
            return head_; 
        }

        Node* getTail(){
            return tail_; 
        }

        // A function to insert a node at the end of the list
        void appendNode(Order1 newOrder){
            // Create newnode
            Node* newnode = new Node();
            Node* tail_ = head_;
            // Insert order data into node
            newnode->data = newOrder;
            // This newnode is the last node, its next will be NULL
            newnode->next = NULL;
            // If the list is empty, the new node will be head
            if (head_ == NULL){
                newnode->prev = NULL;
                // Both head
                head_ = newnode; 
                tail_ = newnode; 
                return;
            }
            // Otherwise, traverse until last node
            while (tail_->next != NULL){
                tail_ = tail_->next;
            }
            // Update the next of last node
            tail_->next = newnode; 
            // Make last node as new node's prev
            newnode->prev = tail_;
            return;
        }

        // A function to delete a node in a linked list
        void deleteNode(Node* del){
            if (head_ == NULL || del == NULL){
                return; 
            }
            // If head node is to be deleted
            if (head_ == del){
                head_ = del->next;
            }
            // If the node to be deleted is not the last node
            if (del->next != NULL){
                del->next->prev = del->prev; 
            }
            // If the node to be deleted is not the first node
            if (del->prev != NULL){
                del->prev->next = del->next; 
            }
            // Finally, free memory occupied by del
            delete (del);

            return; 
        }  
        
        // A function that print the list of order ID
        void print(){
            while(head_ !=NULL){
                cout << head_->data.orderID << endl;
                head_ = head_->next; 
            }
}                    
};
// A function to get the first node of the list
// Order1 getHead(Node** head){

// }





int main(){
    Order1 bid_order(33.75, "12:02:33", 300, "O10021", "BUY");
    Order1 bid_order1(33.75, "12:03:34", 700, "O23021", "BUY");
    Order1 bid_order2(33.75, "12:01:34", 800, "O12421", "BUY");
    Order1 bid_order3(33.75, "12:06:34", 500, "O35421", "BUY");

    // Hash table to map orderID to node pointers
    std::unordered_map <std::string, Node*> orderAddress; 
    // Create a bid order object
    std::map <double, DLinkedList, std::greater<double>> bid;

    // Create a DLL object
    DLinkedList order_list;

    order_list.appendNode(bid_order);
    bid.insert(std::make_pair(bid_order.price, order_list)); 
    bid[bid_order1.price].appendNode(bid_order1); 
    bid[bid_order2.price].appendNode(bid_order2); 
    bid[bid_order3.price].appendNode(bid_order3); 

    bid[bid_order1.price].print(); 

    // orderAddress[bid_order.orderID] = node_address

    // Node* head = NULL;
    // Node* delnode = head; 
    // appendNode(&head, bid_order);
    // appendNode(&head, bid_order1);
    // appendNode(&head, bid_order2);
    // appendNode(&head, bid_order3);

    // print(head);
    // Order1 bid_order2(33.74, "12:02:34", 700, "O23021", "BUY");
    // std::map <double, std::list<Order1>, std::greater<double>> bid;
    // std::unordered_map <std::string, Order1*> orderAddress; 
    // std::list<Order1> order_list; 
    // order_list.push_back(bid_order);
    // bid.insert(std::make_pair(33.75, order_list)); 
    // bid[bid_order.price].push_back(bid_order); 
    // orderAddress[bid_order.orderID] = &(bid[bid_order.price].back());  // stores the memory address of bid order
    // cout << &bid_order << " " << &(bid[bid_order.price].back()) << endl;
    // bid[bid_order1.price].push_back(bid_order1); 
    // orderAddress[bid_order1.orderID] = &(bid[bid_order1.price].back()); 
    
    // std::list<Order1>::iterator it = bid[bid_order.price].begin();
    // for (; it!=bid[bid_order.price].end(); ++it){
        // cout << *it << endl;
        // cout << &(it->orderID) << endl;
        // cout << it->orderID << endl;
    // }

    // cout << "Order " <<  bid_order.orderID << " located at "<< orderAddress[bid_order.orderID] << endl;

    // bid[bid_order.price].erase(bid[bid_order.price].begin() + orderAddress[bid_order.orderID] - &(bid[bid_order.price][0]));

    // std::list<Order1>::iterator it = bid[33.75].begin();
    // for (; it!=bid[33.75].end(); ++it){
    //     cout << it->orderID << endl;
    // }





    // std::unordered_map<std::string> umtest ={}
    // // Read order message from csv file, store in a vector
    // std::string filename = "order_message.csv"; 
    // std::vector <std::string> ordermessage_vect = read_csv(filename);

    // // Pass order message into matching engine
    // OrderBook APPL;  

    // // Timer starts
    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // for (int i{0} ; i <  ordermessage_vect.size() ; ++i){
    //     APPL.matchingEngine(ordermessage_vect.at(i));
        
    // }
    // APPL.print();

    // // Timer ends    
    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    // std::cout << "Elapsed Time = " << std::chrono::duration<double>(end - begin).count() << "[s]" << std::endl;

    
    return 0; 
}