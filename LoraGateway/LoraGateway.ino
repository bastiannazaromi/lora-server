#include <SPI.h> // include libraries
#include <LoRa.h>

const long frequency = 926E6; // LoRa Frequency
const int csPin = 18;         // LoRa radio chip select
const int resetPin = 12;      // LoRa radio reset
const int irqPin = 26;        // change for your board; must be a hardware interrupt pin

#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "Harapan Bersama";
const char *password = "poltekharber";

String host = "http://loraswat.com/api/";
String urlGetPelanggan = host + "pelanggan/get";
String urlPostData = host + "data/post?sn=";

#include <ArduinoJson.h>

JsonDocument doc;

String responPostData = "", responGet = "", sn = "", lati = "", longi = "";
float volume = 0, ntu = 0, ph = 0;

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

  Serial.println();
  delay(1000);

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

  String serialNumber = doc_2["sn"];
  String latitude     = doc_2["data"]["latitude"];
  String longitude    = doc_2["data"]["longitude"];
  String nilai_volume = doc_2["data"]["volume"];
  String nilai_ntu    = doc_2["data"]["ntu"];
  String nilai_ph     = doc_2["data"]["ph"];

  sn      = serialNumber;
  lati    = latitude;
  longi   = longitude;
  volume  = nilai_volume.toFloat();
  ntu     = nilai_ntu.toFloat();
  ph      = nilai_ph.toFloat();
  
  Serial.println("Serial Number : " + serialNumber);
  Serial.println("Latitude : " + latitude);
  Serial.println("Longitude : " + longitude);
  Serial.println();
  Serial.println("Volume : " + nilai_volume + "mL");
  Serial.println();
  Serial.println("NTU : " + nilai_ntu);
  Serial.println("PH : " + nilai_ph);

  Serial.println();
}

void getPelanggan() {
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(urlGetPelanggan.c_str());
    
    // If you need Node-RED/server authentication, insert user and password below
    //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
    
    // Send HTTP GET request
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
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

void postData() {
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    String serverPath = urlPostData + sn + "&lat=" + lati + "&long=" + longi;

    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());
    
    // Send HTTP GET request
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
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

void postDataSensor() {
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    String serverPath = urlPostData + sn + "&ph=" + (String) ph + "&ntu=" + (String) ntu;

    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());
    
    // Send HTTP GET request
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
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

void postDataVolume() {
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    String serverPath = urlPostData + sn + "&volume=" + (String) volume;

    // Your Domain name with URL path or IP address with path
    http.begin(serverPath.c_str());
    
    // Send HTTP GET request
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
    // Free resources
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
