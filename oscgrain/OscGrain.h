#ifndef OscGrain_h
#define OscGrain_h

#include <DaisyDuino.h>
#include <vector>

class OscGrain
{
private:
    std::vector<Oscillator> oscs;
    bool isPlaying = false;
    int duration;
    unsigned long startTime;
    std::vector<float> amplitudes;

public:
    OscGrain();
    OscGrain(float sample_rate, std::vector<float>& frequencies, std::vector<float>& amplitudes);
    float Process();
    void Play(int duration);
};

#endif