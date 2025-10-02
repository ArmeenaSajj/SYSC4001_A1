# SYSC4001_A1
The objective of this section is to build a small simulator of an interrupt system, which could be
used for performance analysis of different parts of the interrupt process. This simulator will also
be used in Assignment 2 and 3.
A template code can be downloaded from here: https://github.com/sysc4001/Assignment-1
The template contains the following files:
- interrupts.cpp
- interrupts.hpp
- build.sh
- trace.txt
- vector_table.txt
- device_table.txt
You are expected to edit interrupts.cpp with your simulation code according to the instructions
below. Go through interrupts.hpp to get an idea of all the helper functions you have at your
disposal.
The build.sh is a script file. Run it (using source build.sh) to compile your code and to
generate the executable in the 'bin' directory. trace.txt is an example input file we provide you. Feel
free to make more input files as necessary to solve the assignment. vector_table.txt and
device_table.txt are the vector table file and device table file (explained more below), feel free to
play around with the values in these files to see how the output changes.
