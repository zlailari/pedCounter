/**
 * Written by Ze'ev Lailari 11/6/14
 *
 * Code to load on to Arduino to capture # and speed of bikes 
 * on a path. The analog input is attached to a pressure sensor
 * 
 * Can be easily modified to capture # of pedestrians/bikes if 
 * you don't care about finding the speed of bikes
 *
 **/
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // Value read from the pot
int count = 0;              // Total # objects that have passed
int interval = 0;           // Interval between successive peaks (for bike speed)
int baseline = 0;           // Baseline pressure reading (changes with temp/altitude
int runsForBase = 100;      // Take the average of the first x readings for baseline
int currentIter = 0;
boolean up = false;         // For peak finding

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);            
  
  // Set baseline based on first [runsForBase] readings
  if(!(currentIter > runsForBase)) {
    baseline = ((baseline*(currentIter-1))+ sensorValue)/currentIter;
    currentIter++;
  }
  
  //Checks if pressure peak, prints time since last peak
  if(sensorValue>=(baseline+2) && up==false) {
    up = true;
    count = count + 1;
    Serial.println("speed = ");
    Serial.print(interval/1000);
    Serial.print(" m/s");
    // Delay 100ms so that it won't give multiple readings
    delay(100);
    interval = 100;
  } 
  // resets detect variable if peak has passed
  if(sensorValue<=(baseline+1) && up==true) {
    up = false;  
  }
  interval++;

  // wait 1 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(1);                     
}