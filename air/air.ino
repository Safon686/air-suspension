#include <EEPROM.h>

int val,val1,val2;
uint32_t timer;
#define NOT_INITIALIZED -1
int ledPin = 13;                 
int a,b,z,x=0; 
bool setup_height;
int interval = 200;

void setup() {
 Serial.begin(9600);
  pinMode(13, OUTPUT);  
  pinMode(5, OUTPUT); //valve up
  pinMode(6, OUTPUT); //valve down
  pinMode(16, INPUT); //height sensor value
  pinMode(17, INPUT); //calibration button
  pinMode(18,INPUT);  //start preset button
  EEPROM.get(0, val1); //read preset 
  setup_height = false; //stat preset button
  timer = millis();     //timer
  long previosmillis = 0;
}
void work_valve(int port) {           //function of operation of valves with interruption of opening
  if (millis() - timer > interval) {  //from experience I can say that work with interruption is more stable for the formation of the suspension
  digitalWrite(port, HIGH);
  timer =millis(); 
  }
   if (millis()+100-timer>interval) {
  digitalWrite(port, LOW);
  }
}
bool key_pressed(int port) {          //function read button
  int value = analogRead(port);
  return value > 200;
}
void loop() {
   val = analogRead(16);               //height sensor value (preset)
   if (key_pressed(17)) {              //calibrate preset button
   EEPROM.put(0, val);                 //write value 
   Serial.println("Wtite to EEPROM");
   Serial.println(val); 
   delay(1000);
   }
      
   EEPROM.get(0, val1);                //read preset sensor value
   if (key_pressed(18)) {              //preset button
   setup_height = true;
   }
   if (setup_height && val != val1) { //
   Serial.println("Wtite 1");
   a=val1-50;                         //set variables with suspension error tolerance
   b=val1+50; 
   Serial.println("Wtite 2");
   if (val < a) {                    //if the current value is less than the preset, turn on the up valve
    work_valve(5);
   }
   Serial.println("Wtite 3");   
   if (val >= b) {                    //if the current value is less than the preset, turn the valve down
    work_valve(6);
   }
   if ( b > val && val > a) {        //Suspension status indication
      digitalWrite(13, HIGH);
      setup_height = false;          //off function preset after val == val1
      digitalWrite(5, LOW);          //we exclude the possibility of an open state of any valve
      digitalWrite(6, LOW);
   }
   else {
      digitalWrite(13, LOW);
   }
  } 
}
