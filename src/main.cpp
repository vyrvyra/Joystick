#include <Arduino.h> // This project uses the C++20 language standard version.
#include <ESP32BLECombo.h> //library for ESP32 BLE Combo HID device

ESP32BLECombo ble; // calling the ESP32BLECombo class to create an object called ble

const int xPozition = 34; //analog pin for X axis
const int yPozition = 35; //analog pin for Y axis
const int PinLeftClick = 26; //digital pin for left mouse button
const int PinRightClick = 25; // digital pin for right mouse button
const int PinMiddleClick = 27; // digital pin for middle mouse button
const int PinUnicate1 = 28; // digital pin for unicate button1
const int PinUnicate2 = 33; // digital pin for unicate button2
const uint8_t leftClick = 1; // constant for left mouse button
const uint8_t rightClick = 2; // constant for right mouse button
const uint8_t middleClick = 3; // constant for middle mouse button
const uint8_t unicate1 = 4; // constant for whatever key you want to use I use it as ctrl key later in the code
const uint8_t unicate2 = 5; // constant for whatever key you want to use I use it as r key later in the code
bool isLeftPressed = false; // variable to track the state of the left mouse button
bool isRightPressed = false; // variable to track the state of the right mouse button
bool isMiddlePressed = false; // variable to track the state of the middle mouse button
bool isUnicate1Pressed = false; // variable to track the state of the unicate button
bool isUnicate2Pressed = false; // variable to track the state of the unicate button

const int Center = 2048; // center position for the analog sticks
const int Deadzone = 350; // deadzone for the analog sticks
const int MAX_Speed = 20; // maximum speed for mouse movement

void setup() {
  Serial.begin(115200);
  pinMode(PinLeftClick, INPUT_PULLUP);
  pinMode(PinRightClick, INPUT_PULLUP);
  pinMode(PinMiddleClick, INPUT_PULLUP);
  pinMode(PinUnicate1, INPUT_PULLUP);
  pinMode(PinUnicate2, INPUT_PULLUP);
  
  ESP32BLEComboConfig cfg;
  cfg.mode = ESP32BLEComboMode::KEYBOARD_MOUSE;
  cfg.deviceName = "Knipl fr?";
  cfg.manufacturer = "Espressif";
  cfg.batteryLevel = 100;
  cfg.appearance = ESP32BLEComboAppearance::AUTO;
  ble.begin(cfg);
  Serial.println("ESP32 BLE Combo mys pripravena k parovani.");
}

void loop() {
  if (ble.isConnected()){
    int RAW_X = analogRead(xPozition);
    int RAW_Y = analogRead(yPozition);
    int pohybX = 0;
    int pohybY = 0;
    if (RAW_X < Center - Deadzone) {
        pohybX = map(RAW_X, Center - Deadzone, 0, 0, -MAX_Speed);
      } else if (RAW_X > Center + Deadzone) {
        pohybX = map(RAW_X, Center + Deadzone, 4095, 0, MAX_Speed);
      }
    if (RAW_Y < Center - Deadzone){
        pohybY = map(RAW_Y, Center - Deadzone, 0, 0, MAX_Speed);
      } else if (RAW_Y > Center + Deadzone){
        pohybY = map(RAW_Y, Center + Deadzone, 4095, 0, -MAX_Speed);
      }
    if (pohybX != 0 || pohybY != 0){
      ble.mouseMove(pohybX,pohybY);
    }
    bool currentLeft = (digitalRead(PinLeftClick) == LOW);
    if (currentLeft != isLeftPressed) {
      isLeftPressed = currentLeft;
      if (isLeftPressed) {
        ble.mousePress(leftClick);
      } else {
        ble.mouseRelease(leftClick);
      }
    }
    bool currentRight = (digitalRead(PinRightClick) == LOW);
    if (currentRight != isRightPressed) {
      isRightPressed = currentRight;
      if (isRightPressed) {
        ble.mousePress(rightClick);
      } else {
        ble.mouseRelease(rightClick);
      }
    } 
    bool currentMiddle = (digitalRead(PinMiddleClick) == LOW);
    if (currentMiddle != isMiddlePressed) {
      isMiddlePressed = currentMiddle;
      if (isMiddlePressed) {
        ble.mousePress(middleClick);
      } else {
        ble.mouseRelease(middleClick);
      }
    }
    bool currentUnicate1 = (digitalRead(PinUnicate1) == LOW);
    if (currentUnicate1 != isUnicate1Pressed) {
      isUnicate1Pressed = currentUnicate1;
      if (isUnicate1Pressed) {
        ble.press(KEY_CTRL);
      } else {
        ble.release(KEY_CTRL);
      }
    }
    bool currentUnicate2 = (digitalRead(PinUnicate2)==LOW);
    if (currentUnicate2 != isUnicate2Pressed){
      isUnicate2Pressed = currentUnicate2;
      if (isUnicate2Pressed){
        ble.press(KEY_F9);
      } else {
        ble.release(KEY_F9);
      }
    }
  }
}
