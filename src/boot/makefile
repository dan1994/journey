ROOT_DIR=../..

include ${ROOT_DIR}/base.mk

SRC_DIR=.
BIN_DIR=../../bin/boot
SOURCES=$(SRC_DIR)/boot.asm
TARGET=$(BIN_DIR)/boot.bin

.PHONY: all
all: compile

.PHONY: gdb
gdb: compile
	@gdb -q 2> /dev/null

.PHONY: run
run: compile
	$(call log_run,Qemu $(patsubst ../../%,%,${TARGET}))
	${Q}qemu-system-x86_64 -hda $(TARGET) 2> /dev/null

.PHONY: view
view: compile
	@ndisasm $(TARGET) | less

.PHONY: compile
compile: message $(BIN_DIR) $(TARGET)
	$(call log_message,Bootloader Build Complete)

$(TARGET): $(SOURCES)
	$(call log_compile,src/boot/$<)
	${Q}nasm -f bin $< -o $@

$(BIN_DIR):
	@mkdir -p $@

.PHONY: message
message:
	$(call log_message,Bootloader Build Start)

.PHONY: clean
clean:
	$(call log_clean)
	${Q}rm -rf $(BIN_DIR)
