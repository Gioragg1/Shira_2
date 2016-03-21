//func.c

#include "delay.h"


//#include "delay.h"
//#include "Pic_IO.h"
//#include "eeprom.h"
//#include "main.h"
//#include "main.h"


//#include "main.h"


//#include "globals.h"



//---------------------------------------------------------------------
// void Setup(void)
//
// Initializes program variables and peripheral registers.
//---------------------------------------------------------------------

void init_system( )
{
  LATA = LATAinit;
  LATB = LATBinit;
  LATC = LATCinit;
  //  LATD = LATDinit;
  //  LATE = LATEinit;
  TRISA = TRISAimage;
  TRISB = TRISBimage;
  TRISC = TRISCimage;
  //  TRISD = TRISEimage;
  //  TRISE = TRISEimage;

  ANSELA = ANSELAimage;
  ANSELB = ANSELBimage;
  ANSELC = ANSELCimage;

  WPUB = 0b00000001; //enable the wee? pullups
  nRBPU = 0; // PORTB pull-ups are enabled provided that the pin is an input and the corresponding WPUB bit is set


  //T1CON = 0b00010001; //for 10ms basic tick
  T1CON = 0b00000001; //for 5 ms basic tick
  TMR1IF = 0;
  TMR1IE = 1;

  //FVRCON = 0b11010000;//this is for fix voltage refernce. if the external fix voltage is connected this is not needed

  FVRCONbits.FVRS = 2; //so FVR is 2.048V
  FVRCONbits.FVREN = 1; //enable the FVR

  Count_10m = 0;
  stat.msec5 = 0;
  stat.msec100 = 0;
  stat.msec1000 = 0;

  Count_100m = 0; // Clear the timing counters.
  Count_1000m = 0;
  //SET_BATT_LED_OFF;
  test_led_time_count = 0;
  exit_sleep_press = 4;
}


//void my_eeprom_write(unsigned char addr, unsigned char value)
//{
//	EEPROM_WRITE(addr, value);
//}

unsigned char ATOI( unsigned char data )
{
  if ( data <= '9' )
    return data - '0';
  else
    return data - '7';
}

void blink_RED_LED( unsigned char blinks, unsigned int timeMs )
{
  unsigned char i;
  for ( i = 0; i < blinks; i++ )
    {
      RED_LED = 1;
      DelayMs( timeMs );
      RED_LED = 0;
      DelayMs( timeMs );
    }
  NOP( );
  NOP( );
}

void blink_GREEN_LED( unsigned char blinks, unsigned int timeMs )
{
  unsigned char i;
  for ( i = 0; i < blinks; i++ )
    {
      GREEN_LED = 1;
      DelayMs( timeMs );
      GREEN_LED = 0;
      DelayMs( timeMs );
    }
  NOP( );
  NOP( );
}

void blink_GREEN_LED_and_RED_LED( unsigned char blinks, unsigned int timeMs )
{
  unsigned char i;
  for ( i = 0; i < blinks; i++ )
    {
      RED_LED = 1;
      GREEN_LED = 1;
      DelayMs( timeMs );
      GREEN_LED = 0;
      RED_LED = 0;
      DelayMs( timeMs );
    }
  NOP( );
  NOP( );
}

void activate_buzzer( unsigned char beeps, unsigned char On_duration, unsigned char Off_duration )
{
  unsigned char i;
  while ( beeps )
    //for ( i = 0; i <= beeps; i++ )
    {
      buzz_on( On_duration );
      buzz_off( Off_duration );
      beeps--;
    }
  NOP( );
  NOP( );
}

void buzz_off( unsigned char duration )
{
  BUZZER_A = 0;
  BUZZER_B = 0;
  while ( duration )
    {
      CLRWDT( );
      if( msec100buzz )
        {
          msec100buzz = 0;
          duration--;
          //BUZZER_A = 0;
          //BUZZER_B = 0;
        }
    }
}

void buzz_on( unsigned char duration )
{
  unsigned char temp_GIE;
  unsigned int calc_duration;
  calc_duration = duration * BUZZER_DURATION_COAF;
  temp_GIE = GIE;
  GIE = 0;
  while ( calc_duration )
    {
      calc_duration--;
      CLRWDT( );
      BUZZER_A = 1;
      BUZZER_B = 0;
      DelayUs( 70 );
      BUZZER_A = 0;
      BUZZER_B = 1;
      DelayUs( 70 );
    }

  GIE = temp_GIE;

}

