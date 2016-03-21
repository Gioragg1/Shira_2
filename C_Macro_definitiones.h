/* 
 * File:   C_Macro_definitiones.h
 * Author: a
 *
 * Created on 11 ??????? 2013, 14:58
 */

#ifndef C_MACRO_DEFINITIONES_H
#define	C_MACRO_DEFINITIONES_H

#ifdef	__cplusplus
extern "C" {
#endif




#define TRUE   1
#define FALSE  0

#define IN     1
#define OUT    0

#define Led_ON  1
#define Led_OFF 0

#define	ON	1
#define	OFF	0

#define	on	1
#define	off     0

#define enable	1
#define	disable 0

#define EN	1
#define	DIS     0

#define	MSB	1
#define	LSB	0

// This macro extracts a low byte from a 2 byte word
#define Lo(X)   (unsigned char)(X&0x00ff)

// This macro extracts a high byte from a 2 byte word
#define Hi(X)   (unsigned char)((X>>8)&0x00ff)

//for example:   if x=0x0f neg(x) will be 0xf0
#define neg(a)          a^0xff			//A macro that returns the negative of a char

#define BIT_TEST( x, y) (( (x) & (1<<(y))) != 0)

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b)    ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b)  ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b)   ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b)  ((a) & (1<<(b)))

/* x=target variable, y=mask */
#define BITMASK_SET(x,y)    ((x) |= (y))
#define BITMASK_CLEAR(x,y)  ((x) &= (~(y)))
#define BITMASK_FLIP(x,y)   ((x) ^= (y))
#define BITMASK_CHECK(x,y)  ((x) & (y))



#ifdef	__cplusplus
}
#endif

#endif	/* C_MACRO_DEFINITIONES_H */

