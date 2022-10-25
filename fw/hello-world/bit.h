#ifndef BIT_H
#define BIT_H

#define BIT_SET(reg, bit) (reg |=  (1 << bit))
#define BIT_CLR(reg, bit) (reg &= ~(1 << bit))
#define BIT_TST(reg, bit) (reg &   (1 << bit))

#define BIT_MSK(bit) (1 << bit)

#endif