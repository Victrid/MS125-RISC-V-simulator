code: src/main_seq.cpp linkfile/libcore.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o code src/main_seq.cpp -lcore -lmemory -lparser

linkfile:
	mkdir linkfile

linkfile/libparser.a: src/parser.cpp linkfile
	g++ -O3 -c -o libparser.o src/parser.cpp
	ar rcs linkfile/libparser.a libparser.o
	rm libparser.o

linkfile/libmemory.a: src/memory.cpp linkfile
	g++ -O3 -c -o libmemory.o src/memory.cpp
	ar rcs linkfile/libmemory.a libmemory.o
	rm libmemory.o

linkfile/libcore.a: src/core.cpp linkfile
	g++ -O3 -c -o libcore.o src/core.cpp
	ar rcs linkfile/libcore.a libcore.o
	rm libcore.o

linkfile/libcorep.a : src/core_pipeline.cpp linkfile
	g++ -O3 -c -o libcorep.o src/core_pipeline.cpp
	ar rcs linkfile/libcorep.a libcorep.o
	rm libcorep.o

# sequential test

test_1: src/test1.cpp linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_1 src/test1.cpp -lgtest -lmemory -lparser

test_stdin: src/test1_stdin.cpp linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_stdin src/test1_stdin.cpp -lgtest -lmemory -lparser

test_2: src/test2.cpp linkfile/libcore.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_2 src/test2.cpp -lgtest -lcore -lmemory -lparser

test_3: src/test3.cpp linkfile/libcore.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_3 src/test3.cpp -lgtest -lcore -lmemory -lparser

.PHONY: unit_test
unit_test: test_1 test_2 test_3 test_stdin
	./test_1
	./test_2
	./test_3
	./test_stdin < dataset/stdin-testset/test1.data
	./test_4

.PHONY: unit_test_1
unit_test_1: test_1
	./test_1

.PHONY: unit_test_2
unit_test_2: test_2
	./test_2

.PHONY: unit_test_3
unit_test_3: test_3
	./test_3

.PHONY: unit_test_stdin
unit_test_stdin: test_stdin
	./test_stdin < dataset/stdin-testset/test1.data

#pipeline test

test_4: src/test4.cpp linkfile/libcorep.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_4 src/test4.cpp -lgtest -lcorep -lmemory -lparser

test_5: src/test5.cpp linkfile/libcorep.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_5 src/test5.cpp -lgtest -lcorep -lmemory -lparser

.PHONY: unit_test_4
unit_test_4: test_4
	./test_4

.PHONY: unit_test_5
unit_test_5: test_5
	./test_5


.PHONY: clean
clean:
	rm -rf ./test_1
	rm -rf ./test_2
	rm -rf ./test_3
	rm -rf ./test_4
	rm -rf ./test_5
	rm -rf ./test_stdin
	rm -rf ./code
	rm -rf ./test_gcovr
	rm -rf ./*.gcda
	rm -rf ./*.gcno
	rm -rf ./linkfile
