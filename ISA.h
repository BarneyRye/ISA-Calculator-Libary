#ifndef ISA_H
#define ISA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>


//Max heights and lapse rates for each atmospheric layer
//Troposphere
#define troposhpere_altitude 11000.0f
#define troposhpere_lapse_rate -0.0065f
//Tropopause (Isothermal Layer)
#define tropopause_altitude 20000.0f
//Stratosphere
#define stratosphere_middle_altitude 32000.0f //Transition altitude for stratosphere lapse rates
#define stratosphere_lower_lapse_rate 0.001f
#define stratosphere_top_altitude 47000.0f
#define stratosphere_upper_lapse_rate 0.0028f
//Stratopause (Isothermal Layer)
#define stratopause_altitude 51000.0f
//Mesosphere
#define mesosphere_middle_altitude 71000.0f //Transition altitude for mesosphere lapse rates
#define mesosphere_lower_lapse_rate -0.0028f
#define mesosphere_top_altitude 85000.0f
#define mesosphere_upper_lapse_rate -0.0020f


class isa { //International Standard Atmosphere Calculator
    public:
        isa(float slTemp = 288.15f, float slRho = 1.225f, float slPres = 101325.0f); //Constructor with default sea level conditions
        float getAltTemp(float altitude); //Get temperature at altitude in Kelvin
        float getAltPres(float altitude); //Get pressure at altitude in Pascals
        float getAltRho(float altitude); //Get density at altitude in kg/m^3
        float getAltSpeedOfSound(float altitude); //Get speed of sound at altitude in m/s
        float tempKtoC(float tempK); //Convert Kelvin to Celsius
        float tempKtoF(float tempC); //Convert Kelvin to Fahrenheit

    private:
        float _slTemp; //Sea level properties
        float _slRho;
        float _slPres;
        float _seaLevel;
        float _g; //Gravitational acceleration, assumes constant g,R and gamma
        float _R; //Specific gas constant for dry air
        float _gamma; //Ratio of specific heats for dry air

        float calcTropShpereTemp(float altitude); //Private functions to calculate properties at respective layers
        float calcTropShperePres(float altitude, float temp);
        float calcTropoPausePres(float altitude, float temp_base, float pres_base);
        float calcStratoSphereTemp(float altitude, float temp_base);
        float calcStratoSpherePres(float altitude, float temp_base, float temp_strato, float pres_base);
        float calcStratoPausePres(float altitude, float temp_base,float pres_base);
        float calcMesoSphereTemp(float altitude, float temp_base);
        float calcMesoSpherePres(float altitude, float temp_base, float temp_meso, float pres_base);
};

#endif // ISA_H