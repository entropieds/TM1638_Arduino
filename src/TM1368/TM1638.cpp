#include <Arduino.h>
#include "TM1638.h"

void TM1368Contro::chip_init(uint8_t clk, uint8_t dio, uint8_t stb) {
  this->CLK_PIN = clk;
  this->DIO_PIN = dio;
  this->STB_PIN = stb;
  pinMode(clk, OUTPUT);
  pinMode(dio, OUTPUT);
  pinMode(stb, OUTPUT);
  digitalWrite(clk, HIGH);
  digitalWrite(stb, HIGH);
  TM1368Contro::clear_reg();
}

void TM1368Contro::send_data(uint8_t data) {
  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(CLK_PIN, LOW);
    digitalWrite(DIO_PIN, data & 0x01);
    data >>= 1;
    digitalWrite(CLK_PIN, HIGH);
  }
}

void TM1368Contro::send_command(uint8_t command) {
  digitalWrite(STB_PIN, LOW);
  TM1368Contro::send_data(command);
  digitalWrite(STB_PIN, HIGH);
}

void TM1368Contro::send_to_address(uint8_t data, uint8_t address) {
  TM1368Contro::send_command(0x44);
  digitalWrite(STB_PIN, LOW);
  TM1368Contro::send_data(0xC0 | address);
  TM1368Contro::send_data(data);
  digitalWrite(STB_PIN, HIGH);
}

void TM1368Contro::clear_reg() {
  TM1368Contro::send_command(0x40);
  digitalWrite(STB_PIN, LOW);
  TM1368Contro::send_data(0xC0);
  for(uint8_t i = 0; i < 16; ++i)
    TM1368Contro::send_data(0x00);
  digitalWrite(STB_PIN, HIGH);
}

void TM1368Contro::send_int(uint16_t aVal) {
  uint16_t digit = 0;
    uint16_t len = 0;
    uint16_t num = aVal;
    
    if (aVal == 0) {
      TM1368Contro::send_to_address(0x3F, 0x00);
      return;
    }

    while (num != 0) {
      ++len;
      num /= 10;
    }

    for (uint16_t i = 0; i < len; ++i) {
      digit = aVal % 10;
      aVal /= 10;
      TM1368Contro::send_to_address(TM1368Contro::dec2bcd(digit), (0x00 + 2*i));
    }
}

uint8_t TM1368Contro::dec2bcd(uint8_t dec){
	return ((dec/10 * 16) + (dec % 10));
}