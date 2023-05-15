CC=gcc-13
INCDIRS=-I/opt/homebrew/cellar/criterion/2.4.1_2/include
OPT=-O0
OMP=-fopenmp
SRC=basis
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
CFLAGS=-Wall -Wextra $(OMP) $(INCDIRS) $(OPT)

BINARY=bin

TEST=tests
TESTS=$(wildcard $(TEST)/*.c)
TESTBINS=$(patsubst $(TEST)/%.c, $(TEST)/bin/%, $(TESTS))

all: $(BINARY)

$(BINARY): $(OBJS) main.c
	$(CC) $(CFLAGS) main.c $(OBJS) -o $(BINARY)

$(OBJS): | $(OBJ)

$(OBJ):
	mkdir $@

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST)/bin: 
	mkdir $@

$(TEST)/bin/%: $(TEST)/%.c $(OBJS) | $(TEST)/bin
	$(CC) $(CFLAGS) $< $(OBJS) -o $@ -lcriterion

test: $(TESTBINS)
	for test in $(TESTBINS) ; do ./$$test ; done

clean:
	rm -r obj
	rm -rf $(BINARY)
	rm -r tests/bin