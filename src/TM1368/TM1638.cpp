#include <Arduino.h>
#include "TM1638.h"

/******************************************************************/
/*Private: No                                                     */
/*Return value: NON                                               */
/*Input: -clock pin                                               */
/*       -data input output pin                                   */
/*       -chip selection                                          */
/*                                                                */
/******************************************************************/

void TM1368Control::chip_init(uint8_t clk, uint8_t dio, uint8_t stb, uint8_t brighnest) {
  this->CLK_PIN = clk;                              // set clock pin
  this->DIO_PIN = dio;                              // set data out/input pin
  this->STB_PIN = stb;                              // chip selection pin
  pinMode(clk, OUTPUT);                             // conf clk pin  
  pinMode(dio, OUTPUT);                             // conf DIO pin
  pinMode(stb, OUTPUT);                             // conf STB pin
  digitalWrite(clk, HIGH);                          // set clock pin to high
  digitalWrite(stb, HIGH);                          // set STB pin to high
  TM1368Control::clear_reg();                       // call method to clear address register
  TM1368Control::send_command(0x80 | brighnest);    // set the brightnest of display
}

/******************************************************************/
/*Private: Yes                                                    */
/*Return value: NON                                               */
/*Input: -data  uint8_t                                           */
/*                                                                */
/*                                                                */
/*description:                                                    */
/*  Send serial data byte by byte.                                */
/******************************************************************/

void TM1368Control::send_data(uint8_t data) {
  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(CLK_PIN, LOW);                       // Set falling edge 
    digitalWrite(DIO_PIN, data & 0x01);               // send one byte
    data >>= 1;                                       // Shift data
    digitalWrite(CLK_PIN, HIGH);                      // Set rissing edge 
  }
}



/************************************************************************************************/
/*Private: Yes                                                                                  */
/*Return value: NON                                                                             */
/*Input: -command  uint8_t                                                                      */
/*                                                                                              */
/*                                                                                              */
/*description:                                                                                  */
/*  Set down STB to init serial interface and send command to set modes.                        */
/************************************************************************************************/

void TM1368Control::send_command(uint8_t command) {
  digitalWrite(STB_PIN, LOW);
  TM1368Control::send_data(command);
  digitalWrite(STB_PIN, HIGH);
}

/******************************************************************/
/*Private: Yes                                                    */
/*Return value: NON                                               */
/*Input: -data                                                    */
/*       -address                                                 */
/*                                                                */
/*description:                                                    */
/*  send data to specific display addres with fixed address mode  */
/******************************************************************/

void TM1368Control::send_to_address(uint8_t data, uint8_t address) {
  TM1368Control::send_command(0x44); // Set fixed address mode
  digitalWrite(STB_PIN, LOW);
  TM1368Control::send_data(0xC0 | address); // Set display addres
  TM1368Control::send_data(data);
  digitalWrite(STB_PIN, HIGH);
  TM1368Control::increment_addr(last_addr); // Call method to inceremnt address
}

/******************************************************************/
/*Private: Yes                                                    */
/*Return value: uint8_t lenght of data                            */
/*Input: -aVal                                                    */
/*       -numeral                                                 */
/*                                                                */
/*description:                                                    */
/*  Method to convert number to invidual digit.                   */
/*Numeral value convers number to specifict numeral system        */
/******************************************************************/

uint8_t TM1368Control::convert_numeral(uint8_t numeral, uint32_t aVal) {
  uint8_t len = 0;     // init lenght of digit
  uint32_t num = aVal; 
// Get lenght of number
  while (num != 0) {
    ++len;
    num /= numeral;
  }
// Save invidual digits to array
  for (uint8_t i = 0; i < len; ++i) {
    digit_array[i] = aVal % numeral;
    aVal /= numeral;
  }
  return len;
}

/******************************************************************/
/*Private: Yes                                                    */
/*Return value: -uint8_t                                          */
/*Input: NON                                                      */
/*                                                                */
/*                                                                */
/*description:                                                    */
/*  Read serial data from buttons.                                */
/******************************************************************/

uint8_t TM1368Control::read() {
  uint8_t tmp = 0x00;    
  pinMode(DIO_PIN,INPUT);
  // Read serial data from buttons.
  for (uint8_t i = 0; i < 8; ++i) {
    tmp >>= 1;
    digitalWrite(CLK_PIN, LOW);
    if (digitalRead(DIO_PIN)) {
      tmp |= 0x80;
    }
    digitalWrite(CLK_PIN, HIGH);
  }
  pinMode(DIO_PIN,OUTPUT);
  digitalWrite(DIO_PIN, LOW);
  return tmp;
}

