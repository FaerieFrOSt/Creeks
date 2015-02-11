#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/creeks.bin isodir/boot/creeks.bin
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "creeks" {
	multiboot /boot/creeks.bin
}
EOF
grub-mkrescue -o creeks.iso isodir
