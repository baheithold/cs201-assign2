OBJS = string.o queue.o sll.o bst.o gst.o avl.o scanner.o trees.o
OOPTS = -Wall -Wextra -std=c99 -g -c
LOPTS = -Wall -Wextra -std=c99 -g
EXECUTABLES = trees gst-0-0 avl-0-0

all: $(EXECUTABLES)

trees:	$(OBJS)
		gcc $(LOPTS) $(OBJS) -o trees

gst-0-0: gst.o bst.o queue.o sll.o integer.o gst-0-0.o
		gcc $(LOPTS) gst.o bst.o queue.o sll.o integer.o gst-0-0.o -o gst-0-0

avl-0-0: avl.o bst.o queue.o sll.o integer.o avl-0-0.o
		gcc $(LOPTS) avl.o bst.o queue.o sll.o integer.o avl-0-0.o -o avl-0-0

integer.o: 	integer.c integer.h
		gcc $(OOPTS) integer.c

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

gst-0-0.o: 	./Testing/gst-0-0.c gst.h bst.h
		gcc $(OOPTS) ./Testing/gst-0-0.c

avl-0-0.o: 	./Testing/avl-0-0.c gst.h bst.h
		gcc $(OOPTS) ./Testing/avl-0-0.c

test-all:	trees gst-0-0
		@echo Testing...

test:	$(EXECUTABLES)
		@echo Testing gst-0-0
		@./gst-0-0 > ./Testing/myresults/gst-0-0.txt
		@diff ./Testing/expectedresults/gst-0-0.expected ./Testing/myresults/gst-0-0.txt
		@echo Testing avl-0-0
		@avl-0-0 > ./Testing/myresults/avl-0-0.txt
		@diff ./Testing/expectedresults/avl-0-0.expected ./Testing/myresults/avl-0-0.txt

valgrind: trees gst-0-0 avl-0-0
		valgrind ./trees -g ./Testing/t-0-0.corpus ./Testing/t-0-0.commands

clean:
		rm -f vgcore.* *.o $(EXECUTABLES)