/******************************************************************/
/*Private: Yes                                                    */
/*Return value: NON                                               */
/*Input: NON                                                      */
/*                                                                */
/*                                                                */
/*description:                                                    */
/*  Method to clear address register.                             */
/******************************************************************/

void TM1368Control::clear_reg() {
  TM1368Control::send_command(0x40); // Set autoincrement address mode
  digitalWrite(STB_PIN, LOW);
  TM1368Control::send_data(0xC0);   // Set begin of adrress register
  for(uint8_t i = 0; i < 16; ++i)
    TM1368Control::send_data(0x00); // Clear all address
  digitalWrite(STB_PIN, HIGH);
  last_addr = 0x00;
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: uint32_t number, bool dot (default = false)                                        */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Send unsignet integer to display, if is dot true then set on last digit decimal dot.    */
/********************************************************************************************/

void TM1368Control::send_int(uint32_t aVal, bool dot) {
  uint8_t len = 0;
  // If value is 0 than is directli set to diplay
  if (aVal == 0){
    TM1368Control::send_to_address(symbol_array[0], last_addr);
    return;
  }
  // get lenght of number and store digits to array in decimal system
  len = TM1368Control::convert_numeral(10, aVal);
  // go throught array and send invidial digits
  for (uint8_t i = len; i > 1; --i) {
    TM1368Control::send_to_address(symbol_array[digit_array[i-1]], last_addr);
  }
  // On last digits send number and deciaml dot if is set dot to true
  TM1368Control::send_to_address(symbol_array[digit_array[0]] | (dot ? 0b10000000 : 0), last_addr);
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: uint32_t number                                                                    */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Send unsignet integer to display in hexadecimal.                                        */
/********************************************************************************************/

void TM1368Control::send_hex(uint32_t aVal) {
  uint8_t len = 0;
// if is number = 0 send directli to diplay
  if (aVal == 0){
    TM1368Control::send_to_address(symbol_array[0], last_addr );
    return;
  }
// get lenght of number in hexadecimal
  len = TM1368Control::convert_numeral(16, aVal);
// go throught array and send to display
  for (uint8_t i = len; i > 0; --i) {
    TM1368Control::send_to_address(symbol_array[digit_array[i-1]], last_addr);
  }
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: float number                                                                       */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Send unsignet float to display in decimal.                                              */
/********************************************************************************************/

void TM1368Control::send_double(float aVal) {
// Seperate float to int and decimal part 
  uint32_t int_part = uint32_t(aVal);
  uint32_t decimal = (aVal - (uint32_t)aVal) * 100; // Set float on two decimal

// if integer part is 0 than is directli sent to diplay with deciaml dot
  if (int_part == 0) {
    TM1368Control::send_to_address(0b10000000 | symbol_array[0] , last_addr);
  }

  TM1368Control::send_int(int_part, true);  // Send integer part with decimal dots
  TM1368Control::send_int(decimal);         // Send decimal part
  
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: String                                                                             */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Send String to diplay.                                                                  */
/********************************************************************************************/

void TM1368Control::send_string(char* aVal) {
  uint8_t len = 0;              // Support variable to get lenght of string
  uint8_t decimal_flag = 0;   // Flag to indicate decimal dot.
  uint8_t decimal_increment = 1;  // Variable to compensate lenght of array without decimal dot
// Get lenght of string and store value.
  for (char* i = aVal; *i > '\0'; ++i) {
    if (*i != '.') // Check if there is decimal dot
      digit_array[len] = *i - '0';
    else {
      digit_array[len] = 0x2E; // Stroe value of decimal dot
      decimal_increment = 0;  
    }
    ++len; 
  }
  
  len += decimal_increment; // compensate lenght of array without deciaml dot

  for (uint8_t i = 0; i < len - decimal_increment; ++i) {
    // Check if there is decimal dot
    if (digit_array[i] != 0x2E ) {
      TM1368Control::send_to_address(symbol_array[digit_array[i]], last_addr - decimal_flag);
    }
    else {
      decimal_flag += 2; // Set decimal flag to edit addres
      TM1368Control::send_to_address(0b10000000 | symbol_array[digit_array[i-1]], last_addr - decimal_flag);
    }
  }
  last_addr -= decimal_flag; // Set address to right value
  
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: uint32_t, decimal dot (default = flase), start address                             */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Send int to display with start address.                                                 */
/********************************************************************************************/

void TM1368Control::send_int_to_addr(uint32_t aVal, uint8_t addr ,bool dot) {
 TM1368Control::set_addr(addr);
 TM1368Control::send_int(aVal, dot);
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: uint32_t, start address                                                            */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Send int in hexadecimal to display with start address.                                  */
/********************************************************************************************/

void TM1368Control::send_hex_to_addr(uint32_t aVal, uint8_t addr) {
 TM1368Control::set_addr(addr);
 TM1368Control::send_hex(aVal);
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: uint32_t, start address                                                            */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Send int in hexadecimal to display with start address.                                  */
/********************************************************************************************/

void TM1368Control::send_double_to_addr(float aVal, uint8_t addr) {
 TM1368Control::set_addr(addr);
 TM1368Control::send_double(aVal);
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: char, start address                                                                */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Send char to display with start address.                                                */
/********************************************************************************************/

void TM1368Control::send_char_to_addr(char aVal, uint8_t addr) {
 TM1368Control::set_addr(addr);
 TM1368Control::print_symbol(aVal);
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: string, start address                                                              */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Send string to display with start address.                                              */
/********************************************************************************************/

void TM1368Control::send_string_to_addr(char* aVal, uint8_t addr) {
 TM1368Control::set_addr(addr);
 TM1368Control::send_string(aVal);
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: NON                                                                                */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Delete last symbol on display.                                                          */
/********************************************************************************************/

void TM1368Control::delete_symbol() {
  TM1368Control::decrement_addr(last_addr); // Decrement address
  TM1368Control::send_to_address(0x00, last_addr); // Clear register
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: uint8_t, start address                                                             */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  On/Off leds on board with address.                                                      */
/********************************************************************************************/

void TM1368Control::set_led(uint8_t ledVal, uint8_t address) {
  TM1368Control::send_command(0x44);
  digitalWrite(STB_PIN, LOW);
  TM1368Control::send_data(0xC1 | address); // Set address of led
  TM1368Control::send_data(ledVal);
  TM1368Control::increment_addr(last_addr_led);
  digitalWrite(STB_PIN, HIGH);
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: uint8_t                                                                     */
/*Input:                                                                                    */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Get value of button.                                                                    */
/********************************************************************************************/

uint8_t TM1368Control::read_button() {
  uint8_t button = 0x00;
  digitalWrite(STB_PIN, LOW);
  TM1368Control::send_data(0x42);
  for (uint8_t i = 0; i < 4; ++i) {
    button |= TM1368Control::read() << i;
  }
  digitalWrite(STB_PIN, HIGH);
  return button;
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: char                                                                               */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Send symbol to display.                                                                 */
/********************************************************************************************/

void TM1368Control::print_symbol(char symbol) {
  switch (symbol){
    case ('A') :
      TM1368Control::send_to_address(symbol_array[10], last_addr);
      break;    
    case ('B') :
      TM1368Control::send_to_address(symbol_array[11], last_addr);  
      break;
    case ('C') :
      TM1368Control::send_to_address(symbol_array[12], last_addr);
      break;      
    case ('D') :
      TM1368Control::send_to_address(symbol_array[13], last_addr);
      break;     
    case ('E') :
      TM1368Control::send_to_address(symbol_array[14], last_addr);
      break;     
    case ('F') :
      TM1368Control::send_to_address(symbol_array[15], last_addr);
      break;     
    case ('P') :
      TM1368Control::send_to_address(symbol_array[16], last_addr);
      break;     
    case ('U') :
      TM1368Control::send_to_address(symbol_array[17], last_addr);
      break;      
    case ('L') :
      TM1368Control::send_to_address(symbol_array[18], last_addr);
      break;     
    case ('H') :
      TM1368Control::send_to_address(symbol_array[19], last_addr);
      break;     
    case ('J') :
      TM1368Control::send_to_address(symbol_array[20], last_addr);
      break;      
    case ('Q') :
      TM1368Control::send_to_address(symbol_array[21], last_addr);
      break;
    default:
      TM1368Control::send_to_address(symbol_array[symbol - '0'], last_addr);
      break;     
  }
}

/********************************************************************************************/
/*Private: No                                                                               */
/*Return value: NON                                                                         */
/*Input: string                                                                             */
/*                                                                                          */
/*                                                                                          */
/*description:                                                                              */
/*  Send string wich contain special symbols.                                               */
/********************************************************************************************/

void TM1368Control::send_symbol_string(char* aVal) {
  for (char* i = aVal; *i > '\0'; ++i) {
    TM1368Control::print_symbol(*i);
  }
}

void TM1368Control::send_symbol_string_to_addr(char* aVal, uint8_t addr) {
 TM1368Control::set_addr(addr);
 TM1368Control::send_symbol_string(aVal);
}