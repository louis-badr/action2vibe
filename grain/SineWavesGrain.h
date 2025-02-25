#ifndef SineWavesGrain_h
#define SineWavesGrain_h

#include <DaisyDuino.h>

class SineWavesGrain
{
private:
    Oscillator osc;
    bool isPlaying = false;
    int duration;
    float amplitude;
    unsigned long startTime;

public:
    SineWavesGrain(float sample_rate);
    float Process();
    void Play(int duration, float amplitude);
};

#endif