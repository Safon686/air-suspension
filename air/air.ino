#include <EEPROM.h>

int val, val1, val2, valveup, valvedown, sensor, sensor_preset, sensor1;
uint32_t timer;
#define NOT_INITIALIZED -1
int ledPin = 13;
int a, b, z, x = 0;
bool setup_height;
int interval = 200;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(5, OUTPUT); //valve up
  pinMode(6, OUTPUT); //valve down
  pinMode(16, INPUT); //height 1 sensor value
  pinMode(15, INPUT); //height 2 sensor value
  pinMode(14, INPUT); //height 3 sensor value
  pinMode(19, INPUT); //height 4 sensor value
  pinMode(17, INPUT); //calibration button
  pinMode(18, INPUT); //start preset button
  EEPROM.get(10, sensor_preset); //read preset
  setup_height = false; //stat preset button
  timer = millis();     //timer
  long previosmillis = 0;
}
void pre_work_valve(int port) {           //function of operation of valves with interruption of opening
  if (millis() - timer > interval) {      //valve function when approaching the result
    digitalWrite(port, HIGH);             ////from experience I can say that work with interruption is more stable for the formation of the suspension
    timer = millis();
  }
  if (millis() + 100 - timer > interval) {
    digitalWrite(port, LOW);
  }
}
void work_valve(int port) {               //function of operation of valves with interruption of opening
  digitalWrite(port, HIGH);
}
bool key_pressed(int port) {              //function read button
  int value = analogRead(port);
  return value > 200;
}
void work_fun(bool setup_height, int valveup, int valvedown, int sensor, int sensor_preset)  {
 bool setup_heightx=setup_height;
 if (setup_heightx && sensor != sensor_preset) {   
   int conditionlow = 1;
   int conditionup = 1;
    a = sensor_preset - 20;                     //set variables with suspension error tolerance
    b = sensor_preset + 20;
    if ((sensor <= a - 100) && (conditionlow == 1)) {
      work_valve(valvedown);
    }
    else {
      conditionlow = 2;
    }
    if (((a - 100 < sensor) && (sensor < a)) && (conditionlow == 2)) {   //if the current value is less than the preset, turn on the up valve
      pre_work_valve(valvedown);
    }
    if ((sensor >= b + 100) && (conditionup == 1)) {                  //if the current value is less than the preset, turn the valve down
      work_valve(valveup);
    }
    else {
      conditionup = 2;
    }
    if (((b < sensor) && (sensor < b + 100)) && (conditionup == 2)) {
      pre_work_valve(valveup);
    }
    if ( b > sensor && sensor > a) {        //Suspension status indication
      digitalWrite(13, HIGH);
      setup_height = false;          //off function preset after val == val1
      digitalWrite(valvedown, LOW);          //we exclude the possibility of an open state of any valve
      digitalWrite(valveup, LOW);
    }
    else {
      digitalWrite(13, LOW);
   }
    
  }

}
void loop() {
  sensor1 = analogRead(16);                 //height sensor value (preset)
  if (key_pressed(17)) {                //calibrate preset button
    EEPROM.put(10, sensor1);                 //write value
   }

  EEPROM.get(10, sensor_preset);                  //read preset sensor value
  if (key_pressed(18)) {                //preset button
    Serial.println("Read to EEPROM");
    Serial.println(sensor_preset);
    delay(1000);
    setup_height = true;
    
  }
  
  if (setup_height && sensor != sensor_preset) {
    work_fun(setup_height, 5, 6, sensor1, sensor_preset) ;
    setup_height = work_fun;
  }
  
}
