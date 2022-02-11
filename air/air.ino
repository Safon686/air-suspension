#include <EEPROM.h>

int  valveup, valvedown, sensor_preset1, sensor_preset2, sensor_preset3, sensor_preset4, sensor1, sensor2, sensor3, sensor4;
uint32_t timer;
#define NOT_INITIALIZED -1
int a, b;
bool setup_height;
int interval = 200;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(5, OUTPUT); //valve up 1
  pinMode(6, OUTPUT); //valve down 2
  pinMode(7, OUTPUT); //valve up 3
  pinMode(8, OUTPUT); //valve down 4
  pinMode(9, OUTPUT); //valve up 5
  pinMode(10, OUTPUT); //valve down 6
  pinMode(11, OUTPUT); //valve up 7
  pinMode(12, OUTPUT); //valve down 8
  pinMode(16, INPUT); //height 1 sensor value
  pinMode(17, INPUT); //height 2 sensor value
  pinMode(18, INPUT); //height 3 sensor value
  pinMode(19, INPUT); //height 4 sensor value
  pinMode(14, INPUT); //calibration button
  pinMode(15, INPUT); //start preset button
  EEPROM.get(10, sensor_preset1); //read preset sensor1
  EEPROM.get(30, sensor_preset2); //read preset sensor2
  EEPROM.get(50, sensor_preset3); //read preset sensor3
  EEPROM.get(70, sensor_preset4); //read preset sensor4

  setup_height = false; //stat preset button
  timer = millis();     //timer
  long previosmillis = 0;
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
}
void pre_work_valve(int port) {           //function of operation of valves with interruption of opening
  if (millis() - timer > interval) {      //valve function when approaching the result
    digitalWrite(port, LOW);             ////from experience I can say that work with interruption is more stable for the formation of the suspension
    timer = millis();
  }
  if (millis() + 100 - timer > interval) {
    digitalWrite(port, HIGH);
  }
}
void work_valve(int port) {                   //function of operation of valves with interruption of opening
  digitalWrite(port, LOW);
}
bool key_pressed(int port) {                 //function read button
  int value = analogRead(port);
  return value > 200;
}
bool work_fun(bool setup_height, int valveup, int valvedown, int sensor, int sensor_preset)  {
  bool setup_heightx = setup_height;
  int conditionlow = 1;
  int conditionup = 1;
  a = sensor_preset - 20;                                   //set variables with suspension error tolerance
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
  if ((sensor >= b + 100) && (conditionup == 1)) {                     //if the current value is less than the preset, turn the valve down
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
    setup_heightx = false;                //off function preset after val == val1
    digitalWrite(valvedown, HIGH);         //we exclude the possibility of an open state of any valve
    digitalWrite(valveup, HIGH);
    return false;
  }
  else {
    digitalWrite(13, LOW);
  }
  return true;
}
void loop() {
  sensor1 = analogRead(16);                  //height sensor value (preset)
  sensor2 = analogRead(17);
  sensor3 = analogRead(18);
  sensor4 = analogRead(19);
  if (key_pressed(14)) {                     //calibrate preset button
    EEPROM.put(10, sensor1);                 //write value
    EEPROM.put(30, sensor2);
    EEPROM.put(50, sensor3);
    EEPROM.put(70, sensor4);

  }

  EEPROM.get(10, sensor_preset1);            //read preset sensor value
  EEPROM.get(30, sensor_preset2);
  EEPROM.get(50, sensor_preset3);
  EEPROM.get(70, sensor_preset4);
  if (key_pressed(15)) {                     //preset button
    setup_height = true;
  }
  if (setup_height) {                        //functions run together after pressing a button and work independently of each other
    bool setup_height1 = work_fun(setup_height, 5, 6, sensor1, sensor_preset1) ;
    bool setup_height2 = work_fun(setup_height, 7, 8, sensor2, sensor_preset2) ;
    bool setup_height3 = work_fun(setup_height, 9, 10, sensor3, sensor_preset3) ;
    bool setup_height4 = work_fun(setup_height, 11, 12, sensor4, sensor_preset4) ;

    if (setup_height1 == 0 && setup_height2 == 0 && setup_height3 == 0 && setup_height4 == 0) {
      setup_height = false;                   //as a preset is reached on the sensor, the function stops working for an individual wheel.
    }                                         //after stopping the last running function, the state of the button is reset to zero
  }
}
