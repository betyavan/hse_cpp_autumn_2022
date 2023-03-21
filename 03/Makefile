CC=g++
FLAGS=-std=c++20 -Wall -pedantic -Wextra 
TEST_LIB=-lgtest -lgtest_main -lpthread

all: test_bin

test_comp: matrix.hpp matrix.h matrix_utilities.hpp matrix_utilities.h test.cpp
	$(CC) $(FLAGS) -c test.cpp

test_bin: matrix.hpp matrix.h matrix_utilities.hpp matrix_utilities.h test.cpp
	$(CC) $(FLAGS) test.cpp -o test_bin $(TEST_LIB)

test:
	./test_bin

clean:
	rm -f *.o test_bin