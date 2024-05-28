# NixieClock

Nixie clock build from scratch based on Arduino, including 3D modeling, PCB design and programming.
This repository provides resources and code to help you bring your own Nixie clock to life.

![alt text](https://github.com/Fatal3xcept10n/NixieClock/blob/main/Images/Nixie2.jpg?raw=true)
![alt text](https://github.com/Fatal3xcept10n/NixieClock/blob/main/Images/BoardView.jpg?raw=true)

## Features
- Uses RTC to maintain time and date.
- ability to update time and date over bluetooth.
- Implements digit cycling to prevent cathode poisoning
- uses an external MOSFET controlled by microcontroller to shut off the clock during night hours.

## Files Included

- `NixieClock.ino`: Arduino program to control the Nixie tubes and implement clock functionality using DS1307 RTC, shift registers and K155ID1 high voltage address decoders.
- `Nixie.pdsprj`: Proteus PCB file for the custom PCB design.
- `3D_Models/`: Directory containing IPT files for 3D modeling the enclosure using Autodesk Inventor.

## Getting Started

To get started with the Nixie Clock Project, follow these steps:

1. Clone or download this repository to your local machine.
2. Open the `NixieClock.ino` file in the Arduino IDE and upload it to your Arduino Nano.
3. Open the `Nixie.pdsprj` file in Proteus to view the PCB design and implement any modifications, once done go ahead an generate the gerber file and manufacture the PCB.
5. Use the IPT files in the `3D_Models/` directory to customize, generate your STL files and 3D print the enclosure for your Nixie clock.
6. Find a suitable high voltage power supply for the nixies that fits inside the project box, for IN-14 tubes around 180v DC is required at 20mA for 6 tubes.

## Contributing

Contributions to the NixieClock Project are welcome! If you have ideas for improvements or would like to add features, feel free to fork this repository and submit a pull request.
