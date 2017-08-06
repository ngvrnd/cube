
#include <BMI160.h>
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

#define UC_PWR_DISABLE 2
#define UC_LO_BAT_DETECT 3
#define LED_PWR_ENABLE 4
#define LED_LO_BAT_DETECT 5

#define PANEL_0_CTRL_PIN 6

#define NUM_LEDS 64  // 64 per panel.  There are six panels in the prototype cube.

enum class CubeMode { Menu, Run };
CubeMode cubeMode = CubeMode::Run;
enum class FaceMode { Off, Accel, CA, Level };
FaceMode faceMode[6] = { FaceMode::Accel, FaceMode::Accel, FaceMode::Accel, FaceMode::Accel, FaceMode::Accel, FaceMode::Accel };

int ledPower;

#define BRIGHTNESS 5

// In this usage, the faces of the cube are treated as strips of LEDS; the addressing is serial row 
// by row down the face of the panel, with the 0 pixel closest to the DIN connection.
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS, PANEL_0_CTRL_PIN,     NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS, PANEL_0_CTRL_PIN + 1, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS, PANEL_0_CTRL_PIN + 2, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUM_LEDS, PANEL_0_CTRL_PIN + 3, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(NUM_LEDS, PANEL_0_CTRL_PIN + 4, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(NUM_LEDS, PANEL_0_CTRL_PIN + 5, NEO_GRBW + NEO_KHZ800);

const float aMax = 2;
const float aMagMax = sqrt(3 * sq(aMax));
const float aRate = 1600;
const float gMax = 250;

static void eventCallback() 
{
  if (CurieIMU.getInterruptStatus(CURIE_IMU_ZERO_MOTION)) {
    ledPower = LOW; 
    // turn off power to LED panels 
    if (ledPower != LOW) {
      Serial.println("zero motion detected... LED POWER OFF");
    }
  }  

  if (CurieIMU.getInterruptStatus(CURIE_IMU_DOUBLE_TAP)) {
    Serial.println("tap tap! LED POWER ON");
    if (cubeMode == CubeMode::Run) {
      cubeMode = CubeMode::Menu;
      Serial.println("Menu mode");
    } else {
      cubeMode = CubeMode::Run;
      Serial.println("Run mode");
    }
    // turn LED power on.
    ledPower = HIGH;

  } else if (CurieIMU.getInterruptStatus(CURIE_IMU_TAP)) {
    
//    if (CurieIMU.tapDetected(X_AXIS, NEGATIVE))
//      Serial.println("Tap detected on negative X-axis");
//    if (CurieIMU.tapDetected(X_AXIS, POSITIVE))
//      Serial.println("Tap detected on positive X-axis");
//    if (CurieIMU.tapDetected(Y_AXIS, NEGATIVE))
//      Serial.println("Tap detected on negative Y-axis");
//    if (CurieIMU.tapDetected(Y_AXIS, POSITIVE))
//      Serial.println("Tap detected on positive Y-axis");
//    if (CurieIMU.tapDetected(Z_AXIS, NEGATIVE))
//      Serial.println("Tap detected on negative Z-axis");
//    if (CurieIMU.tapDetected(Z_AXIS, POSITIVE))
//      Serial.println("Tap detected on positive Z-axis");

    Serial.println("LED POWER ON");
    ledPower = HIGH;
    
  }
}

void setup() 
{

#define SER_DEB_init   // Use '//' to break up the symbol in order to disable debugging.
#ifdef SER_DEB_init
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device
  Serial.println("Initializing IMU device...");
#endif

  ledPower = HIGH;
  digitalWrite(LED_PWR_ENABLE, ledPower);
  
  /* 
   *   UC_PWR_DISABLE 2
   *   UC_LO_BAT_DETECT 3
   *   LED_PWR_ENABLE 4
   *   LED_LO_BAT_DETECT 5
   **/
   
  pinMode(LED_PWR_ENABLE, OUTPUT);
  pinMode(UC_LO_BAT_DETECT, INPUT);
  pinMode(LED_LO_BAT_DETECT, INPUT);
  
  CurieIMU.begin();

  CurieIMU.setGyroRange(gMax);
  CurieIMU.setAccelerometerRate(aRate);
  CurieIMU.setAccelerometerRange(aMax);

  CurieIMU.attachInterrupt(eventCallback);

  // Reduce threshold to allow detection of weaker taps (>= 750mg)
  CurieIMU.setDetectionThreshold(CURIE_IMU_TAP, 750); // (750mg)

  // Enable Tap detection
  CurieIMU.interrupts(CURIE_IMU_TAP);
  
  // Set the quite time window for 2 taps to be registered as a double-tap (Gap time between taps <= 1000 milliseconds)
  CurieIMU.setDetectionDuration(CURIE_IMU_DOUBLE_TAP, 1000);

  // Enable Double-Tap detection
  CurieIMU.interrupts(CURIE_IMU_DOUBLE_TAP);

  CurieIMU.setDetectionThreshold(CURIE_IMU_ZERO_MOTION, 50);  // 50mg
  CurieIMU.setDetectionDuration(CURIE_IMU_ZERO_MOTION, 6);    // 6s -- delay time to power down LEDs.
  CurieIMU.interrupts(CURIE_IMU_ZERO_MOTION);

  /* Enable Motion Detection */
  CurieIMU.setDetectionThreshold(CURIE_IMU_MOTION, 20);      // 20mg
  CurieIMU.setDetectionDuration(CURIE_IMU_MOTION, 10);       // trigger times of consecutive slope data points
  CurieIMU.interrupts(CURIE_IMU_MOTION);

  // initialize all panels (strips) to off
  strip1.setBrightness(BRIGHTNESS);
  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off

  strip2.setBrightness(BRIGHTNESS);
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off

  strip3.setBrightness(BRIGHTNESS);
  strip3.begin();
  strip3.show(); // Initialize all pixels to 'off

  strip4.setBrightness(BRIGHTNESS);
  strip4.begin();
  strip4.show(); // Initialize all pixels to 'off

  strip5.setBrightness(BRIGHTNESS);
  strip5.begin();
  strip5.show(); // Initialize all pixels to 'off

  strip6.setBrightness(BRIGHTNESS);
  strip6.begin();
  strip6.show(); // Initialize all pixels to 'off
}

uint8_t cx = 0,
        cy = 0,
        cz = 0;

void loop() {

  digitalWrite(LED_PWR_ENABLE, ledPower);

  switch (cubeMode) {
    CubeMode::Run:
      for (int face = 0; face < 6; face++) 
        switch (faceMode[face]) {
          FaceMode::Off:
            break;
          FaceMode::Accel:
            break;
          FaceMode::CA:
            break;
          FaceMode::Level:
            break;
          default:
            serial.println("Fell through the face mode switch!");
            break; 
      } 
      
    default: 
      serial.println("Fell through the cube mode switch!");
      break;
  }
}

void accell() {

  float ax, ay, az;
  float gx, gy, gz;

  // read gyro measurements from device, scaled to the configured range
  CurieIMU.readGyroScaled(gx, gy, gz);
  CurieIMU.readAccelerometerScaled(ax, ay, az);

           
  if (abs(gx) > 1) {
    cx = min(int(abs(gx)), 255);
  } else if (cx > 0) {
    cx--;
  }
  if (abs(gy) > 1) {
    cy = min(int(abs(gy)), 255);
  } else if (cy > 0) {
    cy--;
  }
  if (abs(gz) > 1) {
    cz = min(int(abs(gz)), 255);
  } else if (cz > 0) {
    cz--;
  }

  float aMag = sqrt(sq(ax) + sq(ay) + sq(az));
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

  uint32_t color = strip1.Color(cx, cy, cz, i_aMag);
  for (int i = 0; i < 64; i++) {
    strip1.setPixelColor(i, color);
    strip2.setPixelColor(i, color);
    strip3.setPixelColor(i, color);
    strip4.setPixelColor(i, color);
    strip5.setPixelColor(i, color);
    strip6.setPixelColor(i, color);
  }

  delay(100);

  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  strip5.show();
  strip6.show();


}


