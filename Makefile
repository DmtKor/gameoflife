CC=g++
CFLAGS=-c -Wall

all: clean build
	./build/app

build: main.o
	$(CC) ./build/temp/main.o -o ./build/app -lsfml-graphics -lsfml-window -lsfml-system

main.o:
	$(CC) ./src/main.cc $(CFLAGS) -o ./build/temp/main.o

clean:
	rm -rf ./build/temp/*.o
	rm -rf ./build/app