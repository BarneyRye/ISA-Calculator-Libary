# include "ISA.h"

isa::isa(float slTemp = 288.15f, float slRho = 1.225f, float slPres = 101325.0f) //Constructor with default sea level conditions
    : _slTemp(slTemp), _slRho(slRho), _slPres(slPres), _seaLevel(0.0f), _g(9.80665f), _R(287.058f), _gamma(1.4f) {}

float isa::getAltTemp(float altitude) { //Get temperature at altitude in Kelvin
    if (altitude < troposhpere_altitude) { //Troposphere
        return calcTropShpereTemp(altitude);
    }
    else if (altitude < tropopause_altitude) { //Tropopause
        return calcTropShpereTemp(troposhpere_altitude); //Isothermal layer so return temp at tropopause base/tropsophere top
    }
    else if (altitude < stratosphere_top_altitude) { //Stratosphere
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude); //Stratosphere base calculated from tropopause temp
        return calcStratoSphereTemp(altitude, tropopauseTemp); //Stratosphere temperature calculation
    }
    else if (altitude < stratopause_altitude) { //Stratopause
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude); //Same as above, but uses stratosphere top temp as isothermal layer
        return calcStratoSphereTemp(stratosphere_top_altitude, tropopauseTemp);
    }
    else if (altitude < mesosphere_top_altitude) { //Mesosphere
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude);
        float stratoTemp = calcStratoSphereTemp(stratosphere_top_altitude, tropopauseTemp); //caculate stratosphere top temp for mesosphere base
        return calcMesoSphereTemp(altitude, stratoTemp); //Mesosphere temperature calculation
    }
    else {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude); //85km Max altitude
        float stratoTemp = calcStratoSphereTemp(stratosphere_top_altitude, tropopauseTemp);
        return calcMesoSphereTemp(mesosphere_top_altitude, stratoTemp); //Calculate mesosphere top temp
    }
}

float isa::getAltPres(float altitude) { //Get pressure at altitude in Pascals
    //Calculates pressure/temperature at previous layers to use as base for current layer
    //Works from sea level up to 85km through the layers
    //"presAtLayer" is the pressure at the base of hat layer
    //"layerTemp" is the temperature in that layer or top of that layer if being used for next layer calculation
    if (altitude < troposhpere_altitude) { //Troposphere
        float temp = calcTropShpereTemp(altitude);
        return calcTropShperePres(altitude, temp);
    }
    else if (altitude < tropopause_altitude) {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude); //Tropopause
        float presAtTropopause = calcTropShperePres(troposhpere_altitude, tropopauseTemp);
        return calcTropoPausePres(altitude, tropopauseTemp, presAtTropopause);
    }
    else if (altitude < stratosphere_top_altitude) {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude); //Stratosphere
        float presAtTropopause = calcTropShperePres(troposhpere_altitude, tropopauseTemp);
        float presAtStratosphere = calcTropoPausePres(tropopause_altitude, tropopauseTemp, presAtTropopause);
        float stratoTemp = calcStratoSphereTemp(altitude, tropopauseTemp);
        return calcStratoSpherePres(altitude, tropopauseTemp, stratoTemp, presAtStratosphere);
    }
    else if (altitude < stratopause_altitude) {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude); //Stratopause
        float presAtTropopause = calcTropShperePres(troposhpere_altitude, tropopauseTemp);
        float presAtStratosphere = calcTropoPausePres(tropopause_altitude, tropopauseTemp, presAtTropopause);
        float stratoTemp = calcStratoSphereTemp(stratosphere_top_altitude, tropopauseTemp);
        float presAtStratoPause = calcStratoSpherePres(stratosphere_top_altitude, tropopauseTemp, stratoTemp, presAtStratosphere);
        return calcStratoPausePres(altitude, stratoTemp, presAtStratoPause);
    }
    else if (altitude < mesosphere_top_altitude) {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude); //Mesosphere
        float presAtTropopause = calcTropShperePres(troposhpere_altitude, tropopauseTemp);
        float presAtStratosphere = calcTropoPausePres(tropopause_altitude, tropopauseTemp, presAtTropopause);
        float stratoTemp = calcStratoSphereTemp(stratosphere_top_altitude, tropopauseTemp);
        float presAtStratoPause = calcStratoSpherePres(stratosphere_top_altitude, tropopauseTemp, stratoTemp, presAtStratosphere);
        float presAtMesoSphere = calcStratoPausePres(stratopause_altitude, stratoTemp, presAtStratoPause);
        float mesoTemp = calcMesoSphereTemp(altitude, stratoTemp);
        return calcMesoSpherePres(altitude, stratoTemp, mesoTemp, presAtMesoSphere);
    }
    else {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude); //Above Mesosphere Top
        float presAtTropopause = calcTropShperePres(troposhpere_altitude, tropopauseTemp);
        float presAtStratosphere = calcTropoPausePres(tropopause_altitude, tropopauseTemp, presAtTropopause);
        float stratoTemp = calcStratoSphereTemp(stratosphere_top_altitude, tropopauseTemp);
        float presAtStratoPause = calcStratoSpherePres(stratosphere_top_altitude, tropopauseTemp, stratoTemp, presAtStratosphere);
        float presAtMesoSphere = calcStratoPausePres(stratopause_altitude, stratoTemp, presAtStratoPause);
        float mesoTemp = calcMesoSphereTemp(mesosphere_top_altitude, stratoTemp);
        return calcMesoSpherePres(mesosphere_top_altitude, stratoTemp, mesoTemp, presAtMesoSphere);
    }
}

