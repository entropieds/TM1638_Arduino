#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

// define columns pins
#define COL4 12
#define COL3 11
#define COL2 10
#define COL1 9
// define rows pins
#define ROW1 5
#define ROW2 6
#define ROW3 7
#define ROW4 8

static volatile bool flag = false;  

class keypadMatrix{
  public:
    void timer_init();
    void keypad_init();
    char get_key();
  private:
    void debounce(uint8_t pin);
};

#endif