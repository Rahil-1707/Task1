# Mini Task 3

This is the step that you need to iterate again and again mostly. As not everything works perfectly on the first go. Hence, here are some solutions to the preidentified errors which are prone to occur and are very comman.

---

## **Project 1**

Remember how the how our device (hand gesture volume control) works?

Motion Sensor -> Microcontroller -> Display -> Computer

There must be a problem with any of these modules.

First of all check all the connectins and say for any loose or inappropriate connections, refer back to the schematic. 

If you find any part motion sensor/ microcontroller/ display to be very hot. You might have shorted it. Hence, it may not work. You can replace the part with a new one. 

#### **Testing sensor**

Apply the code given below

```C
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds;
#include "HID-Project.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);   // Controls the display of OLED screen
  

int level = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);  //Sets the data rate in bits per second (baud) for serial data transmission. 
  Wire.begin();    //begins I2C protocol and hence the devices can communicate with each other

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {     //if display dosen't begin we show an error
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");
  // If everything works fine we proceed

  //gesture mode will be entered once proximity mode senses something close
  apds.enableProximity(true);     //These commands senses proximity and gesture
  apds.enableGesture(true);

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(1.5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Volume"); 
  display.display(); 

  Consumer.begin();
}

void loop() {
   

  //read a gesture from the device
    uint8_t gesture = apds.readGesture();
    if(gesture == APDS9960_DOWN) {
      Serial.println("v");
      Consumer.write(MEDIA_VOLUME_DOWN);
      level--; 
      changeVolume();     //calls the function changeVolume to change the volume

    }
    if(gesture == APDS9960_UP) {
      Serial.println("^");
      Consumer.write(MEDIA_VOLUME_UP);
      level++;
      changeVolume();
    }
}

void changeVolume() {
  Serial.println(level);
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("Volume");  
  
  display.setCursor(0, 15);
  
  for (int i = 0; i <= level; i++){
    display.print("[]"); 
    display.display();  
  }
}
```

See if you are able to see anything on the display then the microcontroller and display would be working fine. If you see the that the volume change is seen as you create a gesture with your hand over the sensor then the sensor would also be working fine. The problem would be the connection with the computer and the sensor, check if the USB cable is working, if not change it.

If the display does not change even when the the gesture is made then there can be 2 problems one of sensor, microcontroller or the connection between them. If the light on the sensor is glowing then the connectin between them maybe fine and the sensor would be not working replace it with another sensor that should suffice.

#### **Testing the display**
Display is optional part, even if it is not working the over functionality of the device remains as it is. Hence, if the display is not working, problem maybe with the code, connections with the microcontroller, display itself won't be working. Check if the display is working by making appropriate connections and uploading the code given above. If the display still doesn't work then we must change the display and replace it with a new one.

#### **Testing microntroller**

If you cannot upload code to SENSE and if the IC is hot , you would have probably burnt it , but shorting an SENSE isn't easy , they have inbuilt mechanisms to prevent it so see if there are other issues as mentioned above before assuming that you have burnt your SENSE.

Also, sometimes the some computers may not support such inputs through USB. If everything works fine then this must be the case. Bad luck😭😭. But don't worry mostly this isn't the case. Even if it is the case then you can use an alternate thing to transfer signals to decrease the volume to the computer. That is by some sensor boards that can transfer signals to the internet and then giving a website to control the volume may do it for you. Currently, my knowledge on this subject is minimum. Hence, I can't explain you these steps in detail. But internet has dosens of examples how it can be done.

These would be the cases why the device may not be working. If you find some other issue you may upload it to the project page link given previouly or can even inform and I will try my best to give the solution to your problem.






---
## **Project 3**

#### **Trouble shooting**

Remember how the our lamp works? 

Notification -> IFTTT -> Adafruit -> Node Mcu -> Lamp

There must be problem in any of these modules. 

To test if your lamp works, plug in the 5v and 9v power supplies to your circuit. Then wait for a few seconds for the NodeMcu to connect to your wifi network. Make sure your phone (with IFTTT app) is connected to the internet and send an SMS to that phone from a different phone. As soon as you receive the SMS, the LED strip should light up. After about 2 minutes, it should turn off. If it works, great job! If not, here are a few things you can do to troubleshoot:


- Re-check your connections and make sure there are no loose connections.
- Check the applet you created on IFTTT. Open IFTTT and select "My applets" (bottom right). Click on your applet and click on "View activity log". If it says applet failed, you need to recheck your Adafruit and IFTTT setup.
- Check the code to make sure you have provided the right WiFi and Adafruit credentials. Also ensure that you didn't make any other changes to the code.
- Try to run on some online site (tinkerCAD) after making connections and upload the code there. It will show an error if there is any error in the code.

#### Testing Node MCU

If you cannot upload code to Node MCU and if the IC is hot , you would have probably burnt it , but shorting an Node MCU isn't easy , they have inbuilt mechanisms to prevent it so see if there are other issues as mentioned above before assuming that you have burnt your Node MCU.


And this includes all the parts of project. The problem must be in any of these. If there is some other issue that you may find you may upload it to the instructables page for the project or can inform me on this account.

---