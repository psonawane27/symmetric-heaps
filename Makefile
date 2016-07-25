heaps: my-heaps.o dlmalloc.o memalloc.o
	icc -o $@ $^

dlmalloc.o: dlmalloc.c
	icc -c -DONLY_MSPACES=1 -DHAVE_MORECORE=0 -DUSE_LOCKS=1 $<


clean: 
	rm *.o heaps