void blink_GREEN_LED_and_RED_LED_and_BUZZER( unsigned char blinks, unsigned int timeMs )
{
  unsigned char i;
  for ( i = 0; i < blinks; i++ )
    {
      RED_LED = 1;
      GREEN_LED = 1;
      activate_buzzer( 1, 1, 2 );
      DelayMs( timeMs );
      GREEN_LED = 0;
      RED_LED = 0;
      DelayMs( timeMs );
    }
}

 
void vbat_check( )
{
  printf( "\n\r\n\r Vbat ADC = %d", Vbat_ADC_val );
    
//  if  ( Vbat_ADC_val < total_led_off_bat_voltage ) //todo !!!!!//less then 1 volt for cell
//      {
//      printf( "\n\rbatt_stat = BATT_EMPTY" ); 
//      
//      //main_led_stat = MAIN_LED_OFF; //MAIN LED IS OFF
//      printf( "\n\r LOW BAT LEN IS OFF GOING TO ERROR MODE\n\r" );//todo
//      sw_stat = ERROR;//todo to go to error or stby?
//      }

    if ( Vbat_ADC_val >= total_bat_is_disconnected_voltage )
    {
      printf( "\n\rbatt_stat = DISCONNECTED" );
      batt_stat = DISSCONECTED;
      sw_stat = ERROR;
    }else /*~~~~  BAT IS FULL do TRICKLE CHARGING ~~~~*/
    if( Vbat_ADC_val >= total_batt_full_voltage  )//BAT IS FULL
    {
      batt_stat = FULL_CHARGED;
      printf( "\n\rbatt_stat = FULL_CHARGED" );
    }else   /*~~~~  BAT IS IN NORMAL RANGE AND NEEDS CHARGING ~~~~*/
    if( Vbat_ADC_val >= total_shorted_bat_voltage ) 
    {
      batt_stat = NEED_CHARGING;
      printf( "\n\rbatt_stat = NEED_CHARGING" );
    }else /*~~~~  BAT IS SHORTED ~~~~*/
    {
      printf( "\n\rATT!!! batt_stat = SHORTED !!!" );
      batt_stat = SHORTED;
      sw_stat = ERROR;
    }
}


void Charging_handler()
{
  printf( "\n\r CHARGING IS ");
//if( sw_stat == ST_BY )
    switch ( batt_stat )
    {
    case SHORTED:
      printf( "- NORMAL & TRICKLE\n\r " );
      normal_charging = 1;
      trickl_charging = 1;
      break;

    case DISSCONECTED:
      printf( "OFF \n\r " );
      normal_charging = 0;
      trickl_charging = 0;
      break;
      
    case  NEED_CHARGING:
      printf( "- NORMAL & TRICKLE\n\r " );
      normal_charging = 1;
      trickl_charging = 1;
      green_led = 1;
      break;

    case  FULL_CHARGED:
      normal_charging = 0;
      trickl_charging = 1;
      printf( "- TRICKLE\n\r " );
//    red_led = 0; //todo what to do with the led
      green_led = 1;
      break;
    }
}

unsigned int load_int_from_eeprom( unsigned char add )
{
  unsigned int a;
  a = eeprom_read( add );
  a <<= 8;
  a += eeprom_read( add + 1 );
  return a;
}

void save_int_to_eeprom( unsigned char add, unsigned int data )
{
  eeprom_write( add, Hi( data ) );                             //and save msb
  DelayMs( 10 );
  eeprom_write( add + 1, Lo( data ) );                      //and lsb
  DelayMs( 10 );
}

//
//        MAX_BAT_VOLTAGE_FOR_LED_OFF_EE_ADD,      //3V - AVOVE 3v THE LED CAN BE ON
//        MAX_BAT_VOLTAGE_FOR_LED_OFF_LSB_EE_ADD,
//        MIN_BAT_VOLTAGE_FOR_LED_ON_EE_ADD,      //2.8V BELOW 2.8v THE LED MUST BE OFF
//        MIN_BAT_VOLTAGE_FOR_LED_ON_LSB_EE_ADD,
//        ,
//        BAT_FULL_VOLTAGE_LSB_EE_ADD,
//        BAT_IS_DISCONNECTED_VOLTAGE_EE_ADD,
//        BAT_IS_DISCONNECTED_VOLTAGE_LSB_EE_ADD,


