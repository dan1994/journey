# The following python code builds and run the add-symbol-file command. The reason we need python
# stems from the fact that the binary we debug is raw and the symbols are found in a separate elf
# but not at the same offsets. Using python we solve the following two problems:
# 1. We calculate the correct offset of each section by taking into account the order of the
# sections in the raw binary as specified in the linker script, and their size as specified in the
# elf object file.
# 2. We calculate the offset of vague linkage sections[1], which are not specified in the linker
# script, but contain information about symbols found in other sections (e.g. .text). This allows
# us to correctly debug templates for example.
# [1] https://gcc.gnu.org/onlinedocs/gcc/Vague-Linkage.html
python
from gdb import execute
from pathlib import Path
from subprocess import check_output
from re import search
from typing import List, Dict
from copy import deepcopy


INITIAL_OFFSET_IN_MEMORY = 0x100000


class Section:

    def __init__(self, name: str, offset: int, size: int):
        self.name = name
        self.offset = offset
        self.size = size


def get_linker_sections_names(linker_script_path: Path) -> List[str]:
    with open(linker_script_path) as f:
        lines = f.readlines()

    linker_sections_regex = r"(?P<name>\.\w+)\s*:.*{"

    names = []
    for line in lines:
        match = search(linker_sections_regex, line)
        if not match:
            continue

        names.append(match.group("name"))

    return names


def get_sections_on_disk(object_path: Path) -> Dict[str, Section]:
    command = f"readelf -S -W {object_path}".split()
    lines = check_output(command).decode().split("\n")

    section_regex = r" (?P<name>\.[\w\.]+)\s+\w+\s+\w+\s+(?P<offset>\w+)\s+(?P<size>\w+)"

    sections = {}
    for line in lines:
        match = search(section_regex, line)
        if not match:
            continue

        name = match.group("name")
        offset = int(match.group("offset"), base=16)
        size = int(match.group("size"), base=16)

        sections[name] = Section(name, offset, size)

    return sections


def to_sections_in_memory(
    linker_sections_names: List[str],
    sections_on_disk: Dict[str, Section],
) -> List[Section]:
    sections: Dict[str, Section] = {}

    offset = INITIAL_OFFSET_IN_MEMORY
    for linker_section in linker_sections_names:
        sections[linker_section] = deepcopy(sections_on_disk[linker_section])
        sections[linker_section].offset = offset
        offset += align(sections_on_disk[linker_section].size)

    text_section_offset_diff = sections[".text"].offset - \
        sections_on_disk[".text"].offset
    for name, section in sections_on_disk.items():
        if not section.name.startswith(".text."):
            continue

        sections[name] = deepcopy(section)
        sections[name].offset += text_section_offset_diff

    return sections.values()


def align(address: int) -> int:
    return 4096 * int((address + 4095) / 4096)


def sections_to_args(sections: List[Section]) -> str:
    return " ".join([f"-s {section.name} 0x{section.offset:x}" for section in sections])


root_path = Path(".").absolute()
linker_script_path = root_path / "src" / "kernel" / "linker.ld"
object_path = root_path / "build" / "kernel" / "kernel.o"

linker_sections_names = get_linker_sections_names(linker_script_path)
sections_on_disk = get_sections_on_disk(object_path)
sections_in_memory = to_sections_in_memory(
    linker_sections_names, sections_on_disk)

command = f"add-symbol-file {object_path} {sections_to_args(sections_in_memory)}"

execute(command)
end

target remote | qemu-system-i386 -drive file=bin/os.bin,format=raw,index=0,media=disk -gdb stdio 2> /dev/null
set disassembly-flavor intel
layout src
set confirm off
focus cmd
b main
c
