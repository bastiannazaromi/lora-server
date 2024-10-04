// LORA
#include <SPI.h>
#include <LoRa.h>

const long frequency = 926E6; // LoRa Frequency

const int csPin = 10;   // LoRa radio chip select
const int resetPin = 9; // LoRa radio reset
const int irqPin = 2;   // change for your board; must be a hardware interrupt pin

// GPS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5);

float Latitude, Longitude;
String LatitudeString, LongitudeString;
boolean status_gps = false;

// JSON
#include <ArduinoJson.h>

DynamicJsonDocument doc(1024); // Ubah ukuran buffer sesuai kebutuhan

// WATER FLOW
byte sensorInt = 1;
byte flowsensor = 2;
float konstanta = 6; // konstanta flow meter
volatile byte pulseCount;
float debit;
unsigned int flowmlt;
float totalmlt;
unsigned long oldTime;
float liter = 0;

// TURBIDITY
#include "Wire.h"

// Arrays to save our results in
unsigned long start_times[300];
unsigned long stop_times[300];
unsigned long values[300];

// Define various ADC prescaler
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

#define pinTurbidity A0
unsigned long oldTime2;

String serialNumber = "node-02";

#define pinSelenoid 7

void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600);

  Serial.println("Starting Machine");

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency))
  {
    Serial.println("LoRa init failed. Check your connections.");
    while (true)
      ; // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();

  doc["sn"] = serialNumber;

  // Setup Selenoid
  pinMode(pinSelenoid, OUTPUT);
  digitalWrite(pinSelenoid, LOW);

  // Setup WaterFlow
  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH);

  pulseCount = 0;
  debit = 0.0;
  flowmlt = 0;
  totalmlt = 0;
  oldTime = 0;

  attachInterrupt(sensorInt, pulseCounter, FALLING);

  // Setup Turbidity
  // set up the ADC
  ADCSRA &= ~PS_128; // remove bits set by Arduino library

  // you can choose a prescaler from above.
  // PS_16, PS_32, PS_64 or PS_128
  ADCSRA |= PS_64; // set our own prescaler to 64

  Serial.println();
}

void loop()
{
  bacaWaterFlow();
  bacaTurbidity();

  if (runEvery(3000))
  { // repeat every 3000 millis
    readGps();

    if (status_gps == true)
    {
      doc["data"]["latitude"] = LatitudeString;
      doc["data"]["longitude"] = LongitudeString;
    }
    else
    {
      doc["data"]["latitude"] = NULL;
      doc["data"]["longitude"] = NULL;
    }

    String json_message;
    serializeJson(doc, json_message);
    serializeJson(doc, Serial);

    Serial.println();

    LoRa_sendMessage(json_message); // send a message
  }

  delay(200);
}

void readGps()
{
  while (SerialGPS.available() > 0)
  {
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
  }
  else
  {
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println(F("No GPS detected: check wiring."));
      Serial.println();
    }
    else
    {
      Serial.println(F("No GPS Signal."));
      Serial.println();
    }

    status_gps = false;
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

  String statusPelanggan = doc_2[serialNumber][0];
  String statusSelenoid = doc_2[serialNumber][1];

  // Print values.
  Serial.println("Status Pelanggan : " + statusPelanggan);
  Serial.println("Status Selenoid : " + statusSelenoid);

  if (statusSelenoid == "ON")
  {
    digitalWrite(pinSelenoid, HIGH);
  }
  else
  {
    digitalWrite(pinSelenoid, LOW);
  }

  Serial.println();
}

void bacaWaterFlow()
{
  if ((millis() - oldTime) > 1000)
  {
    detachInterrupt(sensorInt);
    debit = ((1000.0 / (millis() - oldTime)) * pulseCount) / konstanta;
    oldTime = millis();
    flowmlt = (debit / 60) * 1000;
    totalmlt += flowmlt;

    unsigned int frac;

    Serial.print("Debit air: ");
    Serial.print(int(debit));
    Serial.print("L/min");
    Serial.print("\t");

    Serial.print("Volume: ");
    Serial.print(totalmlt);
    Serial.println("mL");

    doc["data"]["volume"] = totalmlt;

    pulseCount = 0;

    attachInterrupt(sensorInt, pulseCounter, FALLING);
  }
}

void bacaTurbidity()
{
  if ((millis() - oldTime2) > 1000)
  {
    oldTime2 = millis();

    unsigned int i;
    unsigned int z;
    z = 0;

    // capture the values to memory
    for (i = 0; i < 300; i++)
    {
      start_times[i] = micros();
      values[i] = analogRead(pinTurbidity);

      if (values[i] >= z)
      {
        z = values[i];
      }

      stop_times[i] = micros();
    }

    float ntu = (z - 912.5) / -0.279;

    Serial.print("ADC : ");
    Serial.println(z);

    Serial.print("NTU : ");
    Serial.println(ntu);

    doc["data"]["ntu"] = ntu;

    Serial.println();
  }
}

void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}

void LoRa_rxMode()
{
  LoRa.enableInvertIQ(); // active invert I and Q signals
  LoRa.receive();        // set receive mode
}

void LoRa_txMode()
{
  LoRa.idle();            // set standby mode
  LoRa.disableInvertIQ(); // normal mode
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
  if (runEvery2(5000))
  {
    String message = "";

    while (LoRa.available())
    {
      message += (char)LoRa.read();
    }

    Serial.print("Baca data dari gateway : ");
    Serial.println(message);

    consumeJson(message);
  }
}

void onTxDone()
{
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

boolean runEvery2(unsigned long interval)
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