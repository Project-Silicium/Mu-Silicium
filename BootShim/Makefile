ARCH              = arm64
TARGET            = aarch64-linux-gnu
CC                = clang
CFLAGS            = --target=$(TARGET)
OBJCOPY           = llvm-objcopy

all: BootShim.elf BootShim.bin

BootShim.bin: BootShim.elf
	$(OBJCOPY) -O binary $< $@

BootShim.elf: BootShim.S
	$(CC) ${CFLAGS} -c $< -o $@ -DCREATE_FDT_POINTER=$(CREATE_FDT_POINTER) -DFDT_POINTER_ADDRESS=$(FDT_POINTER_ADDRESS) -DUEFI_BASE=$(UEFI_BASE) -DUEFI_SIZE=$(UEFI_SIZE)

BootShim.S:
