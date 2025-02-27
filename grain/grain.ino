#include <DaisyDuino.h>
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
    sineGrain = OscGrain(DAISY.get_samplerate());
    DAISY.begin(MyCallback);
}

void loop()
{
    sineGrain.Play(1000, 1.0f);
    delay(5000);
}
