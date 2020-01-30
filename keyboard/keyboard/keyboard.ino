#include "key_matrix.h"

uint8_t my_matrix[LINES][COLUMNS] = { //my keyboard
  {7, 8, 9},
  {4, 5, 6},
  {1, 2, 3},
  {66, 0, 100}  //my keyboard does not have 66 button in matrix
};

void setup() {
  Serial.begin(9600);
  SetupKeyboard(COLUMNS, LINES, my_matrix); //COLUMNS and LINES are set in .h file
}

void loop() {
  if (WasPressed()) { //if any key was pressed
    Serial.println(PressedKey()); //print it`s code from array my_matrix
    delay(100);
    WasPressed(false); //to stop continious printing
  }
}
