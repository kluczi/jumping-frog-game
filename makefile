all: main

main : main.cpp src/*
	clang++ -o main main.cpp -lncurses