CC=g++
CXXFLAG= -std=c++17

release: main.cpp enigma.cpp enigma.h signal.h
	$(CC) -o enigma -s -O2 $(CXXFLAG) main.cpp enigma.cpp

debug: main.cpp enigma.cpp enigma.h signal.h
	$(CC) -o debug -D DEBUG -g $(CXXFLAG) main.cpp enigma.cpp

clean:
	@rm debug
	@rm enigma

