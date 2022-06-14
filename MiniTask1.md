# **Mini Task 1**


Many a times, we may find ourselves watching web series and eating something with it. And if we want to increase or decrease the volume we need to wash the hands or else it will make our laptops dirty. This is a problem that I faced and is comman with all the teenagers of this age. Recently I viewed a project on hackster.io the link is given here (https://www.hackster.io/ZBoss1234/hand-gesture-volume-control-01b9f0). It shows how to change computer volume without pressing any key. I found it very intresting and hence, I am describing it in my own words here.

### 1. Components Required:
   a) Qwiic cable
   b) QTPY SAMD21 
   c) SENSE 
   d) OLED display
   e) Breadbord
   f) 3V voltage supply

### 2. Connections
- Connect a Qwiic cable between QTPY SAMD21 and SENSE
- Create a I2C bus between SENSE and OLED Display
- OLED GND -> SENSE GND
- OLD VCC -> SENSE 3V
- OLED SCL -> SENSE SCL
- OLED SDA -> SENSE SDA

The schematic for this is given below:

<img src="https://hackster.imgix.net/uploads/attachments/1446971/schematic_for_hackster_mpgyQ0qxl3.png" >

So, now all the connections are made properly it's time to code so that our device works according to our requirements

### 3. Code

The code is written down in C. 
To make things easy I have broken down code in 2 parts importing required libraries and code controlling the device.
#### 1) Installing libraries

```c

The below code does it for us
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds;
#include "HID-Project.h"
```
- "APDS9960" - Adafruit
- "GFX" - Adafruit
- "SSD1306" - Adafruit
- HID - NicoHood 

#### 2) Code controlling the device and controlling the device

```c
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

// the loop function runs over and over again forever
// it communicates with the device and changes volume according to the gesture given
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

In the code there were many areas that I was not acquinted with, hence I added comments to it for reference.


## 4. Upload the code:
The next step is to on the device.\
After the code uploads, open the Serial Monitor and hold your finger on the APDS9960 sensor for 3 seconds to activate it. Then start waving your hand left/right over the sensor to decrease/increase the computer volume.

## Additional comments:
One can also control the keyboard keys using this device SENSE

Add "Keyboard.begin();" below "Consumer.begin();" in the setup function. Then use the command, "Keyboard.write('');" to press letter/number keys.

For example, the command "Keyboard.write('4');" types the letter 4 on the computer each time called.

Also, check out the HID library API and example code for more information and ideas: https://github.com/NicoHood/HID
