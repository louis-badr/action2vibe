#ifndef BinRenderer_h
#define BinRenderer_h

#include <vector>

#include "OscGrain.h"

class BinRenderer
{
private:
    Grain *grain;
    std::vector<float> binSizes;
    std::vector<float> binValues;
    int currentBin;

public:
    BinRenderer();
    BinRenderer(Grain &grain, std::vector<float> &binSizes);
    void Update(float sensorValue);
    float GetDistanceToNextBin(float sensorValue);
    float GetDistanceToPreviousBin(float sensorValue);
    float GetDistanceToClosestBin(float sensorValue);
};

#endif
