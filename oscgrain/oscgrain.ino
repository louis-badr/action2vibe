#include <DaisyDuino.h>
#include <vector>

#include "OscGrain.h"

DaisyHardware hw;
OscGrain sineGrain;

void MyCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[0][i] = sineGrain.Process();
    }
}

void setup()
{
    Serial.begin();
    delay(5000);
    // Initialize for Daisy pod at 48kHz
    hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
    std::vector<float> frequencies = {170.0f};
    std::vector<float> amplitudes = {0.5f};
    sineGrain = OscGrain(DAISY.get_samplerate(), frequencies, amplitudes);
    DAISY.begin(MyCallback);
}

void loop()
{
    sineGrain.Play(50);
    delay(5000);
}
