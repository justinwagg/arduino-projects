#include <Adafruit_NeoPixel.h>

int buzzer = 10;
int led = 13;

int left_switch = 2;
int right_switch = 3;
int _buzz_switch = 50;
int _mode_switch = 51;

int mode = 0;
int buzz = 0;

int buzz_led = 52;
int mode_led = 53;

#define NUMPIXELS 24 // # of pixels per board
Adafruit_NeoPixel strip_a(NUMPIXELS, 6, NEO_RGBW);
Adafruit_NeoPixel strip_b(NUMPIXELS, 7, NEO_RGBW);



#define MIN_DELAY 10 // Smallest delay at the start
#define MAX_DELAY 500 // Largest delay towards the end


int hit_count = 0;


void setup() {

  pinMode(left_switch, INPUT_PULLUP);
  pinMode(right_switch, INPUT_PULLUP);

  pinMode(_buzz_switch, INPUT_PULLUP);
  pinMode(_mode_switch, INPUT_PULLUP);

  pinMode(buzz_led, OUTPUT);
  digitalWrite(buzz_led, LOW);

  pinMode(mode_led, OUTPUT);
  digitalWrite(mode_led, LOW);  

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  initiateStrip(&strip_a);
  initiateStrip(&strip_b);

  // just to help understand when the arduino resets
  for (int i=0; i < 10; i++){
    digitalWrite(mode_led, HIGH);
    delay(50);
    digitalWrite(mode_led, LOW); 
    delay(50);
  }

  mode = digitalRead(_mode_switch);
  buzz = digitalRead(_buzz_switch);

}

void loop() {

  mode = digitalRead(_mode_switch);
  digitalWrite(mode_led, mode);

  buzz = digitalRead(_buzz_switch);
  digitalWrite(buzz_led, buzz);

  // if MODE IS TRUE THEN DO THE MORE ELABORATE SHOW
  if(mode == 1){

    if(hit_count < 3){
      // SIMPLE "NORMAL" SECTION BEGIN
      if( digitalRead(left_switch) == 0 ){

        lightUp(&strip_a);
        if(buzz == 1){
          doBuzz(1);
        }
        delay(500);
        lightDown(&strip_a);
        

      }

      if( digitalRead(right_switch) == 0 ){

        lightUp(&strip_b);
        if(buzz == 1){
          doBuzz(1);
        }
        delay(500);
        lightDown(&strip_b);
        
      }
    }
    // SIMPLE "NORMAL" SECTION END
    // NORMAL UP, RED CASCADE DOWN
    else if(hit_count == 3 || hit_count == 4){

      if( digitalRead(left_switch) == 0 ){

        lightUp(&strip_a);
        if(buzz == 1){
          doBuzz(1);
        }
        delay(500);
        // fillDownWithRed(&strip_a, 50);
        lightDownRed2(&strip_a);
        
        

      }

      if( digitalRead(right_switch) == 0 ){

        lightUp(&strip_b);
        if(buzz == 1){
          doBuzz(1);
        }
        delay(500);
        // fillDownWithRed(&strip_b, 50);
        lightDownRed2(&strip_b);
        
      }

    }
    else if(hit_count == 5){
      // ROUTINES START - not controlled by triggers, just for fun
      
      // shimmerCascade(10, 500);
      shimmerCascade2(10, 100);
      alternateRedWhite(100, 5);
      theaterChase2(100);  
      theaterChase4(25);
      theaterChase6(25);
      // while(1==1){}
      stripClearShow(&strip_a);
      stripClearShow(&strip_b);
      hit_count++;
    }
    else if(hit_count == 6 || hit_count == 7){
      // RETURN TO NORMAL
      // should we reset here or does it look cool?
      // on our way back to normal state, do the one level at a time animation
      if( digitalRead(left_switch) == 0 ){

        // lightUp(&strip_a);
        // oneLevelUp(&strip_a, 50);
        oneLevelUp2(&strip_a, 50);
        if(buzz == 1){
          doBuzz(1);
        }
        delay(500);
        stripClearShow(&strip_a);      

      }

      if( digitalRead(right_switch) == 0 ){

        // oneLevelUp(&strip_b, 50);
        oneLevelUp2(&strip_b, 50);
        if(buzz == 1){
          doBuzz(1);
        }
        delay(500);
        stripClearShow(&strip_b);
        
      }    

      // at the end reset the hit count for the next time we enter
      
    }
    else if(hit_count >= 8){
      hit_count = 0;
    }
  
  }
  // ELSE JUST DO REGULAR ON/OFF
  else if(mode == 0){
    shimmerCascade2(10, 100);
    alternateRedWhite(100, 5);
    theaterChase2(100);  
    theaterChase4(25);
    theaterChase6(25);
    stripClearShow(&strip_a);
    stripClearShow(&strip_b);


  }

}

