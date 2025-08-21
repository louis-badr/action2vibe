#include <DaisyDuino.h>
#include "OscGrain.h"

OscGrain::OscGrain() {}

OscGrain::OscGrain(float sample_rate, float frequency, float amplitude, float duration)
{
    this->frequency = frequency;
    this->amplitude = amplitude;
    this->duration = duration * 1000;
    osc.Init(sample_rate);
    osc.SetFreq(frequency);
    osc.SetAmp(0.0f);
    osc.SetWaveform(osc.WAVE_SIN);
    isPlaying = false;
}

float OscGrain::Process()
{
    if (isPlaying)
    {
        if (micros() >= endTime)
        {
            osc.SetAmp(0.0f);
            isPlaying = false;
        }
        else
        {
            return osc.Process();
        }
    }
    return 0.0f;
}

void OscGrain::Play()
{
    // only play grain if it's not already playing
    if (!isPlaying)
    {
        osc.Reset();    // reset phase
        osc.SetAmp(amplitude);
        isPlaying = true;
        endTime = micros() + duration;
    }
}

void OscGrain::SetFrequency(float frequency)
{
    this->frequency = frequency;
    osc.SetFreq(frequency);
}

void OscGrain::SetAmplitude(float amplitude)
{
    this->amplitude = amplitude;
    osc.SetAmp(amplitude);
}

void OscGrain::SetDuration(float duration)
{
    this->duration = duration * 1000;
}

void OscGrain::AdjustDuration()
{
    this->duration = (round(frequency * (duration / 1000000.0)) / frequency) * 1000000;
}