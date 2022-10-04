# ESP32 Thermostat

This thermostat is designed to control DC fans or any other DC device that will be connected to it. It is designed and works properly with a standard 3D Printer <b>100kohm NTC thermistor</b>.

How to build ?
------
The code can be found inside the <b>ESP32_Thermostat</b> directory and can be compiled using the Arduino IDE. For more information read the <a href="https://github.com/nsiatras/esp32_thermostat/wiki/How-to-Compile-with-Arduino-IDE">How to Compile with Arduino IDE</a> Wiki Page.

In order to modify the <b>temperature the thermostat closes the circuit</b> you need to alter the following line on the code

```c
const float fTemperatureToTurnOn = 40;   // This is the temperature, in Celcius, at which the thermostat will close the circuit
```

For more accurate measurements you have to measure the actual resistance of the <b>R1 resistor</b> you will use and alter the following line of code. The resistor I used when writing this code was <b>98900 ohms</b>.
```c
const float fR1 = 98900; // Value of resistor (ohms) used for the voltage divider
```
Board Schematic
------
I designed the board with EasyEDA IDE. The <b>schematic of the board</b> can be found inside the <a href="https://github.com/nsiatras/esp32_thermostat/tree/main/PCB_Board">PCB_Board directory</a> directory along with the BOM and Gerber files. 

Story - Why I build this...?
------
Most budget thermostats on the market are designed to control A/C power appliances and 
I've been having a hard time finding thermostats to use with the DC fans inside PCs and Racks.

I designed this thermostat based on the ESP32-DevKitC development boards, as they are one of the most economical, small footprint and super fast development boards available today.



