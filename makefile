alibcpp:
	cp -r source/* .
	g++ -c ReferenceCounter.cpp -std=c++11
	g++ -c ScrollingBuffer.cpp -std=c++11
	g++ -c Task.cpp -std=c++11
	g++ -c TaskPool.cpp -std=c++11
	g++ -c Timer.cpp -std=c++11
	ar -rc libalibcpp.a *.o
	rm -r *.o *.cpp

install:
	mkdir -p /usr/local/include/alib-cpp/
	rm -f /usr/local/include/alib-cpp/*.h
	cp ./includes/*.h /usr/local/include/alib-cpp/
	cp libalibcpp.a /usr/local/lib/

all:
	make alibcpp
	make install

arduino:
	cp source/JsonStreamWriter.cpp .
	cp source/ReferenceCounter.cpp .
	cp source/ScrollingBuffer.cpp .
	cp source/Task.cpp .
	cp source/TaskPool.cpp .
	cp source/Timer.cpp .

clean:
	rm -rf *.o *.cpp *.a
