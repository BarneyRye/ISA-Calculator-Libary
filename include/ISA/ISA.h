#ifndef ISA_H
#define ISA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define troposhpere_altitude 11000.0f
#define troposhpere_lapse_rate -0.0065f
#define tropopause_altitude 20000.0f
#define stratosphere_altitude 32000.0f
#define stratosphere_lapse_rate 0.001f
#define mesosphere_altitude 47000.0f
#define mesosphere_lapse_rate -0.0028f


class isa {
    public:
        isa(float slTemp = 288.15f, float slRho = 1.225f, float slPres = 101325.0f);
        float getAltTemp(float altitude);
        float getAltPres(float altitude);
        float getAltRho(float altitude);
        float getAltSpeedOfSound(float altitude);

    private:
        float _slTemp;
        float _slRho;
        float _slPres;
        float _seaLevel;
        float _g;
        float _R;
        float _gamma;

        float calcTropShpereTemp(float altitude);
        float calcTropShperePres(float altitude, float temp);
        float calcTropoPausePres(float altitude, float temp, float pres_init);
        float calcStratoSphereTemp(float altitude, float temp_init);
        float calcStratoSpherePres(float altitude, float temp,float pres_init);
        float calcMesoSphereTemp(float altitude, float temp_init);
        float calcMesoSpherePres(float altitude, float temp, float pres_init);
};

#endif // ISA_H