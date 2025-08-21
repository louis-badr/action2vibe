#ifndef ContinuousRenderer_h
#define ContinuousRenderer_h

#include <DaisyDuino.h>
#include <vector>

class ContinuousRenderer
{
private:
    Oscillator osc;
    float grainSize;
    std::vector<float> binSizes;
    std::vector<float> binValues;
    int currentBin;
    float frequency;
    float amplitude;
    float minFreq;
    float maxFreq;
    float previousSensorValue;
    unsigned long currentTime;
    unsigned long previousTime;
    float speed;
    float smoothedSpeed;

public:
    ContinuousRenderer();
    ContinuousRenderer(float sample_rate, float grainSize, float minFreq, float maxFreq, std::vector<float> &binSizes);
    void Update(float sensorValue);
    float Process();
    float GetDistanceToNextBin(float sensorValue);
    float GetDistanceToPreviousBin(float sensorValue);
    float GetDistanceToClosestBin(float sensorValue);
};

#endif
