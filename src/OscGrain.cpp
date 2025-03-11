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
    float out = 0.0f;
    if (isPlaying)
    {
        if (micros() - startTime > duration)
        {
            osc.SetAmp(0.0f);
            isPlaying = false;
        }
        out = osc.Process();
    }
    return out;
}

void OscGrain::Play()
{
    // only play grain if it's not already playing
    if (!isPlaying)
    {
        osc.Reset();
        osc.SetAmp(amplitude);
        isPlaying = true;
        startTime = micros();
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

void OscGrain::SetDuration(int duration)
{
    this->duration = duration * 1000;
}

void OscGrain::AdjustDuration()
{
    this->duration = (round(frequency * (duration / 1000000.0)) / frequency) * 1000000;
}