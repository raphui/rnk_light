export

CONFIG := $(wildcard .config)
ifneq ($(CONFIG),)
include $(CONFIG)
endif

ifeq ($(CONFIG_CPU_ARMV7M),y)
ARMV=armv7m
endif

ifeq ($(CONFIG_CPU_ARM_CORTEX_M4),y)
MCPU=cortex-m4
endif

ifeq ($(CONFIG_CPU_ARM_CORTEX_M7),y)
MCPU=cortex-m7
endif

KCONFIG_AUTOHEADER=config.h

ifeq (${MAKELEVEL}, 0)
INCLUDES	+= -I$(KERNEL_BASE)/include

INCLUDES	+= -include $(KERNEL_BASE)/config.h
ASFLAGS	:= -g $(INCLUDES) -D__ASSEMBLY__ -mcpu=$(MCPU) -mthumb
CFLAGS  :=  -Wall -fno-builtin -ffunction-sections -mcpu=$(MCPU) -mthumb -nostdlib -nostdinc -g $(INCLUDES)
CFLAGS += -MD -MP

ifeq ($(CONFIG_UNWIND),y)
CFLAGS += -funwind-tables
endif

#CFLAGS  :=  -Wall -mlong-calls -fpic -ffunction-sections -mcpu=arm7tdmi -nostdlib -g $(INCLUDES)
#CFLAGS  :=  -Wall -mlong-calls -fpic -ffreestanding -nostdlib -g $(INCLUDES)
LDFLAGS	:= -g $(INCLUDES) -nostartfiles -nostdlib -Wl,-Map=kernel.map#-Wl,--gc-sections

LDSFLAGS := $(INCLUDES)

CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
AR := $(CROSS_COMPILE)ar
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
LDS := $(CROSS_COMPILE)gcc -E -P -C
DTC := dtc

endif

subdirs-y := arch drv kernel ldscripts lib mm utils

linker_files = rnk.lds

ifeq (${MAKELEVEL}, 0)

.PHONY: all clean

all: conf libkernel.a

conf:
	@ln -f -s $(KERNEL_BASE)/arch/arm/include $(KERNEL_BASE)/include/arch
	@ln -f -s $(KERNEL_BASE)/arch/arm/$(ARMV)/include $(KERNEL_BASE)/include/$(ARMV)

cscope:
	@@echo "GEN " $@
	@cscope -b -q -k -R

libkernel.a: conf config.h
	rm -f objects.lst
	rm -f extra_objects.lst
	$(MAKE) -f tools/Makefile.common dir=. all
	 $(AR) rc -o $@ \
		`cat objects.lst | tr '\n' ' '`
 
include $(wildcard *.d)
 
clean:
	$(MAKE) -f tools/Makefile.common dir=. $@
	$(RM) $(OBJS) libkernel.a kernel.img
	$(RM) boards/board.h
	$(RM) include/arch
	$(RM) include/$(ARMV)
	$(RM) include/mach
 
dist-clean: clean
	$(RM) `find . -name *.d`

tests:
	tools/make_apps.sh

endif

.config:
	echo "ERROR: No config file loaded."
	exit 1

%_defconfig:
	cp arch/${ARCH}/configs/$@ .config
	echo "Loading $@..."

config.h: .config
	@bash tools/generate_config.sh

menuconfig: $(KCONFIG)/kconfig-mconf
	$(KCONFIG)/kconfig-mconf Kconfig

nconfig: $(KCONFIG)/kconfig-nconf
	$(KCONFIG)/kconfig-nconf Kconfig

config: tools/kconfig-frontends/frontends/conf/conf
	tools/kconfig-frontends/frontends/conf/conf Kconfig

tools/kconfig-frontends/bin/kconfig-%:
	$(MAKE) -C ./tools/ $(subst tools/kconfig-frontends/bin/,,$@)

ifndef VERBOSE
.SILENT:
endif
