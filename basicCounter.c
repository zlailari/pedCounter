/**
 * Written by Ze'ev Lailari 11/6/14
 *
 * Code to load on to Arduino to capture # and speed of bikes 
 * on a path. The analog input is attached to a pressure sensor
 * 
 * Can be easily modified to capture # of pedestrians/bikes if 
 * you don't care about finding the speed of bikes
 *
 * Used some code from:
 * https://github.com/VernierSoftwareTechnology/arduino/blob/master/VernierAnalogStore/VernierAnalogStore.ino
 * for EEPROM guide
 **/
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int buttonPin = 5;

// Button variables
int buttonVal = 0; // value read from button
int buttonLast = 1; // buffered value of the button's previous state
long btnDnTime; // time the button was pressed down
long btnUpTime; // time the button was released
boolean ignoreUp = false; // whether to ignore the button release because the click+hold was triggered


int sensorValue = 0;        // Value read from the pot
int count = 0;              // Total # objects that have passed
int interval = 0;           // Interval between successive peaks (for bike speed)
int baseline = 0;           // Baseline pressure reading (changes with temp/altitude
int runsForBase = 100;      // Take the average of the first x readings for baseline
int currentIter = 0;
boolean up = false;         // For peak finding
int addr = 0;               // For local storage

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  // Set button input pin
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(analogInPin, INPUT);
  Serial.begin(9600);
  Serial.println("VernierAnalogStore sketch");
  Serial.println("Press and hold to collect new data");
  Serial.println("Press button briefly to read data stored in EEPROM");
  Serial.println("and send it to the Serial Monitor");
  Serial.println(" "); 
}

void loop() {

   // Set baseline based on first [runsForBase] readings
  while(!(currentIter > runsForBase)) {
    baseline = ((baseline*(currentIter-1))+ sensorValue)/currentIter;
    currentIter++;
  }

    // Read the state of the button
  buttonVal = digitalRead(buttonPin);// button down is low
  // Test for button pressed and store the down time
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - btnUpTime) > long(debounce))
  {
    //button pressed
    btnDnTime = millis();// note time of press
  }
  // Test for button release and store the up time
  if (buttonVal == HIGH && buttonLast == LOW && (millis() - btnDnTime) > long(debounce))
  {
    if (ignoreUp == false) ReadEEPROMData();// read data from EEPROM and send to Serial Monitor
    else ignoreUp = false;
      btnUpTime = millis();// note time of button release
  }
  // Test for button held down for longer than the hold time
  if (buttonVal == LOW && (millis() - btnDnTime) > long(holdTime))
  {
    CollectData(); //collect new data
    ignoreUp = true;
    btnDnTime = millis(); // get new button down time
  }
  buttonLast = buttonVal;    
}

void CollectData() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin); 

  //smooth sensorReading by averaging 3 readings
  sensorValue = analogRead(analogInPin);
  xa = sensorValue;
  delay (20);
  interval += 20;
  sensorValue = analogRead(analogInPin);     
  xb = sensorValue;
  delay (20);
  interval += 20
  sensorValue = analogRead(analogInPin);      
  xc = sensorValue;
  sensorValue = (xa +xb + xc)/3;           
  
  //Checks if pressure peak, prints time since last peak
  if(sensorValue>=(baseline+2) && up==false) {
    up = true;
    count = count + 1;
    Serial.println("speed = ");
    Serial.print(interval/1000);
    EEPROM.write(addr, lowByte(interval));
    EEPROM.write(addr + 1, highByte(interval));
    addr += 2;
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

//=================================================
// Events to trigger by short click of button
void ReadEEPROMData()// //Send data to Serial Monitor
{
  int NumberOfPointsRead;// local variable
  Serial.println("Send Data to Serial Monitor");
  Serial.println(" ");
  int Sample = 0;// local variables
  int i = 0;
  int addr = base+ 2; //starting address of memory for data storage
  //note addr is later-incremented
  NumberOfPointsRead = EEPROM.read(base) + EEPROM.read(base+ 1)*256;
  Serial.println( "Counts");
  Serial.print(NumberOfPointsRead);
  Serial.print(" readings taken with a Vernier ");
  Serial.println(SensorName);
  Serial.println("Data Set From Arduino");
  Serial.print("Time");
  Serial.print("\t"); // print a tab character
  Serial.println(Measurement);
  Serial.print("T");
  Serial.print("\t"); // print a tab character
  Serial.println(ShortMeasurement);
  Serial.print("s");
  Serial.print("\t"); // print a tab character
  Serial.println(Units);
  for (i = 0; i <(NumberOfPointsRead)*2; i+=2)
  {
    Count = EEPROM.read(base+2 + i) | EEPROM.read(base+2 + i + 1) << 8;
    //the print below does the division first to avoid overflows
    Serial.print(i/2/1000.0*TimeBetweenReadings);
    Serial.print("\t"); // print a tab character
    SensorReading= Intercept + Voltage * Slope;
    Serial.println(SensorReading);
  }
  Serial.println(" ");
  Serial.println("Press and hold button to collect new data ");
  Serial.println(" ");
  //end of send data to Serial Monitor
}