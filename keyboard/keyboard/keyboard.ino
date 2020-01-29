#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "timer_class.h"

#define COLUMNS 3 //"write" to matrix
#define LINES 4 //"read" from matrix

#define COLUMNS_PORT PORTB //"write" to matrix
#define LINES_PORT_STATE PINC //"read" from matrix

#define DEBOUNCE 15000

uint8_t pressed_key;
bool something_pressed = false;

uint8_t key_matrix[LINES][COLUMNS] = {
  {7, 8, 9},
  {4, 5, 6},
  {1, 2, 3},
  {66, 0, 100}  //my keyboard does not have 66 button in matrix
};

uint8_t column_state[COLUMNS] = {
  0b011,
  0b101,
  0b110
};

uint8_t line_state[LINES] = {
  0b0111,
  0b1011,
  0b1101,
  0b1110
};

Timer timer1;

uint8_t PortMask(uint8_t num_of_bits) { //для настройки DDR
  uint8_t mask = 0;
  mask = (0xFF << num_of_bits);
  return  mask ^= 0xFF;
};

void ReadMatrix() {
  uint8_t cur_line_state;
  for (auto cur_column = 0; cur_column < COLUMNS; cur_column++)
  {
    COLUMNS_PORT = column_state[cur_column];
    cur_line_state = LINES_PORT_STATE & 0b00001111;
    for (auto cur_line = 0; cur_line < LINES; cur_line++)
    {
      if (  cur_line_state == line_state[cur_line])
      {
        something_pressed = true;
        pressed_key = key_matrix[cur_line][cur_column];
      }
    }
  }
};

void setup() {
  DDRB = 0b00001111;  //output port
  COLUMNS_PORT = 0;
  LINES_PORT_STATE = 0;   //input port
  Serial.begin(9600);
  timer1.attachTimerInterrupt(ReadMatrix, DEBOUNCE);  //start reading keyboard with period = Debounce
}

void loop() {
  if (something_pressed) {
    Serial.println(pressed_key);
    delay(100);
    something_pressed = false;
  };
}
