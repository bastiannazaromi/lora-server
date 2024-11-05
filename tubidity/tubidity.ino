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

void setup() {
  Serial.begin(9600);
  
    // set up the ADC
  ADCSRA &= ~PS_128;  // remove bits set by Arduino library

  // you can choose a prescaler from above.
  // PS_16, PS_32, PS_64 or PS_128
  ADCSRA |= PS_64;    // set our own prescaler to 64

}


void loop(){
  unsigned int i;
  unsigned int z;
  z = 0;
   
  // capture the values to memory
  for(i = 0; i < 300; i++) {
    start_times[i] = micros();
    values[i] = analogRead(A0);             
 
    if (values[i] >= z) {
      z = values[i]; 
    }
   
    stop_times[i] = micros();
  }
 
  float ntu = (z - 912.5) / -0.279;
    
  Serial.print("ADC : ");
  Serial.println(z);
 
  Serial.print("NTU : ");
  Serial.println(ntu);
 
  delay(1000);
}

// Turbidity
int pinTurbidity = A0;
float kekeruhan;
float teg;
String air;

void setup() {
  Serial.begin(9600); // Inisialisasi komunikasi serial
}

void loop() {
  int sensorValue = analogRead(A0);
  teg = sensorValue * (5.0 / 1024.0);
  kekeruhan = 100 - (sensorValue / 10.24);

  Serial.print("Sensor Turbidity Output (V) : ");
  Serial.println(teg);
  Serial.print("Kekeruhan Air : ");
  Serial.println(kekeruhan);
  Serial.print("\n");


  delay(1000); // Tunggu 1 detik sebelum pengukuran berikutnya
}

