#ifndef I2C1_H
#define	I2C1_H

#define FOSC            20000000

void init_i2c1(unsigned long baud);
void i2c1_start(void);
void i2c1_rep_start(void);
void i2c1_stop(void);
unsigned char i2c1_read(unsigned char ack);
int i2c1_write(unsigned char data);

#endif	/* I2C_H */

