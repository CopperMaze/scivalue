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

#ifndef SCIVALUE_h
#define SCIVALUE_h

#include "stdint.h"
#include "Arduino.h"

//==============================================================================
// SciFormat Class
//==============================================================================
class SciFormat {
public:
  SciFormat();
  SciFormat(char unit,
            bool isSigned = false,
            uint8_t nDigits = 6,
            float maxDisplay = 1000.0,
            int8_t minExponent = -15,
            int8_t maxExponent = 12);
private:
  friend class SciValue;
  char unit; // single char for the moment
  bool isSigned; // if true print a + sign in front of positive value
  uint8_t nDigits; // number of digits to display. 99.999 takes 5 digits
  float maxDisplay; // if = 1000, 999 print as 999, 1000 print as 1.00k
  int8_t maxExponent; // min exponent to no exceed (ex: -6 => µicro)
  int8_t minExponent; // max exponent to not exceed (ex: 6 => Mega)
  float rounding;
  static const char prefixes[10]; // holds the metric prefixes
};

//==============================================================================
// SciValue class
//==============================================================================
class SciValue: public Printable {
public:
  SciValue(SciFormat& sciFormat):
    sciFormat(&sciFormat) {}

  // printing takes aproximatively 1ms, so use it wisely
  virtual size_t printTo(Print& p) const;

  void operator=(const float& x);
  void operator=(const SciValue& other);
  void operator+=(const float& x);
  void operator+=(const SciValue& other);
  void operator-=(const float& x);
  void operator-=(const SciValue& other);
  void operator*=(const float& x);
  void operator*=(const SciValue& other);
  void operator/=(const float& x);
  void operator/=(const SciValue& other);
  float operator+(const float& x);
  float operator+(const SciValue& other);
  float operator-(const float& x);
  float operator-(const SciValue& other);
  float operator*(const float& x);
  float operator*(const SciValue& other);
  float operator/(const float& x);
  float operator/(const SciValue& other);

private:
  uint8_t getTensNum(float value) const;
  void formatValue();
  SciFormat* sciFormat;
  float value;
  float mantissa;
  int8_t exponent;
  size_t nChar;
};

#endif //whole file
