#include <Arduino.h>
#include "TM1638.h"

//public:

void TM1368Control::chip_init(uint8_t clk, uint8_t dio, uint8_t stb) {
  this->CLK_PIN = clk;
  this->DIO_PIN = dio;
  this->STB_PIN = stb;
  pinMode(clk, OUTPUT);
  pinMode(dio, OUTPUT);
  pinMode(stb, OUTPUT);
  digitalWrite(clk, HIGH);
  digitalWrite(stb, HIGH);
  TM1368Control::clear_reg();
  TM1368Control::send_command(0x8F);
}

// Private:

void TM1368Control::send_data(uint8_t data) {
  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(CLK_PIN, LOW);
    digitalWrite(DIO_PIN, data & 0x01);
    data >>= 1;
    digitalWrite(CLK_PIN, HIGH);
  }
}

void TM1368Control::send_command(uint8_t command) {
  digitalWrite(STB_PIN, LOW);
  TM1368Control::send_data(command);
  digitalWrite(STB_PIN, HIGH);
}

void TM1368Control::send_to_address(uint8_t data, uint8_t address) {
  TM1368Control::send_command(0x44);
  digitalWrite(STB_PIN, LOW);
  TM1368Control::send_data(0xC0 | address);
  TM1368Control::send_data(data);
  digitalWrite(STB_PIN, HIGH);
}

uint8_t TM1368Control::convert_numeral(uint8_t numeral, uint32_t aVal) {
  uint8_t len = 0;
  uint32_t num = aVal;

  while (num != 0) {
    ++len;
    num /= numeral;
  }

  for (uint8_t i = 0; i < len; ++i) {
    this->digit_array[i] = aVal % numeral;
    aVal /= numeral;
  }
  return len;
}

//public:

void TM1368Control::clear_reg() {
  TM1368Control::send_command(0x40);
  digitalWrite(STB_PIN, LOW);
  TM1368Control::send_data(0xC0);
  for(uint8_t i = 0; i < 16; ++i)
    TM1368Control::send_data(0x00);
  digitalWrite(STB_PIN, HIGH);
}

void TM1368Control::send_int(uint32_t aVal) {
  uint8_t len = 0;
  uint32_t num = aVal;

  if (aVal == 0){
    TM1368Control::send_to_address(bcd_array[digit_array[0]], 0x0E );
    return;
  }

  len = TM1368Control::convert_numeral(10, aVal);

  for (uint8_t i = len; i > 0; --i) {
    TM1368Control::send_to_address(bcd_array[digit_array[i-1]], (0x0E - (2*len - 2)) + 2*(len - i));
  }
}

void TM1368Control::send_hex(uint32_t aVal) {
  uint8_t len = 0;
  uint32_t num = aVal;

  if (aVal == 0){
    TM1368Control::send_to_address(hex_array[digit_array[0]], 0x0E );
    return;
  }

  len = TM1368Control::convert_numeral(16, aVal);

  for (uint8_t i = len; i > 0; --i) {
    TM1368Control::send_to_address(hex_array[digit_array[i-1]], (0x0E - (2*len - 2)) + 2*(len - i));
  }
}

void TM1368Control::send_double(float aVal) {
  uint32_t int_part = uint32_t(aVal);
  uint32_t decimal = (aVal - (uint32_t)aVal) * 100;
  uint8_t len;

  //uint8_t last_addr = 0x0E;

  len = convert_numeral(10, decimal);
  last_addr -= 2*len;
  

  for (uint8_t i = len; i > 0; --i) {
    TM1368Control::send_to_address(bcd_array[digit_array[i-1]], (0x0E - (2*len - 2)) + 2*(len - i));
  }

  len = convert_numeral(10, int_part);

  if (int_part == 0) {
    TM1368Control::send_to_address(0b10000000 | bcd_array[0] , last_addr);
    return;
  }


  for (uint8_t i = len; i > 1; --i) {
    TM1368Control::send_to_address(bcd_array[digit_array[i-1]], (last_addr - (2*len - 2)) + 2*(len - i));
  }

  TM1368Control::send_to_address(0b10000000 | bcd_array[digit_array[0]] , last_addr);
}

void TM1368Control::send_char(char aVal) {
  TM1368Control::send_hex(aVal - '0');
}

void TM1368Control::send_string(char* aVal) {
  uint8_t j = 0;
  uint8_t decimal_flag = 0;
  uint8_t decimal_increment = 1;
  for (char* i = aVal; *i > '\0'; ++i) {
    if (*i != '.')
      digit_array[j] = *i - '0';
    else {
      digit_array[j] = *i;
      decimal_increment = 0;
    }
    ++j;
  }
  
  j += decimal_increment;

  for (uint8_t i = 0; i < j - decimal_increment; ++i) {
    if (digit_array[i] != 0x2E ) {
      TM1368Control::send_to_address(bcd_array[digit_array[i]], (0x0E - (2*(j - 1) -2)) + 2*i - decimal_flag);
      //decimal_flag = 0;
    }
    else {
      TM1368Control::send_to_address(0b10000000 | bcd_array[digit_array[i-1]], (0x0E - (2*j-2)) + 2*i);
      decimal_flag = 2;
    }
  }

}
