#ifndef OscGrain_h
#define OscGrain_h

#include <DaisyDuino.h>

class OscGrain
{
private:
    Oscillator osc;
    bool isPlaying = false;
    int duration;
    float amplitude;
    unsigned long startTime;

public:
    OscGrain();
    OscGrain(float sample_rate);
    float Process();
    void Play(int duration, float amplitude);
};

#endif