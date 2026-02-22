GAN Robot (V1) is a BLE (Bluetooth Low Energy) device that grips a magic cube and turns it to scramble or descramble the colors.
It works with a BLE enabled cube and a mobile app (CubeStation) to read the cube and solve it.

But I wanted to control the robot without using the app.

https://github.com/cubing/cubing.js/blob/main/src/cubing/bluetooth/docs/gan-robot.md <br>
describes the BLE protocol so minor changes to the ESP32 BLE Client Example allowed me to control the robot.
I checked the unmodified code into the tree, so you can do a diff.

This program doesn't read a BLE enabled cube, nor does it solve one.
It just turns the cube randomly 20 times and then reverses the order to get back where you started.

Tested with ESP32C3 Dev Module (ESP32 C3 Super Mini) <br>
https://www.amazon.com/Teyleten-Robot-Development-Supermini-Bluetooth/dp/B0D47G24W3/

This program uses the ESP32 BLE library instead of ArduinoBLE.

There is now a GAN Robot V2 (Arthur).  It is faster and has a few other features.
I don't know if this code or anything like it will work on it.

https://github.com/rsimp/gan-scrambler <br>
is a PWA that runs in a bluetooth enabled browser (chrome).
It is much more complete and includes a solver, but I couldn't get it to work.

u/poliva got it to work here: https://pof.eslack.org/gan-scrambler/ <br>
But it won't connect to bluetooth in any of my browsers, so I don't know to much about it.

Poor Photo of Circuit Board <br>
![IMG_1406](https://github.com/user-attachments/assets/758b03e6-8bd2-422f-9ccc-19d03538303c)
You can see an 8 pin motor driver, a 4 pin encoder, and a 2 pin connectors for a DC motor (on the back) for each of the 5 grippers.
The BLE chip has a squiggly antenna wire. <br>

I had to buy a GAN 356 i Carry E smart cube to use with this robot.  It has dimples in its faces for the jaws to grip.
I tried using it with another GAN cube, but it kept slipping.

Alan Nishioka <br>
January 2026
