// Constants
const int fThermistorPin = 34;                          // The pin the thermistor is connected to ESP32
const float fR1 = 100000 ;                              // Value of resistor (ohms) used for the voltage divider
const float fThermistorResistanceAt25Celcius = 100000;  // The resistance of the thermistor at 25Celcius
const int fThermistorBeta = 3950;                       // The beta coefficient or the B value of the thermistor (usually 3000-4000). Check the thermistors's datasheet for the accurate value

// Thermistor Input Sampling Properties
const int fThermistorSamplesPerRound = 10;              // How many samples to geat each round
const int fThermistorSamplesDelayInMs = 50;             // Delay in milliseconds between sample collection

// Temporary variables
int i;

void setup() {
  Serial.begin(9600);
}


void loop() {

  // Read the analog value of the Thermistor Pin
  float thermistorAnalogInput = getAverageThermistorAnalogInput();
  Serial.print("Analog_Value:");
  Serial.print(thermistorAnalogInput);

  // Calculate NTC resistance
  float thermistorResistance = 4095 /  thermistorAnalogInput - 1;
  thermistorResistance = fR1 / thermistorResistance;
  Serial.print(", Thermistor Resistance:");
  Serial.print(thermistorResistance);

  // Convert NTC Thermistor Resistance to Temperature
  float temperature = convertNTCThermistorResistanceToTemperature(thermistorResistance);
  Serial.print(", Temperature:");
  Serial.println(temperature);


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
  result = result / (float)fThermistorSamplesPerRound;
  return result;
}

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
