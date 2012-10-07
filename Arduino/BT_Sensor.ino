// Includes
#include <SoftwareSerial.h>
#include <DHT22.h>


// Pin constants
#define pinLed 4
#define pinRx 2
#define pinTx 3
#define snsLight A0
#define snsMoist A1
#define snsDHT22 6


// DHT22 init
DHT22 myDHT22(snsDHT22);
const float fltBadDHT22Reading = -273.15;
float fltTemp = fltBadDHT22Reading;
float fltHumidity = -1;
unsigned long dhtLastRead = 0;

SoftwareSerial mySerial(pinRx, pinTx); // RX, TX


void setup() {
  // Set the LED
  pinMode(pinLed, OUTPUT);
  
  // Initialize serial output
  Serial.begin(9600);
  mySerial.begin(9600);
  
  // Flash the LED several times for delayed boot
  while(millis()<2000) {
    digitalWrite(pinLed, HIGH);
    delay(25);
    digitalWrite(pinLed, LOW);
    delay(25);
  }
  
  bothSerialPrintln("{\"state\":\"ready\"}");
  
  getDHT22Data();
  dhtLastRead = millis();
  
  // One long LED flash to signify startup complete
  digitalWrite(pinLed, HIGH);
  delay(1000);
  digitalWrite(pinLed, LOW);
  delay(100);
}


void loop() {
  if(millis()-dhtLastRead>=2000 || dhtLastRead>millis()) {
    getDHT22Data();
    dhtLastRead = millis();
  }
  
  String command = getCommand();
  char c[100];
  
  if(command.length()>0) {
    String json = "{\"command\":\"";
    json += command;
    json += "\",\"value\":";
    
    if(command=="led") {
      blinkLed(1, 2000);
      json += "true,";
    } else if(command=="test") {
      blinkLed(5, 75);
      json += "true,";
    } else if(command=="ledon") {
      digitalWrite(pinLed, HIGH);
      json += "\"HIGH\",";
    } else if(command=="ledoff") {
      digitalWrite(pinLed, LOW);
      json += "\"LOW\",";
    } else if(command=="light") {
      dtostrf(getLight(), 4, 2, c);
      json += c;
      json += ",";
      json += "\"unit\":\"%\",";
    } else if(command=="lightraw") {
      dtostrf(getLightRaw(), 4, 0, c);
      json += c;
      json += ",";
      json += "\"unit\":\"raw\",";
    } else if(command=="moistraw") {
      dtostrf(getMoistRaw(), 4, 2, c);
      json += c;
      json += ",";
      json += "\"unit\":\"raw\",";
    } else if(command=="temp") {
      dtostrf(getTempF(), 3, 2, c);
      json += c;
      json += ",";
      json += "\"unit\":\"F\",";
    } else if(command=="tempc") {
      dtostrf(getTempC(), 3, 2, c);
      json += c;
      json += ",";
      json += "\"unit\":\"C\",";
    } else if(command=="humid") {
      dtostrf(getHumidity(), 3, 2, c);
      json += c;
      json += ",";
      json += "\"unit\":\"%\",";
    } else {
      json += "false,";
    }
    
    json += "\"millis\":";
    json += millis();
    json += "}";
    bothSerialPrintln(json);
  }
  
  command = "";
  
  delay(100);
}


String getCommand() {
  String s;
  bool source;
  
  while(mySerial.available()>0) {
    char c = mySerial.read();
    s += c;
    
    source = false;
  }
  
  while(Serial.available()>0) {
    char c = Serial.read();
    s += c;
    
    source = true;
  }
  
  s.trim();
  
  return s;
}


void bothSerialPrint(String s) {
  Serial.print(s);
  mySerial.print(s);
}


void bothSerialPrintln(String s) {
  Serial.println(s);
  mySerial.println(s);
}


// Get and map light reading
float getLight() {
  float pctLight;
  
  float intLight = analogRead(snsLight);
  
  if(intLight<=0)
    pctLight = 0;
  else
    pctLight = intLight/1024*100;
  
  return pctLight;
}

unsigned int getLightRaw() {
  return analogRead(snsLight);
}


unsigned int getMoistRaw() {
  return analogRead(snsMoist);
}


// Get temperature in Celsius
float getTempC() {
  return getTemp();
}


// Get temperature in Fahrenheight
float getTempF() {
  return (fltTemp*9/5)+32;
}


// Get the temperature (in Celsius)
float getTemp() {
 return fltTemp; 
}


// Get the humidity
float getHumidity() {
  return fltHumidity;
}


// Get data from DHT22
void getDHT22Data() {
  DHT22_ERROR_t errorCode;
  
  fltTemp = fltBadDHT22Reading;
  fltHumidity = -1;
  
  errorCode = myDHT22.readData();
  switch(errorCode) {
    case DHT_ERROR_NONE:
    case DHT_ERROR_CHECKSUM:
      fltTemp = myDHT22.getTemperatureC();
      fltHumidity = myDHT22.getHumidity();
      break;
  }
  
  fltTemp = fltTemp*1;
  fltHumidity = fltHumidity*1;
  
  dhtLastRead = millis();
}


// Blink LED t times for d duration
void blinkLed(int t, int d) {
  for(int i=0; i<t; i++) {
    digitalWrite(pinLed, HIGH);
    delay(d);
    digitalWrite(pinLed, LOW);
    delay(d);
  }
}
