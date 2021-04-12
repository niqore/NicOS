/**
 * Lire un octet d'un port
 */
unsigned char port_byte_in(unsigned short port) {

    unsigned char result;
    /* Syntaxe d'assembleur inline
     * Attention, la source et la destionation sont inversés par rapport à NASM
     *
     * '"=a" (result)'; set '=' the C variable '(result)' to the value of register eax
     * '"d" (port)': map the C variable '(port)' into edx register
     *
     * Inputs and outputs are separated by colons
     */
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
    /* Notice how here both registers are mapped to C variables and
     * nothing is returned, thus, no equals '=' in the asm syntax 
     * However we see a comma since there are two variables in the input area
     * and none in the 'return' area
     */
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}