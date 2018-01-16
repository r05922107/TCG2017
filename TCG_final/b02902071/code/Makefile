LDFLAGS=-O2 -static -s 
.PHONY:	clean
search:	main.cc anqi.cc ClientSocket.cpp Protocol.cpp
	$(CXX) -o $@ $^ $(LDFLAGS)
	chmod 777 $@ 

cc:
	cp search ../ubuntu/CreateRoom/Search/

ee:
	cp search ../ubuntu/EnterRoom/Search/

clean:
	rm search 2>NUL


test: testboard.cpp anqi.cc
	rm ./test 2>NUL || true
	$(CXX) -o $@ $(LDFLAGS)
	chmod 777 $@ 

