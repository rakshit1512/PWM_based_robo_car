int t1=A0;
int t2=A1;
int pin5=5;
int pin6=6;
int pin8=8;
int pin7=7;
int irPin=A2;
int flag=0;
unsigned long d=0;
static int gantryCounter=0;
static long StartTime=0;
static long CurrentTime = 0;
unsigned long ElapsedTime = 0;
static long StartTimeG=millis();
static long CurrentTimeG = 0;
unsigned long ElapsedTimeG = 0;
long previousMillisU = millis();
long intervalU = 500;
#include <NewPing.h>
#define TRIGGER_PIN 13
#define ECHO_PIN 12
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup(){
  pinMode(pin5,OUTPUT);
  pinMode(pin6,OUTPUT);
  pinMode(pin7,OUTPUT);
  pinMode(pin8,OUTPUT);
  pinMode(t1,INPUT);
  pinMode(t2,INPUT);
  Serial.begin(9600);
}

void loop(){
if (Serial.available() > 0){
  char s = Serial.read();
  switch(s){
    case 'A':{
      flag=1;
    }
  }
}
unsigned long currentMillisU = millis();
if(currentMillisU - previousMillisU > intervalU){
  previousMillisU = currentMillisU; detectObstacle();
}
if (flag==1){
  gantry();
}
if (flag==3){
//gantryParking();
  CurrentTimeG=millis();
  ElapsedTimeG = CurrentTimeG-StartTimeG;
  if(ElapsedTimeG<2000){
    flag=3;
    leftBlind();
  }
  if(ElapsedTimeG>2000 && ElapsedTimeG<7500){
    flag=3;
    normalLineFollow();
  }
if(ElapsedTimeG>7500){
  stopBuggy();
  Serial.print("Buggy:1 Parked");
  Serial.println(ElapsedTimeG);
  delay(2000);
  flag=-1;
  }
}
}
void gantry(){
  int r1=digitalRead(t1);
  int r2=digitalRead(t2);
  if(r1==LOW&&r2==LOW){
    digitalWrite(pin5,HIGH);
    digitalWrite(pin6,LOW);
    digitalWrite(pin8,HIGH);
    digitalWrite(pin7,LOW);
  }
  if(r1==HIGH&&r2==LOW){
    digitalWrite(pin5,LOW);
    digitalWrite(pin6,HIGH);
    digitalWrite(pin8,HIGH);
    digitalWrite(pin7,LOW);
  }
  if(r1==LOW&&r2==HIGH){
    digitalWrite(pin5,HIGH);
    digitalWrite(pin6,LOW);
    digitalWrite(pin7,HIGH);
    digitalWrite(pin8,LOW);
  }
  if(r1==HIGH&&r2==HIGH){
    digitalWrite(pin5,HIGH);
    digitalWrite(pin6,LOW);
    digitalWrite(pin8,HIGH);
    digitalWrite(pin7,LOW);
  }
  if (digitalRead(irPin)==HIGH){
    StartTime = millis();
    d = pulseIn(irPin,HIGH);
    if(d > 500 and d < 1500){
      //Serial.println(d);
      Serial.println("Gantry: 1");
      stopBuggy();
      delay(1000);
    }
    else if (d> 1500 and d < 2500){
      // Serial.println(d);
      Serial.println("Gantry: 2");
      gantryCounter=gantryCounter+1;
      Serial.print("The gantry Counter is: ");
      Serial.println(gantryCounter);
      stopBuggy();
      delay(1000);
     }
     else if (d > 2500 and d < 3500){
        // Serial.println(d);
        Serial.println("Gantry: 3");
        stopBuggy();
        delay(1000);
      }
      else {
        //Serial.println(d);
        Serial.println("Gantry: Unknown");
      }
      if (gantryCounter>=2){
        StartTimeG = millis();
        flag=3;
      }
      else{
        gantry();
      }
    }
   }
  void stopBuggy(){
    digitalWrite(pin5,LOW);
    digitalWrite(pin6,LOW);
    digitalWrite(pin7,LOW);
    digitalWrite(pin8,LOW);
  }
  void normalLineFollow(){
  int r1=digitalRead(t1);
  int r2=digitalRead(t2);
  if(r1==LOW&&r2==LOW){
    digitalWrite(pin5,HIGH);
    digitalWrite(pin6,LOW);
    digitalWrite(pin8,HIGH);
    digitalWrite(pin7,LOW);
  }
  if(r1==HIGH&&r2==LOW){
    digitalWrite(pin5,LOW);
    digitalWrite(pin6,HIGH);
    digitalWrite(pin8,HIGH);
    digitalWrite(pin7,LOW);
  }
  if(r1==LOW&&r2==HIGH){
    digitalWrite(pin5,HIGH);
    digitalWrite(pin6,LOW);
    digitalWrite(pin7,HIGH);
    digitalWrite(pin8,LOW);
  }
  if(r1==HIGH&&r2==HIGH){
    digitalWrite(pin5,HIGH);
    digitalWrite(pin6,LOW);
    digitalWrite(pin8,HIGH);
    digitalWrite(pin7,LOW);
    stopBuggy();
   }
  }
void leftBlind(){
  int r2=digitalRead(t2);
  if(r2==LOW){
    digitalWrite(pin5,LOW);
    digitalWrite(pin6,LOW);
    digitalWrite(pin7,HIGH);
    digitalWrite(pin8,LOW);
  }
  if(r2==HIGH){
    digitalWrite(pin5,HIGH);
    digitalWrite(pin6,LOW);
    digitalWrite(pin7,HIGH);
    digitalWrite(pin8,LOW);
  }
}
void detectObstacle(){
  delay(50);
  unsigned int distanceCm;
  distanceCm = sonar.ping_cm();
  pinMode(ECHO_PIN,OUTPUT);
  digitalWrite(ECHO_PIN,LOW); pinMode(ECHO_PIN,INPUT);
  //Serial.print("Ping: ");
  //Serial.println(distanceCm);
  //Serial.println("cm");
  if((distanceCm<15) && (distanceCm>0)){
    stopBuggy();
    delay(1000);
   }
  }
