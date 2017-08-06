

#include "CurieIMU.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


#define PIN 6

#define NUM_LEDS 64

#define BRIGHTNESS 255

Adafruit_NeoPixel faces[6] = {Adafruit_NeoPixel(NUM_LEDS, PIN+0, NEO_GRBW + NEO_KHZ800),
                              Adafruit_NeoPixel(NUM_LEDS, PIN+1, NEO_GRBW + NEO_KHZ800),
                              Adafruit_NeoPixel(NUM_LEDS, PIN+2, NEO_GRBW + NEO_KHZ800),
                              Adafruit_NeoPixel(NUM_LEDS, PIN+3, NEO_GRBW + NEO_KHZ800),
                              Adafruit_NeoPixel(NUM_LEDS, PIN+4, NEO_GRBW + NEO_KHZ800),
                              Adafruit_NeoPixel(NUM_LEDS, PIN+5, NEO_GRBW + NEO_KHZ800)};


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

#define SER//_DEB
#ifdef SER_DEB
#define SER_DEB_init
void ser_deb() {
//  // display tab-separated gyro x/y/z values

//  Serial.print("aMag:\t");
//  Serial.print(aMag); 
//  Serial.print("\t");

  Serial.print("c:\t"); 
  Serial.print("\t");
  Serial.print(cx);
  Serial.print("\t");
  Serial.print(cy);
  Serial.print("\t");
  Serial.print(cz);
  Serial.println();

  
//  Serial.print("a:\t"); 
//  Serial.print("\t");
//  Serial.print(ax);
//  Serial.print("\t");
//  Serial.print(ay);
//  Serial.print("\t");
//  Serial.print(az);
//  Serial.println();
}
#endif

uint32_t color = 0;
int state = 0;
size_t pixelIdx = 0;
int face = 0;
bool running = true;
float gx, gy, gz; //scaled Gyro values
int8_t i_aMag = 0;

uint8_t mapXYtoI(uint8_t x, uint8_t y) {
  return (y + 8*x);
}

void setup() {
#define SER//_DEB_init
#ifdef SER_DEB_init
  Serial.begin(38400); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device
  Serial.println("Initializing IMU device...");
#endif

//  CurieIMU.begin();
//
//  // Set the accelerometer range to 250 degrees/second
//  CurieIMU.setGyroRange(250);
//  CurieIMU.setGyroRate(3200);
//  CurieIMU.setAccelerometerRate(1600);
//  CurieIMU.setAccelerometerRange(aMax);
//
  for (int i = 0; i < 6; ++i) {
    faces[i].setBrightness(BRIGHTNESS);
    faces[i].begin();
    faces[i].show(); // Initialize all pixels to 'off
  }
//
//  CurieIMU.autoCalibrateGyroOffset();
//  CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS, 0);
//  CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS, 0);
//  CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, -1);
//
//  pixelIdx = 7;

}

  const size_t trajLen = 23;
  uint8_t px[trajLen] = {1,2,3,4,5,6,7,0,7,7,7,7,7,7,7,7,0,0,0,0,0,0,0};
  uint8_t py[trajLen] = {1,2,3,4,5,6,7,0,0,1,2,3,4,5,6,7,1,2,3,4,5,6,7}; 
  
void loop() {

//  // read gyro measurements from device, scaled to the configured range
//  CurieIMU.readGyroScaled(gx, gy, gz);
//
//  float tmp = min(int(abs(gx)),255);
////  
//  if ((abs(gx) > 1) && (tmp > cx)) {
//    cx = tmp;
//  } else if (cx > 0) {
//    cx--;
//  }
//  
//  tmp = min(int(abs(gy)),255);
//  
//  if ((abs(gy) > 1) && (tmp > cy)) {
//    cy = tmp;
//  } else if (cy > 0) {
//    cy--;
//  }
//
//  tmp = min(int(abs(gz)),255);
//  
//  if ((abs(gz) > 1) && (tmp > cz)) {
//    cz = tmp;
//  } else if (cz >0) {
//    cz--;
//  }
//
//  CurieIMU.readAccelerometerScaled(ax, ay, az);
//
//  delay(10);
//  
//  ser_deb();


  //color = faces[face].Color(cx, cy, cz, 0);
  color = faces[0].Color(10,5,40,1);
  
  switch (state) {
    case 0:
      if (pixelIdx < trajLen) {
        
        faces[face].setPixelColor(mapXYtoI(px[pixelIdx], py[pixelIdx]), color);
        pixelIdx++;
        
      } else {
//        Serial.println("state 0 -> state 1");
        delay(100);
        pixelIdx = 0;
        state ++;
      }
      break;
    case 1:
      if (pixelIdx < trajLen) {
        
        faces[face].setPixelColor(mapXYtoI(px[pixelIdx], py[pixelIdx]), 0);
        pixelIdx++;
        
      } else {
//        Serial.println("state 1 -> state 2");
        faces[face].show();
        delay(100);
        pixelIdx = 0;
        state ++;
      }
      break;  
    case 2:
      if (pixelIdx < trajLen) {
        
        faces[face].setPixelColor(mapXYtoI(px[pixelIdx], py[pixelIdx]), color);
        pixelIdx++;
      
      } else {
//        Serial.println("state 2 -> state 3");        
        faces[face].show();
        delay(100);
        pixelIdx = 0;
        state ++;
      }
      break;  
    default:
//      Serial.println(" default state - > state 0");
      delay(100);
      if (running) {
        pixelIdx = 0;
        face++;
        if (face == 3) face = 0;
        state = 0;      
        faces[face].show();
      }
  }
//  Serial.println(state);
//  Serial.println("Bottom of loop");
//  delay(100);
//  if (running) {
//  }
}

/*1
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
