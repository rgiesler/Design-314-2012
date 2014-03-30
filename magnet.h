void magnet_initialise(void);
char magnet_read (char eeprom_adres, int *x, int *y, int *z);
char magnet_write (char eeprom_adres, unsigned int data_adres, char data);
void format_text (int mag, char *temp);
double get_angle (double mag_x_def, double mag_y_def);