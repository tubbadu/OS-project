echo '





#!/bin/sh

values="0x1A 0x2F 0x3C 0x4B 0x5E 0x6D 0x7A 0x8E 0x9D 0xAB"

# Define the variables
N=10          # Number of iterations
STARTADDR=0x4000000  # Starting address

for value in $values; do
    busybox devmem $STARTADDR 64 $value

    # Increment the address
    STARTADDR=$((STARTADDR + 8))
    
    # Increment the loop counter
    i=$((i + 1))
done

# run the operation
busybox devmem 0x4000640 w 0x1


for value in $values; do
    busybox devmem $STARTADDR 64

    # Increment the address
    STARTADDR=$((STARTADDR + 8))
    
    # Increment the loop counter
    i=$((i + 1))
done


echo "status register: $(busybox devmem 0x4000640 w)"



' > test.sh; clear; sh test.sh