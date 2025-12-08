#!/bin/bash
# build_test.sh
# Full HTLL build workflow including program execution and size

# 1. Remove old binary if exists
rm -f my_program

# 2. Compile HTLL source (first step)
./HTLL HTLL.htll

# 3. Rename generated assembly file
mv finalASM_HTLL_ASM.s HTLL.s

# 4. Compile HTLL library
g++ -shared -fPIC HTLL.cpp wrapper.cpp -o libHTLL-lib.so

# 5. Compile HTLL to assembly
./HTLL HTLL.htll

# 6. Assemble with FASM
fasm HTLL.s HTLL.o

# 7. Link executable
ld -o HTLL HTLL.o -L. -lHTLL-lib --dynamic-linker /lib64/ld-linux-x86-64.so.2 -rpath '$ORIGIN'

# 8. Compile your test program to assembly
./HTLL my_program.htll

# 9. Assemble the test program
fasm my_program.s

# 10. Run the test program
echo "Running my_program:"
./my_program

# 11. Print size of my_program
echo "Size of my_program:"
ls -lh my_program
