#include <DaisyDuino.h>
#include "WhiteNoiseGrain.h"

DaisyHardware hw;
WhiteNoiseGrain noiseGrain;

void MyCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[0][i] = noiseGrain.Process();
    }
}

void setup()
{
    Serial.begin();
    delay(1000);
    noiseGrain = WhiteNoiseGrain(0.5f, 20);
    // Initialize for Daisy pod at 48kHz
    hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
    DAISY.begin(MyCallback);
}

void loop()
{
    noiseGrain.Play();
    delay(3000);
}
