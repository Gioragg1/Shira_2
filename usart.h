/* 
 * File:   usart.h
 * Author: George
 *
 * Created on March 21, 2016, 10:32 PM
 */

#ifndef USART_H
#define	USART_H

#define					RX_BUFF_SIZE	20
#define					RX_TIMEOUT		20				//200ms recive timeout in 10 ms units
extern unsigned char		dummy;
extern unsigned char 	rx_pointer;
extern unsigned char 	rx_timeout;
extern unsigned char 	rx_buufer[RX_BUFF_SIZE];
extern unsigned char 	data_buff[RX_BUFF_SIZE];
extern unsigned char		number_of_recived_bytes;

void init_USART(void);
#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

