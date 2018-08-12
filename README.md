![Google Summer of Code](https://musescore.org/sites/musescore.org/files/Capture%20d'e%CC%81cran%202016-03-01%2009.48.11.png)

## Interrupt-based drivers and libraries for Céu-Arduino

**Organization:** LabLua

**Mentor:** Francisco Sant'Anna

**Student:** M Naveen Kumar

# GSoC 2018
Work done during GSoC 2018 for LabLua organisation is available in this repository. Google Summer of Code is a global program focused on bringing more student developers into open source software development. Students work with an open source organization on a 3 month programming project during their break from school/university.

## Overview
The project consists of developing new interrupt-based drivers for microcontrollers. The drivers built during GSoC 2018 include I2C(Inter Integrated Circuit pronounced I squared C) communication, Real Time Clock and OLED(Organic Light Emitting Diode) display.

## The language Céu
[Céu](http://www.ceu-lang.org/) is a Structured Synchronous Reactive Programming language that aims to offer a higher-level and safer alternative to C. It extends classical structured programming with two main functionalities: event handling and synchronous, deterministic concurrency. ([Github Repository](https://github.com/ceu-lang/ceu))
[Céu-Arduino](https://github.com/ceu-lang/ceu-arduino) supports the development of Arduino applications in the programming language Céu.
To use the drivers clone both these repositories and follow the steps in their respectively README files.

## What works now?
The Drivers made for Céu-Arduino during GSoC 2018 were:
#### I2C
- The Inter-integrated Circuit (I2C) Protocol is a protocol intended to allow multiple “slave” digital integrated circuits (“chips”) to communicate with one or more “master” chips.
- Like the Serial Peripheral Interface (SPI), it is only intended for short distance communications within a single device.
- Like Asynchronous Serial Interfaces (such as RS-232 or UARTs), it only requires two signal wires to exchange information.
- For more details about the driver read the documentation in the [GitHub Repository](https://github.com/ceu-arduino/driver-i2c/tree/pre-v0.40).
#### RTC
- A real-time clock (RTC) is an IC that keeps an updated track of the current time.
- This information can be read by a microprocessor, usually over a serial interface to facilitate the software performing functions that are time dependent.
- RTCs are designed for ultra-low power consumption as they usually continue running when the main system is powered down.
- This enables them to maintain current time against an absolute time reference, usually set by the microprocessor directly.
- For more details about the driver read the documentation in the [GitHub Repository](https://github.com/ceu-arduino/driver-rtc/tree/pre-v0.40).
#### OLED
- OLED (Organic Light Emitting Diodes) is a flat light emitting technology, made by placing a series of organic thin films between two conductors. When electrical current is applied, a bright light is emitted.
- OLEDs are emissive display that do not require a backlight and so are thinner and more efficient than LCD displays (which do require a white backlight).
- The code used for OLED display is applicable to the SSD1306 OLED display which supports I2C communication.
- For more details about the driver read the documentation in the [GitHub Repository](https://github.com/ceu-arduino/driver-oled/tree/pre-v0.40).

The drivers were made for them, functioning as mentioned in their respective repositories.

## How to use?
- To use the Céu-Arduino drivers we need to clone both [Céu](https://github.com/ceu-lang/ceu) and [Céu-Arduino](https://github.com/ceu-lang/ceu-arduino) repositories.
- Clone the respective drivers inside the libraries folder in your ceu-arduino folder.
- Include the driver in the program by it's file name. eg:
    ```
    #include "i2c.ceu"
    ```
- Write the programs as mentioned in the documentation of the drivers available in their README files (The drivers are in the pre-v0.40 branch).

## Future Work
There is always a room of improvement in any project. This project will be extended to build drivers for other interrupt based microcontrollers like EEPROM. Also, the current API can be updated and more features can be added.

## Links
All links below for the repositories of the respective drivers have a README file describing the usage and documentation of the driver. All the work done is pushed to pre-v0.40 branch of the repositories.
- [Project](https://summerofcode.withgoogle.com/dashboard/project/5737152055345152/overview/)
- [I2C driver](https://github.com/ceu-arduino/driver-i2c/tree/pre-v0.40)
- [RTC driver](https://github.com/ceu-arduino/driver-rtc/tree/pre-v0.40)
- [OLED driver](https://github.com/ceu-arduino/driver-oled/tree/pre-v0.40)
- [Weekly Report](https://github.com/mnaveenkumar2009/GSoC-2018/blob/master/report.md)
- [Project Proposal](https://github.com/mnaveenkumar2009/GSoC-2018/blob/master/Project%20proposal.pdf)
