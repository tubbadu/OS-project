echo '

# values="0x1A2F3C4B5E6D7A8E 0x9DAB0CDE1F23456A 0x7B8C9D0E1F2A3B4C 0x5E6F7A8B9C0D1E2F 0x4A5B6C7D8E9F0A1B 0x2C3D4E5F6A7B8C9D 0x8E9F0A1B2C3D4E5F 0x1F2A3B4C5D6E7F8A 0x3C4D5E6F7A8B9C0D 0x9E0F1A2B3C4D5E6F 0x7A8B9C0D1E2F3A4B 0x5C6D7E8F9A0B1C2D 0x4E5F6A7B8C9D0E1F 0x2A3B4C5D6E7F8A9B 0x1D2E3F4A5B6C7D8E 0x9B0C1D2E3F4A5B6C 0x8A9B0C1D2E3F4A5B 0x6C7D8E9F0A1B2C3D 0x5E6F7A8B9C0D1E2F 0x3A4B5C6D7E8F9A0B"
values="0x1 0x0 0x2 0x0 0x3 0x0 0x4 0x0 0x5 0x0 0x6 0x0 0x7 0x0 0x8 0x0"
ARCH=$(uname -m)
if [[ "$ARCH" == "arm"* || "$ARCH" == "aarch64" ]]; then
    echo "ARM architecture detected"
    STARTADDR=0x090c0000  # Starting address ARM
elif [[ "$ARCH" == "riscv"* ]]; then
    echo "RISCV architecture detected"
    STARTADDR=0x04000000  # Starting address RISCV
fi

STATUSADDR=$((STARTADDR + 0x640))

for value in $values; do
    
    echo "number:  $value"
    echo "address: $STARTADDR"
    busybox devmem $STARTADDR 64 $value
    
    # Increment the address
    STARTADDR=$((STARTADDR + 8))
    
    # Increment the loop counter
    i=$((i + 1))
done

# run the operation
busybox devmem $STATUSADDR w 0x1


for value in $values; do

    echo "address: $STARTADDR"
    busybox devmem $STARTADDR 64
    
    # Increment the address
    STARTADDR=$((STARTADDR + 8))
    
    # Increment the loop counter
    i=$((i + 1))
done


echo "status register: $(busybox devmem $STATUSADDR w)"

' > test.sh; clear; sh test.sh