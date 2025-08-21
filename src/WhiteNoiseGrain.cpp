#include <DaisyDuino.h>
#include "WhiteNoiseGrain.h"

WhiteNoiseGrain::WhiteNoiseGrain() {}

WhiteNoiseGrain::WhiteNoiseGrain(float sample_rate, float cutoff, float amplitude, float duration)
{
    this->cutoff = cutoff;
    this->amplitude = amplitude;
    this->duration = duration * 1000;
    nse.Init();
    nse.SetAmp(0);
    filter.Init(sample_rate);
    isPlaying = false;
}

float WhiteNoiseGrain::Process()
{
    if (isPlaying)
    {
        if (micros() >= endTime)
        {
            nse.SetAmp(0);
            isPlaying = false;
        }
        else
        {
            filter.SetFreq(cutoff);
            float signal = nse.Process();
            return filter.Process(signal);
        }
    }
    return 0.0f;
}

void WhiteNoiseGrain::Play()
{
    // only play grain if it's not already playing
    if (!isPlaying)
    {
        nse.SetAmp(amplitude);
        isPlaying = true;
        endTime = micros() + duration;
    }
}