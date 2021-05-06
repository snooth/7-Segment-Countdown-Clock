// The Dogg 2021
// 7 segment digit. 


#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define PIXELS_PER_SEGMENT  3     // Number of LEDs in each Segment
#define PIXELS_DIGITS       4     // Number of connected Digits 
#define PIXELS_PIN          2     // GPIO Pin
#define PIXELS_TOTAL        84    // total strip led
#define LOGO_PIN            6     // GPIO Pin for logo 
#define LOGO_NUMBER_PIXEL   60     // Number of LED for logo
#define TWODOT_PIN          4     // Two dots to make the time pin
#define TWODOT_PIXEL        2     // Number of LED for two dots


Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS_PER_SEGMENT * 7 * PIXELS_DIGITS, PIXELS_PIN, NEO_GRB + NEO_KHZ800); // for main time
Adafruit_NeoPixel logostrip = Adafruit_NeoPixel(LOGO_NUMBER_PIXEL, LOGO_PIN, NEO_GRB + NEO_KHZ800); // for logobox
Adafruit_NeoPixel twodot = Adafruit_NeoPixel(TWODOT_PIXEL, TWODOT_PIN, NEO_GRB + NEO_KHZ800); // for two dots to make up a time. 

//Pixel Arrangement
/*
          b
        a   c
          g
        f   d
          e
*/

// Segment array
byte segments[7] = {
  //abcdefg
  0b0000001,     // Segment g
  0b0000010,     // Segment f
  0b0000100,     // Segment e
  0b0001000,     // Segment d
  0b0010000,     // Segment c
  0b0100000,     // Segment b
  0b1000000      // Segment a
};

//Digits array
byte digits[10] = {
  //abcdefg
  0b1111110,     // 0
  0b0011000,     // 1
  0b0110111,     // 2
  0b0111101,     // 3
  0b1011001,     // 4
  0b1101101,     // 5
  0b1101111,     // 6
  0b0111000,     // 7
  0b1111111,     // 8
  0b1111001      // 9
};


// initiate everything
void setup() {
  strip.begin();
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
  logostrip.begin();
  logostrip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  twodot.begin();
  readTime();
}

// loop the clock 
void loop() {

  // display LED in logo strip
  //logoStrip();
  //logoStripRainbow(2); // 
  logoRainbow(10);

  // display LED in the twodots box
  twodots();
  //twoDotsRainbow(10);

  // test display on all digits
  //testDigits(2);

  // GET AND DISPLAY HOURS
  // get hour and split two digits into single digits
  int newHour = getHour();
  int hourOne = newHour / 10;
  int hourTwo = newHour % 10;

   // if hours is zero then don't display
  Serial.print("hourOne = ");
  Serial.println(hourOne);
  Serial.print("hourTwo = ");
  Serial.println(hourTwo);
  if (hourOne != 0) {
    // display LED digits
    clearDisplay2(); // clear all the pixels first
    disp_Digit1(hourOne);
    // display LED digits 2nd hour digit.
    disp_Digit2(hourTwo);
  }else {
    clearDisplay2(); // clear all the pixels first
    // display LED digits 2nd hour digit.
    disp_Digit2(hourTwo);
  }

  /*
  // for loop testing purposes ONLY
  for (int i = 15; i > 0; i--) {
    int a = i / 10;
    int b = i % 10;
    if (a != 0) {
      // display LED digits
      clearDisplay2(); // clear all the pixels first
      disp_Digit1(a);
      disp_Digit2(b);
    }else {
      clearDisplay2(); // clear all the pixels first
      disp_Digit2(b);
    }
    delay(1000);
    clearDisplay2();
  }
  */

  // GET AND DISPLAY MINUTES
  // get minute and split two digits into single digits
  int newMinute = getMinute();
  int minOne = newMinute / 10;
  int minTwo = newMinute % 10;
  disp_Digit3(minOne);
  disp_Digit4(minTwo);
}

//Clear all the Pixels
void clearDisplay2() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void logoRainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<logostrip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / logostrip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      logostrip.setPixelColor(i, logostrip.gamma32(logostrip.ColorHSV(pixelHue)));
    }
    logostrip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}


// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void twoDotsRainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<twodot.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / twodot.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      twodot.setPixelColor(i, logostrip.gamma32(twodot.ColorHSV(pixelHue)));
    }
    twodot.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void testDigits(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    //delay(wait);  // Pause for a moment
  }
}

// debug to test if RTC time is connected, display to serial monior
void readTime() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
}

// powers the logo battery box. 18 is the number of LED's powering the branding box.
void logoStrip() {
  for (int i = 0; i < LOGO_NUMBER_PIXEL; i++) {
    logostrip.setPixelColor(i, 50, 250, 50);
  }
  logostrip.show();
  //delay(1000);
}

