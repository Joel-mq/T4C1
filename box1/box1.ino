#include <Servo.h>
#include <stdio.h>



// pr : photoresistor
// vb : vibration sensor
// bp : bumper

// PINS

/* LED PINS *//*
  5: LED1
  6: LED2
  7: LED3

/* SERVO PINS *//*
  8: Servo1
  9: Servo2
  10: Servo3

/* VIBRATION PINS *//*
  11: vb1
  12: vb2
  13: vb3
  
/* ANALOG PINS *//*
  A1: button1 (photoresistor)
  A2: button2 (photoresistor)
  A3: sensor (photoresistor)
  A4: sensor (photoresistor)
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
const int bt1 = A1;
const int bt2 = A2;

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
int pr4Output = 0;
const int pr3 = A3;
const int pr4 = A4;


// vibration sensors
const int vb1 = 11;
const int vb2 = 12;
const int vb3 = 13;

// LEDs - bp : bumper
const int bp1LEDs = 5;
const int bp2LEDs = 6;
const int bp3LEDs = 7;


// Servos
Servo servo1;
Servo servo2;
Servo servo3;

// TODO
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
  // LEDs - bumpers
  pinMode(bp1LEDs, OUTPUT);
  pinMode(bp2LEDs, OUTPUT);
  pinMode(bp3LEDs, OUTPUT);
  
  // Servos
  servo1.attach(8);
  servo2.attach(9);
  servo2.attach(10);
  
  // vibration sensors
  pinMode(vb1, INPUT);
  pinMode(vb2, INPUT);
  pinMode(vb3, INPUT);
  
}


void loop()
{ 
  // does timer stuff, should be before any other timer methods
  updateTime();
  
  // other timer stuff
  addTime(&time);
  addTime(&bp1Timer);
  addTime(&bp2Timer);
  addTime(&bp3Timer);
  
  button1Output = analogRead(bt1);
  button2Output = analogRead(bt2);
  //Serial.println(analogRead(A1));
  movePaddle(button1Output < lightMIN, 1, &button1IsPressed);
  movePaddle(button2Output < lightMIN, 2, &button2IsPressed);
  
  
  bumperEvent(bp1LEDs, &bp1Timer, (digitalRead(vb1) == HIGH), 1600, 1600);
  bumperEvent(bp2LEDs, &bp2Timer, (digitalRead(vb2) == HIGH), 1600, 1600);
  bumperEvent(bp3LEDs, &bp3Timer, (digitalRead(vb3) == HIGH), 1600, 1600);
  
  
  
  
  // !!! !!! !!! IMPORTANT !!! !!! !!!
  // delay added as tinkercad panics when looping without a delay
  // can remove to test for final design, could make more responsive
  delay(10);
}


// !!! !!! !!!
// TODO, ADJUST POSITION OF SERVOS
// !!! !!! !!!

// movePaddle
// *buttonState is required make sure the code runs once, meaning a paddle
// can move to it's target location without spamming code
// servoMove either 1 or 2, Left: 1, Right: 2, hardcoded as angles differ
void movePaddle(bool input, int servoMove, bool *buttonState) {
  if (input && !*buttonState) {
    Serial.println("Input recieved, set pin: to HIGH"); 
    *buttonState = true;
    
    if (servoMove == 1) {
      servo1.write(180);
    } else if (servoMove == 2) {
      servo2.write(180);
    } else {
      Serial.println("ERROR: invalid servoMove value in movePaddle"); 
    }
  }
  if (!input && *buttonState) {
    Serial.println("No input recieved, set pin: to LOW"); 
    *buttonState = false;
    
    if (servoMove == 1) {
      servo1.write(0);
    } else if (servoMove == 2) {
      servo2.write(0);
    } else {
      Serial.println("ERROR: invalid servoMove value in movePaddle"); 
    }
  }
}


// TODO: rotate lower disk
void rotateDisk() {
  
}

// bumperEvent function
// only runs if *clock has elapsed enough time
void bumperEvent(int pin, unsigned long *clock, 
                bool condition, unsigned long start, 
                unsigned long reset) {
  digitalWrite(pin, LOW); // to ensure blinkPin doesn't leave the LED on HIGH
  if (*clock > reset && condition) {
    *clock = 0;
  }
  if (*clock <= start) {
    blinkPin(0, 200, 400, pin);
  }
  
  //Serial.println(*clock);
}


// blinkPin
// function starts by modding time by mod, giving a repeating
// timer based on how short mod is (in ms)
// once this timer reaches start, LED at PIN set to HIGH
// once timer reaches stop, LED at PIN set to LOW
void blinkPin(int start, int stop, int mod, int pin) {
  int calc = time % mod;
  Serial.println(calc);
  if (stop <= calc) {
    digitalWrite(pin, LOW);
  } else if (start <= calc) {
    digitalWrite(pin, HIGH);
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
