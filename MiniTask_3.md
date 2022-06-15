# **Mini Task 3**
## **Introduction**
A bat signal that lights up when you have a missed call or message. This is a very simple project that I find on the websit instructables. The link for this is https://www.instructables.com/BatSignal-Lights-Up-Everytime-You-Get-an-SMS/

I personally liked the project a lot since it is very useful and apt for me. As I rarely take up my phone, hence I have a lot of missed calls and messages. Also, the instructions to do it are very simple and any beginner could make it.

At the end of the project it looks like this and feels pretty cool.

![batman](https://github.com/Rahil-1707/Task1/blob/main/batman.png)

Hope you give a try for it!!

---
## **Description**

A clear list of the components is given on the page and also the first step of making the lamp is clearly illustrated one can directly use a 3D printer to build it.

The working of the device occurs like this:

![img3](https://github.com/Rahil-1707/Task1/blob/main/img3.png)

So, firt as we recieve a call or message an app on the mobile informs to virtual switch through internet and then the device recievs the signal through internet and then turns it's LED  on. 

The TRIGGER is an app called IFTTT which is free to install on IOS and Android.

The VIRTUAL SWITCH is an online service called Adafruit which is also free to use.

Next one needs to setup adafruit and IFTT. These details are clearly given on the page and one can follow them.

The next part take us to the code. 

``` C
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "..........SSID..........."//Your wifi name
#define WIFI_PASS "...........PASSWORD........."//your wifi password

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "..........Adafruit ID.........." //Your adafruit name
#define MQTT_PASS "..............AIO KEY............" //Your adafruit AIO key

int led = D7;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe Batsignal = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/Batsignal");
void setup()
{
  Serial.begin(9600);

  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi>");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  digitalWrite(LED_BUILTIN, LOW);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(">");
    delay(50);
  }

  Serial.println("OK!");

  //Subscribe to the Batsignal topic
  mqtt.subscribe(&Batsignal);

  pinMode(led, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(led, LOW);

}

void loop()
{
 digitalWrite(led,LOW);

  //Connect/Reconnect to MQTT
  MQTT_connect();

  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &Batsignal)
    {
      //Print the new value to the serial monitor
      Serial.print("LED: ");
      Serial.println((char*) Batsignal.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) Batsignal.lastread, "ON"))
      {
        digitalWrite(led, HIGH);
        delay(120000);  //Change this value to adjust ON time of LED, default is 2 min
        digitalWrite(led,LOW);
      }
      else if (!strcmp((char*) Batsignal.lastread, "OFF"))
      {
        digitalWrite(led, LOW);
       
      }
     
    }
  }

}


void MQTT_connect()
{

  //  // Stop if already connected
  if (mqtt.connected() && mqtt.ping())
  {
    //    mqtt.disconnect();
    return;
  }

  int8_t ret;

  mqtt.disconnect();

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      ESP.reset();
    }
  }
  Serial.println("MQTT Connected!");
}
```

The code is simple and straight forward also one doesn't need to go in it's detail. However, there are a few things to note here

1. Replace …………SSID.......... with your WiFi (or mobile hotspot) name.

2. Replace…………PASSWORD………… with your WiFi (or mobile hotspot) password.

3. Replace …………Adafruit ID………… with your Adafruit username

4. Replace …………AIO key………… with the active key you had noted after creating the Adafruit toggle switch

Also, this is an optional step. In case you want to change the ON time of the LED strip, you can replace 120000 inside delay(120000); with the equivalent millisecond value.



Next we come up to **circuit** connections:

The below is the schematic that one needs to follow to make the circuit.

![batman_scheme](https://github.com/Rahil-1707/Task1/blob/main/batman_scheme.png)

Following this scheme one can easily design the circuit.

Also, with the circuit description is given as below:

- First, look at the last diagram. Let's start with the transistor. Look at the transistor from the top and face the flat side of the transistor to the right.

- Bend the top pin up, bend the bottom pin down and the middle pin to the left.

- Now connect the left facing pin to D7 of the NodeMcu. Connect the bottom facing pin to GND and let the top facing pin stay not connected.

- Now for the relay. Look at the image of the relay. There are 5 pins. I have labeled the two pins on the left as "IN". The pin between the two IN pins as "OUT1" and the NO pin on the right as "OUT2"

- Connect any one of the "IN" pins to the top facing pin of the transistor (which was left unconnected earlier). Connect the other "IN" pin to Vin of the NodeMcu. Now connect "OUT1" to the negative terminal of a 9v battery or power adapter (12v will work too). Leave the "OUT2" pin unconnected.

- Now for the LED strip. Connect the positive conductor of the LED strip to the positive terminal of the battery. Connect the negative conductor of the LED strip to the unconnected "OUT2" pin of the relay.

- All done! To power the circuit, plug in a 5v power supply (a phone charger should work) to the micro-usb port of the NodeMcu. Oh and by the way, you can ignore the 5v battery in the circuit diagram. It just shows the power supply.

**Testing and assembly**
To test if your lamp works, plug in the 5v and 9v power supplies to your circuit. Then wait for a few seconds for the NodeMcu to connect to your wifi network. Make sure your phone (with IFTTT app) is connected to the internet and send an SMS to that phone from a different phone. As soon as you receive the SMS, the LED strip should light up. After about 2 minutes, it should turn off. If it works, great job! If not, here are a few things you can do to troubleshoot:

- Re-check your connections and make sure there are no loose connections.

- Check the applet you created on IFTTT. Open IFTTT and select "My applets" (bottom right). Click on your applet and click on "View activity log". If it says applet failed, you need to recheck your Adafruit and IFTTT setup.

- Check the code to make sure you have provided the right WiFi and Adafruit credentials. Also ensure that you didn't make any other changes to the code.

---

## **Some areas to improve on**
Overall this was highly simple project and one can learn a lot from it. Some areas where it could be improved are:
- Providing different coloured lights for messages and calls. As we know calls are more important than messages. 
- Blinking the lights again after an hour again so as to remind us that we had a missed call or message earlier.

Also, these depends on the point of view of the person and the releative use for which the device owner is using it for. Some might like them or some would not.