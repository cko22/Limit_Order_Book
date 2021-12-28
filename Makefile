output: main.o Orderbook.o miscellaneous.o 
	g++ main.o Orderbook.o miscellaneous.o -o output

main.o: main.cpp
	g++ -c main.cpp 

Orderbook.o: Orderbook.cpp Orderbook.hpp
	g++ -c Orderbook.cpp 

miscellaneous.o: miscellaneous.cpp miscellaneous.hpp
	g++ -c miscellaneous.cpp 
	
clean:
	rm *.o output