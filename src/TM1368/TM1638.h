#ifndef _TM1368_H_
#define _TM1368_H_

class TM1368Contro {
  private:
    uint8_t CLK_PIN;
    uint8_t DIO_PIN;
    uint8_t STB_PIN;
    uint8_t digit_array[8];
    void send_data(uint8_t data);
    void send_command(uint8_t command);
    void send_to_address(uint8_t data, uint8_t address);
  public:
    void chip_init(uint8_t clk, uint8_t dio, uint8_t stb);
    void clear_reg();
    void send_int(uint32_t aVal);
};


#endif