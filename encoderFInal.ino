/*  Devin Ea mechanical encoder test
 *  mechanical GND to GND
 *  mechanical + to VDD
 *  mechanical DT to digital 10
 *  mechanical CLK to digital 2
 *  
 *  To operate, open serial monitor after uploading
 *  the code, and rotation will cause things to happen.
 */


static int clkPin1 = 2;  //CLK to pin 2 YELLOW, LEFT MOTOR
static int dtPin1 = 8;  //DT to pin 10 WHITE

static int clkPin2 = 3, dtPin2 = 9; //YELLOW to 3, RIGHT MOTOR

int mech2CurrentState = LOW, mech2LastState = mech2CurrentState;

volatile int aVal = 0, bVal = 0, TRAVEL1 = 0, TRAVEL2 = 0; //TRAVEL is position, mentioned in section 3.3, but not required for demonstration
volatile float angleL = 0, angleR = 0;
float angle = 0;

void setup() {
  pinMode(clkPin1, INPUT_PULLUP);
  pinMode(dtPin1, INPUT_PULLUP);

  pinMode(clkPin2, INPUT_PULLUP);
  pinMode(dtPin2, INPUT_PULLUP);

  pinMode(13, INPUT); //button for reset
  
  attachInterrupt(0, mechISR1, RISING);  //enables ISR for pin 2, or clkPin
  attachInterrupt(1, mechISR2, RISING);

  Serial.begin(19200); 
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
      Serial.print(TRAVEL1); Serial.print("\t"); Serial.print(TRAVEL2); Serial.print("\t"); Serial.println("EROR 610");
      /*if(digitalRead(13) == 1){
        angleL = 0; angleR = 0; TRAVEL1 = 0; TRAVEL2=0;
        }*/
}
