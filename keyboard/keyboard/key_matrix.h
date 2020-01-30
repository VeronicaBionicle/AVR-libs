#ifndef key_matrix_h_
#define key_matrix_h_

#include "timer_class.h"

#define DEBOUNCE 15000  //Timer2 period (15 ms) to avoid bouncing

/*Set the number of columns and lines*/
#define COLUMNS 3 //"write" to matrix  
#define LINES 4 //"read" from matrix

/*Set the ports for connecting keypad*/
#define COLUMNS_PORT PORTB //"write" to matrix
#define COLUMNS_DDR DDRB //"write" to matrix
#define LINES_PORT_STATE PINC //"read" from matrix

void SetupKeyboard(uint8_t columns, uint8_t lines, uint8_t new_matrix[LINES][COLUMNS]); //setup the key matrix, new_matrix contains "names" of keys
uint8_t PressedKey();   //returns the pressed key from array new_matrix
bool WasPressed(); //returns true, if any key was pressed
void WasPressed(bool new_status);   //set new status of flag something_pressed

uint8_t PortMask(uint8_t num_of_bits);  //service function to make the mask like 0b00001111
void ArraySetup(uint8_t num_of_bits, uint8_t *state_array); //service functions to setup arrays with states of ports
void ReadMatrix();  //service function for reading the states of key matrix
#endif /*key_matrix_h_*/
