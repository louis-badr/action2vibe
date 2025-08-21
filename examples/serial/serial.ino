#include <Action2Vibe.h>
#include <DaisyDuino.h>

#define BAUD_RATE 115200

typedef struct __attribute__((packed))
{
    uint8_t id;           // Receiver ID
    float frequency;   // Hz
    uint32_t duration_us; // microseconds
    float amplitude;      // 0.0 - 1.0
} VibeData;

DaisyHardware hw;
OscGrain grain;
Oscillator osc;
VibeData vibeData;

bool isPlayingOsc = false;

void AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (isPlayingOsc)
        {
            out[0][i] = osc.Process();
        }
        else
        {
            out[0][i] = grain.Process();
        }
    }
}

void setup(void)
{
    Serial.begin(BAUD_RATE);
    delay(100);
    Serial.println("Encore du travail ...");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K); // init Daisy seed at 48kHz
    delay(100);

    grain = OscGrain(DAISY.get_samplerate(), 80.0f, 1.0f, 12.5f);
    osc.Init(DAISY.get_samplerate());
    osc.SetFreq(80.0f);
    osc.SetAmp(0.0f);
    osc.SetWaveform(osc.WAVE_SIN);

    DAISY.begin(AudioCallback); // start audio callback

    digitalWrite(LED_BUILTIN, LOW);
    // play a tone for 1 second
    // play 10 grains at 80Hz, 1.0 amplitude, 12.5ms duration
    for (int i = 0; i < 10; i++)
    {
        grain.Play();
        delay(50); // wait for 100ms before playing the next grain
    }
}

void loop()
{
    while (Serial.available() >= sizeof(VibeData))
    {
        Serial.readBytes((char *)&vibeData, sizeof(VibeData));
        // sinewave if duration is zero
        if (vibeData.duration_us == 0)
        {
            // if amplitude is zero, stop playing
            if (vibeData.amplitude == 0)
            {
                isPlayingOsc = false;
            }
            else
            {
                osc.SetFreq(vibeData.frequency);
                osc.SetAmp(vibeData.amplitude);
                isPlayingOsc = true;
            }
        }
        grain.SetFrequency(vibeData.frequency);
        grain.SetDuration(vibeData.duration_us / 1000.0f);
        grain.SetAmplitude(vibeData.amplitude);
        grain.Play();
    }
}