test: test.o RasPiDS3.o
	g++ -Wall -o test test.o RasPiDS3.o -std=c++11
RasPiDS3.o: RasPiDS3.cpp
	g++ -Wall -c RasPiDS3.cpp -std=c++11
test.o: test.cpp
	g++ -Wall -c test.cpp -std=c++11
clean:
	rm -f *.o test