void eeprom_data_refresh( )
{
  long_press_time = load_int_from_eeprom( LONG_PRESS_TIME_EE_ADD );
  short_press_time_min = load_int_from_eeprom( SHORT_PRESS_TIME_MIN_EE_ADD );
  short_press_time_max = load_int_from_eeprom( SHORT_PRESS_TIME_MAX_EE_ADD );
  year_time = load_int_from_eeprom( YEAR_TIME_EE_ADD );

  grid_power_low_voltage = load_int_from_eeprom( GRID_POWER_LOW_VOLTAGE_EE_ADD );       //160V
  grid_power_high_voltage = load_int_from_eeprom( GRID_POWER_HIGH_VOLTAGE_EE_ADD );     //180V

  led_off_bat_voltage    = load_int_from_eeprom( MIN_BAT_VOLTAGE_FOR_LED_ON_EE_ADD );   //2.8V 0.93*3
  led_on_bat_voltage    = load_int_from_eeprom( MAX_BAT_VOLTAGE_FOR_LED_OFF_EE_ADD );   //3V 1V*3

  //min_bat_voltage_normal_cahrging = load_int_from_eeprom( MAX_BAT_VOLTAGE_FOR_LED_OFF_EE_ADD ); //3V 1V*3
  
  batt_full_voltage = load_int_from_eeprom( BAT_FULL_VOLTAGE_EE_ADD );
  bat_is_disconnected_voltage = load_int_from_eeprom( BAT_IS_DISCONNECTED_VOLTAGE_EE_ADD );

  num_of_bat_cells = load_int_from_eeprom( NUM_OF_BATT_CELLS_EE_ADD );

  short_time_test_pass = load_int_from_eeprom( SHORT_TEST_TIME_EE_ADD );
  long_time_test_pass = load_int_from_eeprom( LONG_TEST_TIME_EE_ADD );
  week_in_houres = load_int_from_eeprom( WEEK_TIME_EE_ADD );
  //claculating values for num of cells in system & divide in 2 because the voltage divider is 1:2

  shorted_bat_voltage = 15;
  total_shorted_bat_voltage = ( num_of_bat_cells * shorted_bat_voltage );

  //total_min_bat_voltage_normal_cahrging = ( num_of_bat_cells * min_bat_voltage_normal_cahrging );
  total_batt_full_voltage = ( num_of_bat_cells * batt_full_voltage );
  total_bat_is_disconnected_voltage = ( num_of_bat_cells * bat_is_disconnected_voltage ) ;
  total_led_off_bat_voltage = ( num_of_bat_cells * led_off_bat_voltage ) ;
  total_led_on_bat_voltage = ( num_of_bat_cells * led_on_bat_voltage ) ;
}

void eeprom_time_delay( unsigned int houres )
{
  unsigned int temp;
  temp  = load_int_from_eeprom( SAVED_TIME_FOR_TEST_EE_ADD );
  temp = temp - houres;
  save_int_to_eeprom( SAVED_TIME_FOR_TEST_EE_ADD, temp ); //write new time to eeprom
}



void stby_stat_hendler( )
{
  red_led = 0;
  green_led = 1; //todo what to do with the leds
  if ( last_sw_stat != ST_BY )
    {
    printf( "\n\r MOVING TO ST_BY MODE" );
    last_sw_stat = ST_BY;
    //output_hendler( );
//    DelayMs(250);DelayMs(250);DelayMs(250);DelayMs(250);
//    DelayMs(250);DelayMs(250);DelayMs(250);DelayMs(250);
//    DelayMs(250);DelayMs(250);DelayMs(250);DelayMs(250);
    }
  else
    {
      //~~~~~~~~~~~~~~check power grid~~~~~~~~~~~~~
      printf( "\n\r\n\r sw_stat = ST_BY;" );
      if ( GRID_Voltage_ADC_val < grid_power_low_voltage ) //grid power is to low
        {
          printf( "\n\r GRID_POWER FAIL DETECTED \n\r", power_fail_count );
          //charging_stat = OFF; //CHARGING IS OFF
          //power_stat = POWER_FAIL;
          sw_stat = WORK;    //MOVING TO WORK STAT
          stat.msec500 = 1;
        }
      else //grid power GOOD
        {
          main_led_stat = MAIN_LED_OFF; //MAIN LED IS OFF
          //power_stat = POWER_OK;
          printf( "\n\rGRID_POWER IS OK" );
        }
      
      if ( Switch_stat == SHORT_PRESS_DETECTED )
        {
          sw_stat = TEST;
        }
    }
}


void display_voltage()
{
  printf( "\n\r GRID_Voltage_ADC_val =%d ,grid_power_low_voltage = %d ",GRID_Voltage_ADC_val , grid_power_low_voltage );
}


