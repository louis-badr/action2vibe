#ifndef OscGrain_h
#define OscGrain_h

#include <DaisyDuino.h>
#include "Grain.h"

class OscGrain : public Grain
{
private:
    Oscillator osc;
    bool isPlaying;
    float frequency;
    float amplitude;
    float duration;
    unsigned long startTime;

public:
    OscGrain();
    OscGrain(float sample_rate, float frequency, float amplitude, float duration);
    float Process() override;
    void Play() override;
    void SetFrequency(float frequency);
    void SetAmplitude(float amplitude);
    void SetDuration(int duration);
    void AdjustDuration();    
};

#endif