#include <DaisyDuino.h>
#include "WhiteNoiseGrain.h"

WhiteNoiseGrain::WhiteNoiseGrain() {}

WhiteNoiseGrain::WhiteNoiseGrain(float amplitude, int duration)
{
    this->amplitude = amplitude;
    this->duration = duration;
    nse.Init();
    nse.SetAmp(0);
    isPlaying = false;
}

float WhiteNoiseGrain::Process()
{
    if (isPlaying)
    {
        if (millis() - startTime > duration)
        {
            nse.SetAmp(0);
            isPlaying = false;
        }
    }
    return nse.Process();
}

void WhiteNoiseGrain::Play()
{
    // only play grain if it's not already playing
    if (!isPlaying)
    {
        nse.SetAmp(amplitude);
        isPlaying = true;
        startTime = millis();
    }
}