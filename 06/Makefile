CC=g++
FLAGS=-std=c++20 -Wall -pedantic -Wextra 
TEST_LIB=-lgtest -lgtest_main -lpthread

all: test_comp test_bin

test_comp: format.hpp test.cpp
	$(CC) $(FLAGS) -c test.cpp

test_bin: format.hpp test.cpp
	$(CC) $(FLAGS) test.cpp -o test_bin $(TEST_LIB)

test:
	./test_bin

clean:
	rm -f *.o test_bin

