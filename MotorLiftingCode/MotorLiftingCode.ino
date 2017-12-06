/*************************************************************
Motor Shield 1-Channel DC Motor Demo
by Randy Sarafan

For more information see:
https://www.instructables.com/id/Arduino-Motor-Shield-Tutorial/

*************************************************************/
#include <Time.h>;

int counter = 0;

time_t startTime;

 
void setup() {
  
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A

  Serial.begin(115200); 

  // startTime = now();
  
  // timeOffset = 0;

}



void loop(){



  //time_t endTime = now();

  //time_t timeDiff = endTime - startTime;

  

  
  int result = analogRead(A5);
  

  //int target = timeDiff / 100;
  int target = 500;
  int delta = target - result;

  int maxPower = 255;
  int force = (delta/ 1024.0f) * maxPower;
  
  force = abs(force);
  force += 255; 

  

  force = constrain(force, 0, maxPower);

  if (delta < 0) {
    digitalWrite(12, HIGH);
    analogWrite(3, force);   //Spins the motor on Channel A at full speed
    
  }
  else{
    digitalWrite(12, LOW);
    analogWrite(3, force);   //Spins the motor on Channel A at full speed

  }

  digitalWrite(9, LOW);
  Serial.println("Sensor Read");
  Serial.println(result);
  Serial.println("Force:");
  Serial.println(force);
  //Serial.println("Time DIff:");
//  Serial.println(timeDiff);
  Serial.println("Target");
  Serial.println(target);


/*
  
      int result = analogRead(A3);
    Serial.print("Raw Reading");
    Serial.println(result);

  counter++;
  if ((counter % 30) < 15){
    // Read values  
    

  }
  else {
    
       // digitalWrite(12, HIGH);
      //analogWrite(3, 255);   //Spins the motor on Channel A at full speed
  }

 



    if (counter % 10 == 0){

    }
    else{
      digitalWrite(12, LOW);
      analogWrite(3, 255);   //Spins the motor on Channel A at full speed
    }
    
  counter++;
 
   
  int result = analogRead(A0);
  

  int target = 27;
  int delta = target - result;

  if (delta < 0) {
    digitalWrite(12, HIGH);
    analogWrite(3, 255);   //Spins the motor on Channel A at full speed
    
  }
  else{
    digitalWrite(12, LOW);
    analogWrite(3, 255);   //Spins the motor on Channel A at full speed

  }

  digitalWrite(9, LOW);
  
  if (counter == 1000){
    Serial.print("Raw Reading");
    Serial.println(result);  
  
    Serial.print("Delta: ");
    Serial.println(delta);
    counter = 0;
  }
  
  //forward @ full speed
  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  
  delay(3000);
  
  digitalWrite(9, HIGH); //Eengage the Brake for Channel A

  delay(1000);
  
  //backward @ half speed
  digitalWrite(12, LOW); //Establishes backward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 123);   //Spins the motor on Channel A at half speed
  
  delay(3000);
  
  digitalWrite(9, HIGH); //Eengage the Brake for Channel A
  
  delay(1000);

  */
  
}