void lightUp(Adafruit_NeoPixel *strip){

  hit_count++; // this is a normal routine, increment the hit counter
  strip->clear();
  
  // Loop over the first half of the strip
  for (int i = 0; i < NUMPIXELS / 2; i++) {
    int oppositeIndex = NUMPIXELS - 1 - i;  // Calculate the corresponding pixel on the other end
    
    if(i == 11 || i == 12){
      // Pixels 11 and 12 are the eyes, set them to red
      strip->setPixelColor(i, 255, 0, 0, 0);
      strip->setPixelColor(oppositeIndex, 255, 0, 0, 0);

    }
    else{
      // The other pixels are the body, set those to white or some other color
      strip->setPixelColor(i, 0, 0, 0, 255);
      strip->setPixelColor(oppositeIndex, 0, 0, 0, 255);

    }
    
    // Show the updated strip
    strip->show();
    
    int delayTime = calculateDelay(i, NUMPIXELS, MIN_DELAY, MAX_DELAY); // Calculate delay    
    delay(delayTime);

  }

}

void lightDown(Adafruit_NeoPixel *strip){
  // Loop in reverse, starting from the top pair (middle of the strip)
  for (int i = (NUMPIXELS / 2) - 1; i >= 0; i--) {
    int oppositeIndex = NUMPIXELS - 1 - i;  // Calculate the corresponding pixel on the other end

    // Turn off both pixels in the pair
    strip->setPixelColor(i, 0, 0, 0, 0);
    strip->setPixelColor(oppositeIndex, 0, 0, 0, 0);

    strip->show();

    int delayTime = calculateDelay(i, (NUMPIXELS / 2) - 1, MIN_DELAY, MAX_DELAY / 2);
    delay(delayTime);  // Apply the calculated delay    
  }
}

void lightDownRed2(Adafruit_NeoPixel *strip){
  // Loop in reverse, starting from the top pair (middle of the strip)
  for (int i = (NUMPIXELS / 2) - 1; i >= 0; i--) {
    int oppositeIndex = NUMPIXELS - 1 - i;  // Calculate the corresponding pixel on the other end

    // Turn off both pixels in the pair
    strip->setPixelColor(i, 255, 0, 0, 0);
    strip->setPixelColor(oppositeIndex, 255, 0, 0, 0);

    strip->show();

    int delayTime = calculateDelay(i, (NUMPIXELS / 2) - 1, MIN_DELAY, MAX_DELAY / 2);
    delay(delayTime);  // Apply the calculated delay    
  }
}

void lightUpInPairs() {
  strip_a.clear();
  strip_b.clear();  // Turn off all pixels before starting
  
  // GO UP
  // Loop over the first half of the strip
  for (int i = 0; i < NUMPIXELS / 2; i++) {
    int oppositeIndex = NUMPIXELS - 1 - i;  // Calculate the corresponding pixel on the other end
    
    if(i == 11 || i == 12){
      // Pixels 11 and 12 are the eyes, set them to red
      strip_a.setPixelColor(i, 255, 0, 0, 0);
      strip_a.setPixelColor(oppositeIndex, 255, 0, 0, 0);

      strip_b.setPixelColor(i, 255, 0, 0, 0); 
      strip_b.setPixelColor(oppositeIndex, 255, 0, 0, 0);
    }
    else{
      // The other pixels are the body, set those to white or some other color
      strip_a.setPixelColor(i, 0, 0, 0, 255);
      strip_a.setPixelColor(oppositeIndex, 0, 0, 0, 255);

      strip_b.setPixelColor(i, 0, 0, 0, 255);  // Set pixel i to red
      strip_b.setPixelColor(oppositeIndex, 0, 0, 0, 255);
    }
    
    // Show the updated strip
    strip_a.show();
    strip_b.show();
    
    int delayTime = calculateDelay(i, NUMPIXELS, MIN_DELAY, MAX_DELAY); // Calculate delay    
    delay(delayTime);

  }

}

