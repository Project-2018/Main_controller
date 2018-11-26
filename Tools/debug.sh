#!/bin/bash

PORT=${1:-'/dev/ttyACM0'}
#/dev/serial/by-id/usb-Black_Sphere_Technologies_Black_Magic_Probe_DDE578CC-if00

# Go to the build outputs directory
cd $(dirname $(readlink -f $0))/../build

# Find the firmware ELF
elf=$(ls -1 *.elf)
if [ -z "$elf" ]; then
    echo "No firmware found"
    exit 1
fi

/home/richard/GCC/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-size $elf

tmpfile=fwupload.tempfile
cat > $tmpfile <<EOF
target extended-remote $PORT
mon swdp_scan
attach 1
EOF

/home/richard/GCC/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-gdb $elf -x $tmpfile
rm $tmpfile
