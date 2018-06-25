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

## Registers

Here are some of the registers involved with the Two Wire Interface (TWI) hardware module:

Register | Name	| Function
----- | ------------- | -----------------
TWCR | Two Wire Control Register | Controls the actions of the TWI module
TWSR | Two Wire Status Register | Reports the status of the TWI actions
TWDR | Two Wire Data/Address Register | Contains the data you want to transmit or have received
TWBR | Two Wire Bit Rate Register | Controls the frequency of the clock (SCL)

## The TWI Hardware 

![TWI Flow](https://raw.githubusercontent.com/mnaveenkumar2009/Interrupt-Based-Drivers-for-ceu-arduino/master/I2C/assets/TWI_flow.png)

- The Wire library configures the ATmega chip so that the internal TWI hardware uses its pins, which are hardcoded to two of the ATmega's analog pins (4 and 5 on the Duemilanove).
- The Wire library sets up the TWCR register to generate a START condition.
    - It first tests the I2C bus to see if it's free. The bus is free when both SDA and SCL are high (because device pins default to a tri-state, or "not connected" state on the bus. It is up to a common resistor to pull the bus signal high).
    - What is a START condition? The clock line SCL remains high, while the master changes the SDA line to low. This is unique because during normal data transfer, the SDA data line changes state only when the SCL clock line is low. When the data changes to low while the SCL is high, this is the signal to all I2C devices on the bus that a master is about to initiate a communication. 
- TWI hardware interrupts the processor when it's done.
- Wire library checks TWSR for status. Assuming all is well,
- Wire library loads TWDR with the slave's address, plus the "write" bit. Together, this value is known as "SLA+W" ("SLave Address plus Write").
- Wire Library sets up the TWCR register to send the SLA+W.
- If successful, the Wire library loads TWDR with data, TWCR is set, and data is sent.
- Wire library checks TWSR for status. Assuming all is well,
- Wire library sets up the TWCR register to send a STOP condition. In the STOP condition, the SCL line is released (to go high), and then the SDA line goes high. Normally the SDA line must remain stable when the SCL line is high.

A read operation is performed similarly.

Refer [this link](https://playground.arduino.cc/Main/WireLibraryDetailedReference#Intro) for more.

## The Céu API

Céu provides a [driver](https://github.com/ceu-arduino/driver-i2c) for the two wire interface.

The API is as follows:

```
output on/off I2C;// emit I2C to start or stop
output (u8) I2C_REQUEST_SEND;// emit request to send 
output (u8, u8) I2C_REQUEST_RECEIVE;// emit request to receive
input (u8,u8) I2C_REQUEST_DONE;// await the status of the request
```

A detailed description of the API is available [here](https://github.com/mnaveenkumar2009/Interrupt-Based-Drivers-for-ceu-arduino/blob/master/I2C/README.md).

## Comparisons

### Master sender and Slave receiver

#### Requesting equal data

Code :
- Wire Library

    - Master
    ```
    #include <Wire.h>

    void setup() {
        Wire.begin(); // join i2c bus (address optional for master)
    }

    void loop() {
        Wire.beginTransmission(8); // transmit to device #8
        for( int x=0; x<5; x++)
            Wire.write(x);// sends five bytes
        Wire.endTransmission();    // stop transmitting
        delay(500);
    }
    ```
    
    - Slave

    ```
    #include <Wire.h>

    void setup() {
        Wire.begin(8);                // join i2c bus with address #8
        Wire.onReceive(receiveEvent); // register event
        Serial.begin(9600);           // start serial for output
    }

    void loop() {
        delay(100);
    }

    // function that executes whenever data is received from master
    // this function is registered as an event, see setup()
    void receiveEvent(int howMany) {
        while (Wire.available()) { // loop through all
            int c = Wire.read(); // receive byte
            Serial.print(c);         // print the byte as int
        }
    }
    ```

![Output]()

- Céu driver

    - Master
    ```
    #include "i2c.ceu"
    #include "wclock.ceu"
    emit I2C(on); // begin I2C
    loop do
        twi_transmit_buffer = [0,1,2,3,4]; // transmit buffer contains values to transfer. We are sending 5 bytes
        emit I2C_REQUEST_SEND(4); // send the data to slave with address 4
        await I2C_REQUEST_DONE; // await for the request to be completed
        await 1s; // send data every 1s
    end
    ```
    
    - Slave

    ```
    #include "i2c.ceu"
    emit I2C_SET_ADDRESS(4);
    emit I2C(on);
    _Serial.begin(9600);
    loop do
        await I2C_REQUEST_ADDRESSED;
        emit I2C_REQUEST_RECEIVE(0,8);
        await I2C_REQUEST_DONE;
        var u8 i;
        loop i in [1-> $twi_receive_buffer as u8] do
            _Serial.println(twi_receive_buffer[i-1]);
        end
    end
    ```

![Output](https://raw.githubusercontent.com/mnaveenkumar2009/Interrupt-Based-Drivers-for-ceu-arduino/master/I2C/assets/slave_rec01.png)


### Master receiver and Slave sender

#### Requesting equal data




### Concurrency

The C library doesn't support concurrency and the workaround to blink an LED in parallel with I2C requests is to use millis(). The Ceu language supports concurrency as shown below:

```
#include "i2c.ceu"
#include "wclock.ceu"
#include "arduino/arduino.ceu"
#include "gpio.ceu"
emit I2C(on); // begin I2C
output bool PIN_13;
par/and do
    loop do
        twi_transmit_buffer = [0,1,2,3,4]; // transmit buffer contains values to transfer. We are sending 5 bytes
        emit I2C_REQUEST_SEND(4); // send the data to slave with address 4
        await I2C_REQUEST_DONE; // await for the request to be completed
        await 1s; // send data every 1s
    end
with
    loop do
        emit PIN_13(on);
        await 1s;
        emit PIN_13(off);
        await 1s;
    end
end
```