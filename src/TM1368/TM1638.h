#ifndef _TM1368_H_
#define _TM1368_H_

class TM1368Control {
  private:
    uint8_t CLK_PIN;
    uint8_t DIO_PIN;
    uint8_t STB_PIN;
    uint8_t last_addr = 0x00;
    uint8_t digit_array[8];
    void send_data(uint8_t data);
    void send_command(uint8_t command);
    void send_to_address(uint8_t data, uint8_t address);
    uint8_t convert_numeral(uint8_t numeral, uint32_t aVal);
    uint8_t read();
  public:
    void chip_init(uint8_t clk, uint8_t dio, uint8_t stb, uint8_t brighnest);
    void clear_reg();
    void send_int(uint32_t aVal);
    void send_hex(uint32_t aVal);
    void send_double(float aVal);
    void send_char(char aVal);
    void send_string(char* aVal);
    void set_led(uint8_t ledVal, uint8_t address);
    uint8_t read_button();
};

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
  0b01101111   //9
};

const uint8_t hex_array[] = {
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
  0b01110001   //F
};

#endif