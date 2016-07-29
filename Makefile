CC	=	icc

CFLAGS = -Wall -Werror -pedantic -w3

heaps: my-heaps.o dlmalloc.o memalloc.o shmem.o
	$(CC) $(CFLAGS) -o $@ $^

dlmalloc.o: dlmalloc.c
	$(CC) -c -DONLY_MSPACES=1 -DHAVE_MORECORE=0 -DUSE_LOCKS=1 $<

clean: 
	rm -f *.o heaps

