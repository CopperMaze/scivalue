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




// Warning: This is an undocumented test sketch

#include "scivalue.h"
#include "Arduino.h"

const uint8_t nDigits = 5;
const float maxDisplay = 30000;
const int8_t minExponent = -15;
const int8_t maxExponent = 12;

SciFormat sciFormat('A', true, nDigits, maxDisplay, minExponent, maxExponent);
SciValue sciValue(sciFormat);

void test(float value) {
  int16_t tAssign;
  tAssign = micros();
  sciValue = value;
  tAssign = micros() - tAssign;

  int8_t exponent = 0;
  while (abs(value) < 1.0) {
    value *= 10.0;
    exponent -= 1;
  }
  while (abs(value) >= 10.0) {
    value *= 0.1;
    exponent += 1;
  }
  size_t n;
  n += Serial.print(value, nDigits);
  n += Serial.print('e');
  n += Serial.print(exponent);

  for (uint8_t i = 0; i < (nDigits + 7 - n); i++) {
    Serial.print(' ');
  }

  Serial.print("  -->  | ");

  int16_t tPrint;
  tPrint = micros();
  Serial.print(sciValue);
  tPrint = micros() - tPrint;

  Serial.print(" |");

  Serial.print("  tAssign = "); Serial.print(tAssign);
  Serial.print("  tPrint = "); Serial.println(tPrint);
}

float value;
float quanta;

void executeTest() {
  float tmp_maxDisplay = maxDisplay;
  uint8_t count = 0;
  while (tmp_maxDisplay > 1.0) {
    tmp_maxDisplay *= 0.1;
    count++;
  }
  count = nDigits - count;
  for (uint8_t i = 0; i < count; i++) {
    quanta *= 0.1;
  }

  for (int8_t i = minExponent - 6; i <= maxExponent + 6; i += 3) {
    float exponent = 1.0;
    for (int8_t j = i; j < 0; j++) {
      exponent *= 0.1;
    }
    for (int8_t j = i; j > 0; j--) {
      exponent *= 10.0;
    }
    test((value - quanta) * exponent);
    test(value * exponent);
    Serial.println(' ');
  }
}

void setup() {
  delay(3000);

  value = maxDisplay;
  quanta = 1.0;
  executeTest();

  value = maxDisplay;
  value = -value;
  quanta = -1.0;
  executeTest();
}

void loop() {

}

