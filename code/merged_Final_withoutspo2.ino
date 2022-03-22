//gsr
#include <SoftwareSerial.h>
SoftwareSerial bt(5,6); // RX, Tx

const int GSR=A0;
int sensorValue=0;
int gsr_average=0;

//sound
int num_Measure = 128 ; // Set the number of measurements   
int soundpinSignal = A3; // pin connected to pin O module sound sensor  

long Sound_signal;    // Store the value read Sound Sensor   
long sum = 0 ; // Store the total value of n measurements   
long level = 0 ; // Store the average value   
int soundlow = 40;
int soundmedium = 500;

//pulse

int const PULSE_SENSOR_PIN = A2;   // 'S' Signal pin connected to A2

int Signal;                // Store incoming ADC data. Value can range from 0-1024
int Threshold = 550;       // Determine which Signal to "count as a beat" and which to ignore.

 
//pir

int calibrationTime = 30;       
 long unsigned int lowIn;        
 long unsigned int pause = 50; 
 int resultstr;
 int resultend;
 
boolean lockLow = true;
boolean takeLowTime; 
 
int piroutpin = 3; 
int ledPin = 13;
 
void setup(){

// initialize the serial communication:

pinMode(10, INPUT); // Setup for leads off detection LO +
pinMode(11, INPUT); // Setup for leads off detection LO -

pinMode(LED_BUILTIN,OUTPUT);  // Built-in LED will blink to your heartbeat
pinMode (soundpinSignal, INPUT); // Set the signal pin as input   

  Serial.begin(9600);  
bt.begin(9600); 
pinMode(piroutpin, INPUT);
pinMode(ledPin, OUTPUT);
digitalWrite(piroutpin, LOW);
 
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(10);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
}
 
void loop(){

  if (bt.available())  /* If data is available on serial port */
    {
     Serial.write(bt.read()); /* Print character received on to the serial monitor */
    }
    
  long sum=0;
  for(int i=0;i<10;i++)           //Average the 10 measurements to remove the glitch
      {
      sensorValue=analogRead(GSR);
      sum += sensorValue;
      delay(50);
      }
   gsr_average = sum/10;
   /*
   Serial.print("GSR Value: ");
   Serial.print(gsr_average);
    Serial.println(" sweat glands per cm2 ");

*/
 //ecg
 

 
if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
Serial.println("ECG Sensor is Not Connected with body:");
}
else{
// send the value of analog input 0:
Serial.println(analogRead(A1));
}
//Wait for a bit to keep serial data from saturating
delay(1);



//sound


  // Performs 128 signal readings   
  for ( int i = 0 ; i <num_Measure; i ++)  
  {  
   Sound_signal = analogRead (soundpinSignal);  
    sum =sum + Sound_signal;  
  }  
 
  level = (sum / num_Measure)+15; // Calculate the average value 
  /*  
  Serial.print("Sound Intensity Level: ");

  Serial.println (level);  
*/

  sum = 0 ; // Reset the sum of the measurement values  
  delay(200);
 



//pulse



  Signal = analogRead(PULSE_SENSOR_PIN)/10; // Read the sensor value
  /*
 Serial.print("Pulse Rate: ");
  Serial.println(Signal);                // Send the signal value to serial plotter
*/
  if(Signal > Threshold){                // If the signal is above threshold, turn on the LED
    digitalWrite(LED_BUILTIN,HIGH);
  } else {
    digitalWrite(LED_BUILTIN,LOW);     // Else turn off the LED
  }
  delay(1000);

//pir
 
     if(digitalRead(piroutpin) == HIGH){
       digitalWrite(ledPin, HIGH);
       if(lockLow){ 
         lockLow = false; 
         resultstr = millis()/1000;          
         Serial.println("---");
         /*
         Serial.print("Motion detected at ");
         Serial.print(resultstr);
         Serial.println(" sec");
         delay(50);
         */
         }        
         takeLowTime = true;
       }
 
     if(digitalRead(piroutpin) == LOW){      
       digitalWrite(ledPin, LOW);
 
       if(takeLowTime){
        lowIn = millis();         
        takeLowTime = false;     
        }
      
       if(!lockLow && millis() - lowIn > pause){ 
           lockLow = true;      
           resultend =  (millis() - pause)/1000;                
           Serial.print("motion ended at ");  
           Serial.print(resultend);
           Serial.println(" sec");
           delay(50);
           }
       }

Serial.print("GSR Value: ");
Serial.print(gsr_average);
Serial.println(" sweat glands per cm2 ");

Serial.print("Pulse Rate: ");
Serial.println(Signal); 

Serial.print("Motion detected at ");
Serial.print(resultstr);
Serial.println(" sec");

Serial.print("Sound Intensity Level: ");
Serial.println (level);  
 
 bt.print(gsr_average); //send to MIT App
 bt.print("\n");
 
  bt.print(Signal);
 bt.print("\n");
 

  bt.print(resultstr);
 bt.print("\n");

 bt.print(level); //send to MIT App 
 bt.print("\n");

 
  
  delay(1000);
  }
 
