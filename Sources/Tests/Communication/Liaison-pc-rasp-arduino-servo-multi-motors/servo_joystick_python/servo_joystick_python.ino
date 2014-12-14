#include <Servo.h>

#include "protocol_helper.h"

Servo escenter[4];
String mystring = "";

t_frame_bytes extracted_frame; 

void setSpeedcent(int a, int speed){
  //On envoie la vitesse désirée
  int vit = map(speed, 0, 100, 0, 180);
  escenter[a].write(vit);
}

void arm(){
  setSpeedcent(0,10);
  setSpeedcent(1,10);
  setSpeedcent(2,10);
  setSpeedcent(3,10);
  delay(1200);
  setSpeedcent(0,90);
  setSpeedcent(1,90);
  setSpeedcent(2,90);
  setSpeedcent(3,90);
  delay(1200);
  setSpeedcent(0,10);
  setSpeedcent(1,10);
  setSpeedcent(2,10);
  setSpeedcent(3,10);
  delay(1200);
}

void setup(){
    
    escenter[0].attach(2);
    
    escenter[1].attach(4);
    escenter[2].attach(6);
    escenter[3].attach(8);
    arm();
    Serial.begin(9600);
    

}

void loop(){
  
  int value2;
  
  while(Serial.available() > 0)
  {
    Serial.flush();
    
    char value[7];
    Serial.readBytes(value,7);
    extracted_frame = extract_data_bytes((byte*)value);

    Serial.flush();

    for (int i=0;i<5;i++) {
          Serial.print(extracted_frame.array[i]);
          Serial.print(':');
          value[i] = '\0';	  
	  
   }
	// Serial.println(extracted_frame.array);
   Serial.print("\n");

    // To string > int
    // mystring += (char)extracted_frame.type;
    value2 = (char)extracted_frame.type - 48;
    if (value2 < 0) value2 = 0;
    else if (value2 > 255) value2 = 255;

    Serial.print("IDIS:");
     Serial.println(value2);
    //Serial.println(extracted_frame.type);
    
    Serial.flush();

   // trame de move update
   if (value2 == 2)
   {
	// Serial.println("TYPE2");
        for(int i=0;i<4;i++)
        {
		if(extracted_frame.array[i] != 0)
		{
			if(extracted_frame.array[i] > 100) extracted_frame.array[i] = 100;
			else if(extracted_frame.array[i] < 0) extracted_frame.array[i] = 0;	
			setSpeedcent(i,extracted_frame.array[i]);
		}
		else // If it is equal to 0 we put 0....
			setSpeedcent(i,0);
	}

   }
   // On éteint les moteurs
   else {
	// Serial.println("NTYP2");
	for(int i=0;i<4;i++)
		setSpeedcent(i,0);
   }

   delay(20);

   /*
    // int value = Serial.read();
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
    }*/
  }
}
