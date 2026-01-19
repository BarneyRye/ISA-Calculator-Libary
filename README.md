# ISO_Calulator_Library

A libary to calulate atmospheric properties up to 85km using the International Standard Atmosphere Model

The libary allows you to calculate:  </br>
-Pressure (Pa)  </br>
-Temperature (K)  </br>
-Density (kg/m^3)  </br>
-Speed of sound (m/s)  </br>
-Convert Kelvin to Celsius </br>
-Convert Kelvin to Fahrenheit

## Example Code:

```cpp
#include <iostream>
#include <ISA.h>

int main() {
  isa isaCalc;
  float altiude = 14000.0f;

  float tempK = isaCalc.getAltTemp(altiude);
  float pressure = isaCalc.getAltPres(altiude);
  float density = isaCalc.getAltRho(altiude);
  float speed =  isaCalc.getAltSpeedOfSound(altiude);
  float tempC = isaCalc.tempKtoC(tempK);
  float tempF = isaCalc.tempKtoF(tempK);

  ...

  return 0;
}
```
