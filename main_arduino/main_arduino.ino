#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define hs_485min 221
#define hs_485max 516
#define hs_485pin 15
int val;
bool isMove = false;
bool moveleft = false;
bool moveright = false;
char cont;
int location;
int reset90degree = (hs_485min + hs_485max) / 2;

#define RPWM_l 3
#define LPWM_l 9 
#define R_EN_l 5
#define L_EN_l 6

#define RPWM_r 10
#define LPWM_r 11 
#define R_EN_r 12
#define L_EN_r 13

int speed = 50;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  val = (hs_485min + hs_485max) / 2;  // 368
  pwm.setPWM(hs_485pin, 0, val);
  pinMode(LPWM_r, OUTPUT);
  pinMode(RPWM_r, OUTPUT);
  pinMode(R_EN_r, OUTPUT);
  pinMode(L_EN_r, OUTPUT);
  
  pinMode(LPWM_l, OUTPUT);
  pinMode(RPWM_l, OUTPUT);
  pinMode(R_EN_l, OUTPUT);
  pinMode(L_EN_l, OUTPUT);

  digitalWrite(R_EN_r, HIGH);
  digitalWrite(L_EN_r, HIGH);
  digitalWrite(R_EN_l, HIGH);
  digitalWrite(L_EN_l, HIGH);
}

void loop() {
  controll();
  moveServo();
}





void controll(){
  if(Serial.available() > 0){
    cont = Serial.read();
    if(cont == 'f'){ // forward
      analogWrite(RPWM_l, speed); analogWrite(LPWM_l, 0);  analogWrite(RPWM_r, 0);  analogWrite(LPWM_r, speed);
    }else if(cont == 's'){ // stop
      analogWrite(RPWM_l, 0); analogWrite(LPWM_l, 0);  analogWrite(RPWM_r, 0);  analogWrite(LPWM_r, 0);
    }else if(cont == 'b'){ // back
      analogWrite(RPWM_l, 0); analogWrite(LPWM_l, speed);  analogWrite(RPWM_r, speed);  analogWrite(LPWM_r, 0);
    }else if(cont == 'l'){ //left
      analogWrite(RPWM_l, 0); analogWrite(LPWM_l, map(speed, 50, 250, 50, 200));  analogWrite(RPWM_r, 0);  analogWrite(LPWM_r, map(speed, 50, 250, 50, 200));
    }else if(cont == 'r'){ // right
      analogWrite(RPWM_l, map(speed,50, 250, 50, 200)); analogWrite(LPWM_l, 0);  analogWrite(RPWM_r, map(speed,50, 250, 50, 200));  analogWrite(LPWM_r, 0);
    }else if(cont == 'a'){
      speed = Serial.parseInt();
    }else if(cont == 'x'){
      isMove = true; moveleft = false; moveright = true;
    }else if(cont == 'y'){
      isMove = true; moveright = false; moveleft = true;
    }else if(cont == 'o'){
      isMove = false; moveleft = false; moveright = false;
    }else if(cont == 'q'){
      resetNow();
    }
  } 
}

void resetNow(){
  int reset_val = val - reset90degree;
  
  if(reset_val < 0){ // -100
    for(val; val <= reset90degree; val+=2){
      pwm.setPWM(hs_485pin, 0, val);
      delay(20);
    }
  }else if(reset_val > 0){ // 100
    for(val; val >= reset90degree; val-=2){
      pwm.setPWM(hs_485pin, 0, val);
      delay(20);
    }
  }
}





void moveServo(){
  if(isMove && moveleft && !moveright){
    if(val >= hs_485min){
      val-=2;
      pwm.setPWM(hs_485pin, 0, val);
      delay(20);
    }
  }else if(isMove && !moveleft && moveright){
    if(val <= hs_485max){
      val+=2;
      pwm.setPWM(hs_485pin, 0, val);
      delay(20);
    }
  }else if(!isMove && !moveleft && !moveright){
    location = pwm.getPWM(hs_485pin);
    pwm.setPWM(hs_485pin, 0, location);
  }
}
