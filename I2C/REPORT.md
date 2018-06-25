# TWI or Two Wire Interface

The 2-wire Serial Interface (TWI) is ideally suited for typical microcontroller applications. The TWI protocol allows
the systems designer to interconnect up to 128 different devices using only two bi-directional bus lines, one for
clock (SCL) and one for data (SDA). The only external hardware needed to implement the bus is a single pull-up
resistor for each of the TWI bus lines. All devices connected to the bus have individual addresses, and mechanisms
for resolving bus contention are inherent in the TWI protocol.

## TWI Terminology

Term | Description
---------- | -----------------------
Master | The device that initiates and terminates a transmission. The Master also generates the SCL clock
Slave | The device addressed by a Master
Transmitter | The device placing data on the bus
Receiver | The device reading data from the bus

## The C API

## The ceu API







// image : ![Alt Text](url)