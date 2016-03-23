/***********************************************************************
 *		Project:		emrgency light									*
 *    	Author:			shlomo marmor                               	* 
 *    	Written For:			  	                					*
 *    	Filename:       main.c                                          *
 *    	Date:           29.01.2014                                  	*
 *    	File Version:   D.00.01.00                                  	*
 *                                                                  	*
 *    	Tools used:		MPLABX -> 1.95                         			*
 *                		Compiler  -> XC8 1.30							*
 *    	Written and tested on :								*
 ************************************************************************
 *		GENERAL DESCRIPTION:
 *  	
 *  	for emergency light circuit control for 2W led with elc network
 *		control.
 ***********************************************************************
 *	
 *
 *	Revision History:
 *	         Modification     Tracking
 *	Author       Date          Number     Description of Changes
 *	------   ------------    ----------   --------------------------
 * shlomo       05.02.14        1.02        all work with crane board, need to add
 *                                          the long test once a year & sleep mode
 *
 * shlomo       09.02.14        1.04        long test mode was added. still some issues need to be
 *                                          clearifie, what happens when the test is fail, what happen when
 *                                          with the leds & how we know the batterey was changed.
 *
 * shlomo       10.02.14        1.05        sleep mode was added,buzzer was added
 *
 * shlomo       08.05.14        1.06        the softwere on the real board
 *
 * shlomo       11.05.14        1.07        SW run on the board looks good.
 *
 * shlomo       28.05.14        1.08        SW run change method of ADC sampling due to unstable voltage
 *                                          on the transformator output.
 *
 * shlomo       29.05.14        1.09        change in the state machine so work mode will do on after more dibounce
 *
 * shlomo       06.07.14        1.11        sleep mode has been midify & the main led in going on in 1 sec current in sleep is 5-10uA
 *
 * mike         25.7.14         1.12        sw cleanup working times shortened
 *                                          sys going to sleep after wakeup
 *                                          pressing test the light turnes on immidiatly
 *
 * shlomo       07.09.14        1.13        short test is tested with dani and has been modified
 *
 * mike         08.09.14        1.14        the sleep mode works well FVR work in ADC sampeling in sleep mode
 *                                          all working good. bat sampeling with the en pin dont work
 *
 * shlomo       11.09.14        1.15        bug fixed when waiking up FVR is reused
 * 
 ***************************************************************************/
//#define PIC18F45K22
#define PIC18F23K22
//#define PIC18F26K22
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "CONFIG_BITS.h"
#include "main.h"
#include "a2d.h"
#include "globals.h"
#include "delay.h"
//#include "delay.c"
#include "C_Macro_definitiones.h"
#include "Pic_IO.h"
#include "eeprom.h"
#include "func.h"
#include "usart.h"



//#include "usart.c"
//#include "SW USART.c"
//#include "a2d.c"

//#include "func.c"

//#include "ISR.c"

//unsigned char eeprom_read( unsigned char address );
//void eeprom_write( unsigned char address, unsigned char value );


SW_STAT last_sw_stat, sw_stat;
unsigned int tempa, g;

