# Duco Monitor
## _Monitor par duino coin_

[![N|Solid](https://cdn.discordapp.com/attachments/677962332848259083/858335905181466624/1624713617255.jpg)](https://github.com/ponsato/ducomonitor)

Pequeña pantalla para monitorizar el estado de la minería de [duinocoin](https://duinocoin.com). Indica número de ducos del usuario, número de mineros conectados, estimación de ganancias diarias, y el precio del duco en ese momento.
Los datos se actualizan cada 15 segundos, excepto la estimación diaria que se actualiza cada minuto y medio aproximadamente. 
Se incluye esquema de pines y archivos .stl para la impresión de la caja.

## Hardware
- ESP32/ESP8266 existe un archivo diferente para cada uno
- SSD1306 OLED I2C 1.3"/0.96" (en la pantalla de 1.3", GND y VCC están invertidos con respecto a 0.96" El esquema de pines está hecho para 0.96")

