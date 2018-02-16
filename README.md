# Arduino RFID Keyboard 

This little code shows you how easy you can use a RFID Modul in combination with a Keyboard function (from Arduino Pro Micro (ATmega32U4)). Freely usable for all with the exception of military and other anti-social activities.

Created 22.8.2015 by Marc-André Tragé
  
### Pinout for Arduino Pro Micro
![](https://github.com/MTrage/Arduino-RFID-Keyboard/blob/master/circuit-diagram.png)
+ VCC to RFID RC522 Modul needs 3.3v
+ GND  GND
+ IRG  not connected
+ RST  pin 6
+ SDA  pin 10  
+ MISO pin 14
+ SCK  pin 15
+ MOSI pin 16

### What you need for a little RFID fun:
  +  Arduino Pro Micro and Leonardo or equal Boards with a ATmega32U4
     it can emulate via USB a keyboard or a mouse) order types like this => https://www.google.de/search?q=arduino+pro+micro
  +  RFID RC522 Modul like this => https://www.google.de/search?q=rfid+rc522
  +  if you have only 5.0v VCC on Board, use a AMS1117-3.3 like this => http://www.advanced-monolithic.com/pdf/ds1117.pdf
  +  LED Button like this => https://www.google.de/search?q=LED+Light+Tactile+Push+Button
    
If you have any useful changes or improvements please let me know.
(Not a wish (but a command)) have fun (NOW (and for the rest of your life))!
