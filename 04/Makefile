CC=g++
FLAGS=-std=c++20 -Wall -pedantic -Wextra 
TEST_LIB=-lgtest -lgtest_main -lpthread

all: big_int test_bin

big_int: bigint.cpp bigint.h myvector.hpp myvector.h
	$(CC) $(FLAGS) -c bigint.cpp

test_bin: bigint.o test.cpp
	$(CC) $(FLAGS) bigint.o test.cpp -o test_bin $(TEST_LIB)

test:
	./test_bin

clean:
	rm -f *.o test_bin