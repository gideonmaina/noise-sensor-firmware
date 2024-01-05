# airRohr Sensor Firmware for SPH0645 and DNMS sensors

## Features

* Configuration via HTTP in local WiFi or with a Sensor-as Access-Point
* Support for OLED- and LCD-Displays (SSD1306, SH1106 and LCD1602, LCD2004)
* Wide selection of API integrations for measurement reporting
* Ability to print measurements as CSV via USB-serial
* Used with ESP8266 (NodeMCU v2/v3 and compatible) and ESP32 (experimental)

## Contributing

Please refer to [Contributing README](./Contributing.md) for details.

## WiFi configuration

If a (previously) configured WiFi is not reachable within 20 seconds after power-on,
the firmware flips itself into AP mode and creates a WiFi network in the form `airRohr-\[Sensor-ID\]`.

This WiFi network is by default unencrypted. When a client connects to this, it will get
redirected to the sensors web server `http://192.168.4.1/` which allows initial configuration.

Configurable is

* WiFi Access Point to use
* Options for measurements (Sensors to poll, intervals..)
* APIs to send the measurements

The unencrypted Access Point for initial configuration will turn itself off after about
10 minutes. In order to reactivate please power cycle the board.

## Debug via USB-Serial

Connecting/Powering via a computer USB will provide USB serial with the settings 9600 baud 8N1.
By default the sensor will provide human readable debug information of configurable granularity
there.

## Save as CSV

All measurements can also be read as CSV via USB-Serial when using the USB port in the
settings 9600 Baud 8N1. In order to avoid interfering of debug options (see earlier section)
set debug to None in the configuration.

## Wiring

Please refer to the [Pinout of NodeMCU v2 and v3](https://github.com/opendata-stuttgart/meta/wiki/Pinouts-NodeMCU-v2,-v3) for much more detailed information about the individual pin functions.

### GSM
* VCC  -> pin VU
* GND  -> pin GND
* RX   -> pin D1
* TX   -> pin D2

### LCD1602 (I2C, 5V - check your version)

* VCC  ->  Pin VU
* GND  ->  Pin GND
* SCL  ->  Pin D4 (GPIO2)
* SDA  ->  Pin D3 (GPIO0)

### OLED displays with SSD1306 (I2C, 128x64 pixels)

* VCC -> Pin VU
* GND -> Pin GND
* SCL  ->  Pin D4 (GPIO2)
* SDA  ->  Pin D3 (GPIO0)

## sensors.AFRICA API "Pins"

For use of multiple sensors with sensors.AFRICA, you need to specify a *virtual* API  
Pins for use in the sensor registration. The firmware uses the following API pins  
hardcoded. These match what the Luftdaten.info expect and will use by default when
selecting the correct sensor model.

* DNMS/SPH0645 +> Pin 15
