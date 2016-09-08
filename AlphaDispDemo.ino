#include <AlphaDisp.h>

AlphaDisp dsply;

/*
 I built a 16 character display, using 4 Adafruit Alphanumeric FeatherWing displays (see product ID 3130).
 Thus I wanted a better library to handle a nice long 16 character display, as I would be writting to 4
 different I2C device addresses (which must be consecutive, with the leftmost being the lowest address).
 
 This Demo is coded for a single 4 character display.
 When I run this on my 16 character display, I change the numDevices below to 4.  
 Sometime down the road, I am going to assemble some alphanumeric displays using the HT16K33 display driver, driving
 8 characters each (its maximum for this type of display).  I will then need to modify the deviceDigits to 8 for that project.
 If your project contains more than 32 characters, the AlphaDisp.h file will need to be modified at the #define
 for DISPLAY_BUFFER_SIZE, which I set at 32.

 Below is a reference to "dsply.sevenSegFlag".  This is due to a personal thing of mine.  For just about every
 display I write to, I like to have a seven segment font for numbers.  I just like them.  For this project, it
 didn't change the way most digits appear.  It is a "Sam" thing.  By default, it doesn't use that font.

 As I have mentioned elsewhere, using the scroll routines are not friendly if there is other I/O that needs a lot of attention.
 Thus, if your project needs to capture that I/O (eg. GPS device), you will have to write code, using the other routines to do your
 own scrolling.  The scroll routines use the delay() function internally, thus keeping the processor tied up.  

 One other decision I made was to refer to the first position of the display as 1.  So routines like writeDigitRaw & setCursor that refer
 to a character position begin at 1, going to the number of characters total.  The Adafruit libraries started at 0.  I think most people
 think of things starting at 1.
 */

//  Adjust the following line or two for your actual display
#define numDevices 1
#define deviceDigits 4


void setup()
{
  uint8_t abuffer[32];
  dsply.begin(0x70, numDevices, deviceDigits);  //0x70 is address of first device
  dsply.setBrightness(8);  //About mid brightness
  dsply.clear();    //clear out the memory version of the display
  dsply.writeDisplay();  //display the cleared out memory, clearing the display

  for (uint8_t z9=1;z9<(deviceDigits*numDevices)+1;z9++)  //loop through the display
  {
    dsply.writeDigitRaw(z9,0xFFFF);  //  Turn on all segments
    dsply.writeDisplay();   //move memory to the actual display
    delay(200);
    dsply.writeDigitRaw(z9,0);  //clear out test pattern
  }  
    delay(200);
    dsply.clear();
    dsply.writeDigitAscii(2,'H');  //The letter H goes on position 2, in the display memory
    dsply.writeDigitAscii(3,'i');  //The letter i goes on position 3
    dsply.writeDisplay();  //Now show the display memory
    delay(1500);
    dsply.blinkRate(1);  //0=no blink, 1=2Hz blink, 2=1Hz blink, 3=.5Hz blink
    delay(3000);
    dsply.blinkRate(0);  //Stop the blinking

    dsply.clear();
    dsply.scrollDelay(250);  //Set the delay for scrolling (milliseconds between characters)
    sprintf(abuffer,"This is a test line of data for the display");
    dsply.scrollString(abuffer);  //I use sprintf to format data for display, thus show the results here
    dsply.scrollFinish();  //Now, finish the scroll, until blank

    dsply.scrollCharacter('*');  //Want to scroll just one character?  
    dsply.scrollFinish();  //Move it left until it is gone

    dsply.setBrightness(0);  //the lowest brightness setting
    dsply.scrollString("Dim setting");
    dsply.scrollFinish();
    dsply.setBrightness(15);  //full brightness
    dsply.scrollString("Full bright..");
    dsply.scrollFinish();
    delay(1000);

    dsply.sevenSegFlag=false;  //This is set to false in the begin() function
    dsply.setCursor(1);
    dsply.writeString("0123");
    dsply.writeDisplay();
    delay(1000);
    dsply.setCursor(1);
    dsply.writeString("4567");
    dsply.writeDisplay();
    delay(1000);
    dsply.setCursor(1);
    dsply.writeString("89  ");
    dsply.writeDisplay();
    delay(1000);
    dsply.sevenSegFlag=true;  //Square off the numbers, like we have a seven segment display
    dsply.setCursor(1);
    dsply.writeString("0123");
    dsply.writeDisplay();
    delay(1000);
    dsply.setCursor(1);
    dsply.writeString("4567");
    dsply.writeDisplay();
    delay(1000);
    dsply.setCursor(1);
    dsply.writeString("89  ");
    dsply.writeDisplay();
    delay(1000);

    dsply.clear();
    dsply.setCursor(2);  //for the following function, set the cursor to position 2 on the screen (which starts at 1, not 0)
    dsply.blinkRate(2);
    dsply.writeString("END");
    dsply.writeDisplay();
    delay(5000);
    dsply.clear();
    dsply.writeDisplay();   
}

void loop() {
  // put your main code here, to run repeatedly:

}
