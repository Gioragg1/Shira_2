#include "xc.h"
#include "usart.h"

unsigned char 	rx_buufer[RX_BUFF_SIZE];
unsigned char 	data_buff[RX_BUFF_SIZE];
unsigned char 	rx_pointer = 0;
//unsigned char		rx_data_Ready = 0;			//indicates a data is ready to be handeled in rx_buufer 
unsigned char		dummy;							//a temp reg for bad data
unsigned char		number_of_recived_bytes;


unsigned char 	rx_timeout;
//unsigned char 	check_sum;						//check_sum register
unsigned char 	enable_sending_data;

#define	NO_TERMINAL_DATA	0
#define SEND_TERMINAL_DATA	1

bit send_data_flag; 									//bit indicting pc asked the pic to send data
/* Serial initialization */
//-----------------------------------------------------------------------------
void init_USART(void)
//-----------------------------------------------------------------------------
	{
	TRISC7=1;
	TRISC6=1;

//#ifdef FAB_RTS_RECIVER
//	SPBRG =12;					//19200 4Mhz
//#else
	//SPBRG =25;					//9600  4Mhz
//#endif 
        
	TXSTA = 0b00100100;
	RCSTA = 0b10010000;

//low speed brg16 =0    /////////////////////////////
//	SPBRG =25;				//9600  4Mhz
//	SPBRG =12;					//19200 4Mhz

	//SPBRG =103;				//9600  4Mhz
	//SPBRG =51;					//19200 4Mhz
	SPBRG =8;					//115.200 4Mhz
	BAUDCON1bits.BRG16 = 1;
    //BRG16=1;

	RCIF=0;
	RCIE=1;
	}	
//-----------------------------------------------------------------------------
void putch(unsigned char tx_byte)  // output one byte for printf() func
//-----------------------------------------------------------------------------
	{
	while(!TRMT1);				// set when register is empty
//DelayMs(5);
    TXREG = tx_byte;
	}
//-----------------------------------------------------------------------------
void putch_(unsigned char tx_byte)  // output one byte 	for INT 
//-----------------------------------------------------------------------------
	{
	while(!TRMT1);				// set when register is empty
	TXREG = tx_byte;
	}

//-----------------------------------------------------------------------------
void puts_(const char *s)
//-----------------------------------------------------------------------------
	{	
	while(s && *s)		
		putch_(*s++);	
	}

#define SW_TX      	LATAbits.LA1	//LATA1
#define TRISbit_SWTXD 	TRISA1
#define SW_RX         	PORTBbits.RB5           // Receive pin port and pin
#define TRISbit_SWRXD 	TRISB5

/* The following extern declared symbols needs to be defined by user prior 
 * to using UART functions.
 */

//extern  void DelayRXHalfBitUART(void);
//extern  void DelayRXBitUART(void);
//extern  void DelayTXBitUART(void);
//extern  char uartdata;
//extern  char BitCount;

/********************************************************************
;*      Function Name:  Open_SW_UART                                *
;*      Return Value:   void                                        *
;*      Parameters:     void                                        *
;*      Description:    This routine configures the I/O pins for    *
;*                      software UART.                              *
;*******************************************************************/

void Open_SW_UART(void)
{
SW_TX 			=1;
TRISbit_SWTXD 	=0;
//SW_RX         	=0;
//TRISbit_SWRXD 	=1;
}

//for 19200 at 4Mhz
//#define DelayTXBitUART()	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP()

//for 28800 at 4Mhz
#define DelayTXBitUART()	NOP();NOP();NOP();NOP();NOP();NOP();NOP()

/* **********************************************************************
*      Function Name:  WriteUART                                       *
*      Return Value:   void                                            *
*      Parameters:     data: byte to send out the UART                 *
*      Description:    This routine sends a byte out the TXD pin.      *
*      Special Note:   The user must provide a routine:                * 
*                      DelayTXBit():                                   *
*                        DelayTXBit should have:                       *
*                              8 Tcy for overhead                      *
*                              2 Tcy call to DelayTXBit                *
*                              ? Tcy                                   *
*                              2 Tcy for return from DelayTXBit        *
*                      = (((2*OSC_FREQ)/(4*BAUDRATE))+1)/2  Tcy        *
*********************************************************************** */
void putch__(unsigned char uartdata)
//void WriteUART (unsigned char uartdata)
	{
	unsigned char bitcount=8,data;
	SW_TX=0;
	DelayTXBitUART();
	DelayTXBitUART();	
	DelayTXBitUART();	
	while (bitcount--)
		{
		//SW_TX=((uartdata>>=1) & 0x01);
		data=(uartdata & 0x01);
		uartdata>>=1;
		if(!data)
			SW_TX=0;
		else
			SW_TX=1;
		DelayTXBitUART();
		DelayTXBitUART();	
		DelayTXBitUART();	
		}
	SW_TX=1;
	DelayTXBitUART();
	DelayTXBitUART();	
	DelayTXBitUART();	
	}


/**********************************************************************
*    Function Name:  putsUART                                         *
*    Return Value:   void                                             *
*    Parameters:     data: pointer to string of data                  *
*    Description:    This routine transmits a string of characters    *
*                    to the UART data byte including the null.        *
**********************************************************************/
void putsSW_UART(const char *data)
{
  do
  {    // Transmit a byte
    putch(*data);	//WriteUART(*data);
  } while( *data++ );
}
