# The makefile for MP5.
# Type:
#   make              -- to build program lab5
#   make test   -- to run every test driver in the valgrind
#   make run          -- to run commands for performance analysis
#   make clean        -- to delete object files, executable, and core
#                        with tree level of 10 and 20.
#
# You should not need to change this file.
#
# Format for each entry
#    target : dependency list of targets or files
#    <tab> command 1
#    <tab> command 2
#    ...
#    <tab> last command
#    <blank line>   -- the list of commands must end with a blank line

lab5 : bst.o lab5.o
	gcc -Wall -g bst.o lab5.o -o lab5 -lm

bst.o : bst.c bst.h
	gcc -Wall -g -c bst.c -std=c99

lab5.o : lab5.c bst.h
	gcc -Wall -g -c lab5.c

test : lab5
	@echo "Custom test driver"
	@echo "Test:7 addition and deletion test"
	@valgrind ./lab5 -u 7
	@sleep 1s
	@clear
	
	@echo "Test:8 remove node with no children"
	@valgrind ./lab5 -u 8
	@sleep 1s
	@clear
	
	@echo "Test:9 remove head"
	@valgrind ./lab5 -u 9
	@sleep 1s
	@clear
	
	@echo "Test:10 right of parent  //75 - right"
	@valgrind ./lab5 -u 10
	@sleep 1s
	@clear
	
	@echo "Test 11:  node's left subtree max value's parent is the node to be deleted."
	@valgrind ./lab5 -u 11
	@sleep 1s
	@clear
	
	@echo "TEST 12: Poor tree Test "
	@valgrind ./lab5 -u 12
	@sleep 1s
	@echo "Test done."

	
run : lab5
	@echo "optimal 10"
	@./lab5 -o -w 10 -t 1000000
	@echo "random 10"
	@./lab5 -r -w 10 -t 1000000
	@echo "poor 10"
	@./lab5 -p -w 10 -t 1000000
	@echo "optimal 20"
	@./lab5 -o -w 20 -t 1000000
	@echo "random 20"
	@./lab5 -r -w 20 -t 1000000
	@echo "poor 20"
	@./lab5 -p -w 20 -t 1000000
command:
	@echo "make"
	@echo "make run    - run performance analysis test"
	@echo "make test   - run all unit driver tests"
	@echo "make clean"
	
#  @ prefix suppresses output of shell command
#  - prefix ignore errors
#  @command || true to avoid Make's error
#  : is shorthand for true
clean :
	rm -f *.o lab5 core
default : run.sh
	sh run.sh
