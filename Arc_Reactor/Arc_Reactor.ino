#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 2         // LED Pin on controller //2 (D2) on Nano //1 on Gemma
#define WAIT 25
#define NUM_PIXELS 38
#define BUTTON_PIN1 5
#define BUTTON_PIN2 7
#define DECREASE_BRIGHTNESS 150

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
int last_state;             // Previous Color Switch State
int curr_state;             // Current Color Switch
int brightness_pad;         // Value used to Switch between Brightness settings
uint32_t ironBlue = strip.Color(5, 10, 50);
uint32_t ironRed = strip.Color(50, 2, 0);
uint32_t ironWhite = strip.Color(50, 50, 50);
uint32_t ironYellow = strip.Color(50, 5, 0);

void setup() {
  // Sets the Neopixel internal strip brightness
  //    Custom "set_brightness()" function defined below
  //    to dynamically set color values brightness
  strip.begin();
  strip.setBrightness(200);
  strip.show(); 
  
  // Button Setup
  pinMode(BUTTON_PIN1, INPUT_PULLUP);   // Color Switch
  pinMode(BUTTON_PIN2, INPUT_PULLUP);   // Brightness Switch

  // Seed Random with a number from unconnected pin
  randomSeed(analogRead(0));

  // Color-State Switch, opening animation
  last_state = digitalRead(BUTTON_PIN1);
  if (last_state == LOW) {
      colorWipe(ironBlue, WAIT*1.5);
    }
  else {
    colorWipe(ironRed, WAIT*1.5);
    }
}

void loop() {
  int random_wait = random(300, 5000);
  curr_state = digitalRead(BUTTON_PIN1);

  // Color is set to Blue
  if (curr_state == LOW) {
    if (curr_state != last_state) {
      colorWipe(ironBlue, WAIT);
      last_state = curr_state;
      }
    blueBreathe(random_wait);

    // Long enough hold on Blue triggers a White animation
    if (random_wait > 4000){
        // Check if color or brightness switch state has changed
        set_brightness();
        curr_state = digitalRead(BUTTON_PIN1);
        if (curr_state == last_state){whiteBreathe(random_wait);}
        else {
            // Color has changed
            colorWipe(ironRed, WAIT);
            yellowBreathe(random_wait);
          }
      }
    }
  // Color is set to Red
  else {
    if (curr_state != last_state) {
      colorWipe(ironRed, WAIT);
      last_state = curr_state;
      }
    redBreathe(random_wait);

    // Long enough hold on Red triggers a Yellow animation
    if (random_wait > 4000){
      // Check if color or brightness switch state has changed
      set_brightness();
      curr_state = digitalRead(BUTTON_PIN1);
      if (curr_state == last_state){yellowBreathe(random_wait);}
      else {
            // Color has changed
            colorWipe(ironBlue, WAIT);
            whiteBreathe(random_wait);
          }
    }
  }
  last_state = curr_state;
} //END LOOP

void blueBreathe(int random_wait) {
  set_brightness();
  blueFadeUp();
  delay(random_wait);
  blueFadeDown();
}

void whiteBreathe(int random_wait) {
  uint32_t c;
  int r = 5;
  int g = 10;
  int b = 50;

  // Shift Color from Light-Blue to White
  for (int v = 0; v < 50; v++){
    if (r < 50) {r++;}
    if (g < 50) {g++;}
    c = strip.Color(r, g, b);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(1);
  }
  
  // Fade up White
  set_brightness();
  whiteFadeUp();
  delay(random_wait);
  
  // Random Animation Select
  set_brightness();
  int mod = random_wait % 3;
  switch(mod) {
    case 0:
      for (int repeat = 6; repeat>0; repeat--){
        whiteFadeDown();
        delay(2);
        whiteFadeUp();
        delay(2);
      }
      break;
    case 1:
      for (int repeat = 5; repeat>0; repeat--){
        colorWipe(strip.Color(255/10, 255/5, 255-brightness_pad), WAIT);
        colorWipe(strip.Color(255-brightness_pad, 255-brightness_pad, 255-brightness_pad), WAIT);
      }
      break;
    case 2:
      two_color(strip.Color(255-brightness_pad, 255-brightness_pad, 255-brightness_pad), strip.Color((255-brightness_pad)/10, (255-brightness_pad)/5, 255-brightness_pad), WAIT);
      colorWipe(strip.Color(255-brightness_pad, 255-brightness_pad, 255-brightness_pad), WAIT);
      break;
  }
  delay(1000);
  whiteFadeDown();

  // Shift Color from White to Light-Blue
  for (int v = 255; v > 0; v--){
    if (r > 5) {r--;}
    if (g > 10) {g--;}
    if (b > 50) {b--;}
    c = strip.Color(r, g, b);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(1);
  }
}


