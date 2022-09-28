#include <Servo.h>
#include <stdio.h>

Servo myservo;


// pr : photoresistor
// vb : vibration sensor
// bp : bumper

// PINS

/* SERVO PINS *//*
  8: Servo1
  9: Servo2

/* ANALOG PINS *//*
  A1: button1 (photoresistor)
  A2: button2 (photoresistor)
    A3: sensor (photoresistor)
*/


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
int button1Output = 0;
int button2Output = 0;
const int bt1 = A0;
const int bt2 = A1;

bool button1IsPressed = false;
bool button2IsPressed = false;

// not const as we can manually adjust for light levels later
// example, very dark room

// TODO (?): lightMAX can be changed to account for maximum value
// reached by the analog pin, which can be used to activate the paddle
// after reaching (example) 1/2 of lightMAX, for now I am just using lightMIN
// in the code
int lightMAX = 300;
int lightMIN = 100;


// photoresistor
int pr3Output = 0;
const int pr3 = A2;


// vibration sensors
const int vb1 = 11;
const int vb2 = 12;
const int vb3 = 13;

// LEDs
const int bp1LEDs = 10;


// Servos



int openAngle;


// Timer Event function
void timerEvent(void (*function)(bool), unsigned long *clock, 
                bool condition, unsigned long start, 
                unsigned long stop, unsigned long reset) {
  
  if (start <= *clock && *clock <= stop) {
    function(condition);
    //Serial.println(*clock);
  }
  if (*clock > reset) {
    *clock = 0;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(vb1, INPUT);
  pinMode(vb2, INPUT);
  pinMode(vb3, INPUT);
  myservo.attach(8);
  //servo2.attach(9);
}


void loop()
{ 
  // does timer stuff, should be before any other timer methods
  updateTime();
  
  // other timer stuff
  addTime(&bp1Timer);
  
  button1Output = analogRead(A1);
  //button2Output = analogRead(A2);
  //Serial.println(analogRead(A1));
  movePaddle(button1Output < lightMIN, 1, &button1IsPressed);
  //movePaddle(button2Output < lightMIN, &servo2, &button2IsPressed);
  
  
  //timerEvent(&test, &bp1Timer, (digitalRead(vb1) == HIGH), 300, 800, 5000);
  
  
  
  
  // !!! !!! !!! IMPORTANT !!! !!! !!!
  // delay added as tinkercad panics when looping without a delay
  // can remove to test for final design, could make more responsive
  delay(10);
}


void test(bool condition) {
  if (condition) {
    Serial.println("test success"); 
  } else {
    Serial.println("test failure"); 
  }
}


// movePaddle
// *buttonState is required make sure the code runs once, meaning a paddle
// can move to it's target location without spamming code
void movePaddle(bool input, int temp, bool *buttonState) {
  if (input && !*buttonState) {
    Serial.println("Input recieved, set pin: to HIGH"); 
    *buttonState = true;
    
    myservo.write(180);
  }
  if (!input && *buttonState) {
    Serial.println("No input recieved, set pin: to LOW"); 
    *buttonState = false;
    
    myservo.write(0);
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


// time stuff
void updateTime() {
  deltaTime = millis() - previousTickTime;
  previousTickTime = millis();
}

void addTime(unsigned long *timer) {
  *timer += deltaTime;
}
