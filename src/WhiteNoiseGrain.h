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
    float cutoff;
    float amplitude;
    float duration;
    unsigned long endTime;

public:
    WhiteNoiseGrain();
    WhiteNoiseGrain(float sample_rate, float cutoff, float amplitude, float duration);
    float Process() override;
    void Play() override;
};

#endif