void work_stat_hendler( )
{
    red_led = 1;
    green_led = 0;
    main_led_stat = MAIN_LED_ON;
    normal_charging = 0;
    trickl_charging = 0;

    if ( last_sw_stat != WORK )
    {
      printf( "\n\rTO WORK MODE" );
      last_sw_stat = WORK;
      //output_hendler( );
//      DelayMs(250);DelayMs(250);DelayMs(250);DelayMs(250);
//      DelayMs(250);DelayMs(250);DelayMs(250);DelayMs(250);
//      DelayMs(250);DelayMs(250);DelayMs(250);DelayMs(250);
    }
  else
    {
      //~~~~~~~~~~~~~~~~~~~~~~test power grid~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      printf( "\n\r\n\r sw_stat = WORK;" );
//      if ( GRID_Voltage_ADC_val < grid_power_low_voltage ) //grid power is to low
//        {
//          printf( "\n\rPOWER FAIL DETECTED" );
//          //power_stat = POWER_FAIL;
//          sw_stat = WORK;    //MOVING TO WORK STAT
//        }
      if(GRID_Voltage_ADC_val>60)       //compensate in for the voltage drop when charging
        GRID_Voltage_ADC_val-=60;
      if ( GRID_Voltage_ADC_val >= (grid_power_high_voltage+25) ) //grid power is back
        {
          main_led_stat = MAIN_LED_OFF; //MAIN LED IS OFF
          printf( "\n\rPOWER IS BACK" );
          sw_stat = ST_BY;    //MOVING TO WORK STAT
          stat.msec500 = 1;
        }

      if(Vbat_ADC_val <total_led_off_bat_voltage)
        {
         sw_stat = SLEEP;    //MOVING TO WORK STAT
         printf( "\n\rBATT US VERY LOW SO GOING TO SLEEP" );
         stat.msec500 = 1;
        }
     
  //if( Vbat_ADC_val < total_led_on_bat_voltage)
  //    {
  //      
  //    }
    }
}

void test_stat_hendler( )
{
  main_led_stat = MAIN_LED_ON; //main led is on for test
//  normal_charging = 0;
//  trickl_charging = 0;
  if ( last_sw_stat != TEST )
    {
      printf( "\n\rMOVING TO TEST MODE" );
      last_sw_stat = TEST;
      red_led = 1 ;
      green_led = 0;
    }
  else
    {
    red_led = 1 - RED_LED;
    green_led = 1 - red_led;
    
     printf( "\n\r\n\r sw_stat = TEST;" );

      if ( Switch_stat == SW_RELEASED )//if we still detect sw pressed recount
        { 
          sw_stat = ST_BY; //going back to stand by mode
          printf( "\n\rTEST IS DONE" );
          printf( "\n\rgoing to ST_BY" );
        }
    }
}

//in error mode we are trying to charge tje batt 
//and if the batt is full exit eror 

void error_stat_hendler( )
{
  red_led = 0;
  green_led = 0;
  main_led_stat = 0;

  if ( last_sw_stat != ERROR )
    {
      printf( "\n\rTO ERROR MODE" );
      last_sw_stat = ERROR;
    }
  else
    {
      printf( "\n\r\n\r sw_stat = ERROR;" );

      saved_error_buzzer_count = load_int_from_eeprom( ERROR_BUZER_COUNT_EE_ADD );
      if ( error_buzzer_count >= saved_error_buzzer_count )
        {
          error_buzzer_count = 0;
          activate_buzzer( 1, 1, 2 );
        }
      else
        error_buzzer_count++;

      blink_RED_LED( 2, 50 );

      if( Vbat_ADC_val >= total_shorted_bat_voltage )//BAT VOLTAGE ID CRITICAL
        {
          sw_stat = ST_BY;
          printf( "\n\r BAT VOLTAGE IS GOOD! " );
          printf( "\n\r EXITING ERROR MODE" );
        }
    }
}

