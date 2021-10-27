#include <EEPROM.h>

int val,val1,val2;

#define NOT_INITIALIZED -1

int ledPin = 13;                 // Светодиод подключенный к вход/выходу 13
int a,b,z,x=0; 
void setup()
{
 Serial.begin(9600);
  pinMode(13, OUTPUT);       // устанавливает режим работы - выход для 13го вход/выхода (pin)
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(17, INPUT);
   pinMode(12, INPUT);
   EEPROM.get(0, val1);
   

}

bool key_presesd(int port) {
  int value = analogRead(port);
  return value > 200;
}

 }
void loop()
{
   val = analogRead(16); // считываем значение с входа
   if (key_pressed(17)) {
   
   
    EEPROM.put(0, val);  //условно 500
    Serial.println("Wtite to EEPROM");
    Serial.println(val); 
    Serial.println("Read from EEPROM");
    Serial.println(val1);
       delay(1000);
  }
  
   EEPROM.get(0, val1);
    a=val1-50; //450
    b=val1+50; //550
   if (val <= a) {
    digitalWrite(5, HIGH);  
     }
     else {
      digitalWrite(5, LOW);
     }
  if (val >= b) {
    digitalWrite(6, HIGH);  
  }
  else {
    digitalWrite(6, LOW);
  }
    if ( b > val && val > a) {
    
  digitalWrite(13, HIGH);  
  }
  else {
  digitalWrite(13, LOW);

  
  }
  
  
}
