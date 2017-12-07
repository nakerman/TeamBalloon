#include <Time.h>;

int counter = 0;

time_t startTime;

int state;
const int bottomPosition = 1000.0;
const int topPosition = 350.0;
int setDuration = 20; // To get from weight sensor
int pastBlocks;
long blockDistance;
int millisPerDiscreteBlock;
int previousPosition = bottomPosition + 100;

// So that the average is 10000
long blockTotalDists = 10000;
long blockDistCounts = 1;


void setup() {
  
  //Setup Channel A
  pinMode(12, OUTPUT); // Initiates Motor Channel A pin
  pinMode(9, OUTPUT); // Initiates Brake Channel A pin
  digitalWrite(9, LOW); // Disengage the Brake for Channel A

  Serial.begin(115200); 

  time_t startTime = now();

  millisPerDiscreteBlock = 200;
  long miliStartTime = millis();
  pastBlocks = 0;
  blockDistance = 0;

  state = 0;
}


void loop(){


  time_t currentTimeSeconds = now();
  time_t timeDifferenceSeconds = currentTimeSeconds - startTime;
  
  int currentPosition = analogRead(A5);
  //Serial.println(currentPosition);
  int targetPosition;
  int distanceOffsetPerSecond;
  int distanceOffset;
  int distanceChanged;
  int numberOfSecondsAdded;
  long timeInMillis;
  int millisPerDiscreteBlock;
  long miliStartTime;
  int numberOfBlocks;
  int timeThatShouldBeLeft;
  int trueTimeLeft;
  int positionOffset;


      
  switch(state) {
    case 0: // Return to base position
      targetPosition = 1024;
      if (timeDifferenceSeconds > 3) {
        state = 1;
        Serial.println("Changing to slow rise");
      }
      else {
        moveToTarget(currentPosition, targetPosition, 255);
      }

      pastBlocks = millis() / millisPerDiscreteBlock;
      previousPosition = currentPosition;
    break;
    case 1: // Move to top

      // Need to check if being pushed down.
      // Average the offset for the last 0.2 seconds of readings
      // Then check if going down


      distanceOffset = currentPosition - previousPosition;
      blockDistance += distanceOffset;

      
      millisPerDiscreteBlock = 200;
      miliStartTime = millis();
      numberOfBlocks = 0;
      timeInMillis = millis();
      numberOfBlocks = timeInMillis / millisPerDiscreteBlock; 

      if (numberOfBlocks > pastBlocks) { 
        // A block has been completed, lets check if the button was pressed
        if (blockDistance  > 50) { // As inverted 
          Serial.println("Someone has pushed the sensor");

          // Need to update values, to reflect this

          // Where we should be at this time
          // timeDifferenceSeconds;

          // Where we are
          distanceOffsetPerSecond = (bottomPosition - topPosition) / setDuration;
          trueTimeLeft = (bottomPosition - currentPosition) / distanceOffsetPerSecond;

          // Minus the number of seconds from start time off the diff

          startTime += abs(timeDifferenceSeconds -  trueTimeLeft);
          
          
          pastBlocks += 1;
          blockDistance = 0;
     
          break;
        }
        pastBlocks += 1;
        blockDistance = 0;
      }


      distanceOffsetPerSecond = (bottomPosition - topPosition) / setDuration;
      distanceOffset = distanceOffsetPerSecond * timeDifferenceSeconds;
      
      targetPosition = bottomPosition - distanceOffset;
      targetPosition = max(topPosition, targetPosition);

      /*
      if ((previousPosition + 2) < currentPosition) {
        Serial.println("Someone has pushed the sensor");
        distanceChanged = abs(previousPosition - currentPosition);
        numberOfSecondsAdded = distanceChanged / distanceOffsetPerSecond;
        setDuration += numberOfSecondsAdded;
        previousPosition = currentPosition;
        break;
      }
      */

      previousPosition = currentPosition;
      
      distanceOffsetPerSecond = (bottomPosition - topPosition) / setDuration;
      distanceOffset = distanceOffsetPerSecond * timeDifferenceSeconds;
      targetPosition = bottomPosition - distanceOffset;
      targetPosition = max(topPosition, targetPosition);

      moveToTarget(currentPosition, targetPosition, 255);


      /*
      Serial.println("Sensor Reading");
      Serial.println(currentPosition);
      Serial.println("Target");
      Serial.println(targetPosition);
      Serial.println("distanceOffsetPerSecond");
      Serial.println(distanceOffsetPerSecond);
      Serial.println("distanceOffset");
      Serial.println(distanceOffset);
      Serial.println("Target");
      Serial.println(targetPosition);
      Serial.println("");
      */

      if (topPosition > currentPosition) {
        // We're at the top :D
      }
      
    break;
    case 2: 
      
    break;
  }

  /*
  time_t endTime = now();
  time_t timeDiff = endTime - startTime;



  int maxTarget = 350; // Due to additional weight
  
  
  int targetPosition = 500;
  int delta = target - truePosition;

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
  Serial.println(truePosition);
  Serial.println("Force:");
  Serial.println(force);
  //Serial.println("Time DIff:");
//  Serial.println(timeDiff);
  Serial.println("Target");
  Serial.println(target);



  
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

void moveToTarget(int currentPosition, int targetPosition, int power){
  int delta = targetPosition - currentPosition;
  if (delta < 0) {
    digitalWrite(12, HIGH);
    analogWrite(3, power);   //Spins the motor on Channel A at full speed
    
  }
  else{
    digitalWrite(12, LOW);
    analogWrite(3, power);   //Spins the motor on Channel A at full speed

  }
}

