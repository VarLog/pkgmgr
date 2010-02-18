CFLAGS += -O0 -pipe

CFLAGS += -Wall -pedantic -std=c99 `pkg-config --print-errors --cflags sqlite3` -D_XOPEN_SOURCE=500
LDFLAGS += `pkg-config --print-errors --libs sqlite3` -ldl
LIBARCHIVE_LIBS = `pkg-config --print-errors --libs libarchive`
DEBUG = -DDEBUG -g
OBJ = main.o sqlite.o utils.o options.o

all: main

main: $(OBJ)
	gcc -o main $(OBJ) $(LDFLAGS) $(LIBARCHIVE_LIBS) $(DEBUG)  
main.o: main.c
	gcc -c main.c -o main.o $(CFLAGS) $(DEBUG) 
sqlite.o: sqlite.c
	gcc -c sqlite.c -o sqlite.o $(CFLAGS) $(DEBUG) 
utils.o: utils.c
	gcc -c utils.c -o utils.o $(CFLAGS) $(DEBUG)
options.o: options.c
	gcc -c options.c -o options.o $(CFLAGS) $(DEBUG) 
clean:
	rm -vf main $(OBJ)

