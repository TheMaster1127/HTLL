#!/bin/bash

# Exit immediately if any command fails
set -e

# Check if a filename was provided
if [ -z "$1" ]; then
    echo "Usage: ./compile_and_copy.sh <your_file.htll>"
    exit 1
fi

HTLL_SOURCE_FILE="$1"
ASM_OUTPUT_FILE="finalASM_HTLL_ASM.s"

echo "--- Cleaning old files..."
rm -f "$ASM_OUTPUT_FILE"

echo "--- Compiling $HTLL_SOURCE_FILE with HTLL..."
./HTLL "$HTLL_SOURCE_FILE"

echo "--- Copying generated assembly to clipboard..."
cat "$ASM_OUTPUT_FILE" | wl-copy

echo "--- Done. Assembly code is in your clipboard."