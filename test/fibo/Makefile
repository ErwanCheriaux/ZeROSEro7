CC=gcc
CFLAGS=-I.

EXE  = main
DEPS = fibo.h
OBJ  = main.o fibo.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: main clean

$(EXE): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f $(EXE) $(OBJ)