//display single and double digits, this is for the first digit.
void disp_Digit1(int num) {
  clearDisplay2(); // clear all the pixels first
  writeDigit(0, num);
  strip.show();
}

//display single and double digits, this is for the second digit, add more for each additional digit. 
void disp_Digit2(int num) {
  //clearDisplay();
  writeDigit(1, num);
  strip.show();
}

//display single and double digits, this is for the first digit.
void disp_Digit3(int num) {
  //clearDisplay();
  writeDigit(2, num);
  strip.show();
}

//display single and double digits, this is for the second digit, add more for each additional digit. 
void disp_Digit4(int num) {
  //clearDisplay();
  writeDigit(3, num);
  strip.show();
}

// gets hour and RTC
int getHour() {

  tmElements_t tm;
  RTC.read(tm);

  int currentHour = tm.Hour;
  //int currentHour = 1;
  int currentMinute = tm.Minute;
  int todayDay = tm.Day;
  int todayMonth = tm.Month;
  Serial.print("Hours is ");
  Serial.println(currentHour);
  if (currentHour == 13) {        
    Serial.print("It's 1PM ");
    int newHour = 1;
    return newHour;
  }
  else if (currentHour == 14) {
    Serial.print("It's 2PM ");
    int newHour = 2;
    return newHour;
  }
  else if (currentHour == 15) {
    Serial.print("It's 3PM ");
    int newHour = 3;
    return newHour;
  }
  else if (currentHour == 16) {
    Serial.print("It's 4PM ");
    int newHour = 4;
    return newHour;
  }
  else if (currentHour == 17) {
    Serial.print("It's 5PM ");
    int newHour = 5;
    return newHour;
  }
  else if (currentHour == 18) {
    Serial.print("It's 6PM ");
    int newHour = 6;
    return newHour;
  }
  else if (currentHour == 19) {
    Serial.print("It's 7PM ");
    int newHour = 7;
    return newHour;
  }
  else if (currentHour == 20) {
    Serial.print("It's 8PM ");
    int newHour = 8;
    return newHour;
  }
  else if (currentHour == 21) {
    Serial.print("It's 9PM ");
    int newHour = 9;
    return newHour;
  }
  else if (currentHour == 22) {
    Serial.print("It's 10PM ");
    int newHour = 10;
    return newHour;
  }
  else if (currentHour == 23) {
    Serial.print("It's 11PM ");
    int newHour = 11;
    return newHour;
  }
  else if (currentHour == 24) {
    Serial.print("It's midnight ");
    int newHour = 01;
    return newHour;
  }
  else if (currentHour == 00) {
    Serial.print("It's midnight ");
    int newHour = 12;
    return newHour;
  }
  else {
    return currentHour;
  }
}

// gets minute and RTC
int getMinute() {

  tmElements_t tm;
  RTC.read(tm);

  int currentMinute = tm.Minute;
  //int currentMinute = 18; // debug to test digit
  Serial.print("Minute is ");
  Serial.println(currentMinute);
  return currentMinute;
}

// colors for the time and twodots
int r = 60;
int g = 179;
int b = 113;

// powers the twodots box. 2 is the number of LED's powering the branding box, and had two LEDS
void twodots() {
  for (int i = 0; i < TWODOT_PIXEL; i++) {
    twodot.setPixelColor(i, r, g, b);
  }
  twodot.show();
  //delay(1000);
}

// power each digit
void writeDigit(int index, int val) {
  byte digit = digits[val];
  for (int i = 6; i >= 0; i--) {
    int offset = index * (PIXELS_PER_SEGMENT * 7) + i * PIXELS_PER_SEGMENT;
    
    uint32_t color;
    if (digit & 0x01 != 0) {
      if (val == 1) color = strip.Color(r, g, b);
      if (val == 2) color = strip.Color(r, g, b);
      if (val == 3) color = strip.Color(r, g, b);
      if (val == 4) color = strip.Color(r, g, b);
      if (val == 5) color = strip.Color(r, g, b);
      if (val == 6) color = strip.Color(r, g, b);
      if (val == 7) color = strip.Color(r, g, b);
      if (val == 8) color = strip.Color(r, g, b);
      if (val == 9) color = strip.Color(r, g, b);
      if (val == 0) color = strip.Color(r, g, b);
    }
    else
      color = strip.Color(0, 0, 0);

    for (int j = offset; j < offset + PIXELS_PER_SEGMENT; j++) {
      strip.setPixelColor(j, color);
    }
    digit = digit >> 1;
  }
}
