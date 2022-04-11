static int clkPin1 = 2;  //CLK to pin 2 YELLOW, LEFT MOTOR, pin2 to yellow
static int dtPin1 = 5;  //DT to pin 5 WHITE, left motor
static int clkPin2 = 3, dtPin2 = 6; //YELLOW to 3, RIGHT MOTOR,pin 6 to white

volatile int aVal = 0, bVal = 0, TRAVEL1 = 0, TRAVEL2 = 0; //TRAVEL is position, mentioned in section 3.3, but not required for demonstration,travel one left, travel one right
volatile float angleL = 0, angleR = 0;   
signed int angle = -45;
#include <Wire.h>

#define SLAVE_ADDRESS 0x04
int pwm1 = 50, pwm2 = 50;
float currTime1 = 0, currTime2 = 0, prevTime1 = 0, prevTime2 = 0;
int *addy;                
void setup() {
 
  Serial.begin(250000);
  pinMode(clkPin1, INPUT_PULLUP);         //Encoder Begins
  pinMode(dtPin1, INPUT_PULLUP);
  pinMode(clkPin2, INPUT_PULLUP);
  pinMode(dtPin2, INPUT_PULLUP);

  pinMode(13, INPUT); //button for reset
  
  attachInterrupt(0, mechISR1, RISING);  //enables ISR for pin 2, or clkPin
  attachInterrupt(1, mechISR2, RISING);
                                         //Encoder ends
  pinMode(4,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);          //left motor speed control, input should be analog write with the correct duty cycle you want, for example, 3.6 v is 50% duty cycle, so pwm 124
  pinMode(10,OUTPUT);         //right motor speed control,
  pinMode(12,INPUT);
  digitalWrite(4,HIGH);

                      
  digitalWrite(7, LOW); // left motor direction control, low meeans forward positive
 
  digitalWrite(8, LOW);  // right motor direction control, low means forward positive

  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);
  //Wire.onRequest(sendData);
 // rotate(-45, addy); rotate(-30, addy); forward(4);
  
}
 
void mechISR1(){ //clkPin ISR
    cli();

    aVal = digitalRead(clkPin1); bVal = digitalRead(dtPin1);
    if(aVal == bVal){
      //Serial.println("CCW");  //if pins are equal, moved CCW
      TRAVEL1-=4;
    }
    else {
      //Serial.println("CW"); //if pins are different, moved CW
      TRAVEL1+=4;
    }
    
    sei();
  }
  
  void mechISR2(){ //clkPin ISR
    cli();

    aVal = digitalRead(clkPin2); bVal = digitalRead(dtPin2);
    if(aVal == bVal){
      //Serial.println("CW");  //if pins are equal, moved CCW
      TRAVEL2+=4;
    }
    else {
      //Serial.println("CCW"); //if pins are different, moved CW
      TRAVEL2-=4;
    }
    
    sei();
  }


void rotate(int angle, int* addy){

  double full = 5973.33;
  float desired = full*angle/360;
  int target = abs((int)desired-(200*abs(angle)/90));
  TRAVEL1 = 0; TRAVEL2 = 0;
  Serial.println(angle);
  if(angle == 250){return;}
  if(angle < 0){
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    while(abs(TRAVEL2) < target || abs(TRAVEL1) < target){
      angle = *addy;
        if(abs(angle) <= 5 || angle == 250){return;}
        if(abs(TRAVEL1) < target)analogWrite(9,pwm1);
        if(abs(TRAVEL2) < target)analogWrite(10,pwm2);
      
      }
        analogWrite(9,0);
        analogWrite(10,0);
        digitalWrite(7, LOW);
        digitalWrite(8, LOW);
    }
    
  else {
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);

    while(abs(TRAVEL2) < target +240|| abs(TRAVEL1) < target-250){
      angle = *addy;
      if(abs(angle) <= 5|| angle == 250){return;}
        if(abs(TRAVEL1) < target -250)analogWrite(9,pwm1);
        if(abs(TRAVEL2) < target +240)analogWrite(10,pwm2);
      }
        analogWrite(9,0);
        analogWrite(10,0);
        digitalWrite(7, LOW);
        digitalWrite(8, LOW);
    }
  
  }

  void forward(float feet){
     TRAVEL1=0;
     TRAVEL2=0;
     pwm1=50;
     pwm2=50;
    digitalWrite(7, LOW); // left motor direction control, low meeans forward positive
    digitalWrite(8, LOW);  // right motor direction control, low means forward positive
    while((TRAVEL1 < feet*2032) && (TRAVEL2 < feet*2032)) {
        if(angle ==250){return;}
        analogWrite(9,pwm1); analogWrite(10,pwm2);
        
      
     if(TRAVEL1 > TRAVEL2){
      if(angle ==250){return;}
          while(TRAVEL1 > TRAVEL2) {
          ++pwm2;
          
          analogWrite(10,pwm2);
          analogWrite(9,0);
          }                       
      }
      
      else if (TRAVEL1 < TRAVEL2){ 
        if(angle ==250){return;}
        while(TRAVEL1 < TRAVEL2) {
        ++pwm1;
        analogWrite(9,pwm1);
        analogWrite(10,0);
        }     
    }
    pwm1 = 50; pwm2 = 50;
    }
    analogWrite(9, 0); analogWrite(10,0);
  }

void loop() {
  addy = &angle;
  analogWrite(9, 0); analogWrite(10,0);
  
  while(angle != 250){
    delay(1000);
    rotate(angle, addy);
    while(abs(angle) <=5){
     forward(0.25); 
    }
  }
  
}
void receiveData(){
  while(Wire.available()){
    angle = (Wire.read()-27);
   
   
  }
}
