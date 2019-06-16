#define DAC1 25
float vOutput = 3.3;
const int DACresolution = 256;
volatile int interruptCounter;
bool firstOn = true;
bool sineOn = false;
int sineValues[DACresolution];
int triangularValues[DACresolution];
int busFq = 80000000;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR settedUpFunctionsTimer(){
  static int actualValueCount = 0; 
  portENTER_CRITICAL_ISR(&timerMux);
  if(firstOn){
    actualValueCount = 0;
    firstOn = false;
  }
  if(sineOn){
    dacWrite(DAC1, sineValues[actualValueCount]); 
  }else{
    dacWrite(DAC1, triangularValues[actualValueCount]); 
  }
  actualValueCount++;
  if(actualValueCount>DACresolution) actualValueCount = 0;
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

void setup() {
  Serial.begin(115200);
  initializeSineArray();
  initializeTriangularArray(1000,50);
}

void loop() { // Generate a Sine wave
  if(firstOn){
    firstOn = false;
    Serial.print(2);
    int a = 0;
    for(;a<DACresolution;a++){
      Serial.print("\n triangularValues Value: ");
      Serial.print(triangularValues[a]);
    }
  }
}

void initializeTimer(int fq){
  timer = timerBegin(0, int(busFq/fq), true);
  timerAttachInterrupt(timer, &settedUpFunctionsTimer, true);
  timerAlarmWrite(timer, 1, true);
  timerAlarmEnable(timer);
}

float degToRad(float deg){
  return deg*100000 / 57296;
}

int sineGeneratorOn(float deg){
  return deg*100000 / 57296;
}

int pulseOn(float deg){
  return deg*100000 / 57296;
}

int triangularOn(float deg){
  return deg*100000 / 57296;
}

void initializeTriangularArray(int frequency, float trise){
  float period = 1/(float(frequency));
  int inflexionPoint = (DACresolution*trise)/100;
  int actPosition = 0;
  for(; actPosition < DACresolution ; actPosition++){
    if(actPosition < inflexionPoint){
      triangularValues[actPosition] = int(((actPosition*1.0)/inflexionPoint)*DACresolution);
    }else{
      triangularValues[actPosition] = int((((DACresolution - actPosition)*1.0)/(DACresolution - inflexionPoint))*DACresolution);
    }
  }
}

void initializeSineArray(){
  float plusDAC = 1/(float(DACresolution));
  float degValue = 0;
  int actPosition = 0;
  for(; actPosition < DACresolution ; actPosition++){
  degValue = degValue + plusDAC;
  if(degValue>1) degValue = 0;
    sineValues[actPosition] = int((float(DACresolution)*sin(degToRad(degValue))));
  }
}
