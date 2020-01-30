#include "key_matrix.h"

Timer keyboard_timer; //uses timer2
uint8_t pressed_key;
bool something_pressed = false;
uint8_t column_state[COLUMNS], line_state[LINES]; //states of ports
uint8_t key_matrix[LINES][COLUMNS]; //the copy of array from main

void SetupKeyboard(uint8_t columns, uint8_t lines, uint8_t new_matrix[LINES][COLUMNS]) {
  ArraySetup(columns, column_state);
  ArraySetup(lines, line_state);
  COLUMNS_DDR = PortMask(columns);  //output port
  COLUMNS_PORT = 0;
  LINES_PORT_STATE = 0;   //input port
  for (uint8_t x = 0; x < LINES; x++) { //copy array from main
    for (uint8_t y = 0; y < COLUMNS; y++) {
      key_matrix[x][y] = new_matrix[x][y];
    }
  }
  keyboard_timer.attachTimerInterrupt(ReadMatrix, DEBOUNCE);  //start reading keyboard with period = Debounce
}

uint8_t PressedKey() {
  return pressed_key;
}

bool WasPressed() {
  return something_pressed;
}

void WasPressed(bool new_status) {
  something_pressed = new_status;
}

/*Service functions*/
uint8_t PortMask(uint8_t num_of_bits) { //makes mask like 0b00001111
  return (0xFF << num_of_bits) ^ 0xFF;
}

void ArraySetup(uint8_t num_of_bits, uint8_t *state_array) { //makes states like 0b011, 0b101, 0b110
  uint8_t mask = PortMask(num_of_bits);
  for (auto i = 0; i < num_of_bits; i++) {
    state_array[i] = mask ^ (1 << (num_of_bits - 1 - i));
  };
};

void ReadMatrix() {
  uint8_t cur_line_state;
  for (auto cur_column = 0; cur_column < COLUMNS; cur_column++)
  {
    COLUMNS_PORT = column_state[cur_column];
    cur_line_state = LINES_PORT_STATE & PortMask(LINES);
    for (auto cur_line = 0; cur_line < LINES; cur_line++)
    {
      if ( cur_line_state == line_state[cur_line])
      {
        something_pressed = true;
        pressed_key = key_matrix[cur_line][cur_column];
      }
    }
  }
}
