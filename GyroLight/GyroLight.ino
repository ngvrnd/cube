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

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS, PIN+1, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS, PIN+2, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUM_LEDS, PIN+3, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(NUM_LEDS, PIN+4, NEO_GRBW + NEO_KHZ800);
//Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(NUM_LEDS, PIN+5, NEO_GRBW + NEO_KHZ800);

Adafruit_NeoPixel faces[6] = {Adafruit_NeoPixel(NUM_LEDS, PIN+0, NEO_GRBW + NEO_KHZ800),
                              Adafruit_NeoPixel(NUM_LEDS, PIN+1, NEO_GRBW + NEO_KHZ800),
                              Adafruit_NeoPixel(NUM_LEDS, PIN+1, NEO_GRBW + NEO_KHZ800),
                              Adafruit_NeoPixel(NUM_LEDS, PIN+1, NEO_GRBW + NEO_KHZ800),
                              Adafruit_NeoPixel(NUM_LEDS, PIN+1, NEO_GRBW + NEO_KHZ800),
                              Adafruit_NeoPixel(NUM_LEDS, PIN+1, NEO_GRBW + NEO_KHZ800)};


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

  for (int i = 0; i < 6; ++i) {
    faces[i].setBrightness(BRIGHTNESS);
    faces[i].begin();
    faces[i].show(); // Initialize all pixels to 'off
  }

}

uint8_t intensit = 0;
uint8_t increment = 1;

const unsigned int clrMax = 128;

bool first = true;

uint8_t mapXYtoI(uint8_t x, uint8_t y) {
  return (y + 8*x);
}

uint32_t color = strip.Color(cx, cy, cz, i_aMag);
int state = 0;
int pixelIdx = 0;
int face = 0;
bool running = true;

void loop() {
  
  switch (state) {
    case 0:
      if (pixelIdx < 8) {
        faces[face].setPixelColor(pixelIdx, color);
      } else {
        pixelIdx = 0;
        state ++;
        face ++;
      }
      break;
    case 1:
      if (pixelIdx < 8) {
        faces[face].setPixelColor(pixelIdx, color);
      } else {
        pixelIdx = 0;
        state ++;
        face ++;
      }

      break;  
    default:
    if (running) {
      pixelIdx = state = face = 0;      
    }
  }
  
  if (running) {
    faces[face].show()
  }
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
