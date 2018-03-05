OBJS = string.o queue.o sll.o scanner.o trees.o
OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -std=c99 -g
EXECUTABLES = trees

all: trees

trees:	$(OBJS)
		gcc $(LOPTS) $(OBJS) -o trees

string.o:	string.c string.h
		gcc $(OOPTS) string.c

queue.o:	queue.c queue.h sll.h
		gcc $(OOPTS) queue.c

sll.o:	sll.c sll.h
		gcc $(OOPTS) sll.c

scanner.o:	scanner.c scanner.h
		gcc $(OOPTS) scanner.c

trees.o:	trees.c
		gcc $(OOPTS) trees.c

test-all:	trees
		@echo Testing...

test:	trees
		@echo Testing Preliminary...
		@./trees -g ./Testing/t-0-0.corpus ./Testing/t-0-0.commands

valgrind: trees
		valgrind ./trees

clean:
		rm -f vgcore.* $(OBJS) $(EXECUTABLES)
