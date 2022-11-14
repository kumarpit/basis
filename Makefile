out: basis.o utils.o
	gcc-12 -fopenmp basis.o utils.o -o out

basis.o: basis.c basis.h
	gcc-12 -c -fopenmp basis.c

utils.o: utils.c utils.h
	gcc-12 -c -fopenmp utils.c

clean:
	rm *.o *.exe out