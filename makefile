alibcpp:
	cp -r source/* .
	g++ -c ReferenceCounter.cpp -std=c++11
	g++ -c ScrollingBuffer.cpp -std=c++11
	g++ -c Task.cpp -std=c++11
	g++ -c TaskPool.cpp -std=c++11
	g++ -c Timer.cpp -std=c++11
#	g++ -c comm/RF24TP.cpp -std=c++11
#	g++ -c comm/RF24TPPacket.cpp -std=c++11
#	g++ -c sensor/USRange.cpp -std=c++11
	ar -rc libalibcpp.a *.o
	rm -r *.o *.cpp comm/ sensor/

uninstall:
	rm -f /usr/local/lib/libalibcpp.a
	rm -fr /usr/local/include/alib-cpp/
	
install:
	make uninstall
	mkdir -p /usr/local/include/alib-cpp/	
	cp -r ./includes/* /usr/local/include/alib-cpp/
	cp libalibcpp.a /usr/local/lib/

all:
	make alibcpp
	make install

arduino:
	cp ./Arduino_alib-cpp.h ../
	cp ./Arduino_alib-cpp-comm.h ../
	cp source/JsonStreamWriter.cpp .
	cp source/ReferenceCounter.cpp .
	cp source/ScrollingBuffer.cpp .
	cp source/Task.cpp .
	cp source/TaskPool.cpp .
	cp source/Timer.cpp .
	cp source/comm/RF24TP.cpp comm-RF24TP.cpp
	cp source/comm/RF24TPPacket.cpp comm-RF24TPPacket.cpp
	cp source/sensor/USRange.cpp sensor-USRange.cpp

clean:
	rm -rf *.o *.cpp *.a comm/ sensor/
