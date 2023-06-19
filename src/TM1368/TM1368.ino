#include "TM1638.h"


#define DIO_PIN 2   // Data pin
#define CLK_PIN 3   // Clock pin
#define STB_PIN 4   // Strobe pin
#define BRIGHTNES 0x0C

TM1368Control Panel;

void setup() {
  Panel.chip_init(CLK_PIN, DIO_PIN, STB_PIN, BRIGHTNES);
  Panel.send_int(69);  
  Panel.send_string("56.28.56");
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}
