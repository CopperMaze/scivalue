// Copyright (c) 2014 Benoît Roehr <benoit.roehr.ec@gmail.com>

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Arduino.h"
#include "scivalue.h"


/*==============================================================================
To use scivalue library, simply declare a SciFormat class. The constructor
takes 5 parameters. The only parameter you need to give is the Unit parameter.
The format will define how the value will be printed.
==============================================================================*/
SciFormat ampere('A');
SciFormat volt('V');


/*==============================================================================
Here is an example of the full sciFormat construction. You're invited to tweak
these values and see how the library modify its output.
==============================================================================*/
const char unit           = 'W';  //the SI letter of your unit
const bool isSigned       = true; //print a '+' caracter in front
const uint8_t nDigits     = 5;    //number of total digits, before and after '.'
const float maxDisplay    = 200;  //the value at which metric prefixe changes
const uint8_t minExponent = -15;  //the minimal exponent to reach. ex: -3 is mW
const uint8_t maxExponent = 12;   //the maximal exponent to reach. ex: 6 is MW

SciFormat watt(unit, isSigned, nDigits, maxDisplay, minExponent, maxExponent);


/*==============================================================================
Once you have declared your formats, you can declare your values. The
constructor takes only one parameter. You assign a format to your value. You
can assign the same format to different values.
==============================================================================*/
SciValue current(ampere);
SciValue voltage(volt);
SciValue power(watt);
SciValue power2(watt);

void setup() {
  delay(5000);

  //values can be assigned (float)
  voltage = 12.0;
  current = 5e-6;

  //values can be multiplied together
  power = voltage * current;

  //...and, of course, values can be printed !
  Serial.print(voltage);
  Serial.print(" x ");
  Serial.print(current);
  Serial.print(" = ");
  Serial.println(power);

  //this for loop will iterate trough all the prefixes available
  power2 = 1.0e-15;
  for (int8_t i = minExponent; i < maxExponent + 1; i += 3) {
    Serial.print("power2 = ");
    Serial.println(power2);
    power2 *= 1000;
  }
}

void loop() {
}

/*==============================================================================

Some notes:

The max display value can be exceeded when the max exponent have been reached
and there is still enough digits to print the value. For exemple,
maxExponent = 0, nDigits = 6, value = 1e5 will print as "100000._U". 1e6 will
print "ovf......".

Otherwise, the max display value level is never displayed. The scale changes
just before the mantissa reach the maxDisplay value.
So for exemple, for nDigits = 4 and maxDisplay = 1000.0,
999.90 will be printed as 999.9_U, and 999.95 will be rounded at 1000 and then
printed as "1.000kU".

The minExponent may be usefull when dealing with extremely small values or zero.
For exemple, if you are measuring voltage, you may not need to display femto
scale value. If milli volts are enough, you may set minExponent = -3. Zero will
then print as "0.000mV" instead of "0.000fV".

Inputing a negative value into a non signed formatted value will still print
the minus sign in front of the value, resulting in an extra character.

Otherwise, the size of the output screen is always the same, making scivalue
easy to use with lcd where string size managament can sometimes be tricky.
When overflown, spaces are added to keep strings length the same.

==============================================================================*/

//whole file
