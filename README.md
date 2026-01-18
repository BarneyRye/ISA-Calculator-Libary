# ISO_Calulator_Library

A libary to calulate atmospheric properties up to 85km using the International Standard Atmosphere Model

The libary allows you to calcualte:  </br>
-Pressure  </br>
-Temperature  </br>
-Density  </br>
-Speed of sound  </br>

## Example Code:

```cpp
#include <iostream>
#include <ISA.h>

int main() {
  isa isaCalc;
  float altiude = 14000.0f;

  float temp = isaCalc.getAltTemp(altiude);
  float pressure = isaCalc.getAltPres(altiude);
  float density = isaCalc.getAltRho(altiude);
  float speed =  isaCalc.getAltSpeedOfSound(altiude);

  ...

  return 0;
}
```
