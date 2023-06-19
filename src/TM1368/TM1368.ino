#include "TM1638.h"
#include "keyboard.h"

#define DIO_PIN 2   // Data pin
#define CLK_PIN 3   // Clock pin
#define STB_PIN 4   // Strobe pin
#define BRIGHTNES 0x0C

TM1368Control Panel;
keypadMatrix KeyPad;

char key = '\0';

ISR (TIMER1_COMPA_vect) {
  flag = true;
}

void setup() {
  Panel.chip_init(CLK_PIN, DIO_PIN, STB_PIN, BRIGHTNES);
  KeyPad.keypad_init();
  KeyPad.timer_init();
  Serial.begin(9600);
}

void loop() {
  if (flag) {
    flag = false;
    key = KeyPad.get_key();
  cli();
  if (key != '\0')
    Panel.send_char(key);
  
  if (key == '*') 
    Panel.clear_reg();
  
  if (key == '#') 
    Panel.delete_symbol();
  sei();
  }
}
