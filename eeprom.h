char eeprom_skryf (char eeprom_adres, unsigned int data_adres, char data);
char eeprom_lees (char eeprom_adres, unsigned int data_adres, char *data);
void i2c_init(void);