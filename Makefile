-include .config

MAJOR_VER = 1
MINOR_VER = 2

TOP_DIR := $(shell pwd)
IMG_DIR := $(CONFIG_IMAGE_PATH)

CROSS_COMPILE = $(CONFIG_CROSS_COMPILE)

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS = -std=gnu99 -ffreestanding -nostdinc -nostdlib -fno-builtin -I$(TOP_DIR)/include -I$(TOP_DIR)/arch/$(CONFIG_ARCH)/include -include g-bios.h -D__GBIOS_VER__=\"$(MAJOR_VER).$(MINOR_VER)\" -O2 -Wall -Werror -march=$(CONFIG_ARCH_VER)

ASFLAGS = $(CFLAGS) -D__ASSEMBLY__

ifeq ($(CONFIG_ARCH),arm)
	CFLAGS += -mno-thumb-interwork -mabi=aapcs-linux
	# LDFLAGS = -m armelf_eabi
else ifeq ($(CONFIG_ARCH),risc-v)
	CFLAGS += -mabi=ilp32
	LDFLAGS += -m elf32lriscv
endif

ifeq ($(CONFIG_DEBUG),y)
	CFLAGS += -g
else

endif

#ifeq ($(CONFIG_VERBOSE),y)
#	CFLAGS += -DCONFIG_VERBOSE
#endif

builtin-obj = built-in.o

MAKEFLAGS = --no-print-directory

export AS CC LD OBJDUMP OBJCOPY ASFLAGS CFLAGS LDFLAGS MAKEFLAGS
export builtin-obj TOP_DIR

DEFCONFIG_LIST = $(shell ls build/configs)

include build/rules/common.mk

dir-y := arch board driver core lib

subdir-objs := $(foreach n, $(dir-y), $(n)/$(builtin-obj))

all: g-bios.elf g-bios.bin g-bios.hex
	$(CROSS_COMPILE)size g-bios.elf
	@echo

include/autoconf.h: .config
	@build/generate/autoconf.py $< $@

g-bios.bin: g-bios.elf
	$(OBJCOPY) -O binary -S $< $@

g-bios.hex: g-bios.elf
	$(OBJCOPY) -O ihex -S $< $@

g-bios.elf: $(subdir-objs)
	$(LD) $(LDFLAGS) -T board/$(CONFIG_PLAT)/memory.ld -T build/sections.ld $^ -o $@

$(subdir-objs): $(dir-y)

$(dir-y): include/autoconf.h
	@make $(obj_build)$@

.PHONY: $(dir-y)

help:
	@echo 'usage: make <device>_defconfig'
	@echo 'the available <device>_defconfig list:'
	@for cfg in $(DEFCONFIG_LIST); do echo "   $$cfg"; done
	@echo

# FIXME
$(DEFCONFIG_LIST):
	@echo "configure for board '$(@:%_defconfig=%)'"
#	@./build/generate/defconfig.py $@
	@cp -v build/configs/$@ .config
	@echo

install: g-bios.bin
	@mkdir -p $(IMG_DIR)
	@for fn in $^; do \
		cp -v $$fn $(IMG_DIR); \
	done
	@echo

clean:
	@for dir in $(dir-y); do \
		make $(obj_build)$$dir clean; \
	 done
	@rm -vf g-bios.*
	@echo

distclean: clean
	@rm -vf .config include/autoconf.h
	@echo

clear:
	@./build/clearup.sh

.PHONY: $(dir-y)
