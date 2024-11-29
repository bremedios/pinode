# Pi Heater
Welcome to my Raspberry Pi heater.  This is used to control an oil heater to have it maintain a hold a set temperature
in a room.

This requires at least 1 Raspberry Pi to operate, but you can use multiple Raspberry Pis.  You can use multiple
Raspberry Pis to have the sensors farther away from the heater to reduce any impact that the heater has on the sampled
temperature.

# System Configuration
There currently is no available configuration for this.  Everything is currently hard-coded as this is a
work-in-progress.  Keep an eye on the project for a release that does allow for a more detailed system configuration.

# Hardware Setup
This is how the hardware pin-out is configured in code.  This is currently not configurable.

| Module                           | Pin         | Description                |
|----------------------------------|-------------|----------------------------|
| Pi Node Temperature Sensor       | 7 (GPIO 4)  | DHT22 / DHT11 sensor input |
| Pi Heater | 11 (GPIO 0) | Heater Enable / Disable    |