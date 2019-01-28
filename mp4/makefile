# The makefile for MP4.
# Type:
#   make         -- to build program lab3
#   make driver -- to compile testing program
#   make clean   -- to delete object files, executable, and core
#   make design  -- check for simple design errors (incomplete)
#   make list.o  -- to compile only list.o (or: use lab3.o, sas_support.o)
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

lab4 : list.o mem.o lab4.o
	gcc -Wall -g list.o mem.o lab4.o -o lab4

list.o : list.c datatypes.h list.h
	gcc -Wall -g -c list.c

mem.o : mem.c datatypes.h  mem.h
	gcc -Wall -g -c mem.c

lab4.o : lab4.c datatypes.h mem.h list.h
	gcc -Wall -g -c lab4.c


#  @ prefix suppresses output of shell command
#  - prefix ignore errors
#  @command || true to avoid Make's error
#  : is shorthand for true
design :
	@grep -e "-> *head" lab4.c mem.c ||:
	@grep -e "-> *tail" lab4.c mem.c ||:
	@grep -e "-> *current_list_size" lab4.c mem.c ||:
	@grep -e "-> *list_sorted_state" lab4.c mem.c ||:
	@grep -e "-> *next" lab4.c mem.c ||:
	@grep -e "-> *prev" lab4.c mem.c ||:
	@grep -e "-> *data_ptr" lab4.c mem.c ||:
	@grep "list_node_t" lab4.c mem.c ||:
	@grep "su_id" list.c ||:
	@grep "channel" list.c ||:
	@grep "mem_" list.c ||:

clean :
	rm -f *.o lab4 driver core a.out
