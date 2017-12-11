#include <Time.h>;

int state = 0;

const int bottomPosition = 1000.0;
const int topPosition = 350.0;
int secondsToRise = 10; // To get from weight sensor

int pastTimeBlocks = 0; // Past number of time blocks
long currentTimeBlockDistance = 0; // Distance travelled during a time block
int millisPerTimeBlock = 200;

int previousPosition = bottomPosition + 100; // The position of the bellows in the last iteration

long milliStartTime;

void setup() {

  // Setup Channel A
  pinMode(12, OUTPUT); // Initiates Motor Channel A pin
  pinMode(9, OUTPUT); // Initiates Brake Channel A pin
  digitalWrite(9, LOW); // Disengage the Brake for Channel A

  Serial.begin(115200);

  milliStartTime = millis();
}


void loop(){
  long timeDifferenceMillis =  millis() - milliStartTime;
  int currentPosition = analogRead(A5);

  int targetPosition;
  float distanceOffsetPerMilli; // Distance to travel per ms

  int distanceOffset;
  long timeInMillis;
  int numberOfBlocks;
  int timeThatShouldBeLeft;
  int trueTimeLeft;
  int positionOffset;

  switch(state) {
    case 0: // Return to base position
      if (timeDifferenceMillis > 3000) {
        state = 1;
        Serial.println("Changing to slow rise");
        milliStartTime = millis();
        previousPosition = currentPosition;
      }
      else {
        moveToTarget(currentPosition, bottomPosition, 50);
      }
    break;
    case 1: // Slowly Rise
      distanceOffset = currentPosition - previousPosition;
      currentTimeBlockDistance += distanceOffset;

      numberOfBlocks = timeDifferenceMillis / millisPerTimeBlock;

      if (numberOfBlocks > pastTimeBlocks) { // A new block has been completed
        if (currentTimeBlockDistance  > 20) { // As inverted
          Serial.println("Someone has pushed the sensor");

          // Where we are
          distanceOffsetPerMilli = ((bottomPosition - topPosition) + 0.0) / (secondsToRise * 1000);
          trueTimeLeft = (bottomPosition - currentPosition) / distanceOffsetPerMilli;

          // Minus the number of seconds from start time off the diff
          Serial.println("Start time before");
          Serial.println(milliStartTime);
          milliStartTime += (abs(timeDifferenceMillis -  trueTimeLeft));
          Serial.println("Start time after");
          Serial.println(milliStartTime);
          Serial.println("");
        }
        pastTimeBlocks += 1;
        currentTimeBlockDistance = 0;
      }

      distanceOffsetPerMilli = ((bottomPosition - topPosition) + 0.0) / (secondsToRise * 1000);
      distanceOffset = distanceOffsetPerMilli * timeDifferenceMillis; // How far it should be up

      targetPosition = bottomPosition - distanceOffset;
      targetPosition = max(topPosition, targetPosition);
  /*
      Serial.println("Target");
      Serial.println(targetPosition);
      Serial.println("distanceOffsetPerMilli");
      Serial.println(distanceOffsetPerMilli);
*/
      previousPosition = currentPosition;

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
