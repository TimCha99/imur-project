#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define hs_485min  221 // 최소 펄스 값 (900μs): (900μs/16667μs)×4096≈221
#define hs_485max  516 // 최대 펄스 값 (2100μs): (2100μs/16667μs)×4096≈516 
#define hs_485num  15
#define ledman 13
int val;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  val = (hs_485min + hs_485max) / 2;
  pwm.setPWM(hs_485num, 0, val); // 90도 회전
  //pwm.setPWM(ledman, 0, 4095);
  
}

void loop() {
  for(val; val <= hs_485max; val+=2){
    pwm.setPWM(hs_485num, 0, val);
    Serial.println(val);
    delay(30);
  }

  for(val; val >= hs_485min; val -= 2){
    pwm.setPWM(hs_485num, 0, val);
    Serial.println(val);
    delay(30);
  }



//for (int i = val; i <= hs_485max; i+=2){
//  pwm.setPWM(hs_485num, 0, i);
//  Serial.println(i);
//  delay(20);
//  }
//
//Serial.println(val);
//delay(2000);
//
//for (int j = val; j >= hs_485min; j-=2){
//  pwm.setPWM(hs_485num, 0, j);
//  Serial.println(j);
//  delay(20);
//  }
//
//Serial.println(val);
//delay(2000);
}
