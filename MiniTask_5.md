# Mini Task 5

## **Wifi Jammer**

### **Introduction**

Recently I viewed this project on hackster.io and the link to this project is given here (https://www.hackster.io/akarsh98/wi-fi-jammer-from-an-esp8266-wifi-jammer-deauther-8187b8). It was a quite simple project yet it was intresting and fun. What this device does is it jams the wifi and even has multiple attacks with which it can attack the wifi. 

---

### **Description**

Here, I describe how the project can be made in my own words. Also, please use it only against your own networks and devices!Please check the legal regulations in your country before using it.I don't take any responsibility for what you do with this program.

So, the parts required for this project are ESP8266 board, NodeMCU style board, he used Firebeetle board from DFRobot as it has on board battery charging and monitoring solution. The board looks like this

![board4](https://raw.githubusercontent.com/Rahil-1707/Task1/main/board4.webp)

Second step is to go and download the arduino IDE and setup it and install esp2866 board. 


The third step is coding the module. 

Write the following code save it as esp8266_deauther.ino
```c
/*
   ===========================================
      Copyright (c) 2018 Stefan Kremser
             github.com/spacehuhn
   ===========================================
 */

extern "C" {
  // Please follow this tutorial:
  // https://github.com/spacehuhn/esp8266_deauther/wiki/Installation#compiling-using-arduino-ide
  // And be sure to have the right board selected
  #include "user_interface.h"
}
#include <EEPROM.h>

#include <ArduinoJson.h>
#if ARDUINOJSON_VERSION_MAJOR != 5
// The software was build using ArduinoJson v5.x
// version 6 is still in beta at the time of writing
// go to tools -> manage libraries, search for ArduinoJSON and install the latest version 5
#error Please upgrade/downgrade ArduinoJSON library to version 5!
#endif

#include "oui.h"
#include "language.h"
#include "functions.h"
#include "Settings.h"
#include "Names.h"
#include "SSIDs.h"
#include "Scan.h"
#include "Attack.h"
#include "CLI.h"
#include "DisplayUI.h"
#include "A_config.h"
#include "webfiles.h"

#include "LED.h"

// Run-Time Variables //
LED led;
Settings settings;
Names    names;
SSIDs    ssids;
Accesspoints accesspoints;
Stations     stations;
Scan   scan;
Attack attack;
CLI    cli;
DisplayUI displayUI;

#include "wifi.h"

uint32_t autosaveTime = 0;
uint32_t currentTime  = 0;

bool booted = false;

void setup() {
    // for random generator
    randomSeed(os_random());

    // start serial
    Serial.begin(115200);
    Serial.println();

    // start SPIFFS
    prnt(SETUP_MOUNT_SPIFFS);
    prntln(SPIFFS.begin() ? SETUP_OK : SETUP_ERROR);

    // Start EEPROM
    EEPROM.begin(4096);

    // auto repair when in boot-loop
    uint8_t bootCounter = EEPROM.read(0);

    if (bootCounter >= 3) {
        prnt(SETUP_FORMAT_SPIFFS);
        SPIFFS.format();
        prntln(SETUP_OK);
    } else {
        EEPROM.write(0, bootCounter + 1); // add 1 to the boot counter
        EEPROM.commit();
    }

    // get time
    currentTime = millis();

    // load settings
    settings.load();

    // set mac for access point
    wifi_set_macaddr(SOFTAP_IF, settings.getMacAP());

    // start WiFi
    WiFi.mode(WIFI_OFF);
    wifi_set_opmode(STATION_MODE);
    wifi_set_promiscuous_rx_cb([](uint8_t* buf, uint16_t len) {
        scan.sniffer(buf, len);
    });

    // set mac for station
    wifi_set_macaddr(STATION_IF, settings.getMacSt());

    // start display
    if (settings.getDisplayInterface()) {
        displayUI.setup();
        displayUI.mode = displayUI.DISPLAY_MODE::INTRO;
    }

    // copy web files to SPIFFS
    copyWebFiles(false);

    // load everything else
    names.load();
    ssids.load();
    cli.load();

    // create scan.json
    scan.setup();

    // set channel
    setWifiChannel(settings.getChannel());

    // load Wifi settings: SSID, password,...
    #ifdef DEFAULT_SSID
    if (settings.getSSID() == "pwned") settings.setSSID(DEFAULT_SSID);
    #endif // ifdef DEFAULT_SSID
    loadWifiConfigDefaults();

    // dis/enable serial command interface
    if (settings.getCLI()) {
        cli.enable();
    } else {
        prntln(SETUP_SERIAL_WARNING);
        Serial.flush();
        Serial.end();
    }

    // start access point/web interface
    if (settings.getWebInterface()) startAP();

    // STARTED
    prntln(SETUP_STARTED);

    // version
    prntln(settings.getVersion());

    // setup LED
    led.setup();
}

void loop() {
    currentTime = millis();

    led.update();    // update LED color
    wifiUpdate();    // manage access point
    attack.update(); // run attacks
    displayUI.update();
    cli.update();    // read and run serial input
    scan.update();   // run scan
    ssids.update();  // run random mode, if enabled

    // auto-save
    if (settings.getAutosave() && (currentTime - autosaveTime > settings.getAutosaveTime())) {
        autosaveTime = currentTime;
        names.save(false);
        ssids.save(false);
        settings.save(false);
    }

    if (!booted) {
        // reset boot counter
        EEPROM.write(0, 0);
        EEPROM.commit();
        booted = true;
#ifdef HIGHLIGHT_LED
        displayUI.setupLED();
#endif // ifdef HIGHLIGHT_LED
    }
}
```

Navigate to Tools > Board. Select the appropriate board that you are using NodeMCU(12E) works in most of the cases. Select the correct comm. port by going to Tools > Port. And then hit the upload button.

When the tab says Done Uploading you are ready to use the device.

Hence, the device is ready to use and can used

Last step is playing with the device and the most fun part. 
- Connect the module with a power supply using the micro USB connector on board or just switch on the switch if you have connected a battery like I have done

- As soon as the module is connected a WiFi named pwned will be hosted. You will have to connect to this WiFi network using a phone or a laptop and enter password as deauther.

- When connected to the device, open an internet browser and navigate to 192.168.4.1. This is the main website from where you can control everything

- Now select the WiFi network you want to attack.

- Move over to the attack tab and select the type of attack you want to perform as:

The device can also be connected with the battery to make it portable as the he does. The below is the picture containing the battery.

![battery_4](https://raw.githubusercontent.com/Rahil-1707/Task1/main/battery_4.webp)

Scan for WiFi devices, block selected connections, create dozens of networks and confuse WiFi scanners!

---

### **Some areas for improvement**

A good casing could be formed around the battery and the microcontroller, to make it durable and preventing it from damage. 

Overall the project was fun looking at and should not be used illegally. And the owner of this repository is not responsible with what you do with this project. 