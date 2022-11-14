out: basis.o utils.o
	gcc -fopenmp basis.o utils.o -o out

basis.o: basis.c basis.h
	gcc -c -fopenmp basis.c

utils.o: utils.c utils.h
	gcc -c -fopenmp utils.c

clean:
	rm *.o *.exe out