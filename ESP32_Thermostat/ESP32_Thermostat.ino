// Constants
const int fThermistorPin = 34;                          // The pin the thermistor is connected to ESP32
const int fMosfetPin = 4;                               // The pin the MOSFET is connected to ESP32
const float fR1 = 98900;                                // Value of resistor (ohms) used for the voltage divider
const float fThermistorResistanceAt25Celcius = 100000;  // The resistance of the thermistor at 25Celcius
const int fThermistorBeta = 3955;                       // The beta coefficient or the B value of the thermistor (usually 3000-4000). Check the thermistors's datasheet for the accurate value
const float fFinalResultCalibration = -6.5;             // How much to add or substract from the final temperature result

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
  Serial.print("Analog_Value:");
  Serial.print(thermistorAnalogInput);

  // Calculate NTC resistance
  float thermistorResistance = 4095 /  thermistorAnalogInput - 1;
  thermistorResistance = fR1 / thermistorResistance;
  Serial.print(", Thermistor Resistance:");
  Serial.print(thermistorResistance);

  // Convert NTC Thermistor Resistance to Temperature
  float temperature = convertNTCThermistorResistanceToTemperature(thermistorResistance);
  temperature = temperature + fFinalResultCalibration;
  Serial.print(", Temperature:");
  Serial.println(temperature);

  if(temperature > 30){
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
