#ifndef BIT_H
#define BIT_H

#define BIT_SET(reg, bit) (reg |=  (1 << (bit)))
#define BIT_CLR(reg, bit) (reg &= ~(1 << (bit)))
#define BIT_TST(reg, bit) (reg &   (1 << (bit)))

#define GET_BYTE(reg, byte) ((reg >> (byte*8)) & 0xFF)
#define GET_BYTE3(reg) ((reg >> 24) & 0xFF)
#define GET_BYTE2(reg) ((reg >> 16) & 0xFF)
#define GET_BYTE1(reg) ((reg >> 8 ) & 0xFF)
#define GET_BYTE0(reg) ((reg >> 0 ) & 0xFF)
#define WRITE_BYTE(reg, byte, val) ( reg = ((reg & ~(0xFF << (byte*8))) | (val << (byte*8))) )

#define BIT_MSK(bit) (1 << bit)

#endif