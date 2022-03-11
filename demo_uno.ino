
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

  delay(500);
 
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
                        
  rotate(-90);
   
  Serial.print(TRAVEL1); Serial.print("\t"); Serial.println(TRAVEL2);
  
}

void rotate(double angle){
  
  double full = 4012.53;
  float desired = full*angle/360;
  int intTRAVEL2 = TRAVEL2;

  if(angle < 0){
    Serial.println(desired);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    while(abs(TRAVEL2) < abs((intTRAVEL2+(int)desired))+(300)){
        analogWrite(9,100);
        analogWrite(10,110);
        //Serial.print(TRAVEL1); Serial.print("\t"); Serial.println(TRAVEL2);
      }
        analogWrite(9,0);
        analogWrite(10,0);
        digitalWrite(7, LOW);
        digitalWrite(8, LOW);
    }
    
  else {
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);

        while(abs(TRAVEL2) < abs((intTRAVEL2+(int)desired))){
        analogWrite(9,100);
        analogWrite(10,100);
      }
        analogWrite(9,0);
        analogWrite(10,0);
        digitalWrite(7, LOW);
        digitalWrite(8, LOW);
    }
  
  }
