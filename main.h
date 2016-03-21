/*
 * File:   main.h
 * Author: shlomo marmor
 *
 * Created on 30.01.2014
 */

#define SW_TYPE	'D'				//'R' for relese ver 'D' for development ver
#define SW_VER1	0
#define SW_VER2	1
#define SW_VER3	15
#define SW_VER_STRING 	"SW VER D 1.15"
//#define ICD_DEBUG     //for runing in debug and lab mode or in relese mode
#define  GO_TO_SLEEP_IMIDIATLY_AFTER_POWERUP        //this def will be canceled in ICD_DEBUG mode


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~some parameters for the circuit~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void blink_RED_LED( unsigned char blinks, unsigned int timeMs );
void blink_GREEN_LED( unsigned char blinks, unsigned int timeMs );
void blink_GREEN_LED_and_RED_LED_and_BUZZER( unsigned char blinks, unsigned int timeMs );
void blink_GREEN_LED_and_RED_LED( unsigned char blinks, unsigned int timeMs );
void stby_stat_hendler( void );
void work_stat_hendler( void );
void test_stat_hendler( void );
void error_stat_hendler( void );
void sleep_stat_hendler( void );
void long_test_stat_hendler( void );
void output_hendler( void );
unsigned int load_int_from_eeprom( unsigned char add );
void save_int_to_eeprom( unsigned char add, unsigned int data );
void eeprom_data_refresh( );
void activate_buzzer( unsigned char beeps,unsigned char On_duration,unsigned char Off_duration);
void buzz_off(unsigned char duration);
void buzz_on(unsigned char duration);


//#define GRID_POWER_FAILOR             385                   //the grid power is down or fail.(ADC units)
//#define MAIN_LED_OFF_VOLTAGE          434                  // 465 is for 10.5V voltage that the le dis turned off after power grid is fail or down (histeresis option)(ADC units)
//
//#define LONG_PRESS_TIME                200               //time to detect long sw press(counter units)
//#define SHORT_PRESS_TIME_MIN           3                //time to detect short sw press(counter units)
//#define SHORT_PRESS_TIME_MAX           150              //time to detect short sw press(counter units)
//
//
//#define YEAR_TIME                       8760           //IF WE SAVE EVREY HOURS SO IN A YEAR WE HAVE 8760 SAVES

//#define VIN_VOLTAGE_DIVIDER_COAF       0.139441857                 //this is for voltage calculation for crane card its 1/7 = 0.142857 2M & 330K
//#define TRANSFORMATOR_PROP             1                         // this is for power grid calculation if there is no transformator its 1


//#define SORTED_BAT_VOLTGE                177              //CRYTICAL BAT VOLTAGE
//#define MIN_BAT_VOLTAGE_NORMAL_CAHRGING     355           //min voltage for charging battery(ADC units)
//#define MAX_BAT_VOLTAGE_NORMAL_CAHRGING     659            //max voltage for normal charging(ADC units)
//#define BAT_IS_SHORTED_VOLTAGE              698


#define ADC_CHANNEL_VIN 1
#define ADC_CHANNEL_VBAT 0

#define V_BAT_TEST_EN_TRIS TRISC0
#define V_BAT_TEST_EN_LAT     LATCbits.LATC0

//struct { // Flags for program
//    unsigned msec10 : 1;        // 10msec time flag
//    unsigned msec100 : 1;       // 100msec time flag
//    unsigned msec300 : 1;       // 100msec time flag
//    unsigned msec1000 : 1;      // 1000msec time flag
//    unsigned sec10 : 1;         // 10sec time flag
//    unsigned min1 : 1;          // 1min time flag
//    unsigned bit6 : 1;
//    unsigned bit7 : 1;
//} stat;

enum POWER_STAT{        //THIS IS FOR THE GRIS POWER MONITOR
    POWER_OK,
    POWER_FAIL
}power_stat;

