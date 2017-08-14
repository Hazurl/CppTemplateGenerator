all:
	g++ -std=c++17 -Wall cppGen.cpp -o fgen

install: fgen
	mv fgen /usr/bin