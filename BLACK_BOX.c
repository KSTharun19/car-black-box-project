/*
 * File:   BLACK_BOX.c
 * Author: tharun
 *
 * Created on February 6, 2025, 7:57 PM
 */


#include <xc.h>
#include "main.h"
#include "external_eeprom.h"
int view = 0;
//converting from BCD to char and stored that characters in time array
void display_time(unsigned char *clock_register)
{
    //time[10];
    
    time[0] = (((clock_register[0] >> 4) & 0x0F) + '0');
    time[1] = ((clock_register[0] & 0x0F) + '0');
    time[2] = ':';
    time[3] = (((clock_register[1] >> 4) & 0x0F) + '0');
    time[4] = ((clock_register[1] & 0x0F) + '0');
    time[5] = ':';
    time[6] = (((clock_register[2] >> 4) & 0x0F) + '0');
    time[7] = ((clock_register[2] & 0x0F) + '0');
    time[8] = '\0';
    clcd_print(time, LINE2(0));
}
//reading the clock time from RTC to array capable to Store 3 Bytes
void get_time(unsigned char *clock_register)
{
    clock_register[0] = read_ds1307(HOUR_ADDR);
    clock_register[1] = read_ds1307(MIN_ADDR);
    clock_register[2] = read_ds1307(SEC_ADDR);
}
void store_events(void)
{
    if(ADDR == 120)
    {
        view++;
        ADDR = 0x00;
        event_count = 10;
    }
    for(int i=0; i < 8; i++)
    {
        write_ext_eeprom(ADDR++, time[i]);
    }
    write_ext_eeprom(ADDR++, events[Event_index][0]);
    write_ext_eeprom(ADDR++,events[Event_index][1]);
    
    write_ext_eeprom(ADDR++,((speed / 10) + 48));
    write_ext_eeprom(ADDR++,((speed % 10) + 48));
    
}
void Dash_board(void)
{
    unsigned char Clock_register[3];
    unsigned short adc_val;
    speed;
    clcd_print("TIME     EV   SP", LINE1(0));
    get_time(Clock_register);
    
    display_time(Clock_register);
    
    if(key == GEAR_UP)
    {
        if(Event_index == 8)
        {
            Event_index = 1;
        }
        else if(Event_index < 7)
        {
            Event_index++;
        }
            event_count++;
            store_events();
    }
    else if(key == GEAR_DOWN)
    {
        if(Event_index == 8)
        {
            Event_index = 1;
        }
        else if(Event_index > 1)
        {
            Event_index--;
        }
            event_count++;
            store_events();
    }
    else if(key == COLLISION)
    {
            event_count++;
            Event_index = 8;
            store_events();
    }
    else  if(key == ENTER)
    {
            clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
            main_flag = MAIN_MENU;
    }
    
            clcd_print(events[Event_index], LINE2(9));
    
    //READING VALUE FROM THE ADC BY CONNECTING POTENTIOMETR1
    
     adc_val = read_adc(CHANNEL0);
     speed = (adc_val / 10.33);
     if(speed < 99)
     {
            clcd_putch(((speed % 10) + 48), LINE2(15));
            clcd_putch(((speed / 10) + 48), LINE2(14));
     }
}
void main_menu()
{

    clcd_putch('*', LINE1(1));
    if(main_index < 3 && key == GEAR_UP)
    {
         menu_count++;
         main_index++;
         clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    }
    else if(main_index > 0 && key == GEAR_DOWN)
    {
         menu_count--;
         main_index--;
         clcd_write(CLEAR_DISP_SCREEN, INST_MODE);       
    }
    else if(key == ENTER)
    { 
        clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
        main_flag = menu_count;
        
    }
    else if(key == EXIT)
    {
        clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
        clear_flag = 0;
        main_flag = DASH_BOARD;
    }
    else
    {
        clcd_print(Main_menu[main_index], LINE1(3));
        clcd_print(Main_menu[main_index+1], LINE2(3)); 
    }
}
void view_log(void)
{
    int i = 0; // Current log index
    int ADDR1 = 0x00;
    unsigned char time_data[10], event_data[3], speed_data[3];

    if (clear_flag == 0 && event_count > 0)
    {
        clcd_print("# TIME     EV SP", LINE1(0));

        while (1) 
        {
            // Calculating the address of the ith log in EEPROM
            unsigned char ADDR2 = ((i) * 12);
            ADDR1 = ADDR2;

            // Read and store the time (9 chars)
            for (int j = 0; j < 8; j++)
            {
                time_data[j] = read_ext_eeprom(ADDR1++);
            }
            time_data[9] = '\0';  

            // Reading and store the event (2 chars)
            event_data[0] = read_ext_eeprom(ADDR1++);
            event_data[1] = read_ext_eeprom(ADDR1++);
            event_data[2] = '\0';
            
            // Reading and storing the speed (2 chars)
            speed_data[0] = read_ext_eeprom(ADDR1++);
            speed_data[1] = read_ext_eeprom(ADDR1++);
            speed_data[2] = '\0'; 

            // Display the current log
            clcd_putch((i + '0'), LINE2(0)); 
            clcd_print(time_data, LINE2(2));
            clcd_print(event_data, LINE2(11));
            clcd_print(speed_data, LINE2(14));

           //NAVIGATING AND EXITING 
            while (1)
            {
                key = read_digital_keypad(STATE);

                if (key == EXIT) 
                {
                    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
                    main_flag = MAIN_MENU;
                    return;
                }
                else if (key == GEAR_UP)
                {
                    
                    if (i < event_count - 1 && i < 9)
                    {
                        //__delay_us(400);
                        i++;
                        //__delay_us(400);
                    }
                    __delay_ms(200);
                    break;
                }
                else if (key == GEAR_DOWN) 
                {
                    
                    if (i > 0)
                    {
                        //__delay_us(100);
                        i--;
                        //__delay_us(100);
                    }
                    __delay_ms(200);
                    break; 
                }
            }
        }
    }
    else
    {
        // No logs available
        ADDR = 0x00;
        event_count = 0;
        clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
        clcd_print("NO_LOGS(EMPTY)", LINE1(2));
       __delay_us(410000);
        clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
        main_flag = MAIN_MENU;
    }
}
void Download_log(void)
{
    int ADDR1 = 0x00;
    unsigned char time_data[10], event_data[3], speed_data[3];

    if (clear_flag == 0 && event_count > 0)
    {
        puts("TIME      EVENT  SPEED\r\n");

        for (int i = 0; i < event_count; i++)
        {
            if(ADDR1 == 120)
            {
                ADDR1 = 0;
                event_count = 10;
            }
            // Reading and storing the time (9 chars)
            for (int j = 0; j < 8; j++)
            {
                time_data[j] = read_ext_eeprom(ADDR1++);
               
            }
            time_data[9] = '\0';  
            puts(time_data);
            puts("  ");

            
            event_data[0] = read_ext_eeprom(ADDR1++);
            event_data[1] = read_ext_eeprom(ADDR1++);
            event_data[2] = '\0';  // Null-terminate
            puts(event_data);
            puts("     ");

            
            speed_data[0] = read_ext_eeprom(ADDR1++);
            speed_data[1] = read_ext_eeprom(ADDR1++);
            speed_data[2] = '\0';  // Null-terminate
            puts(speed_data);
            puts("\r\n");
            
        }

        clcd_print("DOWNLOADED", LINE1(1));
       
    }
    else
    {
        ADDR = 0x00;
        event_count = 0;
        clcd_print("                ", LINE2(0));
        clcd_print("NO_LOGS(EMPTY)", LINE1(2));
    }

    __delay_us(410000);
     ADDR1 = 0x00;
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    main_flag = MAIN_MENU;
}
void clear_log(void)
{
    clear_flag = 1;
    ADDR = 0x00;
    clcd_print("CLEARED......", LINE2(2));
    __delay_us(410000);
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    main_flag = MAIN_MENU;
}
void set_time(void)
{
    unsigned char Clock_register[3];
    unsigned char blink = 0;
    unsigned char time_flag = 0;
    //unsigned long delay = 30000;
    
    get_time(Clock_register);
    clcd_print("HH MM SS", LINE1(0));
    display_time(Clock_register);
    
    while(1)
    {
        key = read_digital_keypad(STATE);
       
        if(key == GEAR_DOWN)
        {
            if(time_flag == 2)
            {
                time_flag = 0;
            }
            else
            {
                time_flag++;
            }
        }
        else if(key == GEAR_UP)
        {
            if(time_flag == 0)
            {
                unsigned char hour = bcd_to_binary(Clock_register[0]);
                    if(hour == 23)
                    {
                       hour = 0;
                    }
                    else
                    {
                       hour++; 
                    }
                Clock_register[0] = binary_to_bcd(hour);
            }
            else if(time_flag == 1)
            {
                   unsigned char minute = bcd_to_binary(Clock_register[1]);
                    if(minute == 59)
                    {
                       minute = 0;
                    }
                    else
                    {
                       minute++; 
                    }
                Clock_register[1] = binary_to_bcd(minute);
            }
            else if(time_flag == 2)
            {
                unsigned char second = bcd_to_binary(Clock_register[2]);
                    if(second == 59)
                    {
                       second = 0;
                    }
                    else
                    {
                      second++; 
                    }
                Clock_register[2] = binary_to_bcd(second);
            }
            display_time(Clock_register);
            //__delay_us(300);
        }
        else if (key == EXIT)
        {
            clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
            main_flag = MAIN_MENU;
            break;
        }
        else if(key == ENTER)
        {
            write_ds1307(HOUR_ADDR, Clock_register[0]);
            write_ds1307(MIN_ADDR, Clock_register[1]);
            write_ds1307(SEC_ADDR, Clock_register[2]);
            clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
            main_flag = MAIN_MENU;
            break;
        }
        
        if(blink)
        {
           if(time_flag == 0)
           {
               clcd_putch(' ', LINE2(0));
               clcd_putch(' ', LINE2(1));
               
           }
           else if(time_flag == 1)
           {
               clcd_putch(' ', LINE2(3));
               clcd_putch(' ', LINE2(4));
           }
           else if(time_flag == 2)
           {
               clcd_putch(' ', LINE2(6));
               clcd_putch(' ', LINE2(7));
           }
        }
        else
        {
            display_time(Clock_register);
        }
        blink = !blink;
        __delay_ms(500);
    }
}

