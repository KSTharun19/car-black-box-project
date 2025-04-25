#ifndef MAIN_H
#define MAIN_H

#include "clcd.h"
#include "adc.h"
#include "digital_keypad.h"
#include "ds1307.h"
#include "i2c.h"
#include "i2c1.h"
#include "uart.h"
#include "external_eeprom.h"

unsigned char events[10][3] = {"ON", "GN", "G1", "G2", "G3", "G4", "G5", "GR", "C0"};
unsigned char *Main_menu[] = {"VIEW_LOG        ", "CLEAR_LOG       ", "DOWNLOAD_LOG    ", "SET_TIME        ", "                "};
unsigned char key;

//enum for events

enum events
{
   DASH_BOARD   = 0,
   MAIN_MENU    = 1,
   VIEW_LOG     = 2,
   CLEAR_LOG    = 3,
   DOWNLOAD_LOG = 4,
   SET_TIME     = 5,
  };
  
  enum timing
  {
      HOURS = 0,
      MINUTES = 1,
      SECONDS = 2,
  };
 

//Variable initialization
int main_flag;
int Event_index;
static int main_index;
unsigned short speed;
char time[10];
static unsigned char ADDR = 0x00;
unsigned char clear_flag;
unsigned char menu_flag;
unsigned char menu_count = 2;
unsigned char twist_flag;
int enter_key;
int event_count;


//FUNCTION DECLARATIONS
void Dash_board(void);
void main_menu(void);
void display_time(unsigned char *clock_register);
void get_time(unsigned char *clock_register);
void store_events(void);
void Download_log(void);
void clear_log(void);
void view_log(void);
void set_time(void);
unsigned char bcd_to_binary(unsigned char BCD);
unsigned char binary_to_bcd(unsigned char binary);

#endif