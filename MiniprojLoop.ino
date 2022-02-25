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
  Duty = 255;
  angle = -1* rotor.read();
  switch (angleState) {
    case 0: //0 degrees
      while ((angle % 3120) != 0) {
        if ((angle % 3120) <= 1559) {
          analogWrite(9,Duty);
        } else {
          //turn ccw
        }
      }analogWrite(9,0);

      break;
    case 1: //90 degrees
      while ((angle % 3120) != 779) {
        if (((angle % 3120) <= 2339) || ((angle % 3120) >= 779)) {
          analogWrite(9,Duty);
        } else {
          //turn ccw
        }
      } analogWrite(9,0);
      break;
    case 2: // 180 degrees
      while ((angle % 3120) != 1559) {
        if ((angle % 3120) >= 1559) {
          analogWrite(9,Duty);
        } else {
          //turn ccw
        }
      } analogWrite(9,0);
      break;
    default:  //270 degrees
      while ((angle % 3120) != 2339) {
        if (((angle % 3120) <= 2339) || ((angle % 3120) >= 779)) {
          analogWrite(9,Duty);
        } else {
          //turn ccw
        }
      } analogWrite(9,0);
      break;
  }
}
void receiveData(int byteCount) {

  while (Wire.available()) {
    angleState = Wire.read();
  }
}
