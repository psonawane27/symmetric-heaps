CC	=	icc

heaps: my-heaps.o dlmalloc.o memalloc.o shmem.o
	$(CC) -Wall -Werror -w3 -o $@ $^

dlmalloc.o: dlmalloc.c
	$(CC) -c -DONLY_MSPACES=1 -DHAVE_MORECORE=0 -DUSE_LOCKS=1 $<

memalloc.o: memalloc.c
	$(CC) -Wall -Werror -w3 -c $< -o $@

shmem.o: shmem.c
	$(CC) -Wall -Werror -w3 -c $< -o $@

clean: 
	rm -f *.o heaps

