.PHONY: check
check: test bench
	./test
	./bench

.PHONY: clean
clean:
	rm -f *.o test bench

test: morton.o test.o
test.o: test.c morton.h Makefile
bench: morton.o bench.o
bench.o: bench.c morton.h Makefile
morton.o: morton.c morton.h Makefile