void turnOffInPairs() {
  // Loop in reverse, starting from the top pair (middle of the strip)
  for (int i = (NUMPIXELS / 2) - 1; i >= 0; i--) {
    int oppositeIndex = NUMPIXELS - 1 - i;  // Calculate the corresponding pixel on the other end

    // Turn off both pixels in the pair
    strip_a.setPixelColor(i, 0, 0, 0, 0);
    strip_a.setPixelColor(oppositeIndex, 0, 0, 0, 0);

    strip_b.setPixelColor(i, 0, 0, 0, 0);
    strip_b.setPixelColor(oppositeIndex, 0, 0, 0, 0);

    strip_a.show();
    strip_b.show();

    int delayTime = calculateDelay(i, (NUMPIXELS / 2) - 1, MIN_DELAY, MAX_DELAY / 2);
    delay(delayTime);  // Apply the calculated delay    
  }
}



void doBuzz(int nTimes){
  
  for (int i=0; i < nTimes; i++){
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW); 
    delay(100);
  }

}

int calculateDelay(int i, int maxIndex, int minDelay, int maxDelay) {
  // Quadratic scaling from i=0 to maxIndex
  float scale = float(i) / maxIndex;        // Normalize i to a range between 0 and 1
  float scaledDelay = minDelay + (scale * scale) * (maxDelay - minDelay);  // Quadratic curve
  return int(scaledDelay);
}


void initiateStrip(Adafruit_NeoPixel *strip){

  strip->begin();
  strip->clear();
  strip->show(); 

}

void stripClearShow(Adafruit_NeoPixel *strip){

  strip->clear();
  strip->show(); 

}


void theaterChase(Adafruit_NeoPixel *strip, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip->clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip->numPixels(); c += 3) {
        strip->setPixelColor(c, strip->Color(225, 225, 225, 0)); // Set pixel 'c' to value 'color'
      }
      strip->show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}


// chaos on the board
void theaterChase2(int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip_a.clear();         //   Set all pixels in RAM to 0 (off)
      strip_b.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c < NUMPIXELS; c += 3) {
        strip_a.setPixelColor(c, strip_a.Color(255, 0, 0, 0)); // Set pixel 'c' to value 'color'
        strip_b.setPixelColor(c, strip_b.Color(225, 0, 0, 0)); // Set pixel 'c' to value 'color'
      }
      strip_a.show(); // Update strip with new contents
      strip_b.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void theaterChase3(int wait) {
  for (int a = 0; a < 10; a++) {  // Repeat the chase 10 times
    for (int b = 0; b < NUMPIXELS; b++) {  // 'b' will move the band across the strip

      // Clear both strips before setting the next band of LEDs
      strip_a.clear();
      strip_b.clear();

      // Middle LED (full brightness)
      strip_a.setPixelColor(b, strip_a.Color(255, 0, 225, 0));  // Full power pinkish color
      strip_b.setPixelColor(b, strip_b.Color(225, 0, 225, 0));  // Full power for strip_b
      
      // LED before the middle (50% brightness) - only if it exists
      if (b - 1 >= 0) {
        strip_a.setPixelColor(b - 1, strip_a.Color(128, 0, 113, 0));  // Half power for previous LED
        strip_b.setPixelColor(b - 1, strip_b.Color(113, 0, 113, 0));  // Half power for previous LED
      }
      
      // LED after the middle (50% brightness) - only if it exists
      if (b + 1 < NUMPIXELS) {
        strip_a.setPixelColor(b + 1, strip_a.Color(128, 0, 113, 0));  // Half power for next LED
        strip_b.setPixelColor(b + 1, strip_b.Color(113, 0, 113, 0));  // Half power for next LED
      }

      // Show the updates on both strips
      strip_a.show();
      strip_b.show();

      // Delay before moving the "band" of LEDs to the next position
      delay(wait);
    }
  }
}


