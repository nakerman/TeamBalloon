int redPin = 11;
int grnPin = 9;
int bluPin = 10;
 
//uncomment this line if using a Common Anode LED
//#define COMMON_ANODE
// Color arrays
int black[3]  = { 0, 0, 0 };
int red[3]    = { 100, 0, 0 };
int green[3]  = { 0, 100, 0 };

int yellow[3] = { 80, 190, 0 };

int orange[3]  = {255,50,0};
// etc.

// Set initial color
int redVal = black[0];
int grnVal = black[1]; 
int bluVal = black[2];

int wait = 50;
int hold = 0;

int prevR = redVal;
int prevG = grnVal;
int prevB = bluVal;
 
void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(grnPin, OUTPUT);
  pinMode(bluPin, OUTPUT);  
}
 
void loop()
{
  //setColor(100, 0, 0);  // red
  //setColor(255,0,50);
  //setColor(0,0,255);
  crossFade(red);
  crossFade(orange);
  crossFade(yellow);
  crossFade(green);
}

int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = 1020/step;              //   divide by 1020
  } 
  return step;
}

int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;           
    } 
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}

void crossFade(int color[3]) {
  // Convert to 0-255
  int R = (color[0] * 255) / 100;
  int G = (color[1] * 255) / 100;
  int B = (color[2] * 255) / 100;

  int stepR = calculateStep(prevR, R);
  int stepG = calculateStep(prevG, G); 
  int stepB = calculateStep(prevB, B);

  for (int i = 0; i <= 1020; i++) {
    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);

    analogWrite(redPin, redVal);   // Write current values to LED pins
    analogWrite(grnPin, grnVal);      
    analogWrite(bluPin, bluVal); 

    delay(wait); // Pause for 'wait' milliseconds before resuming the loop
  }
    prevR = redVal; 
    prevG = grnVal; 
    prevB = bluVal;
    delay(hold); // Pause for optional 'wait' milliseconds before resuming the loop

}

