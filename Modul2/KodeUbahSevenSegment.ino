#include <Arduino.h>
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4}; 

byte digitPattern[16][8] = {
  {0,0,0,0,0,0,1,1}, // 0
  {1,0,0,1,1,1,1,1}, // 1
  {0,0,1,0,0,1,0,1}, // 2
  {0,0,0,0,1,1,0,1}, // 3
  {1,0,0,1,1,0,0,1}, // 4
  {0,1,0,0,1,0,0,1}, // 5
  {0,1,0,0,0,0,0,1}, // 6
  {0,0,0,1,1,1,1,1}, // 7
  {0,0,0,0,0,0,0,1}, // 8
  {0,0,0,0,1,0,0,1}, // 9
  {0,0,0,1,0,0,0,1}, // A
  {1,1,0,0,0,0,0,1}, // b
  {0,1,1,0,0,0,1,1}, // C
  {1,0,0,0,0,1,0,1}, // d
  {0,1,1,0,0,0,0,1}, // E
  {0,1,1,1,0,0,0,1}  // F
};

void displayDigit(int num) {
  for(int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], digitPattern[num][i]);
  }
}

void setup() {
  for(int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], HIGH);
  }
}

void loop() {
  for(int i = 15; i >= 0; i--) {
    displayDigit(i); 
    delay(1000);     
  }
}
