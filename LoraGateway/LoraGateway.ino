#include <SPI.h> // include libraries
#include <LoRa.h>

const long frequency = 926E6; // LoRa Frequency
const int csPin = 18;         // LoRa radio chip select
const int resetPin = 12;      // LoRa radio reset
const int irqPin = 26;        // change for your board; must be a hardware interrupt pin

#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "Loraswat";
const char *password = "12345678";

String host = "https://loraswat.com/api/";
String urlGetPelanggan = host + "pelanggan/get";
String urlPostData = host + "data/post?sn=";

String responPostData = "", responGet = "", sn = "", lati = "", longi = "";
float volume = 0, ntu = 0, ph = 0;
boolean statusRx = false;

unsigned long oldTime;

void setup()
{
  Serial.begin(9600); // initialize serial
  while (!Serial);

  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println("Connecting");
 
  connectToWifi();

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
}

void loop()
{
  if (runEvery(5000))
  {
    getPelanggan();

    if (responGet != "") {
      LoRa_sendMessage(responGet);
    }
   
    Serial.println("Send Message!");
  }

  if (statusRx == true) {
    if (runEvery3(5000)) {
      postData();
    }
  
    if (runEvery4(6000)) {
      postDataSensor();
    }
  
    if (runEvery5(4000)) {
      postDataVolume();
    }
  }
}

void consumeJson(String message)
{
  sn      = getValue(message, '#', 0);
  lati    = getValue(message, '#', 1);
  longi   = getValue(message, '#', 2);
  volume  = getValue(message, '#', 3).toFloat();
  ntu     = getValue(message, '#', 4).toFloat();
  ph      = getValue(message, '#', 5).toFloat();
  
  Serial.println("Serial Number : " + sn);
  Serial.println("Latitude : " + lati);
  Serial.println("Longitude : " + longi);
  Serial.println();
  Serial.println("Volume : " + (String) volume + "mL");
  Serial.println();
  Serial.println("NTU : " + (String) ntu);
  Serial.println("PH : " + (String) ph);

  Serial.println();
}

void getPelanggan() {
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    http.begin(urlGetPelanggan.c_str());
    
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code : ");
      Serial.println(httpResponseCode);
      responGet = http.getString();
      Serial.println("Respon dari server web : " + responGet);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);

      responGet = "";
    }
    
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

void postData() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    String serverPath = urlPostData + sn + "&lat=" + lati + "&long=" + longi;

    http.begin(serverPath.c_str());
    
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code : ");
      Serial.println(httpResponseCode);
      responPostData = http.getString();
      Serial.println("Respon dari server web : " + responPostData);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);

      responPostData = "";
    }

    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

void postDataSensor() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    String serverPath = urlPostData + sn + "&ph=" + (String) ph + "&ntu=" + (String) ntu;

    http.begin(serverPath.c_str());
    
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code : ");
      Serial.println(httpResponseCode);
      responPostData = http.getString();
      Serial.println("Respon dari server web : " + responPostData);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);

      responPostData = "";
    }
    
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

void postDataVolume() {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;

    String serverPath = urlPostData + sn + "&volume=" + (String) volume;

    http.begin(serverPath.c_str());
    
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code : ");
      Serial.println(httpResponseCode);
      responPostData = http.getString();
      Serial.println("Respon dari server web : " + responPostData);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);

      responPostData = "";
    }
    
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
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
  if (runEvery2(3000)) {
    String message = "";

    while (LoRa.available())
    {
      message += (char)LoRa.read();
    }
  
    Serial.print("Baca data dari node : ");
    Serial.println(message);

    statusRx = true;
    consumeJson(message);
  }
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

boolean runEvery3(unsigned long interval)
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

boolean runEvery4(unsigned long interval)
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

boolean runEvery5(unsigned long interval)
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

void connectToWifi() {
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println();
  delay(1000);
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
