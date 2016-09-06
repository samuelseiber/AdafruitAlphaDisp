# AdafruitAlphaDisp
Library for the Adafruit alphanumeric backpack (using the HT16K33 chip)

I was putting together a project using 4 Adafruit featherwing LED backpacks (Adafruit product id 3130).  That would give me a 16 character display.  I would be using 4 i2c addresses, and wanted to hide the details of the i/o from my main sketch.  Thus I thought I would build a library to handle the job.  I used some of the code from the Adafruit libries, and wrote the rest.  My library goes into a folder called AlphaDisp within the libraries folder.  
These are the functions it supports:
begin(i2c accr, Number of Devices, Number of digits per device)  to initialize the devices
setBrightness(0-15)  to set the brightness of all devices
blinkRate(0-3)  to set the blink rate of all of the devices
writeDisplay()  write memory stored display to the displays
clear()  clear the memory stored display (all LEDs off)
writeDigitRaw(character position, bitmap of character)  writes to the memory stored display, the bitmap for 1 character
writeDigitAscii(character position, Ascii character, optional boolean for decimal)  write Ascii character to memory
scrollDelay(millisecond delay factor)  for the scrolling routines, the delay between character shifts
scrollCharacter(character, optional boolean decimal indicator) write one character to scroll
scrollFinish()  shift everything to the left until the display is blank
scrollString(string)  scroll the given string
setCursor(character position on the display)  Set location on the display to write static text
writeString(string)  write the string at the current cursor location of the display

