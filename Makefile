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

CFLAGS = -ffreestanding -nostdinc -nostdlib -fno-builtin -I$(TOP_DIR)/include -include g-bios.h -D__GBIOS_VER__=\"$(MAJOR_VER).$(MINOR_VER)\" -D__LITTLE_ENDIAN -O2 -Wall -Werror -mno-thumb-interwork -march=$(CONFIG_ARCH_VER) -mabi=aapcs-linux

#ifeq ($(CONFIG_VERBOSE),y)
#	CFLAGS += -DCONFIG_VERBOSE
#endif

ASFLAGS = $(CFLAGS) -D__ASSEMBLY__

# LDFLAGS = -m armelf_eabi

builtin-obj = built-in.o

MAKEFLAGS = --no-print-directory

export AS CC LD OBJDUMP OBJCOPY ASFLAGS CFLAGS LDFLAGS MAKEFLAGS
export builtin-obj TOP_DIR

# FIXME
DEFCONFIG_PATH = build/configs/arm
DEFCONFIG_LIST = $(shell cd $(DEFCONFIG_PATH) && ls *_defconfig)

include build/rules/common.mk

dir-y := arch/$(CONFIG_ARCH) core driver lib

subdir-objs := $(foreach n, $(dir-y), $(n)/$(builtin-obj))

all: include/autoconf.h $(dir-y) g-bios.bin g-bios.hex
	@echo

include/autoconf.h: .config
	@build/generate/autoconf.py $< $@

g-bios.bin: g-bios.elf
	$(OBJCOPY) -O binary -S $< $@

g-bios.hex: g-bios.elf
	$(OBJCOPY) -O ihex -S $< $@

g-bios.elf: $(subdir-objs)
	$(LD) $(LDFLAGS) -Tarch/$(CONFIG_ARCH)/$(CONFIG_PLAT)/mem.lds -T build/g-bios.lds $^ -o $@

$(dir-y):
	@make $(obj_build)$@

.PHONY: $(dir-y)

help:
	@echo 'usage: make <device>_defconfig'
	@echo 'the available <device>_defconfig list:'
	@for cfg in $(DEFCONFIG_LIST); do echo "   $$cfg"; done
	@echo

# FIXME
$(DEFCONFIG_LIST):
	@echo "configure for board \"$(@:%_defconfig=%)\""
#	@./build/generate/defconfig.py $@
	@cp -v build/configs/arm/$@ .config
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
