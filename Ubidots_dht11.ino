//Memasukkan Library
#include "UbidotsEsp32Mqtt.h"
#include "DHT.h"

//Variabel Tetap Tanpa = dan ;
#define DHTpin 5 // D5 on the nodemcu ESP8266
#define DHTTYPE DHT11
DHT dht(DHTpin,DHTTYPE);
/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-wGV4KnaDVCba4zsp4yWBujB9vKDb8h";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL_1 = ""; // Put here your Variable label to which data  will be published
char const *VARIABLE_LABEL_2 = "";
const int PUBLISH_FREQUENCY = 2000; // Update rate in milliseconds

unsigned long timer;
//int static const uint8_t DHTPin = 2; // Pin used to read data from GPIO34 ADC_CH6.
//int DHTPin = 5; // Pin used to read data from GPIO5 ADC_CH6.

Ubidots ubidots(UBIDOTS_TOKEN);
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/
void setup(){
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("DHT11 test!");
  dht.begin();
  ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  timer = millis();
}

void loop(){
  // put your main code here, to run repeatedly:
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  if (abs(millis() - timer) > PUBLISH_FREQUENCY) // triggers the routine every 5 seconds
  {
    float value1 = dht.readTemperature();
    float value2 = dht.readHumidity(); 
    ubidots.add(VARIABLE_LABEL_1, value1); // Insert your variable Labels and the value to be sent
    ubidots.add(VARIABLE_LABEL_2, value2);
    ubidots.publish(DEVICE_LABEL);
    timer = millis();
  }
  delay(2000);
  ubidots.loop();
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
//  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print("%\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print("°C");
  Serial.print(" || ");
  Serial.print(f);
  Serial.print("°F\n");
//  Serial.print("Heat index: ");
//  Serial.print(hi);
//  Serial.println(" *F");
}
