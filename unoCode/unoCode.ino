#include<LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#include <SoftwareSerial.h>
#define DEBUG true
#include <stdlib.h>

//SoftwareSerial s(9,6); // (Rx, Tx)
 SoftwareSerial Serial1(9, 10); // RX, TX

    
String str;

//A0 = LDR
//A1 = LM35
//A2 =Voltage
//A3=Current


//TEMPERATURE
const int sensor=A1; // Assigning analog pin A1 to variable 'temperature  sensor'
float tempc;  //variable to store temperature in degree Celsius 
float vout;  //temporary variable to hold temperature sensor reading

int error;
//light
double Light (int RawADC0)
{
  double Vout=RawADC0*0.0048828125;
  //int lux=500/(10*((5-Vout)/Vout));//use this equation if the LDR is in the upper part of the divider
  int lux=(2500/Vout-500)/10;
  return lux;
}
int light;



//Voltage
const int voltageSensor = A2;
float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;


//CURRENT
const int currentPin = A3;
int sensitivity = 20;
int adcValue= 0;
int offsetVoltage = 2500;
double adcVoltage = 0;
double currentValue = 0;





void setup()
{
pinMode(sensor,INPUT); // Configuring pin A1 as input
pinMode(9,INPUT);
  pinMode(6,OUTPUT);
//s.begin(9600);


Serial.begin(9600);
 Serial1.begin(9600);
lcd.begin(16,2);  
delay(500);

}


void loop() 
{

//TEMPERATURE
vout=analogRead(sensor);
vout=(vout*4.88);
tempc=(vout/10); // Storing value in Degree Celsius




//Light 
light=int(Light(analogRead(0)));


//Voltage
value = analogRead(voltageSensor);
vOUT = (value * 5.0) / 1024.0;
vIN = vOUT / (R2/(R1+R2)); //print vin


//current
  adcValue = analogRead(currentPin);
  adcVoltage = (adcValue / 1024.0) * 5000;
  currentValue = ((adcVoltage - offsetVoltage) / sensitivity);






  Serial.print(vIN); Serial.print("V; ");
  lcd.setCursor(0, 0); lcd.print("V="); lcd.setCursor(2, 0); lcd.print(vIN);

Serial.print(currentValue); Serial.print("A; ");
  lcd.setCursor(9, 0); lcd.print("A="); lcd.setCursor(11, 0); lcd.print(currentValue);
 
  
  Serial.print(light); Serial.print("L; ");
  lcd.setCursor(0, 1); lcd.print("L="); lcd.setCursor(2,1); lcd.print(light);
    
  Serial.print(tempc); Serial.print("T; ");
  lcd.setCursor(9,1 ); lcd.print("T="); lcd.setCursor(11,1); lcd.print(tempc);
    str =String(vIN)+String('V')+String(currentValue)+String('A')+String(light)+String('L')+String(tempc)+String('T');
    Serial1.println(str);

delay(5000); //Delay of 1 second for ease of viewing in serial 

}



 
