// all time units in ms
unsigned long time = 0;
unsigned long LOOP_TIME = 1000;
unsigned long previousTickTime = 0;

void setup()
{
  Serial.begin(9600);
  /*
  pinMode(8, OUTPUT);
  pinMode(9, INPUT);
  
  pinMode(LED_BUILTIN, OUTPUT);
  */
}

void loop()
{
  //blinkPin(100, 250, 500, 8);
  
  // changing time by time changed during loop
  resetTimer(&time, LOOP_TIME);
  time += millis() - previousTickTime;
  previousTickTime = millis();
  
}

// blinkPin
// function starts by modding time by mod, giving a repeating
// timer based on how short mod is (in ms)
// once this timer reaches start, LED at PIN set to HIGH
// once timer reaches stop, LED at PIN set to LOW
void blinkPin(int start, int stop, int mod, int PIN) {
  int calc = time % mod;
  //Serial.println(calc);
  if (stop <= calc) {
    digitalWrite(PIN, LOW);
  } else if (start <= calc) {
    digitalWrite(PIN, HIGH);
  }
}

// makes sure *time stays small, adjust value if desired
void resetTimer(unsigned long *time, unsigned long maxTime) {
  
  if (*time >= maxTime) {
    Serial.println(*time);
    *time -= maxTime;
  }
}
