#ifndef WhiteNoiseGrain_h
#define WhiteNoiseGrain_h

#include <DaisyDuino.h>
#include "Grain.h"

class WhiteNoiseGrain : public Grain
{
private:
    WhiteNoise nse;
    Tone filter;
    bool isPlaying;
    float duration;
    float amplitude;
    unsigned long startTime;

public:
    WhiteNoiseGrain();
    WhiteNoiseGrain(float sample_rate, float amplitude, float duration);
    float Process() override;
    void Play() override;
};

#endif