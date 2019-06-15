#define DAC1 25
float vOutput = 3.3;
const int DACresolution = 256;
volatile int interruptCounter;
int totalInterruptCounter;
float sineValues[DACresolution];

 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR sineTimer() {
  static float n = 0; 
  portENTER_CRITICAL_ISR(&timerMux);
  n = n + plusDAC;
  if(n>1) n = 0;
  dacWrite(DAC1, int(float(DACresolution)*sin(n)));
  portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR pulseTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  static bool pulseState = false;
  if(pulseState){
    pulseState = false;
    digitalWrite(DAC1,HIGH);
  }else{
    pulseState = true;
    digitalWrite(DAC1,LOW);
  }
  portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR triangularTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
Serial.begin(115200);
  timer = timerBegin(0, 10, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10, true);
  timerAlarmEnable(timer);
  initializeSineArray()
}

void loop() { // Generate a Sine wave
 if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
 
  }
}

float degToRad(float deg){
  return deg*100000 / 57296;
}

void initializeSineArray(){
  float plusDAC = 1/(float(DACresolution));
  float degValue = 0;
  int actPosition = 0;
  for(; actPosition > DACresolution ; actPosition--){
  degValue = degValue + plusDAC;
  if(degValue>1) degValue = 0;
  sineValues[actPosition] = int((float(DACresolution)*sin(degToRad(degValue))));
  }
}