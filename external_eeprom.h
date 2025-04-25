
#ifndef EXT_EEPROM_H
#define	EXT_EEPROM_H

#define SLAVE_WRITE1             0b10100000 // 0xA0
#define SLAVE_READ1              0b10100001 // 0xA1
#define _XTAL_FREQ                  20000000

unsigned char read_ext_eeprom(unsigned char addr);
void write_ext_eeprom(unsigned char addr, unsigned char data);

#endif	
