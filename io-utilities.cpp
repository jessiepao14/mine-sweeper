#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "io-utilities.hpp"

void emit_byte(int encoded_byte) {
    putchar(encoded_byte);
}
void emit_utf_8(int code_point) {
    if (code_point > 0x10FFFF) {
        printf("(%08X code point is too large)", code_point);
    } else if (code_point <= 0x7F) {
        emit_byte(code_point);
    } else if (code_point <= 0x7FF) {
        emit_byte(0xC0 | (code_point >> 6));
        emit_byte(0x80 | (code_point & 0x3F));
    } else if (code_point <= 0xFFFF) {
        emit_byte(0xE0 | (code_point >> 12));
        emit_byte(0x80 | ((code_point >> 6) & 0x3F));
        emit_byte(0x80 | (code_point & 0x3F));
    } else {
        emit_byte(0xF0 | (code_point >> 18));
        emit_byte(0x80 | ((code_point >> 12) & 0x3F));
        emit_byte(0x80 | ((code_point >> 6) & 0x3F));
        emit_byte(0x80 | (code_point & 0x3F));
    }
}
