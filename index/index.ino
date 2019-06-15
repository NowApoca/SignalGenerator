/*
 * Copyright (c) 2018, circuits4you.com
 * All rights reserved.
 * 
 * ESP32 DAC - Digital To Analog Conversion Example
 */

#define DAC1 25
int DACresolution = 256;
float plusDAC = 1/(float(DACresolution));
float n = 0;
float vOutput = 3.3;

void setup() {
  Serial.begin(115200);
}

void loop() { // Generate a Sine wave
  int Value = 128; //255= 3.3V 128=1.65V
  
  dacWrite(DAC1, Value);
  ///Serial.print(sin(n)*1);
  // Serial.print(256.0*sin(n),6);
  Serial.print('\n');
  Serial.print(sin(degToRad(n)),6);
  Serial.print('\n');
  n = n + plusDAC;
  if(n>1) n =0;
}

float degToRad(float deg){
  return deg*100000 / 57296;
}
