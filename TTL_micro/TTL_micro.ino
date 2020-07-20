/*
  TTL_micro.ino
  Programmed by Akito Kosugi
  ver. 1.0    2017.03.26
  ver. 1.1    2017.04.19
 */

#define volumePin   A0
#define bncPin    10    
#define spkPin    9   
#define ledPin    13    

int volumeVal;
int volumeMin;
float volumeMax;
float duration;
float maxDuration;
int iti;
unsigned long time = 0;

void setup() {

  pinMode(bncPin, OUTPUT);
  pinMode(spkPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  volumeMax = 1023;
  
  iti = 1000;
  maxDuration = 10;

  Serial.begin(9600);
  Serial.println("Dump A/D Value");

}

void loop() {

  volumeVal = analogRead(volumePin);
  duration = ((volumeMax - volumeVal)/volumeMax) * maxDuration;
  
  time = millis();
  while(millis() < time + duration){
    digitalWrite(bncPin,HIGH);
    digitalWrite(ledPin,HIGH);
  }
  digitalWrite(bncPin,LOW);
  digitalWrite(ledPin,LOW);

  delay(iti);
  
  Serial.print(volumeVal, DEC);
  Serial.print(",");
  Serial.print(duration, DEC);
  Serial.print("\n");

}
