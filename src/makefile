BUILD_DIR=../build
BIN_DIR=../bin

BOOTLOADER=$(BIN_DIR)/boot/boot.bin
KERNEL=$(BIN_DIR)/kernel/kernel.bin
TARGET=$(BIN_DIR)/os.bin

.PHONY: all
all: $(TARGET)

$(TARGET): $(BOOTLOADER) $(KERNEL)
	rm -f $(TARGET)
	dd if=$(BOOTLOADER) >> $@
	dd if=$(KERNEL) >> $@
	dd if=/dev/zero bs=512 count=100 >> $@

$(BOOTLOADER):
	make -C boot

$(KERNEL):
	make -C kernel

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
