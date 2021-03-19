SRC_DIR=.
BIN_DIR=../../bin/boot
SOURCES=$(SRC_DIR)/boot.asm
TARGET=$(BIN_DIR)/boot.bin

.PHONY: all
all: compile

.PHONY: gdb
gdb: compile
	gdb -x $(SRC_DIR)/gdb_startup.txt

.PHONY: run
run: compile
	qemu-system-x86_64 -hda $(TARGET)

.PHONY: view
view: compile
	ndisasm $(TARGET) | less

.PHONY: compile
compile: $(BIN_DIR) $(TARGET)

$(TARGET): $(SOURCES)
	nasm -f bin $< -o $@

$(BIN_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