void sleep_stat_hendler( )
{
//  if ( last_sw_stat != SLEEP )
//    {
//      printf( "\n\rTO SLEEP MODE" );
//      last_sw_stat = SLEEP;
//      normal_charging = 0;
//      trickl_charging = 0;
//      red_led = 0;
//      green_led = 0;
//      main_led_stat = 0;
//    }
//  else
//    {
    printf( "\n\r\n\r\n\r sw_stat = SLEEP" );
      //      TRISD = TRISD_MASK_AT_SLEEP;
      //      TRISE = TRISE_MASK_AT_SLEEP;
      FVRCONbits.FVREN = 0; //disnable the FVR

      WDT_cnter = 156; //set counter for WDT evrey 32msec so 156*32=5 sec
      blink_GREEN_LED_and_RED_LED_and_BUZZER( 6, 40 );
      //wait here making shure the user relese the SW
      while  ( !TEST_SW )//continue incation system go to sleep after the user relese the sw
        //  blink_GREEN_LED_and_RED_LED_and_BUZZER( 1, 40 );
        DelayMs( 255 );
      DelayMs( 255 ); //to avoid noise on the SW
      while  ( !TEST_SW )//continue incation system go to sleep after the user relese the sw
        blink_GREEN_LED_and_RED_LED_and_BUZZER( 1, 40 );
      //~~~~~~~time base for 50Ms~~~~~~~~~~~~~~~~
      //do WDT*3 so the timing will be 17Ms*3=49Ms ~50Ms
      //                      pulse_send_interval=my_eeprom_read(PULSE_SENDING_INTERVAL_EE_ADD);//##$$
      //                      WDT_cnter = WDT_MULTIPLYER_FOR_50Ms;

      TRISA = TRISA_MASK_AT_SLEEP;
      TRISB = TRISB_MASK_AT_SLEEP;
      TRISC = TRISC_MASK_AT_SLEEP;
      LATA = 0x00;
      LATB = 0x00;
      LATC = 0x00;

      exit_sleep_count = 0;
      while ( sw_stat == SLEEP )                   //enter to sleep as mutch as needed//##$$
        {
          /////////////////////////////
#ifdef  debeg
          T0CON = 0b11000101;    //1:64 PRESCALER GIVING 16.3ms full range inTMR0
          TMR0 = 0;
          TMR0IE = 0;
          TMR0IF = 0;
          while ( !TMR0IF );
#else
          SLEEP( );
#endif

          //GREEN_LED = 1 - GREEN_LED;

          if ( GREEN_LED )
            GREEN_LED = 0;
          WDT_cnter--;
          if ( !WDT_cnter )
            {
              FVRCONbits.FVRS = 2; //so FVR is 2.048V
              FVRCONbits.FVREN = 1; //enable the FVR
              GREEN_LED = 1;
              WDT_cnter = 156;
                            
              GRID_Voltage_ADC_val = readA2D( ADC_CHANNEL_VIN );
              if ( GRID_Voltage_ADC_val > grid_power_low_voltage ) //grid power is to low
                {
                  DelayMs(30);
                  GRID_Voltage_ADC_val = readA2D( ADC_CHANNEL_VIN );
              
                  if ( GRID_Voltage_ADC_val > grid_power_low_voltage ) //grid power is to low
                    {
                      DelayMs(30);
                      GRID_Voltage_ADC_val = readA2D( ADC_CHANNEL_VIN );

                      if ( GRID_Voltage_ADC_val > grid_power_low_voltage ) //grid power is to low
                        {
                        init_system( ); // Initialize System Function and Variables
                        init_USART( );
                        printf( "\n\r Vin ADC = %d", GRID_Voltage_ADC_val );
                        printf( " GRID POWER DETECTED so WAKING UP" );
                        sw_stat = ST_BY;
                        }
                    }
                }
              if (sw_stat == SLEEP)
              FVRCONbits.FVREN = 0; //disnable the FVR
            }
          
          
          if( !TEST_SW )//press detected
            {
              exit_sleep_count++;
              if ( exit_sleep_count >= exit_sleep_press )
                {
                 
                  init_system( ); // Initialize System Function and Variables
                  init_USART( );
                   printf( "\n\rTEST_SW press detected so" );
                   printf( "WAKING UP" );
                  sw_stat = ST_BY;
                }
            }
          else
            exit_sleep_count = 0;
        }
//    }
}

