#include "HX711.h"

HX711 scale(3, 2);

void setup() {
  Serial.begin(9600);

  scale.set_scale(203350.0f);    // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

  Serial.println("Readings:");
}

float forceHistory[4] = {0,0,0,0};
int forceHistoryIndex = 0;
void loop() {
  if (Serial.available() > 0) {
    forceHistory[forceHistoryIndex] = scale.get_units();
    forceHistoryIndex = (forceHistoryIndex + 1) % 4;
    
    float force = (forceHistory[0] + forceHistory[1] + forceHistory[2] + forceHistory[3])/4.0f;
    
    // read the incoming byte:
    char commandChar = Serial.read();
    
    // only do 1 char commands for now
    switch(commandChar){
      case 't': // tare
      scale.tare();
      break;
      
      case 'm': // measure
      {
        Serial.print(force, 10);
        Serial.print(", ");
        
        float coinWeight = 0.00576f;
        float coins = (force/coinWeight);
        Serial.print(coins, 2);
        Serial.print(" coins, ");
        int seconds = coins*30;
        Serial.print(seconds);
        Serial.println(" seconds");
        break;
      }
      
      case '\n':
      default:
      break;
    }
  }
}
