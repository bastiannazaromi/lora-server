// LORA
#include <SPI.h>
#include <LoRa.h>

const long frequency = 915E6;  // LoRa Frequency

const int csPin = 10;          // LoRa radio chip select
const int resetPin = 9;        // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin

// GPS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5);

float Latitude , Longitude;
String LatitudeString, LongitudeString;
boolean status_gps = false;

// WATER FLOW
byte sensorInt    = 1;
byte flowsensor   = 3;
float konstanta   = 9.5; //konstanta flow meter
volatile byte pulseCount;
float debit;
unsigned int flowmlt;
float totalmlt;
unsigned long oldTime;
float liter       = 0;

// Turbidity
int pinTurb = A0;
float V;
float ntu;
float VRata2;
float VHasil;

unsigned long oldTime2;

// PH
#define pinPh A1
unsigned long int avgValue;
float b, phValue;
int buf[10], temp;
unsigned long oldTime3;

#define pinSelenoid 7

String serialNumber = "node-01";

void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600);

  Serial.println();
  Serial.println("Starting Machine");

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

  // Setup Selenoid
  pinMode(pinSelenoid, OUTPUT);
  digitalWrite(pinSelenoid, LOW);

  // Setup WaterFlow
  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH);

  pulseCount  = 0;
  debit       = 0.0;
  flowmlt     = 0;
  totalmlt    = 0;
  oldTime     = 0;

  attachInterrupt(sensorInt, pulseCounter, FALLING);

  Serial.println();
}

void loop()
{
  bacaWaterFlow();
  bacaTurbidity();
  bacaPh();
  
  if (runEvery(2000)) { // repeat every 3000 millis
    readGps();
  
    if (status_gps == false) {
      LatitudeString = "0";
      LongitudeString = "0";
    }

    String msg = (String) serialNumber + "#" + (String) LatitudeString + "#" + (String) LongitudeString + "#" + (String) totalmlt + "#" + (String) ntu + "#" + String(phValue, 2) + "#OK";
    
    Serial.println();

    Serial.println("Kirim ke Gateway : " + msg);
    LoRa_sendMessage(msg); // send a message
  }

  delay(200);
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
  String respon = getValue(message, '&', 0);

  Serial.print("Baca data dari gateway : ");
  Serial.println(respon);
  
  String statusPelanggan = getValue(respon, '#', 1);
  String statusSelenoid = getValue(respon, '#', 2);

  // Print values.
  Serial.println("Status Pelanggan : " + statusPelanggan);

  if (statusSelenoid == "ON") {
    Serial.println("Status Kran : Tutup");
    
    digitalWrite(pinSelenoid, HIGH);
  } else {
    Serial.println("Status Kran : Buka");
    
    digitalWrite(pinSelenoid, LOW);
  }

  Serial.println();
}

void bacaWaterFlow() {
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

    pulseCount = 0;

    attachInterrupt(sensorInt, pulseCounter, FALLING);
  }
}


void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}

void bacaTurbidity() {
  if ((millis() - oldTime2) > 1000)
  {
    oldTime2 = millis();
    
    V = 0;
    for(int i = 0; i < 800; i++)
    {
      V += ((float)analogRead(pinTurb) / 1023) * 5;
    }
  
    VRata2 = V / 800;
    VHasil = roundf(VRata2 * 10.0f) / 10.0f;
  
    if(VHasil < 2.5)
    {
      ntu = 3000;
    }
    else
    {
      ntu = -1120.4 * square(VHasil) + 5742.3 * VHasil - 4353.8;
    }

    if (ntu < 0) {
      ntu = 0;
    }
  
    Serial.print("Tegangan : ");
    Serial.print(VHasil);
    Serial.print(" V");
  
    Serial.print("\t kekeruhan : ");
    Serial.print(ntu);
    Serial.println(" ntu");
    delay(10);
    
    Serial.println();
  }
}

void bacaPh() {
  if ((millis() - oldTime3) > 5000)
  {
    oldTime3 = millis();

    for (int i = 0; i < 10; i++)       //Get 10 sample value from the sensor for smooth the value
    { 
      buf[i] = analogRead(pinPh);
      delay(10);
    }
    
    for (int i = 0; i < 9; i++)        //sort the analog from small to large
    {
      for (int j = i + 1; j < 10; j++)
      {
        if (buf[i] > buf[j])
        {
          temp = buf[i];
          buf[i] = buf[j];
          buf[j] = temp;
        }
      }
    }
    
    avgValue = 0;
    
    for (int i = 2; i < 8; i++) {                      //take the average value of 6 center sample
      avgValue += buf[i];
    }
    
    phValue = (float) avgValue * 5.0 / 1024 / 6; //convert the analog into millivolt
    phValue = 3.5 * phValue;                      //convert the millivolt into pH value
    
    Serial.print("pH : ");  
    Serial.println(phValue, 2);
    
    Serial.println();
  }
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
  if (runEvery2(3000)) {
    String message = "";

    while (LoRa.available()) {
      message += (char)LoRa.read();
    }
  
    consumeJson(message);
  }
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

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
 
  for(int i=0; i <= maxIndex && found <= index; i++){
    if(data.charAt(i) == separator || i == maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  } 
 
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
