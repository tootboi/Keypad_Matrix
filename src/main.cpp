#include <Arduino.h>

const int debounce = 10;
unsigned long prevPressTime;

int pressCount;
int lastPressCount;

const int rowNum = 3;
const int colNum = 3;

byte rowPins[rowNum] = {5, 6, 7};
byte colPins[colNum] = {8, 9, 10};

//    btn matrix layout:
//    |----------------|
//    |[1]    [2]   [3]|
//    |[4]    [5]   [6]|
//    |[7]    [8]   [9]|
//    |----------------|

int getKey();

void setup() {
  for(byte r=0; r<rowNum; r++) {
    pinMode(rowPins[r], INPUT_PULLUP);
  }
}

void loop() {
  int key = getKey();
  if(key) {
    Serial.print("         loop key: ");
    Serial.println(key);
  }
}

int getKey() {
  int currKey=0;
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
            currKey = c+colNum*r+1;

            Serial.println("========================");
            Serial.print("row: ");
            Serial.print(r);
            Serial.print(" col: ");
            Serial.print(c);
            Serial.print(" key: ");
            Serial.println(currKey);
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
  // Serial.print(" currkey: ");
  // Serial.println(currKey);
  return(currKey);
}