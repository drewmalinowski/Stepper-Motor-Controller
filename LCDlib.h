#define LCD_DATA (BIT_0|BIT_1|BIT_2|BIT_3|BIT_4|BIT_5|BIT_6| BIT_7)
#define LCD_EN BIT_5
#define LCD_RS BIT_15
#define LCD_RW BIT_4

void initialize_PMP();
void initialize_LCD();
int LCD_read(int RS);
void writeLCD(int addr, char c);
char readLCD(int addr);
void LCD_puts(char *char_string);
void LCD_putc(char a);
char busyLCD(void);