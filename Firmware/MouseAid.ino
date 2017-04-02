//
// Mouse Aid - HAD Prize 2017
//
// danjovic@hotmail.com
// April 2th 2017
//

#include <DigiMouse.h>


//     State Table
//  --------------+---------
//      OUTPUT    |  INPUT
//  ----+----+----+----+----
//   P0 | P1 | P2 | P0 | P2
//  ----+----+----+----+----
//    Z |  0 |  Z | !L | !R
//  ----+----+----+----+----
//    0 |  1 |  Z |  0 | !U
//  ----+----+----+----+----
//    Z |  1 |  0 | !D |  0
//  ----+----+----+----+----

#define MAX_FIB_INDEX  6 // maximum index for fibonacci table
static uint8_t fib_index = 0;
const uint8_t fib_step[MAX_FIB_INDEX]={ 1, 2, 3, 5, 8, 13 };
static uint8_t last_scan = 0;

void setup() {
  DigiMouse.begin(); //start or reenumerate USB - BREAKING CHANGE from old versions that didn't require this
}

void loop() {

  uint8_t button_state = 0; // 0 0 0 0 R L D U

  //-----------------------------------------------------------------------------
  // Scan Keyboard
  //-----------------------------------------------------------------------------
  // Set P1=0, P0,P2 = inputs with pullup
  pinMode(0, INPUT_PULLUP);   // P0
  pinMode(1, OUTPUT);         // P1
  digitalWrite(1, LOW);
  pinMode(2, INPUT_PULLUP);   // P2
  DigiMouse.delay(1);

  if (digitalRead(0) == 0) button_state |= (1 << 2); // Left
  if (digitalRead(2) == 0) button_state |= (1 << 3); // Right

  // Set P1=1, P0=0, P2 = input with pullup
  pinMode(0, OUTPUT);          // P0
  digitalWrite(1, HIGH);       // P1
  digitalWrite(0, LOW);
  // pinMode(2, INPUT_PULLUP); // P2 no change, still input
  DigiMouse.delay(1);

  if (digitalRead(2) == 0) button_state |= (1 << 0); // Up

  // Set P1=1, P2=0, P0 = input with pullup
  pinMode(0, INPUT_PULLUP);    // P0
  //  digitalWrite(1,HIGH);        // P1  no change from last state
  pinMode(2, OUTPUT);    // P2
  digitalWrite(2, LOW);
  DigiMouse.delay(1);

  if (digitalRead(0) == 0) button_state |= (1 << 1); // Down


  //-----------------------------------------------------------------------------
  // Update movements on change
  //-----------------------------------------------------------------------------
  if (button_state != last_scan) {
    last_scan = button_state;

    // Eliminate useless sequences: L+R
    if ( (button_state & 0x0c) ==0x0c ) {
       // Do L+R Stuff
       if ( (button_state & 1) && (fib_index < (MAX_FIB_INDEX-1)) ) fib_index++ ;  // Up
       if ( (button_state & 2) && (fib_index > 0                ) ) fib_index--;  // Down
         
      } else {
           if (button_state & 4) DigiMouse.moveX(-fib_step[fib_index]);  // Left
           if (button_state & 8) DigiMouse.moveX( fib_step[fib_index]);  // Right       
     }

    // Eliminate useless sequences: U+D
    if ( (button_state & 0x03) == 0x03 ) {
       // Do U+D Stuff
       fib_index = 0;  // U+R reset fib index
      } else {
        if (button_state & 1) DigiMouse.moveY(-fib_step[fib_index]);  // Up
        if (button_state & 2) DigiMouse.moveY( fib_step[fib_index]);  // Down  
     }

  }

  //-----------------------------------------------------------------------------
  // Debouncing
  //-----------------------------------------------------------------------------
  DigiMouse.delay(5);

}
