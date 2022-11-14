out: basis.o utils.o
	gcc basis.o utils.o -o out

basis.o: basis.c basis.h
	gcc -c basis.c

utils.o: utils.c utils.h
	gcc -c utils.c

clean:
	rm *.o *.exe out