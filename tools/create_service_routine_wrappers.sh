#!/bin/bash

set -eu
[[ "${V:-0}" != "0" ]] && set -x

ISR_DECLARATION_FILE="${1}"
TARGET_PATH="${2}"

interrupts=(
    $(
        gcc -E "${ISR_DECLARATION_FILE}" 2>/dev/null |
        grep "extern \"C\" void isr_" |
        sed -r 's/^\s*extern "C" void (\w+)_wrapper\([^)]*\).*$/\1/'
    )
)

mkdir -p "$(dirname ${TARGET_PATH})"

echo "section .asm
" > ${TARGET_PATH}

for interrupt in "${interrupts[@]}"; do
    echo "global ${interrupt}_wrapper
extern ${interrupt}
${interrupt}_wrapper:
    pushad
    cld
    call ${interrupt}
    popad
    iret
" >> ${TARGET_PATH}
done
