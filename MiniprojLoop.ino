#include <Encoder.h>

#include <Wire.h>
#define ENCODER_OPTIMIZE_INTERRUPTS
#define SLAVE_ADDRESS 0x04
static int clkPin1 = 2;  //CLK to pin 2 YELLOW TO YELLOW
static int dtPin1 = 5;  //DT to pin 5 WHITE TO WHITE 
Encoder rotor(2, 5);
int positionLeft = 1, output = 0, num = 0;    
int i = 0;
int angle = 0;
int previousState = 0;
int angleState = 0;
int period = 10;
unsigned long time_now = 990;                        //time now starts at 995ms
float angularvel = 0;
int Duty = 0;
float currentEnc = 0;
float prevEnc = 0;
float kp=0.654798757066813;
float ki=0.159515064274842;
float error = 0;
float Ts=0;
float Tc=0;
float D=0;
float I=0;
int u=0;
int erroru=0;
float prevu=0;
void setup() {
  Serial.begin(250000);
  pinMode(4,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(12,INPUT);
  digitalWrite(4,HIGH);
  pinMode(clkPin1, INPUT_PULLUP);
  pinMode(dtPin1, INPUT_PULLUP);

  analogWrite(9, Duty);                         //initilize voltage command to be zero. 
  digitalWrite(7, LOW);
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);

  Serial.println("Ready!");
}

void loop() {
  
  if (angleState==0){
        Serial.print("angle state zero");
        
        controller(0,rotor.read()%3200);

  }
   if (angleState==3){
        Serial.print("angle state one");
        controller(800,rotor.read()%3200);
   }
   if (angleState ==2){
        Serial.print("angle state two");
        controller(1600,rotor.read()%3200);
   }
   if (angleState ==1) {
        Serial.print("angle state three");
        controller(2400,rotor.read()%3200);
      
  }                                                    

  
}                                                      //void loop ends
void receiveData(int byteCount) {

  while (Wire.available()) {
    angleState = Wire.read();
  }
}

void controller (float desired, float current) {

  
  
  error=current-desired;
  error=error/(3200)*2*PI;
  I=I+Ts*error;
  u=kp*10*error+ki*I;
  erroru = (u - prevu);
  
  
  Duty=erroru/15.9*255;
  analogWrite(9,Duty);
  
  Ts=millis()-Tc;
  Tc=millis();
  prevu=u;
  
}
