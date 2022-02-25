#include <Encoder.h>
#define ENCODER_OPTIMIZE_INTERRUPTS

static int clkPin1 = 2;  //CLK to pin 2 YELLOW TO YELLOW
static int dtPin1 = 8;  //DT to pin 8 WHITE TO WHITE

Encoder rotar(2, 8);
int positionLeft = 1, output = 0, num = 0;

void setup() {
  pinMode(clkPin1, INPUT_PULLUP);
  pinMode(dtPin1, INPUT_PULLUP);

  pinMode(12, INPUT);

  Serial.begin(19200); 
}

void loop(){
  num = rotar.read();
  if(num != positionLeft){
    if(positionLeft % 78 == 0) 
    {
      output = positionLeft/78;
      Serial.println(output);
    }
    positionLeft = num;
    delay(1);
    }
    
  if(digitalRead(12)==1){
    output = 0;
    }

    if(digitalRead(12)==HIGH){
    Serial.println("buttoin presed");
    rotar.write(0); output = 0;
    delay(1000);
    }
  }
