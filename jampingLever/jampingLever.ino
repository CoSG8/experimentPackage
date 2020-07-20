/*
  Programmed by Akito Kosugi
  ver. 2.2  2020.07.16
*/

#define spkPin  2                // スピーカーはDEGITAL OUT2に接続
#define buttonPin  3
#define ledPin  5
  
int toneTimeTh1 = 150;
int toneTimeTh2 =400;
int toneTimeTh3 = 500;
int taskTimeTh = 3000;
int trialNum = 0;
int successNum = 0;
int iti = 5000;
unsigned long runTime = 0; 
unsigned long taskStartTime = 0; 
unsigned long successStartTime = 0; 
int Fs = 100;
bool value = false;
bool isTaskReady = false;
bool isTask = false;
bool isSuccess = false;


void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(spkPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  
  isTaskReady = true;  
}


void loop() {
  value = digitalRead(buttonPin);
  runTime = millis();
      
  if(value == true){
    if(isTask == false && isTaskReady == true){
      tone(spkPin, 500,toneTimeTh3);
      isTask = true;
      isTaskReady = false;
      taskStartTime = runTime;
      trialNum += 1;
    }
    else if(runTime > taskStartTime+toneTimeTh3){
      if (isTask == true && isSuccess == false){
        isSuccess = true;
        tone(spkPin, 987, toneTimeTh1);
        delay(toneTimeTh1);
        tone(spkPin, 1318, toneTimeTh2);
        successStartTime = runTime;
        successNum += 1;
      }
    }
  }

  // LED
  if(isTask == true){
    digitalWrite(ledPin, HIGH);
    if(runTime>taskStartTime+taskTimeTh){
      isTask = false;
      digitalWrite(ledPin, LOW);
    }
  }
  if(isSuccess == true){
    if(runTime>successStartTime+toneTimeTh1+toneTimeTh2){
      isSuccess = false;
      isTask = false;
      digitalWrite(ledPin, LOW);
    }    
  }

  if(isTaskReady == false){
    if(runTime > taskStartTime+ iti){
      isTaskReady = true;
    }

    
  }
  //シリアルモニターに表示
  Serial.print(runTime, DEC);
  Serial.print(",");
  Serial.print(trialNum, DEC);
  Serial.print(",");
  Serial.print(successNum, DEC);
  Serial.print("\n");
  
  delay(1000 / Fs);

}
