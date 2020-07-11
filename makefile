code: src/main_seq.cpp src/core.cpp src/parser.cpp src/memory.cpp
	g++ -O3 -o code src/main_seq.cpp src/core.cpp src/parser.cpp src/memory.cpp 

test_1: src/test1.cpp src/core.cpp src/parser.cpp src/memory.cpp
	g++ -o test_1 src/test1.cpp src/parser.cpp src/memory.cpp -lgtest

test_stdin: src/test1_stdin.cpp src/core.cpp src/parser.cpp src/memory.cpp
	g++ -o test_stdin src/test1_stdin.cpp src/parser.cpp src/memory.cpp -lgtest
	
test_2: src/test2.cpp src/core.cpp src/parser.cpp src/memory.cpp
	g++ -o test_2 src/test2.cpp src/core.cpp src/parser.cpp src/memory.cpp -lgtest

test_3: src/test3.cpp src/core.cpp src/parser.cpp src/memory.cpp
	g++ -o test_3 src/test3.cpp src/core.cpp src/parser.cpp src/memory.cpp -lgtest

test_gcovr: src/test_all.cpp src/core.cpp src/parser.cpp src/memory.cpp
	g++ -fprofile-arcs -ftest-coverage -fPIC -O0 -o test_gcovr src/test_all.cpp src/core.cpp src/parser.cpp src/memory.cpp -lgtest

.PHONY: unit_test
unit_test: clean test_1 test_2 test_3 test_stdin
	./test_1
	./test_2
	./test_3
	./test_stdin < dataset/stdin-testset/test1.data

.PHONY: unit_test_1
unit_test_1: clean test_1
	./test_1

.PHONY: unit_test_2
unit_test_2: clean test_2
	./test_2

.PHONY: unit_test_3
unit_test_3: clean test_3
	./test_3

.PHONY: unit_test_stdin
unit_test_stdin: clean test_stdin
	./test_stdin < dataset/stdin-testset/test1.data

.PHONY: clean
clean:
	rm -rf ./test_1
	rm -rf ./test_2
	rm -rf ./test_3
	rm -rf ./test_stdin
	rm -rf ./code
	rm -rf ./test_gcovr
	rm -rf ./*.gcda
	rm -rf ./*.gcno
