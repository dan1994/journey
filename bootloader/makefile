.PHONY: all
all: compile

.PHONY: gdb
gdb: compile
	gdb -x gdb_startup.txt

.PHONY: run
run: compile
	qemu-system-x86_64 -hda boot.bin

.PHONY: view
view: compile
	ndisasm boot.bin | less

.PHONY: compile
compile: boot.bin

boot.bin: boot.asm message.txt
	nasm -f bin $< -o $@
	dd if=./message.txt >> $@
	dd if=/dev/zero bs=512 count=1 >> $@

message.txt:
	echo 'Hello, World! This is my awesome message!!!' > $@

.PHONY: clean
clean:
	rm boot.bin message.txt
