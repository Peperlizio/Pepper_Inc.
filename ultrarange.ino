/* 
Misuratore di distanza con sensore a ultrasuoni HC-SR04 (pin 9;10) + LCD 16x2 I2C e azionamento proporzionale di un servo (pin8).
la misurazione è filtrata tramite media delle letture a loro volta filtrate tramite un filtro Kalman
20/4/2020 Patrik@gmail.com
*/
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Servo servo;

float kk=0.75;  // potenza filtro Kalman (non superare il 0.85)
int angle = 10; // angolo di partenza del servo
int sample = 10;  //quantità sample 

const int trigPin = 9; //Pin di mandata utlrasuono
const int echoPin = 10; //Pin di ricezione ultrasuono

unsigned int duration, distance, distresult; 

void setup() 
{
  Serial.begin(9600);              //inizializzazione seriale
  servo.attach(8);                 // pin servo
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  Serial.begin(9600);  
  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
}

void loop() 
{ 
 for(int i=0; i<= sample; i++)
 {
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH); 
 delayMicroseconds(5); 
 digitalWrite(trigPin, LOW);  
 duration = kk*duration + (1.0-kk)*pulseIn(echoPin, HIGH);
 unsigned int incremental = (duration*0.343)/2;
 
 distance = distance+incremental;
 float kk=0.75;
 float sensorValue = 0;
 
 //if (incremental >= 1000);
 //{ distance = distance-incremental;
 //i--;
//}
 }
   
  unsigned int distresult=distance/sample;
  Serial.print(distresult);
  Serial.print("      ");
  angle= map(distresult, 10, 1500, 10, 180);
  //int angle = analogRead(0);
  //angle = map(angle, 0, 1024, 0, 180);
  servo.write(angle);
  delay(20);
  //Serial.println(angle);
  //Serial.println(distresult);
  distance=0;
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Distanza in mm:");
  lcd.setCursor(5,1);
  lcd.print(distresult);
  
  
}


void reset() //subroutine di reset inseribile a piacimento
{ 
  for(angle = 10; angle < 180; angle++) // scan from 0 to 180 degrees
    {                                  
      servo.write(angle);               
      delay(15);                   
    } 
  for(angle = 180; angle > 10; angle--) // now scan back from 180 to 0 degrees  
    {                                
      servo.write(angle);           
      delay(15);       
    } 
    servo.write(angle);
    
} 
