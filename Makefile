CC	=	icc

CFLAGS = -Wall -Werror -pedantic -w3

LD = $(CC)

LDFLAGS = $(CFLAGS)

heaps: my-heaps.o dlmalloc.o memalloc.o shmem.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

dlmalloc.o: dlmalloc.c
	$(CC) -c -DONLY_MSPACES=1 -DHAVE_MORECORE=0 -DUSE_LOCKS=1 $<

clean: 
	rm -f *.o heaps

