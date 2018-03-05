OBJS = string.o bst.o stack.o queue.o dll.o sll.o scanner.o trees.o
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

stack.o:	stack.c stack.h dll.h
		gcc $(OOPTS) stack.c

sll.o:	sll.c sll.h
		gcc $(OOPTS) sll.c

dll.o:	dll.c dll.h
		gcc $(OOPTS) dll.c

bst.o:	bst.c bst.h queue.h
		gcc $(OOPTS) bst.c

scanner.o:	scanner.c scanner.h
		gcc $(OOPTS) scanner.c

trees.o:	trees.c bst.h
		gcc $(OOPTS) trees.c

test-all:	trees
		@echo Testing...

test:	trees
		@echo Testing...

valgrind: trees
		valgrind ./trees

clean:
		rm -f vgcore.* $(OBJS) $(EXECUTABLES)
