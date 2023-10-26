ROOT_DIR=../..

include ${ROOT_DIR}/base.mk

INCLUDE_DIR=$(ROOT_DIR)/include/kernel
SRC_DIR=.
BUILD_DIR=$(ROOT_DIR)/build/kernel
BIN_DIR=$(ROOT_DIR)/bin/kernel

# Use cut to remove the ./ prefix printed by find
SOURCES=$(shell find $(SRC_DIR) -type f -regextype posix-egrep -iregex '.*\.(asm|c(pp)?)' | cut -c 3-)
OBJECTS=$(SOURCES:%=$(BUILD_DIR)/%.o)
TARGET_ELF=$(BUILD_DIR)/kernel.o
TARGET=$(BIN_DIR)/kernel.bin

TOOLCHAIN_PREFIX=i686-elf-
CC=$(TOOLCHAIN_PREFIX)gcc
CXX=$(TOOLCHAIN_PREFIX)g++
LD=$(TOOLCHAIN_PREFIX)ld

# -ffreestanding - No supporting environment such as libc and program execution
# doesn't necessarily start at main
# -fstrength-reduce - Somehow related to loop unrolling
# -fno-builtin - Don't allow GCC to replace certain function calls with its
# internal optimized version.
# -fno-use-cxa-atexit - Disable the libc mechanism that automatically calls
# destructors when exit is called. We call destructors ourselves. Leaving this
# flag out causes linkage errors, as it searches for libc symbols.
# -fno-threadsafe-statics - Disable the libc mechanism that protects the
# creation of local statics in a multi-threaded environment. This mechanism
# calls __cxa_guard_acquire and __cxa_guard_release which are complex.

FLAGS_SYNTAX=-Werror -Wall -Wno-unused-function -Wno-unused-label -Wno-unused-parameter
FLAGS_ENV=-ffreestanding -nostdlib -nostartfiles -nodefaultlibs -fno-use-cxa-atexit -m32
FLAGS_OPTIMIZATION=-O0 -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin -fno-threadsafe-statics

CFLAGS_SYNTAX=-std=gnu99 $(FLAGS_SYNTAX)
CFLAGS_ENV=$(FLAGS_ENV)
CFLAGS_OPTIMIZATION=$(FLAGS_OPTIMIZATION)

CXXFLAGS_SYNTAX=-std=c++2a $(FLAGS_SYNTAX)
CXXFLAGS_ENV=-fno-rtti -fno-exceptions $(FLAGS_ENV)
CXXFLAGS_OPTIMIZATION=$(FLAGS_OPTIMIZATION)

ASMFLAGS=-f elf -g
CFLAGS=-I $(INCLUDE_DIR) -I $(INCLUDE_DIR)/std -g $(CFLAGS_SYNTAX) $(CFLAGS_ENV) $(CFLAGS_OPTIMIZATION) -D_DEBUG
CXXFLAGS=-I $(INCLUDE_DIR) -I $(INCLUDE_DIR)/std -g $(CXXFLAGS_SYNTAX) $(CXXFLAGS_ENV) $(CXXFLAGS_OPTIMIZATION) -D_DEBUG
LINKFLAGS=-g -relocatable
BINFLAGS=$(CFLAGS)

.PHONY: all
all: compile

.PHONY: view
view: compile
	@ndisasm $(TARGET) | less

.PHONY: compile
compile: message $(BUILD_DIR) $(BIN_DIR) $(TARGET)
	$(call log_message,Kernel Build Complete)

$(TARGET): $(TARGET_ELF) $(SRC_DIR)/linker.ld
	$(call log_link,$(patsubst ../../%,%,$@))
	${Q}$(CXX) $(BINFLAGS) -T $(SRC_DIR)/linker.ld $< -o $@

$(TARGET_ELF): $(OBJECTS)
	$(call log_link,$(patsubst ../../%,%,$@))
	${Q}$(LD) $(LINKFLAGS) $^ -o $@

# ISR methods require the `-mgeneral-regs-only` flag to be compiled correctly.
$(BUILD_DIR)/interrupts/isr.cpp.o: interrupts/isr.cpp
	@mkdir -p $(@D)
	$(call log_compile,src/kernel/$<)
	${Q}$(CXX) -mgeneral-regs-only $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	$(call log_compile,src/kernel/$<)
	${Q}$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(@D)
	$(call log_compile,src/kernel/$<)
	${Q}$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.asm.o: %.asm
	@mkdir -p $(@D)
	$(call log_compile,src/kernel/$<)
	${Q}nasm $(ASMFLAGS) $< -o $@

$(BUILD_DIR):
	@mkdir -p $@

$(BIN_DIR):
	@mkdir -p $@

.PHONY: message
message:
	$(call log_message,Kernel Build Start)

.PHONY: clean
clean:
	$(call log_clean)
	${Q}rm -rf $(BUILD_DIR) $(BIN_DIR)
