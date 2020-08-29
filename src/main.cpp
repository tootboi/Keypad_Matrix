#include <Arduino.h>

const int debounce = 10;
unsigned long prevPressTime;

int pressCount;
int lastPressCount;

const int rowNum = 3;
const int colNum = 3;

byte rowPins[rowNum] = {5, 6, 7};
byte colPins[colNum] = {8, 9, 10};

void setup() {
  for(byte r=0; r<rowNum; r++) {
    pinMode(rowPins[r], INPUT_PULLUP);
  }
}

void loop() {
  pressCount = 0;
  if((millis() - prevPressTime) > debounce) {
    prevPressTime = millis();
    for(byte c=0; c<colNum; c++) {
      //pulse col.
      pinMode(colPins[c], OUTPUT);
      digitalWrite(colPins[c], LOW);
      for(byte r=0; r<rowNum; r++) {
        //check if btn is pressed.
        if(digitalRead(rowPins[r]) == 0) {
          pressCount++;
          //pressCount==1 limits concurrent btn press to 1.
          //lastPressCount==0 ensures that all btns were released.
          if(pressCount == 1 && lastPressCount == 0) {
            Serial.println("========================");
            Serial.print("row: ");
            Serial.print(r);
            Serial.print(" col: ");
            Serial.print(c);
            Serial.print(" key: ");
            Serial.println(c+colNum*r);
            break;
          }
        }
      }
      //end col pulse.
      digitalWrite(colPins[c], HIGH);
      pinMode(colPins[c], INPUT);
    }
    //save current pressCount.
    lastPressCount = pressCount;
  }
}