void main( void )
{
  OSCCON = 0b01010000; //set clock to 4Mhz internal
  init_system( ); // Initialize System Function and Variables


  init_USART( );
  //enable_sending_data2 = 1;
  //eeprom_data_refresh( );
  //last_sw_stat = INIT;
  // sw_stat = INIT;
  //Open_SW_UART( );

  test_sw_count = 0;
  GRID_Voltage_ADC_val = 0;
  Vbat_ADC_val = 0;
  test_faild_flag = 0;
  DIMM_SHDN = 1; //to enabl the led

  //	backup_Zoom_data();

  printf( "\n\r~~Sys Started~~~" );         //clear the screen of the terminal
  printf( "\n\rRCON = %x " ,RCON );         //clear the screen of the terminal
  //RCON=
  eeprom_data_refresh( );

  sw_stat = SLEEP; //ST_BY;
  V_BAT_TEST_EN_TRIS = 1; //going to inptut mode  
  PEIE = 1;
  GIE = 1;

  //activate_buzzer( 3,1,2);
  blink_GREEN_LED_and_RED_LED_and_BUZZER( 3, 40 );
  //while (1)
  //  {
  //     V_BAT_TEST_EN_TRIS = 1; //going to inptut mode
  //     activate_buzzer( 1,5,5);
  //  }
  //
  //
  //V_BAT_TEST_EN_LAT = 0; //writing 0 to latch so gnd is connected
  start_sampling_flag = 1; //flag for evrey 5mS sample


//    while ( 1 ) //	Infinite loop
//    {
//    CLRWDT( );
//    if ( stat.msec500 )
//        {
//          stat.msec500 = 0;
//printf( "\n\rtick~~~" );         //clear the screen of the terminal
//          RED_LED=1-RED_LED;
//          GREEN_LED=1-GREEN_LED;
//        }
//      }

  while ( 1 ) //	Infinite loop
    {
    //CLRWDT( );
      if ( stat.msec5 )
        {
          stat.msec5 = 0;
         CLRWDT( );
          if ( start_sampling_flag )
            {
              start_sampling_flag = 0;
              GRID_Voltage_ADC_val = 0;
              Vbat_ADC_val = 0;
              //VBAT_TEST_EN_TRIS = 0;
              //VBAT_TEST_EN = 0;//setting the output as 0
              DelayMs(1);
              /* peak detector for the VBATT and the VIN */
              while ( second_sample_count < 40 )//sampeling 40 times evrey 1mS
                {
                  Vin_ADC_accum = readA2D( ADC_CHANNEL_VIN );
                  if ( Vin_ADC_accum > GRID_Voltage_ADC_val )
                    GRID_Voltage_ADC_val = Vin_ADC_accum;

                  Vbat_ADC_val_accum = readA2D( ADC_CHANNEL_VBAT );
                  if ( Vbat_ADC_val_accum > Vbat_ADC_val )
                    Vbat_ADC_val = Vbat_ADC_val_accum;
                  DelayMs( 1 );
                  second_sample_count++;
                }
              //printf( "\r\n Vin ADC = %d\r\n", GRID_Voltage_ADC_val );
              //printf( "\r\n Vbat ADC = %d\r\n", Vbat_ADC_val );
              //VBAT_TEST_EN_TRIS = 1;//setting the pin to input for current saving
              BAT_CHARGE_EN = previous_charge_stat;
              //BAT_CHARGE_EN = 1;
              TRICKLE_CHARGE_EN = previous_trickle_stat;
              second_sample_count = 0;
            }
        }

   


      if ( stat.msec10 )
        {
          stat.msec10 = 0;
          //CLRWDT( );
          if ( start_analog_sequence_flag )
            {
              if ( analog_delay_count < 6 )//for 60mS delay so the voltage is stabilized
                analog_delay_count++;
              else
                {
                  start_analog_sequence_flag = 0; //reset flag for analog samples
                  analog_delay_count = 0; //reset delay counter
                  start_sampling_flag = 1; //flag for evrey 5mS sample
                }
            }

          if( !TEST_SW )//press detected
            {
              // sw_direction = UP;
              test_sw_count++;
              if( test_sw_count >= short_press_time_min ) //long press is detected
                {
                  if(Switch_stat == SW_RELEASED)   //if it is the 1st time 
                    stat.msec500 = 1;               //gump fast to test to immidiatly light the led
                  
                  Switch_stat = SHORT_PRESS_DETECTED;
                  test_sw_count = short_press_time_min;
                }
            }
          else      //switch relesed
            {
              if( test_sw_count )
                {
                  test_sw_count--;
                  {
                    if( !test_sw_count )
                      {
                        Switch_stat = SW_RELEASED;
                      }
                  }
                }
            }
          //              if( test_sw_count == long_press_time ) //long press is detected
          //                {
          //                  Switch_stat = LONG_PRESS_DETECTED;
          //                  printf("\r\nTEST SW LONG_PRESS_DETECTED");
          //                }
          //            }
          //          else// botton not pressed or has been realesed
          //            {
          //              if ( sw_direction == UP )
          //                sw_direction = DOWN;
          //              else
          //                {
          //                  if ( test_sw_count > short_press_time_min && test_sw_count < short_press_time_max )
          //                    {
          //                      Switch_stat = SHORT_PRESS_DETECTED;
          //                      // printf( "\r\n TEST SW SHORT_PRESS_DETECTED \r\n " );
          //                    }
          //
          //                  test_sw_count = 0; //reset thr debouncre counter for next press
          //                }
          //            }
        }
      //~~~~~ every 100Ms ~~~~~~~~~~~~~~~~~
      if ( stat.msec100 )
        {
          stat.msec100 = 0;

          //CLRWDT( );
          //RX_timeout_handler( );
        }

      if ( stat.msec500 )
        {
          stat.msec500 = 0;
          //starting Vin voltage sampeling sequence
          previous_charge_stat = BAT_CHARGE_EN ; //saving charging stat
          previous_trickle_stat = TRICKLE_CHARGE_EN;
          BAT_CHARGE_EN = 0;
          TRICKLE_CHARGE_EN = 0;
          start_analog_sequence_flag = 1;
          
          //printf( "\r\n Tick 1sec\r\n" );
          vbat_check();
          Charging_handler();
          display_voltage();
          switch ( sw_stat )
            {
              case SLEEP:
                sleep_stat_hendler( );
                break;
              case ST_BY:
                stby_stat_hendler( );
                break;
              case WORK:
                work_stat_hendler( );
                break;
              case TEST:
                test_stat_hendler( );
                break;
              case ERROR:
                error_stat_hendler( );
                break;
              case LONG_TEST:
                long_test_stat_hendler( );
                break;
              case SHORT_TEST:
                short_test_stat_hendler( );
                break;
              default:
                break;
            }
          output_hendler( );
        }

      if ( stat.msec1000 )
        {
          stat.msec1000 = 0;
          //~~~~~~~~~~~~check bat voltage~~~~~~~~~~~~~~
          
        }


      if ( stat.sec10 )
        {
          stat.sec10 = 0;
          //printf( "\r\n Tick 10sec\r\n" );
          void eeprom_data_refresh( );
        }


      if ( stat.min1 )
        {
          stat.min1 = 0;
       

          // printf( "\r\n \r\n Tick 1min\r\n\r\n" );

        }
      if ( stat.min10 )
        {
          stat.min10 = 0;
        }

      if ( stat.hour1 )
        {
          stat.hour1 = 0;
          saved_hours_counter = load_int_from_eeprom( SAVED_TIME_FOR_TEST_EE_ADD ); //read eeprom time to saved_hours_counter;

          if ( !( saved_hours_counter % week_in_houres ) )//check if 1 week has passed
            {
              sw_stat = SHORT_TEST; //RUN TEST
            }
          if ( sw_stat != LONG_TEST )//if we are not in long test mode check if need to do a long_test
            {
              if ( saved_hours_counter >= year_time )
                {
                  sw_stat = LONG_TEST; //RUN TEST
                }
              else
                {
                  saved_hours_counter++;
                  save_int_to_eeprom( SAVED_TIME_FOR_TEST_EE_ADD, saved_hours_counter );
                }
            }
        }
    }
}


