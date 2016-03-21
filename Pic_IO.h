/* 
 * File:   Pic_IO.h
 * Author: shlomo marmor
 *
 * Created on 29.01.2014
 */

#ifndef PIC_IO_H
#define	PIC_IO_H

#ifdef	__cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------
//  I/O direction Definitions
//---------------------------------------------------------------------
 //---------------------------------------------------------------------
//  I/O init Definitions
//---------------------------------------------------------------------


#define	VBAT_PIC                PORTAbits.AN0		//An In
#define	VIN_TEST_PIC            PORTAbits.AN1		//An In
#define	TEST_VREF       	LATAbits.LATA2		//An Out
#define	VREF                    PORTAbits.AN3		//VERF
#define	BUZZER_A                LATAbits.LATA4		//Di In
#define	BUZZER_B                LATAbits.LATA5		//Di In
#define	GREEN_LED               LATAbits.LATA6		//Di Out
#define	RED_LED                 LATAbits.LATA7		//Di Out


#define	TRISAimage              0b00001011;
#define	LATAinit                0b00000000;
#define	ANSELAimage             0b00001011;
#define TRISA_MASK_AT_SLEEP     0b00000010;


#define	TEST_SW                  PORTBbits.RB0 		//Di In
#define	IO1                      PORTBbits.RB1 		//Di In
#define	IO2                      PORTBbits.RB2		//Di In
#define	MAIN_LED_PIC             LATBbits.LATB3		//Di In
#define	IO3                      PORTBbits.RB4		//Di In
#define	IO4                      PORTBbits.RB5		//Di In
#define	TX2                      LATBbits.LATB6		//COMM		batt green led
#define	RX2                      PORTBbits.RB7		//COMM		batt red led


#define	TRISBimage               0b10110111;
#define	LATBinit                 0b00000000;
#define	ANSELBimage              0b00000000;//ANALOG IS RA3,RA4
#define TRISB_MASK_AT_SLEEP      0b00000001;


#define	VBAT_TEST_EN_TRIS         TRISCbits.TRISC0 		//Di In
#define	VBAT_TEST_EN              LATCbits.LATC0 		//Di In
#define	DIMM_SHDN                 LATCbits.LATC1		//Di In
#define	BAT_CHARGE_EN             LATCbits.LATC2		//Di In
#define	NC_1                      LATCbits.LATC3 		//DI Out		SPI serial clock
#define	TRICKLE_CHARGE_EN         LATCbits.LATC4 		//DI In			SPI serial data
#define	NC_2                      LATCbits.LATC5		//Di Out		SPI serial sata
#define	TX_PIC                    LATCbits.LATC6 		//DI Out		serial debug output
#define	RX_PIC                    PORTCbits.RC7 		//DI In			serial debug inout

#define	TRISCimage              0b10000001;
#define	LATCinit                0b00000010;
#define	ANSELCimage             0b00000000;
#define TRISC_MASK_AT_SLEEP     0b00000000;
//
//#define	CS_AXEL                     LATDbits.LATD0 		//Di Out
//#define	RS_422_RTS                  LATDbits.LATD1		//Di Out
//#define	nSEL                        LATDbits.LATD2		//Di Out
//#define	BAT_CHARGE_EN               LATDbits.LATD3          //DI Out
//#define	MAIN_LED_PIC                LATDbits.LATD4 		//DI Out
//#define	CHARGER_SHDN                LATDbits.LATD5		//Di Out
//#define	TX_2                        LATDbits.LATD6 		//DI Out		serial debug output
//#define	RX_2                        PORTCbits.RD7 		//DI In			serial debug inout
//
//#define	TRISDimage              0b10000000;
//#define	LATDinit                0b00000000;
//#define	ANSELDimage             0b00000000;
//#define TRISD_MASK_AT_SLEEP     0b00000000;
//
//
//#define	FREQ1_pic	LATEbits.LATE0 		//Di Out		auxilary power
//#define	FREQ2_pic	LATEbits.LATE1		//Di Out
//#define	FREQ3_pic	LATEbits.LATE2		//Di Out		jumper test power
//
//#define	TRISEimage              0b00000000;
//#define	LATEinit                0b00000000;
//#define	ANSELEimage      	0b00000000;
//#define TRISE_MASK_AT_SLEEP     0b00000000;

#ifdef	__cplusplus
}
#endif

#endif	/* PIC_IO_H */

