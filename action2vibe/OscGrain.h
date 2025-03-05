#ifndef OscGrain_h
#define OscGrain_h

#include <DaisyDuino.h>
#include <vector>

class OscGrain
{
private:
    std::vector<Oscillator> oscs;
    bool isPlaying;
    int duration;
    std::vector<float> amplitudes;
    unsigned long startTime;

public:
    OscGrain();
    OscGrain(float sample_rate, std::vector<float> &frequencies, std::vector<float> &amplitudes, int duration);
    float Process();
    void Play();
};

#endif