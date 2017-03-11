/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

/*
   This sketch example demonstrates how the BMI160 on the
   Intel(R) Curie(TM) module can be used to read gyroscope data
*/

#include "CurieIMU.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif



#define SER//_DEB
#ifdef SER_DEB
#define SER_DEB_init
void ser_deb() {
//  // display tab-separated gyro x/y/z values
  Serial.print("a:\t");
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(ay);
  Serial.print("\t");
  Serial.print(az);
  Serial.print("\t");

  Serial.print("aMag:\t");
  Serial.print(aMag); 
  Serial.print("\t");

  Serial.print("c:\t"); 
  Serial.print("\t");
  Serial.print(cx);
  Serial.print("\t");
  Serial.print(cy);
  Serial.print("\t");
  Serial.print(cz);
  Serial.println();
}
#endif

#define PIN 6

#define NUM_LEDS 64

#define BRIGHTNESS 255

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS, PIN+1, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS, PIN+2, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUM_LEDS, PIN+3, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(NUM_LEDS, PIN+4, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(NUM_LEDS, PIN+5, NEO_GRBW + NEO_KHZ800);

float ax, ay, az;
const float aMax = 2;
const float aMagMax = sqrt(3*sq(aMax));
float aMag = 0.0;
int i = 0;
int j = 0;
int k = 0;
int l = 0;
int m = 0;
int n = 0;

uint8_t cx = 0,
        cy = 0,
        cz = 0;
        
void setup() {
#define SER_D//EB_init
#ifdef SER_DEB_init
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device
  Serial.println("Initializing IMU device...");
#endif

  CurieIMU.begin();

  // Set the accelerometer range to 250 degrees/second
  CurieIMU.setGyroRange(250);
  CurieIMU.setAccelerometerRate(1600);
  CurieIMU.setAccelerometerRange(aMax);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off
    
  strip2.setBrightness(BRIGHTNESS);
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off
    
  strip3.setBrightness(BRIGHTNESS);
  strip3.begin();
  strip3.show(); // Initialize all pixels to 'off
  
  strip4.setBrightness(BRIGHTNESS);
  strip4.begin();
  strip4.show(); // Initialize all pixels to 'off
}

uint8_t intensit = 0;
uint8_t increment = 1;

const unsigned int clrMax = 128;

bool first = true;

uint8_t mapXYtoI(uint8_t x, uint8_t y) {
  return (y + 8*x);
}

void loop() {

  if (first) {

    first = false;
    uint32_t colorval;
    
    colorval = strip.Color(intensit, 0, clrMax-intensit,0);
    //strip.setPixelColor(32, colorval);
    strip.setPixelColor(mapXYtoI(2,3), colorval);  
    strip.setPixelColor(28, colorval);
    strip.setPixelColor(35, colorval);  
    strip.setPixelColor(36, colorval);
    strip2.setPixelColor(0, colorval);  
    strip3.setPixelColor(0, 0, 0, 0, 10);
  
  //  uint32_t * pixels = (uint32_t *)strip.getPixels();
  
   // pixels[10] = strip.Color(0,30,0,0);
    
    if (intensit == clrMax) increment = -1;
    else if (intensit == 0) increment = 1;
  
    intensit += increment;
    delay(1);
    
    strip.show();
    strip2.show();
    strip3.show();
//  strip4.show();
//  strip5.show();
//  strip6.show();
//  
    
  }
  
  //gyroloop();
}

void gyroloop() {
  float gx, gy, gz; //scaled Gyro values

  // read gyro measurements from device, scaled to the configured range
  CurieIMU.readGyroScaled(gx, gy, gz);

  if (abs(gx) > 1) {
    cx = min(int(abs(gx)),255);
  } else if (cx >0) {
    cx--;
  }
  if (abs(gy) > 1) {
    cy = min(int(abs(gy)),255);
  } else if (cy >0) {
    cy--;
  }
  if (abs(gz) > 1) {
    cz = min(int(abs(gz)),255);
  } else if (cz >0) {
    cz--;
  }

  CurieIMU.readAccelerometerScaled(ax, ay, az);

  float aMag = sqrt(sq(ax)+sq(ay)+sq(az));
  if (aMag > 0) {

    aMag /= aMagMax;
    aMag *= 10;
  } else {
    aMag -= 1;
  }
  uint8_t i_aMag = int(aMag);
  
#ifdef SER_DEB
  ser_deb();
#endif

//  for (int i = 0; i < 64; i++) {
//    strip.setPixelColor(i, strip.Color(0,0,0,0));
//    strip2.setPixelColor(i, strip.Color(0,0,0,0));
//    strip3.setPixelColor(i, strip.Color(0,0,0,0));
//    strip4.setPixelColor(i, strip.Color(0,0,0,0));
//    strip5.setPixelColor(i, strip.Color(0,0,0,0));
//    strip6.setPixelColor(i, strip.Color(0,0,0,0));
//   }

//  Serial.println(intensit);

  uint32_t color = strip.Color(cx, cy, cz, i_aMag);
  for (int i = 0; i < 64; i++) {
    strip.setPixelColor(i, color);
    strip2.setPixelColor(i, color);
    strip3.setPixelColor(i, color);
    strip4.setPixelColor(i, color);
    strip5.setPixelColor(i, color);
    strip6.setPixelColor(i, color);
   }

  color = strip.Color(cz, cx, cy, i_aMag/2);
  strip.setPixelColor((i)%64, color);
  strip.setPixelColor((i)%64, color);
  strip2.setPixelColor((i)%64, color);
  strip3.setPixelColor((i)%64, color);
  strip4.setPixelColor((i)%64, color);
  strip5.setPixelColor((i)%64, color);  
  strip6.setPixelColor((i++)%64, color);


//  strip.setPixelColor((i)%64, color);
//  strip.setPixelColor((i)%64, color);
//  strip2.setPixelColor((i)%64, color);
//  strip3.setPixelColor((i)%64, color);
//  strip4.setPixelColor((i)%64, color);
//  strip5.setPixelColor((i)%64, color);  
//  strip6.setPixelColor((i++)%64, color);
 
 delay(100);
//  if (intensit == 40) increment = -1;
//  else if (intensit == 0) increment = 1;
//  intensit += increment;
  
  strip.show();
  strip2.show();
  strip3.show();
  strip4.show();
  strip5.show();
  strip6.show();
  
}

/*
   Copyright (c) 2016 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/
