# Duco Monitor
## _Monitor by duino coin_

[![N|Solid](https://cdn.discordapp.com/attachments/677962332848259083/858335905181466624/1624713617255.jpg)](https://github.com/ponsato/ducomonitor)

Small display for monitoring the status of mining [duinocoin](https://duinocoin.com). It indicates the user's number of ducos, number of connected miners, estimated daily earnings, and the current price of the duco.
Data is updated every 15 seconds, except for the daily estimate which is updated approximately every minute and a half. 
Pin layout and .stl files for printing the box are included.

## Hardware
- ESP32/ESP8266 there is a separate file for each
- SSD1306 OLED I2C 1.3"/0.96" (on the 1.3" display, GND and VCC are reversed with respect to 0.96". The pin layout is made for 0.96").
