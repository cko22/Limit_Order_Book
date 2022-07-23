output: main.o Orderbook.o miscellaneous.o 
	g++ main.o Orderbook.o miscellaneous.o -o output

main.o: main.cpp
	g++ -std=c++14 -c main.cpp -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk
	# g++ main.cpp -o main -std=c++14 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk

Orderbook.o: Orderbook.cpp Orderbook.hpp
	g++ -std=c++14 -c Orderbook.cpp -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk
	# g++ Orderbook.cpp -o Orderbook -std=c++14 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk

miscellaneous.o: miscellaneous.cpp miscellaneous.hpp
	g++ -std=c++14 -c miscellaneous.cpp -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk
	# g++ miscellaneous.cpp -o miscellaneous -std=c++14 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk

clean:
	rm *.o output
