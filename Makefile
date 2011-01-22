CC=cc
CFLAGS=-Wall -Werror -O1
INCLUDE=-I/usr/include/malloc/

demo : demo.c ctree.h libctree.dylib
	$(CC) $(CFLAGS) -L. -lctree $< -o $@

# works only on APPLE (use -shared for other sytems)
libctree.dylib : ctree.c
	$(CC) $(CFLAGS) $(INCLUDE) -dynamiclib -c $< -o $@

.PHONY: clean

clean:
	rm -rf libctree.dylib demo
