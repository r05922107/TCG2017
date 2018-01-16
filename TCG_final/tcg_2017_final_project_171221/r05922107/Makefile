LDFLAGS=-static -s -O2 -lws2_32
.PHONY:	clean
search.exe:	main.cc anqi.cc ClientSocket.cpp Protocol.cpp HashTable.cpp
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm search.exe 2>NUL
