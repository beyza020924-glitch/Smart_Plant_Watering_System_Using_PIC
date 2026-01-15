# Smart_Plant_Watering_System_Using_PIC
The system will monitor soil moisture levels using a sensor and activate a water pump automatically when the moisture level drops below a specified threshold. It is an excellent application of embedded systems in real-life environmental monitoring and automation.

The system includes the following key components:
✓ Soil Moisture Sensor: Detects the moisture level in the soil and sends analog data
to the Arduino.
✓ Relay Module: Acts as an electronic switch to turn the water pump on and off.
✓ Mini Submersible Water Pump: Waters the plant when activated by the relay.
✓ 16x2 LCD Display: Shows live moisture readings and system status.
✓ Power Source: Provides energy to the pump and the Arduino board.

When the system is powered on, the Arduino continuously reads analog values
from the soil moisture sensor. These values are converted into digital format using
the built-in ADC module. If the soil moisture is below the programmed threshold,
the Arduino sends a HIGH signal to the relay module, which activates the water
pump. Water is pumped into the soil until the desired moisture level is reached.
The LCD screen displays the current moisture level and the watering status
throughout the process.

To build the system, the following microcontroller interfaces are utilized:
✓ GPIO (General Purpose Input/Output Ports)
  o Used to control the relay (output) and receive data from the soil
  moisture sensor (input).
  o Also used to interface with the 16x2 LCD display.
✓ ADC (Analog-to-Digital Converter)
  o Converts the analog voltage from the soil moisture sensor into a digital
  value that the Arduino can process.
✓ Timer0
  o Used to manage regular intervals between moisture level checks and to
  prevent excessive activation of the pump (e.g., allow it to run for a set
  period and pause before the next reading).


o Used to manage regular intervals between moisture level checks and to
prevent excessive activation of the pump (e.g., allow it to run for a set
period and pause before the next reading).
