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

clean:
	rm -rf *.o *.cpp *.a comm/ sensor/

arduino:
	./setup_arduino.py

clean-arduino:
	rm ../*.h ../*.cpp ../includes