void theaterChase4(int wait) {
  for (int a = 0; a < 5; a++) {  // Repeat the chase 10 times
    for (int b = 0; b < NUMPIXELS; b++) {  // 'b' moves the band across the strip

      // Clear both strips before setting the next band of LEDs
      strip_a.clear();
      strip_b.clear();

      // Middle two LEDs (full brightness)
      strip_a.setPixelColor(b, strip_a.Color(255, 0, 0, 0));  // Full power for center LED
      strip_b.setPixelColor(b, strip_b.Color(225, 0, 0, 0));  // Full power for center LED

      // The next LED next to the center (full brightness)
      if (b + 1 < NUMPIXELS) {
        strip_a.setPixelColor(b + 1, strip_a.Color(255, 0, 0, 0));  // Full power
        strip_b.setPixelColor(b + 1, strip_b.Color(225, 0, 0, 0));  // Full power
      }

      // LEDs just next to the middle (66% brightness)
      if (b - 1 >= 0) {
        strip_a.setPixelColor(b - 1, strip_a.Color(170, 0, 0, 0));  // 66% brightness
        strip_b.setPixelColor(b - 1, strip_b.Color(170, 0, 0, 0));  // 66% brightness
      }
      if (b + 2 < NUMPIXELS) {
        strip_a.setPixelColor(b + 2, strip_a.Color(170, 0, 0, 0));  // 66% brightness
        strip_b.setPixelColor(b + 2, strip_b.Color(170, 0, 0, 0));  // 66% brightness
      }

      // Outer LEDs (33% brightness)
      if (b - 2 >= 0) {
        strip_a.setPixelColor(b - 2, strip_a.Color(85, 0, 0, 0));    // 33% brightness
        strip_b.setPixelColor(b - 2, strip_b.Color(85, 0, 0, 0));    // 33% brightness
      }
      if (b + 3 < NUMPIXELS) {
        strip_a.setPixelColor(b + 3, strip_a.Color(85, 0, 0, 0));    // 33% brightness
        strip_b.setPixelColor(b + 3, strip_b.Color(85, 0, 0, 0));    // 33% brightness
      }

      // Show the updates on both strips
      strip_a.show();
      strip_b.show();

      // Delay before moving the "band" of LEDs to the next position
      delay(wait);
    }
  }
}

void theaterChase6(int wait) {
  for (int a = 0; a < 5; a++) {  // Repeat the chase 10 times

    // Step 1: Chase up strip A
    for (int b = 0; b < NUMPIXELS; b++) {
      strip_a.clear();
      strip_b.clear();  // Clear strip B since we're currently only lighting strip A

      // Middle LEDs (full brightness)
      strip_a.setPixelColor(b, strip_a.Color(255, 0, 0, 0));
      if (b + 1 < NUMPIXELS) {
        strip_a.setPixelColor(b + 1, strip_a.Color(255, 0, 0, 0));
      }

      // LEDs just next to the middle (66% brightness)
      if (b - 1 >= 0) {
        strip_a.setPixelColor(b - 1, strip_a.Color(170, 0, 0, 0));
      }
      if (b + 2 < NUMPIXELS) {
        strip_a.setPixelColor(b + 2, strip_a.Color(170, 0, 0, 0));
      }

      // Outer LEDs (33% brightness)
      if (b - 2 >= 0) {
        strip_a.setPixelColor(b - 2, strip_a.Color(85, 0, 0, 0));
      }
      if (b + 3 < NUMPIXELS) {
        strip_a.setPixelColor(b + 3, strip_a.Color(85, 0, 0, 0));
      }

      strip_a.show();
      strip_b.show();  // Keep strip B off during this step
      delay(wait);
    }

    // Step 2: Chase up strip B
    for (int b = 0; b < NUMPIXELS; b++) {
      strip_a.clear();  // Clear strip A since we're currently lighting strip B
      strip_b.clear();

      // Middle LEDs (full brightness)
      strip_b.setPixelColor(b, strip_b.Color(225, 0, 0, 0));
      if (b + 1 < NUMPIXELS) {
        strip_b.setPixelColor(b + 1, strip_b.Color(225, 0, 0, 0));
      }

      // LEDs just next to the middle (66% brightness)
      if (b - 1 >= 0) {
        strip_b.setPixelColor(b - 1, strip_b.Color(150, 0, 0, 0));
      }
      if (b + 2 < NUMPIXELS) {
        strip_b.setPixelColor(b + 2, strip_b.Color(150, 0, 0, 0));
      }

      // Outer LEDs (33% brightness)
      if (b - 2 >= 0) {
        strip_b.setPixelColor(b - 2, strip_b.Color(75, 0, 0, 0));
      }
      if (b + 3 < NUMPIXELS) {
        strip_b.setPixelColor(b + 3, strip_b.Color(75, 0, 0, 0));
      }

      strip_a.show();  // Keep strip A off during this step
      strip_b.show();
      delay(wait);
    }
  }
}


