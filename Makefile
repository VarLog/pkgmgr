CFLAGS += -O0 -pipe

CFLAGS += -Wall -pedantic -std=c99 `pkg-config --cflags sqlite3` -D_XOPEN_SOURCE=500
LDFLAGS += `pkg-config --libs sqlite3` -ldl
DEBUG = -DDEBUG -g
OBJ = main.o sqlite.o utils.o options.o

all: main

main: $(OBJ)
	gcc -o main $(OBJ) $(LDFLAGS) $(DEBUG)  
main.o: main.c
	gcc -c main.c -o main.o $(CFLAGS) $(DEBUG) $(FUNCFLAGS)
sqlite.o: sqlite.c
	gcc -c sqlite.c -o sqlite.o $(CFLAGS) $(DEBUG) $(FUNCFLAGS)
utils.o: utils.c
	gcc -c utils.c -o utils.o $(CFLAGS) $(DEBUG) $(FUNCFLAGS)
options.o: options.c
	gcc -c options.c -o options.o $(CFLAGS) $(DEBUG) $(FUNCFLAGS) 
clean:
	rm -vf main $(OBJ)

