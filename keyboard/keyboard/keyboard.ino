#include <TimerOne.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

int Button_state;
bool Keyboard_state = false;
int Mass[3][4] = {{9,6,3,100},{7,4,1,66},{8,5,2,0} };
int portState[3] = {6,5,3};
int pinState[4] = {7,11,13,14};
byte outPortMask(byte num_of_bits){ //для настройки DDR на выход
  byte mask = 0;
  mask = (0xFF<<num_of_bits);
  return  mask ^= 0xFF;
};
  
void Timer_int(){
  int PIN_st;
for(auto i=0; i<3; i++)
    { 
       PORTB=portState[i];
          PIN_st=PINC&0b00001111;
       for(auto j=0; j<4; j++)
       {

          if(  PIN_st == pinState[j])
          {
                   // Serial.println(PIN_st);
            Keyboard_state = true;
             Button_state = Mass[i][j];                                 
          }
       }                          
    }  

};

void setup() {
  // put your setup code here, to run once:
DDRB = 0b00001111;
PORTB = 0b00000000;
DDRC = 0b00000000;
Serial.begin(9600);
Timer1.initialize(10000);
Timer1.attachInterrupt(Timer_int);
}

void loop() {
  if (Keyboard_state){
  Serial.println(Button_state);
    delay(100);
     Keyboard_state = false;
     };
}
