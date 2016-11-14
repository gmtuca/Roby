#include <IRremote.h>

#define ir 11

IRrecv irrecv(ir);
decode_results results;

//IR Buttons
const int CH_MINUS    = 0xFFA25D;
const int CH          = 0xFF629D;
const int CH_PLUS     = 0xFFE21D;
const int PREV        = 0xFF22DD;
const int NEXT        = 0xFF02FD;
const int PLAY_PAUSE  = 0xFFC23D;
const int VOL_MINUS   = 0xFFE01F;
const int VOL_PLUS    = 0xFFA857;
const int EQ          = 0xFF906F;
const int _0          = 0xFF6897;
const int _100_PLUS   = 0xFF9867;
const int _200_PLUS   = 0xFFB04F;
const int _1          = 0xFF30CF;
const int _2          = 0xFF18E7;
const int _3          = 0xFF7A85;
const int _4          = 0xFF10EF;
const int _5          = 0xFF38C7;
const int _6          = 0xFF5AA5;
const int _7          = 0xFF42BD;
const int _8          = 0xFF4AB5;
const int _9          = 0xFF52AD;

const int REPEATED    = 0xFFFFFFFF;

//Motors
#define enA 10
#define in1 9
#define in2 8

#define enB 5
#define in3 7
#define in4 6

#define LED 13
#define ON true
#define OFF false

#define echo 2
#define scream 3

#define A 0
#define B 1

#define FORWARD 2
#define BACKWARD 3
#define STOP 4

void led(boolean on){
  digitalWrite(LED, on ? HIGH : LOW);
}

void go(int direct, int side){
  int en, i, j;
  
  switch(side){
    case A:
      en = enA;
      i = in1;
      j = in2;
      break; 
    case B:
      en = enB;
      i = in3;
      j = in4;
      break;
    default:
      return;
  }
      
  switch(direct){
    case FORWARD:
      analogWrite(en, 255);
      digitalWrite(i, HIGH);
      digitalWrite(j, LOW);
      break;
    case BACKWARD:
      analogWrite(en, 255);
      digitalWrite(i, LOW);
      digitalWrite(j, HIGH);
      break;
    case STOP:
      analogWrite(en, 0);
      digitalWrite(i, LOW);
      digitalWrite(j, LOW);
    default:
      return;
  }
}

void setup()
{

  Serial.begin(9600);

  irrecv.enableIRIn(); // Start the receiver

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  pinMode(echo, INPUT);
  pinMode(scream, OUTPUT);
}

int getUltrasonicDistance(){
  int duration, distance; //cm
  digitalWrite(scream, HIGH);
  delayMicroseconds(1000);
  digitalWrite(scream, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2)/29.1;

  Serial.print(distance);
  Serial.println(" cm");
  
  return distance;
}

void printWheelInfo(){
  Serial.print("enA: ");
  Serial.print(analogRead(enA));
  Serial.print(", in1: ");
  Serial.print(digitalRead(in1));
  Serial.print(", in2: ");
  Serial.println(digitalRead(in2));

  Serial.print("enB: ");
  Serial.print(analogRead(enB));
  Serial.print(", in3: ");
  Serial.print(digitalRead(in3));
  Serial.print(", in4: ");
  Serial.println(digitalRead(in4));
}

bool obstacle() {
  int distance = getUltrasonicDistance();
  return distance > 0 && distance <= 20;
}

void go_all(int direct){
  go(direct, A);
  go(direct, B);
}

int buttonPressed(){
   if (irrecv.decode(&results)) {
    irrecv.resume(); // Receive the next value
    return results.value;
  }
  else{
    return 0;
  }
}

bool started = false;
bool autopilot = false;

void loop()
{
  delay(150);

  Serial.println(".");
  int b = buttonPressed();
  Serial.println(b);
  if(!started){
    if(b == CH_PLUS){//CH_PLUS = ON button
      Serial.println("START");
      started = true;
      led(ON);
    }
  }
  else if(b == CH_MINUS){//CH_PLUS = OFF button
    Serial.println("STOP");
    started = false;
    go_all(STOP);
    led(OFF);
  }
  else{
    if(b == CH){//CH_PLUS = toggle between autopilot and manual
      autopilot = !autopilot;
      Serial.print("Autopilot: ");
      Serial.print(autopilot ? "ON" : "OFF");
      go_all(STOP);
    }

    if(autopilot){//TODO make this asynchronous...
      if(obstacle()){
        go_all(STOP);
        delay(100);
        go_all(BACKWARD);
        delay(450);
        go(FORWARD, A);
        delay(200);
        go(FORWARD, B);
      }
      else{
        go_all(FORWARD);
      }
    }
    else{//manual
      switch(b){
        case _2: go_all(FORWARD);  break;
        case _8: go_all(BACKWARD); break;
        case _4: go(FORWARD, A);   //turn left
                 go(BACKWARD, B);  break;
        case _6: go(FORWARD, B);   //turn right
                 go(BACKWARD, A);  break;
        case _5: go_all(STOP);     break;
        default:                   break;
      }
    }
  }

  /*
  if(distance > 0 && distance <= 20){
    Serial.println("STOP");
    go(STOP, A);
    go(STOP, B);
    led(ON);
    while(true){}
  }
  */
}


