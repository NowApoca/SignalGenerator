#define DAC1 25
int DACresolution = 256;
float plusDAC = 1/(float(DACresolution));
float n = 0;
float vOutput = 3.3;
bool pulseState = false;
volatile int interruptCounter;
int totalInterruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR sineTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  n = n + plusDAC;
  if(n>1) n = 0;
  dacWrite(DAC1, int(float(DACresolution)*sin(n)));
  portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR pulseTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
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
