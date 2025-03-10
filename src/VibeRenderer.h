#ifndef VibeRenderer_h
#define VibeRenderer_h

#include <vector>

#include "OscGrain.h"

class VibeRenderer
{
private:
    Grain *grain;
    std::vector<float> binSizes;
    std::vector<float> binValues;
    int currentBin;

public:
    VibeRenderer();
    VibeRenderer(Grain &grain, std::vector<float> &binSizes);
    void Update(float sensorValue);
    float GetDistanceToNextBin(float sensorValue);
    float GetDistanceToPreviousBin(float sensorValue);
    float GetDistanceToClosestBin(float sensorValue);
};

#endif
