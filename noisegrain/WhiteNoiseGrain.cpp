#include <DaisyDuino.h>
#include "WhiteNoiseGrain.h"

WhiteNoiseGrain::WhiteNoiseGrain()
{
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
            isPlaying = false;
            nse.SetAmp(0);
        }
        }
    return nse.Process();
}

void WhiteNoiseGrain::Play(int duration, float amplitude)
{
    this->duration = duration;
    this->amplitude = amplitude;
    nse.SetAmp(amplitude);
    isPlaying = true;
    startTime = millis();
}