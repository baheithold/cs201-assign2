OBJS = string.o queue.o sll.o bst.o gst.o avl.o scanner.o trees.o
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

bst.o: 	bst.c bst.h queue.h
		gcc $(OOPTS) bst.c

gst.o: 	gst.c gst.h bst.h
		gcc $(OOPTS) gst.c

avl.o: 	avl.c avl.h bst.h
		gcc $(OOPTS) avl.c

scanner.o:	scanner.c scanner.h
		gcc $(OOPTS) scanner.c

trees.o:	trees.c gst.h avl.h
		gcc $(OOPTS) trees.c

test-all:	trees
		@echo Testing...

test:	trees
		@echo Testing Preliminary...
		@./trees -g ./Testing/t-0-0.corpus ./Testing/t-0-0.commands

valgrind: trees
		valgrind ./trees -g ./Testing/t-0-0.corpus ./Testing/t-0-0.commands

clean:
		rm -f vgcore.* $(OBJS) $(EXECUTABLES)
