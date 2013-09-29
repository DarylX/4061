/* CSci4061 F2013 Assignment 1
 * login: poon0045
 * date: 9/25/2013
 * name: Vivian Poon, Daryl Xayavong
 * id: 4186433, 3948244 */

The purpose of this program and what it does:
This program (graphexec) will execute a graph of user programs in parallel using fork, exec and wait in a controlled fashion. A data dependency specifies that a program requires input from its predecessor(s) before it can execute. Make creates a dependence graph of all the files to be compiled at runtime. Independent files are compiled in parallel while dependent files are compiled once all its control dependencies have been resolved.

How to compile and use from the shell:
Included with the program files is a Makefile that allows for easy compilation from the shell.
From the terminal, cd into the correct directory that contains this Makefile.
A "make" command will compile the graphexec program.
It can be run with a graph file by typing "./graphexec some-graph-file.txt"
After the program runs, excess files can be removed with a "make clean" command.

Example syntax:
>> make
>> ./graphexec tests/testgraph1.txt
>> make clean 