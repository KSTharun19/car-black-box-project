/* 
 * File:   digital_keypad.h
 * Author: Adil
 *
 * Created on 15 April, 2020, 8:22 PM
 */

#ifndef DIGITAL_KEYPAD_H
#define	DIGITAL_KEYPAD_H

#define LEVEL_DETECTION         0
#define LEVEL                   0

#define STATE_DETECTION         1
#define STATE                   1

#define KEYPAD_PORT             PORTB
#define KEYPAD_PORT_DDR         TRISB

#define INPUT_LINES             0x3F

#define GEAR_UP                 0x3E
#define GEAR_DOWN               0x3D
#define COLLISION               0x3B
#define ENTER                   0x37
#define EXIT                     0x2F
#define SW6                     0x1F

#define ALL_RELEASED            INPUT_LINES

unsigned char read_digital_keypad(unsigned char mode);
void init_digital_keypad(void);

#endif	/* DIGITAL_KEYPAD_H */

