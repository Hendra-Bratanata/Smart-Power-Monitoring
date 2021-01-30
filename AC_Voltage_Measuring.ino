
#include <Filters.h> //Easy library to do the calculations
#include <SPI.h>     //Libraries for the OLED display
#include <Wire.h>
#include "EmonLib.h"

EnergyMonitor emon0;
EnergyMonitor emon1;
EnergyMonitor emon2;

int pin_sct0 = A0;
int pin_sct1 = A1;
int pin_sct2 = A2;

bool data1=true;
bool data2=false;
bool data3=false;
bool data4=false;
float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 40.0/testFrequency;     // how long to average the signal, for statistist


int Sensor1;
int Sensor2;
int Sensor3;
float intercept = -0.04; // to be adjusted based on calibration testing
float slope = 0.0405; // to be adjusted based on calibration testing
float current_Volts1; // Voltage
float current_Volts2; // Voltage
float current_Volts3; // Voltage
unsigned long printPeriod = 1000; //Refresh rate
unsigned long previousMillis = 0;


void setup() {
  Serial.begin( 9600 );    // start the serial port
  emon0.current(pin_sct0, 80);
emon1.current(pin_sct1, 80);
emon2.current(pin_sct2, 80);
}

void loop() {

 

  
  RunningStatistics inputStats1;                //Easy life lines, actual calculation of the RMS requires a load of coding
  inputStats1.setWindowSecs( windowLength );
  RunningStatistics inputStats2;                //Easy life lines, actual calculation of the RMS requires a load of coding
  inputStats2.setWindowSecs( windowLength );
  RunningStatistics inputStats3;                //Easy life lines, actual calculation of the RMS requires a load of coding
  inputStats3.setWindowSecs( windowLength );
  float dataTemp = 0;
  while( data1 ) {
    
    Sensor1 = analogRead(A3);  // read the analog in value:
    inputStats1.input(Sensor1);  // log to Stats function
        
    if((unsigned long)(millis() - previousMillis) >= printPeriod) {
      previousMillis = millis();   // update time every second
            
      Serial.print( "\n" );
      
      current_Volts1 = intercept + slope * inputStats1.sigma(); //Calibartions for offset and amplitude
      current_Volts1= current_Volts1*(40.3231);                //Further calibrations for the amplitude
//       if(current_Volts < 223){
//        current_Volts = 0;
//        dataTemp += current_Volts;
//      }
//      else{
//        dataTemp += current_Volts;
//      }
      
      double Irms0 = emon0.calcIrms(1480);
      Serial.print("Arus R : ");
      Serial.print(Irms0);
      Serial.print("\t");
      Serial.print( "Voltage R: " );
      Serial.print( current_Volts1 ); //Calculation and Value display is done the rest is if you're using an OLED display
      data1=false;
      data2=true;
      data3=false;
    
    }
  }
  while(data2){
    
    Sensor2 = analogRead(A6);  // read the analog in value:
    inputStats2.input(Sensor2);  // log to Stats function
        
    if((unsigned long)(millis() - previousMillis) >= printPeriod) {
      previousMillis = millis();   // update time every second
            
      Serial.print( "\n" );
      
      current_Volts2 = intercept + slope * inputStats2.sigma(); //Calibartions for offset and amplitude
      current_Volts2= current_Volts2*(40.3231);                //Further calibrations for the amplitude
//      if(dataTemp == 0){
//        current_Volts = 0;
//      }
       double Irms1 = emon1.calcIrms(1480);
       Serial.print("Arus S : ");
       Serial.print(Irms1);
       Serial.print("\t");
      Serial.print( "Voltage S: " );
      Serial.print( current_Volts2 ); //Calculation and Value display is done the rest is if you're using an OLED display
      
      data1=false;
      data2=false;
      data3=true;
  
  }

}
//
 while(data3){

     Sensor3 = analogRead(A7);  // read the analog in value:
    inputStats3.input(Sensor3);  // log to Stats function
        
    if((unsigned long)(millis() - previousMillis) >= printPeriod) {
      previousMillis = millis();   // update time every second
            
      Serial.print( "\n" );
      
      current_Volts3 = intercept + slope * inputStats3.sigma(); //Calibartions for offset and amplitude
      current_Volts3= current_Volts3*(40.3231);                //Further calibrations for the amplitude
//       if(dataTemp==0){
//        current_Volts = 0;
//      }
        double Irms2 = emon2.calcIrms(1480);
        Serial.print("Arus T : ");
        Serial.print(Irms2);
        Serial.print("\t");
      Serial.print( "Voltage T: " );
      Serial.print( current_Volts3 ); //Calculation and Value display is done the rest is if you're using an OLED display
      Serial.println("");
      data1=true;
      data2=false;
      data3=false;
    
  }
}
}