void long_test_stat_hendler( void )
{
  if ( last_sw_stat != LONG_TEST )
    {
      printf( "\n\rENTERING LONG TEST MODE\n\r " );
      if ( last_sw_stat == WORK )//if the test began when the sys is in work mode so the test is delayed in 24 hours
        {
          printf( "\n\rTEST DELAYED" );
          eeprom_time_delay( 24 ); //the test will try again in 24 houres
          sw_stat = WORK;
        }
      else
        {
          //printf( "\n\r STARTING LONG TEST! \n\r " );
          last_sw_stat = LONG_TEST;
          //printf( "\n\r TEST_COUNT START \n\r " );
          test_time_count = 0;
        }
    }
  else//long test mode is on
    {
      if ( GRID_Voltage_ADC_val < grid_power_low_voltage ) //grid power is to low
        {
          eeprom_time_delay( 24 ); //the test will try again in 24 houres
          charging_stat = OFF; //CHARGING IS OFF
          power_stat = POWER_FAIL;
          //printf( "\n\rPOWER FAIL DETECTED");
          sw_stat = WORK;    //MOVING TO WORK STAT
        }
      else//so we can do the test
        {
          if ( Vbat_ADC_val > min_bat_voltage_for_test )//bat voltage is steel high enghouf
            {
              sw_stat = LONG_TEST;
              test_time_count++;
              printf( "\n\rTEST IS %d seconds\n\r ", test_time_count );
            }
          else //bat voltage is low
            if ( test_time_count >= long_time_test_pass )//test has sucseed going back to normal mode
            {
              printf( "\n\rSYSTEM TEST HAS BEEN SUCCSEFULL\n\r " );
              //printf( "\n\r GOING BACK TO ST_BY MODE \n\r " );
              sw_stat = ST_BY;
              test_time_count = 0;
              saved_hours_counter = 0;
              save_int_to_eeprom( SAVED_TIME_FOR_TEST_EE_ADD, saved_hours_counter ); //restart 1 year counting
            }
          else//test is fail go to error mode
            {
              printf( "\n\rSYSTEM TEST HAS BEEN FAILD\n\r " );
              //printf( "\n\r GOING TO ERROR MODE \n\r " );
              test_time_count = 0;
              test_faild_flag = 1;
              sw_stat = ERROR;    //MOVING TO WORK STAT
            }
        }
      if ( sw_stat == LONG_TEST )//so setting outputs
        {
          normal_charging = 0;
          trickl_charging = 0;
          red_led = 0;
          green_led = 0;
          main_led_stat = 1;
        }
    }
}

void short_test_stat_hendler( )
{
  if ( last_sw_stat != SHORT_TEST )
    {
      printf( "\n\rENTERING SHORT TEST MODE\n\r " );
      if ( last_sw_stat == WORK )//if the test began when the sys is in work mode so the test is delayed in 24 hours
        {
          printf( "\n\rWE ARE IN WORK MODE" );
          eeprom_time_delay( 24 ); //the test will try again in 24 houres
          sw_stat = WORK;
        }
      else
        {
          last_sw_stat = SHORT_TEST;
          test_time_count = 0;
        }
    }
  else//short test mode is on
    {
      if ( GRID_Voltage_ADC_val < grid_power_low_voltage ) //grid power is to low
        {
          eeprom_time_delay( 24 ); //the test will try again in 24 houres
          charging_stat = OFF; //CHARGING IS OFF
          power_stat = POWER_FAIL;
          //printf("\n\rPOWER FAIL DETECTED");
          sw_stat = WORK;    //MOVING TO WORK STAT
        }
      else//so we can do the test
        {
          if ( test_time_count >= short_time_test_pass )//test has sucseed going back to normal mode
            {
              printf( "\n\r SYSTEM TEST HAS BEEN SUCCSEFULL!!\n\r " );
              sw_stat = ST_BY;
              test_time_count = 0;
            }
          else
            {
          if ( Vbat_ADC_val > min_bat_voltage_for_test )//bat voltage is steel high enghouf
            {
              sw_stat = SHORT_TEST;
              test_time_count++;
              printf( "\n\rTEST_COUNT IS %d seconds\n\r", test_time_count );
            }
          else//test is fail go to error mode
            {
              printf( "\n\r SYSTEM TEST HAS BEEN FAILD! \n\r " );
              test_time_count = 0;
              test_faild_flag = 1;
              sw_stat = ERROR;    //MOVING TO WORK STAT
            }
            }
        }
      if ( sw_stat == SHORT_TEST )//so setting outputs
        {
          normal_charging = 0;
          trickl_charging = 0;
          red_led = 0;
          green_led = 0;
          main_led_stat = 1;
        }
    }
}

void output_hendler( )
{
  BAT_CHARGE_EN = normal_charging;
  TRICKLE_CHARGE_EN = trickl_charging;
  RED_LED = red_led;
  GREEN_LED = green_led;
  MAIN_LED_PIC = main_led_stat;
  if( last_main_led_stat != main_led_stat )     //if the led stat changed
    activate_buzzer( 1, 1, 2 );             //make a beep
  last_main_led_stat = main_led_stat;         //save the les stat for the next time
}




