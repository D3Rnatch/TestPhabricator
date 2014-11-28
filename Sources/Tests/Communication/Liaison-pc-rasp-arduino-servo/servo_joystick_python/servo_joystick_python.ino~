#include <Servo.h>

Servo escenter;
String mystring = "";


void setSpeedcent(int speed){
  //On envoie la vitesse désirée
  int vit = map(speed, 0, 100, 0, 180);
  escenter.write(vit);
}

void arm(){
  setSpeedcent(30);
  delay(1200);
  setSpeedcent(90);
  delay(1200);
  setSpeedcent(30);
  delay(1200);
}

void setup(){
    
    escenter.attach(10);
    arm();
    Serial.begin(9600);
    

}

void loop(){
  
  int value2;
  
  while(Serial.available() > 0)
  {
    int value = Serial.read();
    if(isDigit(value)){
      mystring += (char)value;
    }
    
    if(value == '\n'){
      //myservo.write(mystring.toInt()):
      value2 = mystring.toInt();
      if(value2 > 100)
        value2 = 100;
      if(value2<0)
        value2 = 0;
      setSpeedcent(value2);
      Serial.println(mystring);
      
      mystring = "";
    }
  }
}
