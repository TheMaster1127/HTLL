#!/bin/bash
# test.sh

# 1. Compile your test program to assembly
./HTLL my_program.htll

# 2. Assemble the test program
fasm my_program.s

# 3. Run the test program
echo "Running my_program:"
./my_program

# 4. Print size of my_program
echo "Size of my_program:"
ls -lh my_program
