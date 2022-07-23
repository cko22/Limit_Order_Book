# Introduction
Market participants interact with the exchange by placing or cancelling orders to buy or sell an instrument of a given quantity at a given price. The exchange collects these orders and stores them in what is known as the limit order book (LOB). The book contains all limit orders for which no matches have been found yet. A limit order book consists of a bid side and an ask side. 

An order matching engine is at the core of an exchange. All incoming limit orders are passed on to the matching engine, where they are matched against the limit orders in the limit order book. If no matches are found, the new limit order will be stored in the limit order book, on the appropriate side. 

The objectives of this project are:
- To build a limit order book
- To build an efficient price/time matching engine using STL containers

# References
1. https://www.oxford-man.ox.ac.uk/projects/market-simulator/
1. https://jellepelgrims.com/posts/matching_engines#python-implementation
1. https://gist.github.com/jdrew1303/e06361070468f6614d52216fb91b79e5
1. https://web.archive.org/web/20141222151051/https://dl.dropboxusercontent.com/u/3001534/engine.c
