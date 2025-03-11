#ifndef AccelRenderer_h
#define AccelRenderer_h

#include <vector>

#include "OscGrain.h"

class AccelRenderer
{
private:
    Grain *grain;
    float frequency;
    float previousSensorValue;

public:
    AccelRenderer();
    AccelRenderer(Grain &grain);
    void Update(float sensorValue);
};

#endif
