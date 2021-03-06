/*
 * @file_name	: lcd.c
 *
 * @author	: Amey Dashaputre, Tanmay Kothale, Varun Mehta
 *
 * @references	: 1. https://github.com/cu-ecen-aeld/final-project-support-midhun-venkat/blob/master/final_test_client_integrated/lcd.c
 *		  2. https://www.instructables.com/How-to-Interface-a-16x4-LCD-With-an-Arduino/
 *
 * @brief	: Socket server application code. Connects to client, reads data stored by facial recognition
 *		  application and temperature sensor application and sends it to client.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lcd.h"

//Defines for GPIO pins on Raspberry Pi
#define LCD_E 23
#define LCD_RS 22
#define LCD_D4 24
#define LCD_D5 25
#define LCD_D6 8
#define LCD_D7 7

void pulseEnable ()
{
   digitalWrite (LCD_E, HIGH) ; 
   delay(2); //  2 millisecond pause - enable pulse must be > 450ns
   digitalWrite (LCD_E, LOW) ; 
}

/*
  send a byte to the lcd in two nibbles
  before calling use SetChrMode or SetCmdMode to determine whether to send character or command
*/
void lcd_byte(char bits)
{
  digitalWrite (LCD_D4,(bits & 0x10)) ;  
  digitalWrite (LCD_D5,(bits & 0x20)) ;  
  digitalWrite (LCD_D6,(bits & 0x40)) ;  
  digitalWrite (LCD_D7,(bits & 0x80)) ;  
  pulseEnable();

  digitalWrite (LCD_D4,(bits & 0x1)) ;  
  digitalWrite (LCD_D5,(bits & 0x2)) ;  
  digitalWrite (LCD_D6,(bits & 0x4)) ;  
  digitalWrite (LCD_D7,(bits & 0x8)) ;  
  pulseEnable();      	
}

void SetCmdMode() 
{
  digitalWrite (LCD_RS, 0); // set for commands
}

void SetChrMode() 
{
  digitalWrite (LCD_RS, 1); // set for characters
}

void lcd_text(char *s)
{
  while(*s) 
	lcd_byte(*s++);
 }

void lcd_init()
{
   wiringPiSetupGpio () ; // use BCIM numbering
   // set up pi pins for output
   pinMode (LCD_E,  OUTPUT);
   pinMode (LCD_RS, OUTPUT);
   pinMode (LCD_D4, OUTPUT);
   pinMode (LCD_D5, OUTPUT);
   pinMode (LCD_D6, OUTPUT);
   pinMode (LCD_D7, OUTPUT);
   
   //pinMode (GPIO_LED, OUTPUT); // added a led gpio for indication
   // initialise LCD
   SetCmdMode(); // set for commands
   lcd_byte(0x33); // full init 
   lcd_byte(0x32); // 4 bit mode
   lcd_byte(0x28); // 2 line mode
   lcd_byte(0x0C); // display on, cursor off, blink off
   lcd_byte(0x01);  // clear screen
   delay(3);        // clear screen is slow!
}

void lcd_clear()
{
   SetCmdMode();
   lcd_byte(0x01);  // clear screen
   delay(3);        // clear screen is slow!
}


void lcd_print(char *msg) 
{
  SetCmdMode();    // set for commands
  delay(2);
  lcd_byte(0x01);  //Clear screen
  delay(3);
  lcd_byte(0x02);
  delay(3);
  lcd_byte(0x80);  // set home loc
  delay(3);
  SetChrMode(); 
  lcd_text(msg);
  delay(5);
}

