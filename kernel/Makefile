HOST ?= $(shell ../default-host.sh)
HOSTARCH := $(shell ../target-triplet-to-arch.sh $(HOST))

EXE := creeks.bin

CFLAGS ?= -g
CPPFLAGS ?=
LDFLAGS ?=
LIBS ?=

DESTDIR ?=
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
BOOTDIR ?= $(EXEC_PREFIX)/boot
INCLUDEDIR ?= $(PREFIX)/include

CFLAGS := $(CFLAGS) -ffreestanding -fbuiltin -Wall -Wextra
CPPFLAGS := $(CPPFLAGS) -D__is_creeks_kernel -Iinclude
LDFLAGS := $(LDFLAGS)
LIBS := $(LIBS) -nostdlib -lk -lgcc

ARCHDIR := arch/$(HOSTARCH)

LINKER := $(ARCHDIR)/linker.ld

include $(ARCHDIR)/make.config

CFLAGS := $(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS := $(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS := $(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS := $(LIBS) $(KERNEL_ARCH_LIBS)

OBJS := $(KERNEL_ARCH_OBJS) kernel/kernel.o kernel/gdt.o kernel/idt.o kernel/interrupt.o \
		kernel/pic.o kernel/memory.o

CRTI_OBJ := $(ARCHDIR)/crti.o
CRTBEGIN_OBJ := $(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ := $(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ := $(ARCHDIR)/crtn.o

ALL_OUR_OBJS := $(CRTI_OBJ) $(OBJS) $(CRTN_OBJ)

OBJ_LINK_LIST := $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)

all: $(EXE)

.PHONY: all clean install install-headers install-kernel

$(EXE): $(OBJ_LINK_LIST) $(LINKER)
	$(CC) -T $(LINKER) -o $@ $(CFLAGS) $(OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu99 $(CFLAGS) $(CPPFLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f $(EXE) $(OBJS) $(ALL_OUR_OBJS) *.o */*.o */*/*.o

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv include $(DESTDIR)$(INCLUDEDIR)

install-kernel: $(EXE)
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp creeks.bin $(DESTDIR)$(BOOTDIR)
