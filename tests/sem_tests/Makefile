TARGET_NAME := sem_test

OBJS := sem_test.o
OBJS += $(TESTS_BASE)/common/boot-v7m.o
OBJS += $(TESTS_BASE)/common/stm32f4xx.o
OBJS += $(TESTS_BASE)/common/main.o

MCPU=cortex-m4

INCLUDES	+= -I$(KERNEL_BASE)/include
INCLUDES	+= -I$(KERNEL_BASE)/boards
INCLUDES	+= -I$(KERNEL_BASE)/lib
INCLUDES	+= -include $(KERNEL_BASE)/config.h

CFLAGS  :=  -Wall -fno-builtin -ffunction-sections
CFLAGS += -mcpu=$(MCPU) -mthumb -nostdlib $(INCLUDES)

LDFLAGS := -L$(KERNEL_BASE) -lkernel

LD_SCRIPT = $(KERNEL_BASE)/rnk.lds

CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
AR := $(CROSS_COMPILE)ar
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy

all: $(TARGET_NAME)

clean:
	$(RM) $(TARGET_NAME)
	$(RM) $(OBJS)
	
$(TARGET_NAME): $(OBJS) 
	@@echo "LD " $@
	@$(LD) -o $@ $^ $(LDFLAGS) -T$(LD_SCRIPT)
	@$(OBJCOPY) $@ -O binary $@.bin
	

%.o: %.c
	@@echo "CC " $<
	@$(CC) $(CFLAGS) -c $< -o $@
