SRC_DIR=.
BIN_DIR=../../bin/boot
SOURCES=$(SRC_DIR)/boot.asm
IMAGE=$(BIN_DIR)/boot.bin

export PREFIX:="$(HOME)/opt/cross"
export TARGET:=i686-elf
export PATH:="$(PREFIX)/bin:$(PATH)"

.PHONY: all
all: compile

.PHONY: gdb
gdb: compile
	gdb -x $(SRC_DIR)/gdb_startup.txt

.PHONY: run
run: compile
	qemu-system-x86_64 -hda $(IMAGE)

.PHONY: view
view: compile
	ndisasm $(IMAGE) | less

.PHONY: compile
compile: $(BIN_DIR) $(IMAGE)

$(IMAGE): $(SOURCES)
	nasm -f bin $< -o $@

$(BIN_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
