#include <DaisyDuino.h>
#include "WhiteNoiseGrain.h"

WhiteNoiseGrain::WhiteNoiseGrain() {}

WhiteNoiseGrain::WhiteNoiseGrain(float sample_rate, float amplitude, float duration)
{
    this->amplitude = amplitude;
    this->duration = duration * 1000;
    nse.Init();
    nse.SetAmp(0);
    filter.Init(sample_rate);
    isPlaying = false;
}

float WhiteNoiseGrain::Process()
{
    float cutoff_freq = 500.0f;
    float signal;
    filter.SetFreq(cutoff_freq);
    if (isPlaying)
    {
        if (micros() - startTime > duration)
        {
            nse.SetAmp(0);
            isPlaying = false;
        }
    }
    signal = nse.Process();
    return filter.Process(signal);
}

void WhiteNoiseGrain::Play()
{
    // only play grain if it's not already playing
    if (!isPlaying)
    {
        nse.SetAmp(amplitude);
        isPlaying = true;
        startTime = micros();
    }
}