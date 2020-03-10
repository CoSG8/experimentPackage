/*
  lever_perista.ino
  Programmed by Akito Kosugi
  ver. 1.3 2020.03.09

*/

#define LorR    0                // L/RはANALOG IN A0に接続
#define ledPin  13                // LEDはDEGITAL OUT13に接続
#define spkPin  2                // スピーカーはDEGITAL OUT2に接続
#define spkPin2 3 
#define pumpPin 4
#define switchPin 5


// 変更するパラメータ
int leverTh = 1000;               // valueが一定時間この値以下だとレバー引いていると判断
int homeTh = 500;
int holdTimeTh =  200;            // 必要な保持時間 [ms]int successTimeTh = 300; 
int successTimeTh = 300; 
int pumpTimeTh = 200;
int toneTimeTh1 = 150;
int toneTimeTh2 = 250;
int startTimeTh = 3000;
int Fs = 100;                     // サンプリングレート [Hz]

// 変数
int value;                        // joystickからの入力信号
int baseVal = 0;
int trialNum = 0;
unsigned long runTime = 0;        // タイマー
unsigned long holdTime = 0;       // 保持時間を数える変数
unsigned long pullStartTime = 0;  // 保持時間を数える変数
unsigned long holdStartTime = 0;  // 保持時間を数える変数
unsigned long successTime = 0;
unsigned long pumpTime = 0;
unsigned long pumpStartTime = 0;
bool isSwitch = false;
bool isSwitch_on = false;
bool isPull = false;              // レバーを引いているるかのフラグ
bool isHold = false;              // レバーを保持しているかのフラグ
bool isHome = false; 
bool isSuccess = false;
bool isPull_pre = false;
bool isHold_pre = false;
bool isPull_start = false;
bool isHold_start = false;
bool isTaskStart = false ;
bool isTaskReady = false ;

void setup(){

  pinMode(LorR, INPUT);
  pinMode(spkPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(57600);
  Serial.println("value");

}

void loop() {

  runTime = millis();

  //センサーより信号を取得
  value = analogRead(LorR);
  isSwitch = digitalRead(switchPin);

  if(isSwitch == true){
    if(isSwitch_on == false){
      isSwitch_on = true;
      pumpStartTime = runTime;
    }
  }
  if(isSwitch_on == true){
    pumpTime = runTime-pumpStartTime;
    digitalWrite(pumpPin, HIGH);
    digitalWrite(ledPin, HIGH);
  }
  if(pumpTime > pumpTimeTh){
    digitalWrite(pumpPin, LOW);
    digitalWrite(ledPin, LOW);
    pumpTime = 0;
    isSwitch_on = false;
  }  

  // タスク開始は一定時間後
  if (isTaskStart == false) {
    if (runTime > startTimeTh) {
      isTaskStart = true;
      isTaskReady = true;
      baseVal = value;
      leverTh = baseVal * 1.7;
      homeTh = baseVal * 1.1;
      digitalWrite(ledPin, LOW);
    }
    else {
      isTaskReady = false;
      digitalWrite(ledPin, HIGH);
    }
  }

  // レバーを引いているかの判定
  if (value > leverTh) {
    isPull = true;
    if (isPull_pre == false) {
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
  if (isHome) {
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
        if (isHold_pre == false) {
          isHold_start = true;
        }
      }
    }
  }

  if (isHold_start) {
    trialNum += 1;
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
  if (isSuccess) {
    // digital outをon
    digitalWrite(ledPin, HIGH);
    digitalWrite(pumpPin, HIGH);
  }
  else if(isSuccess == false && isSwitch_on == false) {
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
  Serial.print(pumpTime);
  Serial.print(",");
  Serial.print(pumpStartTime);
  Serial.print(",");
  Serial.print(holdTime);
  Serial.print(",");
  Serial.print(holdStartTime);
  Serial.print(",");
  Serial.print(isSwitch);
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
