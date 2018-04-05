#Makefile modified from magarwal on the Beastie forums.
#Created 03/23/2018.

OBJS 		= integer.o real.o string.o sll.o queue.o scanner.o bst.o
MAIN 		=  gst.o avl.o
TESTS 		= $(GSTtests) $(AVLtests)
TREES  		= trees
OOPTS 		= -Wall -Wextra -std=c99 -g -c
LOPTS 		= -Wall -Wextra -std=c99 -g
GSTtests 	= gst-0-0 gst-0-1 gst-0-2 gst-0-3 gst-0-4 gst-0-5 gst-0-6 gst-0-7 \
		      gst-0-8 gst-0-9 gst-0-10
AVLtests 	= avl-0-0 avl-0-1 avl-0-2 avl-0-3 avl-0-4 avl-0-5 avl-0-6 avl-0-7 \
		      avl-0-8 avl-0-9 avl-0-10
TREEStests 	= t-0-0 t-0-1 t-0-2 t-0-3 t-0-4 t-0-5 t-0-6 t-0-7

all: 	$(OBJS) $(MAIN) $(TESTS) $(TREES)

################################################################################
#                                                 Classes for Primitive Types
#INTEGER
integer.o: 	integer.c integer.h
	gcc $(OOPTS) integer.c

#REAL
real.o: real.c real.h
	gcc $(OOPTS) real.c

#STRING
string.o: 	string.c string.h
	gcc $(OOPTS) string.c

################################################################################
#                                                                         SLL

sll.o: 	sll.c sll.h
	gcc $(OOPTS) sll.c

################################################################################
#                                                                         QUEUE

queue.o: 	queue.c queue.h sll.h
	gcc $(OOPTS) queue.c

################################################################################
#                                                                         BST

bst.o: 	bst.c bst.h queue.h
	gcc $(OOPTS) bst.c

################################################################################
#                                                                         GST

gst.o: 	gst.c gst.h bst.h
	gcc $(OOPTS) gst.c

################################################################################
#                                                                         AVL

avl.o: 	avl.c avl.h bst.h
	gcc $(OOPTS) avl.c

################################################################################
#                                                                      scanner

scanner.o: 	scanner.c
	gcc $(OOPTS) scanner.c

################################################################################
#                                                                         trees

trees: 	trees.c $(OBJS) $(MAIN)
	gcc $(LOPTS) trees.c $(OBJS) $(MAIN) -o trees

################################################################################
#                                                Test everything except trees

test: 	all
	@for x in $(GSTtests); do \
		echo Testing $$x...; \
		./$$x > ./Testing/0/gst/actual/$$x.actual; \
		diff ./Testing/0/gst/expected/$$x.expected ./Testing/0/gst/actual/$$x.actual; \
		done
	@for x in $(AVLtests); do \
		echo Testing $$x...; \
		./$$x > ./Testing/0/avl/actual/$$x.actual; \
		diff ./Testing/0/avl/expected/$$x.expected ./Testing/0/avl/actual/$$x.actual; \
		done

################################################################################
#                                                            Test only trees

testTreesGST: 	all
	@for x in $(TREEStests); do \
		echo Testing $$x with GST...; \
		./trees -g ./Testing/0/trees/$$x.data ./Testing/0/trees/$$x.more \
		> ./Testing/0/trees/actual/$$x-g.actual; \
		diff ./Testing/0/trees/expected/$$x-g.expected ./Testing/0/trees/actual/$$x-g.actual; \
		done

testTreesAVL: 	all
	@for x in $(TREEStests); do \
		echo Testing $$x with AVL...; \
		./trees -r ./Testing/0/trees/$$x.data ./Testing/0/trees/$$x.more \
		> ./Testing/0/trees/actual/$$x-r.actual; \
		diff ./Testing/0/trees/expected/$$x-r.expected ./Testing/0/trees/actual/$$x-r.actual; \
		done

testTreesBeowulf: 	all
	@echo Testing Beowulf GST...; \
	./trees -g ./Testing/big/beowulf.data ./Testing/big/beowulf.more \
	> ./Testing/big/actual/beowulf-g.actual; \
	diff ./Testing/big/expected/beowulf-g.expected ./Testing/big/actual/beowulf-g.actual;
	@echo Testing Beowulf AVL...; \
	./trees -r ./Testing/big/beowulf.data ./Testing/big/beowulf.more \
	> ./Testing/big/actual/beowulf-r.actual; \
	diff ./Testing/big/expected/beowulf-r.expected ./Testing/big/actual/beowulf-r.actual

testTreesShakespeare: 	all
	@echo Testing Shakespeare GST...; \
	./trees -g ./Testing/big/shakespeare.data ./Testing/big/shakespeare.more \
	> ./Testing/big/actual/shakespeare-g.actual; \
	diff ./Testing/big/expected/shakespeare-g.expected ./Testing/big/actual/shakespeare-g.actual;
	@echo Testing Shakespeare AVL...; \
	./trees -r ./Testing/big/shakespeare.data ./Testing/big/shakespeare.more \
	> ./Testing/big/actual/shakespeare-r.actual; \
	diff ./Testing/big/expected/shakespeare-r.expected ./Testing/big/actual/shakespeare-r.actual

testTreesWords: 	all
	@echo Testing Words GST...; \
	./trees -g ./Testing/big/words.data ./Testing/big/words.more \
	> ./Testing/big/actual/words-g.actual;
	@echo Testing Words AVL...; \
	./trees -r ./Testing/big/words.data ./Testing/big/words.more \
	> ./Testing/big/actual/words-r.actual;

################################################################################
#                                            Valgrind everything except trees

valgrind: 	all
	for x in $(TESTS); do echo -------; echo valgrind $$x; echo -------; valgrind ./$$x; echo; done

################################################################################
#                                                         Clean everything

clean:
	rm -f $(OBJS) $(MAIN) $(TESTS) $(TREES)

################################################################################
#                                                              Debug tests

debug: 	all
	for x in $(GSTtests) $(AVLtests); do echo -------; echo $$x; echo -------; ./$$x; echo; done

debugTreesAVL: 	all
	for x in $(TREEStests); do echo -------; echo $$x; echo -------; ./trees -r $$x.data $$x.more; echo; done

debugTreesGST: 	all
	for x in $(TREEStests); do echo -------; echo $$x; echo -------; ./trees -g $$x.data $$x.more; echo; done

################################################################################
#                                     Valgrind debug everything except trees

valgrind-debug: all
	for x in $(GSTtests) $(AVLtests); echo valgrind $$x; valgrind ./$$x; echo; done

################################################################################
#                                                                 GST tests

gst: 	$(GSTtests) $(OBJS) $(MAIN)
	for x in $(GSTtests); do make $$x; done

$(GSTtests): 	%: ./Testing/0/gst/%.c gst.o bst.o
	gcc $(LOPTS) $(OBJS) $(MAIN) $< -o $@

################################################################################
#                                                                AVL tests

avl: 	$(AVLtests) $(OBJS) $(MAIN)
	for x in $(AVLtests); do make $$x; done

$(AVLtests): 	%: ./Testing/0/avl/%.c avl.o bst.o
	gcc $(LOPTS) $(OBJS) $(MAIN) $< -o $@
