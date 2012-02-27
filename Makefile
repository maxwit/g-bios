-include .config

MAJOR_VER = 1
MINOR_VER = 0

TOP_DIR := $(shell pwd)
IMG_DIR := $(CONFIG_IMAGE_PATH)

CROSS_COMPILE = $(CONFIG_CROSS_COMPILE:"%"=%)

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS = -ffreestanding -nostdinc -nostdlib -fno-builtin -I$(TOP_DIR)/include -include witrom.h -D__WITROM_VER__=\"$(MAJOR_VER).$(MINOR_VER)\" -D__LITTLE_ENDIAN -O2 -Wall -Werror -mno-thumb-interwork -march=$(CONFIG_ARCH_VER) -mabi=aapcs-linux

#ifeq ($(CONFIG_DEBUG),y)
#	CFLAGS += -DCONFIG_DEBUG
#endif

# fxime: to add "-mtune=xxx, -mfloat-abi=xxx"

ASFLAGS = $(CFLAGS) -D__ASSEMBLY__ -DCONFIG_GTH

LDFLAGS = -m armelf_linux_eabi

builtin-obj = built-in.o

MAKEFLAGS = --no-print-directory

export AS CC LD OBJDUMP OBJCOPY ASFLAGS CFLAGS LDFLAGS MAKEFLAGS
export builtin-obj TOP_DIR

# fixme
DEFCONFIG_PATH = build/configs/arm
DEFCONFIG_LIST = $(shell cd $(DEFCONFIG_PATH) && ls *_defconfig)

include build/rules/common.mk

dir-y := arch/$(CONFIG_ARCH) core driver lib

subdir-objs := $(foreach n, $(dir-y), $(n)/$(builtin-obj))

all: include/autoconf.h $(dir-y) witrom.bin witrom.dis
#all: include/autoconf.h $(dir-y) witrom.bin
	@echo

include/autoconf.h: .config
	@build/generate/autoconf.py $< $@
	@sed -i -e '/CONFIG_CROSS_COMPILE/d' -e '/CONFIG_ARCH_VER\>/d'  $@
	@sed -i '/^$$/d' $@

witrom.bin: witrom.elf
	$(OBJCOPY) -O binary -S $< $@

witrom.dis: witrom.elf
	$(OBJDUMP) -D $< > $@

witrom.elf: $(subdir-objs)
	$(LD) $(LDFLAGS) -T arch/$(CONFIG_ARCH)/witrom.lds -Ttext $(CONFIG_GTH_START_MEM) $^ -o $@

$(dir-y):
	@make $(obj_build)$@

.PHONY: $(dir-y)

# fixme
$(DEFCONFIG_LIST):
	@echo "configure for board \"$(@:%_defconfig=%)\""
	@./build/generate/defconfig.py $@
	@echo

install: witrom.bin
	@mkdir -p $(IMG_DIR)
	@for fn in $^; do \
		cp -v $$fn $(IMG_DIR); \
	done
	@echo

clean:
	@for dir in $(dir-y); do \
		make $(obj_build)$$dir clean; \
	 done
	@rm -vf witrom.*
	@echo

distclean: clean
	@rm -vf .config include/autoconf.h
	@echo

clear:
	@./build/clearup.sh

.PHONY: $(dir-y)
