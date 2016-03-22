#include "xc.h"

#define					RX_BUFF_SIZE	20



unsigned char 	rx_buufer[RX_BUFF_SIZE];
unsigned char 	data_buff[RX_BUFF_SIZE];
unsigned char 	rx_pointer = 0;
//unsigned char		rx_data_Ready = 0;			//indicates a data is ready to be handeled in rx_buufer 
unsigned char		dummy;							//a temp reg for bad data
unsigned char		number_of_recived_bytes;

#define					RX_TIMEOUT		20				//200ms recive timeout in 10 ms units
unsigned char 	rx_timeout;
//unsigned char 	check_sum;						//check_sum register
unsigned char 	enable_sending_data;

#define	NO_TERMINAL_DATA	0
#define SEND_TERMINAL_DATA	1

bit send_data_flag; 									//bit indicting pc asked the pic to send data

//		on receve the format is C 5 R N	;
//					EXAMPLE:write 5 to eeprom(fill 7 tokens)	C 5 R 7	;	
//						
//						C=start bit
//						5=masage len 5 numbers total
//						R=command   R=> reset the countes on the cart to the next num
//						N= hex number to reset the cart to
//						;=stop bit
/********************************************************************

some printf samples :

	printf( "\r\n  A2D = %d.%d volts",volts,decivolts );   
	putch(0xF5);
	putch(new_msg_to_send);
				
********************************************************************/
// for hitachi camera
///* Serial initialization */
//void init_USART(void)
//	{
//	TXSTA = 0b00000000;					
//	RCSTA = 0b00000000;		
//	NOP();
//	NOP();
//	TRISC7=1;
//	TRISC6=1;
//	SPBRG =25;				//9600  4Mhz
//	//SPBRG =51;					//4800 4Mhz
//	
//	TXSTA = 0b01100100;		//enable 9 bit tx			
//	RCSTA = 0b11010000;		//enable 9 bit rx
//	RCIF=0;
//	RCIE=1;
//	}	


/* Serial initialization */
void init_USART(void)
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

void putch(unsigned char tx_byte)  // output one byte for printf() func
	{
//	unsigned char num,j,cnt;
//	
//	num=tx_byte;
//	cnt=0;
//	//calculate parity
//	for (j=0;j<8;j++)
//		{
//		if(num&0x01)
//			cnt++;
//		num>>=1;
//		}	
//	cnt&=0x01;
//	if(cnt)
//		TX9D=1;
//	else
//		TX9D=0;
//
//
//#ifndef CRANE_VISION_CONTROLLER    //disable the printf in crane vision since it is stoping the camera from working
	while(!TRMT1);				// set when register is empty
//DelayMs(5);
    TXREG = tx_byte;
//	NOP();
//        NOP();
//        while(!TRMT2);				// set when register is empty
////        DelayMs(5);
//	TX2REG = tx_byte;
////#endif
//	NOP();
//        NOP();
	}

void putch_(unsigned char tx_byte)  // output one byte 	for INT 
	{
	while(!TRMT1);				// set when register is empty
	TXREG = tx_byte;
	}

////send  "size" itemss from tx_data buffer
//unsigned char send_data(unsigned char *tx_data , unsigned char size ) 
//	{
//	while( size-- )
//		{
//		putch(*tx_data);		//send one byte
//		tx_data++;				//increment pointer
//		}
//	return 1;
//	}
//	
//
// Output a string via the serial port same as puts but for int 
void puts_(const char *s)
	{	
	while(s && *s)		
		putch_(*s++);	
	}
//



//////////////////////////////////////////////////////////////////////////

//unsigned char uartdata;
//unsigned char BitCount;
//unsigned char SW_TX_Buff[50];


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


///********************************************************************
//*    Function Name:  getsUART                                       *
//*    Return Value:   void                                           *
//*    Parameters:     buffer: pointer to string                      *
//*                    len: length of characters to receive           *
//*    Description:    This routine receives a string of characters   *
//*                    from the SW UART of length specified by len.   *
//********************************************************************/
//void getsUART(char *buffer, unsigned char len)
//{
//  char i;    // Length counter
//
//  for(i=0;i<len;i++)   // Only retrieve len characters
//  {
//    *buffer = getcUART(); // Get a character from the USART
//                       // and save in the string
//    buffer++;          // Increment the string pointer
//  }
//}
//
///* **********************************************************************
//*      Function Name:  ReadUART                                        *
//*      Return Value:   char: received data                             *
//*      Parameters:     void                                            *
//*      Description:    This routine waits for a start bit, and then    *
//*                      reads all 8-bits.                               *
//*      Special Note:   The user must provide two routines:             *
//*                      DelayRXHalfBit():                               *
//*                        DelayRXHalfBit should have:                   *
//*                              5 Tcy for overhead                      *
//*                              2 Tcy call to DelayRXHalfBit            *
//*                              ? Tcy                                   *
//*                              2 Tcy for return from DelayRXHalfBit    *
//*                              = (((2*OSC_FREQ)/(8*BAUDRATE))+1)/2 Tcy *
//*                      DelayRXBit():                                   *
//*                        DelayRXBit should have:                       *
//*                              10 Tcy for overhead                     *
//*                              2 Tcy call to DelayRXBit                *
//*                              ? Tcy                                   *
//*                              2 Tcy for return from DelayRXBit        *
//*                              = (((2*OSC_FREQ)/(4*BAUDRATE))+1)/2 Tcy *
//*********************************************************************** */
//
//unsigned char ReadUART(void)
//{
//unsigned char bitcount=8;
//unsigned char uartdata;
//
//while (LATBbits.PORTBbits.RB5){
//	while (PORTBbits.PORTBbits.RB5);
//	DelayRXHalfBitUART();
//}
//
//while (bitcount--){
//	DelayRXBitUART();
//	STATUSbits.C = PORTBbits.PORTBbits.RB5 ? 1:0;
//	uartdata=uartdata>>1 | uartdata<<7;
//}
//return uartdata;
//}

/*=========================Function header =================================
 *
 * FUNCTION 		RX_timeout_handler( )
 *
 * DESCRIPTION      reset the RX buff after a perion of no activity
 *
 *===========================================================================*/
//void RX_timeout_handler( )
//{
//  if ( rx_timeout )
//    {
//      rx_timeout--;
//      if ( !rx_timeout )
//        {
//          RCIE = 0; //temporary stop recive int
//          for ( rx_pointer = 0; rx_pointer < RX_BUFF_SIZE; rx_pointer++ ) //reset the rx buffer and the pointer
//            {
//              rx_buufer[rx_pointer] = 0; //and reset the buff
//            }
//          rx_pointer = 0;
//          RCIE = 1;
//        }
//    }
//}
