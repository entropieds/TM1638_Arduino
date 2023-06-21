#include "TM1638.h"
#include "keyboard.h"

#define DIO_PIN 2   // Data pin
#define CLK_PIN 3   // Clock pin
#define STB_PIN 4   // Strobe pin
#define BRIGHTNES 0x0A

TM1368Control Panel;

ISR (TIMER1_COMPA_vect) {
  flag = true;
}

void setup() {
  Panel.chip_init(CLK_PIN, DIO_PIN, STB_PIN, BRIGHTNES);
  Panel.send_char('5');
  Panel.send_symbol_string("UJHLQP9");
  Serial.begin(9600);
}

void loop() {

}