enum BATT_STAT{
    SHORTED,            //if we got zero in bat voltage
    //EMPTY ,             //batt is lower then 1V per cell
    NEED_CHARGING,      //for normal condition
    FULL_CHARGED,       //for full charged battary
    DISSCONECTED        //if we got to high voltage in Vbat
}batt_stat;

enum CHARGING_STAT{ //todo delete this !!!
    NO_POWER,           //for battery fault or dissconnected
    CHARGING_OFF,       //if main led is working
    TRICKLE_CHARGING,   //for batterey FULL_CHARGED
    NORMAL_CHARGING,    //normal charging
}charging_stat;

typedef enum {
    INIT,
    SLEEP,
    ST_BY,
    WORK,
    TEST,
    ERROR,
    LONG_TEST,
    SHORT_TEST,
}SW_STAT;

extern SW_STAT last_sw_stat,sw_stat;

enum SWITCH_DIRECTION{
    UP,
    DOWN,
}sw_direction;

enum TEST_SW_STAT{
    SW_RELEASED,
    SHORT_PRESS_DETECTED,
    LONG_PRESS_DETECTED,
}Switch_stat;

enum MAIN_LED_STAT{
    MAIN_LED_OFF,
    MAIN_LED_ON,
}main_led_stat;





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

enum UNIT_TYPE {
    SHIRA_2,
} unit_type;





//---------------------------------------------------------------------
//		TMR1 and timing Definitions
//---------------------------------------------------------------------


//#define	T1H_PRESET_VAL	0x3C    //for 100Ms at 4Mhz
//#define	T1L_PRESET_VAL	0xAF

#define	T1H_PRESET_VAL	0xEC    //for 10Ms at 4Mhz
#define	T1L_PRESET_VAL	0x77

//#define	T1H_PRESET_VAL	0x76    //for 10Ms at 4Mhz
//#define	T1L_PRESET_VAL	0x3B

#define	COUNT_100MSEC		10      	// Number of Compare timeouts for 100ms
#define	COUNT_1000MSEC		10		// Number of Compare timeouts for 1000ms
#define	COUNT_500MSEC		5		// Number of Compare timeouts for 500ms
#define	COUNT_10SEC		10		// Number of Compare timeouts for 10sec
#define	COUNT_1MIN		60		// Number of Compare timeouts for 1minit
#define	COUNT_10MIN		10		// Number of Compare timeouts for 1minit
#define	COUNT_1HOUR		6		// Number of Compare timeouts for 1minit

struct{														// Flags for program
 unsigned	msec5:1;				// 10msec time flag
 unsigned	msec10:1;				// 10msec time flag
 unsigned	msec100:1;				// 100msec time flag
 unsigned       msec500:1;                              // 500msec time flag
 unsigned	msec1000:1;				// 1000msec time flag
 unsigned	sec10:1;
 unsigned	min1:1;
 unsigned       min10:1;
 unsigned	hour1:1;
} stat ;
unsigned char Count_100m, // 	100msec counter
Count_1000m, //	1000msec counter
Count_500m,//300msec Counter
Count_10m,//300msec Counter
Count_10Sec,
Count_1min, //	1min counter
Count_10min, //	1min counter
Count_1hour; //	1hour counter

unsigned char start_analog_sequence_flag;
unsigned char analog_delay_count;
unsigned char start_sampling_flag;
unsigned char analog_samples_count;
unsigned char second_sample_count;
unsigned char power_fail_count;

//signed int wanted_zoom;
long working_timeout;

unsigned char curent_save_position;

//volatile union
//	{
//	unsigned char b[4];
//	unsigned int i[2];
//	unsigned long l;
//	}volatile tmpl;
//
//volatile union
//	{
//	unsigned char b[2];
//	unsigned int i;
//	}volatile temp_i;

unsigned char a; //tmp reg
unsigned char i; //tmp reg

