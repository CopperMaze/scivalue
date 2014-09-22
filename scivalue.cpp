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

#ifndef SCIVALUE_cpp
#define SCIVALUE_cpp

#include "scivalue.h"

//==============================================================================
// Metric prefixes table
//==============================================================================
const char SciFormat::prefixes[10] = {
  'f', 'p', 'n', 'u', 'm', '_', 'k', 'M', 'G', 'T'
};

//==============================================================================
// SciFormat Constructor
//==============================================================================
SciFormat::SciFormat(char unit,
                     bool isSigned,
                     uint8_t nDigits,
                     float maxDisplay,
                     int8_t minExponent,
                     int8_t maxExponent):
  unit(unit),
  isSigned(isSigned),
  nDigits(nDigits),
  maxDisplay(maxDisplay),
  minExponent((minExponent > -15) ? minExponent : -15 ),
  maxExponent((maxExponent < 12) ? maxExponent : 12)

{
  //recalculating the maxDisplay value for perfect rounding
  float rounding = 0.5;
  float tmp_maxDisplay = maxDisplay;
  uint8_t count = 0;
  while (tmp_maxDisplay > 1.0) {
    tmp_maxDisplay *= 0.1;
    count++;
  }
  count = nDigits - count;
  for (uint8_t i = 0; i < count; i++) {
    rounding *= 0.1;
  }
  maxDisplay -= rounding;
}


//==============================================================================
// Assignation method
// Compute mantissa and exponent based on the format
//==============================================================================
void SciValue::operator=(const float& newValue) {
  mantissa = value = newValue;
  exponent = 0;

  //now we search the right exponent and we fit the mantissa into the display
  while ((exponent < sciFormat->maxExponent) &&
         !(abs(mantissa) < sciFormat->maxDisplay)) {
    exponent += 3;
    mantissa *= 0.001;
  }
  while ((exponent > sciFormat->minExponent) &&
         (abs(mantissa) < sciFormat->maxDisplay * 0.001)) {
    exponent -= 3;
    mantissa *= 1000.0;
  }
}

void SciValue::operator=(const SciValue& other) {
  *this = other.value;
}

//==============================================================================
// Compound operators
//==============================================================================
void SciValue::operator+=(const float& x) {
  *this = value + x;
}
void SciValue::operator+=(const SciValue& other) {
  *this += other.value;
}
void SciValue::operator-=(const float& x) {
  *this = value - x;
}
void SciValue::operator-=(const SciValue& other) {
  *this += -other.value;
}
void SciValue::operator*=(const float& x) {
  *this = value * x;
}
void SciValue::operator*=(const SciValue& other) {
  *this = value * other.value;
}
void SciValue::operator/=(const float& x) {
  *this = value / x;
}
void SciValue::operator/=(const SciValue& other) {
  *this = value / other.value;
}
//==============================================================================
// Arithmetic operators
//==============================================================================
float SciValue::operator+(const float& x) {
  return value + x;
}
float SciValue::operator+(const SciValue& other) {
  return value + other.value;
}
float SciValue::operator-(const float& x) {
  return value - x;
}
float SciValue::operator-(const SciValue& other) {
  return value - other.value;
}
float SciValue::operator*(const float& x) {
  return value * x;
}
float SciValue::operator*(const SciValue& other) {
  return value * other.value;
}
float SciValue::operator/(const float& x) {
  return value / x;
}
float SciValue::operator/(const SciValue& other) {
  return value / other.value;
}

//==============================================================================
// Printing method.
//==============================================================================
size_t SciValue::printTo(Print& p) const {
  //counting number of digit before the point
  uint8_t nTens = 0;
  if (abs(mantissa) >= 1.0) {
    float tmp_mantissa = mantissa;
    while ((abs(tmp_mantissa)) >= 1.0) {
      tmp_mantissa *= 0.1;
      nTens += 1;
    }
  }
  else {
    //even if mantissa < 1.0, we still have to print the zero: value = 0.123
    nTens = 1;
  }

  //calculating the number of decimals to display after the point
  int8_t nDecimals;
  nDecimals = sciFormat->nDigits - nTens;

  //print return value
  size_t n = 0;

  //if the number of decimals is < 0, it means the display is overflown
  if ((nDecimals) < 0) {
    n += p.print("ovf");
    uint8_t nSpaces = sciFormat->nDigits + sciFormat->isSigned;

    //adding spaces to keep the same length
    for (uint8_t i = 0; i < nSpaces; i++) {
      n += p.print(' ');
    }
  }

  else {
    //if the format allows signed value, determine the sign
    if (sciFormat->isSigned && (mantissa >= 0)) {
      n += p.print('+');
    }

    n += p.print(mantissa, nDecimals);

    //if there is no decimals to print, print the point
    if (nDecimals == 0) {
      Serial.print('.');
    }

    //getting the right SI prefix
    size_t prefixID = (size_t)(5 + (exponent / 3));
    n += p.print(sciFormat->prefixes[prefixID]);

    //printing the Unit
    n += p.print(sciFormat->unit);
  }

  return n;
}

#endif //whole file