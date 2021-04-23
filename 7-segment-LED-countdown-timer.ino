// The Dogg 2021
// Date countdown timer on 7 segment digit. 
// The following calculates the remaining working days from the 22nd April 2021 to 30th June 2021

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define PIXELS_PER_SEGMENT  2     // Number of LEDs in each Segment
#define PIXELS_DIGITS       2     // Number of connected Digits 
#define PIXELS_PIN          2     // GPIO Pin
#define LOGO_PIN            3     // GPIO Pin for logo 
#define LOGO_NUMBER_PIXEL   6     // Number of LED for logo

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS_PER_SEGMENT * 7 * PIXELS_DIGITS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel logostrip = Adafruit_NeoPixel(LOGO_NUMBER_PIXEL, LOGO_PIN, NEO_GRB + NEO_KHZ800);

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

//Clear all the Pixels
void clearDisplay() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    logostrip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  logostrip.show();
}

void setup() {
  strip.begin();
  logostrip.begin();
  readTime();

}

void loop() {

  // display LED in logo strip
  logoStrip();

  // calculate the number of remaining days to 30th June 2021
  calDays();

  // split two digits into single digits
  int REM = calDays();
  int one = REM / 10;
  int two = REM % 10;
  
  // display LED digits
  disp_Digit1(one);
  disp_Digit2(two);

}

// debug to test if RTC time is connected
void readTime() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
}

// powers the logo battery box. 6 is the number of LED's powering the branding box.
void logoStrip() {
  for (int i = 0; i < 6; i++) {
    logostrip.setPixelColor(i, 50, 250, 50);
  }
  logostrip.show();
  delay(1000);

}

//display single and double digits, this is for the first digit.
void disp_Digit1(int num) {
  //clearDisplay();
  writeDigit(0, num);
  strip.show();
}

//display single and double digits, this is for the second digit, add more for each additional digit. 
void disp_Digit2(int num) {
  //clearDisplay();
  writeDigit(1, num);
  strip.show();
}

// calculates the remaining working days from 22nd April 2021 to 30th June 2021.
int calDays() {

  tmElements_t tm;
  RTC.read(tm);

  int todayDay = tm.Day;
  int todayMonth = tm.Month;
  //int todayDay = 29;
  //int todayMonth = 6;
  int todayYear = tmYearToCalendar(tm.Year);
  int date1;
  int date2;
  if (todayMonth == 4) {        // for the month of April
    Serial.println("**********");
    Serial.print("Day is ");
    Serial.println(todayDay);
    Serial.print("Month is ");
    Serial.println(todayMonth);
    if (todayDay >= 19 && todayDay <= 23) {
      date1 = (30 - todayDay) - 2;
      date2 = date1 + 43;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else if (todayDay >= 26 && todayDay <= 30 ) {
      date1 = (30 - todayDay);
      date2 = date1 + 43;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else {
      Serial.println("Weekend");
    }
  }
  else if (todayMonth == 5) {       // for the month of May
    Serial.println("**********");
    Serial.print("Day is ");
    Serial.println(todayDay);
    Serial.print("Month is ");
    Serial.println(todayMonth);
    if (todayDay >= 3 && todayDay <= 7) {
      date1 = (31 - todayDay) - 8;
      date2 = date1 + 22;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else if (todayDay >= 10 && todayDay <= 14 ) {
      date1 = (31 - todayDay) - 6;
      date2 = date1 + 22;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else if (todayDay >= 17 && todayDay <= 21 ) {
      date1 = (31 - todayDay) - 4;
      date2 = date1 + 22;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else if (todayDay >= 24 && todayDay <= 28 ) {
      date1 = (31 - todayDay) - 2;
      date2 = date1 + 22;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else if (todayDay == 31) {
      date1 = (31 - todayDay);
      date2 = date1 + 22;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else {
      Serial.println("Weekend");
    }
  }
  else if (todayMonth == 6) {     // for the month of june
    Serial.println("**********");
    Serial.print("Day is ");
    Serial.println(todayDay);
    Serial.print("Month is ");
    Serial.println(todayMonth);
    if (todayDay >= 1 && todayDay <= 4) {
      date1 = (30 - todayDay) - 8;
      date2 = date1;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else if (todayDay >= 7 && todayDay <= 11 ) {
      date1 = (30 - todayDay) - 6;
      date2 = date1;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else if (todayDay >= 14 && todayDay <= 18 ) {
      date1 = (30 - todayDay) - 4;
      date2 = date1;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else if (todayDay >= 21 && todayDay <= 25 ) {
      date1 = (30 - todayDay) - 2;
      date2 = date1;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else if (todayDay >= 28 && todayDay <= 30) {
      date1 = (30 - todayDay);
      date2 = date1;
      Serial.print("DAYSREM = ");
      Serial.println(date2);
      return date2;
    }
    else {
      Serial.println("Weekend");
    }
  }
}

// power each digit
void writeDigit(int index, int val) {
  byte digit = digits[val];
  for (int i = 6; i >= 0; i--) {
    int offset = index * (PIXELS_PER_SEGMENT * 7) + i * PIXELS_PER_SEGMENT;
    
    uint32_t color;
    if (digit & 0x01 != 0) {
      if (val == 1) color = strip.Color(0, 50, 50);
      if (val == 2) color = strip.Color(0, 50, 50);
      if (val == 3) color = strip.Color(0, 50, 50);
      if (val == 4) color = strip.Color(0, 50, 50);
      if (val == 5) color = strip.Color(0, 50, 50);
      if (val == 6) color = strip.Color(0, 50, 50);
      if (val == 7) color = strip.Color(0, 50, 50);
      if (val == 8) color = strip.Color(0, 50, 50);
      if (val == 9) color = strip.Color(0, 50, 50);
      if (val == 0) color = strip.Color(0, 50, 50);
    }
    else
      color = strip.Color(0, 0, 0);

    int j = offset;
    for (int j = offset; j < offset + PIXELS_PER_SEGMENT; j++) {
      strip.setPixelColor(j, color);
    }
    digit = digit >> 1;
  }
}
