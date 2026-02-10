GAN Robot (V1) is a BLE (Bluetooth Low Energy) device that grips a magic cube and turns it to scramble or descramble the colors.
It works with a BLE enabled cube and a mobile app (CubeStation) to read the cube and solve it.

But I wanted to run the robot without the app.

https://github.com/cubing/cubing.js/blob/main/src/cubing/bluetooth/docs/gan-robot.md \
describes the BLE protocol so minor changes to the ESP32 BLE Client Example allowed me to control the robot.

This program doesn't read a BLE enabled cube, nor does it solve one.
It just turns the cube randomly 20 times and the reverses the order to get back where you started.

Tested with ESP32C3 Dev Module (ESP32 C3 Super Mini) \
https://www.amazon.com/Teyleten-Robot-Development-Supermini-Bluetooth/dp/B0D47G24W3/

This program uses ESP32 BLE library instead of ArduinoBLE.

There is a V2 robot now (Arthur).  It is faster and has a few other features.
I don't know if this code or anything like it will work on it.

https://github.com/rsimp/gan-scrambler \
is a PWA that runs in a bluetooth enabled browser (chrome).
It is much more complete and includes a solver, but I couldn't get it to work.

u/poliva got it to work here: https://pof.eslack.org/gan-scrambler/ \
But it won't connect to bluetooth in any of my browsers, so I don't know to much about it.

Alan Nishioka\
January 2026
