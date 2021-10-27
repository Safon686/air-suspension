#include <EEPROM.h>

int val,val1,val2;

#define NOT_INITIALIZED -1

int ledPin = 13;                 
int a,b,z,x=0; 
void setup()
{
 Serial.begin(9600);
  pinMode(13, OUTPUT);  
  pinMode(5, OUTPUT); //valve up
  pinMode(6, OUTPUT); //valve down
  pinMode(17, INPUT); //height sensor value
  pinMode(12, INPUT); //calibration button
  EEPROM.get(0, val1); //read preset 
}

bool key_pressed(int port) {    //function read button
  int value = analogRead(port);
  return value > 200;
}

void loop()
{
   val = analogRead(16);               //height sensor value (preset)
   if (key_pressed(17)) { 
    EEPROM.put(0, val);                //write value 
    Serial.println("Wtite to EEPROM");
    Serial.println(val); 
    delay(1000);
   }
  
   EEPROM.get(0, val1);                //read preset sensor value
   a=val1-50;                          //set variables with suspension error tolerance
   b=val1+50; //550
   if (val <= a) {
    digitalWrite(5, HIGH);             //if the current value is less than the preset, turn on the up valve
     }
     else {
      digitalWrite(5, LOW);
     }
   if (val >= b) {                    //if the current value is less than the preset, turn the valve down
     digitalWrite(6, HIGH);  
   }
   else {
    digitalWrite(6, LOW);
   }
   if ( b > val && val > a) {        //Suspension status indication
      digitalWrite(13, HIGH);  
   }
   else {
      digitalWrite(13, LOW);
  }
}
