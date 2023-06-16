#include <Arduino.h>
#include "TM1638.h"

const uint8_t bcd_array[] = {
  // gfedcba
  0b00111111,  //0
  0b00000110,  //1
  0b01011011,  //2 
  0b01001111,  //3
  0b01100110,  //4
  0b01101101,  //5
  0b01111101,  //6
  0b00000111,  //7
  0b01111111,  //8
  0b01101111
};

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
  TM1368Contro::send_command(0x8F);
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

void TM1368Contro::send_int(uint32_t aVal) {
  uint32_t digit = 0;
  uint32_t len = 0;
  uint32_t num = aVal;

  if (aVal == 0)
    TM1368Contro::send_to_address(bcd_array[digit_array[0]], 0x00 );

  while (num != 0) {
    ++len;
    num /= 10;
  }

  for (uint8_t i = 0; i < len; ++i) {
    digit = aVal % 10;
    aVal /= 10;
    digit_array[i] = digit;
  }

  for (uint8_t i = len; i > 0; --i) {
    TM1368Contro::send_to_address(bcd_array[digit_array[i-1]], 0x00 + 2*(len - i));
  }
}