void oneLevelUp2(Adafruit_NeoPixel *strip, int wait) {
  hit_count++;
  int levels = NUMPIXELS / 2;  // Number of levels (pairs of LEDs)

  // Step through each level from bottom (0 and 23) to top (11 and 12)
  for (int i = 0; i < levels - 1; i++) {  // Exclude the final pair from this loop
    // Clear the current strip before lighting the next level
    strip->clear();

    // Light up the current level (both sides of strip_a)
    strip->setPixelColor(i, 0, 0, 0, 255);  // White for LED i
    strip->setPixelColor(NUMPIXELS - 1 - i, 0, 0, 0, 255);  // White for opposite LED

    // Show the changes on strip_a
    strip->show();
    delay(wait);  // Pause before moving to the next level
  }

  // Step 2: Light up the final two LEDs (the top two) in red
  strip->clear();  // Clear the strip before setting the final two LEDs
  strip->setPixelColor(levels - 1, 255, 0, 0, 0);  // Red for LED 11 (top)
  strip->setPixelColor(levels, 255, 0, 0, 0);  // Red for LED 12 (top)

  // Show the final red LEDs at the top
  strip->show();
}



void fillDownWithRed(Adafruit_NeoPixel *strip, int wait) {
  // int levels = NUMPIXELS;  // Number of levels (pairs of LEDs)
  int levels = NUMPIXELS / 2;  // Number of levels (pairs of LEDs)

  // Step 1: Start from the middle and progressively fill down to the edges
  for (int i = levels - 1; i >= 0; i--) {
    // Clear both strips before lighting the next pair
    strip->clear();

    // Light up all the LEDs from the middle to the current level in red
    for (int j = levels - 1; j >= i; j--) {
      strip->setPixelColor(j, 255, 0, 0, 0);  // Red for LED j on strip_a
    }

    // Show the changes on both strips
    strip->show();

    // Wait for the next step in the animation
    delay(wait);
  }
}

