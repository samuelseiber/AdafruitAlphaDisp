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


#include "AlphaDisp.h"
#include <Wire.h>

#include "AlphaDispFont.cpp"

AlphaDisp::AlphaDisp (void)
{
}

void AlphaDisp::setBrightness(uint8_t b) 
{
  for (uint8_t devAddr=i2c_addr;devAddr<numberOfDevices+i2c_addr;devAddr++)
  {
    if (b > 15) b = 15;
    Wire.beginTransmission(devAddr);
    Wire.write(HT16K33_CMD_BRIGHTNESS | b);
    Wire.endTransmission();  
  }	
}

void AlphaDisp::blinkRate(uint8_t b) 
{
  for (uint8_t devAddr=i2c_addr;devAddr<numberOfDevices+i2c_addr;devAddr++)
  {
    Wire.beginTransmission(devAddr);
    if (b > 3) b = 0; // turn off if not sure
    Wire.write(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1)); 
    Wire.endTransmission();
  }	
}


void AlphaDisp::begin(uint8_t _addr = 0x70, uint8_t ND=1, uint8_t DD=4) 
{
  i2c_addr = _addr;
  numberOfDevices=1;  //Set the default
  if ((ND>0) && (ND<9)) numberOfDevices=ND;  //Don't change anything if out of range
  digitsPerDev=4;  //The standard device from Adafruit has 4 digits
  if ((DD>0) && (DD<9)) digitsPerDev=DD; //Don't change anything if out of range
  Wire.begin();
  for (uint8_t devAddr=i2c_addr;devAddr<(numberOfDevices+i2c_addr);devAddr++)
  {	  
    Wire.beginTransmission(devAddr);
    Wire.write(0x21);  // turn on oscillator
    Wire.endTransmission();
  }	
    blinkRate(HT16K33_BLINK_OFF);
    setBrightness(0); // min brightness
	s_delay=250;
	cursorAddr=0;
	sevenSegFlag=false;
}

void AlphaDisp::writeDisplay(void) 
{
  for (uint8_t devAddr=i2c_addr;devAddr<numberOfDevices+i2c_addr;devAddr++)
  {
    Wire.beginTransmission(devAddr);
    Wire.write((uint8_t)0x00); // start at address $00

    for (uint8_t i=0; i<digitsPerDev; i++) {
      Wire.write(displaybuffer[i+((devAddr-i2c_addr)*digitsPerDev)] & 0xFF);    
      Wire.write(displaybuffer[i+((devAddr-i2c_addr)*digitsPerDev)] >> 8); 
    }
    Wire.endTransmission();  
  }  
}

void AlphaDisp::clear(void) 
{
  for (uint8_t i=0; i<DISPLAY_BUFFER_SIZE; i++) {
    displaybuffer[i] = 0;
  }
}

void AlphaDisp::writeDigitRaw(uint8_t n, uint16_t bitmap) 
{
  if ((n<=numberOfDevices*digitsPerDev) && (n!=0)) displaybuffer[n-1] = bitmap;  //Bypass if pointer out of range
}

void AlphaDisp::writeDigitAscii(uint8_t n, uint8_t a,  boolean d) 
{
  uint8_t tmpchr=a;
  if (a>127) return;   //Out of bounds on our table, so do nothing
  if ((n>numberOfDevices*digitsPerDev) || (n==0)) return;  //exit if "off screen"  
  if ((sevenSegFlag) && isdigit(tmpchr)) tmpchr-=48;
  uint16_t font = pgm_read_word(alphafonttable+tmpchr);
  displaybuffer[n-1] = font;  //display memory array starts at 0,for humans, the screen starts at 1
  if (d) displaybuffer[n-1] |= (1<<14);
}

void AlphaDisp::scrollDelay(unsigned long dly)
  {
	 s_delay=dly;
  }
  
void AlphaDisp::scrollCharacter(uint8_t character, boolean d)
  {
	 for (uint8_t z9=1;z9<numberOfDevices*digitsPerDev;z9++) displaybuffer[z9-1]=displaybuffer[z9];  //shift display memory left
	 writeDigitAscii(numberOfDevices*digitsPerDev,character,d);
	 writeDisplay();
	 delay(s_delay);
  }
  
void AlphaDisp::scrollFinish(void)
  {
	  for (uint8_t z9=0;z9<numberOfDevices*digitsPerDev;z9++) scrollCharacter(' ',false);
  }
 
 void AlphaDisp::scrollString(uint8_t *characters)
 {
	while (*characters) scrollCharacter(*characters++, false);
 }
void AlphaDisp::setCursor(uint8_t CA)
{
    if ((CA<=(numberOfDevices*digitsPerDev)) && (CA!=0)) cursorAddr=CA;  //the first position is 1
}
void AlphaDisp::writeString(uint8_t *characters)
{
	while (*characters)
	{
		if (cursorAddr<((numberOfDevices*digitsPerDev)+1))  //still on the display area?
		{
			writeDigitAscii(cursorAddr++,*characters++);
		}
		else return;  //Exit if trying to write past end of the display area
	}
}
