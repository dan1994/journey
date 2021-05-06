SRC_DIR=.
BUILD_DIR=../../build/kernel
BIN_DIR=../../bin/kernel

SOURCES=$(shell find $(SRC_DIR) -type f -iname '*.asm')
OBJECTS=$(SOURCES:%=$(BUILD_DIR)/%.o)
TARGET_ELF=$(BUILD_DIR)/kernel.o
TARGET=$(BIN_DIR)/kernel.bin

TOOLCHAIN_PREFIX=/opt/cross/bin/i686-elf-
GCC=$(TOOLCHAIN_PREFIX)gcc
LD=$(TOOLCHAIN_PREFIX)ld

.PHONY: all
all: compile

.PHONY: view
view: compile
	ndisasm $(TARGET)

.PHONY: compile
compile: $(BUILD_DIR) $(BIN_DIR) $(TARGET)

$(TARGET): $(TARGET_ELF) $(SRC_DIR)/linker.ld
	$(GCC) -ffreestanding -O0 -nostdlib -T $(SRC_DIR)/linker.ld $< -o $@

$(TARGET_ELF): $(OBJECTS)
	$(LD) -g -relocatable $^ -o $@

$(BUILD_DIR)/%.asm.o: %.asm
	nasm -f elf -g $< -o $@

$(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)