ARCH              = arm64
TARGET            = aarch64-linux-gnu
CROSS_COMPILE     = $(TARGET)-
CC                = $(CROSS_COMPILE)gcc
OBJCOPY           = $(CROSS_COMPILE)objcopy

all: PoC.elf PoC.bin

PoC.bin: PoC.elf
	$(OBJCOPY) -O binary $< $@

PoC.elf: PoC.S
	$(CC) -c $< -o $@