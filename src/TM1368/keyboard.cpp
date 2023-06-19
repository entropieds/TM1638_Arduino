#include <Arduino.h>
#include "keyboard.h"

uint8_t rows_array[] = {ROW1, ROW2, ROW3, ROW4};
uint8_t col_array[] = {COL1, COL2, COL3, COL4};

char key_map [4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void keypadMatrix::timer_init() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 8000;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11);
  TIMSK1 |= (1 << OCIE1A);
}

void keypadMatrix::keypad_init() { 
  for (uint8_t rows = 0; rows < 4; ++rows){
    pinMode(rows_array[rows], INPUT_PULLUP);
  }
  
  for (uint8_t col = 0; col < 4; ++col) {
    pinMode(col_array[col], OUTPUT);
    digitalWrite(col_array[col], HIGH);
  }
}

char keypadMatrix::get_key() {
  for (uint8_t col = 0; col < 4; ++col) {
    digitalWrite(col_array[col], LOW);
    for (uint8_t row = 0; row < 4; ++row) {
      //delay(10);
      keypadMatrix::debounce(rows_array[row]);
      if (!digitalRead(rows_array[row])) {
        while (!digitalRead(rows_array[row])){}
        digitalWrite(col_array[col], HIGH);
        return key_map [row][col];
      }
    }
    digitalWrite(col_array[col], HIGH);
  }
  return '\0';
}

void keypadMatrix::debounce(uint8_t pin) {
  uint8_t current_state = digitalRead(pin);
  uint16_t state_count = 1800;
  while(state_count != 0){
    uint8_t new_state = digitalRead(pin);
    if (current_state == new_state){
      --state_count;
    } else {
      ++state_count;
    }
    current_state = new_state;
  }
}
