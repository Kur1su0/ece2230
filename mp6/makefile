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

lab6 : graph.o lab6.o
	gcc -Wall -g graph.o lab6.o -o lab6 -lm -std=c99

graph.o : graph.c graph.h
	gcc -Wall -g -c graph.c -std=c99

lab6.o : lab6.c graph.h
	gcc -Wall -g -c lab6.c -std=c99

h1 : lab6.o
	@echo "expected 1 -- 2 -- 3 -- 5 with cost of 11"
	./lab6 -g 1 -h 1 -s1 -d 5 -v
h2 : lab6.o
	@echo "expected 4 -- 5 -- 6 -- 3 with cost of 21"
	./lab6 -g 1 -h2:

h3 : lab6.o
	@echo "expected 1 -- 2 -- 3 -- 5 with cost 11 and"
	@echo "expected 1 -- 6 -- 4 -- 5 with cost 20"
	./lab6 -g1 -h3 -s1 -d5

g2 : lab6.o
	@echo "expected 14 paths"
	./lab6 -g2 -h3 -n 15 -s14 -d0
g3 : lab6.o
	@echo "expected a=20 has more #avg than a=7"
	@echo "a=7"
	./lab6 -g3 -h2 -n 100 -a 7 -r 1
	@echo "a=20"
	./lab6 -g3 -h2 -n 100 -a 20 -r 1
	
#  @ prefix suppresses output of shell command
#  - prefix ignore errors
#  @command || true to avoid Make's error
#  : is shorthand for true
clean :
	rm -f *.o lab6 core
