CC=g++
# ALL protector on! PIE, Canary, RELRO, NX
CXXFLAG= -std=c++17 -pie -fstack-protector -z noexecstack -z now

debug: main.cpp maze.cpp maze.h text.h
	$(CC) -D DEBUG -g -o debug $(CXXFLAG) main.cpp maze.cpp

release: main.cpp maze.cpp maze.h text.h
	$(CC) -s -o maze $(CXXFLAG) main.cpp maze.cpp

clean:
	@rm debug
	@rm maze


