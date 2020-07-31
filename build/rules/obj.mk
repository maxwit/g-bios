include .config

include build/rules/common.mk

path := $(patsubst %/,%,$(path))
include $(path)/Makefile

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.S
	$(CC) $(ASFLAGS) -o $@ -c $<

PHONY := $(foreach n, $(PHONY), $(path)/$(n))

obj-y := $(foreach n, $(obj-y), $(path)/$(n))
subdir-obj := $(foreach n, $(dir-y), $(path)/$(n)/built-in.o)

builtin-obj := $(path)/built-in.o

all: $(dir-y) $(builtin-obj)

$(builtin-obj): $(obj-y) $(subdir-obj)
	$(LD) $(LDFLAGS) -r $^ -o $@

PHONY += $(dir-y)

$(dir-y):
	@make $(obj_build)$(path)/$@

clean: .config
	@for dir in $(dir-y); do \
		make $(obj_build)$(path)/$$dir clean; \
	 done
	@rm -vf $(path)/*.o

.PHONY: $(PHONY) FORCE
