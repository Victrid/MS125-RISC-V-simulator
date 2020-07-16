code: src/main_pred-sat.cpp linkfile/libcorepr.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o code src/main_pred-sat.cpp -lcorepr -lmemory -lparser

.PHONY: code_pip
code_pip: src/main_pip.cpp linkfile/libcorep.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o code src/main_pip.cpp -lcorep -lmemory -lparser

.PHONY: code_seq
code_seq: src/main_seq.cpp linkfile/libcore.a linkfile/libmemory.a linkfile/libparser.a
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

linkfile/libcorepr.a : src/core_predictor.cpp linkfile
	g++ -O3 -c -o libcorepr.o src/core_predictor.cpp
	ar rcs linkfile/libcorepr.a libcorepr.o
	rm libcorepr.o

linkfile/libcoret.a: src/core_tomasulo.cpp linkfile
	g++ -O3 -c -o libcoret.o src/core_tomasulo.cpp
	ar rcs linkfile/libcoret.a libcoret.o
	rm libcoret.o

# sequential test

test_1: src/test1.cpp linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_1 src/test1.cpp -lgtest -lmemory -lparser

test_2: src/test2.cpp linkfile/libcore.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_2 src/test2.cpp -lgtest -lcore -lmemory -lparser

test_3: src/test3.cpp linkfile/libcore.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_3 src/test3.cpp -lgtest -lcore -lmemory -lparser

.PHONY: unit_test
unit_test: test_1 test_2 test_3 test_5 test_6
	./test_1
	./test_2
	./test_3
	./test_5
	./test_6

.PHONY: unit_test_1
unit_test_1: test_1
	./test_1

.PHONY: unit_test_2
unit_test_2: test_2
	./test_2

.PHONY: unit_test_3
unit_test_3: test_3
	./test_3

#pipeline test

test_5: src/test5.cpp linkfile/libcorep.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_5 src/test5.cpp -lgtest -lcorep -lmemory -lparser

.PHONY: unit_test_5
unit_test_5: test_5
	./test_5

#predictor test

test_6: src/test6.cpp linkfile/libcorepr.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_6 src/test6.cpp -lgtest -lcorepr -lmemory -lparser

.PHONY: unit_test_6
unit_test_6: test_6
	./test_6

# tomasulo test

test_7: src/test7.cpp linkfile/libcoret.a linkfile/libmemory.a linkfile/libparser.a
	g++ -O3 -L linkfile -o test_7 src/test7.cpp -lgtest -lcoret -lmemory -lparser

.PHONY: unit_test_7
unit_test_7: test_7
	./test_7

.PHONY: clean
clean:
	rm -rf ./test_1
	rm -rf ./test_2
	rm -rf ./test_3
	rm -rf ./test_5
	rm -rf ./test_6
	rm -rf ./test_7
	rm -rf ./code
	rm -rf ./test_gcovr
	rm -rf ./*.gcda
	rm -rf ./*.gcno
	rm -rf ./linkfile
