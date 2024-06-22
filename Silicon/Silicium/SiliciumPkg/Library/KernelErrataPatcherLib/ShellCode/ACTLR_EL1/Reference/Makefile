ARCH              = arm64
TARGET            = aarch64-linux-gnu
CROSS_COMPILE     = $(TARGET)-
CC                = $(CROSS_COMPILE)gcc
OBJCOPY           = $(CROSS_COMPILE)objcopy

all: ShellCode.elf ShellCode.bin ShellCode.S

ShellCode.bin: ShellCode.elf
	$(OBJCOPY) -O binary $< $@

ShellCode.elf: ShellCode.S
	$(CC) -c $< -o $@

ShellCode.S: ShellCode.c
	$(CC) -S -o $@ -O1 -fno-stack-protector $<
