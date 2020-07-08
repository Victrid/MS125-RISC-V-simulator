.PHONY: test
test: src/test1.cpp src/test2.cpp src/core.cpp src/parser.cpp src/memory.cpp
	g++ -o test1 src/test1.cpp src/core.cpp src/parser.cpp src/memory.cpp -lgtest
	g++ -o test2 src/test2.cpp src/core.cpp src/parser.cpp src/memory.cpp -lgtest

.PHONY: unit_test
unit_test: clean test
	./test1
	./test2

.PHONY: clean
clean:
	rm -rf ./test1
	rm -rf ./test2