#ifndef PORTS_H
#define PORTS_H

static inline unsigned char inb(unsigned short int __port) {
    unsigned char _v;
    __asm__ __volatile__ ("inb %w1,%0":"=a" (_v):"Nd" (__port));
    return _v;
}

static inline unsigned short int inw(unsigned short int __port) {
    unsigned short _v;
    __asm__ __volatile__ ("inw %w1,%0":"=a" (_v):"Nd" (__port));
    return _v;
}

static inline unsigned int inl(unsigned short int __port) {
    unsigned int _v;
    __asm__ __volatile__ ("inl %w1,%0":"=a" (_v):"Nd" (__port));
    return _v;
}

static inline void outb(unsigned char __value, unsigned short int __port) {
    __asm__ __volatile__ ("outb %b0,%w1": :"a" (__value), "Nd" (__port));
}

static inline void outw(unsigned short int __value, unsigned short int __port) {
    __asm__ __volatile__ ("outw %w0,%w1": :"a" (__value), "Nd" (__port));
}

static inline void outl(unsigned int __value, unsigned short int __port) {
    __asm__ __volatile__ ("outl %0,%w1": :"a" (__value), "Nd" (__port));
}

/**
 * Lire un octet d'un port
 */
static inline unsigned char port_byte_in(unsigned short port) {

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

static inline void port_byte_out(unsigned short port, unsigned char data) {
    /* Notice how here both registers are mapped to C variables and
     * nothing is returned, thus, no equals '=' in the asm syntax 
     * However we see a comma since there are two variables in the input area
     * and none in the 'return' area
     */
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

static inline unsigned short port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

static inline void port_word_out(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

#endif