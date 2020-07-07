test: src/test.cpp src/parser.cpp
	g++ -o test src/test.cpp src/parser.cpp src/memory.cpp -lgtest

.PHONY: unit_test
unit_test: clean test
	./test

.PHONY: clean
clean:
	rm -rf ./test