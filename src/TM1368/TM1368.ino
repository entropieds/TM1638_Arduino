#include "TM1638.h"

#define DIO_PIN 2   // Data pin
#define CLK_PIN 3   // Clock pin
#define STB_PIN 4   // Strobe pin
#define BRIGHTNES 0x0A

TM1368Control Panel;

uint8_t buttonNum = 0;
uint8_t address = 0x00;
char txt[8];
bool delete_flag = false;


void setup() {
  Panel.chip_init(CLK_PIN, DIO_PIN, STB_PIN, BRIGHTNES);
  Serial.begin(9600);
  Serial.println("Enter message:");
}

void loop() {
  idn_btn(Panel.read_button());
  while (Panel.read_button()) {
    Panel.clear_reg();
    Panel.send_int_to_addr(buttonNum, address);
    Panel.set_led(1, address + 1);
    delete_flag = true;
  }
  
  if (delete_flag) {
    Panel.delete_symbol();
    address = 0x00;
    Panel.send_symbol_string_to_addr(txt, address);
  }
  
  Panel.set_led(0, address + 1);
  read_from_serial();

  delay(50);
}

void read_from_serial() {
  address = 0x00;
  if (Serial.available() != 0) {
  String msg= Serial.readString();       //read until timeout
  msg.trim();                            // remove any \r \n whitespace at the end of the String
  uint8_t len = msg.length();
  msg.toCharArray(txt,len + 1);
  txt[len + 1] = '\0';
  Panel.send_symbol_string_to_addr(txt, address);
  }
}


void idn_btn(uint8_t buttton) {
  switch (buttton) {
    case 1 :
      buttonNum = 1;
      address = 0x00;
      break;
    case 2 :
      buttonNum = 2;
      address = 0x02;
      break;
    case 4 :
      buttonNum = 3;
      address = 0x04;
      break;
    case 8 :
      buttonNum = 4;
      address = 0x06;
      break;
    case 16 :
      buttonNum = 5;
      address = 0x08;
      break;
    case 32 :
      buttonNum = 6;
      address = 0x0A;
      break;
    case 64 :
      buttonNum = 7;
      address = 0x0C;
      break;
    case 128 :
      buttonNum = 8;
      address = 0x0E;
      break;
    default :
      buttonNum = 0;
      delete_flag = false;
      break;
  }
}