//----------------- flags defines ------------------------------------
unsigned char FLearn; // Learn mode active
//--------------------------------------------------------------------
// timings
#define TLEARN  100							//  10s  learn timeout

unsigned int CLearn, CTLearn; // learn timers and counter




unsigned int tmpI;

unsigned char msec100buzz;
unsigned char BuzzTimer;
#define	NORMAL_BUZZ_TIME	2	//2*72ms
#define	RECIVE_BUZZ_TIME	2	//2*72ms
#define	LEARN_BUZZ_TIME		4
#define	LEARN_END_BUZZ_TIME	4
#define	HIT_BUZZ_TIME		3


#define set_pwm_2900hz	  	PR2=85 ; CCPR2L=40
#define set_pwm_1000hz	  	PR2=250; CCPR2L=125
#define set_pwm_1500hz	  	PR2=166; CCPR2L=83

//#define piezo_buzz		
#ifdef  piezo_buzz              //for AC operated buzzer
#define buzz_on			CCP2CON=0x0f
#define buzz_off		CCP2CON=0x00
#else                           //for DC operated buzzer
//#define buzz_on			BUZZ=1
//#define buzz_off		BUZZ=0
#endif


unsigned char lrn_sw_presed_on_remote;

unsigned char i;




unsigned char eraze_transmiters_cnt;
void my_eeprom_write(unsigned char addr, unsigned char value);

#define SET_BATT_LED_OFF    LED_BATT_GREEN=0; LED_BATT_RED =0
#define SET_BATT_LED_GREEN  LED_BATT_GREEN=1; LED_BATT_RED =0
#define SET_BATT_LED_ORANGE LED_BATT_GREEN=1; LED_BATT_RED =1
#define SET_BATT_LED_RED    LED_BATT_GREEN=0; LED_BATT_RED =1

unsigned int    batt_voltage,
                solar_cell_voltage;

#define  BATT_FULLY_CHARHED  624 //for 14V
#define  BATT_VOLTAGE_LOW    408 //for 9V

unsigned char Sync_Last;
#define  _LEARN_WINDOW_WHEN_JUMPER_INSERTED_30_SEC   4;         //for 30 Sec in 1 sec units when jumper is inserted
#define  _LEARN_WINDOW_1_MIN    6;         //for 1 minit in 10 sec units
#define  _LEARN_WINDOW_3_MIN    18;         //for 3 minit in 10 sec units

unsigned char Learn_Window;





/*  LED HANDLER */
unsigned int    RED_LED_10mS_Timer,
                GREEN_LED_10mS_Timer,
                ORANGE_LED_10mS_Timer;

/* Led timonf in 10mSec units*/
#define _10mS   1
#define _20mS   2
#define _30mS   3
#define _50mS   5
#define _100mS   10
#define _200mS   20
#define _300mS   30
#define _500mS   50
#define _1Sec   100




unsigned char led_blink_cnt;
enum TIME_IN_SEC_UNITS{ _0Sec_IN_SEC_UNITS,
                        _1Sec_IN_SEC_UNITS,
                        _2Sec_IN_SEC_UNITS,
                        _3Sec_IN_SEC_UNITS,
                        _4Sec_IN_SEC_UNITS,
                        _5Sec_IN_SEC_UNITS,
                        _6Sec_IN_SEC_UNITS,
                        _7Sec_IN_SEC_UNITS,
                        _8Sec_IN_SEC_UNITS,
                        _9Sec_IN_SEC_UNITS};

unsigned char title_timeout;

#define     _100MSec_IN_100Msec_UNITS   1
#define     _1Sec_IN_100Msec_UNITS      10
#define     _2Sec_IN_100Msec_UNITS      20
#define     _3Sec_IN_100Msec_UNITS      30
#define     _10Sec_IN_100Msec_UNITS     100
#define     BUZZER_DURATION_COAF        666

unsigned int unit_ID;

#define EXITING_ERROR "\n\rEXITING ERROR"

unsigned char last_main_led_stat;