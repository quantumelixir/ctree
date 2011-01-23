CC=cc
CFLAGS=-Wall
INCLUDE=-I/usr/include
LIBRARY=-L/usr/lib

demo : demo.c
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBRARY) -lctree $< -o $@

.PHONY: clean

clean:
	rm -rf demo
