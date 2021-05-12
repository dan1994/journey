ROOT_DIR=../..
INCLUDE_DIR=$(ROOT_DIR)/include
SRC_DIR=.
BUILD_DIR=$(ROOT_DIR)/build/kernel
BIN_DIR=$(ROOT_DIR)/bin/kernel

SOURCES=$(shell find $(SRC_DIR) -type f -regextype posix-egrep -iregex '.*\.(asm|c)')
OBJECTS=$(SOURCES:%=$(BUILD_DIR)/%.o)
TARGET_ELF=$(BUILD_DIR)/kernel.o
TARGET=$(BIN_DIR)/kernel.bin

TOOLCHAIN_PREFIX=/opt/cross/bin/i686-elf-
GCC=$(TOOLCHAIN_PREFIX)gcc
LD=$(TOOLCHAIN_PREFIX)ld

# -ffreestanding - No supporting environment such as libc and program execution
# doesn't necessarily start at main
# -fstrength-reduce - Somehow related to loop unrolling
# -fno-builtin - Don't allow GCC to replace certain function calls with its
# internal optimized version.

CFLAGS_SYNTAX=-std=gnu99 -Werror -Wall -Wno-unused-function -Wno-unused-label -Wno-cpp -Wno-unused-parameter
CFLAGS_ENV=-ffreestanding -nostdlib -nostartfiles -nodefaultlibs
CFLAGS_OPTIMIZATION=-O0 -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin

ASMFLAGS=-f elf -g
CFLAGS=-I $(INCLUDE_DIR) -g $(CFLAGS_SYNTAX) $(CFLAGS_ENV) $(CFLAGS_OPTIMIZATION)
LINKFLAGS=-g -relocatable
BINFLAGS=$(CFLAGS)

.PHONY: all
all: compile

.PHONY: view
view: compile
	ndisasm $(TARGET) | less

.PHONY: compile
compile: $(BUILD_DIR) $(BIN_DIR) $(TARGET)

$(TARGET): $(TARGET_ELF) $(SRC_DIR)/linker.ld
	$(GCC) $(BINFLAGS) -T $(SRC_DIR)/linker.ld $< -o $@

$(TARGET_ELF): $(OBJECTS)
	$(LD) $(LINKFLAGS) $^ -o $@

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(@D)
	$(GCC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.asm.o: %.asm
	mkdir -p $(@D)
	nasm $(ASMFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)