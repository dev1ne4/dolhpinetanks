#include <Encoder.h>
#define ENCODER_OPTIMIZE_INTERRUPTS

static int clkPin1 = 2;  //CLK to pin 2 YELLOW TO YELLOW
static int dtPin1 = 5;  //DT to pin 5 WHITE TO WHITE 
Encoder rotar(2, 5);
int positionLeft = 1, output = 0, num = 0;     
                       
                                                     // encoder implementation
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
  
}

void loop() {
                        
   num = rotar.read();                     // encoder beings
  if(num != positionLeft){
    if(positionLeft % 78 == 0) 
    {
      output = positionLeft/78;
     
    }
    positionLeft = num;
    delay(1);
    }
    
  
   // if(digitalRead(12)==HIGH){
    
  //  rotar.write(0); output = 0;
    
 //   }

    delay(1);
    
  if(millis()<=999) {                                                  //encoder ends
        Serial.print(millis());
        Serial.print("\t");
        Serial.print(Duty/255*7.2);
        Serial.print("\t");
        Serial.print(angularvel);
        Serial.println("\t");
        
  }
  if (millis()>=990){                                      //995 because encoder reading happens at millis = 1000
  
  
        
 
  
    if(millis() >= time_now + period){                     //Encoder reading won't happen unless millis function went through one duration of period. 
        Duty = 255;
        analogWrite(9,Duty);
        time_now += period;
        currentEnc = rotar.read();
        
        Serial.print(time_now);
        Serial.print("\t");
        Serial.print(Duty/255*7.2);
        Serial.print("\t");
        angularvel= 1000*2*3.14*(currentEnc-prevEnc)/(3120*10);
        Serial.print(angularvel);
        Serial.println("\t");
        
        
        if(millis() >= time_now+period){                 //create error message when the millis function 
        Serial.print("ERROR, sampling time over");
        }
        prevEnc = currentEnc;
    }
    
    
    
    
    if(millis()>= 5000){                                //constantly checking whether its 5 seconds or not, if 5 seconds infinite loop
      Serial.print("Finished");
      analogWrite(9,0);
    while(1);
    }
  }
  
}
