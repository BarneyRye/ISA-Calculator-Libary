# include "ISA.h"

isa::isa(float slTemp = 288.15f, float slRho = 1.225f, float slPres = 101325.0f)
    : _slTemp(slTemp), _slRho(slRho), _slPres(slPres), _seaLevel(0.0f), _g(9.80665f), _R(287.058f), _gamma(1.4f) {}

float isa::getAltTemp(float altitude) {
    if (altitude < troposhpere_altitude) {
        return calcTropShpereTemp(altitude);
    }
    else if (altitude < tropopause_altitude) {
        return calcTropShpereTemp(tropopause_altitude);
    }
    else if (altitude < stratosphere_altitude) {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude);
        return calcStratoSphereTemp(altitude, tropopauseTemp);
    }
    else if (altitude < mesosphere_altitude) {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude);
        float stratoTemp = calcStratoSphereTemp(stratosphere_altitude, tropopauseTemp);
        return calcMesoSphereTemp(altitude, stratoTemp);
    }
    else {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude);
        float stratoTemp = calcStratoSphereTemp(stratosphere_altitude, tropopauseTemp);
        return calcMesoSphereTemp(mesosphere_altitude, stratoTemp);
    }
}

float isa::getAltPres(float altitude) {
    if (altitude < troposhpere_altitude) {
        float temp = calcTropShpereTemp(altitude);
        return calcTropShperePres(altitude, temp);
    }
    else if (altitude < tropopause_altitude) {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude);
        float presAtTropopause = calcTropShperePres(troposhpere_altitude, tropopauseTemp);
        return calcTropoPausePres(altitude, tropopauseTemp, presAtTropopause);
    }
    else if (altitude < stratosphere_altitude) {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude);
        float presAtTropopause = calcTropoPausePres(tropopause_altitude, tropopauseTemp, calcTropShperePres(troposhpere_altitude, tropopauseTemp));
        float stratoTemp = calcStratoSphereTemp(altitude, tropopauseTemp);
        return calcStratoSpherePres(altitude, stratoTemp, presAtTropopause);
    }
    else if (altitude < mesosphere_altitude) {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude);
        float presAtTropopause = calcTropoPausePres(tropopause_altitude, tropopauseTemp, calcTropShperePres(troposhpere_altitude, tropopauseTemp));
        float stratoTemp = calcStratoSphereTemp(stratosphere_altitude, tropopauseTemp);
        return calcStratoSpherePres(stratosphere_altitude, stratoTemp, presAtTropopause);
    }
    else {
        float tropopauseTemp = calcTropShpereTemp(troposhpere_altitude);
        float presAtTropopause = calcTropoPausePres(tropopause_altitude, tropopauseTemp, calcTropShperePres(troposhpere_altitude, tropopauseTemp));
        float stratoTemp = calcStratoSphereTemp(stratosphere_altitude, tropopauseTemp);
        return calcStratoSpherePres(mesosphere_altitude, stratoTemp, presAtTropopause);
    }
}

float isa::getAltRho(float altitude) {
    float pres = getAltPres(altitude);
    float temp = getAltTemp(altitude);
    return pres / (_R * temp);
}

float isa::getAltSpeedOfSound(float altitude) {
    float temp = getAltTemp(altitude);
    return sqrtf(_gamma * _R * temp);
}

float isa::calcTropShpereTemp(float altitude) {
    return _slTemp + troposhpere_lapse_rate * (altitude - _seaLevel);
}

float isa::calcTropShperePres(float altitude, float temp) {
    return _slPres * powf((temp / _slTemp), (-_g / (troposhpere_lapse_rate * _R)));
}

float isa::calcTropoPausePres(float altitude, float temp, float pres_init) {
    return pres_init * expf((-_g / (_R * temp)) * (altitude - troposhpere_altitude));
}

float isa::calcStratoSphereTemp(float altitude, float temp_init) {
    return temp_init + stratosphere_lapse_rate * (altitude - tropopause_altitude);
}

float isa::calcStratoSpherePres(float altitude, float temp, float pres_init) {
    return pres_init * powf(
        (temp / calcStratoSphereTemp(tropopause_altitude, calcTropShpereTemp(troposhpere_altitude))),
        (-_g / (stratosphere_lapse_rate * _R)));
}

float isa::calcMesoSphereTemp(float altitude, float temp_init) {
    return temp_init + mesosphere_lapse_rate * (altitude - stratosphere_altitude);
}

float isa::calcMesoSpherePres(float altitude, float temp, float pres_init) {
    return pres_init * powf(
        (temp / calcMesoSphereTemp(stratosphere_altitude, calcStratoSphereTemp(tropopause_altitude, calcTropShpereTemp(troposhpere_altitude)))),
        (-_g / (mesosphere_lapse_rate * _R)));
}
