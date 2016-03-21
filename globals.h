/* 
 * File:   globals.h
 * Author: eitan
 *
 * Created on 30 ????? 2014, 14:04
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

//~~~~~~~~~~~~~~global variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    unsigned int test_sw_count;
    unsigned int GRID_Voltage_ADC_val,Vbat_ADC_val;
    unsigned int Vin_ADC_accum,Vbat_ADC_val_accum;
    unsigned int previous_charge_stat;
    unsigned int previous_trickle_stat;
    unsigned int test_led_time_count;
    unsigned char normal_charging;
    unsigned char trickl_charging;
    unsigned char red_led;
    unsigned char green_led;
    unsigned int hour_counter;
    unsigned int saved_hours_counter;
    unsigned int week_in_houres;
    unsigned int saved_error_buzzer_count;
    unsigned char error_buzzer_count;
    unsigned int grid_power_low_voltage;
    unsigned int grid_power_high_voltage;
    unsigned int long_press_time;
    unsigned int short_press_time_min;
    unsigned int short_press_time_max;
    unsigned int year_time;
    unsigned int shorted_bat_voltage;
    unsigned int min_bat_voltage_normal_cahrging;
    unsigned int batt_full_voltage;
    unsigned int bat_is_shorted_voltage;
    unsigned int num_of_bat_cells;

    unsigned int total_shorted_bat_voltage;
    unsigned int total_min_bat_voltage_normal_cahrging;
    unsigned int total_batt_full_voltage;
    unsigned int total_bat_is_disconnected_voltage;
    unsigned int total_led_off_bat_voltage;
    unsigned int total_led_on_bat_voltage;

    unsigned int time_to_pass_test;
    unsigned int test_time_count;
    unsigned int min_bat_voltage_for_test;
    unsigned int short_time_test_pass;
    unsigned int long_time_test_pass;
    unsigned int blink_sec_count;
    static persistent int test_faild_flag;

    unsigned int shorted_bat_voltage;
    unsigned int min_bat_voltage_normal_cahrging;
    unsigned int batt_full_voltage;
    unsigned int bat_is_disconnected_voltage;
    unsigned int led_off_bat_voltage;
    unsigned int led_on_bat_voltage;


    unsigned int WDT_cnter;
    unsigned int exit_sleep_press;
    unsigned int exit_sleep_count;

#endif

