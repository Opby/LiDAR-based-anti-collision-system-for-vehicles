/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLypYf-HvQ"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "el1yrmvNSttFlWRpGNX4DstGWgLEN13n"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// Include Libraries
#include "Arduino.h"
#include "LIDARLite.h"
#include "Wire.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


// object initialization
LIDARLite lidarLite;

// Global variables and defines
char auth[] = BLYNK_AUTH_TOKEN;
const unsigned char Passive_buzzer = 14;


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "hojojo";
char pass[] = "20402040";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
  float currentDistance1 = lidarLite.distance(); // try not to send >10 values/secondz
  delay (1000);
  float currentDistance2 = lidarLite.distance();
  float currentDistance = currentDistance2 / 100;
  float speednow = (currentDistance1 - currentDistance2) / 100;
  float speedmph = speednow * 2.2369;
  float hittime;
  if (speednow <= 0) 
  {
    hittime=1000;
  }
  else
  {
    hittime = currentDistance/speednow;
  }
   ;
  Blynk.virtualWrite(V5, currentDistance); 
  Blynk.virtualWrite(V6, speedmph);
  Blynk.virtualWrite(V7, hittime); // send data to app
  if (currentDistance <= 3 or hittime <= 3) {
    tone(Passive_buzzer, 523) ; //DO note 523 Hz
    delay (1000); 
    noTone(Passive_buzzer) ; //Turn off the pin attached to the tone()
    };
  
  Serial.print(currentDistance);
  Serial.println(" [m]");
  Serial.print(speedmph);
  Serial.println(" [mph]");
  Serial.print(hittime);
  Serial.println(" [s]");
}

void setup()
{
  pinMode (Passive_buzzer,OUTPUT) ;
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  lidarLite.begin(0, true); // Set configuration to default '0', change this number to try out alternate configurations. I2C speed set to 400 kHz.
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}
