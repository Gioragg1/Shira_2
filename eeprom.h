//eeprom.h



#define EEPROM_START_ADD	 0x00

enum saved_eeprom_add
	{
        GRID_POWER_LOW_VOLTAGE_EE_ADD=EEPROM_START_ADD,
        GRID_POWER_LOW_VOLTAGE_LSB_EE_ADD,
        GRID_POWER_HIGH_VOLTAGE_EE_ADD,
        GRID_POWER_HIGH_VOLTAGE_LSB_EE_ADD,
        SPARE1,
        SPARE2,
        SPARE3,
        SPAER4,

        LONG_PRESS_TIME_EE_ADD,                
        LONG_PRESS_TIME_LSB_EE_ADD,                
        SHORT_PRESS_TIME_MIN_EE_ADD,           
        SHORT_PRESS_TIME_LSB_MIN_EE_ADD,           
        SHORT_PRESS_TIME_MAX_EE_ADD,          
        SHORT_PRESS_TIME_LSB_MAX_EE_ADD,
        SPARE5,
        SPARE6,
        
        YEAR_TIME_EE_ADD,                      
        YEAR_TIME_LSB_EE_ADD,                      
        SAVED_TIME_FOR_TEST_EE_ADD,
        SAVED_TIME_FOR_TEST_LSB_EE_ADD,
        WEEK_TIME_EE_ADD,
        WEEK_TIME_LSB_EE_ADD,
        ERROR_BUZER_COUNT_EE_ADD,
        ERROR_BUZER_COUNT_LSB_EE_ADD,

        MAX_BAT_VOLTAGE_FOR_LED_OFF_EE_ADD,      //3V - AVOVE 3v THE LED CAN BE ON
        MAX_BAT_VOLTAGE_FOR_LED_OFF_LSB_EE_ADD,
        MIN_BAT_VOLTAGE_FOR_LED_ON_EE_ADD,      //2.8V BELOW 2.8v THE LED MUST BE OFF
        MIN_BAT_VOLTAGE_FOR_LED_ON_LSB_EE_ADD,
        BAT_FULL_VOLTAGE_EE_ADD,
        BAT_FULL_VOLTAGE_LSB_EE_ADD,
        BAT_IS_DISCONNECTED_VOLTAGE_EE_ADD,
        BAT_IS_DISCONNECTED_VOLTAGE_LSB_EE_ADD,

        SPARE11,
        SPARE12,
        NUM_OF_BATT_CELLS_EE_ADD,
        NUM_OF_BATT_CELLS_LSB_EE_ADD,
        SHORT_TEST_TIME_EE_ADD,
        SHORT_TEST_TIME_LSB_EE_ADD,
        LONG_TEST_TIME_EE_ADD,
        LONG_TEST_TIME_LSB_EE_ADD,
	};

__EEPROM_DATA(	0,220,                      //220 is for 160V  GRID_POWER_FAILOR point
		0,238,                      //238 is for 180v  voltage that the led turned off after power grid is fail or down (histeresis option)(ADC units)
		0xFF,0xFF,
                0xFF,0xFF);

__EEPROM_DATA(	0x01,0xA4,                   //LONG_PRESS_TIME_EE_ADD time to detect long sw press(counter units) 200
		0x00,0x03,                   //SHORT_PRESS_TIME_MIN_EE_ADD time to detect short sw press(counter units)
		0x00,0x96,                   //SHORT_PRESS_TIME_MAX_EE_ADD time to detect short sw press(counter units)
                0xFF,0xFF);

__EEPROM_DATA(	0x22,0x38,                   //YEAR_TIME_EE_ADD IF WE SAVE EVREY HOURS SO IN A YEAR WE HAVE 8760 SAVES
		0x00,0x00,                   //SAVED_TIME_FOR_TEST
		0x00,0xA8,                   //TIME FOR 1 WEEK 168 HOURES
                0x00,0x0A);                  //ERROR_BUZER_COUNT_EE_ADD 10 SEC


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * 
 *  we work with NiCd 3 cells 3.7V 800mA so all the parameters are for this batterey. in the analog input circuit we have 1:2 voltage divider.
 * so we define this (all the numbers are for 1 cell):
 * 0-0.66V  - batt SHORTED
 * 0.66V-1V - batt EMPTY LED is off
 * 1V - 1.46V -batt  NEED_CHARGING LED can work & normal charging can be done
 * 1.46v - 1.7V - FULL_CHARGED need trickle charge
 * 1.7 and higher - bat DISSCONECTED led off & charge off
 *
 * 1V -    above this voltage maib led can be turned on in power failior
 * 0.93V - balow this voltage maib led must be OFF

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

__EEPROM_DATA(	0x00,120,                    // MAX_BAT_VOLTAGE_FOR_LED_OFF 120ADC =~1v per cell
                0x00,114,                    // MIN_BAT_VOLTAGE_FOR_LED_ON_EE_ADD 114ADC =~0.93v per cell
		0x00,173,                   // BAT_FULL_VOLTAGE_LSB_EE_ADD 1.46V - 173ADC
                0x00,203);                  // BAT_IS_DISCONNECTED_VOLTAGE_EE_ADD 1.7V - 203ADC

__EEPROM_DATA(	0xFF,0xFF,                  
		0x00,0x03,                   //NUM_OF_BATT_CELLS
		0X00,0x78,                  //SHORT TEST TIME IS 2 MIN 120 SEC
                0x15,0x18);                 //LONG TEST TIME IS 90 MIN 5400 SEC
					
__EEPROM_DATA(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF);				//reset all eepromm adresses
__EEPROM_DATA(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF);				//reset all eepromm adresses

__EEPROM_DATA(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF);				//reset all eepromm adresses
__EEPROM_DATA(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF);				//reset all eepromm adresses

