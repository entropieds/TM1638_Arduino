#ifndef _TM1368_H_
#define _TM1368_H_

class TM1368Control {
  private:
    uint8_t CLK_PIN;
    uint8_t DIO_PIN;
    uint8_t STB_PIN;
    uint8_t last_addr = 0x00; // variable to keep last display address
    uint8_t last_addr_led = 0x00; // variable to keep last led address
    uint8_t digit_array[8]; // Array to storage invidual digits of numbers
    void send_data(uint8_t data);
    void send_command(uint8_t command);
    void send_to_address(uint8_t data, uint8_t address);
    uint8_t convert_numeral(uint8_t numeral, uint32_t aVal);
    uint8_t read();
    void inline increment_addr(uint8_t &addr) {addr += 0x02; } // Private methodt to increment address
    void inline decrement_addr(uint8_t &addr) {addr -= 0x02; } // Private methodt to dercrement address
    void inline set_addr(uint8_t addr) {last_addr = addr;}  // Private methodt to set address
  public:
    void chip_init(uint8_t clk, uint8_t dio, uint8_t stb, uint8_t brighnest);
    void clear_reg();
    void send_int(uint32_t aVal, bool dot = false);
    void send_hex(uint32_t aVal);
    void send_double(float aVal);
    void send_string(char* aVal);
    void send_int_to_addr(uint32_t aVal, uint8_t addr ,bool dot = false);
    void send_hex_to_addr(uint32_t aVal, uint8_t addr);
    void send_double_to_addr(float aVal, uint8_t addr);
    void send_char_to_addr(char aVal, uint8_t addr);
    void send_string_to_addr(char* aVal, uint8_t addr);
    void delete_symbol();
    void set_led(uint8_t ledVal, uint8_t address);
    void print_symbol(char symbol);
    void send_symbol_string(char* aVal);
    uint8_t read_button();
};
// Array contains data for 7-segment disp patterns
const uint8_t symbol_array[] = {
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
  0b01101111,  //9
  0b01110111,  //A
  0b01111100,  //B
  0b00111001,  //C
  0b01011110,  //D
  0b01111001,  //E
  0b01110001,  //F
  0b01110011,  //P
  0b00111110,  //U
  0b00111000,  //L
  0b01110110,  //H
  0b00011110,  //J
  0b01100111,  //Q
};

#endif