#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

//servo head
#define head_min 150
#define head_max 600
#define head_pin 15

//tilt
#define tilt_min 180
#define tilt_max 310
#define tilt_pin 7

//fire
#define fire_pistol 570
#define fire_origin 620
#define fire_pin 3


int val;
int val_tilt;
int val_fire;
bool isMove = false;
bool moveleft = false;
bool moveright = false;

bool movetilt = false;
bool uptilt = false;
bool downtilt = false;

char cont;
int location;
int tiltlocation;
int reset90degree = (head_min + head_max) / 2;

#define RPWM_l 3
#define LPWM_l 9 
#define R_EN_l 5
#define L_EN_l 6

#define RPWM_r 10
#define LPWM_r 11 
#define R_EN_r 12
#define L_EN_r 13

#define led 2
int speed = 150;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  val = (head_min + head_max) / 2;  // 368
  pwm.setPWM(head_pin, 0, val);
  val_tilt = tilt_min;
  pwm.setPWM(tilt_pin, 0, val_tilt);
  val_fire = fire_origin;
  pwm.setPWM(fire_pin, 0, val_fire);
  pinMode(LPWM_r, OUTPUT);
  pinMode(RPWM_r, OUTPUT);
  pinMode(R_EN_r, OUTPUT);
  pinMode(L_EN_r, OUTPUT);
  
  pinMode(LPWM_l, OUTPUT);
  pinMode(RPWM_l, OUTPUT);
  pinMode(R_EN_l, OUTPUT);
  pinMode(L_EN_l, OUTPUT);

  pinMode(led, OUTPUT);

  digitalWrite(R_EN_r, HIGH);
  digitalWrite(L_EN_r, HIGH);
  digitalWrite(R_EN_l, HIGH);
  digitalWrite(L_EN_l, HIGH);
  digitalWrite(led, HIGH);
}

void loop() {
  controll();
  moveServo();
  moveTilt();
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
      resetNow(); tiltReset();
    }else if(cont == 'u'){
      movetilt = true; uptilt = true; downtilt = false;
    }else if(cont == 'd'){
      movetilt = true; uptilt = false; downtilt = true;
    }else if(cont == 'O'){
      movetilt = false; uptilt = false; downtilt = false;
    }else if(cont == 'G'){
      fire_fire();  // DANGER!!!
    }else if(cont == 'Y'){
      digitalWrite(led, HIGH);
    }else if(cont == 'N'){
      digitalWrite(led, LOW);
    }else if(cont == 'i'){
      movetilt = false; uptilt = false; downtilt = false;
      isMove = false; moveright = false; moveleft = false;
    }
  } 
}

void fire_fire(){  // DANGER!!!
  for(val_fire; val_fire >= fire_pistol; val_fire -=2){
    pwm.setPWM(fire_pin, 0, val_fire);
    delay(15);
  }
  val_fire = fire_origin;
  pwm.setPWM(fire_pin, 0, val_fire);
}


void moveTilt(){
  if(movetilt && uptilt && !downtilt){
    if(val_tilt <= tilt_max){
      val_tilt++;
      pwm.setPWM(tilt_pin, 0, val_tilt);
      delay(15);
    }
  }else if(movetilt && !uptilt && downtilt){
    if(val_tilt >= tilt_min){
      val_tilt--;
      pwm.setPWM(tilt_pin, 0, val_tilt);
      delay(15);
    }
  }else if(!movetilt && !uptilt && !downtilt){
    tiltlocation = val_tilt;
    pwm.setPWM(tilt_pin, 0, tiltlocation);
  }
}



void resetNow(){
  int reset_val = val - reset90degree;
  
  if(reset_val < 0){ // -100
    for(val; val <= reset90degree; val+=2){
      pwm.setPWM(head_pin, 0, val);
      delay(20);
    }
  }else if(reset_val > 0){ // 100
    for(val; val >= reset90degree; val-=2){
      pwm.setPWM(head_pin, 0, val);
      delay(20);
    }
  }
}

void tiltReset(){
  for(val_tilt; val_tilt >= tilt_min; val_tilt-=2){
    pwm.setPWM(tilt_pin, 0, val_tilt);
    delay(20);
  }
}


void moveServo(){
  if(isMove && moveleft && !moveright){
    if(val >= head_min){
      val--;
      pwm.setPWM(head_pin, 0, val);
      delay(15);
    }
  }else if(isMove && !moveleft && moveright){
    if(val <= head_max){
      val++;
      pwm.setPWM(head_pin, 0, val);
      delay(15);
    }
  }else if(!isMove && !moveleft && !moveright){
    location = val;
    pwm.setPWM(head_pin, 0, location);
  }
}
