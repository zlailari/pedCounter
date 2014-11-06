
#include <Time.h>  

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the pot
int count = 0;
int timeStart = 0;
int timeEnd = 0;
boolean up = false;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  setTime(0);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);            

  // print the results to the serial monitor:
  Serial.print("sensor = " );                       
  Serial.print(sensorValue);
  Serial.print(", ");
  
  if(sensorValue>41) {
    Serial.println("");
    if(timeStart > 0) {
      timeEnd = now(); 
      int diff = timeStart - timeEnd;
      Serial.println("Diff is "+diff);
      timeStart = 0;
      timeEnd = 0; 
    }
    else {
      timeStart = now();
    }
    up = true;
    count = count + 1;
  }
  if(sensorValue<=38 && up==true) {
    up = false;  
  }
  
  Serial.print("count=");
  Serial.println(count); 
  

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);                     
}