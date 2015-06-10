test: test.o RasPiDS3.o
	g++ -Wall -o test test.o RasPiDS3.o -std=c++11 -pthread
RasPiDS3.o: RasPiDS3.cpp
	g++ -Wall -c RasPiDS3.cpp -std=c++11 -pthread
test.o: test.cpp
	g++ -Wall -c test.cpp -std=c++11 -pthread
clean:
	rm -f *.o test