unsigned char bcd_to_binary(unsigned char BCD)
{
    return((BCD >> 4)* 10) + (BCD & 0x0F);
}

unsigned char binary_to_bcd(unsigned char binary)
{
    return((binary/10) << 4) | (binary % 10);
}


//if(time_flag == 0)
//            {
//                if(delay == 0)
//                {
//                   delay = 30000;
//                   if(blink == 0)
//                   {
//                      Clock_register[0] = ' ';   
//                   }
//                   else
//                   {
//                      Clock_register[0] = read_ds1307(HOUR_ADDR); 
//                   }
//                   Clock_register[1] = read_ds1307(MIN_ADDR);
//                   Clock_register[2] = read_ds1307(SEC_ADDR);
//                   display_time(Clock_register);
//                   blink = !blink;
//                }
//                else 
//                {
//                   delay--;
//                }
//            }
//            else if(time_flag == 1)
//            {
//                if(delay == 0)
//                {
//                   delay = 30000;
//                   if(blink == 0)
//                   {
//                      Clock_register[1] = ' ';   
//                   }
//                   else
//                   {
//                      Clock_register[1] = read_ds1307(MIN_ADDR); 
//                   }
//                   Clock_register[0] = read_ds1307(HOUR_ADDR);
//                   Clock_register[2] = read_ds1307(SEC_ADDR);
//                   display_time(Clock_register);
//                   blink = !blink;
//                }
//                else 
//                {
//                   delay--;
//                }
//            }
//            else if(time_flag == 2)
//            {
//                if(delay == 0)
//                {
//                   delay = 30000;
//                   if(blink == 0)
//                   {
//                      Clock_register[2] = ' ';   
//                   }
//                   else
//                   {
//                      Clock_register[2] = read_ds1307(SEC_ADDR); 
//                   }
//                   Clock_register[0] = read_ds1307(HOUR_ADDR);
//                   Clock_register[1] = read_ds1307(MIN_ADDR);
//                   display_time(Clock_register);
//                   blink = !blink;
//                }
//                else 
//                {
//                   delay--;
//                }
//            }
//    




