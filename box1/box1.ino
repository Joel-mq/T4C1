// pr : photoresistor
// vb : vibration sensor
// bp : bumper

// all time units in ms
unsigned long time = 0;
unsigned long LOOP_TIME = 10000;
unsigned long previousTickTime = 0;
unsigned long deltaTime = 0;

// bumper timers
unsigned long bp1Timer = 0;
unsigned long bp2Timer = 0;
unsigned long bp3Timer = 0;


// paddle timers
unsigned long timeSinceLastButtonInput = 0;


// buttons (which are actually photoresistors but for 
//          the sake of readability they are buttons)
int pr1Output = 0;
int pr2Output = 0;
const int pr1 = A0;
const int pr2 = A1;

bool button1IsPressed = false;
bool button2IsPressed = false;

// not const as we can manually adjust for light levels later
// example, very dark room
int paddleLightSensitivity = 300;


// photoresistor
int pr3Output = 0;
const int pr3 = A2;


// vibration sensors
const int vb1 = 11;
const int vb2 = 12;
const int vb3 = 13;

// LEDs
const int bp1LEDs = 10;


// motors



void setup()
{
  Serial.begin(9600);
  pinMode(vb1, INPUT);
  pinMode(vb2, INPUT);
  pinMode(vb3, INPUT);
}


void loop()
{ 
  // does timer stuff, should be before any other timer methods
  updateTime();
  
  // other timer stuff
  addTime(&bp1Timer);
  
  pr1Output = analogRead(A0);
  pr2Output = analogRead(A1);
  movePaddle(pr1Output < paddleLightSensitivity, 8, &button1IsPressed);
  movePaddle(pr2Output < paddleLightSensitivity, 9, &button2IsPressed);
  
  
  if (digitalRead(vb1) == HIGH && bp1Timer >= 1000) {
    bp1Timer = 0;
    Serial.println("vb1");
  }
  
  
  
  // !!! !!! !!! IMPORTANT !!! !!! !!!
  // delay added as tinkercad panics when looping without a delay
  // can remove to test for final design, could make more responsive
  delay(10);
}


void updateTime() {
  deltaTime = millis() - previousTickTime;
  previousTickTime = millis();
}

void addTime(unsigned long *timer) {
  *timer += deltaTime;
}


// movePaddle
// *buttonState is required make sure the code runs once, meaning a paddle
// can move to it's target location without spamming code
void movePaddle(bool input, int output, bool *buttonState) {
  if (input && !*buttonState) {
    Serial.println("Input recieved, set pin: " + (String)output + " to HIGH"); 
    *buttonState = true;
    
    /*
      ADD MOTOR STUFF HERE
    */
  }
  if (!input && *buttonState) {
    Serial.println("No input recieved, set pin: " + (String)output + " to LOW"); 
    *buttonState = false;
    
    /*
      ADD MORE MOTOR STUFF HERE
    */
  }
}


void bumper(int input, int output, int timeActive) {
  
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
