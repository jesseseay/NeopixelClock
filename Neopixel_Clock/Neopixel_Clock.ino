/* in terminal: git commit -am "message" [enter]
 *  commits to github
 *  git commit
 *  git push
 */



#include <Wire.h>


#include <Adafruit_NeoPixel.h>

#define PIN 2    
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
// NEO_KHZ800 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
// NEO_KHZ400 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
// NEO_GRB Pixels are wired for GRB bitstream (most NeoPixel products)
// NEO_RGB Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);


uint32_t black = strip.Color(0, 0, 0); //0
uint32_t brown = strip.Color(100, 52, 19); // 1 pink
//uint32_t brown = strip.Color(109, 83, 16); // 1

uint32_t red = strip.Color(136, 0, 0); // 2
uint32_t orange = strip.Color(136, 35, 0); // 3
uint32_t yellow = strip.Color(120, 90, 0); // 4
uint32_t green = strip.Color(10, 100, 10);// 5
uint32_t blue = strip.Color(0, 0, 67); // 6
uint32_t violet = strip.Color(62, 0, 80); // 7
uint32_t grey = strip.Color(36, 49, 48); //  8
uint32_t white = strip.Color(128, 128, 128); // 9

uint32_t colorcode[] = {black, brown, red, orange, yellow, green, blue, violet, grey, white};

int Ho = 0;
int Ur = 0;
int Mi = 0;
int Nu = 0;

void setup() {
  strip.begin();
  strip.show();                // initialize all pixels to 'off'

  Wire.begin();
  Serial.begin(9600);

  // clear /EOSC bit
  // Sometimes necessary to ensure that the clock
  // keeps running on just battery power. Once set,
  // it shouldn't need to be reset but it's a good
  // idea to make sure.
  Wire.beginTransmission(0x68); // address DS3231
  Wire.write(0x0E); // select register
  Wire.write(0b00011100); // write register bitmap, bit 7 is /EOSC
  Wire.endTransmission();
}

void loop()
{
  // send request to receive data starting at register 0
  Wire.beginTransmission(0x68); // 0x68 is DS3231 device address
  Wire.write((byte)0); // start at register 0
  Wire.endTransmission();
  Wire.requestFrom(0x68, 3); // request three bytes (seconds, minutes, hours)

  while (Wire.available())
  {
    int seconds = Wire.read(); // get seconds
    int minutes = Wire.read(); // get minutes
    int hours = Wire.read();   // get hours

    seconds = (((seconds & 0b11110000) >> 4) * 10 + (seconds & 0b00001111)); // convert BCD to decimal
    minutes = (((minutes & 0b11110000) >> 4) * 10 + (minutes & 0b00001111)); // convert BCD to decimal
    hours = (((hours & 0b00100000) >> 5) * 20 + ((hours & 0b00010000) >> 4) * 10 + (hours & 0b00001111)); // convert BCD to decimal (assume 24 hour mode)

    Serial.print(hours); Serial.print(":"); Serial.print(minutes); Serial.print(":"); Serial.println(seconds);

    delay(10);

    Ho = hours / 10;
    strip.setPixelColor(0, colorcode[Ho]);

    Ur = hours % 10;
    strip.setPixelColor(1, colorcode[Ur]);

    Mi = minutes / 10;
    strip.setPixelColor(2, colorcode[Mi]);

    Nu = minutes % 10;
    strip.setPixelColor(3, colorcode[Nu]);

    strip.show();

    delay(10);

  }
}


