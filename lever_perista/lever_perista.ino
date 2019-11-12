/*
  lever_perista.ino
  Akito Kosugi
  ver.1.2 2019.11.12
 
*/

#define LorR    0                // L/RはANALOG IN A0に接続
#define spkPin  2                // スピーカーはDEGITAL OUT2に接続
#define spkPin2 3                // スピーカーはDEGITAL OUT2に接続
#define pumpPin 4

#define ledPin  13                // LEDはDEGITAL OUT13に接続


// 変更するパラメータ
int leverTh = 1000;               // valueが一定時間この値以下だとレバー引いていると判断
int homeTh = 500;  
int holdTimeTh =  200;            // 必要な保持時間 [ms]
int successTimeTh = 300; 
int toneTimeTh1 = 150;
int toneTimeTh2 = 250;
int startTimeTh = 3000; 
int Fs = 100;                     // サンプリングレート [Hz]

// 変数
int value ;                       // joystickからの入力信号
int baseVal = 0;
int trialNum = 0;
unsigned long runTime = 0;        // タイマー
unsigned long holdTime = 0;       // 保持時間を数える変数
unsigned long pullStartTime = 0;  // 保持時間を数える変数
unsigned long holdStartTime = 0;  // 保持時間を数える変数
unsigned long successTime = 0; 
bool isPull = false ;             // レバーを引いているるかのフラグ
bool isHold = false ;             // レバーを保持しているかのフラグ
bool isHome = false ;  
bool isSuccess = false;
bool isPull_pre = false;
bool isHold_pre = false;
bool isPull_start = false;
bool isHold_start = false;
bool isTaskStart = false ;
bool isTaskReady = false ;

void setup() {

  pinMode(LorR, INPUT);
  pinMode(spkPin, OUTPUT);
  pinMode(spkPin2, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(57600);
  Serial.println("value");

}

void loop() {

  runTime = millis();

  //センサーより信号を取得
  value = analogRead(LorR);

  // タスク開始は一定時間後
  if(isTaskStart == false){
    if (runTime > startTimeTh){
      isTaskStart = true;
      isTaskReady = true;
      baseVal = value;
      leverTh = baseVal * 1.7;
      homeTh = baseVal * 1.1;
      digitalWrite(ledPin, LOW);
    }
    else{
      isTaskReady = false;
      digitalWrite(ledPin, HIGH);
    }
  }

  // レバーを引いているかの判定
  if (value > leverTh) {
    isPull = true;
    if(isPull_pre == false) {
      isPull_start = true;
    }
  }
  else {
    isPull = false;
//    isHold = false;
  }
  if (value < homeTh) {
    isHome = true;
  }
  else {
    isHome = false;
  }
  if(isHome){
    isTaskReady = true;
  }

  if (isPull_start) {
     pullStartTime = runTime;
     isPull_start = false;
  } 
  
  // レバーを保持しているかの判定
  if (isTaskReady) {
    if (isPull) {
      holdTime = runTime - pullStartTime;
      if (holdTime > holdTimeTh) {
        isHold = true;
        if(isHold_pre == false) {
          isHold_start = true;
        }
      }
    }
  }
  
  if (isHold_start) {
    trialNum +=1;
    holdStartTime = runTime;
    tone(spkPin, 987, toneTimeTh1);
    isHold_start = false;
  }

  // レバーを保持していたら音がなりLEDが光る
  if (isHold) {
    successTime = runTime - holdStartTime;
    if(successTime > toneTimeTh1){
      tone(spkPin, 1318, toneTimeTh2);
      digitalWrite(spkPin2, HIGH);
    }
    if(successTime > toneTimeTh1 + toneTimeTh2){
      digitalWrite(spkPin2, LOW);
      isSuccess = true;
      isTaskReady = false;
    }
    if(successTime > successTimeTh+toneTimeTh1 + toneTimeTh2){
      isHold = false;
      isSuccess = false;
    }
  }

  if(isSuccess) {   
    // digital outをon
    digitalWrite(ledPin, HIGH);
    digitalWrite(pumpPin, HIGH);
  }
  else {
    // digital outをoff
    digitalWrite(ledPin, LOW);
    digitalWrite(pumpPin, LOW);
  }
 

  //シリアルモニターに表示
  Serial.print(runTime, DEC);
  Serial.print(",");
  Serial.print(value);
  Serial.print(",");
  Serial.print(leverTh);
  Serial.print(",");
  Serial.print(holdTime);
  Serial.print(",");
  Serial.print(holdStartTime);
  Serial.print(",");
  Serial.print(isTaskStart);
  Serial.print(",");
  Serial.print(isTaskReady);
  Serial.print(",");
  Serial.print(isHome);
  Serial.print(",");
  Serial.print(isPull);
  Serial.print(",");
  Serial.print(isHold);
  Serial.print(",");
  Serial.print(trialNum, DEC);
  Serial.print(",");
  Serial.print("\n");
//
//  Serial1.print(runTime, DEC);
//  Serial1.print(",");
//  Serial1.print(value);
//  Serial1.print(",");
//  Serial1.print(isTaskStart);
//  Serial1.print(",");
//  Serial1.print(isPull);
//  Serial1.print(",");
//  Serial1.print(isHold);
//  Serial1.print(",");
//  Serial1.print(trialNum, DEC);
//  Serial1.print(",");
//  Serial1.print("\n");

  isPull_pre = isPull;
  isHold_pre = isHold;
  delay(1000 / Fs);
}
