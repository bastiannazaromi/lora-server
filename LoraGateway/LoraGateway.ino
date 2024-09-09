#include <SPI.h> // include libraries
#include <LoRa.h>

const long frequency = 926E6; // LoRa Frequency
const int csPin = 18;         // LoRa radio chip select
const int resetPin = 12;      // LoRa radio reset
const int irqPin = 26;        // change for your board; must be a hardware interrupt pin

#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "TIK Master";
const char *password = "0625118301";

String host = "http://192.168.21.51/lora-swat/";
String sendGps = "sendLatLong";

#include <ArduinoJson.h>

JsonDocument doc;

void setup()
{
  Serial.begin(9600); // initialize serial
  while (!Serial);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency))
  {
    Serial.println("LoRa init failed. Check your connections.");
    while (true)
      ; // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Gateway");
  Serial.println("Only receive messages from nodes");
  Serial.println("Tx: invertIQ enable");
  Serial.println("Rx: invertIQ disable");
  Serial.println();

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();

  doc["serialNumber"]["node-01"]["status"] = "ON";
}

void loop()
{
  if (runEvery(5000))
  { // repeat every 5000 millis
    doc["serialNumber"]["node-01"]["selenoid"] = "OFF";

    String json_message;
    serializeJson(doc, json_message);

    LoRa_sendMessage(json_message); // send a message

    Serial.println("Send Message!");
  }
}

void consumeJson(String message)
{
  int str_len = message.length() + 1;
  char json[str_len];

  message.toCharArray(json, str_len);

  DynamicJsonDocument doc_2(1024);
  DeserializationError error = deserializeJson(doc_2, json);

  // Test if parsing succeeds.
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  String serialNumber = doc_2["serialNumber"];
  String latitude = doc_2["data"]["latitude"];
  String longitude = doc_2["data"]["longitude"];

  Serial.println("Serial Number : " + serialNumber);
  Serial.println("Latitude : " + latitude);
  Serial.println("Longitude : " + longitude);

  Serial.println();
}

void LoRa_rxMode()
{
  LoRa.disableInvertIQ(); // normal mode
  LoRa.receive();         // set receive mode
}

void LoRa_txMode()
{
  LoRa.idle();           // set standby mode
  LoRa.enableInvertIQ(); // active invert I and Q signals
}

void LoRa_sendMessage(String message)
{
  LoRa_txMode();        // set tx mode
  LoRa.beginPacket();   // start packet
  LoRa.print(message);  // add payload
  LoRa.endPacket(true); // finish packet and send it
}

void onReceive(int packetSize)
{
  String message = "";

  while (LoRa.available())
  {
    message += (char)LoRa.read();
  }

  Serial.print("Baca data dari node : ");
  Serial.println(message);

  consumeJson(message);
}

void onTxDone()
{
  Serial.println("TxDone");
  LoRa_rxMode();
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
