#include <DaisyDuino.h>
#include "OscGrain.h"

OscGrain::OscGrain() {}

OscGrain::OscGrain(float sample_rate, std::vector<float> &frequencies, std::vector<float> &amplitudes, int duration)
{
    this->amplitudes = amplitudes;
    this->duration = duration;
    for (float freq : frequencies)
    {
        Oscillator osc;
        osc.Init(sample_rate);
        osc.SetFreq(freq);
        osc.SetAmp(0.0f);
        osc.SetWaveform(osc.WAVE_SIN);
        oscs.push_back(osc);
    }
    isPlaying = false;
}

float OscGrain::Process()
{
    float out = 0.0f;
    if (isPlaying)
    {
        if (millis() - startTime > duration)
        {
            for (Oscillator &osc : oscs)
            {
                osc.SetAmp(0.0f);
            }
            isPlaying = false;
        }
        for (Oscillator &osc : oscs)
        {
            out += osc.Process();
        }
        out /= oscs.size();
    }
    return out;
}

void OscGrain::Play()
{
    // only play grain if it's not already playing
    if (!isPlaying)
    {
        for (int i = 0; i < oscs.size(); i++)
        {
            oscs[i].SetAmp(amplitudes[i]);
        }
        isPlaying = true;
        startTime = millis();
    }
}