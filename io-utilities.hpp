#ifndef _IO_UTILITIES_HPP
#define _IO_UTILITIES_HPP

/**
 * Helper function for sending code points > 0x7F to a UTF-8 capable terminal.
 */
void emit_utf_8(int code_point);

#endif
