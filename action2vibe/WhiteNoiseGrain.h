#ifndef WhiteNoiseGrain_h
#define WhiteNoiseGrain_h

#include <DaisyDuino.h>
#include "Grain.h"

class WhiteNoiseGrain : public Grain
{
private:
    WhiteNoise nse;
    bool isPlaying;
    int duration;
    float amplitude;
    unsigned long startTime;

public:
    WhiteNoiseGrain();
    WhiteNoiseGrain(float amplitude, int duration);
    float Process() override;
    void Play() override;
};

#endif