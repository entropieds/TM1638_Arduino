#include "TM1638.h"


#define DIO_PIN 2   // Data pin
#define CLK_PIN 3   // Clock pin
#define STB_PIN 4   // Strobe pin

TM1368Control Panel;

float a = 666666.6999;

void setup() {
  Panel.chip_init(CLK_PIN, DIO_PIN, STB_PIN);
  Panel.send_string("7355608");
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}
