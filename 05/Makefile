CC=g++
FLAGS=-std=c++20 -Wall -pedantic -Wextra 
TEST_LIB=-lgtest -lgtest_main -lpthread

all: test_bin


test_bin: data.hpp serializer.hpp test.cpp
	$(CC) $(FLAGS) test.cpp -o test_bin $(TEST_LIB)

test:
	./test_bin

clean:
	rm -f *.o test_bin