#include <SoftwareSerial.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h" 
#define REPORTING_PERIOD_MS     1000
SoftwareSerial bt(5,6); // RX, Tx 
PulseOximeter pox;
uint32_t tsLastReport = 0;

const int GSR=A0;
int sensorValue=0;
int gsr_average=0;

int num_Measure = 128 ; // Set the number of measurements   
int soundpinSignal = A3; // pin connected to pin O module sound sensor  

long Sound_signal;    // Store the value read Sound Sensor   
long sum = 0 ; // Store the total value of n measurements   
long level = 0 ; // Store the average value   
int soundlow = 40;
int soundmedium = 500;
int const PULSE_SENSOR_PIN = A2;   // 'S' Signal pin connected to A2

int Signal;                // Store incoming ADC data. Value can range from 0-1024
int Threshold = 550;       // Determine which Signal to "count as a beat" and which to ignore.

void onBeatDetected()
{
    Serial.println("Beat!");
}
 
void setup(){

// initialize the serial communication:

pinMode(10, INPUT); // Setup for leads off detection LO +
pinMode(11, INPUT); // Setup for leads off detection LO -

pinMode(LED_BUILTIN,OUTPUT);  // Built-in LED will blink to your heartbeat
pinMode (soundpinSignal, INPUT); // Set the signal pin as input  
 

  Serial.begin(9600);  
bt.begin(9600); 
 
  /*Serial.print("calibrating sensor "); */
    for(int i = 0; i < calibrationTime; i++){
      /* Serial.print("."); */
      delay(10);
      }
      /*
    Serial.println(" done");
    */
    Serial.print("Initializing pulse oximeter..");
 
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
    Serial.println("SENSOR ACTIVE");
    delay(50);
}
 
void loop(){

  if (bt.available())  /* If data is available on serial port */
    {
     Serial.write(bt.read()); /* Print character received on to the serial monitor */
    }

       // Make sure to call update as fast as possible
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
     
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
 
        tsLastReport = millis();
    }
    
  long sum=0;
  for(int i=0;i<10;i++)           //Average the 10 measurements to remove the glitch
      {
      sensorValue=analogRead(GSR);
      sum += sensorValue;
      delay(100);
      }
   gsr_average = sum/10;
 
 
if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
Serial.println(" ECG Not connected:");
}
else{
// send the value of analog input 0:
Serial.println(analogRead(A1));
}
//Wait for a bit to keep serial data from saturating
delay(1);
   
  for ( int i = 0 ; i <num_Measure; i ++)  
  {  
   Sound_signal = analogRead (soundpinSignal);  
    sum =sum + Sound_signal;  
  }  
 
  level = (sum / num_Measure)+15; // Calculate the average value 

  sum = 0 ; // Reset the sum of the measurement values  
 delay(500);
 
  Signal = analogRead(PULSE_SENSOR_PIN)/10; // Read the sensor value
 
  if(Signal > Threshold){                // If the signal is above threshold, turn on the LED
    digitalWrite(LED_BUILTIN,HIGH);
  } else {
    digitalWrite(LED_BUILTIN,LOW);     // Else turn off the LED
  }
 delay(1000);



Serial.print("GSR-Value: ");                        

Serial.print(gsr_average);                            
Serial.print("µS ");

Serial.print(" Pulse-Rate: ");
Serial.print(Signal); 

Serial.print(" BPM  ");

Serial.print(" Sound-Intensity-level: ");
Serial.println (level);  

 
 bt.print(gsr_average); //send to MIT App
 bt.print("\n");
 
  bt.print(Signal);
 bt.print("\n");
 
 bt.print(level); //send to MIT App 
 bt.print("\n");

bt.print(spo);
bt.print("\n");
 
  
  delay(1000);
  }
 