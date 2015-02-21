SYSTEM_HEADER_PROJECTS="libc kernel"
PROJECTS="libc kernel"

export PATH="/home/faerie/C/creeks/cross/bin:$PATH"
export MAKE=${MAKE:-make}
export HOST=${HOST:-$(./default-host.sh)}
export AR=${HOST}-ar
export AS=${HOST}-as
export CC=${HOST}-gcc

export PREFIX=/usr
export EXEC_PREFIX=$PREFIX
export BOOTDIR=/boot
export LIBDIR=$EXEC_PREFIX/lib
export INCLUDEDIR=$PREFIX/include

export CFLAGS='-g'
export CPPFLAGS=''

# configure the cross-compiler to use the system root
export CC="$CC --sysroot=$PWD/sysroot"

# workaround with the elf format
if echo "$HOST" | grep -Eq -- 'elf($|-)'; then
	export CC="$CC -isystem=$INCLUDEDIR"
fi
