/**
 * Copyright 2022 Nikolaos Siatras
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 */


// The following code works with the setup/schematic you can find on the following PDF:
// https://github.com/nsiatras/esp32_thermostat/blob/main/PCB_Board/Schematic_ESP32_Thermostat_2022-09-03.pdf
// and has been tested with a 100K NTC Thermistor


// Constants
const int fThermistorPin = 34;                          // The pin the thermistor is connected to ESP32
const int fMosfetPin = 4;                               // The pin the MOSFET is connected to ESP32
const float fR1 = 98900;                                // Value of resistor (ohms) used for the voltage divider
const float fThermistorResistanceAt25Celcius = 100000;  // The resistance of the thermistor at 25Celcius
const int fThermistorBeta = 3955;                       // The beta coefficient or the B value of the thermistor (usually 3000-4000). Check the thermistors's datasheet for the accurate value
const float fFinalResultCalibration = -6.5;             // How much to add or substract from the final temperature result

const float fTemperatureToTurnOn = 40;                  // This is the temperature, in Celcius, at which the thermostat will close the circuit

// Thermistor Input Sampling Properties
const int fThermistorSamplesPerRound = 10;              // How many samples to geat each round
const int fThermistorSamplesDelayInMs = 50;             // Delay in milliseconds between sample collection

// Temporary variables
int i;


void setup() {
  pinMode(fMosfetPin, OUTPUT);
  Serial.begin(9600);
}


void loop() {

  // Read the analog value of the Thermistor Pin
  float thermistorAnalogInput = getAverageThermistorAnalogInput();
  //Serial.print("Analog_Value:");
  //Serial.print(thermistorAnalogInput);

  // Calculate NTC Thermistor resistance
  float thermistorResistance = 4095 /  thermistorAnalogInput - 1;
  thermistorResistance = fR1 / thermistorResistance;
  //Serial.print(" Thermistor Resistance:");
  //Serial.print(thermistorResistance);

  // Convert NTC Thermistor Resistance to Temperature
  float temperature = convertNTCThermistorResistanceToTemperature(thermistorResistance);
  temperature = temperature + fFinalResultCalibration;
  Serial.print(" Temperature:");
  Serial.println(temperature);

  // If temperature exceeds the fTemperatureToTurnOn
  // then turn the Mosfet Pin to High.
  if(temperature >= fTemperatureToTurnOn){
    digitalWrite(fMosfetPin, HIGH);
  }else{
    digitalWrite(fMosfetPin, LOW);
  }

  // We measure the temperature every 2 seconds
  delay(2000 - (fThermistorSamplesPerRound * fThermistorSamplesDelayInMs));
}

/**
 * Reads the analog value of the thermistor pin 
 * for fThermistorSamplesPerRound times
 * and return's the average
 */
float getAverageThermistorAnalogInput(){
  float result = 0.0;
  for (i=0; i < fThermistorSamplesPerRound; i++){
    result += analogRead(fThermistorPin);
    delay (fThermistorSamplesDelayInMs);
  }
  return result / (float)fThermistorSamplesPerRound;
}

/**
 * Converts the NTC Thermistor's resistance to 
 * Celcius Temperature
 */
float convertNTCThermistorResistanceToTemperature(float resistance){
  float temperature = 0;
  temperature = resistance / fThermistorResistanceAt25Celcius; 
  temperature = log(temperature);   
  temperature /= fThermistorBeta;
  temperature += 1.0 / (25 + 273.15);
  temperature = 1.0 / temperature;  
  temperature -= 273.15;  
  return temperature;
}
