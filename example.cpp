#include "ISA.h"
#include <iostream>

int main() {
    isa atm;
    float altitude;

    std::cout << "Enter altitude in meters: ";
    std::cin >> altitude;

    float tempK = atm.getAltTemp(altitude);
    float presPa = atm.getAltPres(altitude);
    float rho = atm.getAltRho(altitude);
    float speedOfSound = atm.getAltSpeedOfSound(altitude);
    float tempC = atm.tempKtoC(tempK);
    float tempF = atm.tempKtoF(tempK);

    std::cout << "At altitude: " << altitude << " m\n";
    std::cout << "Temperature: " << tempK << " K (" << tempC << " C, " << tempF << " F)\n";
    std::cout << "Pressure: " << presPa << " Pa\n";
    std::cout << "Density: " << rho << " kg/m³\n";
    std::cout << "Speed of Sound: " << speedOfSound << " m/s\n";
}