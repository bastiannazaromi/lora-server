#include <SPI.h>
#include <LoRa.h>

const long frequency = 926E6;  // LoRa Frequency

const int csPin = 10;          // LoRa radio chip select
const int resetPin = 9;        // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial SerialGPS(3, 4); 

float Latitude , Longitude;
String LatitudeString, LongitudeString;
boolean status_gps = false;

#include <ArduinoJson.h>

JsonDocument doc;

String serialNumber = "node-01";

#define pinSelenoid 7

void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600);

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();

  doc["serialNumber"] = serialNumber;

  pinMode(pinSelenoid, OUTPUT);
  digitalWrite(pinSelenoid, LOW);
  
  Serial.println("Starting Machine");
  
  Serial.println();
}

void loop()
{
  if (runEvery(1000)) { // repeat every 1000 millis
    readGps();
  
    if (status_gps == true) {
      doc["data"]["latitude"] = LatitudeString;
      doc["data"]["longitude"] = LongitudeString;
    } else {
      doc["data"]["latitude"] = NULL;
      doc["data"]["longitude"] = NULL;
    }

    String json_message;
    serializeJson(doc, json_message);
    
    LoRa_sendMessage(json_message); // send a message
  }

  delay(500);
}

void readGps() {
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude, 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude, 6);
      }
    }
  }
  
  if (gps.location.isValid())
  {
    status_gps = true;
  } else {
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println(F("No GPS detected: check wiring."));
      Serial.println();
    } else {
      Serial.println(F("No GPS Signal."));
      Serial.println();
    }

    status_gps = false;
  }
}

void consumeJson(String message) {
  int str_len = message.length() + 1;
  char json[str_len];
  
  message.toCharArray(json, str_len);

  DynamicJsonDocument doc_2(1024);
  DeserializationError error = deserializeJson(doc_2, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  String statusPelanggan = doc_2["serialNumber"][serialNumber]["status"];
  String statusSelenoid = doc_2["serialNumber"][serialNumber]["selenoid"];

  // Print values.
  Serial.println("Status Pelanggan : " + statusPelanggan);
  Serial.println("Status Selenoid : " + statusSelenoid);

  Serial.println();
}

void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }

  Serial.print("Baca data dari gateway : ");
  Serial.println(message);

  consumeJson(message);
}

void onTxDone() {
  Serial.println("Berhasil kirim data ke Gateway");
  LoRa_rxMode();
  Serial.println();
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
