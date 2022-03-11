


static int clkPin1 = 2;  //CLK to pin 2 YELLOW, LEFT MOTOR, pin2 to yellow
static int dtPin1 = 5;  //DT to pin 5 WHITE, left motor
static int clkPin2 = 3, dtPin2 = 6; //YELLOW to 3, RIGHT MOTOR,pin 6 to white

volatile int aVal = 0, bVal = 0, TRAVEL1 = 0, TRAVEL2 = 0; //TRAVEL is position, mentioned in section 3.3, but not required for demonstration,travel one left, travel one right
volatile float angleL = 0, angleR = 0;   
float angle = 0;

                       
                                                     // encoder implementation
int period = 10;
unsigned long time_now = 990;                        //time now starts at 995ms
float angularvelleft = 0;
float angularvelright =0;
float Dutyleft = 0;
float Dutyright = 0;
float currentEncleft = 0;
float currentEncright = 0;
float prevEncleft = 0;
float prevEncright = 0;
float Va=8.2;                                       //change va and delta va to adjust for differentt response
float deltaVa=0;                                  //change delta va to adjust for different response. 
float Valeft=0;
float Varight=0;
float velocity = 0;
float rotationalVel=0;
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

                      
  digitalWrite(7, HIGH); // left motor direction control, low meeans forward positive
 
  digitalWrite(8, HIGH);  // right motor direction control, low means forward positive

  Valeft=(Va+deltaVa)/2;
  Varight=(Va-deltaVa)/2;
 
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

void loop() {
                        
  
   
  if(millis()<=999) {                                                  //encoder ends
Serial.print(millis()); Serial.print("\t"); Serial.print(Dutyleft/255*8.2); Serial.print("\t"); Serial.print(Dutyright/255*8.2); Serial.print("\t"); Serial.print(angularvelleft); Serial.print("\t");Serial.println(angularvelright);
        
        
  }
  if (millis()>=990){                                      //995 because encoder reading happens at millis = 1000
  
  
        
 
  
    if(millis() >= time_now + period){                     //Encoder reading won't happen unless millis function went through one duration of period. 
       Dutyleft = Valeft/8.2*255;                                     //Dutyleft is 5 less than duty right
       Dutyright = Varight/8.2*255;
  
        analogWrite(9,Dutyleft);
        analogWrite(10,Dutyright);
        time_now += period;
        currentEncleft = TRAVEL1;
        currentEncright = TRAVEL2;
        
        Serial.print(time_now);
        Serial.print("\t");
        Serial.print(Dutyleft/255*8.2);
        Serial.print("\t");
        Serial.print(Dutyright/255*8.2);
        Serial.print("\t");
        
        angularvelleft= 1000*2*3.14*(currentEncleft-prevEncleft)/(3120*10);
        angularvelright= 1000*2*3.14*(currentEncright-prevEncright)/(3120*10);
        velocity=0.075*(angularvelleft+angularvelright)*0.5;
        rotationalVel=0.075*(angularvelleft-angularvelright)/0.28;
        
        
        Serial.print(angularvelleft);
        Serial.print("\t");
        Serial.print(angularvelright);
        Serial.print("\t");
        Serial.print(velocity);
        Serial.print("\t");
        Serial.println(rotationalVel);
        
        
        
        if(millis() >= time_now+period){                 //create error message when the millis function 
        Serial.print("ERROR, sampling time over");
        }
        prevEncleft = currentEncleft;
        prevEncright = currentEncright;
    }
    
    
    
    
    if(millis()>= 5000){                                //constantly checking whether its 5 seconds or not, if 5 seconds infinite loop
      Serial.print("Finished");
      analogWrite(9,0);
      analogWrite(10,0);
    while(1);
    }
  }
  
}
