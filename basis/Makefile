out: matrix.o utils.o vec.o
	gcc-13 -fopenmp matrix.o utils.o vec.o -o out

vec.o: vec.c vec.h
	gcc-13 -c  vec.c

matrix.o: matrix.c matrix.h
	gcc-13 -c -fopenmp matrix.c

utils.o: utils.c utils.h
	gcc-13 -c -fopenmp utils.c

clean:
	rm *.o *.exe out