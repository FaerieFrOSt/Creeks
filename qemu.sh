#!/bin/sh
set -e
. ./iso.sh

debug=""

if [ ! $1 ]; then
	set $1=""
fi

if [ $1 = "-g" ]; then
	debug="-s -S /dev/zero"
fi

qemu-system-$(./target-triplet-to-arch.sh $HOST) $debug -cdrom creeks.iso
