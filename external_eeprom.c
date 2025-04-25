#include <xc.h>
#include "i2c1.h"
#include "external_eeprom.h"


unsigned char read_ext_eeprom(unsigned char addr)
{
    unsigned char data;
    
    i2c1_start();
    i2c1_write(SLAVE_WRITE1);
    i2c1_write(addr);
    i2c1_rep_start();
    i2c1_write(SLAVE_READ1);
    data = i2c1_read(0);
    i2c1_stop();
    
    return data;
}

void write_ext_eeprom(unsigned char addr, unsigned char data) // SEc_ADDR, data
{
    i2c1_start();
    i2c1_write(SLAVE_WRITE1);
    i2c1_write(addr);
    i2c1_write(data);
    i2c1_stop();
    __delay_us(3000);
}