float isa::getAltRho(float altitude) { //Get density at altitude in kg/m^3
    float pres = getAltPres(altitude); //Using ideal gas law to calculate density
    float temp = getAltTemp(altitude);
    return pres / (_R * temp);
}

float isa::getAltSpeedOfSound(float altitude) { //Get speed of sound at altitude in m/s
    float temp = getAltTemp(altitude); //Using speed of sound formula
    return sqrtf(_gamma * _R * temp);
}

float isa::tempKtoC(float tempK) { //Convert Kelvin to Celsius
    return tempK - 273.15f;
}

float isa::tempKtoF(float tempK) { //Convert Kelvin to Fahrenheit
    return (tempK - 273.15f) * (9.0f/5.0f) + 32.0f;
}

// Private layer calculation functions:
//Temperature:
// For L!=0, T = T0 + L*(h - h0)
// For L=0, T = T0
//Pressure:
// For L!=0, P = P0*(T/T0)^(-g/(L*R))
// For L=0, P = P0*exp(-g/(R*T0)*(h - h0))
float isa::calcTropShpereTemp(float altitude) {  //Troposphere temperature calculation
    return _slTemp + troposhpere_lapse_rate * (altitude - _seaLevel);
}

float isa::calcTropShperePres(float altitude, float temp_trop) { //Troposphere pressure calculation
    return _slPres * powf((temp_trop / _slTemp), (-_g / (troposhpere_lapse_rate * _R)));
}

float isa::calcTropoPausePres(float altitude, float temp_base, float pres_base) { //Tropopause pressure calculation
    return pres_base * expf((-_g / (_R * temp_base)) * (altitude - troposhpere_altitude));
}

float isa::calcStratoSphereTemp(float altitude, float temp_base) { //Stratosphere temperature calculation
    if (altitude < stratosphere_middle_altitude) { //If statement determines how far into layer the altitude is, to do the correct calculation
        return temp_base + stratosphere_lower_lapse_rate * (altitude - tropopause_altitude);
    }
    else {
        float temp_middle = temp_base + stratosphere_lower_lapse_rate * (stratosphere_middle_altitude - tropopause_altitude);
        return temp_middle + stratosphere_upper_lapse_rate * (altitude - stratosphere_middle_altitude);
    }
}

float isa::calcStratoSpherePres(float altitude, float temp_base, float temp_strato, float pres_base) { //Stratosphere pressure calculation
    if (altitude < stratosphere_middle_altitude) {
        return pres_base* powf(
            (temp_strato/temp_base),
            (-_g / (stratosphere_lower_lapse_rate * _R))
        );
    }
    else {
        float temp_middle = calcStratoSphereTemp(stratosphere_middle_altitude, temp_base);
        float pres_middle = pres_base * powf(
            (temp_middle / temp_base),
            (-_g / (stratosphere_lower_lapse_rate * _R))
        );
        return pres_middle * powf(
            (temp_strato / temp_middle),
            (-_g / (stratosphere_upper_lapse_rate * _R))
        );
    }
}

float isa::calcStratoPausePres(float altitude, float temp_base, float pres_base) { //Stratopause pressure calculation
    return pres_base * expf(
        ((-_g / (_R * temp_base))) * (altitude - stratopause_altitude)
    );
}

float isa::calcMesoSphereTemp(float altitude, float temp_base) { //Mesosphere temperature calculation
    if (altitude < mesosphere_middle_altitude) {
        return temp_base + mesosphere_lower_lapse_rate * (altitude - stratopause_altitude);
    }
    else {
        float temp_middle = temp_base + mesosphere_lower_lapse_rate * (mesosphere_middle_altitude - stratopause_altitude);
        return temp_middle + mesosphere_upper_lapse_rate * (altitude - mesosphere_middle_altitude);
    }
}

float isa::calcMesoSpherePres(float altitude, float temp_base, float temp_meso, float pres_base) { //Mesosphere pressure calculation
    if (altitude < mesosphere_middle_altitude) {
        return pres_base * powf(
            (temp_meso / temp_base),
            (-_g / (mesosphere_lower_lapse_rate * _R))
        );
    }
    else {
        float temp_middle = calcMesoSphereTemp(mesosphere_middle_altitude, temp_base);
        float pres_middle = pres_base * powf(
            (temp_middle / temp_base),
            (-_g / (mesosphere_lower_lapse_rate * _R))
        );
        return pres_middle * powf(
            (temp_meso / temp_middle),
            (-_g / (mesosphere_upper_lapse_rate * _R))
        );
    }
}
