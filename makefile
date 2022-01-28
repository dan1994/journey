include base.mk

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
	@gdb

.PHONY: run
run: compile
	$(call log_run,Qemu $(patsubst ../../%,%,${TARGET}))
	${Q}qemu-system-x86_64 -hda $(TARGET) 2> /dev/null

.PHONY: view
view: compile
	@ndisasm $(TARGET) | less

.PHONY: compile
compile: $(TARGET)

$(TARGET): $(BOOTLOADER) $(KERNEL)
	$(call log_message,Creating Kernel Image)
	$(call log_image,$@)
	${Q}rm -f $@
	${Q}dd if=$(BOOTLOADER) >> $@ 2> /dev/null
	${Q}dd if=$(KERNEL) >> $@ 2> /dev/null
	${Q}dd if=/dev/zero bs=512 count=100 >> $@ 2> /dev/null
	$(call log_message,Image Created)

.PHONY: $(BOOTLOADER)
$(BOOTLOADER):
	${Q}$(MAKE) -C $(SRC_DIR)/boot ${NO_PRINT_DIRECTORY}

.PHONY: $(KERNEL)
$(KERNEL):
	${Q}$(MAKE) -C $(SRC_DIR)/kernel ${NO_PRINT_DIRECTORY}

.PHONY: clean
clean:
	$(call log_clean)
	${Q}rm -rf $(BUILD_DIR) $(BIN_DIR)
