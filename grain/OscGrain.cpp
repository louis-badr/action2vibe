#include <DaisyDuino.h>
#include "OscGrain.h"

OscGrain::OscGrain() {}

OscGrain::OscGrain(float sample_rate)
{
    osc.Init(sample_rate);
    osc.SetFreq(440.0f);
    osc.SetAmp(0.0f);
    osc.SetWaveform(osc.WAVE_SIN);

    isPlaying = false;
}

float OscGrain::Process()
{
    if (isPlaying)
    {
        if (millis() - startTime > duration)
        {
            isPlaying = false;
            osc.SetAmp(0);
        }
    }
    return osc.Process();
}

void OscGrain::Play(int duration, float amplitude)
{
    this->duration = duration;
    this->amplitude = amplitude;
    osc.SetAmp(amplitude);
    osc.Reset();
    isPlaying = true;
    startTime = millis();
}