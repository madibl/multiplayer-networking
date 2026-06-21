TARGET ?= my_default_target

.PHONY: ${TARGET}

${TARGET}:
	g++ ${TARGET}.cpp -o build/${TARGET}

all: 
	g++ server.cpp -o build/server
	g++ client.cpp -o build/client

clean:
	rm -f *.o server
	rm -f *.o client