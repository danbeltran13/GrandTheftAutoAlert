#include <IRremote.h>

//ULTRASONIC 1
#define TRIG1 4
#define ECHO1 5
//ULTRASONIC 2
#define TRIG2 6
#define ECHO2 7
//ULSTRASONIC 3
#define TRIG3 8
#define ECHO3 9
//ULSTRASONIC 4
#define TRIG4 10
#define ECHO4 11

int setDistance[4];
int maxChange = 15;
int ledPin = 2;
int alarmPin = 3;
int redPin = A5;
int greenPin = A4;
bool armed = false;
const int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(TRIG3, OUTPUT);
  pinMode(ECHO3, INPUT);
  pinMode(TRIG4, OUTPUT);
  pinMode(ECHO4, INPUT);

  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);

  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
}

int distance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long dist = pulseIn(echo, HIGH) / 58.2;
  return dist;

}
void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    irrecv.resume();
    if (results.value == 16753245) {

      //This, happens when arming
      if (armed == true) {
        Serial.println("UnArmed");
        armed = false;
        digitalWrite(ledPin, LOW);
        tone( 3, 1500, 100);
        delay(500);



      } else {
        Serial.println("Armed");
        armed = true;
        digitalWrite(ledPin, HIGH);
        tone( 3, 1500, 100);
        delay(100);
        tone( 3, 1500, 100);
        delay(1000);

        // sets Distance
        setDistance[0] = distance(4, 5);
        setDistance[1] = distance(6, 7);
        setDistance[2] = distance(8, 9);
        setDistance[3] = distance(10, 11);
        
      }
    }

  }

  if( armed == true){
  int newDistance[4];
  newDistance[0] = distance(4, 5);
  newDistance[1] = distance(6, 7);
  newDistance[2] = distance(8, 9);
  newDistance[3] = distance(10, 11);

  for (int i = 0; i < 4; ++i) {
    // Checks change in distance
    if (abs(newDistance[i] - setDistance[i]) > maxChange) {
      Serial.println("MOTION DETECTED!!");
      Serial.print(i);
      Serial.print(" ");
      Serial.println(newDistance[i] - setDistance[i]);
      for (int i = 500; i < 2000; i += 10) {
        if(i%20==0){
          digitalWrite(redPin,HIGH);  
          digitalWrite(greenPin,LOW);  
        }
         if(i%40 ==0){
          digitalWrite(redPin,LOW);  
          digitalWrite(greenPin,HIGH);  
        }
         
        tone( 3, i, 20);
        delay(20);
      }
      for (int i = 2000; i > 500; i -= 10) {
        tone( 3, i, 20);
                if(i%20==0){
          digitalWrite(redPin,HIGH);  
          digitalWrite(greenPin,LOW);  
        }
         if(i%40==0){
          digitalWrite(redPin,LOW);  
          digitalWrite(greenPin,HIGH);  
        }
         
        delay(20);
      }
                digitalWrite(redPin,LOW);  
          digitalWrite(greenPin,LOW); 
    }

    
  }
  }
  delay(10);

}
