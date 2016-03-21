#include <xc.h>
#include "delay.h"

#define	SOLAR_CELL		0
#define	BATT			11

//---------------------------------------------------------------------
// unsigned char readA2D()
//	reads the a2d 4 times , and returnes the avareging reading beetwen 0=>(0v) to 1024 =>(5v)
//---------------------------------------------------------------------

#define REPET_SAMPELING_COUNTER 4 							//beter be 2^n (2,4,8,16) to help the pic to get easy divide
//-----------------------------------------------------------------------------
unsigned int readA2D(unsigned char channel)
//-----------------------------------------------------------------------------
	{
	unsigned char i;
	unsigned int A2D_Result;

    ADCON0 = (channel << 2) + 0x01;					// enable ADC,
	ADCON1=0b00000000;
    ADCON1bits.PVCFG = 2;
    ADCON2=0b10101111;								//A2D right jestifide

	//BATT_TEST_EN_TRIS=0;
	//BATT_TEST_EN=0;
	DelayUs(10);
	//DelayMs(10);
	A2D_Result=0;
	for(i=0;i<REPET_SAMPELING_COUNTER;i++)	
		{
		DelayUs(10);
		GODONE=1;												//start A2D conversion
		while(GODONE)
			continue;										// Wait for conversion to complete	
		A2D_Result+=ADRESH<<8;								//add the result to the suming register
		A2D_Result+=ADRESL;
		}
	A2D_Result=A2D_Result/REPET_SAMPELING_COUNTER;

//	ADCON1=0x87;											//set all analog pines as digital
	
	//BATT_TEST_EN_TRIS=1;
	return A2D_Result;
	}

// sample once and return 8 MSB
unsigned char sample_A2D( unsigned char channel )
	{
	channel=channel<<2;			//	set channal word
	ADCON0 &= 0b11000011;                   // Reset channel select
	ADCON0 |= channel;			// Select new channel
	DelayUs(250);				// Short acquisition delay
	GO_DONE = 1;					// Start conversion
	while(GO_DONE);
	return(ADRESH);				//select the 8 MSB of result
	}


void Init_Adc( void )                      
	{
	//ADCON0 = 0b00000000;		//a2d off
	// = 0b00001110;		//all channales are digital ch1 analog
	 ADCON0 =  0x01;					// enable ADC,
	ADCON1=0b00000000;
    ADCON1bits.PVCFG = 2; //so internal Vref is connected to ADC
    //if the external Vref is connected
    //ADCON1bits.PVCFG = 1; //so internal Vref is connected to ADC

    ADCON2=0b10101111;								//A2D right jestifide
}


