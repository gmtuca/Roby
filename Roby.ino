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
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  pinMode(echo, INPUT);
  pinMode(scream, OUTPUT);
  
  go(FORWARD, B);
  go(FORWARD, A);
}

void loop()
{
  int duration, distance; //cm
  digitalWrite(scream, HIGH);
  delayMicroseconds(1000);
  digitalWrite(scream, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2)/29.1;
  
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);

  if(distance > 0 && distance <= 20){
    Serial.println("STOP");
    go(STOP, A);
    go(STOP, B);
    led(ON);
    while(true){}
    
  }
}


