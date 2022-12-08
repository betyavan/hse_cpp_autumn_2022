CC=g++
FLAGS=-std=c++20 -Wall -pedantic -Werror -Wextra 
TEST_LIB=-lgtest -lgtest_main -lpthread

all: parser functions test_bin

parser: parser.cpp parser.h
	$(CC) $(FLAGS) -c parser.cpp

functions: functions.cpp functions.h
	$(CC) $(FLAGS) -c functions.cpp

test_bin: parser.o functions.o test.cpp
	$(CC) $(FLAGS) parser.o functions.o test.cpp -o test_bin $(TEST_LIB)

test:
	./test_bin

clean:
	rm -f *.o test_bin