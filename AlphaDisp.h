/*
This software is derived from works of Adafruit Industries as well as
Samuel Seiber.

The MIT License (MIT)

Copyright (c) 2012 Adafruit Industries

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Copyright (c) 2016 Samuel Seiber

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

*/


#ifndef AlphaDisp_h
#define AlphaDisp_h

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


#include <Wire.h>

#define DISPLAY_BUFFER_SIZE 32
#define HT16K33_BLINK_CMD 0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF 0
#define HT16K33_BLINK_2HZ  1
#define HT16K33_BLINK_1HZ  2
#define HT16K33_BLINK_HALFHZ  3

#define HT16K33_CMD_BRIGHTNESS 0xE0

class AlphaDisp {
 public:
  AlphaDisp(void);
  void begin(uint8_t _addr, uint8_t ND, uint8_t DD);
  void setBrightness(uint8_t b);
  void blinkRate(uint8_t b);
  void writeDisplay(void);
  void clear(void);
  void writeDigitRaw(uint8_t n, uint16_t bitmask);
  void writeDigitAscii(uint8_t n, uint8_t ascii, boolean dot = false);
  void scrollDelay(unsigned long dly);
  void scrollCharacter(uint8_t character, boolean d=false);
  void scrollFinish(void);
  void scrollString(byte *characters);
  void setCursor(uint8_t CA);  //Set the cursor address
  void writeString(byte *characters);  //Write a string of characters starting at the cursor address
 protected:
  uint8_t i2c_addr;
  unsigned long s_delay;
  uint8_t numberOfDevices;
  uint8_t cursorAddr;
  uint16_t displaybuffer[DISPLAY_BUFFER_SIZE]; 
  uint8_t digitsPerDev;
};

#endif // AlphaDisp