void shimmerCascade(int wait, int shimmerDuration) {
  int levels = NUMPIXELS / 2;  // Number of levels (pairs of LEDs)

  // Step 1: Set the entire board to solid red
  for (int i = 0; i < levels; i++) {
    strip_a.setPixelColor(i, strip_a.Color(255, 0, 0));  // Red for LED i on strip_a
    strip_b.setPixelColor(NUMPIXELS - 1 - i, strip_b.Color(255, 0, 0));  // Red for LED (NUMPIXELS-1-i) on strip_b
  }

  // Show the entire board with solid red
  strip_a.show();
  strip_b.show();
  delay(wait);  // Hold the solid red state before starting shimmer

  // Step 2: Start shimmering from the middle and cascade down
  unsigned long startTime;
  for (int i = levels - 1; i >= 0; i--) {
    startTime = millis();
    
    // Continuously shimmer all levels from the current one and above
    while (millis() - startTime < shimmerDuration) {
      // Shimmer all LEDs from the current level and up
      for (int j = levels - 1; j >= i; j--) {
        // Toggle the LEDs on
        strip_a.setPixelColor(j, strip_a.Color(255, 0, 0));  // Turn red on for LED j on strip_a
        strip_b.setPixelColor(NUMPIXELS - 1 - j, strip_b.Color(255, 0, 0));  // Turn red on for LED on strip_b

        // Show the changes
        strip_a.show();
        strip_b.show();
      }
      delay(100);  // On-time for shimmer

      // Turn off the LEDs for the shimmer
      for (int j = levels - 1; j >= i; j--) {
        strip_a.setPixelColor(j, strip_a.Color(0, 0, 0));  // Turn off LED j on strip_a
        strip_b.setPixelColor(NUMPIXELS - 1 - j, strip_b.Color(0, 0, 0));  // Turn off LED on strip_b

        // Show the changes
        strip_a.show();
        strip_b.show();
      }
      delay(100);  // Off-time for shimmer
    }

    // After the shimmer period, leave all LEDs solid red
    for (int j = levels - 1; j >= i; j--) {
      strip_a.setPixelColor(j, strip_a.Color(255, 0, 0));  // Solid red for LED j on strip_a
      strip_b.setPixelColor(NUMPIXELS - 1 - j, strip_b.Color(255, 0, 0));  // Solid red for LED on strip_b
    }

    // Show the final state of the LEDs after shimmering
    strip_a.show();
    strip_b.show();
  }
}
// alternate count is the num of cycles
void alternateRedWhite(int wait, int alternateCount) {
  int levels = NUMPIXELS / 2;  // Number of levels (pairs of LEDs)

  // Step 1: Set the entire board to solid red
  for (int i = 0; i < NUMPIXELS; i++) {
    strip_a.setPixelColor(i, strip_a.Color(255, 0, 0));  // Red for all LEDs on strip_a
    strip_b.setPixelColor(i, strip_b.Color(255, 0, 0));  // Red for all LEDs on strip_b
  }

  // Show the entire board with solid red
  strip_a.show();
  strip_b.show();
  delay(wait);  // Hold the solid red state before starting the alternation

  // Step 2: Alternating between red and white for both strips
  for (int count = 0; count < alternateCount; count++) {
    // State 1: Both strips' left half (0-11) are red, right half (12-23) are white
    for (int i = 0; i < levels; i++) {
      strip_a.setPixelColor(i, strip_a.Color(255, 0, 0, 0));    // Red for LED i (left half)
      strip_a.setPixelColor(NUMPIXELS - 1 - i, strip_a.Color(0, 0, 0, 255));  // White for LED (right half)
      
      strip_b.setPixelColor(i, strip_b.Color(255, 0, 0, 0));    // Red for LED i (left half)
      strip_b.setPixelColor(NUMPIXELS - 1 - i, strip_b.Color(0, 0, 0, 255));  // White for LED (right half)
    }
    strip_a.show();
    strip_b.show();
    delay(wait);

    // State 2: Both strips' left half (0-11) are white, right half (12-23) are red
    for (int i = 0; i < levels; i++) {
      strip_a.setPixelColor(i, strip_a.Color(0, 0, 0, 255));  // White for LED i (left half)
      strip_a.setPixelColor(NUMPIXELS - 1 - i, strip_a.Color(255, 0, 0));  // Red for LED (right half)
      
      strip_b.setPixelColor(i, strip_b.Color(0, 0, 0, 255));  // White for LED i (left half)
      strip_b.setPixelColor(NUMPIXELS - 1 - i, strip_b.Color(255, 0, 0));  // Red for LED (right half)
    }
    strip_a.show();
    strip_b.show();
    delay(wait);
  }
}



void shimmerCascade2(int wait, int shimmerDuration) {
  int levels = NUMPIXELS / 2;  // Number of levels (pairs of LEDs)

  // Step 1: Set the entire board to solid red
  for (int i = 0; i < NUMPIXELS; i++) {
    strip_a.setPixelColor(i, strip_a.Color(255, 0, 0));  // Red for all LEDs on strip_a
    strip_b.setPixelColor(i, strip_b.Color(255, 0, 0));  // Red for all LEDs on strip_b
  }

  // Show the initial red state on both strips
  strip_a.show();
  strip_b.show();
  delay(wait);  // Hold this red state for a moment

  // Step 2: Start shimmering from the middle and cascade down
  for (int i = levels - 1; i >= 0; i--) {
    unsigned long startTime = millis();
    
    // Shimmer the current level for the duration specified
    while (millis() - startTime < shimmerDuration) {
      // Randomly turn on or off the LEDs in the current level and above
      for (int j = 0; j < NUMPIXELS; j++) {
        // Randomly decide if LED should be on or off
        bool shimmerOn = random(0, 2);  // Generates 0 or 1 randomly

        if (shimmerOn) {
          strip_a.setPixelColor(j, strip_a.Color(255, 0, 0));  // Turn red on for LED j on strip_a
          strip_b.setPixelColor(j, strip_b.Color(255, 0, 0));  // Turn red on for LED j on strip_b
        } else {
          strip_a.setPixelColor(j, strip_a.Color(0, 0, 0));  // Turn off LED j on strip_a
          strip_b.setPixelColor(j, strip_b.Color(0, 0, 0));  // Turn off LED j on strip_b
        }
      }

      // Show the updates on both strips
      strip_a.show();
      strip_b.show();

      // Delay for a short time before the next shimmer update
      delay(100);  // Adjust this delay for the shimmer speed
    }
  }
}

