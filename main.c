/*
 * File:   main.c
 * Author: tharun
 *
 * Created on February 7, 2025, 1:49 PM
 */


#include <xc.h>
#include "main.h"
#include "i2c.h"
#include "i2c1.h"
#pragma config WDTE=OFF

void init_config(void)
{
    init_i2c(100000);
    init_ds1307();
    init_clcd();
    init_i2c1(100000);
    init_uart(9600);
    init_digital_keypad();
    
    init_adc();
    
}

void main(void) {
    init_config();
    while(1)
    {
        
      key = read_digital_keypad(STATE);

      if(main_flag == DASH_BOARD)
      { 
          Dash_board();
      }
      else if(main_flag == MAIN_MENU)
      {
          main_menu();
      }
      else if(main_flag == VIEW_LOG)
      {
            view_log();
      }
      else if(main_flag == CLEAR_LOG)
      {
            clear_log();
      }
      else if(main_flag == DOWNLOAD_LOG)
      {
            Download_log();
      }
      else if(main_flag == SET_TIME)
      {
            set_time();
      }
      
    }
    return;
}