void redBreathe(int random_wait) {
  uint32_t c;
  set_brightness();
  redFadeUp();
  delay(random_wait);
  redFadeDown();
}

void yellowBreathe(int random_wait) {
  uint32_t c;
  int r = 50;
  int g = 5;
  int b = 0;

  // Shift Color from Red to Yellow
  for (int v = 0; v < 100; v++){
    if (r < 100) {r++;}
    if (g < 50) {g++;}
    c = strip.Color(r, g, 0);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(1);
  }
  
  // Fade up Yellow
  set_brightness();
  yellowFadeUp();
  delay(random_wait);

  // Random Animation Select
  set_brightness();
  int mod = random_wait % 3;
  switch(mod) {
    case 0:
      for (int repeat = 6; repeat>0; repeat--){
        yellowFadeDown();
        delay(2);
        yellowFadeUp();
        delay(2);
      }
      break;
    case 1:
      for (int repeat = 5; repeat>0; repeat--){
        colorWipe(strip.Color((255-brightness_pad), (255-brightness_pad)/20, 0), WAIT);
        colorWipe(strip.Color(255-(brightness_pad/2), (255-(brightness_pad/2))/2, 0), WAIT);
      }
      break;
    case 2:
      two_color(strip.Color(255-(brightness_pad/2), (255-(brightness_pad/2))/2, 0), strip.Color((255-brightness_pad), (255-brightness_pad)/20, 0), WAIT);
      colorWipe(strip.Color(255-(brightness_pad/2), (255-(brightness_pad/2))/2, 0), WAIT);
      break;
  }
  delay(1000);
  yellowFadeDown();

  // Shift Color from Yellow to Red
  for (int v = 255; v > 0; v--){
    if (r > 50) {r--;}
    if (g > 5) {g--;}
    c = strip.Color(r, g, 0);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(1);
    }
}

void set_brightness() {
  if (digitalRead(BUTTON_PIN2) == HIGH) {
      brightness_pad = DECREASE_BRIGHTNESS;
    }
  else {
      brightness_pad = 0;
    }
  }

void blueFadeUp() {
  uint32_t c;
  for (int b = 50; b < 255-brightness_pad; b++){
    c = strip.Color(b/10, b/5, b);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(1);
    }
  }

void blueFadeDown() {
  uint32_t c;
  for (int b = 255-brightness_pad; b > 50; b--){
    c = strip.Color(b/10, b/5, b);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(1);
    }
  }

void whiteFadeUp() {
  uint32_t c;
  for (int b = 50; b < 150-brightness_pad; b++){
    c = strip.Color(b, b, b);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(2);
    }
  }

void whiteFadeDown() {
  uint32_t c;
  for (int b = 150-brightness_pad; b > 50; b--){ 
    c = strip.Color(b, b, b);
    for(uint16_t i=0; i<strip.numPixels(); i++){
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(2);
    }
  }

void redFadeUp() {
  uint32_t c;
  for (int r = 50; r < 255-brightness_pad; r++){
    c = strip.Color(r, r/20, 0);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(1);
    }
  }

void redFadeDown() {
  uint32_t c;
  for (int r = 255-brightness_pad; r > 50; r--){
    c = strip.Color(r, r/20, 0);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(1);
    }
  }

void yellowFadeUp() {
  uint32_t c;
  for (int y = 100; y < 255-(brightness_pad/2); y++) {
    c = strip.Color(y, y/2, 0);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(2);
    }
  }

void yellowFadeDown() {
  uint32_t c;
  for (int y = 255-(brightness_pad/2); y > 100; y--) {
    c = strip.Color(y, y/2, 0);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(2);
    }
  }

void two_color(uint32_t color1, uint32_t color2, uint8_t wait) {
  wait = wait * 2;
  strip.show();
  bool switch_color = true;
  for(int r=0; r<10; r++) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      if(i%3 == 0) {
          switch_color = !switch_color;
          strip.show();                          //  Update strip to match
          delay(100);                           //  Pause for a moment
        }
      if(switch_color == true) {
        strip.setPixelColor(i, color1);
        }
      else {
        strip.setPixelColor(i, color2);
        }
      }
    }
  }

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  set_brightness();
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
    }
  }

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
