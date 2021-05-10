SRC_DIR=src
BUILD_DIR=build
BIN_DIR=bin

BOOTLOADER=$(BIN_DIR)/boot/boot.bin
KERNEL=$(BIN_DIR)/kernel/kernel.bin
TARGET=$(BIN_DIR)/os.bin

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
compile: $(TARGET)

$(TARGET): $(BOOTLOADER) $(KERNEL)
	rm -f $@
	dd if=$(BOOTLOADER) >> $@
	dd if=$(KERNEL) >> $@
	dd if=/dev/zero bs=512 count=100 >> $@

.PHONY: $(BOOTLOADER)
$(BOOTLOADER):
	make -C $(SRC_DIR)/boot

.PHONY: $(KERNEL)
$(KERNEL):
	make -C $(SRC_DIR)/kernel

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
