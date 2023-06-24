#include "TM1638.h"

#define DIO_PIN 2   // Data pin
#define CLK_PIN 3   // Clock pin
#define STB_PIN 4   // Strobe pin
#define BRIGHTNES 0x0A

TM1368Control Panel;

void setup() {
  Panel.chip_init(CLK_PIN, DIO_PIN, STB_PIN, BRIGHTNES);
  Panel.print_symbol('5');
  Panel.send_string("5.9.5.5.9");
  Serial.begin(9600);
}

void loop() {
  
}
