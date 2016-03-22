#include "xc.h"


#include "main.h"



//ISR.c

void interrupt ISR( void )
{
  CLRWDT( );    //todo remove at end

  //---------------------------------------------------------------------
  // 	At each TMR1 interrupt, Count values are maintained to determine
  //  	when 10ms, 100msec, and 1000msec had elapsed.
  if ( TMR1IF && TMR1IE )
    {
      TMR1IF = 0; // Clear the interrupt flag.
      TMR1H = T1H_PRESET_VAL;
      TMR1L = T1L_PRESET_VAL;

      //CLRWDT( ); //$$ i dont loke it remove to main $$$ clear WDT every 10 Msec
      stat.msec5 = 1;
      Count_10m++;
      if ( Count_10m > 1 )
        {
          stat.msec10 = 1;
          Count_10m = 0;

         // led_handler_timer_10_mS( );

          Count_100m++;
          if ( Count_100m >= COUNT_100MSEC ) // Set flag and zero count if
            { // 1000msec had elapsed.
              //GREEN_LED = 1 - GREEN_LED;

              Count_100m = 0;
              stat.msec100 = 1; // 100msec had elapsed.
              msec100buzz=1;

              Count_500m++;
              if ( Count_500m >= COUNT_500MSEC ) // Set flag and zero count if
                { // 1000msec had elapsed.
                  stat.msec500 = 1;
                  Count_500m = 0;
                }

              Count_1000m++;
              if ( Count_1000m >= COUNT_1000MSEC ) // Set flag and zero count if
                { // 1000msec had elapsed.
                  stat.msec1000 = 1;
                  Count_1000m = 0;


                  Count_10Sec++;
                  if ( Count_10Sec >= COUNT_10SEC )
                    {
                      Count_10Sec = 0;
                      stat.sec10 = 1;
                    }

#ifdef	ICD_DEBUG		//for debug so sys will start runing after SW press
                  //              Count_1min = COUNT_1MIN; //the minits will behaive like seconds for debuging
#endif

                  if ( Count_1min >= COUNT_1MIN ) // Set flag and zero count if
                    { // 1minit had elapsed.
                      stat.min1 = 1;
                      Count_1min = 0;

                      Count_10min++;
                      if ( Count_10min >= COUNT_10MIN )
                        {
                          stat.min10 = 1;
                          Count_10min = 0;

                          Count_1hour++;
                          if ( Count_1hour >= COUNT_1HOUR )
                            {
                              stat.hour1 = 1;
                              Count_1hour = 0;
                            }
                        }
                    }
                }
            }
        }
    }


  if ( RC1IF && RC1IE ) //receive interrupt routine
    {
      if ( OERR1 || FERR1 ) //test for erors
        {
          dummy = RCREG; //if we have some resew all data
          SPEN1 = 0;
          CREN1 = 0;
          NOP( );
          NOP( );
          NOP( );
          SPEN1 = 1;
          CREN1 = 1;
          rx_pointer = 0; //reset rx buff
        }
      else
        {
          rx_timeout = RX_TIMEOUT; //set timeout timer

          rx_buufer[rx_pointer] = RCREG1; //read and save data

          if ( rx_buufer[rx_pointer] == 0xFF ) //if its the end of the string
            {
              number_of_recived_bytes = rx_pointer; //save the number of the recived bytes

              for ( rx_pointer = 0; rx_pointer <= number_of_recived_bytes; rx_pointer++ ) //copy the data to the data buff
                {
                  data_buff[rx_pointer] = rx_buufer[rx_pointer];
                  rx_buufer[rx_pointer] = 0; //and reset the rx buff and the pointer
                }
              rx_pointer = 0;
            }
          else
            rx_pointer++;


          if ( rx_pointer > RX_BUFF_SIZE - 1 )
            {
              rx_pointer = 0;
            }
        }
      RC1IF = 0;
    }

  //  if ( RC2IF && RC2IE ) //receive interrupt routine
  //    {
  //      if ( OERR2 || FERR2 ) //test for erors
  //        {
  //          dummy = RCREG2; //if we have some resew all data
  //          //TX2REG = RCREG2;
  //          SPEN2 = 0;
  //          CREN2 = 0;
  //          NOP( );
  //          NOP( );
  //          NOP( );
  //          SPEN2 = 1;
  //          CREN2 = 1;
  //          rx_pointer = 0; //reset rx buff
  //        }
  //      else
  //        //        {
  //        //          rx_timeout2 = RX_TIMEOUT; //set timeout timer
  //        //
  //        //          rx_buufer[rx_pointer] = RCREG2; //read and save data
  //        ////TX2REG = RCREG2;
  //        //          if ( rx_buufer[rx_pointer] == 0xFF ) //if its the end of the string
  //        //            {
  //        //              number_of_recived_bytes = rx_pointer; //save the number of the recived bytes
  //        //
  //        //              for ( rx_pointer = 0; rx_pointer <= number_of_recived_bytes; rx_pointer++ ) //copy the data to the data buff
  //        //                {
  //        //                  data_buff[rx_pointer] = rx_buufer[rx_pointer];
  //        //                  rx_buufer[rx_pointer] = 0; //and reset the rx buff and the pointer
  //        //                }
  //        //              rx_pointer = 0;
  //        //            }
  //        //          else
  //        //            rx_pointer++;
  //        //
  //        //
  //        //          if ( rx_pointer > RX_BUFF_SIZE - 1 )
  //        //            {
  //        //              rx_pointer = 0;
  //        //            }
  //        //        }
  //        {
  //          //TXREG=RCREG2;
  //          rx_timeout2 = RX_TIMEOUT;						//set timeout timer
  //          rx_buufer2[rx_pointer2] = RCREG2;				//read and save data
  //          TXREG2 = rx_buufer2[rx_pointer2];				//echo data back
  //
  //          if( rx_pointer2 == 0 )								//if its the firest recived word
  //            {
  //              if ( rx_buufer2[rx_pointer2] == ':' )  // || rx_buufer[rx_pointer] == 'R' || rx_buufer[rx_pointer] == 'O')	//test for start of data stream	and if start detectrd start reciving
  //                {
  //                  //TXREG2=rx_buufer2[rx_pointer2];				//echo data back
  //                  rx_pointer2++;
  //                  rx_buufer[2] = 10;						//set dumi as msg len for next recived word it will be updated to the right word automaticly
  //                }
  //            }
  //
  //          else if( rx_pointer2 == ( rx_buufer2[1] - 1 ) )//if we recived all the long massage
  //            {
  //              if( rx_buufer2[rx_pointer2] == ';' )			//if we recived a valid stop bit
  //                {
  //                  U2_rx_data_Ready = 1;					//indicates main a data is ready to be handeled in rx_buufer
  //                  //					rx_timeout=0;
  //                  //TXREG='A';
  //                  rx_pointer2 = 0;								//reset pointers
  //                  rx_timeout2 = 0;								//and time out					//CREN=0;									//stop reciving data to enable main to test the recived data
  //                }
  //            }
  //          else
  //            {
  //              if( rx_pointer2++ > RX_BUFF_SIZE - 1 )		//increment the buff pointer	if buffr over flow reset buffer pointer
  //                rx_pointer2 = 0;
  //            }
  //        }
  //      RC2IF = 0;
  //    }

  if ( INT1F )
    {
      INT1F = 0;
    }
}



