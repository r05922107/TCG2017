LDFLAGS=-static -s -O2
.PHONY:	clean
search.exe:	main.cc anqi.cc ClientSocket.cpp Protocol.cpp
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm search.exe 2>NUL
