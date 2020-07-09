test_1: src/test1.cpp src/core.cpp src/parser.cpp src/memory.cpp
	g++ -o test_1 src/test1.cpp src/core.cpp src/parser.cpp src/memory.cpp -lgtest

test_2: src/test2.cpp src/core.cpp src/parser.cpp src/memory.cpp
	g++ -o test_2 src/test2.cpp src/core.cpp src/parser.cpp src/memory.cpp -lgtest

.PHONY: unit_test_1
unit_test: clean test_1 test_2
	./test_1
	./test_2

unit_test_1: clean test_1
	./test_1

unit_test_2: clean test_2
	./test_2

.PHONY: clean
clean:
	rm -rf ./test_1
	rm -rf ./test_2
