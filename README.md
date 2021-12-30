# rect
Rotary Engine Compression Tester

## Summary

Rotary engines (Wankel-style) have 3 rotor faces, each with it's own compression metric.
As the rotor turns the spark plug hole sees 3 pressure values that reflect the compression for each rotor face.
This is difficult to measure on a simple analog gauge.
Using a microcontroller we measure the pressure for each a face a number of times as the rotor is cranked.
Processing the samples we determine a cranking speed and an average pressure/compression for that rotor face.
Normalising and reporting those results tells the user if they need to rebuild their rotary engine :-)

## Parts

The idea is to have a self contained unit with the UI included.
That is: CPU, analog measurement, LCD and button (reset).
Soft floating point is ok, this is not a demanding application.

Here is what I had in my parts bin:

Arduino Uno (Rev 3)
https://docs.arduino.cc/hardware/uno-rev3

DF Robot LCD12864 Arduino Shield
https://www.dfrobot.com/product-1084.html
