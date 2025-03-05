#ifndef VibeRenderer_h
#define VibeRenderer_h

#include <vector>

#include "OscGrain.h"

class VibeRenderer
{
private:
    OscGrain *grain;
    std::vector<float> binSizes;
    std::vector<float> binValues;
    int currentBin;
public:
    VibeRenderer();
    VibeRenderer(OscGrain &grain, std::vector<float> &binSizes);
    void Update(float sensorValue);
};

#endif
