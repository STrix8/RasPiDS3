CXXFLAGS	= -std=c++11
OBJS		= test.o RasPiDS3.a 

test : $(OBJS)
	g++ -o RasPiDS3 $(OBJS)

RasPiDS3.a : RasPiDS3.o
	ar r $@ $<
	ranlib $@

.cpp.o : 
	g++ -c $< -g -02

all : test

clean :
	rm -rf *.o *.a test
