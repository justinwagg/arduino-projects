#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// NEOPIXELS

#define NUMPIXELS 50 // Popular NeoPixel ring size

Adafruit_NeoPixel strip_a(NUMPIXELS, 9, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_b(NUMPIXELS, 10, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_c(NUMPIXELS, 11, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_d(NUMPIXELS, 12, NEO_GRB + NEO_KHZ800);


/*
Strip A = Tall Building (5x5)
Strip B = Mass MoCA
Strip C = Church
Strip D = Short Wide Building, (3 Tall, 7 Wide)
*/


// The active LEDS for each strip
#define stripASize 25
#define stripBSize 8
#define stripCSize 8
#define stripDSize 21

// setting an array of the active or visible LED's in each strip, allows us to make patterns
// in windows by only looping over the "active" LED's.
// if we set an entire strip when only parts of the strip are visible 
int stripAActives[stripASize]={3, 5, 7, 9, 11, 12, 14, 16, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49};
int stripBActives[stripBSize]={33, 34, 38, 40, 42, 44, 46, 48};
int stripCActives[stripCSize]={31, 32, 33, 39, 40, 47, 48, 49};
int stripDActives[stripDSize]={22, 23, 24, 26, 27, 28, 30, 31, 32, 34, 35, 36, 39, 40, 41, 43, 44, 45, 47, 48, 49};


// MARQUEE LIGHTS
int marqueeBulbs = 8;
int marqueeBulbPower = 3;

int treeBulbs = 7; // trigger
int treePower = 2;

// for the neon and bulbsthe we need to know how many times we've taken 
// the pin to ground i.e. "clicks" so we can track what mode we're in 
// both lights start out in mode 0, i.e. "off"
int marqueeBulbMode = 0; 
int treeBulbMode = 0;


// LIGHT SENSOR
int lightSensor = A0;
int lastLightReading;

// TIMES
unsigned long previousMillis = 0;  // stores the last millis when we automatically triggered the lights
const long triggerInterval = 180000;  // interval at which to trigger the lights (milliseconds)



void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

  // maruqee bulbs
  pinMode(marqueeBulbPower, OUTPUT);
  digitalWrite(marqueeBulbPower, HIGH);
  pinMode(marqueeBulbs, OUTPUT);
  delay(1000);
  marqueeBulbMode = setMarqueeBulbs(1); // steady on

  // tree bulbs
  pinMode(treePower, OUTPUT);
  digitalWrite(treePower, HIGH);
  delay(1000);
  pinMode(treeBulbs, OUTPUT);
  treeBulbMode = setTreeBulbs(1); // steady on  

  // sensor
  pinMode(lightSensor, INPUT);

  // strips
  strip_a.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_b.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_c.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_d.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  strip_a.clear(); // set all pixels to off
  strip_b.clear(); // set all pixels to off
  strip_c.clear(); // set all pixels to off
  strip_d.clear(); // set all pixels to off

  delay(500);

  doWarmRandomOffActives(&strip_a, stripASize, stripAActives);
  doWarmRandomOffActives(&strip_b, stripBSize, stripBActives);
  doWarmRandomOffActives(&strip_c, stripCSize, stripCActives);
  doWarmRandomOffActives(&strip_d, stripDSize, stripDActives);      
  
  delay(1000);
  
}

void loop() {

  fuji

  // if it's been over triggerInterval, trigger the light display
  if (currentMillis - previousMillis >= triggerInterval) {
    previousMillis = currentMillis;
    doRoutine();
    delay(10000);
  }

  if( checkLightSensor() ){
    doRoutine();
    previousMillis = currentMillis; 
    delay(10000);
  }

}

bool checkLightSensor(){

  // Any change in lights, up or down by this value will trigger the display
  int sensorTriggerDelta = 20; 

  int currentLightSensorReading = analogRead(lightSensor);
  int lightReadingDelta = abs(currentLightSensorReading - lastLightReading);

  Serial.print("currentLightSensorReading is ");
  Serial.print(currentLightSensorReading);
  Serial.print(" lastLightReading is ");
  Serial.print(lastLightReading);
  Serial.print(" lightReadingDelta is ");
  Serial.println(lightReadingDelta);

  lastLightReading = currentLightSensorReading;

  if(lightReadingDelta > sensorTriggerDelta){
    return true;
  }
  else{
    return false;
  }

}


void doRoutine(){

  marqueeBulbMode = setMarqueeBulbs(2); // set marquee to flash
  treeBulbMode = setTreeBulbs(2); // set marquee to flash

  // christmas
  doChristmasActives(&strip_a, stripASize, stripAActives);
  doChristmasActives(&strip_b, stripBSize, stripBActives);
  doChristmasActives(&strip_c, stripCSize, stripCActives);
  doChristmasActives(&strip_d, stripDSize, stripDActives);

  delay(1000);  

  // hanukkah
  doHanukkahActives(&strip_a, stripASize, stripAActives);
  doHanukkahActives(&strip_b, stripBSize, stripBActives);
  doHanukkahActives(&strip_c, stripCSize, stripCActives);
  doHanukkahActives(&strip_d, stripDSize, stripDActives);

  delay(1000);

  // kwanza
  doKwanzaActives(&strip_a, stripASize, stripAActives);
  doKwanzaActives(&strip_b, stripBSize, stripBActives);
  doKwanzaActives(&strip_c, stripCSize, stripCActives);
  doKwanzaActives(&strip_d, stripDSize, stripDActives);

  delay(1000);

  doRedGreen();

  delay(1000);

  theaterChase(&strip_a, 50);
  theaterChase(&strip_b, 50);
  theaterChase(&strip_c, 50);
  theaterChase(&strip_d, 50);

  delay(1000);

  // do we want to set all pixels to off before rainbowing!?
  // strip_a.clear(); // set all pixels to off
  // strip_a.show();
  // strip_b.clear(); // set all pixels to off
  // strip_b.show();
  // strip_c.clear(); // set all pixels to off
  // strip_c.show();
  // strip_d.clear(); // set all pixels to off
  // strip_d.show();

  // rainbow
  rainbow(&strip_a, 10);
  rainbow(&strip_b, 10);
  rainbow(&strip_c, 10);
  rainbow(&strip_d, 10);

  delay(1000);

  strip_a.clear(); // set all pixels to off
  strip_a.show();
  strip_b.clear(); // set all pixels to off
  strip_b.show();
  strip_c.clear(); // set all pixels to off
  strip_c.show();
  strip_d.clear(); // set all pixels to off
  strip_d.show();

  // reset to strips to warm
  doWarmRandomOffActives(&strip_a, stripASize, stripAActives);
  doWarmRandomOffActives(&strip_b, stripBSize, stripBActives);
  doWarmRandomOffActives(&strip_c, stripCSize, stripCActives);
  doWarmRandomOffActives(&strip_d, stripDSize, stripDActives);

  marqueeBulbMode = setMarqueeBulbs(1); // reset marquee to steady
  treeBulbMode = setTreeBulbs(1); // reset marquee to steady


  

}

void doChristmasActives(Adafruit_NeoPixel *strip, int len, int pins[]){
  /*
  Sets a strip to green/red
  */
  for(int i=0; i<len; i=i+2){
    strip->setPixelColor(pins[i], strip->Color(255, 0, 0));
    strip->show();
    delay(25);
  }

  for(int i=1; i<len; i=i+2){
    strip->setPixelColor(pins[i], strip->Color(0,255,0));
    strip->show();
    delay(25);
  }

}


void doRedGreen(){

  for (int i = 0; i <= 5; i++) {

    strip_b.fill(strip_c.Color(255, 0, 0));
    strip_d.fill(strip_d.Color(0, 255, 0));
    strip_a.fill(strip_b.Color(255, 0, 0));
    strip_c.fill(strip_a.Color(0, 255, 0));
    
    strip_b.show();
    strip_d.show();
    strip_a.show();
    strip_c.show(); 

    delay(1000);

    strip_b.fill(strip_c.Color(0, 255, 0));
    strip_d.fill(strip_d.Color(255, 0, 0));
    strip_a.fill(strip_b.Color(0, 255, 0));
    strip_c.fill(strip_a.Color(255, 0, 0));  

    strip_b.show();
    strip_d.show();
    strip_a.show();
    strip_c.show();   
    
    delay(1000);  

  }

}

void theaterChase(Adafruit_NeoPixel *strip, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip->clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip->numPixels(); c += 3) {
        strip->setPixelColor(c, strip->Color(225, 225, 225)); // Set pixel 'c' to value 'color'
      }
      strip->show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}


void doKwanzaActives(Adafruit_NeoPixel *strip, int len, int pins[]){
  // Sets every third window to either green/yellow/red

  // green
  for(int i=0; i<len; i=i+3){
    strip->setPixelColor(pins[i], strip->Color(0, 255, 0));
    strip->show();
    delay(25);
  }

  // yellow
  for(int i=1; i<len; i=i+3){
    strip->setPixelColor(pins[i], strip->Color(255, 255, 0));
    strip->show();
    delay(25);
  }

  // red
  for(int i=2; i<len; i=i+3){
    strip->setPixelColor(pins[i], strip->Color(255, 0, 0));
    strip->show();
    delay(25);
  }  
}


void doHanukkahActives(Adafruit_NeoPixel *strip, int len, int pins[]){
  // Sets every third window to either red, warm, or white

  //blue
  for(int i=0; i<len; i=i+2){
    strip->setPixelColor(pins[i], strip->Color(0,0,255));
    strip->show();
    delay(25);
  }
  //white
  for(int i=1; i<len; i=i+2){
    strip->setPixelColor(pins[i], strip->Color(255, 255, 255));
    strip->show();
    delay(25);
  }  
}

void rainbow(Adafruit_NeoPixel *strip, int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip->rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip->show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}



void doWarm(Adafruit_NeoPixel *strip){
    for(int i=NUMPIXELS; i>=0; i--){
    strip->setPixelColor(i, strip->Color(199, 193, 20));
    strip->show();
    delay(5);
  }
}

void doWarmRandomOffActives(Adafruit_NeoPixel *strip, int len, int pins[]){
  /*
  Sets a strip to a combination of a warm color, and off
  The pixel combination of warm/off is randomized, so that 
  each time the strip is wiped to warm, a different scene will show 
  in each window.
  */
    for(int i=len; i>=0; i--){
      long rand = random(100);
      long threshold = 70; // this number controls the percent of total windows that are set to off
      if(rand >= threshold){
        strip->setPixelColor(pins[i], strip->Color(0, 0, 0));
      }
      else{
        // 199, 193, 20 -- original
        // 239,192,112
        strip->setPixelColor(pins[i], strip->Color(255,249,50));
      }
    strip->show();
    delay(5);
  }
}



int setMarqueeBulbs(int mode) {
/*
MODES
0 = off
1 = steady
2 = twinkle
3 = glisten
4 = waves
5 = sequential
6 = chasing
7 = fade
8 = combination
*/  

  if(marqueeBulbMode != mode){
    // if the marqueeMode doesn't equal the desired mode, set it
    Serial.print("marquee is currently in mode: ");
    Serial.println(marqueeBulbMode);

    Serial.print("Setting the marquee to mode: ");
    Serial.println(mode);

    // function will cycle the button on and off until getting to 9
    // afterwhich it resets the counter to 0 and will continue to cycle
    // until the mode hits the desired state
    for (int i = marqueeBulbMode; i != mode; i++) {
      Serial.print("counter i = ");
      Serial.println(i);
      digitalWrite(marqueeBulbs, HIGH);
      delay(50);
      digitalWrite(marqueeBulbs, LOW);
      delay(50);
      if (i == 9) i = 0; // reset the counter, there is no 9th mode, this goes back to zero, or off
    }
  }
  else{
  Serial.print("ELSE!! marquee is currently in mode: ");
    Serial.println(marqueeBulbMode);

    Serial.print("Desired mode is: ");
    Serial.println(mode);    

  }
  return mode; // sets the global marqueeMode so we keep track of where we are in the cycle

}

int setTreeBulbs(int mode) {
/*
MODES
0 = off
1 = steady
2 = twinkle
3 = glisten
4 = waves
5 = sequential
6 = chasing
7 = fade
8 = combination
*/  

  if(treeBulbMode != mode){
    // if the marqueeMode doesn't equal the desired mode, set it
    Serial.print("marquee is currently in mode: ");
    Serial.println(treeBulbMode);

    Serial.print("Setting the marquee to mode: ");
    Serial.println(mode);

    // function will cycle the button on and off until getting to 9
    // afterwhich it resets the counter to 0 and will continue to cycle
    // until the mode hits the desired state
    for (int i = treeBulbMode; i != mode; i++) {
      Serial.print("counter i = ");
      Serial.println(i);
      digitalWrite(treeBulbs, HIGH);
      delay(50);
      digitalWrite(treeBulbs, LOW);
      delay(50);
      if (i == 9) i = 0; // reset the counter, there is no 9th mode, this goes back to zero, or off
    }
  }
  else{
  Serial.print("ELSE!! tree is currently in mode: ");
    Serial.println(treeBulbMode);

    Serial.print("Desired mode is: ");
    Serial.println(mode);    

  }
  return mode; // sets the global marqueeMode so we keep track of where we are in the cycle

}

// int setMarqueeNeon(int mode) {
// /*
// MODES
// 0 = off
// 1 = steady
// 2 = blink
// 3 = blink fast
// */  

//   if(marqueeNeonMode != mode){
//     // if the marqueeMode doesn't equal the desired mode, set it
//     Serial.print("marquee neon is currently in mode: ");
//     Serial.println(marqueeNeonMode);

//     Serial.print("Setting the marquee NEON to mode: ");
//     Serial.println(mode);

//     // function will cycle the button on and off until getting to 9
//     // afterwhich it resets the counter to 0 and will continue to cycle
//     // until the mode hits the desired state
//     for (int i = marqueeNeonMode; i != mode; i++) {
//       Serial.print("NEON counter i = ");
//       Serial.println(i);
//       digitalWrite(marqueeNeon, HIGH);
//       delay(50);
//       digitalWrite(marqueeNeon, LOW);
//       delay(50);
//       if (i == 3) i = 0; // reset the counter, there is no 9th mode, this goes back to zero, or off
//     }
//   }
//   return mode; // sets the global marqueeMode so we keep track of where we are in the cycle

// }