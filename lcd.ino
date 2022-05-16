
#include <Adafruit_NeoPixel.h> //includes Adafruit’s neopixel library
//pin numbers/ports on sunnyview board
#define PIN A7
#define breakBeamSensor1 0
#define breakBeamSensor2 1
#define breakBeamSensor3 A1
#define breakBeamSensor4 2
#define breakBeamSensor5 3
#define breakBeamSensor6 A6
#define NUM_LEDS 24
#define NUM_RINGS 6
int green = 65536/3;
int blue = 65536/2;
int blueDark = 65536*(2/3);
int purple = 65536*(5/6);
int yellow = 65536/6;
int red = 0;
int pastColour = 0;

int colour[] = {65536/3,8000, 43690, 52000, 65536/6, 0};

//Initialize neopixel strip light
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS * NUM_RINGS, PIN, NEO_GRB + NEO_KHZ800);
//Initialize array for break beam sensors
int broken[] = {0,0,0,0,0,0};

long firstPixelHue = 0;//Keeps track of pixel hue for rainbow effect
int currentGoal;//Keeps track of of current sensor & ring that needs to be scored on
int old; 

void setup() {
//Setup strip light:
  strip.begin();
  strip.show();  
  strip.setBrightness(50);
//Setup break beam sensors as input type
  pinMode(breakBeamSensor1, INPUT);
  pinMode(breakBeamSensor2, INPUT);
  pinMode(breakBeamSensor3, INPUT);
  pinMode(breakBeamSensor4, INPUT);
  pinMode(breakBeamSensor5, INPUT);
  pinMode(breakBeamSensor6, INPUT);
//Select a random number from 0 to 5 to be first goal/target
  currentGoal = random(6);
  Serial.begin(9600);
}

void loop() {
//loop to refresh input and output
    refreshSensors();
    refreshGame();
    refreshLights();
//If pixel hue exceeds 65536 set it back to 0 to continue looping rainbow effect
    if(firstPixelHue >= 65536) {
      firstPixelHue = 0;
    }
}

void refreshSensors() {
//Checks each sensor if it has been broken then adds the corresponding value to the array
  for(int t = 0; t < 6; t++) {
      switch(t) {
        case 0:
          broken[t] = digitalRead(breakBeamSensor1);
          break;
        case 1:
          broken[t] = digitalRead(breakBeamSensor2);
          break;
        case 2:
          broken[t] = digitalRead(breakBeamSensor3);
          break;
        case 3:
          broken[t] = digitalRead(breakBeamSensor4);
          break;
        case 4:
          broken[t] = digitalRead(breakBeamSensor5);
          break;
        case 5:
          broken[t] = digitalRead(breakBeamSensor6);
      }
   }
}

void refreshLights() {
//Checks each light if it is the current goal
  for(int i = 0; i < NUM_RINGS; i++) {
    if(i == currentGoal) {
//If it is, then set the LEDs to rainbow
      rainbow(i);
    } else {
//If not, set it to default “resting” colour
      setRingOff(i);
    }
  }
}

void refreshGame() {
  int num = random(6);
  while(num == currentGoal){
    num = random(6);
  }
//If the sensor value of the current sensor has detected something
   if(broken[currentGoal] == HIGH) {
//Set new goal
//      remix(num); // calls remix method, that does randomize animation
      forcedToBeBoring();
      delay(100);
      currentGoal = num;
      Serial.println("Detect");
      
   }
}






/*
 * Stehlik said my old methods were too complicated );
 * Method is each time the goal is reached (Putting hand in correct hole)
 * First will cycle through the rainbow on all the LED Rings (1 colour per LED ring)
 */
void forcedToBeBoring(){
  int hue;
  for(int i = 0; i <=6; i++){ //will loop through all 6 led rings to light them up
    hue = colour[i]; //will select the colour for the LED ring from a Array of ints,index is the i int 
   for(int j = 0; j < NUM_LEDS; j++){  //this will loop through the individual led's in each individual led ring
  strip.setPixelColor(j + (NUM_LEDS * i),strip.gamma32(strip.ColorHSV(hue))); 
  strip.show();
 
    }
     delay(250);
  }
  off();
}
/*
 * This method Will wait 250ms and then make all the LED's green in order to start the next goal
 */
void off(){
 for(int i = 0; i <=6; i++){ //loops through all 6 led rings 
 
   for(int j = 0; j < NUM_LEDS; j++){ //loop through the individual led's in each led ring to light them up
  strip.setPixelColor(j + (NUM_LEDS * i),strip.gamma32(strip.ColorHSV(20000))); 
  strip.show();
 
    }
     
  }
  delay(250);
  // some colour codes that I should probably know
  // 20000 green
  // 0 red
  // 30000 turq
  // 5000 orange
  // 10000 yellow
  
     
}

int select(){
  int num = random(1,6);
  while(num == pastColour){
    num = random(1,6);
  }
  if(num == 1){
    return(green);
    pastColour = 1;
  }else if(num == 2){
    return(blue);
    pastColour = 2;
  }else if(num == 3){
    return(blueDark);
    pastColour = 3;
  }else if(num == 4){
    return(purple);
    pastColour = 4;
  }else if(num == 5){
    return(yellow);
    pastColour = 5;
  }else if(num == 6){
     pastColour = 6;
    return(red);
   
  }
  
}

void rainbow(int ringNum) {
//sets a ring to rainbow effect
 
    for(int i=0; i< NUM_LEDS; i++) {//for each of the LEDs on the ring
//Set the hue as 65536/number of LEDs on the ring + offset
      int pixelHue = firstPixelHue + (i * 65536L / NUM_LEDS);

//Set the LED on the chain to the hue
      strip.setPixelColor(i + (NUM_LEDS * ringNum), strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    firstPixelHue += 512;//increase offset

  
}

void setRingOff(int ringNum) {
//Sets the ring to default colour RGB (235, 52, 207)/pink
   for(int i = 0; i < NUM_LEDS; i++){
       strip.setPixelColor(i + (NUM_LEDS * ringNum), 235, 52, 207);
       
   }
   strip.show();
}
