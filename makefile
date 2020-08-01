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

.PHONY: clean
clean:
	rm -rf ./code
	rm -rf ./linkfile
